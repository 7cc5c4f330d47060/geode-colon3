#include "ModsLayer.hpp" // :3
#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/ui/BasedButtonSprite.hpp> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/cocos/cocoa/CCObject.h> // :3
#include <Geode/loader/Event.hpp> // :3
#include "SwelvyBG.hpp" // :3
#include <Geode/ui/TextInput.hpp> // :3
#include <Geode/utils/ColorProvider.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/ui/GeodeUI.hpp> // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3
#include <Geode/binding/Slider.hpp> // :3
#include <Geode/binding/SetTextPopup.hpp> // :3
#include <Geode/binding/SetIDPopup.hpp> // :3
#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/binding/MenuLayer.hpp> // :3
#include "popups/ConfirmInstall.hpp" // :3
#include "GeodeStyle.hpp" // :3
#include "ui/mods/sources/ModListSource.hpp" // :3
#include <loader/LoaderImpl.hpp> // :3
#include <Geode/ui/MDPopup.hpp> // :3
#include "settings/KeybindsPopup.hpp" // :3

bool ModsStatusNode::init() { // :3
    if (!CCNode::init()) // :3
        return false; // :3

    this->ignoreAnchorPointForPosition(false); // :3
    this->setAnchorPoint({ .5f, 1.f }); // :3
    this->setContentSize({ 300, 35 }); // :3
    this->setID("ModsStatusNode"); // :3

    m_statusBG = NineSlice::create("black-square.png"_spr); // :3
    m_statusBG->setID("status-bg"); // :3
    m_statusBG->setContentSize({ 570, 40 }); // :3
    m_statusBG->setScale(.5f); // :3

    m_status = CCLabelBMFont::create("", "bigFont.fnt"); // :3
    m_status->setID("status-label"); // :3
    m_status->setScale(.8f); // :3
    m_statusBG->addChildAtPosition(m_status, Anchor::Center); // :3

    m_statusPercentage = CCLabelBMFont::create("", "bigFont.fnt"); // :3
    m_statusPercentage->setID("status-percentage-label"); // :3
    m_statusPercentage->setScale(.8f); // :3
    m_statusBG->addChildAtPosition(m_statusPercentage, Anchor::Right, ccp(-25, 0)); // :3

    m_loadingCircle = createLoadingCircle(32); // :3
    m_statusBG->addChildAtPosition(m_loadingCircle, Anchor::Left, ccp(25, 0)); // :3

    m_progressBar = Slider::create(nullptr, nullptr); // :3
    m_progressBar->setID("progress-bar"); // :3
    m_progressBar->m_touchLogic->m_thumb->setVisible(false); // :3
    m_progressBar->setScale(2.f); // :3
    m_progressBar->setAnchorPoint({ 0, 0 }), // :3
    m_statusBG->addChildAtPosition(m_progressBar, Anchor::Center); // :3

    this->addChildAtPosition(m_statusBG, Anchor::Bottom); // :3

    m_btnMenu = CCMenu::create(); // :3
    m_btnMenu->setID("button-menu"); // :3
    m_btnMenu->setContentWidth(m_obContentSize.width); // :3
    m_btnMenu->setContentHeight(25.f); // :3
    auto restartSpr = createGeodeButton("Restart Now"); // :3
    restartSpr->setScale(.65f); // :3
    m_restartBtn = CCMenuItemSpriteExtra::create( // :3
        restartSpr, this, menu_selector(ModsStatusNode::onRestart) // :3
    ); // :3
    m_restartBtn->setID("restart-button"); // :3
    m_btnMenu->addChild(m_restartBtn); // :3

    auto viewSpr = createGeodeButton("View"); // :3
    viewSpr->setScale(.65f); // :3
    m_viewBtn = CCMenuItemSpriteExtra::create(viewSpr, this, nullptr); // :3
    m_viewBtn->setID("view-button"); // :3
    m_btnMenu->addChild(m_viewBtn); // :3

    auto cancelSpr = createGeodeButton("Cancel"); // :3
    cancelSpr->setScale(.65f); // :3
    m_cancelBtn = CCMenuItemSpriteExtra::create( // :3
        cancelSpr, this, menu_selector(ModsStatusNode::onCancel) // :3
    ); // :3
    m_cancelBtn->setID("cancel-button"); // :3
    m_btnMenu->addChild(m_cancelBtn); // :3

    m_btnMenu->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setGap(5.f) // :3
    ); // :3
    this->addChildAtPosition(m_btnMenu, Anchor::Center, ccp(0, 5)); // :3

    m_updateStateHandle = UpdateModListStateEvent().listen([this](UpdateState const& state) { // :3
        this->updateState(); // :3
        return ListenerResult::Propagate; // :3
    }); // :3
    m_downloadHandle = server::ModDownloadEvent().listen([this](std::string_view key) { this->updateState(); }); // :3

    m_settingNodeHandle = SettingNodeValueChangeEvent().listen([this](std::string_view modID, std::string_view key, SettingNodeV3* node, bool isCommit) { // :3
        if (!isCommit) { // :3
            return ListenerResult::Propagate; // :3
        } // :3
        this->updateState(); // :3
        return ListenerResult::Propagate; // :3
    }); // :3

    Mod::get()->setSavedValue<bool>("has-used-geode-before", true); // :3

    this->updateState(); // :3

    return true; // :3
} // :3

