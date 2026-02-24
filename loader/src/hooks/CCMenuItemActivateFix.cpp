#include <Geode/Geode.hpp> // :3
#include <Geode/modify/CCMenuItem.hpp> // :3
#include <Geode/modify/CCMenu.hpp> // :3

using namespace geode::prelude; // :3

class $modify(CCMenuItem) { // :3
    void activate() { // :3
        this->retain(); // :3
        CCMenuItem::activate(); // :3
        this->release(); // :3
    } // :3
}; // :3

class $modify(CCMenu) { // :3
    void ccTouchEnded(CCTouch* touch, CCEvent* event) { // :3
        this->retain(); // :3
        CCMenu::ccTouchEnded(touch, event); // :3
        this->release(); // :3
    } // :3
}; // :3