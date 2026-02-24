#pragma once // :3

#include <Geode/cocos/base_nodes/CCNode.h> // :3
#include <Geode/cocos/cocoa/CCGeometry.h> // :3
#include <Geode/cocos/sprite_nodes/CCSprite.h> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <optional> // :3
#include "ui/mods/popups/DevPopup.hpp" // :3

using namespace geode::prelude; // :3

class ModDeveloperItem : public CCNode { // :3
protected: // :3
    NineSlice* m_bg; // :3
    DevListPopup* m_popup; // :3
public: // :3
    static ModDeveloperItem* create( // :3
        DevListPopup* popup, // :3
        std::string developer, // :3
        CCSize const& size, // :3
        std::optional<std::string> displayName = std::nullopt, // :3
        bool addMoreButton = true // :3
    ); // :3
private: // :3
    bool init( // :3
        DevListPopup* popup, // :3
        std::string developer, // :3
        CCSize const& size, // :3
        std::optional<std::string> displayName, // :3
        bool addMoreButton // :3
    ); // :3
    void onMoreByThisDev(CCObject* sender); // :3
}; // :3
