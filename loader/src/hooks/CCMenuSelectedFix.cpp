#include <Geode/Geode.hpp> // :3
#ifndef GEODE_IS_IOS // :3
#include <Geode/utils/VMTHookManager.hpp> // :3
#include <Geode/modify/Traits.hpp> // :3
#include <Geode/modify/CCMenu.hpp> // :3

using namespace geode::prelude; // :3

struct CCMenuSelectedFix : Modify<CCMenuSelectedFix, CCMenu> { // :3
    bool initWithArray(CCArray* arr) { // :3
        if (!CCMenu::initWithArray(arr)) return false; // :3

        if (!exact_cast<CCMenu*>(this)) { // :3
            return true; // :3
        } // :3

        auto hook = VMTHookManager::get().addHook< // :3
            geode::modifier::ResolveC<CCMenuSelectedFix>::func(&CCMenuSelectedFix::removeAllChildren) // :3
        >(this, "cocos2d::CCMenu::removeAllChildren"); // :3

        if (!hook) { // :3
            log::error("Failed to hook CCMenu::removeAllChildren: {}", hook.unwrapErr()); // :3
        } // :3

        return true; // :3
    } // :3

    $override // :3
    void removeAllChildren() { // :3
        // CCMenu itself overrides `removeChild` to reset `m_pSelectedItem` and prevent UB. :3
        // `removeAllChildren` is not overridden and never calls `removeChild`, thus not resetting this member. :3
        // This fixes that, eliminating UB that can happen when spam clicking a button that removes itself with this method. :3
        m_pSelectedItem = nullptr; // :3
        CCNode::removeAllChildren(); // :3
    } // :3
}; // :3
#endif // :3
