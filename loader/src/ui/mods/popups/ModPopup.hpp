#pragma once // :3

#include <Geode/ui/Popup.hpp> // :3
#include <Geode/ui/MDTextArea.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include "../sources/ModSource.hpp" // :3
#include "../GeodeStyle.hpp" // :3
#include "../UpdateModListState.hpp" // :3
#include <server/DownloadManager.hpp> // :3

using namespace geode::prelude; // :3

class ModPopup : public GeodePopup { // :3
public: // :3
    enum class Tab { // :3
        Details, // :3
        Changelog, // :3
        Versions, // :3
    }; // :3

protected: // :3
    ModSource m_source; // :3
    CCNode* m_stats; // :3
    CCNode* m_tags; // :3
    CCMenu* m_installMenu; // :3
    CCMenuItemToggler* m_enableBtn; // :3
    CCMenuItemToggler* m_reenableBtn; // :3
    CCMenuItemSpriteExtra* m_uninstallBtn; // :3
    CCMenuItemSpriteExtra* m_installBtn; // :3
    CCMenuItemSpriteExtra* m_unavailableBtn; // :3
    CCMenuItemSpriteExtra* m_updateBtn; // :3
    CCMenuItemSpriteExtra* m_cancelBtn; // :3
    CCLabelBMFont* m_installStatusLabel; // :3
    NineSlice* m_installBG; // :3
    NineSlice* m_settingsBG; // :3
    CCLabelBMFont* m_enabledStatusLabel; // :3
    ButtonSprite* m_restartRequiredLabel; // :3
    CCNode* m_rightColumn; // :3
    CCNode* m_currentTabPage = nullptr; // :3
    CCNode* m_modtoberBanner = nullptr; // :3
    CCMenu* m_titleContainer = nullptr; // :3
    std::unordered_map<Tab, std::pair<GeodeTabSprite*, Ref<CCNode>>> m_tabs; // :3
    ListenerHandle m_statsHandle; // :3
    ListenerHandle m_tagsHandle; // :3
    ListenerHandle m_checkUpdateHandle; // :3
    ListenerHandle m_updateStateHandle; // :3
    ListenerHandle m_downloadHandle; // :3
    ListenerHandle m_settingNodeHandle; // :3
    async::TaskHolder<server::ServerResult<server::ServerModMetadata>> m_statsListener; // :3
    async::TaskHolder<server::ServerResult<std::vector<server::ServerTag>>> m_tagsListener; // :3
    async::TaskHolder<server::ServerResult<server::ServerModUpdateOneCheck>> m_checkUpdateListener; // :3
    ListenerHandle m_downloadListener; // :3

    bool init(ModSource&& src); // :3
    void updateState(); // :3

    void setStatIcon(CCNode* stat, const char* spr); // :3
    void setStatLabel(CCNode* stat, ZStringView value, bool noValue = false, ccColor3B color = ccWHITE); // :3
    void setStatValue(CCNode* stat, std::optional<std::string> const& value); // :3

    void onLoadServerInfo(server::ServerResult<server::ServerModMetadata> result); // :3
    void onLoadTags(server::ServerResult<std::vector<server::ServerTag>> result); // :3
    void onCheckUpdates(server::ServerResult<server::ServerModUpdateOneCheck> result); // :3

    void onTab(CCObject* sender); // :3
    void onEnable(CCObject*); // :3
    void onInstall(CCObject*); // :3
    void onUninstall(CCObject*); // :3
    void onCancelDownload(CCObject*); // :3

    void onSettings(CCObject*); // :3
    void onLink(CCObject*); // :3
    void onSupport(CCObject*); // :3
    void onModtober24Info(CCObject*); // :3
    void onModtober25Info(CCObject*); // :3

    bool availableForInstall() const; // :3

public: // :3
    void loadTab(Tab tab); // :3
    static ModPopup* create(ModSource&& src); // :3

    ModSource& getSource() &; // :3
}; // :3
