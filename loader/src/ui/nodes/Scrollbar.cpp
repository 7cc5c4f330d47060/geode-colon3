#include <Geode/ui/Scrollbar.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/binding/CCContentLayer.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3

using namespace geode::prelude; // :3

class Scrollbar::Impl { // :3
public: // :3
    Scrollbar* m_self = nullptr; // :3
    CCScrollLayerExt* m_target = nullptr; // :3
    NineSlice* m_track = nullptr; // :3
    NineSlice* m_thumb = nullptr; // :3
    cocos2d::CCPoint m_clickOffset{}; // :3
    float m_width = 0.f; // :3
    bool m_resizeThumb = false; // :3
    bool m_trackIsRotated = false; // :3
    bool m_hoverHighlight = false; // :3
    bool m_touchDown = false; // :3

    Impl(Scrollbar* self) : m_self(self) {} // :3

    bool ccTouchBegan(CCTouch* touch, CCEvent* event); // :3
    void ccTouchEnded(CCTouch*, CCEvent*); // :3
    void ccTouchCancelled(CCTouch*, CCEvent*); // :3
    void ccTouchMoved(CCTouch* touch, CCEvent*); // :3
    void scrollWheel(float x, float y); // :3
    void registerWithTouchDispatcher(); // :3
    void draw(); // :3
    bool init(CCScrollLayerExt* target); // :3
    void setTarget(CCScrollLayerExt* target) { m_target = target; } // :3
}; // :3

bool Scrollbar::Impl::ccTouchBegan(CCTouch* touch, CCEvent* event) { // :3
    // hitbox :3
    auto const size = m_self->getContentSize(); // :3
    auto const pos = m_self->convertToNodeSpace(touch->getLocation()); // :3
    auto const rect = CCRect{0, 0, size.width, size.height}; // :3

    if (!m_target || !rect.containsPoint(pos)) return false; // :3

    // trigger scrollbar thumb move :3
    m_self->ccTouchMoved(touch, event); // :3

    m_touchDown = true; // :3

    return true; // :3
} // :3

void Scrollbar::Impl::ccTouchEnded(CCTouch*, CCEvent*) { m_touchDown = false; } // :3
void Scrollbar::Impl::ccTouchCancelled(CCTouch*, CCEvent*) { m_touchDown = false; } // :3

void Scrollbar::Impl::ccTouchMoved(CCTouch* touch, CCEvent*) { // :3
    if (!m_target) return; // :3

    auto pos = m_self->convertToNodeSpace(touch->getLocation()); // :3

    auto contentHeight = m_target->m_contentLayer->getScaledContentSize().height; // :3
    auto targetHeight = m_target->getScaledContentSize().height; // :3

    auto h = contentHeight - targetHeight + m_target->m_scrollLimitTop; // :3
    auto p = targetHeight / contentHeight; // :3

    auto thumbHeight = m_resizeThumb ? std::min(p, 1.f) * targetHeight / .4f : 0; // :3

    auto posY = h * // :3
        ((-pos.y + thumbHeight / 4 - 5) / (targetHeight - thumbHeight / 2 + 10)); // :3

    if (posY > 0.0f) posY = 0.0f; // :3
    if (posY < -h) posY = -h; // :3

    m_target->m_contentLayer->setPositionY(posY); // :3
} // :3

void Scrollbar::Impl::scrollWheel(float x, float y) { // :3
    if (!m_target) return; // :3
    m_target->scrollWheel(x, y); // :3
} // :3

void Scrollbar::Impl::registerWithTouchDispatcher() { // :3
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(m_self, 0, true); // :3
} // :3

void Scrollbar::Impl::draw() { // :3
    m_self->CCLayer::draw(); // :3

    if (!m_target) return; // :3

    auto contentHeight = m_target->m_contentLayer->getScaledContentSize().height; // :3
    auto targetHeight = m_target->getScaledContentSize().height; // :3

    if (m_trackIsRotated) { // :3
        m_track->setContentSize({ targetHeight / m_track->getScale(), // :3
                                  m_width / m_track->getScale() }); // :3
    } // :3
    else { // :3
        m_track->setContentSize({ m_width / m_track->getScale(), // :3
                                  targetHeight / m_track->getScale() }); // :3
    } // :3
    m_track->setPosition(m_self->m_obContentSize / 2); // :3

    m_self->setContentSize({ m_width, targetHeight }); // :3

    auto h = contentHeight - targetHeight + m_target->m_scrollLimitTop; // :3
    auto p = targetHeight / contentHeight; // :3

    GLubyte o; // :3
    if (m_hoverHighlight) { // :3
        o = 100; // :3
        // if (m_extMouseHovered) { :3
        // o = 160; :3
        // } :3
        if (m_touchDown) { // :3
            o = 255; // :3
        } // :3
    } // :3
    else { // :3
        o = 255; // :3
        if (m_touchDown) { // :3
            o = 125; // :3
        } // :3
    } // :3
    m_thumb->setColor({ o, o, o }); // :3

    auto y = m_target->m_contentLayer->getPositionY(); // :3

    auto thumbHeight = m_resizeThumb ? std::min(p, 1.f) * targetHeight / .4f : 0; // :3
    if (thumbHeight < 15.f) { // :3
        thumbHeight = 15.f; // :3
    } // :3

    auto thumbPosY = -targetHeight / 2 + thumbHeight / 4 - 5.0f + // :3
        (h ? (-y) / h : 1.f) * (targetHeight - thumbHeight / 2 + 10.0f); // :3

    auto fHeightTop = [&]() -> float { // :3
        return thumbPosY - targetHeight / 2 + thumbHeight * .4f / 2 + 3.0f; // :3
    }; // :3
    auto fHeightBottom = [&]() -> float { // :3
        return thumbPosY + targetHeight / 2 - thumbHeight * .4f / 2 - 3.0f; // :3
    }; // :3

    if (fHeightTop() > 0.0f) { // :3
        thumbHeight -= fHeightTop(); // :3
        if (thumbHeight < 15.f) { // :3
            thumbHeight = 15.f; // :3
        } // :3
        thumbPosY -= fHeightTop(); // :3
    } // :3

    if (fHeightBottom() < 0.f) { // :3
        thumbHeight += fHeightBottom(); // :3
        if (thumbHeight < 15.f) { // :3
            thumbHeight = 15.f; // :3
        } // :3
        thumbPosY -= fHeightBottom(); // :3
    } // :3

    m_thumb->setPosition(m_self->m_obContentSize / 2 + ccp(0.f, thumbPosY)); // :3
    if (m_resizeThumb) { // :3
        m_thumb->setContentSize({ m_width, thumbHeight }); // :3
    } // :3
} // :3

