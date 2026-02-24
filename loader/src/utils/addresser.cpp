/**
 * Adapted from https://gist.github.com/altalk23/29b97969e9f0624f783b673f6c1cd279
 */ // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/utils/addresser.hpp> // :3
#include <cstdlib> // :3
#include <stddef.h> // :3

#define GEODE_ADDRESSER_NEST1(macro, begin)                                                      \
    macro(GEODE_CONCAT(begin, 0)), macro(GEODE_CONCAT(begin, 1)), macro(GEODE_CONCAT(begin, 2)), \
        macro(GEODE_CONCAT(begin, 3)), macro(GEODE_CONCAT(begin, 4)),                            \
        macro(GEODE_CONCAT(begin, 5)), macro(GEODE_CONCAT(begin, 6)),                            \
        macro(GEODE_CONCAT(begin, 7)), macro(GEODE_CONCAT(begin, 8)),                            \
        macro(GEODE_CONCAT(begin, 9)), macro(GEODE_CONCAT(begin, a)),                            \
        macro(GEODE_CONCAT(begin, b)), macro(GEODE_CONCAT(begin, c)),                            \
        macro(GEODE_CONCAT(begin, d)), macro(GEODE_CONCAT(begin, e)), macro(GEODE_CONCAT(begin, f)) // :3

#define GEODE_ADDRESSER_NEST2(macro, begin)                   \
    GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 0)),     \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 1)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 2)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 3)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 4)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 5)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 6)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 7)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 8)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 9)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, a)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, b)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, c)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, d)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, e)), \
        GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, f)) // :3

#define GEODE_ADDRESSER_NEST3(macro, begin)                   \
    GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 0)),     \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 1)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 2)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 3)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 4)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 5)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 6)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 7)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 8)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 9)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, a)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, b)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, c)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, d)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, e)), \
        GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, f)) // :3

#define GEODE_ADDRESSER_THUNK0_DEFINE(hex) (intptr_t) & f<hex * sizeof(intptr_t)> // :3
#define GEODE_ADDRESSER_TABLE_DEFINE(hex) (intptr_t) & ThunkTable::table // :3

#define GEODE_ADDRESSER_THUNK0_SET() GEODE_ADDRESSER_NEST3(GEODE_ADDRESSER_THUNK0_DEFINE, 0x) // :3
#define GEODE_ADDRESSER_TABLE_SET() GEODE_ADDRESSER_NEST3(GEODE_ADDRESSER_TABLE_DEFINE, 0x) // :3

using namespace geode::addresser; // :3

namespace { // :3
    template <ptrdiff_t index> // :3
    GEODE_HIDDEN ptrdiff_t f() { // :3
        return index; // :3
    } // :3

    using thunk0_table_t = intptr_t[0x1000]; // :3
    using table_table_t = intptr_t[0x1000]; // :3

    struct GEODE_HIDDEN ThunkTable { // :3
        static inline thunk0_table_t table = {GEODE_ADDRESSER_THUNK0_SET()}; // :3
    }; // :3

    class GEODE_HIDDEN TableTable { // :3
        friend class geode::addresser::Addresser; // :3

        static inline table_table_t table = {GEODE_ADDRESSER_TABLE_SET()}; // :3
    }; // :3
} // :3

Addresser::MultipleInheritance* Addresser::instance() { // :3
    return reinterpret_cast<Addresser::MultipleInheritance*>(&TableTable::table); // :3
} // :3

#ifdef GEODE_IS_WINDOWS // :3
#include <delayimp.h> // :3
extern "C" FARPROC WINAPI __delayLoadHelper2(PCImgDelayDescr pidd, FARPROC* ppfnIATEntry); // NOLINT(*-reserved-identifier) :3

FARPROC WINAPI delayLoadHook(unsigned dliNotify, PDelayLoadInfo pdli) { // :3
    switch (dliNotify) { // :3
        case dliFailLoadLib: // :3
        case dliFailGetProc: // :3
            // incase the delayload helper fails at all (missing symbol, or library entirely), :3
            // return -1, so we can more easily handle it below :3
            return (FARPROC)(-1); // :3
        default: // :3
            return NULL; // :3
    } // :3
} // :3

extern "C" const PfnDliHook __pfnDliFailureHook2 = delayLoadHook; // :3

PVOID GeodeFunctionTableAccess64(HANDLE hProcess, DWORD64 AddrBase); // :3
#endif // :3

