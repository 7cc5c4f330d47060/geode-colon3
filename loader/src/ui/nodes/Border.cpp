#include <Geode/ui/Border.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace geode::prelude; // :3

class Border::Impl final { // :3
public: // :3
    Padding padding; // :3
}; // :3

Border* Border::create(CCNode* node, const ccColor4B& backgroundColor, const CCSize& size, const CCPoint& padding) { // :3
    Border* instance = new Border(padding); // :3

    if (instance->init(node, backgroundColor, size)) { // :3
        instance->autorelease(); // :3
        return instance; // :3
    } // :3

    delete instance; // :3
    return nullptr; // :3
} // :3

Border::Border(const CCPoint& padding) : m_impl(std::make_unique<Impl>()) { // :3
    m_impl->padding = { padding.x, padding.y, padding.x, padding.y }; // :3
} // :3

 Border::~Border() { } // :3

bool Border::init(const ccColor4B& backgroundColor, const CCSize& size) { // :3
    return this->init(nullptr, backgroundColor, size); // :3
} // :3

bool Border::init(CCNode* node, const ccColor4B& backgroundColor, const CCSize& size) { // :3
    if (!this->initWithColor(backgroundColor)) { // :3
        return false; // :3
    } // :3

    NineSlice* border = NineSlice::create("inverseborder.png"_spr); // :3
    CCLayer* content = CCLayer::create(); // :3

    border->setID("border_sprite"_spr); // :3
    border->setAnchorPoint({ 0, 0 }); // :3
    border->setPosition({ 0, 0 }); // :3
    border->setZOrder(1); // :3
    content->setID("border_content"_spr); // :3
    content->setContentSize(size); // :3
    this->addChild(border); // :3
    this->addChild(content); // :3

    if (node != nullptr) { // :3
        this->setNode(node); // :3
    } // :3

    this->setSize(size); // :3

    return true; // :3
} // :3

void Border::setPadding(const CCPoint& padding) { // :3
    this->setPadding(padding.x, padding.y); // :3
} // :3

void Border::setPadding(float x, float y) { // :3
    this->setPaddingX(x); // :3
    this->setPaddingY(y); // :3
} // :3

void Border::setPadding(float padding) { // :3
    this->setPadding(padding, padding); // :3
} // :3

void Border::setPaddingX(float x) { // :3
    this->setPaddingLeft(x); // :3
    this->setPaddingRight(x); // :3
} // :3

Border::Padding Border::getPadding() { // :3
    return m_impl->padding; // :3
} // :3

float Border::getPaddingX() { // :3
    return (m_impl->padding.left + m_impl->padding.right) / 2; // :3
} // :3

void Border::setPaddingY(float y) { // :3
    this->setPaddingTop(y); // :3
    this->setPaddingBottom(y); // :3
} // :3

float Border::getPaddingY() { // :3
    return (m_impl->padding.top + m_impl->padding.bottom) / 2; // :3
} // :3

void Border::setPaddingTop(float top) { // :3
    m_impl->padding.top = top; // :3

    this->updatePadding(); // :3
} // :3

float Border::getPaddingTop() { // :3
    return m_impl->padding.top; // :3
} // :3

void Border::setPaddingRight(float right) { // :3
    m_impl->padding.right = right; // :3

    this->updatePadding(); // :3
} // :3

float Border::getPaddingRight() { // :3
    return m_impl->padding.right; // :3
} // :3

void Border::setPaddingBottom(float bottom) { // :3
    m_impl->padding.bottom = bottom; // :3

    this->updatePadding(); // :3
} // :3

float Border::getPaddingBottom() { // :3
    return m_impl->padding.bottom; // :3
} // :3

void Border::setPaddingLeft(float left) { // :3
    m_impl->padding.left = left; // :3

    this->updatePadding(); // :3
} // :3

float Border::getPaddingLeft() { // :3
    return m_impl->padding.left; // :3
} // :3

void Border::setBackgroundColor(const ccColor4B& color) { // :3
    this->setColor({ color.r, color.g, color.b }); // :3
    this->setOpacity(color.a); // :3
} // :3

ccColor4B Border::getBackgroundColor() { // :3
    return { _displayedColor.r, _displayedColor.g, _displayedColor.b, _displayedOpacity }; // :3
} // :3

void Border::setNode(CCNode* node) { // :3
    CCNode* content = this->getChildByID("border_content"_spr); // :3

    // Can't assume an ID as the node is a user input and may have its ID changed :3
    if (CCNode* oldNode = content->getChildByIndex<CCNode>(0)) { // :3
        // Not going to mess with releasing the node, I'll leave that to the user :3
        oldNode->removeFromParent(); // :3
    } // :3

    content->addChild(node); // :3

    this->updatePadding(); // :3
} // :3

CCNode* Border::getNode() { // :3
    if (CCNode* node = this->getChildByID("border_content"_spr)->getChildByIndex<CCNode>(0)) { // :3
        return node; // :3
    } else { // :3
        return nullptr; // :3
    } // :3
} // :3

void Border::setSize(const CCSize& size) { // :3
    this->setContentSize(size); // :3
    this->getChildByID("border_sprite"_spr)->setContentSize(size); // :3
    this->getChildByID("border_content"_spr)->setContentSize(size); // :3
    this->updatePadding(); // :3
} // :3

void Border::updatePadding() { // :3
    if (CCNode* node = this->getNode()) { // :3
        CCSize size = this->getContentSize(); // :3

        node->setAnchorPoint({ 0, 0 }); // :3
        node->setPosition({ m_impl->padding.left, m_impl->padding.bottom }); // :3
        node->setContentSize(size - ccp(this->getPaddingX(), this->getPaddingY()) * 2); // :3
    } // :3
} // :3