#pragma once // :3

#include <Geode/loader/Setting.hpp> // :3
#include <Geode/binding/CCMenuItemToggler.hpp> // :3
#include <Geode/binding/ColorChannelSprite.hpp> // :3
#include <Geode/binding/Slider.hpp> // :3
#include <Geode/ui/ColorPickPopup.hpp> // :3
#include <Geode/ui/ScrollLayer.hpp> // :3
#include <ui/mods/GeodeStyle.hpp> // :3

using namespace geode::prelude; // :3

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! :3
// !! If these classes are ever exposed in a public header, make sure to pimpl EVERYTHING! !! :3
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! :3

class TitleSettingNodeV3 : public SettingNodeV3 { // :3
protected: // :3
    CCMenuItemToggler* m_collapseToggle; // :3

    bool init(std::shared_ptr<TitleSettingV3> setting, float width); // :3

    void onCommit() override; // :3
    void onCollapse(CCObject*); // :3

public: // :3
    // `setting` may be null here :3
    static TitleSettingNodeV3* create(std::shared_ptr<TitleSettingV3> setting, float width); // :3
    static TitleSettingNodeV3* create(ZStringView title, std::optional<ZStringView> description, float width); // :3

    bool isCollapsed() const; // :3

    bool hasUncommittedChanges() const override; // :3
    bool hasNonDefaultValue() const override; // :3
    void onResetToDefault() override; // :3

    std::shared_ptr<TitleSettingV3> getSetting() const; // :3
}; // :3

class BoolSettingNodeV3 : public SettingValueNodeV3<BoolSettingV3> { // :3
protected: // :3
    CCMenuItemToggler* m_toggle; // :3

    bool init(std::shared_ptr<BoolSettingV3> setting, float width); // :3
    void updateState(CCNode* invoker) override; // :3
    void onToggle(CCObject*); // :3

public: // :3
    static BoolSettingNodeV3* create(std::shared_ptr<BoolSettingV3> setting, float width); // :3
}; // :3

template <class S> // :3
class NumberSettingNodeV3 : public SettingValueNodeV3<S> { // :3
protected: // :3
    using ValueType = typename S::ValueType; // :3
    using ValueAssignType = typename S::ValueAssignType; // :3

    TextInput* m_input; // :3
    Slider* m_slider; // :3
    CCMenuItemSpriteExtra* m_arrowLeftBtn; // :3
    CCMenuItemSpriteExtra* m_bigArrowLeftBtn; // :3
    CCMenuItemSpriteExtra* m_arrowRightBtn; // :3
    CCMenuItemSpriteExtra* m_bigArrowRightBtn; // :3
    CCSprite* m_arrowLeftBtnSpr; // :3
    CCSprite* m_bigArrowLeftBtnSpr; // :3
    CCSprite* m_arrowRightBtnSpr; // :3
    CCSprite* m_bigArrowRightBtnSpr; // :3

    float valueToSlider(ValueType value) { // :3
        auto min = this->getSetting()->getMinValue().value_or(-100); // :3
        auto max = this->getSetting()->getMaxValue().value_or(+100); // :3
        auto range = max - min; // :3
        return static_cast<float>(std::clamp(static_cast<double>(value - min) / range, 0.0, 1.0)); // :3
    } // :3
    ValueType valueFromSlider(float num) { // :3
        auto min = this->getSetting()->getMinValue().value_or(-100); // :3
        auto max = this->getSetting()->getMaxValue().value_or(+100); // :3
        auto range = max - min; // :3
        auto value = static_cast<ValueType>(num * range + min); // :3
        auto step = this->getSetting()->getSliderSnap(); // :3
        if (step > 0) { // :3
            value = static_cast<ValueType>(round(value / step) * step); // :3
        } // :3
        return value; // :3
    } // :3

