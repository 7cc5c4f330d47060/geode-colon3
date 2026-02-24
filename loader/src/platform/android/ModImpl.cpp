#include <Geode/DefaultInclude.hpp> // :3

#include <Geode/loader/Mod.hpp> // :3
#include <loader/ModImpl.hpp> // :3

using namespace geode::prelude; // :3

template <typename T> // :3
T findSymbolOrMangled(void* so, char const* name, char const* mangled) { // :3
    auto res = reinterpret_cast<T>(dlsym(so, name)); // :3
    if (!res) { // :3
        res = reinterpret_cast<T>(dlsym(so, mangled)); // :3
    } // :3
    return res; // :3
} // :3

Result<> Mod::Impl::loadPlatformBinary() { // :3
    auto so = // :3
        dlopen(utils::string::pathToString(this->getBinaryPath()).c_str(), RTLD_LAZY); // :3
    if (so) { // :3
        if (m_platformInfo) { // :3
            delete m_platformInfo; // :3
        } // :3
        m_platformInfo = new PlatformInfo{so}; // :3

        auto geodeImplicitEntry = findSymbolOrMangled<void(*)()>(so, "geodeImplicitEntry", "_Z17geodeImplicitEntryv"); // :3
        if (geodeImplicitEntry) { // :3
            geodeImplicitEntry(); // :3
        } // :3

        auto geodeCustomEntry = findSymbolOrMangled<void(*)()>(so, "geodeCustomEntry", "_Z15geodeCustomEntryv"); // :3
        if (geodeCustomEntry) { // :3
            geodeCustomEntry(); // :3
        } // :3

        return Ok(); // :3
    } // :3
    std::string err = dlerror(); // :3
    log::error("Unable to load the SO: dlerror returned ({})", err); // :3
    return Err("Unable to load the SO: dlerror returned (" + err + ")"); // :3
} // :3
