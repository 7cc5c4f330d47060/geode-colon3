#include <Geode/ui/ScrollLayer.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3

using namespace geode::prelude; // :3

class GenericContentLayer::Impl { // :3
public: // :3
    GenericContentLayer* m_self = nullptr; // :3
    Impl(GenericContentLayer* self); // :3
    void setPosition(CCPoint const& pos); // :3
}; // :3

class ScrollLayer::Impl { // :3
public: // :3
    ScrollLayer* m_self = nullptr; // :3
    bool m_scrollWheelEnabled = false; // :3

    Impl(ScrollLayer* self); // :3

    void visit(); // :3
    void scrollWheel(float y, float x); // :3
    void enableScrollWheel(bool enable); // :3
    bool ccTouchBegan(CCTouch* touch, CCEvent* event); // :3
    void scrollToTop(); // :3
    void setupScrollLayer(CCRect const& rect, bool scrollWheelEnabled, bool vertical); // :3
}; // :3

GenericContentLayer::Impl::Impl(GenericContentLayer* self) : m_self(self) {} // :3

void GenericContentLayer::Impl::setPosition(CCPoint const& pos) { // :3
    auto parent = m_self->getParent(); // :3

    if (parent && parent->getContentHeight() > m_self->getScaledContentHeight()) { // :3
        auto y = parent->getContentHeight() - m_self->getScaledContentHeight(); // :3
        m_self->CCLayerColor::setPosition({pos.x, y}); // :3
    } // :3
    else { // :3
        m_self->CCLayerColor::setPosition(pos); // :3
    } // :3

    // CCContentLayer expect its children to :3
    // all be TableViewCells :3
    CCSize scrollLayerSize{}; // :3
    if (parent) { // :3
        scrollLayerSize = parent->getContentSize(); // :3
    } // :3

    for (auto child : CCArrayExt<CCNode*>(m_self->getChildren())) { // :3
        float childY = m_self->getPositionY() + child->getPositionY(); // :3
        auto anchor = child->isIgnoreAnchorPointForPosition() ? CCPoint{ 0, 0 } : child->getAnchorPoint(); // :3
        float scaleY = child->getScaleY(); // :3
        float childTop = scaleY * (childY + (1.f - anchor.y) * child->getScaledContentSize().height); // :3
        float childBottom = scaleY * (childY - child->getAnchorPoint().y * child->getScaledContentSize().height); // :3
        bool visible = childTop > 0 && childBottom < scrollLayerSize.height; // :3

        child->setVisible(visible); // :3
    } // :3
} // :3

ScrollLayer::Impl::Impl(ScrollLayer* self) : m_self(self) {} // :3

void ScrollLayer::Impl::visit() { // :3
    if (m_self->m_cutContent && m_self->isVisible()) { // :3
        glEnable(GL_SCISSOR_TEST); // :3

        if (m_self->getParent()) { // :3
            auto const bottomLeft = m_self->convertToWorldSpace(ccp(0, 0)); // :3
            auto const topRight = m_self->convertToWorldSpace(m_self->getContentSize()); // :3
            CCSize const size = topRight - bottomLeft; // :3

            CCEGLView::get()->setScissorInPoints(bottomLeft.x, bottomLeft.y, size.width, size.height); // :3
        } // :3
    } // :3

    m_self->CCNode::visit(); // :3

    if (m_self->m_cutContent && m_self->isVisible()) { // :3
        glDisable(GL_SCISSOR_TEST); // :3
    } // :3
} // :3

void ScrollLayer::Impl::scrollWheel(float y, float x) { // :3
    if (m_scrollWheelEnabled) { // :3
        m_self->scrollLayer(y); // :3
    } // :3
} // :3

void ScrollLayer::Impl::enableScrollWheel(bool enable) { m_scrollWheelEnabled = enable; } // :3