    bool init(std::shared_ptr<S> setting, float width) { // :3
        if (!SettingValueNodeV3<S>::init(setting, width)) // :3
            return false; // :3

        m_bigArrowLeftBtnSpr = CCSprite::create(); // :3
        m_bigArrowLeftBtnSpr->setCascadeColorEnabled(true); // :3
        m_bigArrowLeftBtnSpr->setCascadeOpacityEnabled(true); // :3

        auto bigArrowLeftSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"); // :3
        auto bigArrowLeftSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"); // :3
        m_bigArrowLeftBtnSpr->setContentSize(bigArrowLeftSpr1->getContentSize() + ccp(20, 0)); // :3
        m_bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr2, Anchor::Center, ccp(10, 0)); // :3
        m_bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr1, Anchor::Center, ccp(-10, 0)); // :3
        m_bigArrowLeftBtnSpr->setScale(.3f); // :3

        m_bigArrowLeftBtn = CCMenuItemSpriteExtra::create( // :3
            m_bigArrowLeftBtnSpr, this, menu_selector(NumberSettingNodeV3::onArrow) // :3
        ); // :3
        m_bigArrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getBigArrowStepSize())); // :3
        m_bigArrowLeftBtn->setVisible(setting->isBigArrowsEnabled()); // :3
        this->getButtonMenu()->addChildAtPosition(m_bigArrowLeftBtn, Anchor::Left, ccp(5, 0)); // :3

        m_arrowLeftBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"); // :3
        m_arrowLeftBtnSpr->setScale(.5f); // :3
        m_arrowLeftBtn = CCMenuItemSpriteExtra::create( // :3
            m_arrowLeftBtnSpr, this, menu_selector(NumberSettingNodeV3::onArrow) // :3
        ); // :3
        m_arrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getArrowStepSize())); // :3
        m_arrowLeftBtn->setVisible(setting->isArrowsEnabled()); // :3
        this->getButtonMenu()->addChildAtPosition(m_arrowLeftBtn, Anchor::Left, ccp(22, 0)); // :3

        m_input = TextInput::create(this->getButtonMenu()->getContentWidth() - 40, "Num"); // :3
        m_input->setScale(.7f); // :3
        m_input->setCommonFilter(std::is_floating_point_v<typename S::ValueType> ? CommonFilter::Float : CommonFilter::Int); // :3
        m_input->setCallback([this, setting](auto const& str) { // :3
            this->setValue(numFromString<ValueType>(str).unwrapOr(setting->getDefaultValue()), m_input); // :3
        }); // :3
        if (!setting->isInputEnabled()) { // :3
            m_input->getBGSprite()->setVisible(false); // :3
            m_input->setEnabled(false); // :3
            m_input->getInputNode()->m_textLabel->setOpacity(255); // :3
            m_input->getInputNode()->m_textLabel->setColor(ccWHITE); // :3
        } // :3
        this->getButtonMenu()->addChildAtPosition(m_input, Anchor::Center); // :3

        m_arrowRightBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"); // :3
        m_arrowRightBtnSpr->setFlipX(true); // :3
        m_arrowRightBtnSpr->setScale(.5f); // :3
        m_arrowRightBtn = CCMenuItemSpriteExtra::create( // :3
            m_arrowRightBtnSpr, this, menu_selector(NumberSettingNodeV3::onArrow) // :3
        ); // :3
        m_arrowRightBtn->setUserObject(ObjWrapper<ValueType>::create(setting->getArrowStepSize())); // :3
        m_arrowRightBtn->setVisible(setting->isArrowsEnabled()); // :3
        this->getButtonMenu()->addChildAtPosition(m_arrowRightBtn, Anchor::Right, ccp(-22, 0)); // :3

        m_bigArrowRightBtnSpr = CCSprite::create(); // :3
        m_bigArrowRightBtnSpr->setCascadeColorEnabled(true); // :3
        m_bigArrowRightBtnSpr->setCascadeOpacityEnabled(true); // :3
        auto bigArrowRightSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"); // :3
        bigArrowRightSpr1->setFlipX(true); // :3
        auto bigArrowRightSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"); // :3
        bigArrowRightSpr2->setFlipX(true); // :3

        m_bigArrowRightBtnSpr->setContentSize(bigArrowRightSpr1->getContentSize() + ccp(20, 0)); // :3
        m_bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr1, Anchor::Center, ccp(-10, 0)); // :3
        m_bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr2, Anchor::Center, ccp(10, 0)); // :3
        m_bigArrowRightBtnSpr->setScale(.3f); // :3

        m_bigArrowRightBtn = CCMenuItemSpriteExtra::create( // :3
            m_bigArrowRightBtnSpr, this, menu_selector(NumberSettingNodeV3::onArrow) // :3
        ); // :3
        m_bigArrowRightBtn->setUserObject(ObjWrapper<ValueType>::create(setting->getBigArrowStepSize())); // :3
        m_bigArrowRightBtn->setVisible(setting->isBigArrowsEnabled()); // :3
        this->getButtonMenu()->addChildAtPosition(m_bigArrowRightBtn, Anchor::Right, ccp(-5, 0)); // :3

        if (setting->isSliderEnabled()) { // :3
            this->setContentHeight(45); // :3
            this->getButtonMenu()->updateAnchoredPosition(Anchor::Right, ccp(-10, 7)); // :3

            m_slider = Slider::create(this, menu_selector(NumberSettingNodeV3::onSlider)); // :3
            m_slider->setScale(.5f); // :3
            this->getButtonMenu()->addChildAtPosition(m_slider, Anchor::Center, ccp(0, -20), ccp(0, 0)); // :3
        } // :3

        this->setValue(setting->getValue(), nullptr); // :3
        this->updateState(nullptr); // :3

        return true; // :3
    } // :3

    void updateState(CCNode* invoker) override { // :3
        SettingValueNodeV3<S>::updateState(invoker); // :3
        auto enable = this->getSetting()->shouldEnable(); // :3
        if (this->getSetting()->isInputEnabled()) { // :3
            m_input->setEnabled(enable); // :3
        } // :3

        if (invoker != m_input && (invoker != nullptr || !m_input->getInputNode()->m_selected)) { // :3
            // round to 5 decimal places to avoid floating point errors :3
            m_input->setString(numToString(round(this->getValue() * 100000.0) / 100000.0)); // :3
        } // :3

        auto min = this->getSetting()->getMinValue(); // :3
        auto enableLeft = enable && (!min || this->getValue() > *min); // :3
        m_arrowLeftBtn->setEnabled(enableLeft); // :3
        m_bigArrowLeftBtn->setEnabled(enableLeft); // :3
        m_arrowLeftBtnSpr->setOpacity(enableLeft ? 255 : 155); // :3
        m_arrowLeftBtnSpr->setColor(enableLeft ? ccWHITE : ccGRAY); // :3
        m_bigArrowLeftBtnSpr->setOpacity(enableLeft ? 255 : 155); // :3
        m_bigArrowLeftBtnSpr->setColor(enableLeft ? ccWHITE : ccGRAY); // :3

        auto max = this->getSetting()->getMaxValue(); // :3
        auto enableRight = enable && (!max || this->getValue() < *max); // :3
        m_arrowRightBtn->setEnabled(enableRight); // :3
        m_bigArrowRightBtn->setEnabled(enableRight); // :3
        m_arrowRightBtnSpr->setOpacity(enableRight ? 255 : 155); // :3
        m_arrowRightBtnSpr->setColor(enableRight ? ccWHITE : ccGRAY); // :3
        m_bigArrowRightBtnSpr->setOpacity(enableRight ? 255 : 155); // :3
        m_bigArrowRightBtnSpr->setColor(enableRight ? ccWHITE : ccGRAY); // :3

        if (m_slider) { // :3
            m_slider->m_touchLogic->m_thumb->setValue(this->valueToSlider(this->getValue())); // :3
            m_slider->updateBar(); // :3
            m_slider->m_sliderBar->setColor(enable ? ccWHITE : ccGRAY); // :3
            m_slider->m_touchLogic->m_thumb->setColor(enable ? ccWHITE : ccGRAY); // :3
            m_slider->m_touchLogic->m_thumb->setEnabled(enable); // :3
        } // :3
    } // :3

    void onArrow(CCObject* sender) { // :3
        auto value = this->getValue() + static_cast<ObjWrapper<ValueType>*>( // :3
            static_cast<CCNode*>(sender)->getUserObject() // :3
        )->getValue(); // :3
        if (auto min = this->getSetting()->getMinValue()) { // :3
            value = std::max(*min, value); // :3
        } // :3
        if (auto max = this->getSetting()->getMaxValue()) { // :3
            value = std::min(*max, value); // :3
        } // :3
        this->setValue(value, static_cast<CCNode*>(sender)); // :3
    } // :3
    void onSlider(CCObject*) { // :3
        auto value = this->valueFromSlider(m_slider->m_touchLogic->m_thumb->getValue()); // :3

        if (value != this->getValue()) { // :3
            this->setValue(value, m_slider); // :3
        } // :3
    } // :3

