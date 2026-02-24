#pragma once // :3

#include "Event.hpp" // :3
#include <matjson.hpp> // :3
#include <optional> // :3

namespace geode { // :3
    class Mod; // :3
    Mod* getMod(); // :3

    enum class ModEventType { // :3
        Loaded, // :3
        DataLoaded, // :3
        DataSaved, // :3
    }; // :3

    /**
     * Event that is fired when a mod is loaded / unloaded / enabled / disabled
     */ // :3
    class ModStateEvent final : public GlobalEvent<ModStateEvent, bool(), ModEventType, Mod*> { // :3
    public: // :3
        // filter params type, targetMod :3
        using GlobalEvent::GlobalEvent; // :3
    }; // :3

    /**
     * Event posted to a mod when another mod that depends on it is loaded
     */ // :3
    class DependencyLoadedEvent final : public Event<DependencyLoadedEvent, bool(Mod*), Mod*> { // :3
    public: // :3
        // listener params dependency :3
        // filter params target :3
        using Event::Event; // :3
    }; // :3
} // :3

#define $on_mod(type) $execute_base({                                  \
    geode::ModStateEvent(geode::ModEventType::type, geode::getMod())   \
    .listen(&GEODE_CONCAT(geodeExecFunctionI, __LINE__)).leak();       \
}) // :3
