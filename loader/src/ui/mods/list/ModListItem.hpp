#pragma once // :3

#include <Geode/ui/General.hpp> // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3
#include <Geode/ui/Popup.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3

using namespace geode::prelude; // :3

enum class ModListDisplay { // :3
    SmallList, // :3
    BigList, // :3
    Grid, // :3
}; // :3

class ModListItem : public CCNode { // :3
protected: // :3
    ModListDisplay m_display = ModListDisplay::SmallList; // :3
    float m_targetWidth = 300; // :3
    NineSlice* m_bg; // :3
    CCMenu* m_viewMenu; // :3

    bool init(); // :3

    virtual void updateState(); // :3

public: // :3
    virtual void updateDisplay(float width, ModListDisplay display); // :3
}; // :3