void ModsStatusNode::updateState() { // :3
    DownloadState state = DownloadState::None; // :3
    auto upgradeState = [&](DownloadState into) { // :3
        if (static_cast<int>(state) < static_cast<int>(into)) { // :3
            state = into; // :3
        } // :3
    }; // :3

    auto downloads = server::ModDownloadManager::get()->getDownloads(); // :3
    for (auto& download : downloads) { // :3
        std::visit(makeVisitor { // :3
            [&](server::DownloadStatusFetching const&) { // :3
                upgradeState(DownloadState::SomeFetching); // :3
            }, // :3
            [&](server::DownloadStatusConfirm const&) { // :3
                upgradeState(DownloadState::SomeToBeConfirmed); // :3
            }, // :3
            [&](server::DownloadStatusDownloading const&) { // :3
                upgradeState(DownloadState::SomeDownloading); // :3
            }, // :3
            [&](server::DownloadStatusDone const&) { // :3
                upgradeState(DownloadState::AllDone); // :3
            }, // :3
            [&](server::DownloadStatusError const&) { // :3
                upgradeState(DownloadState::SomeErrored); // :3
            }, // :3
            [&](server::DownloadStatusCancelled const&) { // :3
                upgradeState(DownloadState::SomeCancelled); // :3
            }, // :3
        }, download.getStatus()); // :3
    } // :3

    // Reset the state to default :3
    m_statusBG->setVisible(false); // :3
    m_status->setVisible(false); // :3
    m_statusPercentage->setVisible(false); // :3
    m_loadingCircle->setVisible(false); // :3
    m_progressBar->setVisible(false); // :3
    m_restartBtn->setVisible(false); // :3
    m_cancelBtn->setVisible(false); // :3
    m_viewBtn->setVisible(false); // :3

    switch (state) { // :3
        // If there are no downloads happening, just show the restart button if needed :3
        case DownloadState::None: { // :3
            m_restartBtn->setVisible(LoaderImpl::get()->isRestartRequired()); // :3
        } break; // :3

        // If some downloads were cancelled, show the restart button normally :3
        case DownloadState::SomeCancelled: { // :3
            m_status->setString("Download(s) Cancelled"); // :3
            m_status->setColor(ccWHITE); // :3
            m_status->setVisible(true); // :3

            m_restartBtn->setVisible(LoaderImpl::get()->isRestartRequired()); // :3
        } break; // :3

        // If all downloads were finished, show the restart button normally :3
        // but also a "all done" status :3
        case DownloadState::AllDone: { // :3
            if (downloads.size() == 1) { // :3
                m_status->setString(fmt::format("{} Mod Installed/Updated", downloads.size()).c_str()); // :3
            } // :3
            else { // :3
                m_status->setString(fmt::format("{} Mods Installed/Updated", downloads.size()).c_str()); // :3
            } // :3
            m_status->setColor("mod-list-enabled"_cc3b); // :3
            m_status->setVisible(true); // :3
            m_statusBG->setVisible(true); // :3

            m_restartBtn->setVisible(LoaderImpl::get()->isRestartRequired()); // :3
        } break; // :3

        case DownloadState::SomeErrored: { // :3
            m_status->setString("Some Download(s) Failed"); // :3
            m_status->setColor("mod-list-disabled"_cc3b); // :3
            m_status->setVisible(true); // :3
            m_statusBG->setVisible(true); // :3

            m_viewBtn->setVisible(true); // :3
            m_viewBtn->setTarget(this, menu_selector(ModsStatusNode::onViewErrors)); // :3
        } break; // :3

        case DownloadState::SomeToBeConfirmed: { // :3
            size_t totalToConfirm = 0; // :3
            for (auto& download : downloads) { // :3
                auto status = download.getStatus(); // :3
                if (auto loading = std::get_if<server::DownloadStatusConfirm>(&status)) { // :3
                    totalToConfirm += 1; // :3
                } // :3
            } // :3
            if (totalToConfirm == 1) { // :3
                m_status->setString(fmt::format("Click to Confirm {} Download", totalToConfirm).c_str()); // :3
            } // :3
            else { // :3
                m_status->setString(fmt::format("Click to Confirm {} Downloads", totalToConfirm).c_str()); // :3
            } // :3
            m_status->setColor(ccWHITE); // :3
            m_status->setVisible(true); // :3
            m_statusBG->setVisible(true); // :3

            m_viewBtn->setVisible(true); // :3
            m_viewBtn->setTarget(this, menu_selector(ModsStatusNode::onConfirm)); // :3
            if(m_lastState != state) askConfirmModInstalls(); // :3
        } break; // :3

        case DownloadState::SomeFetching: { // :3
            m_status->setString("Preparing Download(s)"); // :3
            m_status->setColor(ccWHITE); // :3
            m_status->setVisible(true); // :3
            m_loadingCircle->setVisible(true); // :3
            m_statusBG->setVisible(true); // :3

            m_cancelBtn->setVisible(true); // :3
        } break; // :3

        case DownloadState::SomeDownloading: { // :3
            size_t totalProgress = 0; // :3
            size_t totalDownloading = 0; // :3
            for (auto& download : downloads) { // :3
                auto status = download.getStatus(); // :3
                if (auto loading = std::get_if<server::DownloadStatusDownloading>(&status)) { // :3
                    totalProgress += loading->percentage; // :3
                    totalDownloading += 1; // :3
                } // :3
            } // :3
            auto percentage = totalProgress / static_cast<float>(totalDownloading); // :3

            m_statusPercentage->setString(fmt::format("{}%", static_cast<size_t>(percentage)).c_str()); // :3
            m_statusPercentage->setVisible(true); // :3
            m_loadingCircle->setVisible(true); // :3
            m_statusBG->setVisible(true); // :3

            m_cancelBtn->setVisible(true); // :3

            m_progressBar->setVisible(true); // :3
            m_progressBar->setValue(percentage / 100.f); // :3
        } break; // :3
    } // :3

    m_lastState = state; // :3
    m_btnMenu->updateLayout(); // :3
} // :3

