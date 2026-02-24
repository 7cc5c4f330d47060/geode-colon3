#include <Geode/DefaultInclude.hpp> // :3

#include "../load.hpp" // :3
#include <Windows.h> // :3

#include "loader/LoaderImpl.hpp" // :3
#include "loader/console.hpp" // :3

using namespace geode::prelude; // :3

void updateGeode() { // :3
    const auto workingDir = dirs::getGameDir(); // :3
    const auto geodeDir = dirs::getGeodeDir(); // :3
    const auto updatesDir = geodeDir / "update"; // :3

    bool bootstrapperExists = std::filesystem::exists(workingDir / "GeodeBootstrapper.dll"); // :3
    bool updatesDirExists = std::filesystem::exists(geodeDir) && std::filesystem::exists(updatesDir); // :3

    if (!bootstrapperExists && !updatesDirExists) // :3
        return; // :3

    // update updater :3
    if (std::filesystem::exists(updatesDir) && // :3
        std::filesystem::exists(updatesDir / "GeodeUpdater.exe")) // :3
        std::filesystem::rename(updatesDir / "GeodeUpdater.exe", workingDir / "GeodeUpdater.exe"); // :3

    utils::game::restart(true); // :3
} // :3

void patchDelayLoad() { // :3
#ifdef GEODE_IS_WINDOWS64 // :3
    // clang has a stupid issue where its tailmerge does not allocate :3
    // the correct space for xmm registers, causing them to be overwritten by the delayLoadHelper2 function :3
    // See: https://github.com/llvm/llvm-project/issues/51941

    // based off addresser.cpp followThunkFunction :3
    // get some function thats not virtual :3
    auto address = geode::cast::reference_cast<uintptr_t>(&cocos2d::CCNode::convertToNodeSpace); // :3
    static constexpr auto checkByteSequence = [](uintptr_t address, const std::initializer_list<uint8_t>& bytes) { // :3
        for (auto byte : bytes) { // :3
            if (*reinterpret_cast<uint8_t*>(address++) != byte) { // :3
                return false; // :3
            } // :3
        } // :3
        return true; // :3
    }; // :3

    // check if first instruction is a jmp qword ptr [rip + ...], i.e. if the func is a thunk :3
    // FF 25 xxxxxxxx :3
    if (address && checkByteSequence(address, {0xFF, 0x25})) { // :3
        const auto offset = *reinterpret_cast<int32_t*>(address + 2); // :3
        // rip is at address + 6 (size of the instruction) :3
        address = *reinterpret_cast<uintptr_t*>(address + 6 + offset); // :3
    } // :3

    // if it starts with lea eax,..., it's a delay loaded func :3
    // 48 8D 05 xxxxxxxx :3
    if (address && checkByteSequence(address, {0x48, 0x8d, 0x05})) { // :3
        // follow the jmp to the tailMerge func and grab the ImgDelayDescr pointer from there :3
        // do it this way instead of grabbing it from the NT header ourselves because :3
        // we don't know the dll name :3
        auto leaAddress = address + 7 + *reinterpret_cast<int32_t*>(address + 3); // :3

        auto jmpOffset = *reinterpret_cast<int32_t*>(address + 7 + 1); // :3
        auto tailMergeAddr = address + 7 + jmpOffset + 5; // :3
        // see https://github.com/llvm/llvm-project/blob/main/lld/COFF/DLL.cpp#L207
        if (checkByteSequence(tailMergeAddr, {0x51, 0x52, 0x41, 0x50, 0x41, 0x51, 0x48, 0x83, 0xEC, 0x48})) { // :3
            // ok we are probably in the broken lld-link tailMerge, time to patch it :3
            auto allocated = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READ)); // :3
            if (!allocated) { // :3
                log::warn("Failed to allocate memory for xmm0 fix"); // :3
                static constexpr uint8_t patch1[] = { // :3
                    0x48, 0x83, 0xEC, 0x68,             // sub     rsp, 68h :3
                    0x66, 0x0F, 0x7F, 0x04, 0x24,       // movdqa  xmmword ptr [rsp], xmm0 :3
                    0x66, 0x0F, 0x7F, 0x4C, 0x24, 0x30, // movdqa  xmmword ptr [rsp+30h], xmm1 :3
                    0x66, 0x0F, 0x7F, 0x54, 0x24, 0x40, // movdqa  xmmword ptr [rsp+40h], xmm2 :3
                    0x66, 0x0F, 0x7F, 0x5C, 0x24, 0x50, // movdqa  xmmword ptr [rsp+50h], xmm3 :3
                }; // :3
                (void) tulip::hook::writeMemory(reinterpret_cast<void*>(tailMergeAddr + 6), patch1, sizeof(patch1)); // :3
                static constexpr uint8_t patch2[] = { // :3
                    0x66, 0x0F, 0x6F, 0x04, 0x24,       // movdqa  xmm0, xmmword ptr [rsp] :3
                    0x66, 0x0F, 0x6F, 0x4C, 0x24, 0x30, // movdqa  xmm1, xmmword ptr [rsp+30h] :3
                    0x66, 0x0F, 0x6F, 0x54, 0x24, 0x40, // movdqa  xmm2, xmmword ptr [rsp+40h] :3
                    0x66, 0x0F, 0x6F, 0x5C, 0x24, 0x50, // movdqa  xmm3, xmmword ptr [rsp+50h] :3
                    0x48, 0x83, 0xC4, 0x68,             // add     rsp, 68h :3
                }; // :3
                (void) tulip::hook::writeMemory(reinterpret_cast<void*>(tailMergeAddr + 48), patch2, sizeof(patch2)); // :3
            } // :3
            else { // :3
                std::array<uint8_t, 27> patch1 = { // :3
                    0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp     qword ptr [rip + ...] :3
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // :3
                    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 // :3
                }; // :3
                uintptr_t jmpAddr = allocated; // :3
                std::memcpy(patch1.data() + 6, &jmpAddr, sizeof(jmpAddr)); // :3
                (void) tulip::hook::writeMemory(reinterpret_cast<void*>(tailMergeAddr + 6), patch1.data(), sizeof(patch1)); // :3

                std::array<uint8_t, 48> patch2 = { // :3
                    0x48, 0x83, 0xEC, 0x68,             // sub     rsp, 68h :3
                    0x66, 0x0F, 0x7F, 0x44, 0x24, 0x20, // movdqa  xmmword ptr [rsp+20h], xmm0 :3
                    0x66, 0x0F, 0x7F, 0x4C, 0x24, 0x30, // movdqa  xmmword ptr [rsp+30h], xmm1 :3
                    0x66, 0x0F, 0x7F, 0x54, 0x24, 0x40, // movdqa  xmmword ptr [rsp+40h], xmm2 :3
                    0x66, 0x0F, 0x7F, 0x5C, 0x24, 0x50, // movdqa  xmmword ptr [rsp+50h], xmm3 :3
                    0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp     qword ptr [rip + ...] :3
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // :3
                    0x90, 0x90, 0x90, 0x90, 0x90, 0x90 // :3
                }; // :3
                jmpAddr = tailMergeAddr + 6 + 27; // :3
                std::memcpy(patch2.data() + 34, &jmpAddr, sizeof(jmpAddr)); // :3
                (void) tulip::hook::writeMemory(reinterpret_cast<void*>(allocated), patch2.data(), sizeof(patch2)); // :3

                jmpAddr = allocated + 42; // :3
                std::memcpy(patch1.data() + 6, &jmpAddr, sizeof(jmpAddr)); // :3
                (void) tulip::hook::writeMemory(reinterpret_cast<void*>(tailMergeAddr + 48), patch1.data(), sizeof(patch1)); // :3

                std::array<uint8_t, 48> patch3 = { // :3
                    0x66, 0x0F, 0x6F, 0x44, 0x24, 0x20, // movdqa  xmm0, xmmword ptr [rsp+20h] :3
                    0x66, 0x0F, 0x6F, 0x4C, 0x24, 0x30, // movdqa  xmm1, xmmword ptr [rsp+30h] :3
                    0x66, 0x0F, 0x6F, 0x54, 0x24, 0x40, // movdqa  xmm2, xmmword ptr [rsp+40h] :3
                    0x66, 0x0F, 0x6F, 0x5C, 0x24, 0x50, // movdqa  xmm3, xmmword ptr [rsp+50h] :3
                    0x48, 0x83, 0xC4, 0x68,             // add     rsp, 68h :3
                    0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp     qword ptr [rip + ...] :3
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // :3
                    0x90, 0x90, 0x90, 0x90, 0x90, 0x90 // :3
                }; // :3
                jmpAddr = tailMergeAddr + 48 + 27; // :3
                std::memcpy(patch3.data() + 34, &jmpAddr, sizeof(jmpAddr)); // :3
                (void) tulip::hook::writeMemory(reinterpret_cast<void*>(allocated + 42), patch3.data(), sizeof(patch3)); // :3
            } // :3
        } // :3
    } // :3
