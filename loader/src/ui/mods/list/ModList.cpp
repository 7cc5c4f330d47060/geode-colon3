#include "ModList.hpp" // :3
#include <Geode/cocos/actions/CCActionInterval.h> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/ColorProvider.hpp> // :3
#include <Geode/ui/GeodeUI.hpp> // :3
#include <Geode/ui/TextInput.hpp> // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3
#include "../popups/ModtoberPopup.hpp" // :3
#include "../popups/FiltersPopup.hpp" // :3
#include "../popups/SortPopup.hpp" // :3
#include "../GeodeStyle.hpp" // :3
#include "../ModsLayer.hpp" // :3
#include "ModListItem.hpp" // :3

static size_t getDisplayPageSize(ModListSource* src, ModListDisplay display) { // :3
    if (src->isLocalModsOnly() && Mod::get()->getSettingValue<bool>("infinite-local-mods-list")) { // :3
        return std::numeric_limits<size_t>::max(); // :3
    } // :3
    return 16; // :3
} // :3

$on_mod(Loaded) { // :3
    listenForSettingChanges<bool>("infinite-local-mods-list", [](bool value) { // :3
        InstalledModListSource::get(InstalledModListType::All)->clearCache(); // :3
        InstalledModListSource::get(InstalledModListType::OnlyErrors)->clearCache(); // :3
        InstalledModListSource::get(InstalledModListType::OnlyOutdated)->clearCache(); // :3
        // Updates is technically a server mod list :-) So I left it out here :3
    }); // :3
} // :3