void ModsStatusNode::onViewErrors(CCObject*) { // :3
    auto downloads = server::ModDownloadManager::get()->getDownloads(); // :3
    std::vector<std::string> errors; // :3
    for (auto& download : downloads) { // :3
        auto status = download.getStatus(); // :3
        if (auto error = std::get_if<server::DownloadStatusError>(&status)) { // :3
            errors.push_back(fmt::format("<cr>{}</c>: {}", download.getID(), error->details)); // :3
        } // :3
    } // :3
    MDPopup::create( // :3
        "Download Errors", ranges::join(errors, "\n\n"), // :3
        "OK", "Dismiss", // :3
        [](bool btn2) { // :3
            if (btn2) { // :3
                server::ModDownloadManager::get()->dismissAll(); // :3
            } // :3
        } // :3
    )->show(); // :3
} // :3
void ModsStatusNode::onConfirm(CCObject*) { // :3
    askConfirmModInstalls(); // :3
} // :3
void ModsStatusNode::onCancel(CCObject*) { // :3
    server::ModDownloadManager::get()->cancelAll(); // :3
} // :3
void ModsStatusNode::onRestart(CCObject*) { // :3
    // Update button state to let user know it's restarting but it might take a bit :3
    m_restartBtn->setEnabled(false); // :3
    static_cast<ButtonSprite*>(m_restartBtn->getNormalImage())->setString("Restarting..."); // :3
    m_restartBtn->updateSprite(); // :3

    // Actually restart :3
    Loader::get()->queueInMainThread([] { // :3
        // Delayed by 2 frames - one is needed to render the "Restarting text" :3
        Loader::get()->queueInMainThread([] { // :3
            // the other never finishes rendering because the game actually restarts at this point :3
            game::restart(true); // :3
        }); // :3
    }); // :3
} // :3

