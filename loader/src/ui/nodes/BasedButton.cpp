#include <Geode/ui/BasedButton.hpp> // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3

using namespace geode::prelude; // :3

TabButton* TabButton::create( // :3
    TabBaseColor unselected, TabBaseColor selected, char const* text, cocos2d::CCObject* target, // :3
    cocos2d::SEL_MenuHandler callback // :3
) { // :3
    auto ret = new TabButton(); // :3
    auto sprOff = TabButtonSprite::create(text, unselected); // :3
    auto sprOn = TabButtonSprite::create(text, selected); // :3
    if (ret->init(sprOff, sprOn, target, callback)) { // :3
        ret->m_offButton->m_colorDip = .3f; // :3
        ret->m_offButton->m_colorEnabled = true; // :3
        ret->m_offButton->m_scaleMultiplier = 1.f; // :3
        ret->m_onButton->setEnabled(false); // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

TabButton* TabButton::create(char const* text, CCObject* target, SEL_MenuHandler callback) { // :3
    return TabButton::create( // :3
        TabBaseColor::Unselected, TabBaseColor::Selected, text, target, callback // :3
    ); // :3
} // :3