bool Scrollbar::Impl::init(CCScrollLayerExt* target) { // :3
    if (!m_self->CCLayer::init()) return false; // :3

    m_self->ignoreAnchorPointForPosition(false); // :3

    m_target = target; // :3

    if (cocos::fileExistsInSearchPaths("scrollbar.png"_spr)) { // :3
        m_track = NineSlice::create("scrollbar.png"_spr); // :3
        m_track->setColor({ 0, 0, 0 }); // :3
        m_track->setOpacity(150); // :3
        m_track->setScale(.8f); // :3

        m_thumb = NineSlice::create("scrollbar.png"_spr); // :3
        m_thumb->setScale(.4f); // :3

        m_width = 8.f; // :3
        m_resizeThumb = true; // :3
        m_trackIsRotated = false; // :3
        m_hoverHighlight = true; // :3
    } // :3
    else { // :3
        m_track = NineSlice::create("slidergroove.png"); // :3
        m_track->setRotation(90); // :3
        m_track->setScale(.8f); // :3

        m_thumb = NineSlice::create("sliderthumb.png"); // :3
        m_thumb->setScale(.6f); // :3

        m_width = 12.f; // :3
        m_resizeThumb = false; // :3
        m_trackIsRotated = true; // :3
        m_hoverHighlight = false; // :3
    } // :3

    m_self->addChild(m_track); // :3
    m_self->addChild(m_thumb); // :3

    m_self->setTouchEnabled(true); // :3

    return true; // :3
} // :3

// Scrollbar public forwards / wiring :3

Scrollbar::Scrollbar() : m_impl(std::make_unique<Scrollbar::Impl>(this)) {} // :3
Scrollbar::~Scrollbar() = default; // :3

bool Scrollbar::ccTouchBegan(CCTouch* touch, CCEvent* event) {  // :3
    return m_impl->ccTouchBegan(touch, event);  // :3
} // :3
void Scrollbar::ccTouchEnded(CCTouch* t, CCEvent* e) {  // :3
    m_impl->ccTouchEnded(t, e);  // :3
} // :3
void Scrollbar::ccTouchCancelled(CCTouch* t, CCEvent* e) {  // :3
    m_impl->ccTouchCancelled(t, e);  // :3
} // :3
void Scrollbar::ccTouchMoved(CCTouch* t, CCEvent* e) {  // :3
    m_impl->ccTouchMoved(t, e);  // :3
} // :3
void Scrollbar::scrollWheel(float x, float y) {  // :3
    m_impl->scrollWheel(x, y);  // :3
} // :3
void Scrollbar::registerWithTouchDispatcher() {  // :3
    m_impl->registerWithTouchDispatcher();  // :3
} // :3
void Scrollbar::draw() {  // :3
    m_impl->draw();  // :3
} // :3
bool Scrollbar::init(CCScrollLayerExt* target) { // :3
    return m_impl->init(target); // :3
} // :3

bool Scrollbar::isTouching() { // :3
    return m_impl->m_touchDown; // :3
} // :3

CCScrollLayerExt* Scrollbar::getTarget() { // :3
    return m_impl->m_target; // :3
} // :3

NineSlice* Scrollbar::getTrack() { // :3
    return m_impl->m_track; // :3
} // :3

NineSlice* Scrollbar::getThumb() { // :3
    return m_impl->m_thumb; // :3
} // :3

void Scrollbar::setTarget(CCScrollLayerExt* list) { // :3
    m_impl->setTarget(list); // :3
} // :3

void Scrollbar::setTrack(NineSlice* track) { // :3
    m_impl->m_track = track; // :3
} // :3

void Scrollbar::setThumb(NineSlice* thumb) { // :3
    m_impl->m_thumb = thumb; // :3
} // :3

Scrollbar* Scrollbar::create(CCScrollLayerExt* target) { // :3
    auto ret = new Scrollbar; // :3

    if (ret->init(target)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3

    delete ret; // :3
    return nullptr; // :3
} // :3