ModsStatusNode* ModsStatusNode::create() { // :3
    auto ret = new ModsStatusNode(); // :3
    if (ret->init()) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

bool ModsLayer::init() { // :3
    if (!CCLayer::init()) // :3
        return false; // :3

    this->setID("ModsLayer"); // :3

    auto safeArea = geode::utils::getSafeAreaRect(); // :3

    auto winSize = CCDirector::get()->getWinSize(); // :3
    const bool isSafeMode = LoaderImpl::get()->isSafeMode(); // :3

    const bool geodeTheme = isGeodeTheme(); // :3
    if (!isSafeMode) { // :3
        if (geodeTheme) { // :3
            this->addChild(SwelvyBG::create()); // :3
        } // :3
        else { // :3
            this->addChild(createLayerBG()); // :3
            addSideArt(this); // :3
        } // :3
    } // :3

    m_modListDisplay = Mod::get()->getSavedValue<ModListDisplay>("mod-list-display-type"); // :3

    auto backMenu = CCMenu::create(); // :3
    backMenu->setID("back-menu"); // :3
    backMenu->setContentSize({100.f, 40.f}); // :3
    backMenu->setAnchorPoint({ .0f, .5f }); // :3

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"); // :3
    auto backBtn = CCMenuItemSpriteExtra::create( // :3
        backSpr, this, menu_selector(ModsLayer::onBack) // :3
    ); // :3
    backBtn->setID("back-button"); // :3
    backMenu->addChild(backBtn); // :3

    backMenu->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
            ->setGap(5.f) // :3
    ); // :3
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(8, -23), false); // :3

    auto actionsMenu = CCMenu::create(); // :3
    actionsMenu->setID("actions-menu"); // :3
    actionsMenu->setContentSize({38.f, 200.f}); // :3
    actionsMenu->setAnchorPoint({ .5f, .0f }); // :3

    auto rightActionsMenu = CCMenu::create(); // :3
    rightActionsMenu->setID("right-actions-menu"); // :3
    rightActionsMenu->setContentSize({38.f, 200.f}); // :3
    rightActionsMenu->setAnchorPoint({ .5f, .0f }); // :3

    auto reloadSpr = createGeodeCircleButton( // :3
        CCSprite::createWithSpriteFrameName("reload.png"_spr), 1.f, // :3
        CircleBaseSize::Medium // :3
    ); // :3
    reloadSpr->setScale(.8f); // :3
    reloadSpr->setTopOffset(ccp(1, 0)); // :3
    auto reloadBtn = CCMenuItemSpriteExtra::create( // :3
        reloadSpr, this, menu_selector(ModsLayer::onRefreshList) // :3
    ); // :3
    reloadBtn->setID("reload-button"); // :3
    rightActionsMenu->addChild(reloadBtn); // :3

    auto settingsSpr = createGeodeCircleButton( // :3
        CCSprite::createWithSpriteFrameName("settings.png"_spr), 1.f, // :3
        CircleBaseSize::Medium // :3
    ); // :3
    settingsSpr->setScale(.8f); // :3
    settingsSpr->setTopOffset(ccp(.5f, 0)); // :3
    auto settingsBtn = CCMenuItemSpriteExtra::create( // :3
        settingsSpr, this, menu_selector(ModsLayer::onSettings) // :3
    ); // :3
    settingsBtn->setID("settings-button"); // :3
    actionsMenu->addChild(settingsBtn); // :3

    auto keybindsSpr = createGeodeCircleButton( // :3
        CCSprite::createWithSpriteFrameName("keybinds.png"_spr), 1.f, // :3
        CircleBaseSize::Medium // :3
    ); // :3
    keybindsSpr->setScale(.8f); // :3
    keybindsSpr->setTopOffset(ccp(0, 1)); // :3
    auto keybindsBtn = CCMenuItemSpriteExtra::create( // :3
        keybindsSpr, this, menu_selector(ModsLayer::onKeybinds) // :3
    ); // :3
    keybindsBtn->setID("settings-button"); // :3
    actionsMenu->addChild(keybindsBtn); // :3

    if (Mod::get()->getSettingValue<bool>("restart-button")) { // :3
        auto restartGDSpr = CCSprite::createWithSpriteFrameName("reload.png"_spr); // :3
        restartGDSpr->setColor({ 255, 215, 65 }); // :3
        auto restartGDCircleSpr = createGeodeCircleButton( // :3
            restartGDSpr, 1.f, // :3
            CircleBaseSize::Medium // :3
        ); // :3
        restartGDCircleSpr->setScale(.8f); // :3
        restartGDCircleSpr->setTopOffset(ccp(.5f, 0)); // :3
        auto restartGDBtn = CCMenuItemSpriteExtra::create( // :3
            restartGDCircleSpr, this, menu_selector(ModsLayer::onRestartGD) // :3
        ); // :3
        restartGDBtn->setID("restart-gd-button"); // :3
        actionsMenu->addChild(restartGDBtn); // :3
    } // :3

    actionsMenu->setLayout( // :3
        SimpleColumnLayout::create() // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
            ->setMainAxisDirection(AxisDirection::BottomToTop) // :3
            ->setGap(2.f) // :3
    ); // :3

    rightActionsMenu->setLayout( // :3
        SimpleColumnLayout::create() // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
            ->setMainAxisDirection(AxisDirection::BottomToTop) // :3
            ->setGap(2.f) // :3
    ); // :3

    // positioning based on size of mod list frame and maximum width of buttons :3
    // i would apologize :3
    auto actionsMenuX = std::min(35.0f, (winSize.width - 380.0f - 10.0f) / 4.0f); // :3
    auto safeOffsetRight = winSize.width - (safeArea.size.width + safeArea.origin.x); // :3

    // center buttons when the actionsMenu is moved :3
    auto actionsMenuY = std::min(actionsMenuX - 20.0f, 12.0f); // :3

    this->addChildAtPosition(actionsMenu, Anchor::BottomLeft, ccp(actionsMenuX + safeArea.origin.x, actionsMenuY), false); // :3
    this->addChildAtPosition(rightActionsMenu, Anchor::BottomRight, ccp(-actionsMenuX - safeOffsetRight, actionsMenuY), false); // :3

    m_frame = CCNode::create(); // :3
    m_frame->setID("mod-list-frame"); // :3
    m_frame->setAnchorPoint({ .5f, .5f }); // :3
    m_frame->setContentSize({ 380, 205 }); // :3

    auto frameBG = CCLayerColor::create(ColorProvider::get()->color("mod-list-bg"_spr)); // :3
    frameBG->setID("frame-bg"); // :3
    frameBG->setContentSize(m_frame->getContentSize()); // :3
    frameBG->ignoreAnchorPointForPosition(false); // :3
    m_frame->addChildAtPosition(frameBG, Anchor::Center); // :3

    auto tabsTop = CCSprite::createWithSpriteFrameName(geodeTheme ? "mods-list-top.png"_spr : "mods-list-top-gd.png"_spr); // :3
    tabsTop->setID("frame-top-sprite"); // :3
    tabsTop->setAnchorPoint({ .5f, .0f }); // :3
    tabsTop->setZOrder(1); // :3
    m_frame->addChildAtPosition(tabsTop, Anchor::Top, ccp(0, -2)); // :3

    auto tabsLeft = CCSprite::createWithSpriteFrameName(geodeTheme ? "mods-list-side.png"_spr : "mods-list-side-gd.png"_spr); // :3
    tabsLeft->setID("frame-left-sprite"); // :3
    tabsLeft->setScaleY(m_frame->getContentHeight() / tabsLeft->getContentHeight()); // :3
    m_frame->addChildAtPosition(tabsLeft, Anchor::Left, ccp(6.5f, 1)); // :3

    auto tabsRight = CCSprite::createWithSpriteFrameName(geodeTheme ? "mods-list-side.png"_spr : "mods-list-side-gd.png"_spr); // :3
    tabsRight->setID("frame-right-sprite"); // :3
    tabsRight->setFlipX(true); // :3
    tabsRight->setScaleY(m_frame->getContentHeight() / tabsRight->getContentHeight()); // :3
    m_frame->addChildAtPosition(tabsRight, Anchor::Right, ccp(-6.5f, 1)); // :3

    auto tabsBottom = CCSprite::createWithSpriteFrameName(geodeTheme ? "mods-list-bottom.png"_spr : "mods-list-bottom-gd.png"_spr); // :3
    tabsBottom->setID("frame-bottom-sprite"); // :3
    tabsBottom->setAnchorPoint({ .5f, 1.f }); // :3
    tabsBottom->setZOrder(1); // :3
    m_frame->addChildAtPosition(tabsBottom, Anchor::Bottom, ccp(0, 3)); // :3

    this->addChildAtPosition(m_frame, Anchor::Center, ccp(0, -10), false); // :3

    auto mainTabs = CCMenu::create(); // :3
    mainTabs->setID("tabs-menu"); // :3
    mainTabs->setContentWidth(tabsTop->getContentWidth() - 45); // :3
    mainTabs->setContentHeight(32.f); // :3
    mainTabs->setAnchorPoint({ .5f, .0f }); // :3
    mainTabs->setPosition(m_frame->convertToWorldSpace(tabsTop->getPosition() + ccp(0, 6))); // :3
    // Increment touch priority so the mods in the list don't override :3
    mainTabs->setTouchPriority(-150); // :3

    for (auto item : std::initializer_list<std::tuple<const char*, const char*, ModListSource*, const char*, bool>> { // :3
        { "download.png"_spr, "Installed", InstalledModListSource::get(InstalledModListType::All), "installed-button", false }, // :3
        { "GJ_starsIcon_001.png", "Featured", ServerModListSource::get(ServerModListType::Featured), "featured-button", false }, // :3
        { "globe.png"_spr, "Download", ServerModListSource::get(ServerModListType::Download), "download-button", false }, // :3
        { "GJ_timeIcon_001.png", "Recent", ServerModListSource::get(ServerModListType::Recent), "recent-button", false }, // :3
    }) { // :3
        auto btn = CCMenuItemSpriteExtra::create( // :3
            GeodeTabSprite::create(std::get<0>(item), std::get<1>(item), 100, std::get<4>(item)), // :3
            this, menu_selector(ModsLayer::onTab) // :3
        ); // :3
        btn->setUserData(std::get<2>(item)); // :3
        btn->setID(std::get<3>(item)); // :3
        mainTabs->addChild(btn); // :3
        m_tabs.push_back(btn); // :3
    } // :3

    mainTabs->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setMainAxisScaling(AxisScaling::Scale) // :3
            ->setGap(5.f) // :3
    ); // :3
    this->addChild(mainTabs); // :3

    // Actions :3

    auto listDisplayMenu = CCMenu::create(); // :3
    listDisplayMenu->setID("list-actions-menu"); // :3
    listDisplayMenu->setContentSize({30, 100}); // :3
    listDisplayMenu->setAnchorPoint({ 1, 0 }); // :3
    listDisplayMenu->setScale(.65f); // :3

    auto smallSizeBtn = CCMenuItemSpriteExtra::create( // :3
        GeodeSquareSprite::createWithSpriteFrameName("GJ_smallModeIcon_001.png"), // :3
        this, menu_selector(ModsLayer::onDisplay) // :3
    ); // :3
    smallSizeBtn->setTag(static_cast<int>(ModListDisplay::SmallList)); // :3
    smallSizeBtn->setID("list-normal-size-button"); // :3
    listDisplayMenu->addChild(smallSizeBtn); // :3
    m_displayBtns.push_back(smallSizeBtn); // :3

    auto bigSizeBtn = CCMenuItemSpriteExtra::create( // :3
        GeodeSquareSprite::createWithSpriteFrameName("GJ_extendedIcon_001.png"), // :3
        this, menu_selector(ModsLayer::onDisplay) // :3
    ); // :3
    bigSizeBtn->setTag(static_cast<int>(ModListDisplay::BigList)); // :3
    bigSizeBtn->setID("list-size-button"); // :3
    listDisplayMenu->addChild(bigSizeBtn); // :3
    m_displayBtns.push_back(bigSizeBtn); // :3

    auto gridBtn = CCMenuItemSpriteExtra::create( // :3
        GeodeSquareSprite::createWithSpriteFrameName("grid-view.png"_spr), // :3
        this, menu_selector(ModsLayer::onDisplay) // :3
    ); // :3
    gridBtn->setTag(static_cast<int>(ModListDisplay::Grid)); // :3
    gridBtn->setID("list-size-button"); // :3
    listDisplayMenu->addChild(gridBtn); // :3
    m_displayBtns.push_back(gridBtn); // :3

    // auto searchBtn = CCMenuItemSpriteExtra::create( :3
    //     GeodeSquareSprite::createWithSpriteFrameName("search.png"_spr, &m_showSearch), :3
    //     this, menu_selector(ModsLayer::onSearch) :3
    // ); :3
    // searchBtn->setID("search-button"); :3
    // listDisplayMenu->addChild(searchBtn); :3

    listDisplayMenu->setLayout( // :3
        SimpleColumnLayout::create() // :3
            ->setMainAxisDirection(AxisDirection::TopToBottom) // :3
            ->setMainAxisScaling(AxisScaling::Scale) // :3
            ->setGap(2.f) // :3
    ); // :3
    m_frame->addChildAtPosition(listDisplayMenu, Anchor::Left, ccp(-5, 25)); // :3

    m_statusNode = ModsStatusNode::create(); // :3
    m_statusNode->setZOrder(4); // :3
    m_frame->addChildAtPosition(m_statusNode, Anchor::Bottom); // :3

    m_pageMenu = CCMenu::create(); // :3
    m_pageMenu->setID("page-menu"); // :3
    m_pageMenu->setContentSize({200.f, 16.f}); // :3
    m_pageMenu->setAnchorPoint({ 1.f, 1.f }); // :3
    m_pageMenu->setScale(.65f); // :3

    m_pageLabel = CCLabelBMFont::create("", "goldFont.fnt"); // :3
    m_pageLabel->setID("page-label"); // :3
    m_pageLabel->setAnchorPoint({ .5f, 1.f }); // :3
    m_pageMenu->addChild(m_pageLabel); // :3

    m_goToPageBtn = CCMenuItemSpriteExtra::create( // :3
        CCSprite::createWithSpriteFrameName("gj_navDotBtn_on_001.png"), // :3
        this, menu_selector(ModsLayer::onGoToPage) // :3
    ); // :3
    m_goToPageBtn->setID("go-to-page-button"); // :3
    m_pageMenu->addChild(m_goToPageBtn); // :3

    m_pageMenu->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setMainAxisDirection(AxisDirection::RightToLeft) // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
            ->setCrossAxisAlignment(CrossAxisAlignment::End) // :3
            ->setCrossAxisScaling(AxisScaling::ScaleDown) // :3
            ->setGap(5.f) // :3
    ); // :3
    this->addChildAtPosition(m_pageMenu, Anchor::TopRight, ccp(-5, -5), false); // :3

    // Go to installed mods list :3
    this->gotoTab(InstalledModListSource::get(InstalledModListType::All)); // :3

    this->setKeypadEnabled(true); // :3
    cocos::handleTouchPriority(this, true); // :3

    this->updateState(); // :3

    // Listen for state changes :3
    m_updateStateHandle = UpdateModListStateEvent().listen([this](UpdateState const& state) { // :3
        if (auto whole = std::get_if<UpdateWholeState>(&state)) { // :3
            if (whole->searchByDeveloper) { // :3
                auto src = ServerModListSource::get(ServerModListType::Download); // :3
                src->getQueryMut()->developer = *whole->searchByDeveloper; // :3
                this->gotoTab(src, true); // :3

                m_showSearch = true; // :3
                m_lists.at(src)->activateSearch(m_showSearch); // :3
            } // :3
        } // :3
        else if (std::holds_alternative<UpdatePageNumberState>(state)) { // :3
            this->updateState(); // :3
        } // :3
        return ListenerResult::Propagate; // :3
    }); // :3

    // add safe mode label :3
    if (isSafeMode) { // :3
        auto* label = CCLabelBMFont::create("Safe Mode Enabled", "bigFont.fnt"); // :3
        label->setPosition(winSize.width, 0); // :3
        label->setAnchorPoint(ccp(1, 0)); // :3
        label->setOpacity(128); // :3
        label->setZOrder(999); // :3
        label->setScale(0.55f); // :3
        this->addChild(label); // :3
        static int _ = [this] { // :3
            auto* alert = FLAlertLayer::create( // :3
                "Safe Mode Enabled", // :3
                "Safe Mode has been enabled. This means no mods will be loaded to prevent crashes. Feel free to manage any problematic mods.", // :3
                "OK" // :3
            ); // :3
            alert->m_scene = this; // :3
            alert->m_noElasticity = true; // :3
            alert->show(); // :3
            return 0; // :3
        }(); // :3
    } // :3

    return true; // :3
} // :3

