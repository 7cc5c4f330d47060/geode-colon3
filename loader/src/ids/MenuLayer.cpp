#include <Geode/modify/IDManager.hpp> // :3
#include <Geode/modify/MenuLayer.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/NodeIDs.hpp> // :3
#include <Geode/ui/BasedButtonSprite.hpp> // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3
#include <Geode/binding/GameManager.hpp> // :3
#include <Geode/binding/PlatformToolbox.hpp> // :3

using namespace geode::prelude; // :3
using namespace geode::node_ids; // :3

$register_ids(MenuLayer) { // :3
    // set IDs to everything :3
    size_t spriteOffset = 0; // :3
    size_t labelOffset = 0; // :3

    setIDSafe(this, 0, "main-menu-bg"); // :3
    setIDSafe<CCSprite>(this, spriteOffset++, "main-title"); // :3

    auto winSize = CCDirector::get()->getWinSize(); // :3
    auto GM = GameManager::sharedState(); // :3

    if(!GM->m_clickedGarage) { // :3
        setIDSafe<CCSprite>(this, spriteOffset++, "character-select-hint"); // :3
    } // :3

    if(!GM->m_clickedEditor) { // :3
        setIDSafe<CCSprite>(this, spriteOffset++, "level-editor-hint"); // :3
    } // :3

    // controller :3
    if (PlatformToolbox::isControllerConnected()) { // :3
        setIDSafe<CCSprite>(this, spriteOffset++, "play-gamepad-icon"); // :3
        setIDSafe<CCSprite>(this, spriteOffset++, "editor-gamepad-icon"); // :3
        setIDSafe<CCSprite>(this, spriteOffset++, "icon-kit-gamepad-icon"); // :3

        setIDSafe<CCSprite>(this, spriteOffset++, "settings-gamepad-icon"); // :3

        if(!GM->getGameVariable("0028")) { // :3
            setIDSafe<CCSprite>(this, spriteOffset++, "mouse-gamepad-icon"); // :3
            setIDSafe<CCSprite>(this, spriteOffset++, "click-gamepad-icon"); // :3

            setIDSafe<CCLabelBMFont>(this, labelOffset++, "mouse-gamepad-label"); // :3
            setIDSafe<CCLabelBMFont>(this, labelOffset++, "click-gamepad-label"); // :3
        } // :3
    } // :3

    setIDSafe<CCLabelBMFont>(this, labelOffset++, "player-username"); // :3

    if(auto node = this->getChildByID("settings-gamepad-icon")) { // :3
        // hide it until someone figures out how to bind the positioning to the actual button :3
        node->setVisible(false); // :3
    } // :3

    // main menu :3
    if (auto menu = this->getChildByType<CCMenu>(0)) { // :3
        menu->setID("main-menu"); // :3
        auto playBtn = setIDSafe(menu, 0, "play-button"); // :3
        auto iconBtn = setIDSafe(menu, 1, "icon-kit-button"); // :3

        setIDSafe(menu, 2, "editor-button"); // :3

        if (auto pfp = setIDSafe(menu, 3, "profile-button")) { // :3
            auto profileMenu = detachAndCreateMenu( // :3
                this, "profile-menu", // :3
                SimpleRowLayout::create() // :3
                    ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
                    ->setGap(5.f), // :3
                pfp // :3
            ); // :3
            profileMenu->setContentSize({ 150.f, 50.f }); // :3
            profileMenu->setPositionX( // :3
                profileMenu->getPositionX() + 150.f / 2 - // :3
                    pfp->getScaledContentSize().height / 2 // :3
            ); // :3
            profileMenu->updateLayout(); // :3
        } // :3

        // the buttons are added in order play, icon, editor which doesn't work :3
        // well with setLayout that deals with children in order :3
        menu->swapChildIndices(playBtn, iconBtn); // :3

        menu->setContentSize({ winSize.width - 140.f, 65.f }); // :3
        menu->setLayout( // :3
            SimpleRowLayout::create() // :3
                ->setGap(18.f) // :3
                ->setCrossAxisScaling(AxisScaling::Grow) // :3
        ); // :3
    } // :3

    // bottom menu :3
    if (auto menu = this->getChildByType<CCMenu>(1)) { // :3
        menu->setID("bottom-menu"); // :3
        auto ach = setIDSafe(menu, 0, "achievements-button"); // :3
        setIDSafe(menu, 1, "settings-button"); // :3
        setIDSafe(menu, 2, "stats-button"); // :3
        setIDSafe(menu, 3, "newgrounds-button"); // :3

        // move daily chest to its own menu :3

        if (auto dailyChest = setIDSafe(menu, -1, "daily-chest-button")) { // :3
            auto menu = detachAndCreateMenu( // :3
                this, // :3
                "right-side-menu", // :3
                ColumnLayout::create(), // :3
                dailyChest // :3
            ); // :3
            menu->setContentSize({ 65.f, 180.f }); // :3
            menu->updateLayout(); // :3
        } // :3

        menu->setContentSize({ winSize.width - 220.f, 65.f }); // :3
        menu->setLayout( // :3
            SimpleRowLayout::create() // :3
                ->setGap(5.f) // :3
        ); // :3
    } // :3

    // social media menu :3
    if (auto menu = this->getChildByType<CCMenu>(2)) { // :3
        menu->setID("social-media-menu"); // :3
        setIDSafe(menu, 0, "robtop-logo-button"); // :3
        setIDSafe(menu, 1, "facebook-button"); // :3
        setIDSafe(menu, 2, "twitter-button"); // :3
        setIDSafe(menu, 3, "youtube-button"); // :3
        setIDSafe(menu, 4, "twitch-button"); // :3
        setIDSafe(menu, 5, "discord-button"); // :3
    } // :3

    // more games menu :3
    if (auto menu = this->getChildByType<CCMenu>(3)) { // :3
        menu->setID("more-games-menu"); // :3
        auto moreGamesBtn = setIDSafe(menu, 0, "more-games-button"); // :3

        // move close button to its own menu :3

        if (auto closeBtn = setIDSafe(menu, 1, "close-button")) { // :3
            auto closeMenu = detachAndCreateMenu( // :3
                this, // :3
                "close-menu", // :3
                SimpleRowLayout::create() // :3
                    ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
                    ->setGap(5.f), // :3
                closeBtn // :3
            ); // :3
            closeMenu->setContentSize({ 200.f, 50.f }); // :3
            closeMenu->setPositionX( // :3
                closeMenu->getPositionX() + 200.f / 2 - // :3
                    closeBtn->getScaledContentSize().width / 2 // :3
            ); // :3
            closeMenu->updateLayout(); // :3
        } // :3

        menu->setContentSize({ 100.f, 50.f }); // :3
        menu->setPositionX( // :3
            menu->getPositionX() - 100.f / 2 + // :3
                getSizeSafe(moreGamesBtn).width / 2 // :3
        ); // :3
        menu->setLayout( // :3
            SimpleRowLayout::create() // :3
                ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
                ->setMainAxisDirection(AxisDirection::RightToLeft) // :3
                ->setGap(5.f) // :3
        ); // :3
    } // :3

    // add a menu to the top right corner and middle left that are empty :3
    // but prolly a place mods want to add stuff :3

    auto topRightMenu = CCMenu::create(); // :3
    topRightMenu->setPosition(winSize.width - 210.f / 2, winSize.height - 50.f / 2); // :3
    topRightMenu->setID("top-right-menu"); // :3
    topRightMenu->setContentSize({ 200.f, 50.f }); // :3
    topRightMenu->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setMainAxisDirection(AxisDirection::RightToLeft) // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
            ->setGap(5.f) // :3
    ); // :3
    this->addChild(topRightMenu); // :3

    auto middleLeftMenu = CCMenu::create(); // :3
    middleLeftMenu->setPosition(25.f, 215.f); // :3
    middleLeftMenu->setID("side-menu"); // :3
    middleLeftMenu->setContentSize({ 50.f, 120.f }); // :3
    middleLeftMenu->setLayout(ColumnLayout::create()); // :3
    this->addChild(middleLeftMenu); // :3
} // :3

// MenuLayer::init is hooked in ../hooks/MenuLayer.cpp :3
