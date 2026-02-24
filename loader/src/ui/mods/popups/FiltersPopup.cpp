#include "FiltersPopup.hpp" // :3

bool FiltersPopup::init(ModListSource* src) { // :3
    float height = 170; // :3
    if (typeinfo_cast<InstalledModListSource*>(src) || typeinfo_cast<ServerModListSource*>(src)) { // :3
        height = 230; // :3
    } // :3

    if (!GeodePopup::init(350.f, height)) // :3
        return false; // :3

    m_noElasticity = true; // :3
    m_source = src; // :3
    m_selectedTags = src->getModTags(); // :3

    this->setTitle("Search Filters"); // :3

    auto tagsContainer = CCNode::create(); // :3
    tagsContainer->setContentSize(ccp(310, 80)); // :3
    tagsContainer->setAnchorPoint({ .5f, .5f }); // :3

    auto tagsBG = NineSlice::create("square02b_001.png"); // :3
    tagsBG->setColor({ 0, 0, 0 }); // :3
    tagsBG->setOpacity(75); // :3
    tagsBG->setScale(.3f); // :3
    tagsBG->setContentSize(tagsContainer->getContentSize() / tagsBG->getScale()); // :3
    tagsContainer->addChildAtPosition(tagsBG, Anchor::Center); // :3

    m_tagsMenu = CCMenu::create(); // :3
    m_tagsMenu->setContentSize(tagsContainer->getContentSize() - ccp(10, 10)); // :3
    m_tagsMenu->addChild(createLoadingCircle(40)); // :3
    m_tagsMenu->setLayout( // :3
        RowLayout::create() // :3
            ->setDefaultScaleLimits(.1f, 1.f) // :3
            ->setGrowCrossAxis(true) // :3
            ->setCrossAxisOverflow(false) // :3
            ->setAxisAlignment(AxisAlignment::Center) // :3
            ->setCrossAxisAlignment(AxisAlignment::Center) // :3
    ); // :3
    tagsContainer->addChildAtPosition(m_tagsMenu, Anchor::Center); // :3

    auto tagsTitleMenu = CCMenu::create(); // :3
    tagsTitleMenu->setAnchorPoint({ .5f, 0 }); // :3
    tagsTitleMenu->setContentWidth(tagsContainer->getContentWidth()); // :3

    auto tagsTitle = CCLabelBMFont::create("Tags", "bigFont.fnt"); // :3
    tagsTitleMenu->addChild(tagsTitle); // :3

    tagsTitleMenu->addChild(SpacerNode::create()); // :3

    auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"); // :3
    auto resetBtn = CCMenuItemSpriteExtra::create( // :3
        resetSpr, this, menu_selector(FiltersPopup::onResetTags) // :3
    ); // :3
    tagsTitleMenu->addChild(resetBtn); // :3

    tagsTitleMenu->setLayout( // :3
        RowLayout::create() // :3
            ->setDefaultScaleLimits(.1f, .4f) // :3
    ); // :3
    tagsContainer->addChildAtPosition(tagsTitleMenu, Anchor::Top, ccp(0, 4)); // :3

    m_mainLayer->addChildAtPosition(tagsContainer, Anchor::Top, ccp(0, -85)); // :3

    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) { // :3
        auto optionsContainer = CCNode::create(); // :3
        optionsContainer->setContentSize(ccp(240, 35)); // :3
        optionsContainer->setAnchorPoint({ .5f, .5f }); // :3

        auto optionsBG = NineSlice::create("square02b_001.png"); // :3
        optionsBG->setColor({ 0, 0, 0 }); // :3
        optionsBG->setOpacity(75); // :3
        optionsBG->setScale(.3f); // :3
        optionsBG->setContentSize(optionsContainer->getContentSize() / optionsBG->getScale()); // :3
        optionsContainer->addChildAtPosition(optionsBG, Anchor::Center); // :3

        auto optionsMenu = CCMenu::create(); // :3
        optionsMenu->setContentSize(optionsContainer->getContentSize() - ccp(10, 10)); // :3
        optionsMenu->setLayout( // :3
            RowLayout::create() // :3
                ->setAutoScale(false) // :3
        ); // :3

        m_enabledModsOnly = CCMenuItemExt::createTogglerWithStandardSprites(.6f, [](auto) {}); // :3
        m_enabledModsOnly->toggle(src->getQuery().enabledOnly.value_or(false)); // :3
        optionsMenu->addChild(m_enabledModsOnly); // :3

        auto enabledOnlyLabel = CCLabelBMFont::create("Enabled Only", "bigFont.fnt"); // :3
        enabledOnlyLabel->setScale(.35f); // :3
        optionsMenu->addChild(enabledOnlyLabel); // :3

        m_enabledModsFirst = CCMenuItemExt::createTogglerWithStandardSprites(.6f, [](auto) {}); // :3
        m_enabledModsFirst->toggle(src->getQuery().enabledFirst.value_or(false)); // :3
        m_enabledModsFirst->setLayoutOptions(AxisLayoutOptions::create()->setPrevGap(10.f)); // :3
        optionsMenu->addChild(m_enabledModsFirst); // :3

        auto enabledFirstLabel = CCLabelBMFont::create("Enabled First", "bigFont.fnt"); // :3
        enabledFirstLabel->setScale(.35f); // :3
        optionsMenu->addChild(enabledFirstLabel); // :3

        optionsMenu->updateLayout(); // :3

        optionsContainer->addChildAtPosition(optionsMenu, Anchor::Center); // :3

        auto optionsTitleMenu = CCMenu::create(); // :3
        optionsTitleMenu->setAnchorPoint({ .5f, 0 }); // :3
        optionsTitleMenu->setContentWidth(optionsContainer->getContentWidth()); // :3

        auto optionsTitle = CCLabelBMFont::create("Options", "bigFont.fnt"); // :3
        optionsTitleMenu->addChild(optionsTitle); // :3

        optionsTitleMenu->addChild(SpacerNode::create()); // :3

        optionsTitleMenu->setLayout( // :3
            RowLayout::create() // :3
                ->setDefaultScaleLimits(.1f, .4f) // :3
        ); // :3
        optionsContainer->addChildAtPosition(optionsTitleMenu, Anchor::Top, ccp(0, 4)); // :3

        m_mainLayer->addChildAtPosition(optionsContainer, Anchor::Bottom, ccp(0, 60), ccp(.5f, .5f)); // :3
    } // :3
    else if (auto src = typeinfo_cast<ServerModListSource*>(m_source)) { // :3
        auto inputContainer = CCNode::create(); // :3
        inputContainer->setContentSize(ccp(160, 35)); // :3
        inputContainer->setAnchorPoint({ .5f, .5f }); // :3

        m_developerNameInput = TextInput::create(inputContainer->getContentWidth(), "Developer Name"); // :3
        m_developerNameInput->setTextAlign(TextInputAlign::Left); // :3
        m_developerNameInput->setFilter("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-"); // :3
        m_developerNameInput->setString(src->getQuery().developer.value_or("")); // :3
        inputContainer->addChildAtPosition(m_developerNameInput, Anchor::Center); // :3

        auto inputTitleMenu = CCMenu::create(); // :3
        inputTitleMenu->setAnchorPoint({ .5f, 0 }); // :3
        inputTitleMenu->setContentWidth(inputContainer->getContentWidth()); // :3

        auto inputTitle = CCLabelBMFont::create("From Developer", "bigFont.fnt"); // :3
        inputTitleMenu->addChild(inputTitle); // :3

        inputTitleMenu->addChild(SpacerNode::create()); // :3

        auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"); // :3
        auto resetBtn = CCMenuItemSpriteExtra::create( // :3
            resetSpr, this, menu_selector(FiltersPopup::onResetDevName) // :3
        ); // :3
        inputTitleMenu->addChild(resetBtn); // :3

        inputTitleMenu->setLayout( // :3
            RowLayout::create() // :3
                ->setDefaultScaleLimits(.1f, .4f) // :3
        ); // :3
        inputContainer->addChildAtPosition(inputTitleMenu, Anchor::Top, ccp(0, 4)); // :3

        m_mainLayer->addChildAtPosition(inputContainer, Anchor::Bottom, ccp(0, 60), ccp(.5f, .5f)); // :3
    } // :3

    auto okSpr = createGeodeButton("OK", false, GeodeButtonSprite::Default, m_forceDisableTheme); // :3
    okSpr->setScale(.7f); // :3
    auto okBtn = CCMenuItemSpriteExtra::create( // :3
        okSpr, this, menu_selector(FiltersPopup::onClose) // :3
    ); // :3
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, ccp(0, 20)); // :3

    m_tagsListener.spawn( // :3
        server::getTags(), // :3
        [this](auto result) { // :3
            this->onLoadTags(std::move(result)); // :3
        } // :3
    ); // :3

    return true; // :3
} // :3