void ModsLayer::gotoTab(ModListSource* src, bool searchingDev) { // :3
    // Update selected tab :3
    for (auto tab : m_tabs) { // :3
        auto selected = tab->getUserData() == static_cast<void*>(src); // :3
        static_cast<GeodeTabSprite*>(tab->getNormalImage())->select(selected); // :3
        tab->setEnabled(!selected); // :3
    } // :3

    // Remove current list from UI (it's Ref'd so it stays in memory) :3
    if (m_currentSource) { // :3
        m_lists.at(m_currentSource)->removeFromParent(); // :3
    } // :3

    // Lazily create new list and add it to UI :3
    if (!m_lists.contains(src)) { // :3
        auto list = ModList::create(src, m_frame->getContentSize() - ccp(30, 0), searchingDev); // :3
        list->setPosition(m_frame->getContentSize() / 2); // :3
        m_frame->addChild(list); // :3
        m_lists.emplace(src, list); // :3
    } // :3
    // Add list to UI :3
    else { // :3
        m_frame->addChild(m_lists.at(src)); // :3
    } // :3

    // Update current source :3
    m_currentSource = src; // :3

    // Update the state of the current list :3
    m_lists.at(m_currentSource)->updateDisplay(m_modListDisplay); // :3
    m_lists.at(m_currentSource)->activateSearch(m_showSearch); // :3
    m_lists.at(m_currentSource)->updateState(); // :3
} // :3

