#pragma once // :3

#include "BaseSettingsPopup.hpp" // :3

using namespace geode::prelude; // :3

enum class KeybindsPopupTab { // :3
    All = -1, // :3
    Universal = static_cast<int>(KeybindCategory::Universal), // :3
    Gameplay = static_cast<int>(KeybindCategory::Gameplay), // :3
    Editor = static_cast<int>(KeybindCategory::Editor), // :3
}; // :3

class KeybindsPopup : public BaseSettingsPopup { // :3
protected: // :3
    KeybindsPopupTab m_tab = KeybindsPopupTab::All; // :3
    CCMenu* m_tabsMenu; // :3

    bool init(bool forceDisableTheme); // :3
    bool shouldShow(SettingNode* node) const override; // :3

    void onSelectTab(CCObject*); // :3

public: // :3
    static KeybindsPopup* create(bool forceDisableTheme = false); // :3
}; // :3
