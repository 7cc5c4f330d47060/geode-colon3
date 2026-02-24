#include <cocos2d.h> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/binding/CCMenuItemToggler.hpp> // :3

using namespace geode::prelude; // :3

AnchorLayoutOptions* AnchorLayoutOptions::create() { // :3
    auto ret = new AnchorLayoutOptions(); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

Anchor AnchorLayoutOptions::getAnchor() const { // :3
    return m_anchor; // :3
} // :3

CCPoint AnchorLayoutOptions::getOffset() const { // :3
    return m_offset; // :3
} // :3

AnchorLayoutOptions* AnchorLayoutOptions::setAnchor(Anchor anchor) { // :3
    m_anchor = anchor; // :3
    return this; // :3
} // :3

AnchorLayoutOptions* AnchorLayoutOptions::setOffset(CCPoint const& offset) { // :3
    m_offset = offset; // :3
    return this; // :3
} // :3

AnchorLayout* AnchorLayout::create() { // :3
    auto ret = new AnchorLayout(); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

void AnchorLayout::apply(CCNode* on) { // :3
    on->ignoreAnchorPointForPosition(false); // :3
    for (auto node : CCArrayExt<CCNode*>(on->getChildren())) { // :3
        if (auto opts = typeinfo_cast<AnchorLayoutOptions*>(node->getLayoutOptions())) { // :3
            auto pos = opts->getOffset(); // :3
            auto size = on->getContentSize(); // :3
            switch (opts->getAnchor()) { // :3
                default: // :3
                case Anchor::Center: pos += size / 2; break; // :3
                case Anchor::TopLeft: pos += ccp(0, size.height); break; // :3
                case Anchor::Top: pos += ccp(size.width / 2, size.height); break; // :3
                case Anchor::TopRight: pos += ccp(size.width, size.height); break; // :3
                case Anchor::Right: pos += ccp(size.width, size.height / 2); break; // :3
                case Anchor::BottomRight: pos += ccp(size.width, 0); break; // :3
                case Anchor::Bottom: pos += ccp(size.width / 2, 0); break; // :3
                case Anchor::BottomLeft: pos += ccp(0, 0); break; // :3
                case Anchor::Left: pos += ccp(0, size.height / 2); break; // :3
            } // :3
            // node->ignoreAnchorPointForPosition(false); :3
            node->setPosition(pos); // :3
        } // :3
    } // :3
} // :3

CCSize AnchorLayout::getSizeHint(CCNode* on) const { // :3
    return on->getContentSize(); // :3
} // :3

CCPoint AnchorLayout::getAnchoredPosition(CCNode* in, Anchor anchor, CCPoint const& offset) { // :3
    auto pos = offset; // :3
    auto size = in->getContentSize(); // :3
    switch (anchor) { // :3
        default: // :3
        case Anchor::Center: pos += size / 2; break; // :3
        case Anchor::TopLeft: pos += ccp(0, size.height); break; // :3
        case Anchor::Top: pos += ccp(size.width / 2, size.height); break; // :3
        case Anchor::TopRight: pos += ccp(size.width, size.height); break; // :3
        case Anchor::Right: pos += ccp(size.width, size.height / 2); break; // :3
        case Anchor::BottomRight: pos += ccp(size.width, 0); break; // :3
        case Anchor::Bottom: pos += ccp(size.width / 2, 0); break; // :3
        case Anchor::BottomLeft: pos += ccp(0, 0); break; // :3
        case Anchor::Left: pos += ccp(0, size.height / 2); break; // :3
    } // :3
    return pos; // :3
} // :3