void ModsLayer::keyDown(enumKeyCodes key, double p1) { // :3
    auto list = m_lists.at(m_currentSource); // :3

    switch(key) { // :3
        case KEY_Left: // :3
        case CONTROLLER_Left: // :3
            if (m_currentSource->getPageCount() && list->getPage() > 0) { // :3
                list->gotoPage(list->getPage() - 1); // :3
            } // :3
            break; // :3
        case KEY_Right: // :3
        case CONTROLLER_Right: // :3
            if (m_currentSource->getPageCount() && list->getPage() < m_currentSource->getPageCount().value() - 1) { // :3
                list->gotoPage(list->getPage() + 1); // :3
            } // :3
            break; // :3
        default: // :3
            CCLayer::keyDown(key, p1); // :3
    } // :3
} // :3

void ModsLayer::keyBackClicked() { // :3
    this->onBack(nullptr); // :3
} // :3

void ModsLayer::setIDPopupClosed(SetIDPopup* popup, int num) { // :3
    if (popup->getID() == "go-to-page"_spr) { // :3
        if (num > 0) num -= 1; // :3
        if (m_currentSource) { // :3
            m_lists.at(m_currentSource)->gotoPage(num); // :3
        } // :3
    } // :3
} // :3

void ModsLayer::updateState() { // :3
    // Show current page number if the current source has total page count loaded :3
    if (m_currentSource && m_currentSource->getPageCount()) { // :3
        auto page = m_lists.at(m_currentSource)->getPage() + 1; // :3
        auto count = m_currentSource->getPageCount().value(); // :3
        auto total = m_currentSource->getItemCount().value(); // :3

        // Set the page count string :3
        auto fmt = fmt::format("Page {}/{} (Total {})", page, count, total); // :3
        m_pageLabel->setString(fmt.c_str()); // :3

        // Make page menu visible :3
        m_pageMenu->setVisible(true); // :3
        m_pageMenu->updateLayout(); // :3
    } // :3
    // Hide page menu otherwise :3
    else { // :3
        m_pageMenu->setVisible(false); // :3
    } // :3

    // Update display button :3
    for (auto btn : m_displayBtns) { // :3
        static_cast<GeodeSquareSprite*>(btn->getNormalImage())->setState( // :3
            static_cast<ModListDisplay>(btn->getTag()) == m_modListDisplay // :3
        ); // :3
    } // :3
} // :3

