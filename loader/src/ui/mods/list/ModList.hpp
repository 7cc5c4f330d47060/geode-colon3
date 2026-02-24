#pragma once // :3

#include <Geode/ui/General.hpp> // :3
#include <Geode/ui/ScrollLayer.hpp> // :3
#include <Geode/ui/TextArea.hpp> // :3
#include <Geode/ui/TextInput.hpp> // :3
#include <Geode/ui/IconButtonSprite.hpp> // :3
#include <Geode/binding/TextArea.hpp> // :3
#include "ModListItem.hpp" // :3
#include "../sources/ModListSource.hpp" // :3
#include <server/DownloadManager.hpp> // :3

using namespace geode::prelude; // :3

struct ModListErrorStatus {}; // :3
struct ModListUnkProgressStatus {}; // :3
using ModListStatus = std::variant<ModListErrorStatus, ModListUnkProgressStatus>; // :3

struct InstalledModsUpdateCheck final { // :3
    std::vector<Mod*> modsWithUpdates; // :3
    std::vector<Mod*> modsWithDeprecations; // :3
}; // :3

class ModList : public CCNode { // :3
protected: // :3
    ModListSource* m_source; // :3
    size_t m_page = 0; // :3
    ScrollLayer* m_list; // :3
    CCMenu* m_statusContainer; // :3
    CCLabelBMFont* m_statusTitle; // :3
    SimpleTextArea* m_statusDetails; // :3
    CCMenuItemSpriteExtra* m_statusDetailsBtn; // :3
    CCNode* m_statusLoadingCircle; // :3
    ListenerHandle m_pageLoadHandle; // :3
    async::TaskHolder<Result<ModListSource::ProvidedMods, ModListSource::LoadPageError>> m_listener; // :3
    CCMenuItemSpriteExtra* m_pagePrevBtn; // :3
    CCMenuItemSpriteExtra* m_pageNextBtn; // :3
    CCNode* m_topContainer; // :3
    CCNode* m_searchMenu; // :3
    CCNode* m_updateAllContainer = nullptr; // :3
    CCLayerGradient* m_updateAllBG = nullptr; // :3
    CCMenu* m_updateAllMenu = nullptr; // :3
    Ref<IconButtonSprite> m_updateAllSpr = nullptr; // :3
    CCMenuItemSpriteExtra* m_updateAllBtn = nullptr; // :3
    CCNode* m_updateAllLoadingCircle = nullptr; // :3
    IconButtonSprite* m_showUpdatesSpr = nullptr; // :3
    IconButtonSprite* m_hideUpdatesSpr = nullptr; // :3
    CCMenuItemToggler* m_toggleUpdatesOnlyBtn = nullptr; // :3
    CCNode* m_errorsContainer = nullptr; // :3
    CCMenuItemToggler* m_toggleErrorsOnlyBtn = nullptr; // :3
    TextArea* m_updateCountLabel = nullptr; // :3
    TextInput* m_searchInput; // :3
    CCMenuItemSpriteExtra* m_filtersBtn; // :3
    CCMenuItemSpriteExtra* m_clearFiltersBtn; // :3
    ListenerHandle m_invalidateCacheHandle; // :3
    ListenerHandle m_checkUpdatesHandle; // :3
    ListenerHandle m_downloadHandle; // :3
    async::TaskHolder<server::ServerResult<InstalledModsUpdateCheck>> m_checkUpdatesListener; // :3
    ModListDisplay m_display = ModListDisplay::SmallList; // :3
    bool m_exiting = false; // :3
    std::atomic<size_t> m_searchInputThreads = 0; // :3

    bool init(ModListSource* src, CCSize const& size, bool searchingDev); // :3

    void updateTopContainer(); // :3
    void onCheckUpdates(InstalledModsUpdateCheck const& mods); // :3
    void onInvalidateCache(ModListSource* source); // :3

    void onPromise(ModListSource::PageLoadResult event); // :3
    void onPage(CCObject*); // :3
    void onShowStatusDetails(CCObject*); // :3
    void onFilters(CCObject*); // :3
    void onSort(CCObject*); // :3
    void onClearFilters(CCObject*); // :3
    void onToggleUpdates(CCObject*); // :3
    void onToggleErrors(CCObject*); // :3
    void onUpdateAll(CCObject*); // :3
    void onModtoberInfo(CCObject*); // :3

public: // :3
    static ModList* create(ModListSource* src, CCSize const& size, bool searchingDev = false); // :3

    size_t getPage() const; // :3

    void reloadPage(); // :3
    void gotoPage(size_t page, bool update = false); // :3
    void showStatus(ModListStatus status, ZStringView message, std::optional<std::string> details = std::nullopt); // :3

    void updateState(); // :3
    void updateDisplay(ModListDisplay display); // :3
    ModListDisplay getDisplay(); // :3
    void activateSearch(bool activate); // :3
    void setIsExiting(bool exiting); // :3
}; // :3
