#include <Geode/loader/GameEvent.hpp> // :3
#include <Geode/modify/GameManager.hpp> // :3

using namespace geode::prelude; // :3

struct GameReloadHook : Modify<GameReloadHook, GameManager> { // :3
    void reloadAllStep5() { // :3
        GameManager::reloadAllStep5(); // :3
        GameEvent(GameEventType::TexturesUnloaded).send(); // :3
    } // :3
}; // :3
