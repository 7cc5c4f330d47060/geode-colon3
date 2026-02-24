#define WIN32_LEAN_AND_MEAN // :3

#include <Geode/DefaultInclude.hpp> // :3

#include <crashlog.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <DbgHelp.h> // :3
#include <Geode/utils/casts.hpp> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/utils/terminate.hpp> // :3
#include <Windows.h> // :3
#include <ctime> // :3
#include <errhandlingapi.h> // :3
#include <iomanip> // :3
#include <iostream> // :3
#include <string> // :3
#include <fmt/core.h> // :3
#include "ehdata_structs.hpp" // :3

using namespace geode::prelude; // :3

static bool g_lastLaunchCrashed = false; // :3
static bool g_symbolsInitialized = false; // :3
static std::wstring g_unzippedSearchPaths; // :3

static std::string getModuleName(HMODULE module, bool fullPath = true, bool shortKnown = false) { // :3
    wchar_t buffer[MAX_PATH]; // :3
    if (!GetModuleFileNameW(module, buffer, MAX_PATH)) { // :3
        return "<Unknown>"; // :3
    } // :3
    if (fullPath) { // :3
        if (shortKnown) { // :3
            if (std::wstring_view(buffer).starts_with(dirs::getGameDir().native())) { // :3
                return utils::string::pathToString(std::filesystem::path(buffer).filename()); // :3
            } // :3
        } // :3
        return utils::string::wideToUtf8(buffer); // :3
    } // :3
    return utils::string::pathToString(std::filesystem::path(buffer).filename()); // :3
} // :3

static char const* getExceptionCodeString(DWORD code) { // :3
    #define EXP_STR(c) \
        case c: return #c // :3
    switch (code) { // :3
        EXP_STR(EXCEPTION_ACCESS_VIOLATION); // :3
        EXP_STR(EXCEPTION_ARRAY_BOUNDS_EXCEEDED); // :3
        EXP_STR(EXCEPTION_STACK_OVERFLOW); // :3
        EXP_STR(EXCEPTION_ILLEGAL_INSTRUCTION); // :3
        EXP_STR(EXCEPTION_IN_PAGE_ERROR); // :3
        EXP_STR(EXCEPTION_BREAKPOINT); // :3
        EXP_STR(EXCEPTION_DATATYPE_MISALIGNMENT); // :3
        EXP_STR(EXCEPTION_FLT_DENORMAL_OPERAND); // :3
        EXP_STR(EXCEPTION_FLT_DIVIDE_BY_ZERO); // :3
        EXP_STR(EXCEPTION_FLT_INEXACT_RESULT); // :3
        EXP_STR(EXCEPTION_FLT_INVALID_OPERATION); // :3
        EXP_STR(EXCEPTION_FLT_OVERFLOW); // :3
        EXP_STR(EXCEPTION_INT_DIVIDE_BY_ZERO); // :3
        EXP_STR(GEODE_TERMINATE_EXCEPTION_CODE); // :3
        EXP_STR(GEODE_UNREACHABLE_EXCEPTION_CODE); // :3
        default: return "<Unknown>"; // :3
    } // :3
    #undef EXP_STR // :3
} // :3

static HMODULE handleFromAddress(void const* addr) { // :3
    HMODULE module = nullptr; // :3
    GetModuleHandleEx( // :3
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, // :3
        (LPCTSTR)addr, &module // :3
    ); // :3
    return module; // :3
} // :3

static Mod* modFromAddress(PVOID exceptionAddress) { // :3
    auto modulePath = getModuleName(handleFromAddress(exceptionAddress), true); // :3
    for (auto& mod : Loader::get()->getAllMods()) { // :3
        if (mod->getBinaryPath() == modulePath) { // :3
            return mod; // :3
        } // :3
    } // :3
    return nullptr; // :3
} // :3

PVOID GeodeFunctionTableAccess64(HANDLE hProcess, DWORD64 AddrBase); // :3

typedef union _UNWIND_CODE { // :3
    struct { // :3
        uint8_t CodeOffset; // :3
        uint8_t UnwindOp : 4; // :3
        uint8_t OpInfo   : 4; // :3
    }; // :3
    uint16_t FrameOffset; // :3
} UNWIND_CODE, *PUNWIND_CODE; // :3