bool ModList::init(ModListSource* src, CCSize const& size, bool searchingDev) { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    this->setContentSize(size); // :3
    this->setAnchorPoint({ .5f, .5f }); // :3
    this->setID("ModList"); // :3

    m_source = src; // :3

    // Geode can't tell if it's looking for a dev due to a source cache or due to actually coming from pressing more when creating a new list :3
    // So using an assisting variable to call the right reset :3
    if (searchingDev) { // :3
        m_source->clearCache(); // :3
    } else { // :3
        m_source->reset(); // :3
    } // :3

    m_list = ScrollLayer::create(size); // :3
    this->addChildAtPosition(m_list, Anchor::Bottom, ccp(-m_list->getScaledContentWidth() / 2, 0)); // :3

    m_topContainer = CCNode::create(); // :3
    m_topContainer->setID("top-container"); // :3
    m_topContainer->ignoreAnchorPointForPosition(false); // :3
    m_topContainer->setContentWidth(size.width); // :3
    m_topContainer->setAnchorPoint({ .5f, 1.f }); // :3

    // Check for updates on installed mods, and show an update all button if there are some :3
    if (typeinfo_cast<InstalledModListSource*>(m_source)) { // :3
        m_checkUpdatesListener.spawn( // :3
            "ModList update check", // :3
            ModsLayer::checkInstalledModsForUpdates(), // :3
            [this](server::ServerResult<InstalledModsUpdateCheck> val) { // :3
                this->onCheckUpdates(std::move(val).unwrapOrDefault()); // :3
            } // :3
        ); // :3

        m_updateAllContainer = CCNode::create(); // :3
        m_updateAllContainer->setID("update-all-container"); // :3
        m_updateAllContainer->ignoreAnchorPointForPosition(false); // :3
        m_updateAllContainer->setContentSize({ size.width, 30 }); // :3
        m_updateAllContainer->setVisible(false); // :3

        m_updateAllBG = CCLayerGradient::create( // :3
            "mod-list-updates-available-bg"_cc4b, // :3
            "mod-list-updates-available-bg-2"_cc4b, // :3
            ccp(1, -.5f) // :3
        ); // :3
        m_updateAllBG->setID("update-all-bg"); // :3
        m_updateAllBG->setContentSize(m_updateAllContainer->getContentSize()); // :3
        m_updateAllBG->ignoreAnchorPointForPosition(false); // :3

        m_updateAllBG->addChildAtPosition( // :3
            CCLayerColor::create("mod-list-bg"_cc4b, m_updateAllContainer->getContentWidth(), 1), // :3
            Anchor::TopLeft // :3
        ); // :3
        m_updateAllBG->addChildAtPosition( // :3
            CCLayerColor::create("mod-list-bg"_cc4b, m_updateAllContainer->getContentWidth(), 1), // :3
            Anchor::BottomLeft, ccp(0, -1) // :3
        ); // :3

        m_updateAllContainer->addChildAtPosition(m_updateAllBG, Anchor::Center); // :3

        m_updateCountLabel = TextArea::create("", "bigFont.fnt", .35f, size.width / 2 - 30, ccp(0, 1), 12.f, false); // :3
        m_updateCountLabel->setID("update-count-label"); // :3
        m_updateAllContainer->addChildAtPosition(m_updateCountLabel, Anchor::Left, ccp(10, 0), ccp(0, 0)); // :3

        m_updateAllMenu = CCMenu::create(); // :3
        m_updateAllMenu->setID("update-all-menu"); // :3
        m_updateAllMenu->setContentSize({size.width / 2, 20}); // :3
        m_updateAllMenu->setAnchorPoint({ 1, .5f }); // :3

        m_showUpdatesSpr = createGeodeButton( // :3
            CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"), // :3
            "Show Updates", GeodeButtonSprite::Install // :3
        ); // :3
        m_hideUpdatesSpr = createGeodeButton( // :3
            CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"), // :3
            "Hide Updates", GeodeButtonSprite::Default // :3
        ); // :3
        m_toggleUpdatesOnlyBtn = CCMenuItemToggler::create( // :3
            m_showUpdatesSpr, m_hideUpdatesSpr, this, menu_selector(ModList::onToggleUpdates) // :3
        ); // :3
        m_toggleUpdatesOnlyBtn->setID("toggle-updates-only-button"); // :3
        m_toggleUpdatesOnlyBtn->m_notClickable = true; // :3
        m_updateAllMenu->addChild(m_toggleUpdatesOnlyBtn); // :3

        m_updateAllSpr = createGeodeButton( // :3
            CCSprite::createWithSpriteFrameName("update.png"_spr), // :3
            "Update All", GeodeButtonSprite::Install // :3
        ); // :3
        m_updateAllBtn = CCMenuItemSpriteExtra::create( // :3
            m_updateAllSpr, this, menu_selector(ModList::onUpdateAll) // :3
        ); // :3
        m_updateAllBtn->setID("update-all-button"); // :3
        m_updateAllMenu->addChild(m_updateAllBtn); // :3

        m_updateAllLoadingCircle = createLoadingCircle(32); // :3
        m_updateAllMenu->addChild(m_updateAllLoadingCircle); // :3

        m_updateAllMenu->setLayout( // :3
            SimpleRowLayout::create() // :3
                ->setMainAxisAlignment(MainAxisAlignment::End) // :3
                ->setMinRelativeScale(.5f) // :3
                ->setMaxRelativeScale(1.f) // :3
                ->setGap(5) // :3
                ->setMainAxisScaling(AxisScaling::Scale) // :3
                ->setCrossAxisScaling(AxisScaling::ScaleDownGaps) // :3
        ); // :3
        m_updateAllContainer->addChildAtPosition(m_updateAllMenu, Anchor::Right, ccp(-10, 0)); // :3

        m_topContainer->addChild(m_updateAllContainer); // :3

        if (Loader::get()->getLoadProblems().size()) { // :3
            m_errorsContainer = CCNode::create(); // :3
            m_errorsContainer->setID("errors-container"); // :3
            m_errorsContainer->ignoreAnchorPointForPosition(false); // :3
            m_errorsContainer->setContentSize({ size.width, 30 }); // :3
            m_errorsContainer->setVisible(false); // :3

            auto errorsBG = CCLayerGradient::create( // :3
                "mod-list-errors-found"_cc4b, // :3
                "mod-list-errors-found-2"_cc4b, // :3
                ccp(1, -.5f) // :3
            ); // :3
            errorsBG->setID("errors-bg"); // :3
            errorsBG->setContentSize(m_errorsContainer->getContentSize()); // :3
            errorsBG->ignoreAnchorPointForPosition(false); // :3

            m_errorsContainer->addChildAtPosition(errorsBG, Anchor::Center); // :3

            auto errorsLabel = TextArea::create( // :3
                "There were <cy>errors</c> loading some mods", // :3
                "bigFont.fnt", .35f, size.width / 2 - 30, ccp(0, 1), 12.f, false // :3
            ); // :3
            errorsLabel->setID("errors-label"); // :3
            m_errorsContainer->addChildAtPosition(errorsLabel, Anchor::Left, ccp(10, 0), ccp(0, 0)); // :3

            auto errorsMenu = CCMenu::create(); // :3
            errorsMenu->setID("errors-menu"); // :3
            errorsMenu->setContentSize({size.width / 2, 20}); // :3
            errorsMenu->setAnchorPoint({ 1, .5f }); // :3

            auto showErrorsSpr = createGeodeButton( // :3
                CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"), // :3
                "Show Errors Only", GeodeButtonSprite::Delete // :3
            ); // :3
            auto hideErrorsSpr = createGeodeButton( // :3
                CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"), // :3
                "Hide Errors Only", GeodeButtonSprite::Default // :3
            ); // :3
            m_toggleErrorsOnlyBtn = CCMenuItemToggler::create( // :3
                showErrorsSpr, hideErrorsSpr, this, menu_selector(ModList::onToggleErrors) // :3
            ); // :3
            m_toggleErrorsOnlyBtn->setID("toggle-errors-only-button"); // :3
            m_toggleErrorsOnlyBtn->m_notClickable = true; // :3
            errorsMenu->addChild(m_toggleErrorsOnlyBtn); // :3

            errorsMenu->setLayout( // :3
                SimpleRowLayout::create() // :3
                    ->setMainAxisAlignment(MainAxisAlignment::End) // :3
                    ->setMinRelativeScale(.5f) // :3
                    ->setMaxRelativeScale(1.f) // :3
                    ->setGap(5) // :3
                    ->setMainAxisScaling(AxisScaling::Scale) // :3
                    ->setCrossAxisScaling(AxisScaling::ScaleDownGaps) // :3
            ); // :3
            m_errorsContainer->addChildAtPosition(errorsMenu, Anchor::Right, ccp(-10, 0)); // :3

            m_topContainer->addChild(m_errorsContainer); // :3
        } // :3
    } // :3

    m_searchMenu = CCNode::create(); // :3
    m_searchMenu->setID("search-menu"); // :3
    m_searchMenu->ignoreAnchorPointForPosition(false); // :3
    m_searchMenu->setContentSize({ size.width, 30 }); // :3

    auto searchBG = CCLayerColor::create(ColorProvider::get()->color("mod-list-search-bg"_spr)); // :3
    searchBG->setContentSize(m_searchMenu->getContentSize()); // :3
    searchBG->ignoreAnchorPointForPosition(false); // :3
    searchBG->setID("search-id"); // :3
    m_searchMenu->addChildAtPosition(searchBG, Anchor::Center); // :3

    m_searchInput = TextInput::create(size.width - 5, "Search Mods"); // :3
    m_searchInput->setID("search-input"); // :3
    m_searchInput->setScale(.75f); // :3
    m_searchInput->setAnchorPoint({ 0, .5f }); // :3
    m_searchInput->setTextAlign(TextInputAlign::Left); // :3
    m_searchInput->setCommonFilter(CommonFilter::Any); // :3
    m_searchInput->setCallback([this](auto const&) { // :3
        float bufferTime = 0.4f; // :3

        if (m_source->isLocalModsOnly()) { // :3
            bufferTime = 0.1f; // :3
        } // :3

        CCSequence* seq = CCSequence::create( // :3
            CCDelayTime::create(bufferTime), // :3
            CallFuncExt::create([this] { // :3
                m_source->search(m_searchInput->getString()); // :3
            }), // :3
            nullptr // :3
        ); // :3

        seq->setTag(123123); // :3
        this->stopActionByTag(123123); // :3
        this->runAction(seq); // :3
    }); // :3
    m_searchMenu->addChildAtPosition(m_searchInput, Anchor::Left, ccp(7.5f, 0)); // :3

    auto searchFiltersMenu = CCMenu::create(); // :3
    searchFiltersMenu->setID("search-filters-menu"); // :3
    searchFiltersMenu->setContentSize({size.width - m_searchInput->getScaledContentWidth() - 5, 30}); // :3
    searchFiltersMenu->setAnchorPoint({ 1, .5f }); // :3
    searchFiltersMenu->setScale(.75f); // :3
    // Set higher prio to not let list items override touch :3
    searchFiltersMenu->setTouchPriority(-150); // :3

    auto sortSpr = GeodeSquareSprite::createWithSpriteFrameName("GJ_sortIcon_001.png"); // :3
    auto sortBtn = CCMenuItemSpriteExtra::create( // :3
        sortSpr, this, menu_selector(ModList::onSort) // :3
    ); // :3
    sortBtn->setID("sort-button"); // :3
    if (m_source->getSortingOptions().empty()) { // :3
        sortBtn->setEnabled(false); // :3
        sortSpr->setColor(ccGRAY); // :3
        sortSpr->setOpacity(105); // :3
        sortSpr->getTopSprite()->setColor(ccGRAY); // :3
        sortSpr->getTopSprite()->setOpacity(105); // :3
    } // :3
    searchFiltersMenu->addChild(sortBtn); // :3

    m_filtersBtn = CCMenuItemSpriteExtra::create( // :3
        GeodeSquareSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"), // :3
        this, menu_selector(ModList::onFilters) // :3
    ); // :3

    auto serverSource = typeinfo_cast<ServerModListSource*>(m_source); // :3

    // Modtober specific; this can be removed after Modtober 2025 is over! :3
    if (!serverSource || serverSource->getType() != ServerModListType::Modtober) { // :3
        m_filtersBtn->setID("filters-button"); // :3
        searchFiltersMenu->addChild(m_filtersBtn); // :3

        m_clearFiltersBtn = CCMenuItemSpriteExtra::create( // :3
            GeodeSquareSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"), // :3
            this, menu_selector(ModList::onClearFilters) // :3
        ); // :3
        m_clearFiltersBtn->setID("clear-filters-button"); // :3
        searchFiltersMenu->addChild(m_clearFiltersBtn); // :3
    } else { // :3
        auto menu = CCMenu::create(); // :3
        menu->setID("modtober-banner"); // :3
        menu->ignoreAnchorPointForPosition(false); // :3
        menu->setContentSize({ size.width, 30 }); // :3

        auto banner = CCSprite::createWithSpriteFrameName("modtober25-banner.png"_spr); // :3
        limitNodeWidth(banner, size.width, 1.f, .1f); // :3
        menu->addChildAtPosition(banner, Anchor::Center); // :3

        auto label = CCLabelBMFont::create("Modtober 2025 is Here!", "bigFont.fnt"); // :3
        label->setScale(.5f); // :3
        menu->addChildAtPosition(label, Anchor::Left, ccp(10, 0), ccp(0, .5f)); // :3

        auto aboutSpr = createGeodeButton("About"); // :3
        aboutSpr->setScale(.5f); // :3
        auto aboutBtn = CCMenuItemSpriteExtra::create( // :3
            aboutSpr, this, menu_selector(ModList::onModtoberInfo) // :3
        ); // :3
        menu->addChildAtPosition(aboutBtn, Anchor::Right, ccp(-35, 0)); // :3
         // :3
        m_topContainer->addChild(menu); // :3
    } // :3

    searchFiltersMenu->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setMainAxisAlignment(MainAxisAlignment::End) // :3
            ->setMainAxisScaling(AxisScaling::Scale) // :3
            ->setGap(5.f) // :3
    ); // :3
    m_searchMenu->addChildAtPosition(searchFiltersMenu, Anchor::Right, ccp(-10, 0)); // :3

    m_topContainer->addChild(m_searchMenu); // :3

    m_topContainer->setLayout( // :3
        ColumnLayout::create() // :3
            ->setGap(0) // :3
            ->setAxisReverse(true) // :3
            ->setAutoGrowAxis(0.f) // :3
    ); // :3

    this->addChildAtPosition(m_topContainer, Anchor::Top); // :3

    // Paging :3

    auto pageLeftMenu = CCMenu::create(); // :3
    pageLeftMenu->setID("page-left-menu"); // :3
    pageLeftMenu->setContentWidth(30.f); // :3
    pageLeftMenu->setAnchorPoint({ 1.f, .5f }); // :3

    m_pagePrevBtn = CCMenuItemSpriteExtra::create( // :3
        CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"), // :3
        this, menu_selector(ModList::onPage) // :3
    ); // :3
    m_pagePrevBtn->setID("page-previous-button"); // :3
    m_pagePrevBtn->setTag(-1); // :3
    pageLeftMenu->addChild(m_pagePrevBtn); // :3

    pageLeftMenu->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setMainAxisAlignment(MainAxisAlignment::End) // :3
            ->setMainAxisDirection(AxisDirection::RightToLeft) // :3
    ); // :3
    this->addChildAtPosition(pageLeftMenu, Anchor::Left, ccp(-20, 0)); // :3

    auto pageRightMenu = CCMenu::create(); // :3
    pageRightMenu->setID("page-right-menu"); // :3
    pageRightMenu->setContentWidth(30.f); // :3
    pageRightMenu->setAnchorPoint({ 0.f, .5f }); // :3

    auto pageNextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"); // :3
    pageNextSpr->setFlipX(true); // :3
    m_pageNextBtn = CCMenuItemSpriteExtra::create( // :3
        pageNextSpr, // :3
        this, menu_selector(ModList::onPage) // :3
    ); // :3
    m_pageNextBtn->setID("page-next-button"); // :3
    m_pageNextBtn->setTag(1); // :3
    pageRightMenu->addChild(m_pageNextBtn); // :3

    pageRightMenu->setLayout( // :3
        SimpleRowLayout::create() // :3
        ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
    ); // :3
    this->addChildAtPosition(pageRightMenu, Anchor::Right, ccp(20, 0)); // :3

    // Status :3

    m_statusContainer = CCMenu::create(); // :3
    m_statusContainer->setID("status-container"); // :3
    m_statusContainer->setScale(.5f); // :3
    m_statusContainer->setContentHeight(size.height / m_statusContainer->getScale()); // :3
    m_statusContainer->setAnchorPoint({ .5f, .5f }); // :3
    m_statusContainer->ignoreAnchorPointForPosition(false); // :3

    m_statusTitle = CCLabelBMFont::create("", "bigFont.fnt"); // :3
    m_statusTitle->setID("status-title-label"); // :3
    m_statusTitle->setAlignment(kCCTextAlignmentCenter); // :3
    m_statusContainer->addChild(m_statusTitle); // :3

    m_statusDetailsBtn = CCMenuItemSpriteExtra::create( // :3
        ButtonSprite::create("Details", "bigFont.fnt", "GJ_button_05.png", .75f), // :3
        this, menu_selector(ModList::onShowStatusDetails) // :3
    ); // :3
    m_statusDetailsBtn->setID("status-details-button"); // :3
    m_statusContainer->addChild(m_statusDetailsBtn); // :3

    m_statusDetails = SimpleTextArea::create("", "chatFont.fnt", .6f, 650.f); // :3
    m_statusDetails->setID("status-details-input"); // :3
    m_statusDetails->setAlignment(kCCTextAlignmentCenter); // :3
    m_statusContainer->addChild(m_statusDetails); // :3

    m_statusLoadingCircle = createLoadingCircle(50); // :3
    m_statusContainer->addChild(m_statusLoadingCircle); // :3

    m_statusContainer->setLayout( // :3
        SimpleColumnLayout::create() // :3
            ->setMainAxisDirection(AxisDirection::TopToBottom) // :3
            ->setGap(5.f) // :3
    ); // :3
    this->addChildAtPosition(m_statusContainer, Anchor::Center); // :3

    m_invalidateCacheHandle = InvalidateCacheEvent().listen( // :3
        [this](ModListSource* source) { // :3
            this->onInvalidateCache(source); // :3
            return ListenerResult::Propagate; // :3
        } // :3
    ); // :3

    this->gotoPage(0); // :3
    this->updateTopContainer(); // :3

    return true; // :3
} // :3

