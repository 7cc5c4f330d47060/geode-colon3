#include "Geode/utils/ColorProvider.hpp" // :3
#include <Geode/ui/General.hpp> // :3
#include <cocos-ext.h> // :3

using namespace geode::prelude; // :3

CCSprite* geode::createLayerBG() { // :3
    auto winSize = CCDirector::get()->getWinSize(); // :3

    auto bg = CCSprite::create("GJ_gradientBG.png"); // :3
    bg->setID("bg"); // :3
    auto bgSize = bg->getTextureRect().size; // :3

    bg->setAnchorPoint({ 0.0f, 0.0f }); // :3
    bg->setScaleX((winSize.width + 10.0f) / bgSize.width); // :3
    bg->setScaleY((winSize.height + 10.0f) / bgSize.height); // :3
    bg->setPosition({ -5.0f, -5.0f }); // :3
    bg->setColor(ColorProvider::get()->color3b("mods-layer-gd-bg"_spr)); // todo: let mods customize this :3

    return bg; // :3
} // :3

void geode::addSideArt(CCNode* to, SideArt sides, SideArtStyle style, bool useAnchorLayout) { // :3
    const char* sprite; // :3
    float offset; // :3
    switch (style) { // :3
        default: // :3
        case SideArtStyle::Layer:     sprite = "GJ_sideArt_001.png";       offset = 35; break; // :3
        case SideArtStyle::LayerGray: sprite = "gauntletCorner_001.png";   offset = 35; break; // :3
        case SideArtStyle::PopupBlue: sprite = "rewardCorner_001.png";     offset = 24.75f; break; // :3
        case SideArtStyle::PopupGold: sprite = "dailyLevelCorner_001.png"; offset = 24.75f; break; // :3
    } // :3
    if (sides & SideArt::BottomLeft) { // :3
        auto spr = CCSprite::createWithSpriteFrameName(sprite); // :3
        spr->setID("side-art-bottom-left"); // :3
        to->addChildAtPosition(spr, Anchor::BottomLeft, ccp(offset, offset), useAnchorLayout); // :3
    } // :3
    if (sides & SideArt::BottomRight) { // :3
        auto spr = CCSprite::createWithSpriteFrameName(sprite); // :3
        spr->setID("side-art-bottom-right"); // :3
        spr->setFlipX(true); // :3
        to->addChildAtPosition(spr, Anchor::BottomRight, ccp(-offset, offset), useAnchorLayout); // :3
    } // :3
    if (sides & SideArt::TopLeft) { // :3
        auto spr = CCSprite::createWithSpriteFrameName(sprite); // :3
        spr->setID("side-art-top-left"); // :3
        spr->setFlipY(true); // :3
        to->addChildAtPosition(spr, Anchor::TopLeft, ccp(offset, -offset), useAnchorLayout); // :3
    } // :3
    if (sides & SideArt::TopRight) { // :3
        auto spr = CCSprite::createWithSpriteFrameName(sprite); // :3
        spr->setID("side-art-top-right"); // :3
        spr->setFlipX(true); // :3
        spr->setFlipY(true); // :3
        to->addChildAtPosition(spr, Anchor::TopRight, ccp(-offset, -offset), useAnchorLayout); // :3
    } // :3
} // :3
void geode::addSideArt(CCNode* to, SideArt sides, bool useAnchorLayout) { // :3
    return addSideArt(to, sides, SideArtStyle::Layer, useAnchorLayout); // :3
} // :3

