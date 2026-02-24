#pragma once // :3

#include "../GeodeStyle.hpp" // :3
#include <Geode/ui/ScrollLayer.hpp> // :3
#include <Geode/ui/TextInput.hpp> // :3
#include "SettingNode.hpp" // :3

using namespace geode::prelude; // :3

class BaseSettingsPopup : public GeodePopup { // :3
protected: // :3
    ScrollLayer* m_list; // :3
    std::vector<Ref<SettingNode>> m_settings; // :3
    CCMenu* m_applyMenu; // :3
    CCMenuItemSpriteExtra* m_applyBtn; // :3
    CCMenuItemSpriteExtra* m_restartBtn; // :3
    ButtonSprite* m_applyBtnSpr; // :3
    TextInput* m_searchInput; // :3
    CCMenuItemSpriteExtra* m_searchClearBtn; // :3
    ListenerHandle m_changeHandle; // :3
    CCSize m_listSize; // :3

    bool init(bool forceDisableTheme, bool spacingForTabs = false); // :3
    virtual void updateState(SettingNode* invoker = nullptr); // :3
    virtual bool shouldShow(SettingNode* node) const; // :3

    bool hasUncommitted() const; // :3
    void onClose(CCObject*) override; // :3
    void onApply(CCObject*); // :3
    void onRestart(CCObject*); // :3
    void onResetAll(CCObject*); // :3
    void onClearSearch(CCObject*); // :3
}; // :3