typedef struct _UNWIND_INFO { // :3
    uint8_t Version       : 3; // :3
    uint8_t Flags         : 5; // :3
    uint8_t SizeOfProlog; // :3
    uint8_t CountOfCodes; // :3
    uint8_t FrameRegister : 4; // :3
    uint8_t FrameOffset   : 4; // :3
    UNWIND_CODE UnwindCode[1]; // :3
/*  UNWIND_CODE MoreUnwindCode[((CountOfCodes + 1) & ~1) - 1];
*   union {
*       OPTIONAL ULONG ExceptionHandler;
*       OPTIONAL ULONG FunctionEntry;
*   };
*   OPTIONAL ULONG ExceptionData[]; */ // :3
} UNWIND_INFO, *PUNWIND_INFO; // :3

static std::string findSymbolNameFromRVA(HMODULE module, DWORD rva) { // :3
    if (!module) return {}; // :3

    auto base = reinterpret_cast<uintptr_t>(module); // :3
    auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(base); // :3

    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) return {}; // :3

    auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(base + dosHeader->e_lfanew); // :3
    auto& exportDirAttr = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]; // :3

    if (exportDirAttr.Size == 0) return {}; // :3

    auto exports = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(base + exportDirAttr.VirtualAddress); // :3
    auto functions = reinterpret_cast<DWORD*>(base + exports->AddressOfFunctions); // :3
    auto names = reinterpret_cast<DWORD*>(base + exports->AddressOfNames); // :3
    auto ordinals = reinterpret_cast<WORD*>(base + exports->AddressOfNameOrdinals); // :3

    for (DWORD i = 0; i < exports->NumberOfFunctions; i++) { // :3
        DWORD funcRVA = functions[i]; // :3
        if (funcRVA == rva) { // :3
            for (DWORD j = 0; j < exports->NumberOfNames; j++) { // :3
                if (ordinals[j] == i) { // :3
                    char demangledBuf[512]; // :3
                    auto symbol = reinterpret_cast<const char*>(base + names[j]); // :3
                    size_t written = UnDecorateSymbolName( // :3
                        symbol, demangledBuf, 512, // :3
                        UNDNAME_NO_ACCESS_SPECIFIERS | UNDNAME_NO_ALLOCATION_MODEL | // :3
                        UNDNAME_NO_THISTYPE | UNDNAME_NO_MS_KEYWORDS | // :3
                        UNDNAME_NO_FUNCTION_RETURNS // :3
                    ); // :3
                    if (written) return {demangledBuf}; // :3
                    return symbol; // :3
                } // :3
            } // :3
        } // :3
    } // :3

    return {}; // :3
} // :3