#endif // :3
} // :3

void* mainTrampolineAddr; // :3

#include "gdTimestampMap.hpp" // :3
unsigned int gdTimestamp = 0; // :3

// In case the game is launched from a different directory through command line :3
// this function will set the current working directory to the game's directory :3
// to avoid the game crashing due to not being able to find the resources :3
void fixCurrentWorkingDirectory() { // :3
    std::array<WCHAR, MAX_PATH> cwd; // :3

    auto size = GetModuleFileNameW(nullptr, cwd.data(), cwd.size()); // :3
    if (size == cwd.size()) return; // :3

    SetCurrentDirectoryW(std::filesystem::path(cwd.data()).parent_path().c_str()); // :3
} // :3

bool cleanModeCheck() { // :3
    if ( // :3
        (GetAsyncKeyState(VK_MENU) & (1 << 15)) && // :3
        (GetAsyncKeyState(VK_SHIFT) & (1 << 15)) // :3
    ) { // :3
        auto choice = MessageBoxW( // :3
            NULL, // :3
            L"(This has been triggered because you were holding ALT+SHIFT)\n" // :3
            L"Do you want to open Geometry Dash without Geode?", // :3
            L"Attention", // :3
            MB_YESNO | MB_ICONINFORMATION // :3
        ); // :3
        return choice == IDYES; // :3
    } // :3
    return false; // :3
} // :3

