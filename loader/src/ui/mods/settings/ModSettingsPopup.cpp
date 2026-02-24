#include "ModSettingsPopup.hpp" // :3
#include <Geode/loader/ModSettingsManager.hpp> // :3

bool ModSettingsPopup::init(Mod* mod, bool forceDisableTheme) { // :3
    if (!BaseSettingsPopup::init(forceDisableTheme)) // :3
        return false; // :3
     // :3
    this->setTitle(fmt::format("Settings for {}", mod->getName())); // :3

    m_mod = mod; // :3
    for (auto key : mod->getSettingKeys()) { // :3
        if (auto sett = mod->getSetting(key)) { // :3
            m_settings.push_back(sett->createNode(m_listSize.width)); // :3
        } // :3
        else { // :3
            m_settings.push_back(UnresolvedCustomSettingNode::create(key, mod, m_listSize.width)); // :3
        } // :3
    } // :3

    auto foldersMenu = CCMenu::create(); // :3
    foldersMenu->setContentSize({ 0, 30 }); // :3
    foldersMenu->setAnchorPoint(ccp(1, 0)); // :3
    foldersMenu->setLayout(RowLayout::create() // :3
        ->setAxisReverse(true) // :3
        ->setGap(5) // :3
        ->setAutoGrowAxis(30) // :3
        ->setGrowCrossAxis(true)); // :3
    m_buttonMenu->addChildAtPosition(foldersMenu, Anchor::BottomRight, ccp(-5, 10)); // :3

    auto createFolderButton = [&, this](auto subSprName, auto callback) { // :3
        auto folderSpr = CCSprite::createWithSpriteFrameName("folderIcon_001.png"); // :3
        auto folderSprSub = CCSprite::createWithSpriteFrameName(subSprName); // :3
        folderSprSub->setColor(ccBLACK); // :3
        folderSprSub->setOpacity(155); // :3
        folderSprSub->setScale(.55f); // :3
        folderSpr->addChildAtPosition(folderSprSub, Anchor::Center, ccp(0, -3)); // :3
        auto buttonSpr = createGeodeButton(folderSpr, "", GeodeButtonSprite::Default, m_forceDisableTheme); // :3
        buttonSpr->setScale(.6f); // :3
        buttonSpr->getIcon()->setScale(buttonSpr->getIcon()->getScale() * 1.4f); // :3
        auto folderBtn = CCMenuItemSpriteExtra::create( // :3
            buttonSpr, this, static_cast<SEL_MenuHandler>(callback) // :3
        ); // :3
        foldersMenu->addChild(folderBtn); // :3
        return folderBtn; // :3
    }; // :3

    createFolderButton("save.png"_spr, &ModSettingsPopup::onOpenSaveDirectory); // :3
    createFolderButton("settings.png"_spr, &ModSettingsPopup::onOpenConfigDirectory); // :3
    auto persistentBtn = createFolderButton("persistent.png"_spr, &ModSettingsPopup::onOpenPersistentDirectory); // :3
    if (!std::filesystem::exists(mod->getPersistentDir(false))) { // :3
        persistentBtn->setEnabled(false); // :3
        auto spr = static_cast<CCSprite*>(persistentBtn->getNormalImage()); // :3
        spr->setCascadeColorEnabled(true); // :3
        spr->setCascadeOpacityEnabled(true); // :3
        spr->setColor(ccGRAY); // :3
        spr->setOpacity(155); // :3
    } // :3

    foldersMenu->updateLayout(); // :3

    this->updateState(); // :3

    return true; // :3
} // :3

void ModSettingsPopup::updateState(SettingNode* invoker) { // :3
    BaseSettingsPopup::updateState(invoker); // :3
    m_restartBtn->setVisible(ModSettingsManager::from(m_mod)->restartRequired()); // :3
} // :3

void ModSettingsPopup::onOpenSaveDirectory(CCObject*) { // :3
    file::openFolder(m_mod->getSaveDir()); // :3
} // :3
void ModSettingsPopup::onOpenConfigDirectory(CCObject*) { // :3
    file::openFolder(m_mod->getConfigDir()); // :3
} // :3
void ModSettingsPopup::onOpenPersistentDirectory(CCObject*) { // :3
    file::openFolder(m_mod->getPersistentDir()); // :3
} // :3

ModSettingsPopup* ModSettingsPopup::create(Mod* mod, bool forceDisableTheme) { // :3
    auto ret = new ModSettingsPopup(); // :3
    if (ret->init(mod, forceDisableTheme)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3