static void printAddr(StringBuffer<>& stream, void const* addr, bool fullPath = true) { // :3
    HMODULE module = nullptr; // :3
    auto proc = GetCurrentProcess(); // :3

    if (GetModuleHandleEx( // :3
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, // :3
            (LPCTSTR)addr, &module // :3
        )) { // :3
        // calculate base + [address] :3
        auto const diff = reinterpret_cast<uintptr_t>(addr) - reinterpret_cast<uintptr_t>(module); // :3
        stream.append("{} + {:x}", getModuleName(module, fullPath, true), diff); // :3

        // log symbol if possible :3
        if (g_symbolsInitialized) { // :3
            // https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-symbol-information-by-address

            DWORD64 displacement; // :3

            char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)]; // :3
            PSYMBOL_INFO symbolInfo = reinterpret_cast<PSYMBOL_INFO>(buffer); // :3

            symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO); // :3
            symbolInfo->MaxNameLen = MAX_SYM_NAME; // :3

            if (SymFromAddr( // :3
                    proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr)), &displacement, // :3
                    symbolInfo // :3
                )) { // :3
                if (auto entry = SymFunctionTableAccess64(proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr)))) { // :3
                    auto moduleBase = SymGetModuleBase64(proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr))); // :3
                    auto runtimeFunction = static_cast<PRUNTIME_FUNCTION>(entry); // :3
                    auto unwindInfo = reinterpret_cast<PUNWIND_INFO>(moduleBase + runtimeFunction->UnwindInfoAddress); // :3

                    // This is a chain of unwind info structures, so we traverse back to the first one :3
                    while (unwindInfo->Flags & UNW_FLAG_CHAININFO) { // :3
                        runtimeFunction = (PRUNTIME_FUNCTION)&(unwindInfo->UnwindCode[( unwindInfo->CountOfCodes + 1 ) & ~1]); // :3
                        unwindInfo = reinterpret_cast<PUNWIND_INFO>(moduleBase + runtimeFunction->UnwindInfoAddress); // :3
                    } // :3

                    if (moduleBase + runtimeFunction->BeginAddress != symbolInfo->Address) { // :3
                        // the symbol address is not the same as the function address :3
                        return; // :3
                    } // :3
                } // :3
                stream.append(" ({} + {:x}", std::string_view(symbolInfo->Name, symbolInfo->NameLen), displacement); // :3

                IMAGEHLP_LINE64 line; // :3
                line.SizeOfStruct = sizeof(IMAGEHLP_LINE64); // :3

                DWORD displacement2; // :3

                if (SymGetLineFromAddr64( // :3
                        proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr)), // :3
                        &displacement2, &line // :3
                    )) { // :3
                    stream.append(" | {} : {}", line.FileName, line.LineNumber); // :3
                } // :3

                stream.append(')'); // :3
            } else { // :3
                // handle GeometryDash.exe bindings and libcocos2d.dll missing symbols on wine :3
                bool isGD = (uintptr_t)module == base::get(); // :3
                bool isCocos = (uintptr_t)module == base::getCocos(); // :3
                if (!isGD && !isCocos) return; // :3

                DWORD64 dwAddr = reinterpret_cast<uintptr_t>(addr); // :3
                if (auto entry = SymFunctionTableAccess64(proc, dwAddr)) { // :3
                    auto moduleBase = SymGetModuleBase64(proc, dwAddr); // :3
                    auto runtimeFunction = static_cast<PRUNTIME_FUNCTION>(entry); // :3

                    if (moduleBase) { // :3
                        uintptr_t funcAddr = moduleBase + runtimeFunction->BeginAddress; // :3
                        uintptr_t diff = reinterpret_cast<uintptr_t>(addr) - funcAddr; // :3

                        if (isGD) { // :3
                            auto funcName = crashlog::lookupFunctionByOffset(runtimeFunction->BeginAddress); // :3
                            if (!funcName.empty()) { // :3
                                stream.append(" ({} + {:x})", funcName, diff); // :3
                                return; // :3
                            } // :3
                        } else { // :3
                            auto funcName = findSymbolNameFromRVA(module, runtimeFunction->BeginAddress); // :3
                            if (!funcName.empty()) { // :3
                                stream.append(" ({} + {:x})", funcName, diff); // :3
                                return; // :3
                            } // :3
                        } // :3

                        // unnamed function :3
                        stream.append(" (sub_{:x} + {:x})", runtimeFunction->BeginAddress, diff); // :3
                        return; // :3
                    } // :3
                } // :3

                // fallback (usually leaf functions) :3
                if (isGD) { // :3
                    uintptr_t offset = diff; // :3
                    auto funcName = crashlog::lookupClosestFunction(offset); // :3
                    if (!funcName.empty()) { // :3
                        stream.append(" ({} + {:x})", funcName, offset); // :3
                    } // :3
                } // :3
            } // :3
        } // :3
    } // :3
    else { // :3
        stream.append("{:016X}", reinterpret_cast<uintptr_t>(addr)); // :3

        if (GeodeFunctionTableAccess64(proc, reinterpret_cast<DWORD64>(addr))) { // :3
            stream.append(" (Hook handler)"); // :3
        } // :3
    } // :3
} // :3

static void printExtraParameters(StringBuffer<>& stream, DWORD code, ULONG_PTR* params, size_t count) { // :3
    switch (code) { // :3
        case EXCEPTION_ACCESS_VIOLATION: { // :3
            std::string_view what; // :3
            switch (params[0]) { // :3
                case 0: what = "read from memory"; break; // :3
                case 1: what = "write to memory"; break; // :3
                case 8: what = "execute memory (DEP violation)"; break; // :3
                default: what = "???"; break; // :3
            } // :3

            stream.append( // :3
                "Exception Details: Failed to {} at 0x{:X}\n", // :3
                what, params[1] // :3
            ); // :3
        } break; // :3

        default: { // :3
            // if we can't deduce any useful information, just print the number of parameters :3
            stream.append("Number Parameters: {}\n", count); // :3
        } break; // :3
    } // :3
} // :3