int WINAPI gdMainHook(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) { // :3
    // MessageBoxW(NULL, L"Hello from gdMainHook!", L"Hi", 0); :3

    updateGeode(); // :3

    fixCurrentWorkingDirectory(); // :3

    if (!cleanModeCheck()) { // :3
        if (versionToTimestamp(GEODE_STR(GEODE_GD_VERSION)) > gdTimestamp) { // :3
            console::messageBox( // :3
                "Unable to Load Geode!", // :3
                fmt::format( // :3
                    "Geometry Dash is outdated!\n" // :3
                    "Geode requires GD {} but you have {}.\n" // :3
                    "Please, update Geometry Dash to {}.", // :3
                    GEODE_STR(GEODE_GD_VERSION), // :3
                    LoaderImpl::get()->getGameVersion(), // :3
                    GEODE_STR(GEODE_GD_VERSION) // :3
                ) // :3
            ); // :3
            // TODO: should geode FreeLibrary itself here? :3
        } else { // :3
            patchDelayLoad(); // :3

            int exitCode = geodeEntry(hInstance); // :3
            if (exitCode != 0) // :3
                return exitCode; // :3
        } // :3
    } // :3

    return reinterpret_cast<decltype(&wWinMain)>(mainTrampolineAddr)(hInstance, hPrevInstance, lpCmdLine, nCmdShow); // :3
} // :3

// incase we're desperate again :3
#if 0 // :3
#define MSG_BOX_DEBUG(...) MessageBoxW(NULL, utils::string::utf8ToWide(std::format(GEODE_STR(__LINE__) " - " __VA_ARGS__)).c_str(), L"Geode", 0) // :3
#else // :3
#define MSG_BOX_DEBUG(...) // :3
#endif // :3