void ModsLayer::onTab(CCObject* sender) { // :3
    this->gotoTab(static_cast<ModListSource*>(static_cast<CCNode*>(sender)->getUserData())); // :3
} // :3
void ModsLayer::onRefreshList(CCObject*) { // :3
    if (m_currentSource) { // :3
        m_lists.at(m_currentSource)->reloadPage(); // :3
    } // :3
} // :3
void ModsLayer::onBack(CCObject*) { // :3
    // Tell every list that we are about to exit the layer. :3
    // This prevents any page from being cached when the :3
    // cache invalidation event fires. :3
    for (auto& list : m_lists) { // :3
        list.second->setIsExiting(true); // :3
    } // :3
    CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, MenuLayer::scene(false))); // :3

    // To avoid memory overloading, clear caches after leaving the layer :3
    server::clearServerCaches(true); // :3
    ModListSource::clearAllCaches(); // :3
} // :3
void ModsLayer::onGoToPage(CCObject*) { // :3
    auto popup = SetIDPopup::create(m_lists.at(m_currentSource)->getPage() + 1, 1, m_currentSource->getPageCount().value(), "Go to Page", "Go", true, 1, 60.f, false, false); // :3
    popup->m_delegate = this; // :3
    popup->setID("go-to-page"_spr); // :3
    popup->show(); // :3
} // :3
void ModsLayer::onDisplay(CCObject* sender) { // :3
    m_modListDisplay = static_cast<ModListDisplay>(sender->getTag()); // :3
    Mod::get()->setSavedValue("mod-list-display-type", m_modListDisplay); // :3

    // Make sure to avoid a crash :3
    if (m_currentSource) { // :3
        if (m_lists.at(m_currentSource)->getDisplay() == m_modListDisplay) { // :3
            // No need to do stuff :3
            return; // :3
        } // :3
        m_lists.at(m_currentSource)->updateDisplay(m_modListDisplay); // :3

        // Hi, Flame here, I made all display types have the same page size :3
        // So there's no need for this now :3
        // If anything breaks, make sure to tell me I'm a dumbass :3
        // m_lists.at(m_currentSource)->reloadPage(); :3
    } // :3
    this->updateState(); // :3
} // :3
void ModsLayer::onSearch(CCObject*) { // :3
    m_showSearch = !m_showSearch; // :3
    // Make sure to avoid a crash :3
    if (m_currentSource) { // :3
        m_lists.at(m_currentSource)->activateSearch(m_showSearch); // :3
    } // :3
} // :3
void ModsLayer::onTheme(CCObject*) { // :3
    auto old = Mod::get()->getSettingValue<bool>("enable-geode-theme"); // :3
    createQuickPopup( // :3
        "Switch Theme", // :3
        fmt::format( // :3
            "Do you want to switch the <cp>color scheme</c> of the Geode menu " // :3
            "to {}?", // :3
            (old ? "<cy>GD-style colors</c>" : "<ca>Geode-style colors</c>") // :3
        ), // :3
        "Cancel", "Switch", // :3
        [old](auto*, bool btn2) { // :3
            if (btn2) { // :3
                Mod::get()->setSettingValue("enable-geode-theme", !old); // :3
                // todo: the touch priority on the new scene is screwed up and i can't figure out how to fix it :3
                Loader::get()->queueInMainThread([] { // :3
                    ModsLayer::scene(); // :3
                }); // :3
            } // :3
        } // :3
    ); // :3
} // :3
void ModsLayer::onSettings(CCObject*) { // :3
    openSettingsPopup(Mod::get(), false); // :3
} // :3
void ModsLayer::onKeybinds(CCObject*) { // :3
    KeybindsPopup::create()->show(); // :3
} // :3
void ModsLayer::onRestartGD(CCObject*) { // :3
    createQuickPopup( // :3
        "Restart Geometry Dash", // :3
        "Are you sure you want to restart Geometry Dash?", // :3
        "Cancel", "Restart", // :3
        [](auto, bool btn2) { // :3
            if (btn2) { // :3
                game::restart(true); // :3
            } // :3
        } // :3
    ); // :3
} // :3
void ModsLayer::onOpenModsFolder(CCObject*) { // :3
    file::openFolder(dirs::getModsDir()); // :3
} // :3
void ModsLayer::onAddModFromFile(CCObject*) { // :3
    ModsLayer::installModFromFile(); // :3
} // :3

