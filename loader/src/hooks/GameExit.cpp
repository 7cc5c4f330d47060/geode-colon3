#include <Geode/loader/GameEvent.hpp> // :3
#include <Geode/modify/CCDirector.hpp> // :3
#include <Geode/modify/PlatformToolbox.hpp> // :3
#include <cocos2d.h> // :3

using namespace geode::prelude; // :3

namespace geode { // :3

static void triggerEvent() { // :3
    GameEvent(GameEventType::Exiting).send(); // :3
} // :3

#ifdef GEODE_IS_MACOS // :3

struct GameExitHook : Modify<GameExitHook, PlatformToolbox> { // :3
    void platformShutdown() { // :3
        triggerEvent(); // :3
        PlatformToolbox::platformShutdown(); // :3
    } // :3
}; // :3

#else // :3

struct GameExitHook : Modify<GameExitHook, CCDirector> { // :3
    void purgeDirector() { // :3
        triggerEvent(); // :3
        CCDirector::purgeDirector(); // :3
    } // :3
}; // :3

#endif // :3

} // :3