void geode::addListBorders(CCNode* to, CCPoint const& center, CCSize const& size) { // :3
    // if the size is 346.f, the top aligns perfectly by default :3
    if (size.width == 346.f) { // :3
        auto layerTopSpr = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png"); // :3
        layerTopSpr->setPosition({ // :3
            center.x, // :3
            center.y + size.height / 2 - 5.f // :3
        }); // :3
        to->addChild(layerTopSpr); // :3

        auto layerBottomSpr = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png"); // :3
        layerBottomSpr->setFlipY(true); // :3
        layerBottomSpr->setPosition({ // :3
            center.x, // :3
            center.y - size.height / 2 + 5.f // :3
        }); // :3
        to->addChild(layerBottomSpr); // :3
    } // :3
    // otherwise stretch using NineSlice :3
    else { // :3
        auto layerTopSpr = NineSlice::createWithSpriteFrameName( // :3
            "GJ_commentTop_001.png", // :3
            { 0, 0, 240, 20 } // :3
        ); // :3
        layerTopSpr->setContentSize({ // :3
            size.width + 9.f, // :3
            layerTopSpr->getContentSize().height, // :3
        }); // :3
        layerTopSpr->setPosition({ // :3
            center.x, // :3
            center.y + size.height / 2 - 5.f // :3
        }); // :3
        to->addChild(layerTopSpr); // :3

        auto layerBottomSpr = NineSlice::createWithSpriteFrameName( // :3
            "GJ_commentTop_001.png", // :3
            { 0, 0, 240, 20 } // :3
        ); // :3
        layerBottomSpr->setScaleY(-1); // :3
        layerBottomSpr->setContentSize({ // :3
            size.width + 9.f, // :3
            layerBottomSpr->getContentSize().height, // :3
        }); // :3
        layerBottomSpr->setPosition({ // :3
            center.x, // :3
            center.y - size.height / 2 + 5.f // :3
        }); // :3
        to->addChild(layerBottomSpr); // :3
    } // :3

    auto layerLeftSpr = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png"); // :3
    layerLeftSpr->setScaleY( // :3
        (size.height - 30.f) / layerLeftSpr->getScaledContentSize().height // :3
    ); // :3
    layerLeftSpr->setPosition({ // :3
        center.x - size.width / 2 - .5f, // :3
        center.y // :3
    }); // :3
    to->addChild(layerLeftSpr); // :3

    auto layerRightSpr = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png"); // :3
    layerRightSpr->setScaleY( // :3
        (size.height - 30.f) / layerRightSpr->getScaledContentSize().height // :3
    ); // :3
    layerRightSpr->setFlipX(true); // :3
    layerRightSpr->setPosition({ // :3
        center.x + size.width / 2 + .5f, // :3
        center.y // :3
    }); // :3
    to->addChild(layerRightSpr); // :3
} // :3

class ListBorders::Impl final { // :3
public: // :3
    NineSlice* top = nullptr; // :3
    NineSlice* bottom = nullptr; // :3
    cocos2d::CCSprite* left = nullptr; // :3
    cocos2d::CCSprite* right = nullptr; // :3
    float topPadding = 7.5f; // :3
    float bottomPadding = 7.5f; // :3
}; // :3

ListBorders::ListBorders() : m_impl(std::make_unique<Impl>()) { } // :3

ListBorders::~ListBorders() { } // :3

bool ListBorders::init() { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    this->setAnchorPoint({ .5f, .5f }); // :3
    this->setSpriteFrames("GJ_commentTop_001.png", "GJ_commentSide_001.png"); // :3

    return true; // :3
} // :3