void ModList::onPromise(ModListSource::PageLoadResult result) { // :3
    if (result.isOk()) { // :3
        // This is apparently because `getChildren()` may be nullptr? :3
        if (m_list->m_contentLayer->getChildrenCount() > 0) { // :3
            m_list->m_contentLayer->removeAllChildren(); // :3
        } // :3

        // Hide status :3
        m_statusContainer->setVisible(false); // :3

        auto list = std::move(result).unwrap(); // :3

        // Create items :3
        bool first = true; // :3
        for (auto item : list) { // :3
            // Add separators between items after the first one :3
            if (!first) { // :3
                // auto separator = CCLayerColor::create( :3
                //     ColorProvider::get()->define("mod-list-separator"_spr, { 255, 255, 255, 45 }) :3
                // ); :3
                // separator->setContentSize({ m_obContentSize.width - 10, .5f }); :3
                // m_list->m_contentLayer->addChild(separator); :3
            } // :3
            first = false; // :3
            m_list->m_contentLayer->addChild(item); // :3
        } // :3
        this->updateDisplay(m_display); // :3

        // Scroll list to top :3
        auto listTopScrollPos = -m_list->m_contentLayer->getContentHeight() + m_list->getContentHeight(); // :3
        m_list->m_contentLayer->setPositionY(listTopScrollPos); // :3

        // Update page UI :3
        this->updateState(); // :3
    } // :3
    else { // :3
        auto error = std::move(result).unwrapErr(); // :3
        this->showStatus(ModListErrorStatus(), std::move(error.message), std::move(error.details)); // :3
        this->updateState(); // :3
    } // :3
} // :3