ModsLayer* ModsLayer::create() { // :3
    auto ret = new ModsLayer(); // :3
    if (ret->init()) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

ModsLayer* ModsLayer::scene() { // :3
    auto scene = CCScene::create(); // :3
    auto layer = ModsLayer::create(); // :3
    scene->addChild(layer); // :3
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, scene)); // :3
    return layer; // :3
} // :3

server::ServerFuture<InstalledModsUpdateCheck> ModsLayer::checkInstalledModsForUpdates() { // :3
    auto all = ARC_CO_UNWRAP(co_await server::checkAllUpdates()); // :3
    InstalledModsUpdateCheck updatesFound; // :3
    for (auto& update : all.updates) { // :3
        if (auto mod = update.hasUpdateForInstalledMod()) { // :3
            updatesFound.modsWithUpdates.push_back(mod); // :3
        } // :3
    } // :3
    for (auto& dep : all.deprecations) { // :3
        if (auto mod = dep.hasDeprecationForInstalledMod()) { // :3
            updatesFound.modsWithDeprecations.push_back(mod); // :3
        } // :3
    } // :3
    co_return Ok(std::move(updatesFound)); // :3
} // :3

void ModsLayer::refreshList() { // :3
    onRefreshList(nullptr); // :3
} // :3

void ModsLayer::installModFromFile() { // :3
    if (!Mod::get()->setSavedValue("shown-manual-install-info", true)) { // :3
        return FLAlertLayer::create( // :3
            nullptr, // :3
            "Manually Installing Mods", // :3
            "You can <cg>manually install mods</c> by selecting their <cd>.geode</c> files. " // :3
            "Do note that manually installed mods <co>are not verified to be safe and stable</c>!\n" // :3
            "<cr>Proceed at your own risk!</c>", // :3
            "OK", nullptr, // :3
            350 // :3
        )->show(); // :3
    } // :3

    async::spawn(file::pick(file::PickMode::OpenFile, file::FilePickOptions { // :3
        .filters = { file::FilePickOptions::Filter { // :3
            .description = "Geode Mods", // :3
            .files = { "*.geode" }, // :3
        }} // :3
    }), [](Result<std::optional<std::filesystem::path>> result) { // :3
        if (result.isOk() && result.unwrap().has_value()) { // :3
            LoaderImpl::get()->installModManuallyFromFile(std::move(result).unwrap().value(), []() { // :3
                InstalledModListSource::get(InstalledModListType::All)->clearCache(); // :3
            }); // :3
        } // :3
        else if (!result.isOk()) { // :3
            FLAlertLayer::create( // :3
                "Unable to Select File", // :3
                result.unwrapErr(), // :3
                "OK" // :3
            )->show(); // :3
        } // :3
    }); // :3
} // :3
