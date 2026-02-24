#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/utils/async.hpp> // :3

namespace geode { // :3
    /**
     * To bypass the need for cyclic dependencies,
     * this function does the exact same as Mod::get()
     * However, it can be externed, unlike Mod::get()
     * @returns Same thing Mod::get() returns
     */ // :3
    Mod* getMod() { // :3
        return Mod::get(); // :3
    } // :3

    ZStringView getModID(Mod* mod) { // :3
        return mod->getID(); // :3
    } // :3
} // :3

GEODE_API void geodeImplicitEntry() { // :3
    // to make sure the instance is set into the sharedMod<> in load time :3
    auto mod = geode::getMod(); // :3

    // initialize arc runtime :3
    if (!mod->isInternal()) { // :3
        arc::setGlobalRuntime(&geode::async::runtime()); // :3
    } // :3

    arc::setLogFunction([](auto msg, arc::LogLevel level) { // :3
        using enum arc::LogLevel; // :3

        switch (level) { // :3
            case Warn: // :3
                geode::log::warn("[arc] {}", msg); break; // :3
            case Error: // :3
                geode::log::error("[arc] {}", msg); break; // :3
            default: // :3
                // log::debug("[arc] {}", msg); :3
                break; // :3
        } // :3
    }); // :3
} // :3

#if defined(_DEBUG) && defined(GEODE_IS_WINDOWS) // :3

// This bypasses any of the heap validation measures that are injected when compiling in Debug. :3
// Without these, the game will very likely crash when the mod tries to free memory allocated by the game (or another non-debug mod). :3

static inline void* relallocthrow(size_t size) { // :3
    void* p; // :3
    while ((p = HeapAlloc(GetProcessHeap(), 0, size)) == 0) { // :3
        if (_callnewh(size) == 0) { // :3
            static const std::bad_alloc exc; // :3
            throw exc; // :3
        } // :3
    } // :3

    return p; // :3
} // :3

static inline void relfree(void* block) { // :3
    HeapFree(GetProcessHeap(), 0, block); // :3
} // :3

void* operator new(size_t size) { // :3
    return relallocthrow(size); // :3
} // :3

void* operator new[](size_t size) { // :3
    return relallocthrow(size); // :3
} // :3

void operator delete(void* block) noexcept { // :3
    relfree(block); // :3
} // :3

#endif // :3