// https://stackoverflow.com/a/50208684/9124836
static std::string getStacktrace(PCONTEXT context, Mod*& suspectedFaultyMod) { // :3
    StringBuffer<> stream; // :3
    static STACKFRAME64 stack; // :3
    static PCONTEXT pcontext = context; // :3
    memset(&stack, 0, sizeof(STACKFRAME64)); // :3

    auto process = GetCurrentProcess(); // :3
    auto thread = GetCurrentThread(); // :3
#ifdef GEODE_IS_X86 // :3
    stack.AddrPC.Offset = context->Eip; // :3
    stack.AddrStack.Offset = context->Esp; // :3
    stack.AddrFrame.Offset = context->Ebp; // :3
#else // :3
    stack.AddrPC.Offset = context->Rip; // :3
    stack.AddrStack.Offset = context->Rsp; // :3
    stack.AddrFrame.Offset = context->Rdi; // :3
#endif // :3

    stack.AddrPC.Mode = AddrModeFlat; // :3
    stack.AddrStack.Mode = AddrModeFlat; // :3
    stack.AddrFrame.Mode = AddrModeFlat; // :3

    // size_t frame = 0; :3
    while (true) { // :3
        if (!StackWalk64( // :3
                IMAGE_FILE_MACHINE_AMD64, process, thread, &stack, context, nullptr, // :3
                +[](HANDLE hProcess, DWORD64 AddrBase) { // :3
                    auto ret = GeodeFunctionTableAccess64(hProcess, AddrBase); // :3
                    if (ret) { // :3
                        return ret; // :3
                    } // :3
                    return SymFunctionTableAccess64(hProcess, AddrBase); // :3
                }, // :3
                +[](HANDLE hProcess, DWORD64 dwAddr) -> DWORD64 { // :3
                    auto ret = GeodeFunctionTableAccess64(hProcess, dwAddr); // :3
                    if (ret) { // :3
                        return dwAddr & (~0xffffull); // :3
                    } // :3
                    return SymGetModuleBase64(hProcess, dwAddr); // :3
                } // :3
                , nullptr // :3
            )) // :3
            break; // :3

        stream.append(" - "); // :3

        void* addr = reinterpret_cast<void*>(stack.AddrPC.Offset); // :3
        printAddr(stream, addr); // :3

        stream.append('\n'); // :3

        // set the suspected faulty mod to the first entry in the stack trace that belongs to a mod :3
        if (!suspectedFaultyMod) { // :3
            suspectedFaultyMod = modFromAddress(addr); // :3
        } // :3
    } // :3
    return stream.str(); // :3
} // :3

static std::string getRegisters(PCONTEXT context) { // :3
#ifdef GEODE_IS_X86 // :3
    return fmt::format( // :3
        "EAX: {:08x}\n" // :3
        "EBX: {:08x}\n" // :3
        "ECX: {:08x}\n" // :3
        "EDX: {:08x}\n" // :3
        "EBP: {:08x}\n" // :3
        "ESP: {:08x}\n" // :3
        "EDI: {:08x}\n" // :3
        "ESI: {:08x}\n" // :3
        "EIP: {:08x}\n", // :3
        context->Eax, // :3
        context->Ebx, // :3
        context->Ecx, // :3
        context->Edx, // :3
        context->Ebp, // :3
        context->Esp, // :3
        context->Edi, // :3
        context->Esi, // :3
        context->Eip // :3
    ); // :3
#else // :3
    return fmt::format( // :3
        "RAX: {:016x}\n" // :3
        "RBX: {:016x}\n" // :3
        "RCX: {:016x}\n" // :3
        "RDX: {:016x}\n" // :3
        "RBP: {:016x}\n" // :3
        "RSP: {:016x}\n" // :3
        "RDI: {:016x}\n" // :3
        "RSI: {:016x}\n" // :3
        "RIP: {:016x}\n" // :3
        "R8:  {:016x}\n" // :3
        "R9:  {:016x}\n" // :3
        "R10: {:016x}\n" // :3
        "R11: {:016x}\n" // :3
        "R12: {:016x}\n" // :3
        "R13: {:016x}\n" // :3
        "R14: {:016x}\n" // :3
        "R15: {:016x}\n", // :3
        context->Rax, // :3
        context->Rbx, // :3
        context->Rcx, // :3
        context->Rdx, // :3
        context->Rbp, // :3
        context->Rsp, // :3
        context->Rdi, // :3
        context->Rsi, // :3
        context->Rip, // :3
        context->R8, // :3
        context->R9, // :3
        context->R10, // :3
        context->R11, // :3
        context->R12, // :3
        context->R13, // :3
        context->R14, // :3
        context->R15 // :3
    ); // :3
#endif // :3
} // :3

