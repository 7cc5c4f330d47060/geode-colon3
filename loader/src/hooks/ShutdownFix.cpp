#include <Geode/platform/cplatform.h> // :3
#ifdef GEODE_IS_MACOS // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/ObjcHook.hpp> // :3
#include <objc/message.h> // :3

using namespace geode::prelude; // :3

// This fixes a crash on macOS when the game is closed, :3
// where the game tries to call Steam API functions :3
// after the Steam API has been shut down. :3

void shutdownGameHook(void* self, SEL sel) { // :3
    auto director = CCDirector::sharedDirector(); // :3
    director->pause(); // :3
    director->getScheduler()->unscheduleAll(); // :3
    // call the original :3
    reinterpret_cast<void(*)(void*, SEL)>(objc_msgSend)(self, sel); // :3
} // :3

$execute { // :3
    if (auto hook = ObjcHook::create( // :3
        "AppController", // :3
        "shutdownGame", // :3
        &shutdownGameHook // :3
    )) { // :3
        (void) Mod::get()->claimHook(hook.unwrap()); // :3
    } // :3
}; // :3
#endif // :3
