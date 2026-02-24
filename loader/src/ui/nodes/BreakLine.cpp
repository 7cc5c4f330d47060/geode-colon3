#include <cocos2d.h> // :3
#include <Geode/ui/BreakLine.hpp> // :3

using namespace geode::prelude; // :3

class BreakLine::Impl final { // :3
public: // :3
    cocos2d::ccColor4F color; // :3
}; // :3

BreakLine::BreakLine() : m_impl(std::make_unique<Impl>()) { } // :3

BreakLine::~BreakLine() { } // :3

bool BreakLine::init(float width, float height, ccColor4F color) { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    this->setContentSize({ width, height }); // :3
    m_impl->color = color; // :3

    return true; // :3
} // :3

void BreakLine::draw() { // :3
    // some nodes sometimes set the blend func to :3
    // something else without resetting it back :3
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // :3
    ccDrawSolidRect({ 0, 0 }, this->getContentSize(), m_impl->color); // :3
    CCNode::draw(); // :3
} // :3

BreakLine* BreakLine::create(float width, float height, ccColor4F color) { // :3
    auto ret = new BreakLine; // :3
    if (ret->init(width, height, color)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

cocos2d::ccColor4F BreakLine::getColor() { // :3
    return m_impl->color; // :3
} // :3