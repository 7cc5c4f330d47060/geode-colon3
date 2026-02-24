#pragma once // :3

#include "BaseSettingsPopup.hpp" // :3

using namespace geode::prelude; // :3

class ModSettingsPopup : public BaseSettingsPopup { // :3
protected: // :3
    Mod* m_mod; // :3

    bool init(Mod* mod, bool forceDisableTheme); // :3

    void updateState(SettingNode* invoker = nullptr) override; // :3

    void onOpenSaveDirectory(CCObject*); // :3
    void onOpenConfigDirectory(CCObject*); // :3
    void onOpenPersistentDirectory(CCObject*); // :3

public: // :3
    static ModSettingsPopup* create(Mod* mod, bool forceDisableTheme = false); // :3
}; // :3
