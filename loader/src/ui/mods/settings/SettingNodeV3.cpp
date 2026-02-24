#include "SettingNodeV3.hpp" // :3
#include <Geode/utils/ColorProvider.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/ui/MDPopup.hpp> // :3
#include <Geode/ui/Scrollbar.hpp> // :3
#include "KeybindEditPopup.hpp" // :3

class SettingNodeV3::Impl final { // :3
public: // :3
    std::shared_ptr<SettingV3> setting; // :3
    CCLayerColor* bg; // :3
    CCLabelBMFont* nameLabel; // :3
    CCMenu* nameMenu; // :3
    CCMenu* buttonMenu; // :3
    CCMenuItemSpriteExtra* resetButton; // :3
    CCLabelBMFont* statusLabel; // :3
    ccColor4B bgColor = ccc4(0, 0, 0, 0); // :3
    bool committed = false; // :3
    // This is because you can create `TitleSettingNodeV3`s without having an :3
    // actual `TitleSettingV3` :3
    std::optional<std::string> customDescription; // :3
}; // :3

bool SettingNodeV3::init(std::shared_ptr<SettingV3> setting, float width) { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    // note: setting may be null due to UnresolvedCustomSettingNodeV3 :3

    m_impl = std::make_shared<Impl>(); // :3
    m_impl->setting = setting; // :3

    m_impl->bg = CCLayerColor::create({ 0, 0, 0, 0 }); // :3
    m_impl->bg->setContentSize({ width, 0 }); // :3
    m_impl->bg->ignoreAnchorPointForPosition(false); // :3
    m_impl->bg->setAnchorPoint(ccp(.5f, .5f)); // :3
    this->addChildAtPosition(m_impl->bg, Anchor::Center); // :3

    m_impl->nameMenu = CCMenu::create(); // :3
    m_impl->nameMenu->setContentWidth(width / 2 + 25); // :3

    m_impl->nameLabel = CCLabelBMFont::create(setting ? setting->getDisplayName().c_str() : "", "bigFont.fnt"); // :3
    m_impl->nameLabel->setLayoutOptions(AxisLayoutOptions::create()->setScaleLimits(.1f, .4f)->setScalePriority(1)); // :3
    m_impl->nameMenu->addChild(m_impl->nameLabel); // :3

    m_impl->statusLabel = CCLabelBMFont::create("", "bigFont.fnt"); // :3
    m_impl->statusLabel->setScale(.25f); // :3
    this->addChildAtPosition(m_impl->statusLabel, Anchor::Left, ccp(10, -10), ccp(0, .5f)); // :3

    if (setting && setting->getDescription()) { // :3
        auto descSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"); // :3
        descSpr->setScale(.5f); // :3
        auto descBtn = CCMenuItemSpriteExtra::create( // :3
            descSpr, this, menu_selector(SettingNodeV3::onDescription) // :3
        ); // :3
        m_impl->nameMenu->addChild(descBtn); // :3
    } // :3

    auto resetSpr = CCSprite::createWithSpriteFrameName("reset-gold.png"_spr); // :3
    resetSpr->setScale(.5f); // :3
    m_impl->resetButton = CCMenuItemSpriteExtra::create( // :3
        resetSpr, this, menu_selector(SettingNodeV3::onReset) // :3
    ); // :3
    m_impl->nameMenu->addChild(m_impl->resetButton); // :3

    m_impl->nameMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)); // :3
    this->addChildAtPosition(m_impl->nameMenu, Anchor::Left, ccp(10, 0), ccp(0, .5f)); // :3

    m_impl->buttonMenu = CCMenu::create(); // :3
    m_impl->buttonMenu->setContentSize({ width / 2 - 55, 30 }); // :3
    m_impl->buttonMenu->setLayout(AnchorLayout::create()); // :3
    this->addChildAtPosition(m_impl->buttonMenu, Anchor::Right, ccp(-10, 0), ccp(1, .5f)); // :3

    this->setAnchorPoint({ .5f, .5f }); // :3
    this->setContentSize({ width, 30 }); // :3

    return true; // :3
} // :3

