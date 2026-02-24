#include <Geode/DefaultInclude.hpp> // :3

#include <Geode/loader/Mod.hpp> // :3
#include <loader/ModImpl.hpp> // :3

using namespace geode::prelude; // :3

template <typename T> // :3
T findSymbolOrMangled(HMODULE load, char const* name, char const* mangled) { // :3
    auto res = reinterpret_cast<T>(GetProcAddress(load, name)); // :3
    if (!res) { // :3
        res = reinterpret_cast<T>(GetProcAddress(load, mangled)); // :3
    } // :3
    return res; // :3
} // :3

char const* getUsefulError(DWORD code) { // :3
    switch (code) { // :3
        case ERROR_MOD_NOT_FOUND: // :3
            return "ERROR_MOD_NOT_FOUND; The mod is either missing the DLL " // :3
                   "file or some of its dependencies. Make sure to list all " // :3
                   "other mods you depend on under dependencies and include " // :3
                   "other DLLs under resources in mod.json. " // :3
                   "If you are not the developer of this mod, report this error " // :3
                   "to them as it is likely not your fault."; // :3

        case ERROR_PROC_NOT_FOUND: // :3
            return "ERROR_PROC_NOT_FOUND; The mod tried to access " // :3
                   "a function defined in another DLL, but the specified " // :3
                   "function was not found. Make sure the other DLL exports " // :3
                   "the given symbol, and that it is defined in the DLL. " // :3
                   "If you are not the developer of this mod, report this error " // :3
                   "to them as it is likely not your fault."; // :3

        case ERROR_DLL_INIT_FAILED: // :3
            return "ERROR_DLL_INIT_FAILED; Likely some global variables " // :3
                   "in the mod threw an exception or otherwise failed. " // :3
                   "ALSO MAKE SURE YOU ARE COMPILING IN RELEASE MODE. " // :3
                   "If you are not the developer of this mod, report this error " // :3
                   "to them as it is likely not your fault."; // :3

        default: break; // :3
    } // :3
    return nullptr; // :3
} // :3

std::string getLastWinError() { // :3
    auto err = GetLastError(); // :3
    if (!err) return "None (0)"; // :3
    auto useful = getUsefulError(err); // :3
    if (useful) return useful; // :3

    return formatSystemError(err); // :3
} // :3

Result<> Mod::Impl::loadPlatformBinary() { // :3
    auto load = LoadLibraryW(this->getBinaryPath().c_str()); // :3
    if (load) { // :3
        if (m_platformInfo) { // :3
            delete m_platformInfo; // :3
        } // :3
        m_platformInfo = new PlatformInfo { load }; // :3

        auto geodeImplicitEntry = findSymbolOrMangled<void(*)()>(load, "geodeImplicitEntry", "_geodeImplicitEntry@0"); // :3
        if (geodeImplicitEntry) { // :3
            geodeImplicitEntry(); // :3
        } // :3

        auto geodeCustomEntry = findSymbolOrMangled<void(*)()>(load, "geodeCustomEntry", "_geodeCustomEntry@0"); // :3
        if (geodeCustomEntry) { // :3
            geodeCustomEntry(); // :3
        } // :3
        return Ok(); // :3
    } // :3
    return Err("Unable to load the DLL: " + getLastWinError()); // :3
} // :3
