#include <cocos2d.h> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/binding/CCMenuItemToggler.hpp> // :3

using namespace geode::prelude; // :3

CopySizeLayout* CopySizeLayout::create() { // :3
    auto ret = new CopySizeLayout(); // :3
    ret->m_targets = CCArray::create(); // :3
    ret->m_targets->retain(); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

CopySizeLayout::~CopySizeLayout() { // :3
    m_targets->release(); // :3
} // :3

CopySizeLayout* CopySizeLayout::add(CCNode* target) { // :3
    m_targets->addObject(target); // :3
    return this; // :3
} // :3

CopySizeLayout* CopySizeLayout::remove(CCNode* target) { // :3
    m_targets->removeObject(target); // :3
    return this; // :3
} // :3

void CopySizeLayout::apply(CCNode* in) { // :3
    AnchorLayout::apply(in); // :3
    for (auto& node : CCArrayExt<CCNode*>(m_targets)) { // :3
        // Prevent accidental infinite loop :3
        if (node == in) continue; // :3
        node->ignoreAnchorPointForPosition(false); // :3
        node->setContentSize(in->getContentSize() * ccp(1 / in->getScaleX(), 1 / in->getScaleY())); // :3
        node->setPosition(in->getContentSize() / 2); // :3
        node->updateLayout(); // :3
    } // :3
} // :3

CCSize CopySizeLayout::getSizeHint(CCNode* in) const { // :3
    return in->getContentSize(); // :3
} // :3