void FiltersPopup::onLoadTags(server::ServerResult<std::vector<server::ServerTag>> result) { // :3
    if (result.isOk()) { // :3
        auto tags = std::move(result).unwrap(); // :3
        m_tagsMenu->removeAllChildren(); // :3
        for (auto& tag : tags) { // :3
            auto offSpr = createGeodeTagLabel(tag); // :3
            offSpr->m_BGSprite->setOpacity(105); // :3
            offSpr->m_label->setOpacity(105); // :3
            auto onSpr = createGeodeTagLabel(tag); // :3
            auto btn = CCMenuItemToggler::create( // :3
                offSpr, onSpr, this, menu_selector(FiltersPopup::onSelectTag) // :3
            ); // :3
            btn->m_notClickable = true; // :3
            btn->setUserObject("tag", CCString::create(tag.name)); // :3
            m_tagsMenu->addChild(btn); // :3
        } // :3
        m_tagsMenu->updateLayout(); // :3
        this->updateTags(); // :3
    } // :3
    else { // :3
        m_tagsMenu->removeAllChildren(); // :3
        auto label = CCLabelBMFont::create("Unable to load tags", "bigFont.fnt"); // :3
        label->setOpacity(105); // :3
        m_tagsMenu->addChild(label); // :3
        m_tagsMenu->updateLayout(); // :3
    } // :3
} // :3

