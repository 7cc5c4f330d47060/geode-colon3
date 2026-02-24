#pragma once // :3

#include "../loader/Event.hpp" // :3
#include <optional> // :3

namespace cocos2d { // :3
    class CCNode; // :3
} // :3

namespace geode { // :3
    template<class T> // :3
    concept InheritsCCNode = std::is_base_of_v<cocos2d::CCNode, T>; // :3

    template <InheritsCCNode T> // :3
    class EnterLayerEvent final : public GlobalEvent<EnterLayerEvent<T>, bool(std::string_view, cocos2d::CCNode*), bool(cocos2d::CCNode*), std::string> { // :3
    public: // :3
        // listener params layer :3
        // filter params targetID :3
        using GlobalEvent<EnterLayerEvent<T>, bool(std::string_view, cocos2d::CCNode*), bool(cocos2d::CCNode*), std::string>::GlobalEvent; // :3
    }; // :3
} // :3