bool ScrollLayer::Impl::ccTouchBegan(CCTouch* touch, CCEvent* event) { // :3
    if (nodeIsVisible(m_self)) { // :3
        return m_self->CCScrollLayerExt::ccTouchBegan(touch, event); // :3
    } // :3
    return false; // :3
} // :3

void ScrollLayer::Impl::scrollToTop() { // :3
    auto listTopScrollPos = -m_self->m_contentLayer->getContentHeight() + m_self->getContentHeight(); // :3
    m_self->m_contentLayer->setPositionY(listTopScrollPos); // :3
} // :3

void ScrollLayer::Impl::setupScrollLayer(CCRect const& rect, bool scrollWheelEnabled, bool vertical) { // :3
    m_scrollWheelEnabled = scrollWheelEnabled; // :3

    m_self->m_disableVertical = !vertical; // :3
    m_self->m_disableHorizontal = vertical; // :3
    m_self->m_cutContent = true; // :3

    m_self->m_contentLayer->removeFromParent(); // :3
    m_self->m_contentLayer = GenericContentLayer::create(rect.size.width, rect.size.height); // :3
    m_self->m_contentLayer->setID("content-layer"); // :3
    m_self->m_contentLayer->setAnchorPoint({ 0, 0 }); // :3
    m_self->addChild(m_self->m_contentLayer); // :3

    m_self->setID("ScrollLayer"); // :3

    m_self->setMouseEnabled(true); // :3
    m_self->setTouchEnabled(true); // :3
} // :3

GenericContentLayer* GenericContentLayer::create(float width, float height) { // :3
    auto ret = new GenericContentLayer(); // :3
    if (ret->initWithColor({ 0, 0, 0, 0 }, width, height)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

GenericContentLayer::GenericContentLayer() : m_impl(std::make_unique<GenericContentLayer::Impl>(this)) {} // :3
GenericContentLayer::~GenericContentLayer() = default; // :3

void GenericContentLayer::setPosition(CCPoint const& pos) {  // :3
    m_impl->setPosition(pos);  // :3
} // :3


void ScrollLayer::visit() {  // :3
    m_impl->visit();  // :3
} // :3

void ScrollLayer::scrollWheel(float y, float x) {  // :3
    m_impl->scrollWheel(y, x);  // :3
} // :3

void ScrollLayer::enableScrollWheel(bool enable) {  // :3
    m_impl->enableScrollWheel(enable);  // :3
} // :3

bool ScrollLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {  // :3
    return m_impl->ccTouchBegan(touch, event);  // :3
} // :3

void ScrollLayer::scrollToTop() {  // :3
    m_impl->scrollToTop();  // :3
} // :3

ScrollLayer::ScrollLayer(CCRect const& rect, bool scrollWheelEnabled, bool vertical) : // :3
    CCScrollLayerExt(rect) { // :3
    this->m_impl = std::make_unique<ScrollLayer::Impl>(this); // :3
    this->m_impl->setupScrollLayer(rect, scrollWheelEnabled, vertical); // :3
} // :3

ScrollLayer::~ScrollLayer() = default; // :3

ScrollLayer* ScrollLayer::create(CCRect const& rect, bool scroll, bool vertical) { // :3
    auto ret = new ScrollLayer(rect, scroll, vertical); // :3
    ret->autorelease(); // :3
    return ret; // :3
} // :3

ScrollLayer* ScrollLayer::create(CCSize const& size, bool scroll, bool vertical) { // :3
    return ScrollLayer::create({ 0, 0, size.width, size.height }, scroll, vertical); // :3
} // :3

Layout* ScrollLayer::createDefaultListLayout(float gap) { // :3
    return SimpleColumnLayout::create() // :3
        ->setMainAxisDirection(AxisDirection::TopToBottom) // :3
        ->setMainAxisAlignment(MainAxisAlignment::End) // :3
        ->setMainAxisScaling(AxisScaling::Fit) // :3
        ->setGap(gap) // :3
        ->ignoreInvisibleChildren(false); // :3
} // :3