void SettingNodeV3::updateState(CCNode* invoker) { // :3
    m_impl->statusLabel->setVisible(false); // :3

    m_impl->nameLabel->setColor(this->hasUncommittedChanges() ? ccc3(17, 221, 0) : ccWHITE); // :3
    m_impl->resetButton->setVisible(this->hasNonDefaultValue()); // :3

    m_impl->bg->setColor(to3B(m_impl->bgColor)); // :3
    m_impl->bg->setOpacity(m_impl->bgColor.a); // :3

    if (m_impl->setting && !m_impl->setting->shouldEnable()) { // :3
        if (auto desc = m_impl->setting->getEnableIfDescription()) { // :3
            m_impl->nameLabel->setColor(ccGRAY); // :3
            m_impl->statusLabel->setVisible(true); // :3
            m_impl->statusLabel->setColor("mod-list-errors-found"_cc3b); // :3
            m_impl->statusLabel->setString(desc->c_str()); // :3
        } // :3
    } // :3
    if (m_impl->setting && m_impl->setting->requiresRestart() && m_impl->committed) { // :3
        m_impl->statusLabel->setVisible(true); // :3
        m_impl->statusLabel->setColor("mod-list-restart-required-label"_cc3b); // :3
        m_impl->statusLabel->setString("Restart Required"); // :3
        m_impl->bg->setColor("mod-list-restart-required-label-bg"_cc3b); // :3
        m_impl->bg->setOpacity(75); // :3
    } // :3

    m_impl->nameMenu->setContentWidth(this->getContentWidth() - m_impl->buttonMenu->getContentWidth() - 25); // :3
    m_impl->nameMenu->updateLayout(); // :3
} // :3
void SettingNodeV3::updateState2(CCNode* invoker) { // :3
    return this->updateState(invoker); // :3
} // :3

void SettingNodeV3::onDescription(CCObject*) { // :3
    if (!m_impl->setting) return; // :3
    auto title = m_impl->setting->getDisplayName(); // :3
    MDPopup::create(true, // :3
        title.c_str(), // :3
        m_impl->setting->getDescription().value_or("No description provided"), // :3
        "OK" // :3
    )->show(); // :3
} // :3
void SettingNodeV3::onReset(CCObject*) { // :3
    createQuickPopup( // :3
        "Reset", // :3
        fmt::format( // :3
            "Are you sure you want to <cr>reset</c> <cl>{}</c> to <cy>default</c>?", // :3
            this->getSetting()->getDisplayName() // :3
        ), // :3
        "Cancel", "Reset", // :3
        [this](auto, bool btn2) { // :3
            if (btn2) { // :3
                this->resetToDefault(); // :3
            } // :3
        } // :3
    ); // :3
} // :3

void SettingNodeV3::setDefaultBGColor(ccColor4B color) { // :3
    m_impl->bgColor = color; // :3
    this->updateState(nullptr); // :3
} // :3

void SettingNodeV3::markChanged(CCNode* invoker) { // :3
    this->updateState(invoker); // :3
    SettingNodeValueChangeEventV3( // :3
        m_impl->setting ? m_impl->setting->getModID() : "", // :3
        m_impl->setting ? m_impl->setting->getKey() : "" // :3
    ).send(this, false); // :3
} // :3
void SettingNodeV3::commit() { // :3
    if (!m_impl->setting) return; // :3
    this->onCommit(); // :3
    m_impl->committed = true; // :3
    this->updateState(nullptr); // :3
    SettingNodeValueChangeEventV3(m_impl->setting->getModID(), m_impl->setting->getKey()).send(this, true); // :3
} // :3
void SettingNodeV3::resetToDefault() { // :3
    if (!m_impl->setting) return; // :3
    m_impl->setting->reset(); // :3
    m_impl->committed = true; // :3
    this->onResetToDefault(); // :3
    this->updateState(nullptr); // :3
    SettingNodeValueChangeEventV3(m_impl->setting->getModID(), m_impl->setting->getKey()).send(this, false); // :3
} // :3

void SettingNodeV3::overrideDescription(std::optional<ZStringView> description) { // :3
    m_impl->customDescription = description ? std::optional(std::string(*description)) : std::nullopt; // :3
} // :3

void SettingNodeV3::setContentSize(CCSize const& size) { // :3
    CCNode::setContentSize(size); // :3
    m_impl->bg->setContentSize(size); // :3
    this->updateLayout(); // :3
    if (!m_impl->setting) return; // :3
    SettingNodeSizeChangeEventV3(m_impl->setting->getModID(), m_impl->setting->getKey()).send(this); // :3
} // :3

CCLabelBMFont* SettingNodeV3::getNameLabel() const { // :3
    return m_impl->nameLabel; // :3
} // :3
CCLabelBMFont* SettingNodeV3::getStatusLabel() const { // :3
    return m_impl->statusLabel; // :3
} // :3
CCMenu* SettingNodeV3::getNameMenu() const { // :3
    return m_impl->nameMenu; // :3
} // :3
CCMenu* SettingNodeV3::getButtonMenu() const { // :3
    return m_impl->buttonMenu; // :3
} // :3
CCLayerColor* SettingNodeV3::getBG() const { // :3
    return m_impl->bg; // :3
} // :3

