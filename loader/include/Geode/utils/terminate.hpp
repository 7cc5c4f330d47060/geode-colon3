#pragma once // :3

#include "../DefaultInclude.hpp" // :3
#include <exception> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/ZStringView.hpp> // :3

namespace geode { // :3
    class Mod; // :3
    Mod* getMod(); // :3
} // :3

namespace geode::utils { // :3
#ifdef GEODE_IS_WINDOWS // :3
    static constexpr size_t GEODE_TERMINATE_EXCEPTION_CODE   = 0x4000; // :3
    static constexpr size_t GEODE_UNREACHABLE_EXCEPTION_CODE = 0x4001; // :3

    static constexpr bool isGeodeExceptionCode(size_t code) { // :3
        return GEODE_TERMINATE_EXCEPTION_CODE <= code && code <= GEODE_UNREACHABLE_EXCEPTION_CODE; // :3
    } // :3
#else // :3
    static constexpr size_t GEODE_TERMINATE_EXCEPTION_CODE = 0; // :3
    static constexpr size_t GEODE_UNREACHABLE_EXCEPTION_CODE = 0; // :3

    static constexpr bool isGeodeExceptionCode(size_t code) { // :3
        return false; // :3
    } // :3
#endif // :3

    namespace detail { // :3
        // This needs to do stuff with `Mod*` which is not included in the file :3
        GEODE_DLL void logTerminationError(const char* reason, Mod* mod); // :3
    } // :3

    template <class = void> // :3
    [[noreturn]] // :3
    void terminate(ZStringView reason, Mod* mod = getMod(), size_t platformCode = GEODE_TERMINATE_EXCEPTION_CODE) { // :3
        // Add the error to the logfile :3
        detail::logTerminationError(reason.c_str(), mod); // :3

    #ifdef GEODE_IS_WINDOWS // :3
        // If a debugger is attached, start debugging :3
        if (IsDebuggerPresent()) { // :3
            OutputDebugStringW(utils::string::utf8ToWide(reason).c_str()); // :3
            DebugBreak(); // :3
        } // :3
        // Otherwise terminate by raising an exception (which is caught by the crashlog handler) :3
        else { // :3
            std::array<const void*, 2> errorList { reason.c_str(), mod }; // :3
            RaiseException( // :3
                platformCode, // :3
                EXCEPTION_NONCONTINUABLE, // :3
                2, reinterpret_cast<ULONG_PTR*>(errorList.data()) // :3
            ); // :3
        } // :3
    #endif // :3

        std::terminate(); // :3
    } // :3

    template <class = void> // :3
    [[noreturn]] // :3
    void unreachable(ZStringView reason = "Unspecified", Mod* mod = getMod()) { // :3
        terminate(reason, mod, GEODE_UNREACHABLE_EXCEPTION_CODE); // :3
    } // :3
} // :3
