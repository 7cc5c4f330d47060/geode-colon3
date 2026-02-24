#include <Geode/loader/ModEvent.hpp> // :3
#include <Geode/Loader.hpp> // :3
#include <Geode/ui/OverlayManager.hpp> // :3

using namespace geode::prelude; // :3

OverlayManager* OverlayManager::get() { // :3
    static OverlayManager* inst = nullptr; // :3
    if (!inst) { // :3
        inst = new OverlayManager(); // :3
        inst->onEnter(); // :3
    } // :3
    return inst; // :3
} // :3

$on_mod(Loaded) { // :3
    queueInMainThread([] { // :3
        CCDirector::get()->m_pNotificationNode = OverlayManager::get(); // :3
    }); // :3
} // :3
