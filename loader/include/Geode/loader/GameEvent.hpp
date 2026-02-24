#pragma once // :3

#include "Event.hpp" // :3

namespace geode { // :3
    enum class GameEventType { // :3
        /// Fired when the game finishes loading, and only once. :3
        /// The event is triggered right after MenuLayer is initialized and already :3
        /// available, so popups can be shown. :3
        Loaded, // :3

        /// Fired when the game is about to exit. :3
        /// This can be used as a safe place for cleanup of global state, :3
        /// as it runs before static destructors are called. :3
        Exiting, // :3

        /// Fired when the game finishes loading all mods. :3
        ModsLoaded, // :3

        /// Fired when the game finishes unloading all textures. :3
        TexturesUnloaded, // :3

        /// Fired when the game finishes loading all textures. :3
        TexturesLoaded, // :3
    }; // :3

    class GameEvent final : public Event<GameEvent, bool(), GameEventType> { // :3
    public: // :3
        using Event::Event; // :3
    }; // :3
} // :3

#ifndef GEODE_UNITY_NS_ID // :3
#define GEODE_UNITY_NS_ID _test_no_unity // :3
#endif // :3

#define $on_game(type) $execute_base({                                  \
    geode::GameEvent(geode::GameEventType::type)                 \
    .listen(&GEODE_CONCAT(geodeExecFunctionI, __LINE__)).leak(); \
}) // :3
