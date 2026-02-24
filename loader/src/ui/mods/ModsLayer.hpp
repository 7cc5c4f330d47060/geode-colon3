#pragma once // :3

#include <Geode/ui/General.hpp> // :3
#include <Geode/ui/ScrollLayer.hpp> // :3
#include <Geode/ui/TextArea.hpp> // :3
#include <Geode/ui/IconButtonSprite.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <Geode/binding/SetTextPopupDelegate.hpp> // :3
#include <Geode/binding/SetIDPopupDelegate.hpp> // :3
#include <Geode/cocos/cocoa/CCObject.h> // :3
#include "list/ModItem.hpp" // :3
#include "list/ModList.hpp" // :3
#include "sources/ModListSource.hpp" // :3
#include "UpdateModListState.hpp" // :3
#include <server/DownloadManager.hpp> // :3
#include <Geode/loader/Setting.hpp> // :3

using namespace geode::prelude; // :3

class ModsStatusNode : public CCNode { // :3
protected: // :3
    enum class DownloadState { // :3
        None, // :3
        SomeCancelled, // :3
        AllDone, // :3
        SomeErrored, // :3
        SomeToBeConfirmed, // :3
        SomeFetching, // :3
        SomeDownloading, // :3
    }; // :3

    NineSlice* m_statusBG; // :3
    CCLabelBMFont* m_status; // :3
    CCLabelBMFont* m_statusPercentage; // :3
    Slider* m_progressBar; // :3
    CCNode* m_loadingCircle; // :3
    CCMenu* m_btnMenu; // :3
    CCMenuItemSpriteExtra* m_viewBtn; // :3
    CCMenuItemSpriteExtra* m_cancelBtn; // :3
    CCMenuItemSpriteExtra* m_restartBtn; // :3
    ListenerHandle m_updateStateHandle; // :3
    ListenerHandle m_downloadHandle; // :3
    DownloadState m_lastState = DownloadState::None; // :3
    ListenerHandle m_settingNodeHandle; // :3

    bool init(); // :3
    void updateState(); // :3

    void onRestart(CCObject*); // :3
    void onViewErrors(CCObject*); // :3
    void onConfirm(CCObject*); // :3
    void onCancel(CCObject*); // :3

public: // :3
    static ModsStatusNode* create(); // :3
}; // :3

class ModsLayer : public CCLayer, public SetIDPopupDelegate { // :3
protected: // :3
    CCNode* m_frame; // :3
    std::vector<CCMenuItemSpriteExtra*> m_tabs; // :3
    ModListSource* m_currentSource = nullptr; // :3
    std::unordered_map<ModListSource*, Ref<ModList>> m_lists; // :3
    CCMenu* m_pageMenu; // :3
    CCLabelBMFont* m_pageLabel; // :3
    CCMenuItemSpriteExtra* m_goToPageBtn; // :3
    ModsStatusNode* m_statusNode; // :3
    ListenerHandle m_updateStateHandle; // :3
    bool m_showSearch = true; // :3
    std::vector<CCMenuItemSpriteExtra*> m_displayBtns; // :3
    ModListDisplay m_modListDisplay; // :3

    bool init(); // :3

    void keyDown(enumKeyCodes key, double) override; // :3
    void keyBackClicked() override; // :3
    void setIDPopupClosed(SetIDPopup*, int value) override; // :3

    void onTab(CCObject* sender); // :3
    void onOpenModsFolder(CCObject*); // :3
    void onAddModFromFile(CCObject*); // :3
    void onRestartGD(CCObject*); // :3
    void onDisplay(CCObject*); // :3
    void onSearch(CCObject*); // :3
    void onGoToPage(CCObject*); // :3
    void onRefreshList(CCObject*); // :3
    void onTheme(CCObject*); // :3
    void onSettings(CCObject*); // :3
    void onKeybinds(CCObject*); // :3
    void onBack(CCObject*); // :3

    void updateState(); // :3

public: // :3
    static ModsLayer* create(); // :3
    static ModsLayer* scene(); // :3

    static server::ServerFuture<InstalledModsUpdateCheck> checkInstalledModsForUpdates(); // :3
    static void installModFromFile(); // :3

    void gotoTab(ModListSource* src, bool searchingDev = false); // :3
    void refreshList(); // :3
}; // :3