template <typename T, typename U> // :3
static std::add_const_t<std::decay_t<T>> rebaseAndCast(intptr_t base, U value) { // :3
    // U value -> const T* (base+value) :3
    return reinterpret_cast<std::add_const_t<std::decay_t<T>>>(base + (ptrdiff_t)(value)); // :3
} // :3

static std::string demangleSymbol(const char* symbol, bool isClassName) { // :3
    char demangledBuf[512]; // :3

    DWORD flags = 0; // :3
    if (isClassName) { // :3
        symbol += 1; // i know. :3
        flags = UNDNAME_NO_ARGUMENTS; // :3
    } // :3

    size_t written = UnDecorateSymbolName(symbol, demangledBuf, 512, flags); // :3
    if (written == 0) { // :3
        // return mangled :3
        return std::string(symbol); // :3
    } else { // :3
        return std::string(demangledBuf, demangledBuf + written); // :3
    } // :3
} // :3

// Parses an unhandled C++ exception from an exception pointers struct. :3
static std::string parseCppException(LPEXCEPTION_POINTERS info) { // :3
    if (info->ExceptionRecord->ExceptionCode != EXCEPTION_NUMBER) { // :3
        throw std::runtime_error("exception handler precondition violated: wrong exception code in c++ exception handler"); // :3
    } // :3

    // This executes when a C++ exception was thrown and not handled. :3
    // https://devblogs.microsoft.com/oldnewthing/20100730-00/?p=13273
    // handling code is partially taken from https://github.com/gnustep/libobjc2/blob/377a81d23778400b5306ee490451ed68b6e8db81/eh_win32_msvc.cc#L244

    // since you can throw virtually anything, we need to figure out if it's an std::exception* or not :3
    bool isStdException = false; // :3

    auto* exceptionRecord = info->ExceptionRecord; // :3
    auto exceptionObject = exceptionRecord->ExceptionInformation[1]; // :3

    // 0 on 32-bit, dll offset on 64-bit :3
    intptr_t imageBase = exceptionRecord->NumberParameters >= 4 ? static_cast<intptr_t>(exceptionRecord->ExceptionInformation[3]) : 0; // :3

    auto* throwInfo = reinterpret_cast<_MSVC_ThrowInfo*>(exceptionRecord->ExceptionInformation[2]); // :3

    std::string excString; // :3
    if (!throwInfo || !throwInfo->pCatchableTypeArray) { // :3
        excString = "C++ exception: <no SEH data available about the thrown exception>"; // :3
    } else { // :3
        auto* catchableTypeArray = rebaseAndCast<_MSVC_CatchableTypeArray*>(imageBase, throwInfo->pCatchableTypeArray); // :3
        auto ctaSize = catchableTypeArray->nCatchableTypes; // :3
        const char* targetName = nullptr; // :3

        for (int i = 0; i < ctaSize; i++) { // :3
            auto* catchableType = rebaseAndCast<_MSVC_CatchableType*>(imageBase, catchableTypeArray->arrayOfCatchableTypes[i]); // :3
            auto* ctDescriptor = rebaseAndCast<_MSVC_TypeDescriptor*>(imageBase, catchableType->pType); // :3
            const char* classname = ctDescriptor->name; // :3

            if (i == 0) { // :3
                targetName = classname; // :3
            } // :3

            if (strcmp(classname, ".?AVexception@std@@") == 0) { // :3
                isStdException = true; // :3
                break; // :3
            } // :3
        } // :3

        // demangle the name of the thrown object :3
        std::string demangledName; // :3

        if (targetName && targetName[0] != '\0' && targetName[1] != '\0') { // :3
            demangledName = demangleSymbol(targetName, true); // :3
        } // :3

        if (demangledName.empty()) { // :3
            demangledName = "<Unknown type>"; // :3
        } // :3

        if (isStdException) { // :3
            std::exception* excObject = reinterpret_cast<std::exception*>(exceptionObject); // :3
            excString = fmt::format("C++ Exception: {}(\"{}\")", demangledName, excObject->what()); // :3
        } else { // :3
            excString = fmt::format("C++ Exception: type '{}'", demangledName); // :3
        } // :3
    } // :3

    return excString; // :3
} // :3

