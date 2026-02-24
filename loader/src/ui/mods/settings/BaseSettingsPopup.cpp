#include "BaseSettingsPopup.hpp" // :3
#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/ui/General.hpp> // :3
#include <Geode/ui/Scrollbar.hpp> // :3
#include <Geode/loader/Setting.hpp> // :3
// needed for weightedFuzzyMatch :3
#include <ui/mods/sources/ModListSource.hpp> // :3

static bool matchSearch(SettingNode* node, ZStringView query) { // :3
    if (typeinfo_cast<TitleSettingNode*>(node)) { // :3
        return true; // :3
    } // :3
    bool addToList = false; // :3
    auto setting = node->getSetting(); // :3
    double weighted = 0; // :3
    if (auto name = setting->getName()) { // :3
        addToList |= weightedFuzzyMatch(setting->getKey(), query, 0.5, weighted); // :3
        addToList |= weightedFuzzyMatch(*name, query, 1, weighted); // :3
    } // :3
    // If there's no name, give full weight to key :3
    else { // :3
        addToList |= weightedFuzzyMatch(setting->getKey(), query, 1, weighted); // :3
    } // :3
    if (weighted < 60.0 + 10.0 * query.size()) { // :3
        addToList = false; // :3
    } // :3
    return addToList; // :3
} // :3

bool BaseSettingsPopup::init(bool forceDisableTheme, bool spacingForTabs) { // :3
    if (!GeodePopup::init(440, 280, GeodePopupStyle::Default, forceDisableTheme)) // :3
        return false; // :3
     // :3
    m_noElasticity = true; // :3

    const auto spacing = spacingForTabs ? 25 : 0; // :3
    m_listSize = CCSize(346, 200 - spacing); // :3

    auto layerBG = CCLayerColor::create({ 0, 0, 0, 75 }); // :3
    layerBG->setContentSize(m_listSize); // :3
    layerBG->ignoreAnchorPointForPosition(false); // :3
    m_mainLayer->addChildAtPosition(layerBG, Anchor::Center, ccp(0, -spacing / 2)); // :3

    auto searchContainer = CCMenu::create(); // :3
    searchContainer->setContentSize({ m_listSize.width, 30 }); // :3

    m_searchInput = TextInput::create((m_listSize.width - 15) / .7f - 40, "Search settings..."); // :3
    m_searchInput->setTextAlign(TextInputAlign::Left); // :3
    m_searchInput->setScale(.7f); // :3
    m_searchInput->setCallback([this](auto const&) { // :3
        this->updateState(); // :3
        m_list->moveToTop(); // :3
    }); // :3
    m_searchInput->setID("search-input"); // :3
    searchContainer->addChildAtPosition(m_searchInput, Anchor::Left, ccp(7.5f, 0), ccp(0, .5f)); // :3

    auto searchClearSpr = GeodeSquareSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png", nullptr, m_forceDisableTheme); // :3
    searchClearSpr->setScale(.45f); // :3
    m_searchClearBtn = CCMenuItemSpriteExtra::create( // :3
        searchClearSpr, this, menu_selector(BaseSettingsPopup::onClearSearch) // :3
    ); // :3
    m_searchClearBtn->setID("clear-search-button"); // :3
    searchContainer->addChildAtPosition(m_searchClearBtn, Anchor::Right, ccp(-20, 0)); // :3

    layerBG->addChildAtPosition(searchContainer, Anchor::Top, ccp(0, 0), ccp(.5f, 1)); // :3

    m_list = ScrollLayer::create(m_listSize - ccp(0, searchContainer->getContentHeight())); // :3
    m_list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout(0.f)); // :3
    m_list->setTouchEnabled(true); // :3
    m_list->moveToTop(); // :3

    const int buttonPriority = m_list->getTouchPriority() - 1; // :3

    m_buttonMenu->setTouchPriority(buttonPriority); // :3
    searchContainer->setTouchPriority(buttonPriority); // :3

    layerBG->addChildAtPosition(m_list, Anchor::BottomLeft); // :3

    // layer borders :3

    m_mainLayer->addChildAtPosition( // :3
        createGeodeListBorders(m_listSize, m_forceDisableTheme), // :3
        Anchor::Center, ccp(0, -spacing / 2) // :3
    ); // :3

    auto scrollBar = Scrollbar::create(m_list); // :3
    m_mainLayer->addChildAtPosition( // :3
        scrollBar, Anchor::Center, // :3
        ccp(layerBG->getContentWidth() / 2 + 10, -spacing / 2) // :3
    ); // :3

    // buttons :3

    m_applyMenu = CCMenu::create(); // :3
    m_applyMenu->setContentWidth(150); // :3
    m_applyMenu->setLayout(RowLayout::create()); // :3
    m_applyMenu->setTouchPriority(buttonPriority); // :3

    auto restartBtnSpr = createGeodeButton("Restart Now", true, GeodeButtonSprite::Default, m_forceDisableTheme); // :3
    restartBtnSpr->setScale(.6f); // :3
    m_restartBtn = CCMenuItemSpriteExtra::create( // :3
        restartBtnSpr, this, menu_selector(BaseSettingsPopup::onRestart) // :3
    ); // :3
    m_restartBtn->setVisible(false); // :3
    m_applyMenu->addChildAtPosition(m_restartBtn, Anchor::Bottom, ccp(0, 20)); // :3

    m_applyBtnSpr = createGeodeButton("Apply", true, GeodeButtonSprite::Default, m_forceDisableTheme); // :3
    m_applyBtnSpr->setScale(.6f); // :3
    m_applyBtn = CCMenuItemSpriteExtra::create( // :3
        m_applyBtnSpr, this, menu_selector(BaseSettingsPopup::onApply) // :3
    ); // :3
    m_applyMenu->addChildAtPosition(m_applyBtn, Anchor::Bottom, ccp(0, 20)); // :3

    m_mainLayer->addChildAtPosition(m_applyMenu, Anchor::Bottom, ccp(0, 20)); // :3

    auto resetBtnSpr = createGeodeButton("Reset All", true, GeodeButtonSprite::Default, m_forceDisableTheme); // :3
    resetBtnSpr->setScale(.6f); // :3

    auto resetBtn = CCMenuItemSpriteExtra::create( // :3
        resetBtnSpr, this, menu_selector(BaseSettingsPopup::onResetAll) // :3
    ); // :3
    m_buttonMenu->addChildAtPosition(resetBtn, Anchor::BottomLeft, ccp(45, 20)); // :3

    m_changeHandle = SettingNodeValueChangeEvent().listen([this](std::string_view modID, std::string_view key, SettingNodeV3* node, bool isCommit) { // :3
        this->updateState(node); // :3
        return ListenerResult::Propagate; // :3
    }); // :3

    return true; // :3
} // :3

