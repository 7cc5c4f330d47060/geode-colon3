#pragma once // :3

#include <Geode/ui/Popup.hpp> // :3
#include "../UpdateModListState.hpp" // :3

using namespace geode::prelude; // :3

class ConfirmUninstallPopup : public Popup { // :3
protected: // :3
    Mod* m_mod; // :3
    CCMenuItemToggler* m_deleteDataToggle; // :3

    bool init(Mod* mod); // :3

    void onUninstall(CCObject*); // :3

public: // :3
    static ConfirmUninstallPopup* create(Mod* mod); // :3
}; // :3