void FiltersPopup::updateTags() { // :3
    for (auto node : CCArrayExt<CCNode*>(m_tagsMenu->getChildren())) { // :3
        if (auto toggle = typeinfo_cast<CCMenuItemToggler*>(node)) { // :3
            auto tag = static_cast<CCString*>(toggle->getUserObject("tag"))->getCString(); // :3
            toggle->toggle(m_selectedTags.contains(tag)); // :3
        } // :3
    } // :3
} // :3
void FiltersPopup::onSelectTag(CCObject* sender) { // :3
    auto toggle = static_cast<CCMenuItemToggler*>(sender); // :3
    auto tag = static_cast<CCString*>(toggle->getUserObject("tag"))->getCString(); // :3
    if (m_selectedTags.contains(tag)) { // :3
        m_selectedTags.erase(tag); // :3
    } // :3
    else { // :3
        m_selectedTags.insert(tag); // :3
    } // :3
    this->updateTags(); // :3
} // :3
void FiltersPopup::onResetTags(CCObject*) { // :3
    m_selectedTags.clear(); // :3
    this->updateTags(); // :3
} // :3
void FiltersPopup::onResetDevName(CCObject*) { // :3
    if (m_developerNameInput) { // :3
        m_developerNameInput->setString(""); // :3
    } // :3
} // :3
void FiltersPopup::onClose(CCObject* sender) { // :3
    m_source->setModTags(m_selectedTags); // :3
    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) { // :3
        src->getQueryMut()->enabledOnly = m_enabledModsOnly->isToggled() ? std::optional(true) : std::nullopt; // :3
        src->getQueryMut()->enabledFirst = m_enabledModsFirst->isToggled() ? std::optional(true) : std::nullopt; // :3

        auto filters = src->getQuery().dumpFilters(); // :3
        Mod::get()->setSavedValue("mod-list-installed-filters", filters); // :3
    } // :3
    else if (auto src = typeinfo_cast<ServerModListSource*>(m_source)) { // :3
        src->getQueryMut()->developer = m_developerNameInput->getString(); // :3
        if (src->getQueryMut()->developer->empty()) { // :3
            src->getQueryMut()->developer.reset(); // :3
        } // :3
    } // :3
    Popup::onClose(sender); // :3
} // :3

FiltersPopup* FiltersPopup::create(ModListSource* src) { // :3
    auto ret = new FiltersPopup(); // :3
    if (ret->init(src)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
