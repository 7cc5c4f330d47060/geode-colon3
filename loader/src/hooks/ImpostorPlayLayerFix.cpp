#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/modify/LevelPage.hpp> // :3

using namespace geode::prelude; // :3

// When pressing spacebar during the transition into LevelSelectLayer, :3
// a PlayLayer will be created but it will never be added to a scene. :3
// It will still exist in memory, potentially causing issues in mods, i.e. Custom Keybinds. :3
// :3
// This workaround solves the issue by making it impossible to start the level during a transition. :3

struct CustomLevelPage : Modify<CustomLevelPage, LevelPage> { // :3
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("Impostor PlayLayer fix") // :3

    void onPlay(cocos2d::CCObject* sender) { // :3
        if (!typeinfo_cast<CCTransitionScene*>(CCScene::get())) { // :3
            LevelPage::onPlay(sender); // :3
        } // :3
    } // :3
}; // :3
