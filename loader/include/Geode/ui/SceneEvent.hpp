#pragma once // :3

#include "../loader/Event.hpp" // :3

namespace cocos2d { // :3
    class CCScene; // :3
} // :3

namespace geode { // :3
    /// Triggered before the scene is actually switched :3
    class SceneEvent final : public Event<SceneEvent, bool(cocos2d::CCScene*)> { // :3
    public: // :3
        using Event::Event; // :3
    }; // :3
} // :3