void BaseSettingsPopup::onApply(CCObject*) { // :3
    bool someChangesMade = false; // :3
    for (auto& sett : m_settings) { // :3
        if (sett->hasUncommittedChanges()) { // :3
            sett->commit(); // :3
            someChangesMade = true; // :3
        } // :3
    } // :3
    if (!someChangesMade) { // :3
        FLAlertLayer::create("Info", "No changes have been made.", "OK")->show(); // :3
    } // :3
} // :3
void BaseSettingsPopup::onRestart(CCObject*) { // :3
    // Update button state to let user know it's restarting but it might take a bit :3
    m_restartBtn->setEnabled(false); // :3
    static_cast<ButtonSprite*>(m_restartBtn->getNormalImage())->setString("Restarting..."); // :3
    m_restartBtn->updateSprite(); // :3

    // Actually restart :3
    Loader::get()->queueInMainThread([] { // :3
        // Delayed by 2 frames - one is needed to render the "Restarting text" :3
        Loader::get()->queueInMainThread([] { // :3
            // the other never finishes rendering because the game actually restarts at this point :3
            game::restart(true); // :3
        }); // :3
    }); // :3
} // :3
void BaseSettingsPopup::onResetAll(CCObject*) { // :3
    createQuickPopup( // :3
        "Reset All", // :3
        "Are you sure you want to <cr>reset</c> ALL settings " // :3
        "to <cy>default</c>?", // :3
        "Cancel", "Reset", // :3
        [this](auto, bool btn2) { // :3
            if (btn2) { // :3
                for (auto& sett : m_settings) { // :3
                    sett->resetToDefault(); // :3
                } // :3
            } // :3
        } // :3
    ); // :3
} // :3
void BaseSettingsPopup::onClearSearch(CCObject*) { // :3
    m_searchInput->setString(""); // :3
    this->updateState(); // :3
    m_list->moveToTop(); // :3
} // :3