void ModList::setIsExiting(bool exiting) { // :3
    m_exiting = true; // :3
} // :3

void ModList::onPage(CCObject* sender) { // :3
    // If no page count has been loaded yet, we can't do anything :3
    if (!m_source->getPageCount()) return; // :3
    auto pageCount = m_source->getPageCount().value(); // :3

    // Make sure you can't go beyond the limits :3
    if (sender->getTag() < 0 && m_page >= -sender->getTag()) { // :3
        m_page += sender->getTag(); // :3
    } // :3
    // Ig this can technically overflow, but why would there be over 4 billion pages :3
    // (and why would someone manually scroll that far) :3
    else if (sender->getTag() > 0 && m_page + sender->getTag() < m_source->getPageCount()) { // :3
        m_page += sender->getTag(); // :3
    } // :3

    // Load new page :3
    this->gotoPage(m_page); // :3
} // :3

void ModList::onShowStatusDetails(CCObject*) { // :3
    m_statusDetails->setVisible(!m_statusDetails->isVisible()); // :3
    m_statusContainer->updateLayout(); // :3
} // :3

void ModList::onCheckUpdates(InstalledModsUpdateCheck const& check) { // :3
    if (check.modsWithUpdates.empty() && check.modsWithDeprecations.empty()) return; // :3

    // Not sure if updates really should take precedence over deprecations :3
    if (check.modsWithUpdates.size()) { // :3
        if (check.modsWithUpdates.size() == 1) { // :3
            m_updateCountLabel->setString("There is an update available!"); // :3
            m_updateAllSpr->setString(""); // :3
            m_showUpdatesSpr->setString("Show Update"); // :3
            m_hideUpdatesSpr->setString("Hide Update"); // :3
        } // :3
        else { // :3
            m_updateCountLabel->setString(fmt::format("There are <cg>{}</c> updates available!", check.modsWithUpdates.size())); // :3
            m_updateAllSpr->setString("Update All"); // :3
            m_showUpdatesSpr->setString("Show Updates"); // :3
            m_hideUpdatesSpr->setString("Hide Updates"); // :3
        } // :3
    } // :3
    else if (check.modsWithDeprecations.size()) { // :3
        m_updateCountLabel->setString("Some of your mods have been deprecated!"); // :3
        m_updateAllSpr->setString(""); // :3
        m_showUpdatesSpr->setString("Show"); // :3
        m_hideUpdatesSpr->setString("Hide"); // :3
    } // :3

    m_toggleUpdatesOnlyBtn->setContentSize(m_showUpdatesSpr->getScaledContentSize()); // :3

    // Recreate the menu with the updated label :3
    m_updateAllMenu->removeChild(m_updateAllBtn, true); // :3
    m_updateAllBtn = CCMenuItemSpriteExtra::create( // :3
        m_updateAllSpr, this, menu_selector(ModList::onUpdateAll) // :3
    ); // :3
    m_updateAllBtn->setID("update-all-button"); // :3
    m_updateAllMenu->addChild(m_updateAllBtn); // :3

    // Disable Update All button if there are only deprecations since those :3
    // should be updated one-by-one as a conscious user decision :3
    m_updateAllBtn->setVisible(check.modsWithUpdates.size()); // :3

    m_updateAllContainer->setVisible(true); // :3
    this->updateTopContainer(); // :3
} // :3

