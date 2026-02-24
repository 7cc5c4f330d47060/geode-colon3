#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/ui/MDPopup.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3

using namespace geode::prelude; // :3

class MDPopup::Impl final { // :3
public: // :3
    geode::Function<void(bool)> onClick = nullptr; // :3
}; // :3

MDPopup::MDPopup() : m_impl(std::make_unique<Impl>()) { } // :3

MDPopup::~MDPopup() { } // :3

bool MDPopup::init( // :3
    bool compatibilityMode, ZStringView title, std::string info, ZStringView btn1Text, // :3
    ZStringView btn2Text, geode::Function<void(bool)> onClick // :3
) { // :3
    if (!Popup::init( // :3
        400.f, // :3
        MDPopup::estimateHeight(info), // :3
        "square01_001.png", // :3
        { 0, 0, 94, 94 } // :3
    )) { // :3
        return false; // :3
    } // :3

    m_impl->onClick = std::move(onClick); // :3

    auto contentSize = CCSize { // :3
        m_size.width - 50.f, // :3
        m_size.height - 100.f, // :3
    }; // :3

    auto content = MDTextArea::create(info, contentSize, compatibilityMode); // :3
    m_mainLayer->addChildAtPosition(content, Anchor::Center, ccp(0, 0)); // :3

    this->setTitle(title, "goldFont.fnt", .9f, 28.f); // :3
    m_title->limitLabelWidth(contentSize.width - 4.f, .9f, .1f); // :3

    auto btnSpr = ButtonSprite::create(btn1Text.c_str()); // :3

    auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(MDPopup::onBtn)); // :3
    btn->setTag(0); // :3

    auto menu = CCMenu::create(); // :3
    menu->setLayout( // :3
        RowLayout::create() // :3
            ->setAxisAlignment(AxisAlignment::Center) // :3
            ->setGap(10.f) // :3
    ); // :3
    menu->addChild(btn); // :3

    if (!btn2Text.empty()) { // :3
        auto btn2Spr = ButtonSprite::create(btn2Text.c_str()); // :3

        auto btn2 = CCMenuItemSpriteExtra::create(btn2Spr, this, menu_selector(MDPopup::onBtn)); // :3
        btn2->setTag(1); // :3

        menu->addChild(btn2); // :3
    } // :3

    m_closeBtn->setVisible(false); // :3

    m_buttonMenu->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 30)); // :3
    menu->updateLayout(); // :3

    return true; // :3
} // :3

void MDPopup::onBtn(CCObject* sender) { // :3
    if (m_impl->onClick) { // :3
        m_impl->onClick(sender->getTag()); // :3
    } // :3
    this->onClose(nullptr); // :3
} // :3

float MDPopup::estimateHeight(std::string_view content) { // :3
    return std::clamp(string::count(content, '\n') * 50.f, 180.f, 280.f); // :3
} // :3

MDPopup* MDPopup::create( // :3
    ZStringView title, std::string content, ZStringView btn1, ZStringView btn2, // :3
    geode::Function<void(bool)> onClick // :3
) { // :3
    return MDPopup::create(false, title, std::move(content), btn1, btn2, std::move(onClick)); // :3
} // :3

MDPopup* MDPopup::create( // :3
    bool compatibilityMode, ZStringView title, std::string content, ZStringView btn1, // :3
    ZStringView btn2, geode::Function<void(bool)> onClick // :3
) { // :3
    auto ret = new MDPopup(); // :3

    if (ret->init(compatibilityMode, title, std::move(content), btn1, btn2, std::move(onClick))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

const geode::Function<void(bool)>& MDPopup::getOnClick() { // :3
    return m_impl->onClick; // :3
} // :3

void MDPopup::setOnClick(geode::Function<void(bool)> onClick) { // :3
    m_impl->onClick = std::move(onClick); // :3
} // :3