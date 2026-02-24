#include <Geode/ui/Button.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace geode::prelude; // :3

class Button::Impl final { // :3
public: // :3
    ButtonCallback m_activateCallback = nullptr; // :3
    ButtonCallback m_selectCallback = nullptr; // :3
    ButtonCallback m_unselectCallback = nullptr; // :3

    bool m_enabled = true; // :3
    bool m_selected = false; // :3
    bool m_defaultsSet = false; // :3

    int m_touchPriority = cocos2d::kCCMenuHandlerPriority; // :3

    float m_scaleMultiplier = 1.26; // :3
    float m_touchMultiplier = 1.f; // :3

    CCPoint m_offset = {0, -15}; // :3
     // :3
    float m_selectedDuration = 0.3f; // :3
    float m_unselectedDuration = 0.4f; // :3

    AnimationType m_animationType = AnimationType::Scale; // :3

    geode::Ref<cocos2d::CCActionInterval> m_clickAction = nullptr; // :3
    geode::Ref<cocos2d::CCActionInterval> m_releaseAction = nullptr; // :3

    geode::Ref<cocos2d::CCActionInterval> m_activeClickAction = nullptr; // :3
    geode::Ref<cocos2d::CCActionInterval> m_activeReleaseAction = nullptr; // :3

    cocos2d::CCNode* m_displayNode = nullptr; // :3

    geode::Ref<cocos2d::CCNodeRGBA> m_defaults = nullptr; // :3
}; // :3

Button::Button() : m_impl(std::make_unique<Impl>()) {} // :3

Button::~Button() {} // :3