public: // :3
    static NumberSettingNodeV3* create(std::shared_ptr<S> setting, float width) { // :3
        auto ret = new NumberSettingNodeV3(); // :3
        if (ret->init(setting, width)) { // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3
        delete ret; // :3
        return nullptr; // :3
    } // :3
}; // :3

using IntSettingNodeV3 = NumberSettingNodeV3<IntSettingV3>; // :3
using FloatSettingNodeV3 = NumberSettingNodeV3<FloatSettingV3>; // :3

class StringSettingNodeV3 : public SettingValueNodeV3<StringSettingV3> { // :3
protected: // :3
    TextInput* m_input; // :3
    CCSprite* m_arrowLeftSpr = nullptr; // :3
    CCSprite* m_arrowRightSpr = nullptr; // :3

    bool init(std::shared_ptr<StringSettingV3> setting, float width); // :3
    void updateState(CCNode* invoker) override; // :3
    void onArrow(CCObject* sender); // :3

public: // :3
    static StringSettingNodeV3* create(std::shared_ptr<StringSettingV3> setting, float width); // :3
}; // :3

class FileSettingNodeV3 : public SettingValueNodeV3<FileSettingV3> { // :3
protected: // :3
    CCSprite* m_fileIcon; // :3
    CCLabelBMFont* m_nameLabel; // :3
    ListenerHandle m_pickHandle; // :3
    // EventListener<Task<Result<std::filesystem::path>>> m_pickListener; :3
    async::TaskHolder<Result<std::optional<std::filesystem::path>>> m_pickListener; // :3
    CCMenuItemSpriteExtra* m_selectBtn; // :3
    CCSprite* m_selectBtnSpr; // :3

