#include <Geode/ui/LoadingSpinner.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace geode::prelude; // :3

class LoadingSpinner::Impl final { // :3
public: // :3
    cocos2d::CCSprite* spinner; // :3
}; // :3

LoadingSpinner::LoadingSpinner() : m_impl(std::make_unique<Impl>()) { } // :3

LoadingSpinner::~LoadingSpinner() { } // :3

bool LoadingSpinner::init(float sideLength) { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    this->setID("loading-spinner"); // :3
    this->setContentSize({ sideLength, sideLength }); // :3
    this->setAnchorPoint({ .5f, .5f }); // :3
    this->setCascadeOpacityEnabled(true); // :3

    m_impl->spinner = CCSprite::create("loadingCircle.png"); // :3
    m_impl->spinner->setBlendFunc({ GL_ONE, GL_ONE }); // :3
    limitNodeSize(m_impl->spinner, m_obContentSize, 1.f, .1f); // :3
    this->addChildAtPosition(m_impl->spinner, Anchor::Center); // :3

    this->spin(); // :3

    return true; // :3
} // :3

void LoadingSpinner::spin() { // :3
    auto action = CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)); // :3
    action->setTag(1); // :3

    m_impl->spinner->stopActionByTag(1); // :3
    m_impl->spinner->runAction(action); // :3
} // :3

LoadingSpinner* LoadingSpinner::create(float sideLength) { // :3
    auto ret = new LoadingSpinner(); // :3
    if (ret->init(sideLength)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

void LoadingSpinner::setVisible(bool visible) { // :3
    CCNode::setVisible(visible); // :3
    if (visible) { // :3
        this->spin(); // :3
    } // :3
} // :3

void LoadingSpinner::setContentSize(const CCSize& size) { // :3
    CCNode::setContentSize(size); // :3
    if (m_impl->spinner) limitNodeSize(m_impl->spinner, m_obContentSize, 1.f, .1f); // :3
} // :3

CCSprite* LoadingSpinner::getSpinner() { // :3
    return m_impl->spinner; // :3
} // :3