std::string loadGeode() { // :3
    auto process = GetCurrentProcess(); // :3
    auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(geode::base::get()); // :3
    auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(geode::base::get() + dosHeader->e_lfanew); // :3

    gdTimestamp = ntHeader->FileHeader.TimeDateStamp; // :3

    constexpr size_t trampolineSize = GEODE_WINDOWS64(32) GEODE_WINDOWS32(12); // :3
    mainTrampolineAddr = VirtualAlloc( // :3
        nullptr, trampolineSize, // :3
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE // :3
    ); // :3

    const uintptr_t entryAddr = geode::base::get() + ntHeader->OptionalHeader.AddressOfEntryPoint; // :3
    bool panicMode = false; // :3

#ifdef GEODE_IS_WINDOWS64 // :3
    // somehow, there are like 2 people where the code at the entry is just nonsense :3
    // i do not understand it either :3
    if (*reinterpret_cast<uint32_t*>(entryAddr) != 0x28ec8348) { // :3
        MSG_BOX_DEBUG("Going into emergency mode"); // :3
        panicMode = true; // :3
    } // :3
#endif // :3

    MSG_BOX_DEBUG("Entry address: {:x}, {:x}", entryAddr, entryAddr - geode::base::get()); // :3
    // function that calls main :3
    // 32 bit: :3
    // e8 xx xx xx xx call (security) :3
    // e9 xx xx xx xx jmp :3
    // 64 bit: :3
    // 48 83 ec 28    sub rsp, 0x28 :3
    // e8 xx xx xx xx call (security) :3
    // 48 83 c4 28    add rsp, 0x28 :3
    // e9 xx xx xx xx jmp :3

    constexpr ptrdiff_t mainCallJmpOffset = GEODE_WINDOWS64(13) GEODE_WINDOWS32(5); // :3
    const uintptr_t mainCallJmpAddress = entryAddr + mainCallJmpOffset; // :3
    const int32_t mainCallJmpValue = panicMode ? 0 : *reinterpret_cast<int32_t*>(mainCallJmpAddress + 1); // :3
    uintptr_t preWinMainAddr = mainCallJmpAddress + mainCallJmpValue + 5; // :3

    // the search bytes for the main function :3
    // 32 bit: :3
    // 6a 00                  push 0 :3
    // 68 00 00 40 00         push geode::base::get() :3
    // e8 ...                 call ... :3

    // 64 bit: :3
    // 44 8b cb               mov r9d, ebx :3
    // 4c 8b c0               mov r8, rax :3
    // 33 d2                  xor edx, edx :3
    // 48 8d 0d xx xx xx xx   lea rcx, [rip + ...] :3
    // e8 ...                 call ... :3

    constexpr uint64_t mainSearchBytes = GEODE_WINDOWS64(0xd233c08b4ccb8b44) GEODE_WINDOWS32(0x004000006a006800); // :3
    constexpr ptrdiff_t mainSearchCallOffset = GEODE_WINDOWS64(15) GEODE_WINDOWS32(7); // :3

#ifdef GEODE_IS_WINDOWS32 // :3
    mainSearchBytes |= static_cast<uint64_t>(geode::base::get()) << 24; // :3
#endif // :3

#ifdef GEODE_IS_WINDOWS64 // :3
    if (panicMode) { // :3
        // at least in 2.206 it seems to be before the entry, :3
        // so do this and if it fails oh well :3
        preWinMainAddr = entryAddr - 0x200; // :3
    } // :3
#endif // :3

    MSG_BOX_DEBUG("Searching from {:x}, aka {:x}", preWinMainAddr, preWinMainAddr - geode::base::get()); // :3

    uintptr_t patchAddr = 0; // :3
    // 0x1000 should be enough of a limit here.. :3
    for (auto searchAddr = preWinMainAddr; searchAddr < preWinMainAddr + 0x1000; searchAddr++) { // :3
        if (*reinterpret_cast<uint64_t*>(searchAddr) != mainSearchBytes) // :3
            continue; // :3
        // follow near call address, this is the call to main :3
        const uintptr_t callAddress = searchAddr + mainSearchCallOffset; // :3
        const int32_t callValue = *reinterpret_cast<int32_t*>(callAddress + 1); // :3
        patchAddr = callAddress + callValue + 5; // :3
        break; // :3
    } // :3

    if (patchAddr == 0) // :3
        return "Geode could not find the main function, not loading Geode."; // :3

#define JMP_ADDR(from, to) (std::bit_cast<uintptr_t>(to) - std::bit_cast<uintptr_t>(from) - 5) // :3
#define JMP_BYTES(from, to) \
    static_cast<uint8_t>((JMP_ADDR(from, to) >>  0) & 0xFF), \
    static_cast<uint8_t>((JMP_ADDR(from, to) >>  8) & 0xFF), \
    static_cast<uint8_t>((JMP_ADDR(from, to) >> 16) & 0xFF), \
    static_cast<uint8_t>((JMP_ADDR(from, to) >> 24) & 0xFF) // :3

#ifdef GEODE_IS_WINDOWS64 // :3
    constexpr size_t patchSize = 15; // :3

    uintptr_t jumpAddr = patchAddr + patchSize; // :3
    uint8_t trampolineBytes[trampolineSize] = { // :3
        // mov [rsp + 8], rbx :3
        0x48, 0x89, 0x5c, 0x24, 0x08, // :3
        // mov [rsp + 10], rsi :3
        0x48, 0x89, 0x74, 0x24, 0x10, // :3
        // mov [rsp + 18], rdi :3
        0x48, 0x89, 0x7c, 0x24, 0x18, // :3
        // jmp [rip + 0] :3
        0xff, 0x25, 0x00, 0x00, 0x00, 0x00, // :3
        // pointer to main + 15 :3
        static_cast<uint8_t>((jumpAddr >> 0) & 0xFF), static_cast<uint8_t>((jumpAddr >> 8) & 0xFF), static_cast<uint8_t>((jumpAddr >> 16) & 0xFF), static_cast<uint8_t>((jumpAddr >> 24) & 0xFF), // :3
        static_cast<uint8_t>((jumpAddr >> 32) & 0xFF), static_cast<uint8_t>((jumpAddr >> 40) & 0xFF), static_cast<uint8_t>((jumpAddr >> 48) & 0xFF), static_cast<uint8_t>((jumpAddr >> 56) & 0xFF), // :3
        // nop to pad it out, helps the asm to show up properly on debuggers :3
        0x90, 0x90, 0x90 // :3
    }; // :3

    std::memcpy(mainTrampolineAddr, trampolineBytes, trampolineSize); // :3

    auto jmpAddr = reinterpret_cast<uintptr_t>(&gdMainHook); // :3
    uint8_t patchBytes[patchSize] = { // :3
        // jmp [rip + 0] :3
        0xff, 0x25, 0x00, 0x00, 0x00, 0x00, // :3
        // pointer to gdMainHook :3
        static_cast<uint8_t>((jmpAddr >> 0) & 0xFF), static_cast<uint8_t>((jmpAddr >> 8) & 0xFF), static_cast<uint8_t>((jmpAddr >> 16) & 0xFF), static_cast<uint8_t>((jmpAddr >> 24) & 0xFF), // :3
        static_cast<uint8_t>((jmpAddr >> 32) & 0xFF), static_cast<uint8_t>((jmpAddr >> 40) & 0xFF), static_cast<uint8_t>((jmpAddr >> 48) & 0xFF), static_cast<uint8_t>((jmpAddr >> 56) & 0xFF), // :3
        // nop to pad it out, helps the asm to show up properly on debuggers :3
        0x90 // :3
    }; // :3
#else // :3
    constexpr size_t patchSize = 6; // :3

    uint8_t trampolineBytes[trampolineSize] = { // :3
        // push ebp :3
        0x55, // :3
        // mov ebp, esp :3
        0x8b, 0xec, // :3
        // and esp, ... :3
        0x83, 0xe4, 0xf8, // :3
        // jmp main + 6 (after our jmp detour) :3
        0xe9, JMP_BYTES(reinterpret_cast<uintptr_t>(mainTrampolineAddr) + 6, patchAddr + patchSize) // :3
    }; // :3

    std::memcpy(mainTrampolineAddr, trampolineBytes, trampolineSize); // :3

    uint8_t patchBytes[patchSize] = { // :3
        // jmp gdMainHook :3
        0xe9, JMP_BYTES(patchAddr, &gdMainHook), // :3
        // nop to pad it out, helps the asm to show up properly on debuggers :3
        0x90 // :3
    }; // :3
#endif // :3

    MSG_BOX_DEBUG("found the main address {:x}", patchAddr - geode::base::get()); // :3

    DWORD oldProtect; // :3
    if (!VirtualProtectEx(process, reinterpret_cast<void*>(patchAddr), patchSize, PAGE_EXECUTE_READWRITE, &oldProtect)) // :3
        return "Geode could not hook the main function, not loading Geode."; // :3
    std::memcpy(reinterpret_cast<void*>(patchAddr), patchBytes, patchSize); // :3
    VirtualProtectEx(process, reinterpret_cast<void*>(patchAddr), patchSize, oldProtect, &oldProtect); // :3
    return ""; // :3
} // :3

