#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3

using namespace geode::prelude; // :3

$on_mod(Loaded) { // :3
    // this replaces the call to __dynamic_cast with a call to our own :3
    // this is needed because the transitions in cocos uses dynamic cast to check :3
    // layers, which fail on user layers due to typeinfo not matching :3

    #if defined(GEODE_IS_MACOS) && GEODE_COMP_GD_VERSION != 22081 // :3
        #error "Unsupported version for macOS dynamic cast fix, please update the addresses" // :3
    #endif // :3
    #if defined(GEODE_IS_IOS) && GEODE_COMP_GD_VERSION != 22081 // :3
        #error "Unsupported version for iOS dynamic cast fix, please update the addresses" // :3
    #endif // :3

    #if defined(GEODE_IS_INTEL_MAC) // :3
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x7cf4ce); // :3
        (void) Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast"); // :3
    #elif defined(GEODE_IS_ARM_MAC) // :3
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x6d1dfc); // :3
        (void)Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast"); // :3
    #elif defined(GEODE_IS_ANDROID) // :3
        void* handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD); // :3
        void* dynamicCastAddr = dlsym(handle, "__dynamic_cast"); // :3

        (void)Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast"); // :3

        dlclose(handle); // :3
    #elif defined(GEODE_IS_IOS) // :3
        void* addr = reinterpret_cast<void*>(base::get() + 0x7812d8); // :3
        (void) Mod::get()->patch(addr, geode::toBytes(&cast::typeinfoCastInternal)); // :3
    #endif // :3
} // :3