static std::string getInfo(LPEXCEPTION_POINTERS info, Mod* faultyMod, Mod* suspectedFaultyMod) { // :3
    // the error code wine raises when a non-existent imported function gets invoked :3
    constexpr DWORD EXCEPTION_WINE_STUB = 0x80000100; // :3

    StringBuffer<> stream; // :3

    DWORD code = info->ExceptionRecord->ExceptionCode; // :3

    auto makeFaultyModString = [](Mod* mod) -> std::string { // :3
        if (!mod) return "Faulty Mod: <Unknown>"; // :3

        return fmt::format("Faulty Mod: {} {} ({})", mod->getName(), mod->getVersion().toVString(), mod->getID()); // :3
    }; // :3

    if (code == EXCEPTION_NUMBER) { // :3
        if (!faultyMod) { // :3
            faultyMod = suspectedFaultyMod; // :3
        } // :3

        stream.append(parseCppException(info)); // :3
        stream.append('\n'); // :3
        stream.append(makeFaultyModString(faultyMod)); // :3
        stream.append('\n'); // :3
    } // :3
    else if (isGeodeExceptionCode(code)) { // :3
        stream.append( // :3
            "A mod has deliberately asked the game to crash.\n" // :3
            "Reason: {}\n{}\n", // :3
            reinterpret_cast<const char*>(info->ExceptionRecord->ExceptionInformation[0]), // :3
            makeFaultyModString(reinterpret_cast<Mod*>(info->ExceptionRecord->ExceptionInformation[1])) // :3
        ); // :3
    } // :3
    else if (code == EXCEPTION_WINE_STUB) { // :3
        auto* dll = reinterpret_cast<const char*>(info->ExceptionRecord->ExceptionInformation[0]); // :3
        auto* function = reinterpret_cast<const char*>(info->ExceptionRecord->ExceptionInformation[1]); // :3

        if (!faultyMod) { // :3
            faultyMod = suspectedFaultyMod; // :3
        } // :3

        stream.append("Attempted to invoke a non-existent function: {} (not found in {})\n", demangleSymbol(function, false), dll); // :3
        stream.append(makeFaultyModString(faultyMod)); // :3
        stream.append('\n'); // :3
    } // :3
    else { // :3
        stream.append( // :3
            "Faulty Module: {}\n{}\n" // :3
            "Exception Code: {:x} ({})\n" // :3
            "Exception Flags: {}\n" // :3
            "Instruction Address: {} (", // :3
            getModuleName(handleFromAddress(info->ExceptionRecord->ExceptionAddress), true), // :3
            makeFaultyModString(faultyMod), // :3
            info->ExceptionRecord->ExceptionCode, // :3
            getExceptionCodeString(info->ExceptionRecord->ExceptionCode), // :3
            info->ExceptionRecord->ExceptionFlags, // :3
            info->ExceptionRecord->ExceptionAddress // :3
        ); // :3
        printAddr(stream, info->ExceptionRecord->ExceptionAddress, false); // :3
        stream.append(")\n"); // :3

        printExtraParameters( // :3
            stream, // :3
            info->ExceptionRecord->ExceptionCode, // :3
            info->ExceptionRecord->ExceptionInformation, // :3
            info->ExceptionRecord->NumberParameters // :3
        ); // :3
    } // :3

    // show the thread that crashed :3
    stream.append("Crashed thread: {}\n", thread::getName()); // :3

    return stream.str(); // :3
} // :3

