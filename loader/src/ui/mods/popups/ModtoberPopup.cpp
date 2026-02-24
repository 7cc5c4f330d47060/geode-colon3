#include "ModtoberPopup.hpp" // :3
#include <Geode/utils/web.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/binding/ButtonSprite.hpp> // :3

bool ModtoberPopup::init() { // :3
    if (!GeodePopup::init(410.f, 270.f)) // :3
        return false; // :3

    m_bgSprite->setVisible(false); // :3

    auto bg = CCSprite::createWithSpriteFrameName("modtober25-popup.png"_spr); // :3
    m_mainLayer->addChildAtPosition(bg, Anchor::Center); // :3

    auto supportSpr = createGeodeButton("Join"); // :3
    supportSpr->setScale(.8f); // :3
    auto supportBtn = CCMenuItemSpriteExtra::create( // :3
        supportSpr, this, menu_selector(ModtoberPopup::onDiscord) // :3
    ); // :3
    m_buttonMenu->addChildAtPosition(supportBtn, Anchor::BottomRight, ccp(-65, 50)); // :3

    return true; // :3
} // :3

void ModtoberPopup::onDiscord(CCObject*) { // :3
    createQuickPopup( // :3
        "Join Modtober", // :3
        "<co>Modtober</c> is being hosted on the <cr>Geode</c> <ca>Discord Server</c>.\n" // :3
        "To participate, join Geode and read the rules for the contest in <cy>#dev-news</c>", // :3
        "Cancel", "Join Discord", // :3
        [](auto, bool btn2) { // :3
            if (btn2) { // :3
                web::openLinkInBrowser("https://discord.gg/9e43WMKzhp");
            } // :3
        }, // :3
        true, // :3
        false // :3
    ); // :3
} // :3

ModtoberPopup* ModtoberPopup::create() { // :3
    auto ret = new ModtoberPopup(); // :3
    if (ret->init()) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3