Button* Button::create(ButtonCallback activateCallback) { // :3
    auto ret = new Button(); // :3
    if (ret->init(std::move(activateCallback))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

Button* Button::createWithNode(CCNode* node, ButtonCallback activateCallback) { // :3
    auto ret = new Button(); // :3
    if (ret->initWithNode(node, std::move(activateCallback))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

Button* Button::createWithSprite(ZStringView fileName, ButtonCallback activateCallback) { // :3
    auto ret = new Button(); // :3
    if (ret->initWithSprite(std::move(fileName), std::move(activateCallback))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

Button* Button::createWithSpriteFrameName(ZStringView frameName, ButtonCallback activateCallback) { // :3
    auto ret = new Button(); // :3
    if (ret->initWithSpriteFrameName(std::move(frameName),std::move(activateCallback))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

Button* Button::createWithLabel(ZStringView text, ZStringView font, ButtonCallback activateCallback) { // :3
    auto ret = new Button(); // :3
    if (ret->initWithLabel(std::move(text), std::move(font), std::move(activateCallback))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

bool Button::init(ButtonCallback activateCallback) { // :3
    if (!CCNodeRGBA::init()) return false; // :3
    m_impl->m_defaults = CCNodeRGBA::create(); // :3
    m_impl->m_activateCallback = std::move(activateCallback); // :3

    setCascadeColorEnabled(true); // :3
    setCascadeOpacityEnabled(true); // :3
    setAnchorPoint({0.5f, 0.5f}); // :3

    return true; // :3
} // :3

bool Button::initWithNode(CCNode* node, ButtonCallback activateCallback) { // :3
    if (!Button::init(std::move(activateCallback))) return false; // :3

    m_impl->m_displayNode = node; // :3

    setContentSize(m_impl->m_displayNode->getScaledContentSize()); // :3
    m_impl->m_displayNode->setPosition(getContentSize() * m_impl->m_displayNode->getAnchorPoint()); // :3

    addChild(m_impl->m_displayNode); // :3
    return true; // :3
} // :3

bool Button::initWithSprite(ZStringView fileName, ButtonCallback activateCallback) { // :3
    if (!Button::init(std::move(activateCallback))) return false; // :3

    m_impl->m_displayNode = CCSprite::create(fileName.c_str()); // :3
    if (!m_impl->m_displayNode) return false; // :3

    setContentSize(m_impl->m_displayNode->getScaledContentSize()); // :3
    m_impl->m_displayNode->setPosition(getContentSize() * m_impl->m_displayNode->getAnchorPoint()); // :3

    addChild(m_impl->m_displayNode); // :3
    return true; // :3
} // :3

bool Button::initWithSpriteFrameName(ZStringView frameName, ButtonCallback activateCallback) { // :3
    if (!Button::init(std::move(activateCallback))) return false; // :3

    m_impl->m_displayNode = CCSprite::createWithSpriteFrameName(frameName.c_str()); // :3
    if (!m_impl->m_displayNode) return false; // :3

    setContentSize(m_impl->m_displayNode->getScaledContentSize()); // :3
    m_impl->m_displayNode->setPosition(getContentSize() * m_impl->m_displayNode->getAnchorPoint()); // :3

    addChild(m_impl->m_displayNode); // :3
    return true; // :3
} // :3

bool Button::initWithLabel(ZStringView text, ZStringView font, ButtonCallback activateCallback) { // :3
    if (!Button::init(std::move(activateCallback))) return false; // :3

    m_impl->m_displayNode = CCLabelBMFont::create(text.c_str(), font.c_str()); // :3
    if (!m_impl->m_displayNode) return false; // :3

    setContentSize(m_impl->m_displayNode->getScaledContentSize()); // :3
    m_impl->m_displayNode->setPosition(getContentSize() * m_impl->m_displayNode->getAnchorPoint()); // :3

    addChild(m_impl->m_displayNode); // :3
    return true; // :3
} // :3

CCNode* Button::getDisplayNode() { // :3
    return m_impl->m_displayNode; // :3
} // :3

CCActionInterval* Button::clickActionForType() { // :3
    switch (m_impl->m_animationType) { // :3
        case AnimationType::None: { // :3
            return nullptr; // :3
        } // :3
        case AnimationType::Custom: { // :3
            return m_impl->m_clickAction; // :3
        } // :3
        case AnimationType::Scale: { // :3
            auto scaleTo = CCScaleTo::create(m_impl->m_selectedDuration, m_impl->m_defaults->getScale() * m_impl->m_scaleMultiplier); // :3
            return CCEaseBounceOut::create(scaleTo); // :3
        } // :3
        case AnimationType::Move: { // :3
            auto moveTo = CCMoveTo::create(m_impl->m_selectedDuration, m_impl->m_defaults->getPosition() + m_impl->m_offset); // :3
            return CCEaseInOut::create(moveTo, 1.5f); // :3
        } // :3
    } // :3

    return nullptr; // :3
}    // :3

CCActionInterval* Button::releaseActionForType() { // :3
    switch (m_impl->m_animationType) { // :3
        case AnimationType::None: { // :3
            return nullptr; // :3
        } // :3
        case AnimationType::Custom: { // :3
            return m_impl->m_releaseAction; // :3
        } // :3
        case AnimationType::Scale: { // :3
            auto scaleTo = CCScaleTo::create(m_impl->m_unselectedDuration, m_impl->m_defaults->getScale()); // :3
            return CCEaseBounceOut::create(scaleTo); // :3
        } // :3
        case AnimationType::Move: { // :3
            auto moveTo = CCMoveTo::create(m_impl->m_selectedDuration, m_impl->m_defaults->getPosition()); // :3
            return CCEaseInOut::create(moveTo, 2.f); // :3
        } // :3
    } // :3
     // :3
    return nullptr; // :3
} // :3

void Button::setAnimationType(AnimationType type) { // :3
    m_impl->m_animationType = type; // :3
} // :3

void Button::setClickAnimation(CCActionInterval* action) { // :3
    m_impl->m_animationType = AnimationType::Custom; // :3
    m_impl->m_clickAction = action; // :3
} // :3

void Button::setReleaseAnimation(CCActionInterval* action) { // :3
    m_impl->m_animationType = AnimationType::Custom; // :3
    m_impl->m_releaseAction = action; // :3
} // :3

void Button::setDefaults() { // :3
    m_impl->m_defaults->setScaleX(getScaleX()); // :3
    m_impl->m_defaults->setScaleY(getScaleY()); // :3

    m_impl->m_defaults->setSkewX(getSkewX()); // :3
    m_impl->m_defaults->setSkewY(getSkewY()); // :3

    m_impl->m_defaults->setRotationX(getRotationX()); // :3
    m_impl->m_defaults->setRotationY(getRotationY()); // :3

    m_impl->m_defaults->setPositionX(getPositionX()); // :3
    m_impl->m_defaults->setPositionY(getPositionY()); // :3

    m_impl->m_defaults->setContentSize(getContentSize()); // :3

    m_impl->m_defaults->setZOrder(getZOrder()); // :3

    m_impl->m_defaults->setOpacity(getOpacity()); // :3
    m_impl->m_defaults->setColor(getColor()); // :3
} // :3

void Button::resetDefaults() { // :3
    m_impl->m_defaultsSet = false; // :3

    stopAction(m_impl->m_activeClickAction); // :3
    stopAction(m_impl->m_activeReleaseAction); // :3

    m_impl->m_activeClickAction = nullptr; // :3
    m_impl->m_activeReleaseAction = nullptr; // :3

    setScaleX(m_impl->m_defaults->getScaleX()); // :3
    setScaleY(m_impl->m_defaults->getScaleY()); // :3

    setSkewX(m_impl->m_defaults->getSkewX()); // :3
    setSkewY(m_impl->m_defaults->getSkewY()); // :3

    setRotationX(m_impl->m_defaults->getRotationX()); // :3
    setRotationY(m_impl->m_defaults->getRotationY()); // :3

    setPositionX(m_impl->m_defaults->getPositionX()); // :3
    setPositionY(m_impl->m_defaults->getPositionY()); // :3

    setContentSize(m_impl->m_defaults->getContentSize()); // :3

    setZOrder(m_impl->m_defaults->getZOrder()); // :3

    setOpacity(m_impl->m_defaults->getOpacity()); // :3
    setColor(m_impl->m_defaults->getColor()); // :3
} // :3

void Button::selected() { // :3
    if (!m_impl->m_enabled || m_impl->m_selected) return; // :3
    if (!m_impl->m_defaultsSet) { // :3
        setDefaults(); // :3
        m_impl->m_defaultsSet = true; // :3
    } // :3

    stopAction(m_impl->m_activeClickAction); // :3
    stopAction(m_impl->m_activeReleaseAction); // :3

    m_impl->m_activeClickAction = clickActionForType(); // :3

    if (m_impl->m_activeClickAction) { // :3
        runAction(m_impl->m_activeClickAction); // :3
    } // :3

    if (m_impl->m_selectCallback) m_impl->m_selectCallback(this); // :3

    m_impl->m_selected = true; // :3
} // :3

void Button::unselected() { // :3
    if (!m_impl->m_selected) return; // :3
     // :3
    stopAction(m_impl->m_activeClickAction); // :3
    stopAction(m_impl->m_activeReleaseAction); // :3

    m_impl->m_activeReleaseAction = releaseActionForType(); // :3

    if (m_impl->m_activeReleaseAction) { // :3
        runAction(m_impl->m_activeReleaseAction); // :3
    } // :3

    if (m_impl->m_unselectCallback) m_impl->m_unselectCallback(this); // :3

    m_impl->m_selected = false; // :3
} // :3

void Button::activate() { // :3
    if (!m_impl->m_enabled) return; // :3
    m_impl->m_selected = false; // :3

    resetDefaults(); // :3

    if (m_impl->m_activateCallback) m_impl->m_activateCallback(this); // :3
} // :3

void Button::setScaleMultiplier(float multiplier) { // :3
    m_impl->m_scaleMultiplier = multiplier; // :3
} // :3

float Button::getScaleMultiplier() { // :3
    return m_impl->m_scaleMultiplier; // :3
} // :3

void Button::setMoveOffset(cocos2d::CCPoint const& offset) { // :3
    m_impl->m_offset = std::move(offset); // :3
} // :3

cocos2d::CCPoint Button::getMoveOffset() { // :3
    return m_impl->m_offset; // :3
} // :3

void Button::setSelectedDuration(float duration) { // :3
    m_impl->m_selectedDuration = duration; // :3
} // :3

float Button::getSelectedDuration() { // :3
    return m_impl->m_selectedDuration; // :3
} // :3

void Button::setUnselectedDuration(float duration) { // :3
    m_impl->m_unselectedDuration = duration; // :3
} // :3

float Button::getUnselectedDuration() { // :3
    return m_impl->m_unselectedDuration; // :3
} // :3

void Button::setEnabled(bool enabled) { // :3
    m_impl->m_enabled = enabled; // :3
    if (!m_impl->m_enabled) { // :3
        unselected(); // :3
    } // :3
} // :3

bool Button::isEnabled() { // :3
    return m_impl->m_enabled; // :3
} // :3

bool Button::isSelected() { // :3
    return m_impl->m_selected; // :3
} // :3

void Button::onEnter() { // :3
    CCNodeRGBA::onEnter(); // :3
    SharedButtonHandler::get()->registerButton(this); // :3
    registerWithTouchDispatcher(); // :3
} // :3

void Button::onExit() { // :3
    CCNodeRGBA::onExit(); // :3
    SharedButtonHandler::get()->unregisterButton(this); // :3
    CCTouchDispatcher::get()->removeDelegate(this); // :3
} // :3

bool Button::ccTouchBegan(CCTouch* touch, CCEvent* event) { // :3
    if (!nodeIsVisible(this)) return false; // :3

    if (SharedButtonHandler::containsTouch(this, touch)) { // :3
        SharedButtonHandler::get()->setActiveButton(this); // :3
        return true; // :3
    } // :3
    return false; // :3
} // :3

void Button::ccTouchMoved(CCTouch* touch, CCEvent* event) { // :3
    SharedButtonHandler::get()->passMoveToButtons(this, touch); // :3
} // :3

void Button::ccTouchEnded(CCTouch* touch, CCEvent* event) { // :3
    SharedButtonHandler::get()->passActivateToButtons(); // :3
} // :3

void Button::ccTouchCancelled(CCTouch* touch, CCEvent* event) { // :3
    SharedButtonHandler::get()->passCancelledToButtons(); // :3
} // :3

void Button::setTouchPriority(int priority) { // :3
    m_impl->m_touchPriority = priority; // :3
} // :3

int Button::getTouchPriority() { // :3
    return m_impl->m_touchPriority; // :3
} // :3

void Button::setTouchMultiplier(float multipler) { // :3
    m_impl->m_touchMultiplier = multipler; // :3
} // :3

float Button::getTouchMultiplier() { // :3
    return m_impl->m_touchMultiplier; // :3
} // :3

void Button::registerWithTouchDispatcher() { // :3
    CCTouchDispatcher::get()->addTargetedDelegate(this, m_impl->m_touchPriority, true); // :3
} // :3

void Button::setActivateCallback(ButtonCallback callback) { // :3
    m_impl->m_activateCallback = std::move(callback); // :3
} // :3

void Button::setSelectCallback(ButtonCallback callback) { // :3
    m_impl->m_selectCallback = std::move(callback); // :3
} // :3

void Button::setUnselectCallback(ButtonCallback callback) { // :3
    m_impl->m_unselectCallback = std::move(callback); // :3
} // :3

class SharedButtonHandler::Impl final { // :3
public: // :3
    Button* m_activeNeighbor = nullptr; // :3
    std::unordered_map<cocos2d::CCNode*, std::unordered_set<Button*>> m_buttons; // :3
}; // :3

SharedButtonHandler::SharedButtonHandler() : m_impl(std::make_unique<Impl>()) {} // :3

SharedButtonHandler::~SharedButtonHandler() {} // :3

SharedButtonHandler* SharedButtonHandler::get() { // :3
    static SharedButtonHandler handler; // :3
    return &handler; // :3
} // :3

void SharedButtonHandler::registerButton(Button* button) { // :3
    auto parent = button->getParent(); // :3
    if (!parent) return; // :3

    m_impl->m_buttons[parent].insert(button); // :3
} // :3

void SharedButtonHandler::unregisterButton(Button* button) { // :3
    auto parent = button->getParent(); // :3
    if (!parent) return; // :3

    if (m_impl->m_activeNeighbor == button) m_impl->m_activeNeighbor = nullptr; // :3

    auto& buttons = m_impl->m_buttons[parent]; // :3

    buttons.erase(button); // :3
    if (buttons.empty()) { // :3
        m_impl->m_buttons.erase(parent); // :3
    } // :3
} // :3

void SharedButtonHandler::passMoveToButtons(Button* button, CCTouch* touch) { // :3
    auto parent = button->getParent(); // :3
    if (!parent) return; // :3

    auto& buttons = m_impl-> m_buttons[parent]; // :3

    bool hasNewNeighbor = false; // :3

    for (auto button : buttons) { // :3
        if (!hasNewNeighbor && !m_impl->m_activeNeighbor && !button->isSelected() && SharedButtonHandler::containsTouch(button, touch)) { // :3
            m_impl->m_activeNeighbor = button; // :3
            hasNewNeighbor = true; // :3
            button->selected(); // :3
        } // :3
        else if (button->isSelected() && !SharedButtonHandler::containsTouch(button, touch)) { // :3
            if (m_impl->m_activeNeighbor == button) { // :3
                m_impl->m_activeNeighbor = nullptr; // :3
            } // :3
            button->unselected(); // :3
        } // :3
    } // :3
} // :3

void SharedButtonHandler::passActivateToButtons() { // :3
    if (!m_impl->m_activeNeighbor || !m_impl->m_activeNeighbor->isSelected()) return; // :3

    m_impl->m_activeNeighbor->unselected(); // :3
    m_impl->m_activeNeighbor->activate(); // :3
    m_impl->m_activeNeighbor = nullptr; // :3
} // :3

void SharedButtonHandler::passCancelledToButtons() { // :3
    if (!m_impl->m_activeNeighbor) return; // :3

    m_impl->m_activeNeighbor->unselected(); // :3
    m_impl->m_activeNeighbor = nullptr; // :3
} // :3

bool SharedButtonHandler::containsTouch(Button* button, CCTouch* touch) { // :3
    float multiplier = button->getTouchMultiplier(); // :3

    auto local = button->convertToNodeSpace(touch->getLocation()); // :3

    float width = button->getContentWidth(); // :3
    float height = button->getContentHeight(); // :3

    float scaledWidth = width * multiplier; // :3
    float scaledHeight = height * multiplier; // :3

    float dx = (scaledWidth - width) * 0.5f; // :3
    float dy = (scaledHeight - height) * 0.5f; // :3

    auto rect = cocos2d::CCRect{-dx, -dy, scaledWidth, scaledHeight}; // :3

    return rect.containsPoint(local); // :3
} // :3

void SharedButtonHandler::setActiveButton(Button* button) { // :3
    m_impl->m_activeNeighbor = button; // :3
    m_impl->m_activeNeighbor->selected(); // :3
} // :3
