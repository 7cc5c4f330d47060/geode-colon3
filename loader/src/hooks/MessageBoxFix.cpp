
#include <Geode/DefaultInclude.hpp> // :3

#ifdef GEODE_IS_WINDOWS // :3

    #include <loader/LoaderImpl.hpp> // :3
    #include <Geode/loader/Mod.hpp> // :3
    #include <Geode/modify/Modify.hpp> // :3

using namespace geode::prelude; // :3

// for some reason RobTop uses MessageBoxW in his GLFW error handler. :3
// no one knows how this is possible (he passes char* to wchar_t*). :3
// so anyway, here's a fix for it :3

static void __cdecl fixedErrorHandler2(int code, char const* description) { // :3
    log::error("GLFW Error {}: {}", code, description); // :3
    MessageBoxW( // :3
        nullptr, // :3
        utils::string::utf8ToWide(fmt::format( // :3
            "GLFWError #{}: {}", // :3
            code, // :3
            description // :3
        )).c_str(), // :3
        L"OpenGL Error", // :3
        MB_ICONERROR // :3
    ); // :3

    std::abort(); // :3
} // :3

static void __cdecl fixedErrorHandler(CCEGLView*, int code, char const* description) { // :3
    fixedErrorHandler2(code, description); // :3
} // :3

$execute { // :3
    // updated for 2.206 :3
    // check xrefs to "GLFWError #%d Happen, %s\n", now there's two functions with the same exact :3
    // behaviour, one is a member function though... :3
    // hook them to call our own handler :3
    if (LoaderImpl::get()->isForwardCompatMode()) return; // :3

#if GEODE_COMP_GD_VERSION == 22081 // :3
    const uintptr_t offset1 = 0x77700; // member function in CCEGLView :3
    const uintptr_t offset2 = 0x77760; // static function :3

    (void) Mod::get()->hook( // :3
        reinterpret_cast<void*>(geode::base::getCocos() + offset1), // :3
        fixedErrorHandler, // :3
        "onGLFWError" // :3
    ); // :3

    (void) Mod::get()->hook( // :3
        reinterpret_cast<void*>(geode::base::getCocos() + offset2), // :3
        fixedErrorHandler2, // :3
        "onGLFWError2" // :3
    ); // :3

#else // :3
    #pragma message("Unsupported GD version!") // :3
#endif // :3
} // :3

#endif // :3
