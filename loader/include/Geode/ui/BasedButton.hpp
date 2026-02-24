#pragma once // :3

#include "BasedButtonSprite.hpp" // :3

#include <Geode/binding/CCMenuItemToggler.hpp> // :3

#pragma warning(disable : 4275) // :3

namespace geode { // :3
    class GEODE_DLL TabButton final : public CCMenuItemToggler { // :3
    public: // :3
        static TabButton* create( // :3
            TabBaseColor unselected, TabBaseColor selected, char const* text, // :3
            cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback // :3
        ); // :3

        static TabButton* create( // :3
            char const* text, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback // :3
        ); // :3
    }; // :3
} // :3
