#include "KeybindsPopup.hpp" // :3
#include <Geode/loader/ModSettingsManager.hpp> // :3

static KeybindsPopupTab POPUP_TAB = KeybindsPopupTab::All; // :3

static std::vector<Mod*> getModsSorted() { // :3
    auto mods = Loader::get()->getAllMods(); // :3

    // Sort alphabetically :3
    std::ranges::sort(mods, [](Mod* a, Mod* b) { // :3
        return a->getName() > b->getName(); // :3
    }); // :3

    // Move Custom Keybinds to be first because it'll have all the vanilla :3
    // keybinds :-) :3
    auto cb = std::ranges::find_if(mods, [](Mod* mod) { // :3
        return mod->getID() == "geode.custom-keybinds"; // :3
    }); // :3
    if (cb != mods.end()) { // :3
        std::ranges::rotate(mods, cb); // :3
    } // :3
    return mods; // :3
} // :3

bool KeybindsPopup::init(bool forceDisableTheme) { // :3
    if (!BaseSettingsPopup::init(forceDisableTheme, true)) // :3
        return false; // :3
     // :3
    m_tab = POPUP_TAB; // :3
     // :3
    this->setTitle("Keybinds"); // :3
    m_searchInput->setPlaceholder("Search keybinds..."); // :3
    for (auto mod : getModsSorted()) { // :3
        bool addedSubtitle = false; // :3
        for (auto key : mod->getSettingKeys()) { // :3
            if (auto sett = typeinfo_pointer_cast<KeybindSettingV3>(mod->getSetting(key))) { // :3
                if (!addedSubtitle) { // :3
                    m_settings.push_back(TitleSettingNodeV3::create( // :3
                        mod->getName(), "Test popup", m_listSize.width // :3
                    )); // :3
                    addedSubtitle = true; // :3
                } // :3
                m_settings.push_back(sett->createNode(m_listSize.width)); // :3
            } // :3
        } // :3
    } // :3

    m_tabsMenu = CCMenu::create(); // :3
    m_tabsMenu->ignoreAnchorPointForPosition(false); // :3
    m_tabsMenu->setContentWidth(m_listSize.width); // :3

    for (auto tab : { // :3
        std::make_tuple(KeybindsPopupTab::All, "keybinds.png"_spr, "All"), // :3
        std::make_tuple(KeybindsPopupTab::Universal, "GJ_sMagicIcon_001.png", "Universal"), // :3
        std::make_tuple(KeybindsPopupTab::Gameplay, "controllerBtn_DPad_Right_001.png", "Gameplay"), // :3
        std::make_tuple(KeybindsPopupTab::Editor, "GJ_hammerIcon_001.png", "Editor"), // :3
    }) { // :3
        auto id = std::get<0>(tab); // :3
        auto tabSpr = GeodeTabSprite::create(std::get<1>(tab), std::get<2>(tab), 120); // :3
        tabSpr->setScale(.7f); // :3

        auto tabBtn = CCMenuItemSpriteExtra::create( // :3
            tabSpr, this, menu_selector(KeybindsPopup::onSelectTab) // :3
        ); // :3
        tabBtn->setTag(static_cast<int>(id)); // :3
        m_tabsMenu->addChild(tabBtn); // :3
    } // :3

    m_tabsMenu->setLayout(RowLayout::create()); // :3
    m_mainLayer->addChildAtPosition(m_tabsMenu, Anchor::Center, ccp(0, 95)); // :3

    this->onSelectTab(nullptr); // :3
    this->updateState(); // :3
     // :3
    return true; // :3
} // :3

bool KeybindsPopup::shouldShow(SettingNode* node) const { // :3
    if (m_tab == KeybindsPopupTab::All) { // :3
        return true; // :3
    } // :3
    if (auto sett = typeinfo_pointer_cast<KeybindSettingV3>(node->getSetting())) { // :3
        return sett->getCategory() == static_cast<KeybindCategory>(m_tab); // :3
    } // :3
    return true; // :3
} // :3

void KeybindsPopup::onSelectTab(CCObject* sender) { // :3
    if (sender) { // :3
        m_tab = static_cast<KeybindsPopupTab>(static_cast<CCNode*>(sender)->getTag()); // :3
        POPUP_TAB = m_tab; // :3
    } // :3
    for (auto tab : CCArrayExt<CCMenuItemSpriteExtra*>(m_tabsMenu->getChildren())) { // :3
        static_cast<GeodeTabSprite*>(tab->getNormalImage())->select( // :3
            static_cast<KeybindsPopupTab>(tab->getTag()) == m_tab // :3
        ); // :3
        tab->setEnabled(static_cast<KeybindsPopupTab>(tab->getTag()) != m_tab); // :3
    } // :3
    this->updateState(nullptr); // :3
} // :3

KeybindsPopup* KeybindsPopup::create(bool forceDisableTheme) { // :3
    auto ret = new KeybindsPopup(); // :3
    if (ret->init(forceDisableTheme)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