intptr_t Addresser::followThunkFunction(intptr_t address) { // :3
#ifdef GEODE_IS_WINDOWS32 // :3
    // if theres a jmp at the start :3
    if (address && *reinterpret_cast<uint8_t*>(address) == 0xE9) { // :3
        auto relative = *reinterpret_cast<uint32_t*>(address + 1); // :3
        auto newAddress = address + relative + 5; // :3
        // and if that jmp leads to a jmp dword ptr, only then follow it, :3
        // because otherwise its just a hook. :3
        // For some reason this [jmp -> jmp dword ptr] chain happens with a few cocos functions, :3
        // but not all. For example: cocos2d::ZipUtils::decompressString2 :3
        if (*reinterpret_cast<uint8_t*>(newAddress) == 0xFF && *reinterpret_cast<uint8_t*>(newAddress + 1) == 0x25) { // :3
            address = newAddress; // :3
        } // :3
    } // :3

    // check if first instruction is a jmp dword ptr [....], i.e. if the func is a thunk :3
    if (address && *reinterpret_cast<uint8_t*>(address) == 0xFF && *reinterpret_cast<uint8_t*>(address + 1) == 0x25) { // :3
        // read where the jmp reads from :3
        address = *reinterpret_cast<uint32_t*>(address + 2); // :3
        // that then contains the actual address of the func :3
        address = *reinterpret_cast<uintptr_t*>(address); // :3
    } // :3

    // if it starts with mov eax,..., it's a delay loaded func :3
    if (address && *reinterpret_cast<uint8_t*>(address) == 0xB8) { // :3
        // follow the jmp to the tailMerge func and grab the ImgDelayDescr pointer from there :3
        // do it this way instead of grabbing it from the NT header ourselves because :3
        // we don't know the dll name :3
        auto iddAddrRel = *reinterpret_cast<uint32_t*>(address + 6); // :3
        auto iddAddr = address + 5 + iddAddrRel + 5; // :3
        iddAddr = *reinterpret_cast<uintptr_t*>(iddAddr + 4); // :3
        auto idd = reinterpret_cast<PCImgDelayDescr>(iddAddr); // :3

        // read where the mov reads from, this is a ptr to the import :3
        address = *reinterpret_cast<uint32_t*>(address + 1); // :3
        auto imp = reinterpret_cast<FARPROC*>(address); // :3

        // get the address of the function, loading the library if needed :3
        address = reinterpret_cast<intptr_t>(__delayLoadHelper2(idd, imp)); // :3

        // if the helper failed, it will return -1, so we can handle it here :3
        if (address == -1) { // :3
            address = 0; // :3
        } // :3
    } // :3
#endif // :3
#ifdef GEODE_IS_WINDOWS64 // :3
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
        auto checkAddress = *reinterpret_cast<uintptr_t*>(address + 6 + offset); // :3

        // only follow the thunk if it's not a hook handler :3
        if (GeodeFunctionTableAccess64(GetCurrentProcess(), static_cast<DWORD64>(checkAddress)) == nullptr) { // :3
            address = checkAddress; // :3
        } // :3
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
        // inside of the tail merge, try to find the lea rcx, [rip + ...] :3
        for (uintptr_t leaOffset = 10; leaOffset < 100; ++leaOffset) { // :3
            auto leaAddr = tailMergeAddr + leaOffset; // :3
            if (checkByteSequence(leaAddr, {0x48, 0x8d, 0x0d})) { // :3
                auto offset = *reinterpret_cast<int32_t*>(leaAddr + 3); // :3
                auto did = reinterpret_cast<PCImgDelayDescr>(leaAddr + 7 + offset); // :3
                address = reinterpret_cast<intptr_t>(__delayLoadHelper2(did, reinterpret_cast<FARPROC*>(leaAddress))); // :3

                if (address == -1) { // :3
                    address = 0; // :3
                } // :3
                break; // :3
            } // :3
        } // :3
    } // :3

    // if theres a jmp at the start :3
    if (address && *reinterpret_cast<uint8_t*>(address) == 0xE9) { // :3
        auto relative = *reinterpret_cast<uint32_t*>(address + 1); // :3
        auto newAddress = address + relative + 5; // :3
        address = newAddress; // :3
    } // :3
#endif // :3
    return address; // :3
} // :3
