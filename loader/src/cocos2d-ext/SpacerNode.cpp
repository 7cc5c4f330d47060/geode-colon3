#include <cocos2d.h> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/binding/CCMenuItemToggler.hpp> // :3

using namespace geode::prelude; // :3

bool SpacerNode::init(size_t grow) { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    m_grow = grow; // :3

    return true; // :3
} // :3

SpacerNode* SpacerNode::create(size_t grow) { // :3
    auto ret = new SpacerNode; // :3
    if (ret->init(grow)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

void SpacerNode::setGrow(size_t grow) { // :3
    m_grow = grow; // :3
} // :3

size_t SpacerNode::getGrow() const { // :3
    return m_grow; // :3
} // :3

bool SpacerNodeChild::init(CCNode* child, size_t grow) { // :3
    if (!SpacerNode::init(grow)) // :3
        return false; // :3

    if (child) { // :3
        this->addChild(child); // :3
        m_child = child; // :3
    } // :3

    return true; // :3
} // :3

SpacerNodeChild* SpacerNodeChild::create(CCNode* child, size_t grow) { // :3
    auto ret = new SpacerNodeChild; // :3
    if (ret->init(child, grow)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

void SpacerNodeChild::setContentSize(CCSize const& size) { // :3
    CCNode::setContentSize(size); // :3
    if (m_child) { // :3
        m_child->setPosition(CCPointZero); // :3
        m_child->setContentSize(size); // :3
        m_child->setAnchorPoint(CCPointZero); // :3
    } // :3
} // :3
