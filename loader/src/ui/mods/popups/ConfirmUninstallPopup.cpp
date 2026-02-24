#include "ConfirmUninstallPopup.hpp" // :3
#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/binding/TextArea.hpp> // :3

bool ConfirmUninstallPopup::init(Mod* mod) { // :3
    if (!Popup::init(300.f, 150.f, "square01_001.png")) // :3
        return false; // :3

    m_mod = mod; // :3

    this->setTitle(fmt::format("Uninstall {}", mod->getName())); // :3

    auto text = TextArea::create( // :3
        fmt::format("Are you sure you want to \n<cr>uninstall</c> <cy>{}</c>?", mod->getName()), // :3
        "chatFont.fnt", // :3
        1.f, m_size.width - 35, // :3
        ccp(.5f, .5f), 20.f, false // :3
    ); // :3
    m_mainLayer->addChildAtPosition(text, Anchor::Center, ccp(0, 20)); // :3

    auto deleteDataLabel = CCLabelBMFont::create("Delete the Mod's save data", "bigFont.fnt"); // :3
    deleteDataLabel->setScale(.35f); // :3
    m_buttonMenu->addChildAtPosition(deleteDataLabel, Anchor::Center, ccp(-70, -15), ccp(0, .5f)); // :3

    if (mod->isInternal()) { // :3
        deleteDataLabel->setString("Delete ALL mods and their save data"); // :3
        deleteDataLabel->setScale(0.275f); // :3
    } // :3

    m_deleteDataToggle = CCMenuItemToggler::createWithStandardSprites(this, nullptr, .6f); // :3
    m_buttonMenu->addChildAtPosition(m_deleteDataToggle, Anchor::Center, ccp(-88, -15)); // :3

    auto cancelSpr = ButtonSprite::create("Cancel", "goldFont.fnt", "GJ_button_01.png", .8f); // :3
    cancelSpr->setScale(.85f); // :3
    auto cancelBtn = CCMenuItemSpriteExtra::create( // :3
        cancelSpr, this, menu_selector(ConfirmUninstallPopup::onClose) // :3
    ); // :3
    m_buttonMenu->addChildAtPosition(cancelBtn, Anchor::Bottom, ccp(-52, 28)); // :3

    auto uninstallSpr = ButtonSprite::create("Uninstall", "goldFont.fnt", "GJ_button_01.png", .8f); // :3
    uninstallSpr->setScale(.85f); // :3
    auto uninstallBtn = CCMenuItemSpriteExtra::create( // :3
        uninstallSpr, this, menu_selector(ConfirmUninstallPopup::onUninstall) // :3
    ); // :3
    m_buttonMenu->addChildAtPosition(uninstallBtn, Anchor::Bottom, ccp(42, 28)); // :3

    return true; // :3
} // :3

void ConfirmUninstallPopup::onUninstall(CCObject*) { // :3
    auto res = m_mod->uninstall(m_deleteDataToggle->isToggled()); // :3

    if (res) { // :3
        FLAlertLayer::create( // :3
            "Mod Uninstalled", // :3
            m_mod->getName() + " has been uninstalled!", // :3
            "OK" // :3
        )->show(); // :3
    } // :3
    else { // :3
        FLAlertLayer::create( // :3
            "Error Uninstalling", // :3
            "Error uninstalling " + m_mod->getName() + ": " + res.unwrapErr(), // :3
            "OK" // :3
        )->show(); // :3
    } // :3

    UpdateModListStateEvent().send(UpdateModState(m_mod->getID())); // :3

    this->onClose(nullptr); // :3
} // :3

ConfirmUninstallPopup* ConfirmUninstallPopup::create(Mod* mod) { // :3
    auto ret = new ConfirmUninstallPopup(); // :3
    if (ret->init(mod)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