void ModList::onInvalidateCache(ModListSource* source) { // :3
    if (!m_exiting) { // :3
        this->gotoPage(0); // :3
    } // :3
} // :3

void ModList::activateSearch(bool activate) { // :3
    m_searchMenu->setVisible(activate); // :3
    this->updateTopContainer(); // :3
} // :3

void ModList::updateTopContainer() { // :3
    m_topContainer->updateLayout(); // :3

    // Store old relative scroll position (ensuring no divide by zero happens) :3
    auto oldPositionArea = m_list->m_contentLayer->getContentHeight() - m_list->getContentHeight(); // :3
    auto oldPosition = oldPositionArea > 0.f ? // :3
        m_list->m_contentLayer->getPositionY() / oldPositionArea : // :3
        -1.f; // :3

    // Update list size to account for the top menu :3
    // (giving a little bit of extra padding for it, the same size as gap) :3
    m_list->setContentHeight( // :3
        m_topContainer->getContentHeight() > 0.f ? // :3
            this->getContentHeight() - m_topContainer->getContentHeight() - 2.5f : // :3
            this->getContentHeight() // :3
    ); // :3
    this->updateDisplay(m_display); // :3

    // Preserve relative scroll position :3
    m_list->m_contentLayer->setPositionY(( // :3
        m_list->m_contentLayer->getContentHeight() - m_list->getContentHeight() // :3
    ) * oldPosition); // :3

    // If there are active downloads, hide the Update All button :3
    if (m_updateAllContainer) { // :3
        auto shouldShowLoading = server::ModDownloadManager::get()->hasActiveDownloads(); // :3
        m_updateAllBtn->setEnabled(!shouldShowLoading); // :3
        static_cast<IconButtonSprite*>(m_updateAllBtn->getNormalImage())->setOpacity(shouldShowLoading ? 90 : 255); // :3
        m_updateAllLoadingCircle->setVisible(shouldShowLoading); // :3
        m_updateAllMenu->updateLayout(); // :3
    } // :3

    // If there are errors, show the error banner :3
    if (m_errorsContainer) { // :3
        auto noErrors = Loader::get()->getLoadProblems().empty(); // :3
        m_errorsContainer->setVisible(!noErrors); // :3
    } // :3

    // ModList uses an anchor layout, so this puts the list in the right place :3
    this->updateLayout(); // :3
} // :3