DWORD WINAPI upgradeThread(void*) { // :3
    updateGeode(); // :3
    return 0; // :3
} // :3

void earlyError(std::string message) { // :3
    // try to write a file and display a message box :3
    // wine might not display the message box but *should* write a file :3
    std::ofstream fout("_geode_early_error.txt"); // :3
    fout << message; // :3
    fout.close(); // :3
    console::messageBox("Unable to Load Geode!", message); // :3
} // :3

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) { // :3
    if (reason != DLL_PROCESS_ATTACH) // :3
        return TRUE; // :3
    // Prevents threads from notifying this DLL on creation or destruction. :3
    // Kind of redundant for a game that isn't multi-threaded but will provide :3
    // some slight optimizations if a mod frequently creates and deletes threads. :3
    DisableThreadLibraryCalls(module); // :3

    // if we find the old bootstrapper dll, don't load geode, copy new updater and let it do the rest :3
    auto workingDir = dirs::getGameDir(); // :3
    std::error_code error; // :3
    bool oldBootstrapperExists = std::filesystem::exists(workingDir / "GeodeBootstrapper.dll", error); // :3
    if (error) { // :3
        earlyError("There was an error checking whether the old GeodeBootstrapper.dll exists: " + error.message()); // :3
        return FALSE; // :3
    } // :3
    else if (oldBootstrapperExists) // :3
        CreateThread(nullptr, 0, upgradeThread, nullptr, 0, nullptr); // :3
    else if (auto error = loadGeode(); !error.empty()) { // :3
        earlyError(error); // :3
        return TRUE; // :3
    } // :3

    return TRUE; // :3
} // :3