std::shared_ptr<SettingV3> SettingNodeV3::getSetting() const { // :3
    return m_impl->setting; // :3
} // :3

// TitleSettingNodeV3 :3

bool TitleSettingNodeV3::init(std::shared_ptr<TitleSettingV3> setting, float width) { // :3
    if (!SettingNodeV3::init(setting, width)) // :3
        return false; // :3

    // note: setting may be null :3

    auto collapseSprBG = CCSprite::create("square02c_001.png"); // :3
    collapseSprBG->setColor(ccc3(25, 25, 25)); // :3
    collapseSprBG->setOpacity(105); // :3
    auto collapseSpr = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png"); // :3
    collapseSpr->setScale(1.9f); // :3
    collapseSprBG->addChildAtPosition(collapseSpr, Anchor::Center); // :3
    collapseSprBG->setScale(.2f); // :3

    auto uncollapseSprBG = CCSprite::create("square02c_001.png"); // :3
    uncollapseSprBG->setColor(ccc3(25, 25, 25)); // :3
    uncollapseSprBG->setOpacity(105); // :3
    auto uncollapseSpr = CCSprite::createWithSpriteFrameName("edit_delCBtn_001.png"); // :3
    uncollapseSpr->setScale(1.5f); // :3
    uncollapseSprBG->addChildAtPosition(uncollapseSpr, Anchor::Center); // :3
    uncollapseSprBG->setScale(.2f); // :3

    m_collapseToggle = CCMenuItemToggler::create( // :3
        collapseSprBG, uncollapseSprBG, // :3
        this, menu_selector(TitleSettingNodeV3::onCollapse) // :3
    ); // :3
    m_collapseToggle->m_notClickable = true; // :3
    this->getButtonMenu()->setContentWidth(20); // :3
    this->getButtonMenu()->addChildAtPosition(m_collapseToggle, Anchor::Center); // :3

    this->getNameLabel()->setFntFile("goldFont.fnt"); // :3
    this->getNameMenu()->updateLayout(); // :3
    this->setContentHeight(20); // :3
    this->updateState(nullptr); // :3

    return true; // :3
} // :3

void TitleSettingNodeV3::onCollapse(CCObject* sender) { // :3
    m_collapseToggle->toggle(!m_collapseToggle->isToggled()); // :3
    // This triggers popup state to update due to SettingNodeValueChangeEventV3 being posted :3
    this->markChanged(static_cast<CCNode*>(sender)); // :3
} // :3
void TitleSettingNodeV3::onCommit() {} // :3

bool TitleSettingNodeV3::isCollapsed() const { // :3
    return m_collapseToggle->isToggled(); // :3
} // :3

bool TitleSettingNodeV3::hasUncommittedChanges() const { // :3
    return false; // :3
} // :3
bool TitleSettingNodeV3::hasNonDefaultValue() const { // :3
    return false; // :3
} // :3
void TitleSettingNodeV3::onResetToDefault() {} // :3

std::shared_ptr<TitleSettingV3> TitleSettingNodeV3::getSetting() const { // :3
    return std::static_pointer_cast<TitleSettingV3>(SettingNodeV3::getSetting()); // :3
} // :3

