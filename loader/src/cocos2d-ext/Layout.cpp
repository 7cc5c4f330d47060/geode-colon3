#include <cocos2d.h> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/binding/CCMenuItemToggler.hpp> // :3

using namespace geode::prelude; // :3

#pragma warning(disable: 4273) // :3

void CCNode::swapChildIndices(CCNode* first, CCNode* second) { // :3
    m_pChildren->exchangeObject(first, second); // :3
    std::swap(first->m_nZOrder, second->m_nZOrder); // :3
    std::swap(first->m_uOrderOfArrival, second->m_uOrderOfArrival); // :3
} // :3

void CCNode::insertBefore(CCNode* child, CCNode* before) { // :3
    this->addChild(child); // :3
    if ( // :3
        (before && m_pChildren->containsObject(before)) || // :3
        (before = static_cast<CCNode*>(m_pChildren->firstObject())) // :3
    ) { // :3
        child->setZOrder(before->getZOrder()); // :3
        child->setOrderOfArrival(before->getOrderOfArrival() - 1); // :3
    } // :3
} // :3

void CCNode::insertAfter(CCNode* child, CCNode* after) { // :3
    this->addChild(child); // :3
    if (m_pChildren->containsObject(after)) { // :3
        child->setZOrder(after->getZOrder()); // :3
        child->setOrderOfArrival(after->getOrderOfArrival() + 1); // :3
    } // :3
} // :3

bool CCNode::hasAncestor(CCNode* ancestor) { // :3
    if (!ancestor) { // :3
        ancestor = CCScene::get(); // :3
    } // :3
    if (m_pParent == ancestor) { // :3
        return true; // :3
    } // :3
    if (m_pParent) { // :3
        return m_pParent->hasAncestor(ancestor); // :3
    } // :3
    return false; // :3
} // :3

// these use setContentSize and getContentSize because they're virtuals and :3
// some node may override those for wacky behaviour :3

void CCNode::setScaledContentSize(CCSize const& size) { // :3
    this->setContentSize({ size.width / m_fScaleX, size.height / m_fScaleY }); // :3
} // :3

void CCNode::setContentWidth(float width) { // :3
    this->setContentSize({ width, m_obContentSize.height }); // :3
} // :3

void CCNode::setContentHeight(float height) { // :3
    this->setContentSize({ m_obContentSize.width, height }); // :3
} // :3

float CCNode::getContentWidth() const { // :3
    return this->getContentSize().width; // :3
} // :3

float CCNode::getContentHeight() const { // :3
    return this->getContentSize().height; // :3
} // :3

// getScaledContentSize is not const bruh :3

float CCNode::getScaledContentWidth() const { // :3
    return this->getContentWidth() * m_fScaleX; // :3
} // :3

float CCNode::getScaledContentHeight() const { // :3
    return this->getContentHeight() * m_fScaleY; // :3
} // :3