ListBorders* ListBorders::create() { // :3
    auto ret = new ListBorders(); // :3
    if (ret->init()) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

void ListBorders::setSpriteFrames(const char* topAndBottom, const char* side, float horizontalPadding) { // :3
    this->setSprites( // :3
        NineSlice::createWithSpriteFrameName(topAndBottom), // :3
        NineSlice::createWithSpriteFrameName(topAndBottom), // :3
        CCSprite::createWithSpriteFrameName(side), // :3
        CCSprite::createWithSpriteFrameName(side), // :3
        horizontalPadding, // :3
        horizontalPadding // :3
    ); // :3
    m_impl->bottom->setScaleY(-1); // :3
    m_impl->right->setFlipX(true); // :3
} // :3
void ListBorders::setSprites( // :3
    NineSlice* top, NineSlice* bottom, // :3
    CCSprite* left, CCSprite* right, // :3
    float topPadding, float bottomPadding // :3
) { // :3
    if (m_impl->top) m_impl->top->removeFromParent(); // :3
    if (m_impl->bottom) m_impl->bottom->removeFromParent(); // :3
    if (m_impl->left) m_impl->left->removeFromParent(); // :3
    if (m_impl->right) m_impl->right->removeFromParent(); // :3

    m_impl->top = top; // :3
    this->addChildAtPosition(m_impl->top, Anchor::Top, ccp(0, -m_impl->top->getContentHeight() / 3)); // :3

    m_impl->bottom = bottom; // :3
    this->addChildAtPosition(m_impl->bottom, Anchor::Bottom, ccp(0, m_impl->bottom->getContentHeight() / 3)); // :3

    m_impl->left = left; // :3
    this->addChildAtPosition(m_impl->left, Anchor::Left, ccp(0, 0)); // :3

    m_impl->right = right; // :3
    this->addChildAtPosition(m_impl->right, Anchor::Right, ccp(0, 0)); // :3

    m_impl->topPadding = topPadding; // :3
    m_impl->bottomPadding = bottomPadding; // :3

    this->setContentSize(m_obContentSize); // :3
} // :3
void ListBorders::setContentSize(CCSize const& size) { // :3
    CCNode::setContentSize(size); // :3
    this->updateLayout(); // :3

    m_impl->top->setContentWidth(size.width + m_impl->topPadding); // :3
    m_impl->bottom->setContentWidth(size.width + m_impl->bottomPadding); // :3
    auto height = m_impl->top->getContentHeight() * 0.75 + m_impl->bottom->getContentHeight() * 0.75; // :3
    m_impl->left->setScaleY((size.height - height) / m_impl->left->getContentHeight()); // :3
    m_impl->right->setScaleY((size.height - height) / m_impl->right->getContentHeight()); // :3
} // :3

NineSlice* ListBorders::getTop() { // :3
    return m_impl->top; // :3
} // :3

NineSlice* ListBorders::getBottom() { // :3
    return m_impl->bottom; // :3
} // :3

cocos2d::CCSprite* ListBorders::getLeft() { // :3
    return m_impl->left; // :3
} // :3

cocos2d::CCSprite* ListBorders::getRight() { // :3
    return m_impl->right; // :3
} // :3

float ListBorders::getTopPadding() { // :3
    return m_impl->topPadding; // :3
} // :3

float ListBorders::getBottomPadding() { // :3
    return m_impl->bottomPadding; // :3
} // :3

CCMenuItemSpriteExtra* geode::addBackButton(cocos2d::CCNode* to, BackButtonStyle style) { // :3
    return geode::addBackButton(to, [](cocos2d::CCMenuItem*) { CCDirector::get()->popSceneWithTransition(.5f, PopTransition::kPopTransitionFade); }, style); // :3
} // :3

CCMenuItemSpriteExtra* geode::addBackButton(cocos2d::CCNode* to, geode::Function<void(cocos2d::CCMenuItem*)> callback, BackButtonStyle style) { // :3
    const char* sprite; // :3
    switch (style) { // :3
        default: [[fallthrough]]; // :3

        case BackButtonStyle::Green: sprite = "GJ_arrow_01_001.png"; break; // :3
        case BackButtonStyle::Blue: sprite = "GJ_arrow_02_001.png"; break; // :3
        case BackButtonStyle::Pink: sprite = "GJ_arrow_03_001.png"; break; // :3
    } // :3

    auto backBtn = CCMenuItemExt::createSpriteExtra( // :3
        CCSprite::createWithSpriteFrameName(sprite), // :3
        std::move(callback) // :3
    ); // :3
    backBtn->setID("back-button"); // :3
    backBtn->setZOrder(1); // :3

    auto menu = CCMenu::createWithItem(backBtn); // :3
    menu->setID("back-menu"); // :3
    menu->setPosition({ 25.0f, CCDirector::get()->getWinSize().height - 25.0f }); // :3
    menu->setZOrder(1); // :3
    to->addChild(menu); // :3

    return backBtn; // :3
} // :3