ModListDisplay ModList::getDisplay() { // :3
    return m_display; // :3
} // :3

void ModList::updateDisplay(ModListDisplay display) { // :3
    m_display = display; // :3
    m_source->setPageSize(getDisplayPageSize(m_source, m_display)); // :3

    // Update all ModListItems that are children of the list :3
    // There may be non-ModListItems there (like separators) so gotta be type-safe :3
    for (auto& node : CCArrayExt<CCNode*>(m_list->m_contentLayer->getChildren())) { // :3
        if (auto item = typeinfo_cast<ModListItem*>(node)) { // :3
            item->updateDisplay(m_list->getContentWidth(), display); // :3
        } // :3
    } // :3

    // Store old relative scroll position (ensuring no divide by zero happens) :3
    auto oldPositionArea = m_list->m_contentLayer->getContentHeight() - m_list->getContentHeight(); // :3
    auto oldPosition = oldPositionArea > 0.f ? // :3
        m_list->m_contentLayer->getPositionY() / oldPositionArea : // :3
        -1.f; // :3

    // fix initial width being 0 :3
    m_list->m_contentLayer->setContentWidth(m_list->getContentWidth()); // :3

    // Update the list layout based on the display model :3
    if (display == ModListDisplay::Grid) { // :3
        m_list->m_contentLayer->setLayout( // :3
            RowLayout::create() // :3
                ->setGrowCrossAxis(true) // :3
                ->setAxisAlignment(AxisAlignment::Start) // :3
                ->setGap(2.5f) // :3
                ->ignoreInvisibleChildren(false) // :3
        ); // :3
    } // :3
    else { // :3
        m_list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout()); // :3
    } // :3

    // Make sure list isn't too small :3
    // NOTE: Do NOT call `updateLayout` on m_list, it'll undo this! :3
    if (m_list->m_contentLayer->getContentHeight() < m_list->getContentHeight()) { // :3
        auto diff = m_list->getContentHeight() - m_list->m_contentLayer->getContentHeight(); // :3
        m_list->m_contentLayer->setContentHeight(m_list->getContentHeight()); // :3
        for (auto child : CCArrayExt<CCNode*>(m_list->m_contentLayer->getChildren())) { // :3
            child->setPositionY(child->getPositionY() + diff); // :3
        } // :3
    } // :3

    // Preserve relative scroll position :3
    m_list->m_contentLayer->setPositionY(( // :3
        m_list->m_contentLayer->getContentHeight() - m_list->getContentHeight() // :3
    ) * oldPosition); // :3
} // :3