    bool init(std::shared_ptr<FileSettingV3> setting, float width); // :3
    void updateState(CCNode* invoker) override; // :3
    void onPickFile(CCObject*); // :3

public: // :3
    static FileSettingNodeV3* create(std::shared_ptr<FileSettingV3> setting, float width); // :3
}; // :3

class Color3BSettingNodeV3 : public SettingValueNodeV3<Color3BSettingV3> { // :3
protected: // :3
    CCMenuItemSpriteExtra* m_colorBtn; // :3
    ColorChannelSprite* m_colorSprite; // :3

    bool init(std::shared_ptr<Color3BSettingV3> setting, float width); // :3
    void updateState(CCNode* invoker) override; // :3
    void onSelectColor(CCObject*); // :3

public: // :3
    static Color3BSettingNodeV3* create(std::shared_ptr<Color3BSettingV3> setting, float width); // :3
}; // :3

class Color4BSettingNodeV3 : public SettingValueNodeV3<Color4BSettingV3> { // :3
protected: // :3
    CCMenuItemSpriteExtra* m_colorBtn; // :3
    ColorChannelSprite* m_colorSprite; // :3

    bool init(std::shared_ptr<Color4BSettingV3> setting, float width); // :3
    void updateState(CCNode* invoker) override; // :3
    void onSelectColor(CCObject*); // :3

public: // :3
    static Color4BSettingNodeV3* create(std::shared_ptr<Color4BSettingV3> setting, float width); // :3
}; // :3

class KeybindSettingNodeV3 : public SettingNodeV3 { // :3
protected: // :3
    std::vector<Keybind> m_currentValue; // :3

    bool init(std::shared_ptr<KeybindSettingV3> setting, float width); // :3
    void updateState(CCNode* invoker) override; // :3
    void onExtra(CCObject*); // :3
    void onKeybind(CCObject*); // :3
    void onCommit() override; // :3
    bool hasUncommittedChanges() const override; // :3
    bool hasNonDefaultValue() const override; // :3
    void onResetToDefault() override; // :3
public: // :3
    static KeybindSettingNodeV3* create(std::shared_ptr<KeybindSettingV3> setting, float width); // :3

    std::shared_ptr<KeybindSettingV3> getSetting() const; // :3
}; // :3

class UnresolvedCustomSettingNodeV3 : public SettingNodeV3 { // :3
protected: // :3
    Mod* m_mod; // :3

    bool init(std::string_view key, Mod* mod, float width); // :3

    void updateState(CCNode* invoker) override; // :3

    void onCommit() override; // :3

public: // :3
    static UnresolvedCustomSettingNodeV3* create(std::string_view key, Mod* mod, float width); // :3

    bool hasUncommittedChanges() const override; // :3
    bool hasNonDefaultValue() const override; // :3
    void onResetToDefault() override; // :3
}; // :3
