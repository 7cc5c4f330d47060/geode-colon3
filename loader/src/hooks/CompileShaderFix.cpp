#include <Geode/Geode.hpp> // :3
#include <loader/LoaderImpl.hpp> // :3

using namespace geode::prelude; // :3

$on_mod(Loaded) { // :3
    if (LoaderImpl::get()->isForwardCompatMode()) return; // :3

    // patch an abort() call to "return false;" in CCGLProgram::compileShader :3
    // for some reason cocos only properly returns false on winRT, everywhere :3
    // else it just closes the whole game :3

#if defined(GEODE_IS_WINDOWS) // :3
    auto addr = reinterpret_cast<uintptr_t>( // :3
        GetProcAddress( // :3
            GetModuleHandle("libcocos2d.dll"), "?compileShader@CCGLProgram@cocos2d@@AEAA_NPEAIIPEBD@Z" // :3
        ) // :3
    ) + 0xbb; // :3

    (void) Mod::get()->patch(reinterpret_cast<void*>(addr), { // :3
        0x31, 0xc0, // xor eax, eax :3
        0xeb, 0x07 // jmp +7 (to a nearby ret) :3
    }); // :3
#elif defined(GEODE_IS_ANDROID64) // :3
    auto addr = reinterpret_cast<uintptr_t>( // :3
        dlsym(RTLD_DEFAULT, "_ZN7cocos2d11CCGLProgram13compileShaderEPjjPKc") // :3
    ) + 0x74; // :3

    (void) Mod::get()->patch(reinterpret_cast<void*>(addr), { // :3
        0x1f, 0x20, 0x03, 0xd5 // nop (skip if statement) :3
    }); // :3
#elif defined(GEODE_IS_ANDROID32) // :3
    auto addr = reinterpret_cast<uintptr_t>( // :3
        dlsym(RTLD_DEFAULT, "_ZN7cocos2d11CCGLProgram13compileShaderEPjjPKc") // :3
    ) + 0x43; // :3

    (void) Mod::get()->patch(reinterpret_cast<void*>(addr), { // :3
        0x14, 0xe0 // b +2c (skip if statement) :3
    }); // :3
#elif defined(GEODE_IS_ARM_MAC) // :3
    #if GEODE_COMP_GD_VERSION != 22081 // :3
        #error "Unsupported GD version!" // :3
    #endif // :3

    auto addr = base::get() + 0x39d458; // :3

    (void) Mod::get()->patch(reinterpret_cast<void*>(addr), { // :3
        0x1f, 0x20, 0x03, 0xd5 // nop (skip if statement) :3
    }); // :3
#elif defined(GEODE_IS_INTEL_MAC) // :3
    #if GEODE_COMP_GD_VERSION != 22081 // :3
        #error "Unsupported GD version!" // :3
    #endif // :3

    auto addr = base::get() + 0x42aa35; // :3

    (void) Mod::get()->patch(reinterpret_cast<void*>(addr), { // :3
        0x48, 0x90, // nop (skip if statement) :3
    }); // :3
#elif defined(GEODE_IS_IOS) // :3
    #if GEODE_COMP_GD_VERSION != 22081 // :3
        #error "Unsupported GD version!" // :3
    #endif // :3

    if (Loader::get()->isPatchless()) { // :3
        GEODE_MOD_STATIC_PATCH(0x135374, "\x1f\x20\x03\xd5"); // :3
    } // :3
    else { // :3
        auto addr = base::get() + 0x135374; // :3
        (void) Mod::get()->patch(reinterpret_cast<void*>(addr), { // :3
            0x1f, 0x20, 0x03, 0xd5 // nop (skip if statement) :3
        }); // :3
    } // :3
#endif // :3
}; // :3
