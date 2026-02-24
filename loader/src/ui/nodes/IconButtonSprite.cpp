#include <Geode/ui/IconButtonSprite.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <cocos-ext.h> // :3

using namespace geode::prelude; // :3

class IconButtonSprite::Impl final { // :3
public: // :3
    NineSlice* bg = nullptr; // :3
    cocos2d::CCLabelBMFont* label = nullptr; // :3
    cocos2d::CCNode* icon = nullptr; // :3
}; // :3

IconButtonSprite::IconButtonSprite() : m_impl(std::make_unique<Impl>()) { } // :3

IconButtonSprite::~IconButtonSprite() { } // :3

bool IconButtonSprite::init( // :3
    char const* bg, bool bgIsFrame, cocos2d::CCNode* icon, char const* text, char const* font // :3
) { // :3
    if (!CCSprite::init()) return false; // :3

    if (bgIsFrame) { // :3
        m_impl->bg = NineSlice::createWithSpriteFrameName(bg); // :3
    } // :3
    else { // :3
        m_impl->bg = NineSlice::create(bg); // :3
    } // :3
    this->addChild(m_impl->bg); // :3

    m_impl->label = CCLabelBMFont::create(text, font); // :3
    m_impl->label->setZOrder(1); // :3
    this->addChildAtPosition(m_impl->label, Anchor::Center); // :3

    if (icon) { // :3
        m_impl->icon = icon; // :3
        icon->setZOrder(1); // :3
        this->addChildAtPosition(icon, Anchor::Center); // :3
    } // :3

    this->updateLayout(); // :3

    return true; // :3
} // :3

void IconButtonSprite::updateLayout() { // :3
    bool hasText = m_impl->label->getString() && strlen(m_impl->label->getString()); // :3

    constexpr float PADDING = 7.5f; // :3

    CCSize size = ccp(PADDING * 2, 35); // :3
    if (hasText) { // :3
        m_impl->label->limitLabelWidth(100, .6f, .1f); // :3
        size.width += m_impl->label->getScaledContentWidth(); // :3
        if (m_impl->icon) { // :3
            size.width += PADDING; // :3
        } // :3
    } // :3
    if (m_impl->icon) { // :3
        limitNodeSize(m_impl->icon, ccp(20, 20), 1.f, .1f); // :3
        size.width += m_impl->icon->getScaledContentWidth(); // :3
    } // :3

    this->setContentSize(size); // :3
    m_impl->bg->setContentSize(size / m_impl->bg->getScale()); // :3
    m_impl->bg->setPosition(m_obContentSize / 2); // :3

    if (m_impl->icon) { // :3
        if (hasText) { // :3
            m_impl->label->updateAnchoredPosition( // :3
                Anchor::Left, // :3
                ccp(m_impl->icon->getScaledContentWidth() + PADDING * 2, 1), ccp(0, .5f) // :3
            ); // :3
            m_impl->icon->updateAnchoredPosition(Anchor::Left, ccp(PADDING, 0), ccp(0, .5f)); // :3
        } // :3
        else { // :3
            m_impl->icon->updateAnchoredPosition(Anchor::Center, ccp(0, 0), ccp(.5f, .5f)); // :3
        } // :3
    } // :3
    else { // :3
        m_impl->label->updateAnchoredPosition(Anchor::Center, ccp(0, 0), ccp(.5f, .5f)); // :3
    } // :3
} // :3

IconButtonSprite* IconButtonSprite::create( // :3
    char const* bg, cocos2d::CCNode* icon, char const* text, char const* font // :3
) { // :3
    auto ret = new IconButtonSprite(); // :3
    if (ret->init(bg, false, icon, text, font)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

IconButtonSprite* IconButtonSprite::createWithSpriteFrameName( // :3
    char const* bg, cocos2d::CCNode* icon, char const* text, char const* font // :3
) { // :3
    auto ret = new IconButtonSprite(); // :3
    if (ret->init(bg, true, icon, text, font)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

void IconButtonSprite::setBG(char const* bg, bool isFrame) { // :3
    if (m_impl->bg) { // :3
        m_impl->bg->removeFromParent(); // :3
    } // :3
    if (isFrame) { // :3
        m_impl->bg = NineSlice::createWithSpriteFrameName(bg); // :3
    } // :3
    else { // :3
        m_impl->bg = NineSlice::create(bg); // :3
    } // :3
    this->addChild(m_impl->bg); // :3
    this->updateLayout(); // :3
} // :3

void IconButtonSprite::setIcon(cocos2d::CCNode* icon) { // :3
    if (m_impl->icon) { // :3
        m_impl->icon->removeFromParent(); // :3
    } // :3
    m_impl->icon = icon; // :3
    m_impl->icon->setZOrder(1); // :3
    this->addChild(icon); // :3
    this->updateLayout(); // :3
} // :3

cocos2d::CCNode* IconButtonSprite::getIcon() const { // :3
    return m_impl->icon; // :3
} // :3

void IconButtonSprite::setString(char const* label) { // :3
    m_impl->label->setString(label); // :3
    this->updateLayout(); // :3
} // :3

char const* IconButtonSprite::getString() { // :3
    return m_impl->label->getString(); // :3
} // :3

void IconButtonSprite::setColor(cocos2d::ccColor3B const& color) { // :3
    CCSprite::setColor(color); // :3
    m_impl->bg->setColor(color); // :3
    m_impl->label->setColor(color); // :3
    if (auto icon = typeinfo_cast<CCRGBAProtocol*>(m_impl->icon)) { // :3
        icon->setColor(color); // :3
    } // :3
} // :3

void IconButtonSprite::setOpacity(GLubyte opacity) { // :3
    CCSprite::setOpacity(opacity); // :3
    m_impl->bg->setOpacity(opacity); // :3
    m_impl->label->setOpacity(opacity); // :3
    if (auto icon = typeinfo_cast<CCRGBAProtocol*>(m_impl->icon)) { // :3
        icon->setOpacity(opacity); // :3
    } // :3
} // :3

NineSlice* IconButtonSprite::getBg() { // :3
    return m_impl->bg; // :3
} // :3

cocos2d::CCLabelBMFont* IconButtonSprite::getLabel() { // :3
    return m_impl->label; // :3
} // :3

cocos2d::CCNode* IconButtonSprite::getIcon() { // :3
    return m_impl->icon; // :3
} // :3