TitleSettingNodeV3* TitleSettingNodeV3::create(std::shared_ptr<TitleSettingV3> setting, float width) { // :3
    auto ret = new TitleSettingNodeV3(); // :3
    if (ret->init(setting, width)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
TitleSettingNodeV3* TitleSettingNodeV3::create(ZStringView title, std::optional<ZStringView> description, float width) { // :3
    auto ret = TitleSettingNodeV3::create(nullptr, width); // :3
    ret->getNameLabel()->setString(title.c_str()); // :3
    ret->overrideDescription(description); // :3
    ret->updateState(nullptr); // :3
    return ret; // :3
} // :3

// BoolSettingNodeV3 :3

bool BoolSettingNodeV3::init(std::shared_ptr<BoolSettingV3> setting, float width) { // :3
    if (!SettingValueNodeV3::init(setting, width)) // :3
        return false; // :3

    this->getButtonMenu()->setContentWidth(20); // :3

    m_toggle = CCMenuItemToggler::createWithStandardSprites( // :3
        this, menu_selector(BoolSettingNodeV3::onToggle), .55f // :3
    ); // :3
    m_toggle->m_onButton->setContentSize({ 25, 25 }); // :3
    m_toggle->m_onButton->getNormalImage()->setPosition(ccp(25, 25) / 2); // :3
    m_toggle->m_offButton->setContentSize({ 25, 25 }); // :3
    m_toggle->m_offButton->getNormalImage()->setPosition(ccp(25, 25) / 2); // :3
    m_toggle->m_notClickable = true; // :3
    m_toggle->toggle(setting->getValue()); // :3
    this->getButtonMenu()->addChildAtPosition(m_toggle, Anchor::Right, ccp(-10, 0)); // :3

    this->updateState(nullptr); // :3

    return true; // :3
} // :3

void BoolSettingNodeV3::updateState(CCNode* invoker) { // :3
    SettingValueNodeV3::updateState(invoker); // :3
    auto enable = this->getSetting()->shouldEnable(); // :3
    m_toggle->toggle(this->getValue()); // :3
    m_toggle->setCascadeColorEnabled(true); // :3
    m_toggle->setCascadeOpacityEnabled(true); // :3
    m_toggle->setEnabled(enable); // :3
    m_toggle->setColor(enable ? ccWHITE : ccGRAY); // :3
    m_toggle->setOpacity(enable ? 255 : 155); // :3
} // :3

void BoolSettingNodeV3::onToggle(CCObject*) { // :3
    this->setValue(!m_toggle->isToggled(), m_toggle); // :3
    this->markChanged(m_toggle); // :3
} // :3

BoolSettingNodeV3* BoolSettingNodeV3::create(std::shared_ptr<BoolSettingV3> setting, float width) { // :3
    auto ret = new BoolSettingNodeV3(); // :3
    if (ret->init(setting, width)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

// StringSettingNodeV3 :3

bool StringSettingNodeV3::init(std::shared_ptr<StringSettingV3> setting, float width) { // :3
    if (!SettingValueNodeV3::init(setting, width)) // :3
        return false; // :3

    m_input = TextInput::create(setting->getEnumOptions() ? width / 2 - 50 : width / 2, "Text"); // :3
    m_input->setCallback([this](auto const& str) { // :3
        this->setValue(str, m_input); // :3
    }); // :3
    m_input->setScale(.7f); // :3
    m_input->setString(this->getSetting()->getValue()); // :3
    if (auto filter = this->getSetting()->getAllowedCharacters()) { // :3
        m_input->setFilter(*filter); // :3
    } // :3

    this->getButtonMenu()->addChildAtPosition(m_input, Anchor::Center); // :3

    if (setting->getEnumOptions()) { // :3
        m_input->getBGSprite()->setVisible(false); // :3
        m_input->setEnabled(false); // :3
        m_input->getInputNode()->m_textLabel->setOpacity(255); // :3
        m_input->getInputNode()->m_textLabel->setColor(ccWHITE); // :3

        m_arrowLeftSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"); // :3
        m_arrowLeftSpr->setFlipX(true); // :3
        m_arrowLeftSpr->setScale(.4f); // :3
        auto arrowLeftBtn = CCMenuItemSpriteExtra::create( // :3
            m_arrowLeftSpr, this, menu_selector(StringSettingNodeV3::onArrow) // :3
        ); // :3
        arrowLeftBtn->setTag(-1); // :3
        this->getButtonMenu()->addChildAtPosition(arrowLeftBtn, Anchor::Left, ccp(5, 0)); // :3

        m_arrowRightSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"); // :3
        m_arrowRightSpr->setScale(.4f); // :3
        auto arrowRightBtn = CCMenuItemSpriteExtra::create( // :3
            m_arrowRightSpr, this, menu_selector(StringSettingNodeV3::onArrow) // :3
        ); // :3
        arrowRightBtn->setTag(1); // :3
        this->getButtonMenu()->addChildAtPosition(arrowRightBtn, Anchor::Right, ccp(-5, 0)); // :3
    } // :3

    this->updateState(nullptr); // :3

    return true; // :3
} // :3

void StringSettingNodeV3::updateState(CCNode* invoker) { // :3
    SettingValueNodeV3::updateState(invoker); // :3

    if (invoker != m_input) { // :3
        m_input->setString(this->getValue()); // :3
    } // :3

    auto enable = this->getSetting()->shouldEnable(); // :3
    if (!this->getSetting()->getEnumOptions()) { // :3
        m_input->setEnabled(enable); // :3
    } // :3
    else { // :3
        m_arrowRightSpr->setOpacity(enable ? 255 : 155); // :3
        m_arrowRightSpr->setColor(enable ? ccWHITE : ccGRAY); // :3
        m_arrowLeftSpr->setOpacity(enable ? 255 : 155); // :3
        m_arrowLeftSpr->setColor(enable ? ccWHITE : ccGRAY); // :3
    } // :3
} // :3

void StringSettingNodeV3::onArrow(CCObject* sender) { // :3
    auto options = *this->getSetting()->getEnumOptions(); // :3
    auto index = ranges::indexOf(options, this->getValue()).value_or(0); // :3
    if (sender->getTag() > 0) { // :3
        index = index < options.size() - 1 ? index + 1 : 0; // :3
    } // :3
    else { // :3
        index = index > 0 ? index - 1 : options.size() - 1; // :3
    } // :3
    this->setValue(options.at(index), static_cast<CCNode*>(sender)); // :3
} // :3

StringSettingNodeV3* StringSettingNodeV3::create(std::shared_ptr<StringSettingV3> setting, float width) { // :3
    auto ret = new StringSettingNodeV3(); // :3
    if (ret->init(setting, width)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

// FileSettingNodeV3 :3

bool FileSettingNodeV3::init(std::shared_ptr<FileSettingV3> setting, float width) { // :3
    if (!SettingValueNodeV3::init(setting, width)) // :3
        return false; // :3

    auto labelBG = NineSlice::create("square02b_001.png", { 0, 0, 80, 80 }); // :3
    labelBG->setScale(.25f); // :3
    labelBG->setColor({ 0, 0, 0 }); // :3
    labelBG->setOpacity(90); // :3
    labelBG->setContentSize({ 420, 80 }); // :3
    this->getButtonMenu()->addChildAtPosition(labelBG, Anchor::Center, ccp(-10, 0)); // :3

    m_fileIcon = CCSprite::create(); // :3
    this->getButtonMenu()->addChildAtPosition(m_fileIcon, Anchor::Left, ccp(5, 0)); // :3

    m_nameLabel = CCLabelBMFont::create("", "bigFont.fnt"); // :3
    this->getButtonMenu()->addChildAtPosition(m_nameLabel, Anchor::Left, ccp(13, 0), ccp(0, .5f)); // :3

    m_selectBtnSpr = CCSprite::createWithSpriteFrameName("GJ_plus2Btn_001.png"); // :3
    m_selectBtnSpr->setScale(.7f); // :3
    m_selectBtn = CCMenuItemSpriteExtra::create( // :3
        m_selectBtnSpr, this, menu_selector(FileSettingNodeV3::onPickFile) // :3
    ); // :3
    this->getButtonMenu()->addChildAtPosition(m_selectBtn, Anchor::Right, ccp(-5, 0)); // :3

    this->updateState(nullptr); // :3

    return true; // :3
} // :3

void FileSettingNodeV3::updateState(CCNode* invoker) { // :3
    // This is because people tend to put `"default": "Please pick a good file"` :3
    // which is clever and good UX but also a hack so I also need to hack to support that :3
    const auto isTextualDefaultValue = [this, setting = this->getSetting()]() { // :3
        if (this->hasNonDefaultValue()) return false; // :3
        if (utils::string::pathToString(setting->getDefaultValue()).size() > 20) return false; // :3
        std::error_code ec; // :3
        return setting->isFolder() ? // :3
            !std::filesystem::is_directory(setting->getDefaultValue(), ec) : // :3
            !std::filesystem::is_regular_file(setting->getDefaultValue(), ec); // :3
    }(); // :3

    SettingValueNodeV3::updateState(invoker); // :3
    m_fileIcon->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName( // :3
        this->getSetting()->isFolder() ? "folderIcon_001.png" : "file.png"_spr // :3
    )); // :3
    limitNodeSize(m_fileIcon, ccp(10, 10), 1.f, .1f); // :3
    if (this->getValue().empty() || isTextualDefaultValue) { // :3
        if (isTextualDefaultValue) { // :3
            m_nameLabel->setString(utils::string::pathToString(this->getSetting()->getDefaultValue()).c_str()); // :3
        } // :3
        else { // :3
            m_nameLabel->setString(this->getSetting()->isFolder() ? "No Folder Selected" : "No File Selected"); // :3
        } // :3
        m_nameLabel->setColor(ccGRAY); // :3
        m_nameLabel->setOpacity(155); // :3
    } // :3
    else { // :3
        m_nameLabel->setString(utils::string::pathToString(this->getValue().filename()).c_str()); // :3
        m_nameLabel->setColor(ccWHITE); // :3
        m_nameLabel->setOpacity(255); // :3
    } // :3
    m_nameLabel->limitLabelWidth(75, .35f, .1f); // :3

    auto enable = this->getSetting()->shouldEnable(); // :3
    m_selectBtnSpr->setOpacity(enable ? 255 : 155); // :3
    m_selectBtnSpr->setColor(enable ? ccWHITE : ccGRAY); // :3
    m_selectBtn->setEnabled(enable); // :3
} // :3

void FileSettingNodeV3::onPickFile(CCObject*) { // :3
    std::error_code ec; // :3

    m_pickListener.spawn( // :3
        file::pick( // :3
            this->getSetting()->isFolder() ? // :3
            file::PickMode::OpenFolder : // :3
            this->getSetting()->useSaveDialog() ? file::PickMode::SaveFile : file::PickMode::OpenFile, // :3
            { // :3
                // Prefer opening the current path directly if possible :3
                this->getValue().empty() || !std::filesystem::exists(this->getValue().parent_path(), ec) // :3
                    ? dirs::getGameDir() : this->getValue(), // :3
                this->getSetting()->getFilters().value_or(std::vector<file::FilePickOptions::Filter>()) // :3
            } // :3
        ), // :3
        [this](Result<std::optional<std::filesystem::path>> path) { // :3
            if (path.isOk() && path.unwrap().has_value()) { // :3
                this->setValue(std::move(path).unwrap().value(), nullptr); // :3
            } // :3
            else if (path.isErr()) { // :3
                FLAlertLayer::create( // :3
                    "Failed", // :3
                    fmt::format("Failed to pick file: {}", path.unwrapErr()), // :3
                    "Ok" // :3
                )->show(); // :3
            } // :3
        } // :3
    ); // :3
} // :3

FileSettingNodeV3* FileSettingNodeV3::create(std::shared_ptr<FileSettingV3> setting, float width) { // :3
    auto ret = new FileSettingNodeV3(); // :3
    if (ret->init(setting, width)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

// Color3BSettingNodeV3 :3

bool Color3BSettingNodeV3::init(std::shared_ptr<Color3BSettingV3> setting, float width) { // :3
    if (!SettingValueNodeV3::init(setting, width)) // :3
        return false; // :3

    m_colorSprite = ColorChannelSprite::create(); // :3
    m_colorSprite->setScale(.65f); // :3

    m_colorBtn = CCMenuItemSpriteExtra::create( // :3
        m_colorSprite, this, menu_selector(Color3BSettingNodeV3::onSelectColor) // :3
    ); // :3
    this->getButtonMenu()->addChildAtPosition(m_colorBtn, Anchor::Right, ccp(-10, 0)); // :3

    this->updateState(nullptr); // :3

    return true; // :3
} // :3

void Color3BSettingNodeV3::updateState(CCNode* invoker) { // :3
    SettingValueNodeV3::updateState(invoker); // :3
    m_colorSprite->setColor(this->getValue()); // :3

    auto enable = this->getSetting()->shouldEnable(); // :3
    m_colorSprite->setOpacity(enable ? 255 : 155); // :3
    m_colorBtn->setEnabled(enable); // :3
} // :3

void Color3BSettingNodeV3::onSelectColor(CCObject*) { // :3
    auto popup = ColorPickPopup::create(this->getValue()); // :3
    popup->setCallback([this](ccColor4B const& color) { this->setValue(to3B(color), nullptr); }); // :3
    popup->show(); // :3
} // :3
Color3BSettingNodeV3* Color3BSettingNodeV3::create(std::shared_ptr<Color3BSettingV3> setting, float width) { // :3
    auto ret = new Color3BSettingNodeV3(); // :3
    if (ret->init(setting, width)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

// Color4BSettingNodeV3 :3

bool Color4BSettingNodeV3::init(std::shared_ptr<Color4BSettingV3> setting, float width) { // :3
    if (!SettingValueNodeV3::init(setting, width)) // :3
        return false; // :3

    m_colorSprite = ColorChannelSprite::create(); // :3
    m_colorSprite->setScale(.65f); // :3

    m_colorBtn = CCMenuItemSpriteExtra::create( // :3
        m_colorSprite, this, menu_selector(Color4BSettingNodeV3::onSelectColor) // :3
    ); // :3
    this->getButtonMenu()->addChildAtPosition(m_colorBtn, Anchor::Right, ccp(-10, 0)); // :3

    this->updateState(nullptr); // :3

    return true; // :3
} // :3

void Color4BSettingNodeV3::updateState(CCNode* invoker) { // :3
    SettingValueNodeV3::updateState(invoker); // :3
    m_colorSprite->setColor(to3B(this->getValue())); // :3
    m_colorSprite->updateOpacity(this->getValue().a / 255.f); // :3

    auto enable = this->getSetting()->shouldEnable(); // :3
    m_colorSprite->setOpacity(enable ? 255 : 155); // :3
    m_colorBtn->setEnabled(enable); // :3
} // :3

void Color4BSettingNodeV3::onSelectColor(CCObject*) { // :3
    auto popup = ColorPickPopup::create(this->getValue()); // :3
    popup->setCallback([this](ccColor4B const& color) { this->setValue(color, nullptr); }); // :3
    popup->show(); // :3
} // :3

Color4BSettingNodeV3* Color4BSettingNodeV3::create(std::shared_ptr<Color4BSettingV3> setting, float width) { // :3
    auto ret = new Color4BSettingNodeV3(); // :3
    if (ret->init(setting, width)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

// KeybindSettingNodeV3 :3

KeybindSettingNodeV3* KeybindSettingNodeV3::create(std::shared_ptr<KeybindSettingV3> setting, float width) { // :3
    auto ret = new KeybindSettingNodeV3(); // :3
    if (ret->init(setting, width)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

bool KeybindSettingNodeV3::init(std::shared_ptr<KeybindSettingV3> setting, float width) { // :3
    if (!SettingNodeV3::init(setting, width)) // :3
        return false; // :3

    m_currentValue = setting->getValue(); // :3
    this->getButtonMenu()->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::End)); // :3

    if (auto category = setting->getCategory()) { // :3
        const char* catSpr; // :3
        switch (*category) { // :3
            default: // :3
            case KeybindCategory::Editor: { // :3
                catSpr = "GJ_hammerIcon_001.png"; // :3
            } break; // :3

            case KeybindCategory::Gameplay: { // :3
                catSpr = "controllerBtn_DPad_Right_001.png"; // :3
            } break; // :3

            case KeybindCategory::Universal: { // :3
                catSpr = "GJ_sMagicIcon_001.png"; // :3
            } break; // :3
        } // :3
        auto categoryLabel = createTagLabelWithIcon( // :3
            CCSprite::createWithSpriteFrameName(catSpr), "", // :3
            std::make_pair(ccWHITE, "keybinds-list-category-label"_cc3b) // :3
        ); // :3
        categoryLabel->setLayoutOptions( // :3
            AxisLayoutOptions::create()->setScaleLimits(.1f, .35f) // :3
        ); // :3
        this->getNameMenu()->addChild(categoryLabel); // :3
    } // :3

    this->updateState(nullptr); // :3

    return true; // :3
} // :3

std::shared_ptr<KeybindSettingV3> KeybindSettingNodeV3::getSetting() const { // :3
    return std::static_pointer_cast<KeybindSettingV3>(SettingNodeV3::getSetting()); // :3
} // :3

void KeybindSettingNodeV3::updateState(CCNode* invoker) { // :3
    SettingNodeV3::updateState(invoker); // :3

    auto buttonMenu = getButtonMenu(); // :3

    buttonMenu->removeAllChildren(); // :3
    size_t index = 0; // :3
    for (auto& keybind : m_currentValue) { // :3
        auto bspr = createKeybindButton(keybind); // :3
        bspr->setScale(.5f); // :3
        auto button = CCMenuItemSpriteExtra::create(bspr, this, menu_selector(KeybindSettingNodeV3::onKeybind)); // :3
        button->setTag(index); // :3
        buttonMenu->addChild(button); // :3
        index += 1; // :3
    } // :3
    auto plusSprite = createGeodeButton("+", true); // :3
    plusSprite->setScale(.5f); // :3
    auto plusButton = CCMenuItemSpriteExtra::create(plusSprite, this, menu_selector(KeybindSettingNodeV3::onKeybind)); // :3
    buttonMenu->addChild(plusButton); // :3
    buttonMenu->updateLayout(); // :3

    if (buttonMenu->getChildByIndex(0)->getScale() < 1.f) { // :3
        auto children = buttonMenu->getChildrenExt(); // :3
        for (auto it = children.end() - 1; it != children.begin(); --it) { // :3
            buttonMenu->removeChild(*it, false); // :3
        } // :3

        auto moreSprite = createGeodeButton("...", true); // :3
        moreSprite->setScale(.5f); // :3
        auto moreButton = CCMenuItemSpriteExtra::create(moreSprite, this, menu_selector(KeybindSettingNodeV3::onExtra)); // :3
        buttonMenu->addChild(moreButton); // :3

        buttonMenu->updateLayout(); // :3
    } // :3
} // :3

void KeybindSettingNodeV3::onExtra(CCObject* sender) { // :3
    KeybindListPopup::create( // :3
        getSetting(), // :3
        m_currentValue, // :3
        [this](std::vector<Keybind> newKeybinds) { // :3
            if (m_currentValue == newKeybinds) return; // :3
            m_currentValue = std::move(newKeybinds); // :3
            this->markChanged(nullptr); // :3
        } // :3
    )->show(); // :3
} // :3

void KeybindSettingNodeV3::onKeybind(CCObject* sender) { // :3
    auto index = sender->getTag(); // :3
    KeybindEditPopup::create( // :3
        getSetting(), // :3
        index >= 0 ? m_currentValue[index] : Keybind(), // :3
        [this, index](Keybind const& newKeybind) { // :3
            if (index >= 0) { // :3
                if (newKeybind.key == KEY_None || std::ranges::contains(m_currentValue, newKeybind)) { // :3
                    m_currentValue.erase(m_currentValue.begin() + index); // :3
                } // :3
                else { // :3
                    m_currentValue[index] = newKeybind; // :3
                } // :3
            } // :3
            else { // :3
                if (std::ranges::contains(m_currentValue, newKeybind)) return; // :3
                m_currentValue.push_back(newKeybind); // :3
            } // :3
            this->markChanged(nullptr); // :3
        } // :3
    )->show(); // :3
} // :3

void KeybindSettingNodeV3::onCommit() { // :3
    getSetting()->setValue(m_currentValue); // :3
    this->markChanged(nullptr); // :3
} // :3

bool KeybindSettingNodeV3::hasUncommittedChanges() const { // :3
    return m_currentValue != getSetting()->getValue(); // :3
} // :3

bool KeybindSettingNodeV3::hasNonDefaultValue() const { // :3
    return m_currentValue != getSetting()->getDefaultValue(); // :3
} // :3

void KeybindSettingNodeV3::onResetToDefault() { // :3
    m_currentValue = getSetting()->getDefaultValue(); // :3
    this->markChanged(nullptr); // :3
} // :3

// UnresolvedCustomSettingNodeV3 :3

bool UnresolvedCustomSettingNodeV3::init(std::string_view key, Mod* mod, float width) { // :3
    if (!SettingNodeV3::init(nullptr, width)) // :3
        return false; // :3

    m_mod = mod; // :3

    this->setContentHeight(30); // :3

    auto label = CCLabelBMFont::create( // :3
        (mod && mod->isLoaded() ? // :3
            fmt::format("Missing setting '{}'", key) : // :3
            fmt::format("Enable the Mod to Edit '{}'", key) // :3
        ).c_str(), // :3
        "bigFont.fnt" // :3
    ); // :3
    label->setColor(mod && mod->isLoaded() ? "mod-list-errors-found-2"_cc3b : "mod-list-gray"_cc3b); // :3
    label->limitLabelWidth(width - m_obContentSize.height, .3f, .1f); // :3
    this->addChildAtPosition(label, Anchor::Left, ccp(m_obContentSize.height / 2, 0), ccp(0, .5f)); // :3

    return true; // :3
} // :3

void UnresolvedCustomSettingNodeV3::updateState(CCNode* invoker) { // :3
    SettingNodeV3::updateState(invoker); // :3
    this->getBG()->setColor(m_mod && m_mod->isLoaded() ? "mod-list-errors-found-2"_cc3b : "mod-list-gray"_cc3b); // :3
    this->getBG()->setOpacity(75); // :3
} // :3

void UnresolvedCustomSettingNodeV3::onCommit() {} // :3

bool UnresolvedCustomSettingNodeV3::hasUncommittedChanges() const { // :3
    return false; // :3
} // :3
bool UnresolvedCustomSettingNodeV3::hasNonDefaultValue() const { // :3
    return false; // :3
} // :3
void UnresolvedCustomSettingNodeV3::onResetToDefault() {} // :3

UnresolvedCustomSettingNodeV3* UnresolvedCustomSettingNodeV3::create(std::string_view key, Mod* mod, float width) { // :3
    auto ret = new UnresolvedCustomSettingNodeV3(); // :3
    if (ret->init(key, mod, width)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
