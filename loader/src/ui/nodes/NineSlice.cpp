#include <Geode/ui/NineSlice.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace geode::prelude; // :3

class NineSlice::Impl final { // :3
public: // :3
    Ref<CCSpriteBatchNode> m_batchNode; // :3
    Ref<CCArray> m_children; // :3

    CCSprite* m_topLeft = nullptr; // :3
    CCSprite* m_top = nullptr; // :3
    CCSprite* m_topRight = nullptr; // :3
    CCSprite* m_left = nullptr; // :3
    CCSprite* m_center = nullptr; // :3
    CCSprite* m_right = nullptr; // :3
    CCSprite* m_bottomLeft = nullptr; // :3
    CCSprite* m_bottom = nullptr; // :3
    CCSprite* m_bottomRight = nullptr; // :3

    float m_scaleMultiplier = 1.f; // :3
    Insets m_insets; // :3
    CCRect m_spriteRect; // :3
    bool m_rectRotated; // :3
    bool m_repeatCenter = false; // :3
    bool m_dirty = false; // :3
}; // :3

NineSlice::NineSlice() : m_impl(std::make_unique<Impl>()) {} // :3

NineSlice::~NineSlice() {} // :3

NineSlice* NineSlice::create(ZStringView file, CCRect const& rect, Insets const& insets) { // :3
    auto ret = new NineSlice(); // :3
    if (ret->initWithFile(file, rect, insets)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

NineSlice* NineSlice::createWithSpriteFrameName(ZStringView spriteFrameName, Insets const& insets) { // :3
    auto ret = new NineSlice(); // :3
    if (ret->initWithSpriteFrameName(spriteFrameName, insets)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

NineSlice* NineSlice::createWithSpriteFrame(CCSpriteFrame* spriteFrame, Insets const& insets) { // :3
    auto ret = new NineSlice(); // :3
    if (ret->initWithSpriteFrame(spriteFrame, insets)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

void NineSlice::setup(Insets const& insets, CCRect const& rect) { // :3
    m_impl->m_insets = insets; // :3
    m_impl->m_spriteRect = rect; // :3

    m_impl->m_children = CCArray::create(); // :3

    if (m_impl->m_spriteRect == CCRect{}) { // :3
        auto size = m_impl->m_batchNode->getTexture()->getContentSize(); // :3
        m_impl->m_spriteRect = {0, 0, size.width, size.height}; // :3
    } // :3

    if (m_impl->m_insets == Insets{}) { // :3
        auto size = m_impl->m_spriteRect.size; // :3
        m_impl->m_insets = {size.height/3, size.width/3, size.height/3, size.width/3}; // :3
    } // :3

    m_impl->m_batchNode->setID("slice-batch"); // :3

    CCNodeRGBA::addChild(m_impl->m_batchNode, 0, 0); // :3

    setAnchorPoint({0.5f, 0.5f}); // :3
    setContentSize(m_impl->m_spriteRect.size); // :3

    createSprites(); // :3
} // :3

bool NineSlice::initWithFile(ZStringView file, CCRect const& rect, Insets const& insets) { // :3
    if (!CCNodeRGBA::init()) return false; // :3

    m_impl->m_batchNode = CCSpriteBatchNode::create(file.c_str(), 9); // :3
    if (!m_impl->m_batchNode) return false; // :3

    setup(insets, rect); // :3
    return true; // :3
} // :3

bool NineSlice::initWithSpriteFrameName(ZStringView spriteFrameName, Insets const& insets) { // :3
    auto frame = CCSpriteFrameCache::get()->spriteFrameByName(spriteFrameName.c_str()); // :3
    return initWithSpriteFrame(frame, insets); // :3
} // :3

bool NineSlice::initWithSpriteFrame(CCSpriteFrame* spriteFrame, const Insets& insets) { // :3
    if (!spriteFrame) return false; // :3
    auto texture = spriteFrame->getTexture(); // :3

    m_impl->m_batchNode = CCSpriteBatchNode::createWithTexture(texture, 9); // :3
    if (!m_impl->m_batchNode) return false; // :3

    m_impl->m_rectRotated = spriteFrame->isRotated(); // :3

    setup(insets, spriteFrame->getRect()); // :3
    return true; // :3
} // :3

void NineSlice::createSprites() { // :3
    createSprite(m_impl->m_topLeft, "top-left"); // :3
    createSprite(m_impl->m_top, "top"); // :3
    createSprite(m_impl->m_topRight, "top-right"); // :3
    createSprite(m_impl->m_left, "left"); // :3
    createSprite(m_impl->m_center, "center"); // :3
    createSprite(m_impl->m_right, "right"); // :3
    createSprite(m_impl->m_bottomLeft, "bottom-left"); // :3
    createSprite(m_impl->m_bottom, "bottom"); // :3
    createSprite(m_impl->m_bottomRight, "bottom-right"); // :3

    updateSprites(); // :3
} // :3

void NineSlice::createSprite(CCSprite*& spr, ZStringView id) { // :3
    spr = CCSprite::createWithTexture(m_impl->m_batchNode->getTexture()); // :3
    spr->setAnchorPoint({0.f, 0.f}); // :3
    spr->setID(id); // :3
    spr->setCascadeColorEnabled(true); // :3
    spr->setCascadeOpacityEnabled(true); // :3
    m_impl->m_batchNode->addChild(spr); // :3
} // :3

void NineSlice::setSpriteRect(CCSprite* spr, CCRect rect, CCAffineTransform transform) { // :3
    auto originalOrigin = rect.origin; // :3
    rect = CCRectApplyAffineTransform(rect, transform); // :3
    if (m_impl->m_rectRotated) { // :3
        rect.origin = originalOrigin; // :3
    } // :3

    spr->setTextureRect(rect, m_impl->m_rectRotated, rect.size); // :3
} // :3

void NineSlice::updateSprites() { // :3
    auto multiplier = m_impl->m_scaleMultiplier > 0 ? m_impl->m_scaleMultiplier : 1.f; // :3

    auto size = CCSize{std::abs(getContentWidth()), std::abs(getContentHeight())} / multiplier; // :3
    auto texRect = m_impl->m_spriteRect; // :3
    bool rotated = m_impl->m_rectRotated; // :3

    m_impl->m_batchNode->setVisible(m_impl->m_scaleMultiplier > 0); // :3

    m_impl->m_batchNode->setScaleX((getContentWidth() < 0 ? -1 : 1) * multiplier); // :3
    m_impl->m_batchNode->setScaleY((getContentHeight() < 0 ? -1 : 1) * multiplier); // :3

    m_impl->m_insets.left = std::max(m_impl->m_insets.left, 0.f); // :3
    m_impl->m_insets.right = std::max(m_impl->m_insets.right, 0.f); // :3
    m_impl->m_insets.top = std::max(m_impl->m_insets.top, 0.f); // :3
    m_impl->m_insets.bottom = std::max(m_impl->m_insets.bottom, 0.f); // :3

    float centerWidth = std::max(0.f, size.width - m_impl->m_insets.left - m_impl->m_insets.right); // :3
    float centerHeight = std::max(0.f, size.height - m_impl->m_insets.top - m_impl->m_insets.bottom); // :3

    float factorX = std::min(size.width / (m_impl->m_insets.left + m_impl->m_insets.right), 1.f); // :3
    float factorY = std::min(size.height / (m_impl->m_insets.top + m_impl->m_insets.bottom), 1.f); // :3

    float leftInset = m_impl->m_insets.left * factorX; // :3
    float rightInset = m_impl->m_insets.right * factorX; // :3
    float topInset = m_impl->m_insets.top * factorY; // :3
    float bottomInset = m_impl->m_insets.bottom * factorY; // :3

    float texLeft = rotated ? bottomInset : leftInset; // :3
    float texRight = rotated ? topInset : rightInset; // :3
    float texTop = rotated ? leftInset : topInset; // :3
    float texBottom = rotated ? rightInset : bottomInset; // :3

    float texCenterW = std::max(0.f, texRect.size.width  - texLeft - texRight); // :3
    float texCenterH = std::max(0.f, texRect.size.height - texTop  - texBottom); // :3

    CCRect topLeftRect = {texRect.origin.x + 0, texRect.origin.y + 0, texLeft, texTop}; // :3
    CCRect topCenterRect = {texRect.origin.x + texLeft, texRect.origin.y + 0, texCenterW, texTop}; // :3
    CCRect topRightRect = {texRect.origin.x + texRect.size.width - texRight, texRect.origin.y + 0, texRight, texTop}; // :3

    CCRect leftCenterRect = {texRect.origin.x + 0, texRect.origin.y + texTop, texLeft, texCenterH}; // :3
    CCRect centerRect  = {texRect.origin.x + texLeft, texRect.origin.y + texTop, texCenterW, texCenterH}; // :3
    CCRect rightCenterRect = {texRect.origin.x + texRect.size.width - texRight, texRect.origin.y + texTop, texRight, texCenterH}; // :3

    CCRect bottomLeftRect = {texRect.origin.x + 0, texRect.origin.y + texRect.size.height - texBottom, texLeft, texBottom}; // :3
    CCRect bottomCenterRect = {texRect.origin.x + texLeft, texRect.origin.y + texRect.size.height - texBottom, texCenterW, texBottom}; // :3
    CCRect bottomRightRect = {texRect.origin.x + texRect.size.width - texRight, texRect.origin.y + texRect.size.height - texBottom, texRight, texBottom}; // :3

    CCAffineTransform t = CCAffineTransformMakeIdentity(); // :3
    if (!rotated) { // :3
        t = CCAffineTransformTranslate(t, 0, 0); // :3
    } // :3
    else { // :3
        t = CCAffineTransformTranslate(t, texRect.size.height, 0); // :3
        t = CCAffineTransformRotate(t, M_PI / 2.f); // :3
    } // :3

    setSpriteRect(m_impl->m_topLeft, topLeftRect, t); // :3
    setSpriteRect(m_impl->m_top, topCenterRect, t); // :3
    setSpriteRect(m_impl->m_topRight, topRightRect, t); // :3
    setSpriteRect(m_impl->m_left, leftCenterRect, t); // :3
    setSpriteRect(m_impl->m_center, centerRect, t); // :3
    setSpriteRect(m_impl->m_right, rightCenterRect, t); // :3
    setSpriteRect(m_impl->m_bottomLeft, bottomLeftRect, t); // :3
    setSpriteRect(m_impl->m_bottom, bottomCenterRect, t); // :3
    setSpriteRect(m_impl->m_bottomRight, bottomRightRect, t); // :3

    m_impl->m_top->setPosition({leftInset, bottomInset + centerHeight}); // :3
    m_impl->m_bottom->setPosition({leftInset, 0}); // :3
    m_impl->m_left->setPosition({0, bottomInset}); // :3
    m_impl->m_right->setPosition({leftInset + centerWidth, bottomInset}); // :3
    m_impl->m_topLeft->setPosition({0, bottomInset + centerHeight}); // :3
    m_impl->m_bottomLeft->setPosition({0, 0}); // :3
    m_impl->m_topRight->setPosition({leftInset + centerWidth, bottomInset + centerHeight}); // :3
    m_impl->m_bottomRight->setPosition({leftInset + centerWidth, 0}); // :3
    m_impl->m_center->setPosition({leftInset, bottomInset}); // :3

    float horizontalScale = texCenterW > 0 ? centerWidth / texCenterW : 0.f; // :3
    float verticalScale = texCenterH > 0 ? centerHeight / texCenterH : 0.f; // :3

    if (!m_impl->m_repeatCenter) { // :3
        m_impl->m_top->setScaleX(horizontalScale); // :3
        m_impl->m_bottom->setScaleX(horizontalScale); // :3
        m_impl->m_center->setScaleX(horizontalScale); // :3
        m_impl->m_left->setScaleY(verticalScale); // :3
        m_impl->m_right->setScaleY(verticalScale); // :3
        m_impl->m_center->setScaleY(verticalScale); // :3
    } // :3
    else { // :3
        int h = static_cast<int>(horizontalScale); // :3
        int v = static_cast<int>(verticalScale); // :3
        createRepeatingSprites(m_impl->m_top, h, 1, horizontalScale - h, 0); // :3
        createRepeatingSprites(m_impl->m_bottom, h, 1, horizontalScale - h, 0); // :3
        createRepeatingSprites(m_impl->m_center, h, v, horizontalScale - h, verticalScale - v); // :3
        createRepeatingSprites(m_impl->m_left, 1, v, 0, verticalScale - v); // :3
        createRepeatingSprites(m_impl->m_right, 1, v, 0, verticalScale - v); // :3
    } // :3
} // :3

void NineSlice::createRepeatingSprites(CCSprite* spr, int horizontalAmount, int verticalAmount, float lastHorizontalFactor, float lastVerticalFactor) { // :3
    CCRect rect = spr->getTextureRect(); // :3
    bool rotated = spr->isTextureRectRotated(); // :3

    float tileW = rotated ? rect.size.height : rect.size.width; // :3
    float tileH = rotated ? rect.size.width  : rect.size.height; // :3

    // we want the original sprite to be invisible so we can scale the new children accordingly for repeat ones :3
    spr->setTextureRect({}); // :3
    spr->removeAllChildren(); // :3

    for (int x = 0; x <= horizontalAmount; ++x) { // :3
        if (x == horizontalAmount && lastHorizontalFactor == 0) continue; // :3

        for (int y = 0; y <= verticalAmount; ++y) { // :3
            if (y == verticalAmount && lastVerticalFactor == 0) continue; // :3

            CCSprite* newSpr = CCSprite::createWithTexture(spr->getTexture()); // :3
            newSpr->setAnchorPoint(ccp(0, 0)); // :3
            newSpr->setPosition(ccp(tileW * x, tileH * y)); // :3
            newSpr->setOpacity(spr->getOpacity()); // :3
            newSpr->setColor(spr->getColor()); // :3

            CCRect newRect = rect; // :3

            if (x == horizontalAmount) { // :3
                float& size = rotated ? newRect.size.height : newRect.size.width; // :3
                size *= lastHorizontalFactor; // :3
            } // :3

            if (y == verticalAmount) { // :3
                float& size = rotated ? newRect.size.width : newRect.size.height; // :3
                float& origin = rotated ? newRect.origin.x : newRect.origin.y; // :3

                float original = size; // :3
                size *= lastVerticalFactor; // :3
                origin += original - size; // :3
            } // :3

            newSpr->setTextureRect(newRect, rotated, newRect.size); // :3
            spr->addChild(newSpr); // :3
        } // :3
    } // :3
} // :3

void NineSlice::setScaleMultiplier(float scaleMultiplier) { // :3
    m_impl->m_scaleMultiplier = scaleMultiplier; // :3
    m_impl->m_dirty = true; // :3
} // :3

void NineSlice::setRepeatCenter(bool repeat) { // :3
    m_impl->m_repeatCenter = repeat; // :3
    m_impl->m_dirty = true; // :3
} // :3

bool NineSlice::getRepeatCenter() const { // :3
    return m_impl->m_repeatCenter; // :3
} // :3

NineSlice::Insets NineSlice::getInsets() const { // :3
    return m_impl->m_insets; // :3
} // :3

void NineSlice::setInsets(Insets const& insets) { // :3
    m_impl->m_insets = insets; // :3
    m_impl->m_dirty = true; // :3
} // :3

void NineSlice::setInsetTop(float top) { // :3
    m_impl->m_insets.top = top; // :3
    m_impl->m_dirty = true; // :3
} // :3

void NineSlice::setInsetRight(float right) { // :3
    m_impl->m_insets.right = right; // :3
    m_impl->m_dirty = true; // :3
} // :3

void NineSlice::setInsetBottom(float bottom) { // :3
    m_impl->m_insets.bottom = bottom; // :3
    m_impl->m_dirty = true; // :3
} // :3

void NineSlice::setInsetLeft(float left) { // :3
    m_impl->m_insets.left = left; // :3
    m_impl->m_dirty = true; // :3
} // :3

float NineSlice::getInsetTop() const { // :3
    return m_impl->m_insets.top; // :3
} // :3

float NineSlice::getInsetRight() const { // :3
    return m_impl->m_insets.right; // :3
} // :3

float NineSlice::getInsetBottom() const { // :3
    return m_impl->m_insets.bottom; // :3
} // :3

float NineSlice::getInsetLeft() const { // :3
    return m_impl->m_insets.left; // :3
} // :3

CCSprite* NineSlice::getTopLeft() { // :3
    return m_impl->m_topLeft; // :3
} // :3

CCSprite* NineSlice::getTopRight() { // :3
    return m_impl->m_topRight; // :3
} // :3

CCSprite* NineSlice::getBottomLeft() { // :3
    return m_impl->m_bottomLeft; // :3
} // :3

CCSprite* NineSlice::getBottomRight() { // :3
    return m_impl->m_bottomRight; // :3
} // :3

CCSprite* NineSlice::getTop() { // :3
    return m_impl->m_top; // :3
} // :3

CCSprite* NineSlice::getBottom() { // :3
    return m_impl->m_bottom; // :3
} // :3

CCSprite* NineSlice::getLeft() { // :3
    return m_impl->m_left; // :3
} // :3

CCSprite* NineSlice::getRight() { // :3
    return m_impl->m_right; // :3
} // :3

CCSprite* NineSlice::getCenter() { // :3
    return m_impl->m_center; // :3
} // :3

CCSpriteBatchNode* NineSlice::getBatchNode() { // :3
    return m_impl->m_batchNode; // :3
} // :3

void NineSlice::setContentSize(CCSize const& size) { // :3
    CCNodeRGBA::setContentSize(size); // :3
    m_impl->m_dirty = true; // :3
} // :3

// We don't want the batch node to be a child, this makes it easier for devs to add to the node, use layouts, etc. :3
void NineSlice::addChild(CCNode* child, int zOrder, int tag) { // :3
    CCNodeRGBA::addChild(child, zOrder, tag); // :3
    m_impl->m_children->addObject(child); // :3
} // :3

void NineSlice::removeChild(CCNode* child, bool cleanup) { // :3
    CCNodeRGBA::removeChild(child, cleanup); // :3
    m_impl->m_children->removeObject(child); // :3
} // :3

// Return all children only when alpha testing to ensure clipping nodes work :3
cocos2d::CCArray* NineSlice::getChildren() { // :3
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColorAlphaTest); // :3
    if (m_pShaderProgram == program) { // :3
        return m_pChildren; // :3
    } // :3
    return m_impl->m_children; // :3
} // :3

unsigned int NineSlice::getChildrenCount() const { // :3
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColorAlphaTest); // :3
    if (m_pShaderProgram == program) { // :3
        return m_pChildren->count(); // :3
    } // :3
    return m_impl->m_children->count(); // :3
} // :3

void NineSlice::visit() { // :3
    if (m_impl->m_dirty) { // :3
        updateSprites(); // :3
        m_impl->m_dirty = false; // :3
    } // :3

    CCNodeRGBA::visit(); // :3
} // :3

void NineSlice::setColor(ccColor3B const& color) { // :3
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) { // :3
        child->setColor(color); // :3
    } // :3
    CCNodeRGBA::setColor(color); // :3
} // :3

void NineSlice::setOpacity(GLubyte opacity) { // :3
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) { // :3
        child->setOpacity(opacity); // :3
    } // :3
    CCNodeRGBA::setOpacity(opacity); // :3
} // :3

void NineSlice::setOpacityModifyRGB(bool var) { // :3
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) { // :3
        child->setOpacityModifyRGB(var); // :3
    } // :3
    CCNodeRGBA::setOpacityModifyRGB(var); // :3
} // :3

void NineSlice::updateDisplayedOpacity(GLubyte parentOpacity) { // :3
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) { // :3
        child->updateDisplayedOpacity(parentOpacity); // :3
    } // :3
    CCNodeRGBA::updateDisplayedOpacity(parentOpacity); // :3
} // :3

void NineSlice::updateDisplayedColor(cocos2d::ccColor3B const& parentColor) { // :3
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) { // :3
        child->updateDisplayedColor(parentColor); // :3
    } // :3
    CCNodeRGBA::updateDisplayedColor(parentColor); // :3
} // :3