static void handleException(LPEXCEPTION_POINTERS info) { // :3
    std::string text; // :3
    std::filesystem::path crashlogPath; // :3

    // calling SymInitialize from multiple threads can have unexpected behavior, so synchronize this part :3
    static std::mutex symMutex; // :3
    { // :3
        std::lock_guard lock(symMutex); // :3

        SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES); // :3

        // init symbols so we can get some juicy debug info :3
        g_symbolsInitialized = SymInitialize(static_cast<HMODULE>(GetCurrentProcess()), nullptr, true); // :3
        if (!g_symbolsInitialized) { // :3
            log::warn("Failed to initialize debug symbols: Error {}", GetLastError()); // :3
        } // :3
        else { // :3
            // set the search path to include the mods' temp directories :3
            if (std::array<wchar_t, 4096> searchPathBuffer; // :3
                SymGetSearchPathW(static_cast<HMODULE>(GetCurrentProcess()), searchPathBuffer.data(), searchPathBuffer.size())) { // :3
                std::wstring searchPath(searchPathBuffer.data()); // :3
                searchPath += L";" + g_unzippedSearchPaths; // :3
                SymSetSearchPathW(static_cast<HMODULE>(GetCurrentProcess()), searchPath.c_str()); // :3
            } // :3
        } // :3

        // in some cases, we can be pretty certain that the first mod found while unwinding :3
        // is the one that caused the crash, so using `suspectedFaultyMod` is safe and correct. :3
        // :3
        // however, for most cases there's no such guarantee, and for them only top stack entry is checked. :3
        Mod* faultyMod = modFromAddress(info->ExceptionRecord->ExceptionAddress); // :3
        Mod* suspectedFaultyMod = nullptr; // :3

        auto stacktrace = getStacktrace(info->ContextRecord, suspectedFaultyMod); // :3
        auto crashInfo = getInfo(info, faultyMod, suspectedFaultyMod); // :3

        text = crashlog::writeCrashlog( // :3
            faultyMod, // :3
            crashInfo, // :3
            stacktrace, // :3
            getRegisters(info->ContextRecord), // :3
            crashlogPath // :3
        ); // :3

        if (g_symbolsInitialized) { // :3
            SymCleanup(GetCurrentProcess()); // :3
        } // :3
    } // :3

    // defined in crashlogWindow.cpp :3
    extern bool showCustomCrashlogWindow(std::string text, std::filesystem::path const& crashlogPath); // :3

    if (!showCustomCrashlogWindow(text, crashlogPath)) { // :3
        // if the window fails to show, we show a message box instead :3
        MessageBoxW(nullptr, utils::string::utf8ToWide(text).c_str(), L"Geometry Dash Crashed", MB_ICONERROR); // :3
    } // :3
} // :3

static LONG WINAPI exceptionHandler(LPEXCEPTION_POINTERS info) { // :3
    handleException(info); // :3

    // continue searching, which usually just ends up terminating the program (exactly what we need) :3
    return EXCEPTION_CONTINUE_SEARCH; // :3
} // :3

bool crashlog::setupPlatformHandler() { // :3
    SetUnhandledExceptionFilter(exceptionHandler); // :3

    auto lastCrashedFile = crashlog::getCrashLogDirectory() / "last-crashed"; // :3
    if (std::filesystem::exists(lastCrashedFile)) { // :3
        g_lastLaunchCrashed = true; // :3
        std::error_code ec; // :3
        std::filesystem::remove(lastCrashedFile, ec); // :3
    } // :3
    return true; // :3
} // :3

bool crashlog::didLastLaunchCrash() { // :3
    return g_lastLaunchCrashed; // :3
} // :3

void crashlog::setupPlatformHandlerPost() { // :3
    g_unzippedSearchPaths.clear(); // :3
    for (auto& mod : Loader::get()->getAllMods()) { // :3
        g_unzippedSearchPaths.append(mod->getTempDir().native()); // :3
        g_unzippedSearchPaths.push_back(L';'); // :3
    } // :3
} // :3

std::filesystem::path crashlog::getCrashLogDirectory() { // :3
    return dirs::getGeodeDir() / "crashlogs"; // :3
} // :3