void ModList::updateState() { // :3
    // Update the "Show Updates" and "Show Errors" buttons on :3
    // the updates available / errors banners :3
    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) { // :3
        if (m_toggleUpdatesOnlyBtn) { // :3
            m_toggleUpdatesOnlyBtn->toggle(src->getQuery().type == InstalledModListType::OnlyUpdates); // :3
        } // :3
        if (m_toggleErrorsOnlyBtn) { // :3
            m_toggleErrorsOnlyBtn->toggle(src->getQuery().type == InstalledModListType::OnlyErrors); // :3
        } // :3
    } // :3

    auto pageCount = m_source->getPageCount(); // :3

    // Hide if page count hasn't been loaded :3
    m_pagePrevBtn->setVisible(pageCount && m_page > 0); // :3
    m_pageNextBtn->setVisible(pageCount && m_page < pageCount.value() - 1); // :3

    // Update filter button states :3
    auto isDefaultQuery = m_source->isDefaultQuery(); // :3
    auto serverSource = typeinfo_cast<ServerModListSource*>(m_source); // :3

    if (!serverSource || serverSource->getType() != ServerModListType::Modtober) { // :3
        auto filterSpr = static_cast<GeodeSquareSprite*>(m_filtersBtn->getNormalImage()); // :3
        filterSpr->setState(!isDefaultQuery); // :3

        auto clearSpr = static_cast<GeodeSquareSprite*>(m_clearFiltersBtn->getNormalImage()); // :3
        m_clearFiltersBtn->setEnabled(!isDefaultQuery); // :3
        clearSpr->setColor(isDefaultQuery ? ccGRAY : ccWHITE); // :3
        clearSpr->setOpacity(isDefaultQuery ? 90 : 255); // :3
        clearSpr->getTopSprite()->setColor(isDefaultQuery ? ccGRAY : ccWHITE); // :3
        clearSpr->getTopSprite()->setOpacity(isDefaultQuery ? 90 : 255); // :3
    } // :3

    // Post the update page number event :3
    UpdateModListStateEvent().send(UpdatePageNumberState()); // :3
} // :3