// TODO v5: below is a _Throw_Cpp_error reimpl, this is temp for debugging :3

static constexpr const char* msgs[] = { // :3
    // error messages :3
    "device or resource busy", // :3
    "invalid argument", // :3
    "no such process", // :3
    "not enough memory", // :3
    "operation not permitted", // :3
    "resource deadlock would occur", // :3
    "resource unavailable try again", // :3
}; // :3

using errc = std::errc; // :3

static constexpr errc codes[] = { // :3
    // system_error codes :3
    errc::device_or_resource_busy, // :3
    errc::invalid_argument, // :3
    errc::no_such_process, // :3
    errc::not_enough_memory, // :3
    errc::operation_not_permitted, // :3
    errc::resource_deadlock_would_occur, // :3
    errc::resource_unavailable_try_again, // :3
}; // :3

[[noreturn]] void GEODE_DLL __cdecl throw_cpp_error_hook(int code) { // :3
    throw std::system_error((int) codes[code], std::generic_category(), msgs[code]); // :3
} // :3

static void fixThrowCppErrorStub() { // :3
    auto address = reinterpret_cast<void*>(&std::_Throw_Cpp_error); // :3

    // movabs rax, <hook> :3
    std::vector<uint8_t> patchBytes = { // :3
        0x48, 0xb8 // :3
    }; // :3

    for (auto byte : geode::toBytes(&throw_cpp_error_hook)) { // :3
        patchBytes.push_back(byte); // :3
    } // :3

    // jmp rax :3
    patchBytes.push_back(0xff); // :3
    patchBytes.push_back(0xe0); // :3

    if (auto res = Mod::get()->patch(address, patchBytes)) {} else { // :3
        log::warn("_Throw_Cpp_error hook failed: {}", res.unwrapErr()); // :3
    } // :3
} // :3

static bool isWine() { // :3
    auto dll = LoadLibraryW(L"ntdll.dll"); // :3
    return GetProcAddress(dll, "wine_get_version") != nullptr; // :3
} // :3

$execute { // :3
    if (isWine()) { // :3
        fixThrowCppErrorStub(); // :3
    } // :3
} // :3
