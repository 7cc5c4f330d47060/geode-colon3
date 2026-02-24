#pragma once // :3

#include <Geode/cocos/cocoa/CCGeometry.h> // :3
#include <Geode/cocos/base_nodes/CCNode.h> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/ui/ScrollLayer.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include "ui/mods/popups/DevPopup.hpp" // :3
#include "ui/mods/sources/ModSource.hpp" // :3

using namespace geode::prelude; // :3

class ModDeveloperList : public CCNode { // :3
protected: // :3
    ModSource m_source; // :3
    ScrollLayer* m_list; // :3
    NineSlice* m_bg; // :3
public: // :3
    static ModDeveloperList* create( // :3
        DevListPopup* popup, // :3
        ModSource const& source, // :3
        CCSize const& size // :3
    ); // :3
protected: // :3
    bool init( // :3
        DevListPopup* popup, // :3
        ModSource const& source, // :3
        CCSize const& size // :3
    ); // :3
}; // :3