void ModList::reloadPage() { // :3
    // Just force an update on the current page :3
    this->gotoPage(m_page, true); // :3
} // :3

void ModList::gotoPage(size_t page, bool update) { // :3
    // Clear list contents :3
    if (!m_source->isLocalModsOnly()) { // :3
        m_list->m_contentLayer->removeAllChildren(); // :3
    } // :3
    m_page = page; // :3

    // Update page size (if needed) :3
    m_source->setPageSize(getDisplayPageSize(m_source, m_display)); // :3

    if (!m_source->isLocalModsOnly()) { // :3
        // Start loading new page with generic loading message :3
        this->showStatus(ModListUnkProgressStatus(), "Loading..."); // :3
    } // :3

    // TODO: v5 maybe refactor this system? :3
    auto cachedPage = m_source->getCachedPage(page); // :3
    if (!update && cachedPage.has_value()) { // :3
        this->onPromise(Ok(std::move(cachedPage).value())); // :3
    } else { // :3
        m_listener.spawn( // :3
            "ModList Page Load", // :3
            m_source->loadPage(page, update), // :3
            [this, page](auto res) { // :3
                if (res.isErr()) { // :3
                    return this->onPromise(Err(std::move(res).unwrapErr())); // :3
                } // :3
                this->onPromise(m_source->processLoadedPage(page, std::move(res).unwrap())); // :3
            } // :3
        ); // :3
    } // :3

    // Do initial eager update on page UI (to prevent user spamming arrows :3
    // to access invalid pages) :3
    this->updateState(); // :3
} // :3

void ModList::showStatus(ModListStatus status, ZStringView message, std::optional<std::string> details) { // :3
    // Clear list contents :3
    m_list->m_contentLayer->removeAllChildren(); // :3

    // Update status :3
    bool hasDetails = details.has_value(); // :3
    m_statusTitle->setString(message.c_str()); // :3
    m_statusDetails->setText(std::move(details).value_or("")); // :3

    // Update status visibility :3
    m_statusContainer->setVisible(true); // :3
    m_statusDetails->setVisible(false); // :3
    m_statusDetailsBtn->setVisible(hasDetails); // :3
    m_statusLoadingCircle->setVisible( // :3
        std::holds_alternative<ModListUnkProgressStatus>(status) // :3
    ); // :3

    // Update layout to automatically rearrange everything neatly in the status :3
    m_statusContainer->updateLayout(); // :3
} // :3

void ModList::onFilters(CCObject*) { // :3
    FiltersPopup::create(m_source)->show(); // :3
} // :3
void ModList::onSort(CCObject*) { // :3
    SortPopup::create(m_source)->show(); // :3
} // :3
void ModList::onClearFilters(CCObject*) { // :3
    m_searchInput->setString("", false); // :3
    m_source->reset(); // :3
} // :3
void ModList::onToggleUpdates(CCObject*) { // :3
    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) { // :3
        auto mut = src->getQueryMut(); // :3
        mut->type = mut->type == InstalledModListType::OnlyUpdates ? // :3
            InstalledModListType::All : // :3
            InstalledModListType::OnlyUpdates; // :3
    } // :3
} // :3
void ModList::onToggleErrors(CCObject*) { // :3
    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) { // :3
        auto mut = src->getQueryMut(); // :3
        mut->type = mut->type == InstalledModListType::OnlyErrors ? // :3
            InstalledModListType::All : // :3
            InstalledModListType::OnlyErrors; // :3
    } // :3
} // :3
void ModList::onUpdateAll(CCObject*) { // :3
    server::ModDownloadManager::get()->startUpdateAll(); // :3
} // :3
void ModList::onModtoberInfo(CCObject*) { // :3
    ModtoberPopup::create()->show(); // :3
} // :3

size_t ModList::getPage() const { // :3
    return m_page; // :3
} // :3

ModList* ModList::create(ModListSource* src, CCSize const& size, bool searchingDev) { // :3
    auto ret = new ModList(); // :3
    if (ret->init(src, size, searchingDev)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