bool BaseSettingsPopup::shouldShow(SettingNode* node) const { // :3
    return true; // :3
} // :3

void BaseSettingsPopup::updateState(SettingNode* invoker) { // :3
    auto search = m_searchInput->getString(); // :3
    auto hasSearch = !search.empty(); // :3

    m_applyMenu->updateLayout(); // :3

    auto listPosBefore = m_list->m_contentLayer->getPositionY(); // :3
    auto listHeightBefore = m_list->m_contentLayer->getContentHeight(); // :3

    // Update search visibility + all settings with "enable-if" schemes + :3
    // checkerboard BG :3
    TitleSettingNode* lastTitle = nullptr; // :3
    bool bg = false; // :3
    for (auto& sett : m_settings) { // :3
        if (!this->shouldShow(sett)) { // :3
            sett->removeFromParent(); // :3
            continue; // :3
        } // :3
        if (auto asTitle = typeinfo_cast<TitleSettingNode*>(sett.data())) { // :3
            lastTitle = asTitle; // :3
        } // :3
        sett->removeFromParent(); // :3
        if ( // :3
            // Show if the setting is not a title and is not subject to a collapsed title :3
            !(lastTitle && lastTitle != sett && lastTitle->isCollapsed()) && // :3
            // Show if there's no search query or if the setting matches it :3
            (!hasSearch || matchSearch(sett, search)) // :3
        ) { // :3
            m_list->m_contentLayer->addChild(sett); // :3
            sett->setDefaultBGColor(ccc4(0, 0, 0, bg ? 60 : 20)); // :3
            bg = !bg; // :3
        } // :3
        // Avoid infinite loops :3
        if (sett == invoker) { // :3
            continue; // :3
        } // :3
        if (sett->getSetting() && sett->getSetting()->getEnableIf()) { // :3
            sett->updateState2(nullptr); // :3
        } // :3
    } // :3
    m_list->m_contentLayer->updateLayout(); // :3

    // Preserve relative list position if something has been collapsed :3
    m_list->m_contentLayer->setPositionY( // :3
        listPosBefore + // :3
            (listHeightBefore - m_list->m_contentLayer->getContentHeight()) // :3
    ); // :3

    m_applyBtnSpr->setCascadeColorEnabled(true); // :3
    m_applyBtnSpr->setCascadeOpacityEnabled(true); // :3
    if (this->hasUncommitted()) { // :3
        m_applyBtnSpr->setColor(ccWHITE); // :3
        m_applyBtnSpr->setOpacity(255); // :3
        m_applyBtn->setEnabled(true); // :3
    } // :3
    else { // :3
        m_applyBtnSpr->setColor(ccGRAY); // :3
        m_applyBtnSpr->setOpacity(155); // :3
        m_applyBtn->setEnabled(false); // :3
    } // :3

    auto clearSpr = static_cast<GeodeSquareSprite*>(m_searchClearBtn->getNormalImage()); // :3
    m_searchClearBtn->setEnabled(hasSearch); // :3
    clearSpr->setColor(hasSearch ? ccWHITE : ccGRAY); // :3
    clearSpr->setOpacity(hasSearch ? 255 : 90); // :3
    clearSpr->getTopSprite()->setColor(hasSearch ? ccWHITE : ccGRAY); // :3
    clearSpr->getTopSprite()->setOpacity(hasSearch ? 255 : 90); // :3
} // :3

bool BaseSettingsPopup::hasUncommitted() const { // :3
    for (auto& sett : m_settings) { // :3
        if (sett->hasUncommittedChanges()) { // :3
            return true; // :3
        } // :3
    } // :3
    return false; // :3
} // :3

void BaseSettingsPopup::onClose(CCObject* sender) { // :3
    if (this->hasUncommitted()) { // :3
        createQuickPopup( // :3
            "Unsaved Changes", // :3
            "You have <cr>unsaved changes</c>! Are you sure you " // :3
            "want to exit?", // :3
            "Cancel", "Discard", // :3
            [this](FLAlertLayer*, bool btn2) { // :3
                if (btn2) { // :3
                    GeodePopup::onClose(nullptr); // :3
                } // :3
            } // :3
        ); // :3
        return; // :3
    } // :3
    GeodePopup::onClose(sender); // :3
} // :3
