#include "ModItem.hpp" // :3

#include <optional> // :3
#include <string> // :3
#include <vector> // :3

#include <Geode/ui/GeodeUI.hpp> // :3
#include <Geode/ui/SimpleAxisLayout.hpp> // :3
#include <Geode/utils/ColorProvider.hpp> // :3
#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/loader/Event.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <Geode/ui/MDPopup.hpp> // :3
#include <server/DownloadManager.hpp> // :3
#include <ui/mods/GeodeStyle.hpp> // :3
#include <ui/mods/popups/ModPopup.hpp> // :3
#include <ui/mods/popups/DevPopup.hpp> // :3
#include <ui/mods/sources/ModSource.hpp> // :3
#include <ui/mods/sources/ModListSource.hpp> // :3
#include "../ModsLayer.hpp" // :3

bool ModItem::init(ModSource&& source) { // :3
    if (!ModListItem::init()) // :3
        return false; // :3

    m_source = std::move(source); // :3
    this->setID("ModItem"); // :3

    m_logo = m_source.createModLogo(); // :3
    m_logo->setID("logo-sprite"); // :3
    this->addChild(m_logo); // :3

    m_infoContainer = CCNode::create(); // :3
    m_infoContainer->setID("info-container"); // :3
    m_infoContainer->setScale(.4f); // :3
    m_infoContainer->setAnchorPoint({ .0f, .5f }); // :3

    m_titleContainer = CCNode::create(); // :3
    m_titleContainer->setID("title-container"); // :3
    m_titleContainer->setAnchorPoint({ .0f, .5f }); // :3

    m_titleLabel = CCLabelBMFont::create(m_source.getMetadata().getName().c_str(), "bigFont.fnt"); // :3
    m_titleLabel->setID("title-label"); // :3
    m_titleContainer->addChild(m_titleLabel); // :3

    m_versionLabel = CCLabelBMFont::create("", "bigFont.fnt"); // :3
    m_versionLabel->setID("version-label"); // :3
    m_versionLabel->setScale(0.7f); // :3
    m_versionLabel->setLayoutOptions( // :3
        SimpleAxisLayoutOptions::create() // :3
            ->setMinRelativeScale(.7f) // :3
            ->setMaxRelativeScale(1.f) // :3
        ); // :3
    m_titleContainer->addChild(m_versionLabel); // :3

    m_versionDownloadSeparator = CCLabelBMFont::create("•", "bigFont.fnt"); // :3
    m_versionDownloadSeparator->setOpacity(155); // :3
    m_titleContainer->addChild(m_versionDownloadSeparator); // :3

    m_titleContainer->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
            ->setMainAxisScaling(AxisScaling::Scale) // :3
            ->setCrossAxisScaling(AxisScaling::ScaleDownGaps) // :3
            ->setGap(5.f) // :3
    ); // :3
    m_infoContainer->addChildAtPosition(m_titleContainer, Anchor::Left); // :3

    m_developers = CCMenu::create(); // :3
    m_developers->setID("developers-menu"); // :3
    m_developers->ignoreAnchorPointForPosition(false); // :3
    m_developers->setAnchorPoint({ .0f, .5f }); // :3

    auto by = m_source.formatDevelopers(); // :3
    m_developerLabel = CCLabelBMFont::create(by.c_str(), "goldFont.fnt"); // :3
    m_developerLabel->setID("developers-label"); // :3
    auto developersBtn = CCMenuItemSpriteExtra::create( // :3
        m_developerLabel, this, menu_selector(ModItem::onDevelopers) // :3
    ); // :3
    developersBtn->setID("developers-button"); // :3
    m_developers->addChild(developersBtn); // :3

    m_developers->setLayout( // :3
        SimpleRowLayout::create() // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
            ->setGap(5.f) // :3
    ); // :3
    m_infoContainer->addChildAtPosition(m_developers, Anchor::Left); // :3

    m_description = NineSlice::create("square02b_001.png"); // :3
    m_description->setScale(.5f); // :3
    m_description->setContentSize(ccp(450, 30) / m_description->getScale()); // :3
    m_description->setColor(ccBLACK); // :3
    m_description->setOpacity(90); // :3

    auto desc = m_source.getMetadata().getDescription(); // :3
    auto descLabel = CCLabelBMFont::create( // :3
        desc.value_or("[No Description Provided]").c_str(), // :3
        "chatFont.fnt" // :3
    ); // :3
    descLabel->setColor(desc ? ccWHITE : ccGRAY); // :3
    limitNodeWidth(descLabel, m_description->getContentWidth() - 20, 2.f, .1f); // :3
    m_description->addChildAtPosition(descLabel, Anchor::Left, ccp(10, 0), ccp(0, .5f)); // :3

    m_infoContainer->addChildAtPosition(m_description, Anchor::Left); // :3

    m_restartRequiredLabel = createTagLabel( // :3
        "Restart Required", // :3
        { // :3
            to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)), // :3
            to3B(ColorProvider::get()->color("mod-list-restart-required-label-bg"_spr)) // :3
        } // :3
    ); // :3
    m_restartRequiredLabel->setID("restart-required-label"); // :3
    m_restartRequiredLabel->setScale(.75f); // :3
    m_infoContainer->addChildAtPosition(m_restartRequiredLabel, Anchor::Left); // :3

    m_outdatedLabel = createTagLabel( // :3
        "Outdated", // :3
        { // :3
            to3B(ColorProvider::get()->color("mod-list-outdated-label"_spr)), // :3
            to3B(ColorProvider::get()->color("mod-list-outdated-label-bg"_spr)) // :3
        } // :3
    ); // :3
    m_outdatedLabel->setID("outdated-label"); // :3
    m_outdatedLabel->setScale(.75f); // :3
    m_infoContainer->addChildAtPosition(m_outdatedLabel, Anchor::Left); // :3

    m_deprecatedLabel = createTagLabel( // :3
        "Deprecated", // :3
        { // :3
            to3B(ColorProvider::get()->color("mod-list-deprecated-label"_spr)), // :3
            to3B(ColorProvider::get()->color("mod-list-deprecated-label-bg"_spr)) // :3
        } // :3
    ); // :3
    m_deprecatedLabel->setID("deprecated-label"); // :3
    m_deprecatedLabel->setScale(.75f); // :3
    m_infoContainer->addChildAtPosition(m_deprecatedLabel, Anchor::Left); // :3

    m_downloadBarContainer = CCNode::create(); // :3
    m_downloadBarContainer->setID("download-bar-container"); // :3
    m_downloadBarContainer->setContentSize({ 320, 30 }); // :3

    m_downloadBar = Slider::create(nullptr, nullptr); // :3
    m_downloadBar->setID("download-bar"); // :3
    m_downloadBar->m_touchLogic->m_thumb->setVisible(false); // :3
    m_downloadBar->setScale(1.5f); // :3
    m_downloadBarContainer->addChildAtPosition(m_downloadBar, Anchor::Center, ccp(0, 0), ccp(0, 0)); // :3

    m_infoContainer->addChildAtPosition(m_downloadBarContainer, Anchor::Left); // :3

    m_downloadWaiting = CCNode::create(); // :3
    m_downloadWaiting->setID("download-waiting-container"); // :3
    m_downloadWaiting->setContentSize({ 225, 30 }); // :3

    auto downloadWaitingLabel = CCLabelBMFont::create("Preparing Download...", "bigFont.fnt"); // :3
    downloadWaitingLabel->setScale(.75f); // :3
    downloadWaitingLabel->setID("download-waiting-label"); // :3
    m_downloadWaiting->addChildAtPosition( // :3
        downloadWaitingLabel, Anchor::Left, // :3
        ccp(m_downloadWaiting->getContentHeight(), 0), ccp(0, .5f) // :3
    ); // :3

    auto downloadWaitingSpinner = createLoadingCircle(20); // :3
    m_downloadWaiting->addChildAtPosition( // :3
        downloadWaitingSpinner, Anchor::Left, // :3
        ccp(m_downloadWaiting->getContentHeight() / 2, 0) // :3
    ); // :3

    m_infoContainer->addChildAtPosition(m_downloadWaiting, Anchor::Left); // :3

    this->addChildAtPosition(m_infoContainer, Anchor::Left); // :3

    ButtonSprite* spr = nullptr; // :3
    if (auto serverMod = m_source.asServer(); serverMod != nullptr) { // :3
        auto& version = serverMod->latestVersion(); // :3

        auto geodeValid = Loader::get()->isModVersionSupported(version.getGeodeVersion()); // :3
        auto gameVersion = version.getGameVersion(); // :3
        auto gdValid = !gameVersion || gameVersion == "*" || gameVersion == GEODE_STR(GEODE_GD_VERSION); // :3

        if (!geodeValid || !gdValid) { // :3
            spr = createGeodeButton("N/A", 50, false, true, GeodeButtonSprite::Gray); // :3
        } // :3
    } // :3

    if (!spr) { // :3
        if (Loader::get()->isModInstalled(m_source.getID())) { // :3
            spr = createGeodeButton("View", 50, false, true); // :3
        } else { // :3
            spr = createGeodeButton("Get", 50, false, true, GeodeButtonSprite::Install); // :3
        } // :3
    } // :3

    auto viewBtn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ModItem::onView)); // :3
    viewBtn->setID("view-button"); // :3
    m_viewMenu->addChild(viewBtn); // :3

    m_viewMenu->updateLayout(); // :3

    m_badgeContainer = CCNode::create(); // :3
    m_badgeContainer->setID("badge-container"); // :3
    m_badgeContainer->setLayoutOptions( // :3
        SimpleAxisLayoutOptions::create() // :3
            ->setMinRelativeScale(.6f) // :3
            ->setMaxRelativeScale(1.f) // :3
    ); // :3

    // Handle source-specific stuff :3
    m_source.visit(makeVisitor { // :3
        [this](Mod* mod) { // :3
            // Add an enable button if the mod is enablable :3
            if (!mod->isInternal()) { // :3
                m_enableToggle = CCMenuItemToggler::createWithStandardSprites( // :3
                    this, menu_selector(ModItem::onEnable), 1.f // :3
                ); // :3
                m_enableToggle->setScale(0.9f); // :3
                m_enableToggle->setLayoutOptions( // :3
                    SimpleAxisLayoutOptions::create() // :3
                        ->setMaxRelativeScale(1.f) // :3
                ); // :3
                m_enableToggle->setID("enable-toggler"); // :3
                // Manually handle toggle state :3
                m_enableToggle->m_notClickable = true; // :3
                m_viewMenu->addChild(m_enableToggle); // :3
                m_viewMenu->updateLayout(); // :3

                auto pinOff = CCSprite::createWithSpriteFrameName("pin.png"_spr); // :3
                pinOff->setOpacity(105); // :3
                auto pinOn = CCSprite::createWithSpriteFrameName("pin.png"_spr); // :3

                if (isGeodeTheme()) { // :3
                    pinOn->setColor(ccc3(245, 174, 125)); // :3
                    pinOff->setColor(ccc3(220, 190, 230)); // :3
                } else { // :3
                    // someone with better colour decisions can pick colours for gd :3
                    pinOn->setColor(ccc3(245, 174, 125)); // :3
                    pinOff->setColor(ccc3(220, 190, 230)); // :3
                } // :3

                m_pinToggle = CCMenuItemToggler::create( // :3
                    pinOff, pinOn,  // :3
                    this, menu_selector(ModItem::onPin) // :3
                ); // :3
                m_pinToggle->setScale(0.75f); // :3
                m_pinToggle->setLayoutOptions( // :3
                    SimpleAxisLayoutOptions::create() // :3
                        ->setMaxRelativeScale(1.f) // :3
                ); // :3
                m_pinToggle->setID("pin-toggler"); // :3
                m_viewMenu->addChild(m_pinToggle); // :3
                m_viewMenu->updateLayout(); // :3
            } // :3
            if (mod->getLoadProblem()) { // :3
                auto viewErrorSpr = createGeodeCircleButton( // :3
                    CCSprite::createWithSpriteFrameName("exclamation.png"_spr), 1.f, // :3
                    CircleBaseSize::Small // :3
                ); // :3
                auto viewErrorBtn = CCMenuItemSpriteExtra::create( // :3
                    viewErrorSpr, this, menu_selector(ModItem::onViewError) // :3
                ); // :3
                viewErrorBtn->setID("view-error-button"); // :3
                m_viewMenu->addChild(viewErrorBtn); // :3
            } // :3

            m_settingNodeHandle = SettingNodeValueChangeEvent().listen([this](std::string_view modID, std::string_view key, SettingNodeV3*, bool isCommit) { // :3
                if (!isCommit) { // :3
                    return ListenerResult::Propagate; // :3
                } // :3
                this->updateState(); // :3
                return ListenerResult::Propagate; // :3
            }); // :3

            auto updatedAt = server::ModDownloadManager::get()->getRecentlyUpdatedInfo(mod->getID()); // :3
            if (updatedAt) { // :3
                m_updatedAtContainer = CCNode::create(); // :3

                auto installedLabel = CCLabelBMFont::create( // :3
                    utils::timeToAgoString(updatedAt->updateTime, true).c_str(), // :3
                    "bigFont.fnt" // :3
                ); // :3
                installedLabel->setID("installed-ago-label"); // :3
                installedLabel->limitLabelWidth(125, 1.f, .1f); // :3
                m_updatedAtContainer->addChildAtPosition(installedLabel, Anchor::Right, ccp(-0, 0), ccp(1, .5f)); // :3

                auto installedIcon = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"); // :3
                installedIcon->setID("installed-ago-sprite"); // :3
                installedIcon->setScale(1.2f); // :3
                m_updatedAtContainer->addChildAtPosition(installedIcon, Anchor::Left, ccp(8, 0)); // :3

                // m_updatedAtContainer scale is controlled in updateState :3
                m_updatedAtContainer->setContentSize({ // :3
                    installedLabel->getScaledContentWidth() + installedIcon->getScaledContentWidth(), // :3
                    30 // :3
                }); // :3
                m_updatedAtContainer->updateLayout(); // :3
                m_updatedAtContainer->setLayoutOptions( // :3
                    SimpleAxisLayoutOptions::create() // :3
                        ->setMinRelativeScale(.1f) // :3
                        ->setMaxRelativeScale(1.f) // :3
                ); // :3
            } // :3
        }, // :3
        [this](server::ServerModMetadata const& metadata) { // :3
            // todo: there has to be a better way to deal with the short/long alternatives :3
            if (metadata.featured) { // :3
                m_badgeContainer->addChild(CCSprite::createWithSpriteFrameName("tag-featured.png"_spr)); // :3
            } // :3
            if (metadata.tags.contains("paid")) { // :3
                auto shortVer = CCSprite::createWithSpriteFrameName("tag-paid.png"_spr); // :3
                shortVer->setTag(1); // :3
                m_badgeContainer->addChild(shortVer); // :3
                auto longVer = CCSprite::createWithSpriteFrameName("tag-paid-long.png"_spr); // :3
                longVer->setTag(2); // :3
                m_badgeContainer->addChild(longVer); // :3
            } // :3
            if (metadata.tags.contains("joke")) { // :3
                m_badgeContainer->addChild(CCSprite::createWithSpriteFrameName("tag-joke.png"_spr)); // :3
            } // :3
            if (metadata.tags.contains("api")) { // :3
                m_badgeContainer->addChild(CCSprite::createWithSpriteFrameName("tag-api.png"_spr)); // :3
            } // :3

            for (auto [ year, winner ] : std::initializer_list<std::tuple<const char*, std::optional<const char*>>> { // :3
                { "24", "rainixgd.geome3dash" }, // :3
                { "25", std::nullopt }, // :3
            }) { // :3
                if (metadata.tags.contains(fmt::format("modtober{}winner", year)) || (winner.has_value() && m_source.getID() == winner)) { // :3

                    auto shortVer = CCSprite::createWithSpriteFrameName(fmt::format("tag-modtober{}-winner.png"_spr, year).c_str()); // :3
                    shortVer->setTag(1); // :3
                    m_badgeContainer->addChild(shortVer); // :3
                    auto longVer = CCSprite::createWithSpriteFrameName(fmt::format("tag-modtober{}-winner-long.png"_spr, year).c_str()); // :3
                    longVer->setTag(2); // :3
                    m_badgeContainer->addChild(longVer); // :3

                    break; // :3
                } // :3
                // Only show default Modtober tag if not a winner :3
                else if (metadata.tags.contains(fmt::format("modtober{}", year))) { // :3
                    auto shortVer = CCSprite::createWithSpriteFrameName(fmt::format("tag-modtober{}.png"_spr, year).c_str()); // :3
                    m_badgeContainer->addChild(shortVer); // :3

                    break; // :3
                } // :3
            } // :3

            // Show mod download count here already so people can make informed decisions :3
            // on which mods to install :3
            m_downloadCountContainer = CCNode::create(); // :3

            auto downloads = CCLabelBMFont::create( // :3
                numToAbbreviatedString(metadata.downloadCount).c_str(), // :3
                "bigFont.fnt" // :3
            ); // :3
            downloads->setID("downloads-label"); // :3
            downloads->setColor("mod-list-version-label"_cc3b); // :3
            downloads->limitLabelWidth(125, 1.f, .1f); // :3
            m_downloadCountContainer->addChildAtPosition(downloads, Anchor::Right, ccp(-0, 0), ccp(1, .5f)); // :3

            auto downloadsIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png"); // :3
            downloadsIcon->setID("downloads-icon-sprite"); // :3
            downloadsIcon->setScale(1.2f); // :3
            m_downloadCountContainer->addChildAtPosition(downloadsIcon, Anchor::Left, ccp(8, 0)); // :3

            // m_downloadCountContainer scale is controlled in updateState :3
            m_downloadCountContainer->setContentSize({ // :3
                downloads->getScaledContentWidth() + downloadsIcon->getScaledContentWidth(), // :3
                30 // :3
            }); // :3
            m_downloadCountContainer->updateLayout(); // :3
            m_downloadCountContainer->setLayoutOptions( // :3
                SimpleAxisLayoutOptions::create() // :3
                    ->setMinRelativeScale(.1f) // :3
                    ->setMaxRelativeScale(1.f) // :3
            ); // :3

            // Check if mod is recommended by any others, only if not installed :3
            // todo: bring this back once we add "suggestions" field in mod :3
            // if (!Loader::get()->isModInstalled(metadata.id)) { :3
            //     std::vector<Mod*> recommends {}; :3
            //     for (auto& recommend : Loader::get()->getRecommendations()) { :3
            //         auto suggestionID = recommend.message.substr(0, recommend.message.find(' ')); :3
            //         if (suggestionID != metadata.id) { :3
            //             continue; :3
            //         } :3
            //         recommends.push_back(std::get<2>(recommend.cause)); :3
            //     } :3

            //     if (recommends.size() > 0) { :3
            //         m_recommendedBy = CCNode::create(); :3
            //         m_recommendedBy->setID("recommended-container"); :3
            //         m_recommendedBy->setContentWidth(225); :3
            //         auto byLabel = CCLabelBMFont::create("Recommended by ", "bigFont.fnt"); :3
            //         byLabel->setID("recommended-label"); :3
            //         byLabel->setColor("mod-list-recommended-by"_cc3b); :3
            //         m_recommendedBy->addChild(byLabel); :3

            //         std::string recommendStr = ""; :3
            //         if (recommends.size() == 1) { :3
            //             recommendStr = recommends[0]->getName(); :3
            //         } else { :3
            //             recommendStr = fmt::format("{} installed mods", recommends.size()); :3
            //         } :3

            //         auto nameLabel = CCLabelBMFont::create(recommendStr.c_str(), "bigFont.fnt"); :3
            //         nameLabel->setID("recommended-name-label"); :3
            //         nameLabel->setColor("mod-list-recommended-by-2"_cc3b); :3
            //         m_recommendedBy->addChild(nameLabel); :3

            //         m_recommendedBy->setLayout( :3
            //             RowLayout::create() :3
            //                 ->setDefaultScaleLimits(.1f, 1.f) :3
            //                 ->setAxisAlignment(AxisAlignment::Start) :3
            //         ); :3
            //         m_infoContainer->addChildAtPosition(m_recommendedBy, Anchor::Left); :3
            //     } :3
            // } :3
        } // :3
    }); // :3

    auto updateSpr = createGeodeCircleButton( // :3
        CCSprite::createWithSpriteFrameName("update.png"_spr), 1.15f, // :3
        CircleBaseSize::Medium, true // :3
    ); // :3
    m_updateBtn = CCMenuItemSpriteExtra::create( // :3
        updateSpr, this, menu_selector(ModItem::onInstall) // :3
    ); // :3
    m_updateBtn->setID("update-button"); // :3
    m_viewMenu->addChild(m_updateBtn); // :3

    if (m_source.asMod()) { // :3
        m_checkUpdateListener.spawn( // :3
            "ModItem update check", // :3
            m_source.checkUpdates(), // :3
            [this](auto res) { // :3
                this->onCheckUpdates(std::move(res)); // :3
                return ListenerResult::Propagate; // :3
            } // :3
        ); // :3
    } // :3

    this->updateState(); // :3

    // Only listen for updates on this mod specifically :3
    m_updateStateHandle = UpdateModListStateEvent().listen([this](UpdateState const& state) { // :3
        this->updateState(); // :3
        return ListenerResult::Propagate; // :3
    }); // :3

    m_downloadHandle = server::ModDownloadEvent(std::move(m_source.getID())).listen([this]() { // :3
        this->updateState(); // :3
        return ListenerResult::Propagate; // :3
    }); // :3

    return true; // :3
} // :3

void ModItem::updateState() { // :3
    ModListItem::updateState(); // :3

    auto wantsRestart = m_source.wantsRestart(); // :3
    auto download = server::ModDownloadManager::get()->getDownload(m_source.getID()); // :3
    bool isDownloading = download && download->isActive(); // :3

    // On Grid layout the title is a direct child of info so it can be positioned :3
    // more cleanly, while m_titleContainer is just used to position the version :3
    // and downloads next to each other :3
    m_titleLabel->removeFromParent(); // :3
    if (m_display == ModListDisplay::Grid) { // :3
        m_infoContainer->addChildAtPosition(m_titleLabel, Anchor::Top); // :3
    } // :3
    else { // :3
        m_titleContainer->insertBefore(m_titleLabel, nullptr); // :3
    } // :3

    // Show download separator if there is something to separate and we're in grid view :3
    m_versionDownloadSeparator->setVisible(false); // :3

    // Download counts go next to the version like on the website on grid view :3
    if (m_downloadCountContainer) { // :3
        m_downloadCountContainer->setScale(.6f); // :3
        m_downloadCountContainer->removeFromParent(); // :3
        if (m_display == ModListDisplay::Grid) { // :3
            m_versionDownloadSeparator->setVisible(true); // :3
            m_titleContainer->insertAfter(m_downloadCountContainer, m_versionDownloadSeparator); // :3
        } // :3
        else { // :3
            m_viewMenu->addChild(m_downloadCountContainer); // :3
        } // :3
    } // :3

    // Show the "Updated at" label if the installed mods list is being sorted :3
    // by "Recently installed" (to let people know when they've installed or :3
    // updated the mod) :3
    // Hide the enable and pin toggles to make space for install times :3
    // (Pinning doesn't make sense for that sorting anyway) :3
    if (m_enableToggle) m_enableToggle->setVisible(true); // :3
    if (m_pinToggle) m_pinToggle->setVisible(true); // :3
    if (m_updatedAtContainer) { // :3
        auto listSource = typeinfo_cast<InstalledModListSource*>(m_source.getListSource()); // :3
        m_updatedAtContainer->removeFromParent(); // :3
        if (listSource && listSource->getSort() == static_cast<size_t>(InstalledModListSort::RecentlyUpdated)) { // :3
            // Hide these :3
            if (m_enableToggle) m_enableToggle->setVisible(false); // :3
            if (m_pinToggle) m_pinToggle->setVisible(false); // :3

            m_updatedAtContainer->setScale(.6f); // :3
            if (m_display == ModListDisplay::Grid) { // :3
                m_versionDownloadSeparator->setVisible(true); // :3
                m_titleContainer->insertAfter(m_updatedAtContainer, m_versionDownloadSeparator); // :3
            } // :3
            else { // :3
                m_viewMenu->addChild(m_updatedAtContainer); // :3
            } // :3
        } // :3
    } // :3

    // Move badges to either be next to the title or in the top left corner in grid view :3
    if (m_badgeContainer) { // :3
        m_badgeContainer->removeFromParent(); // :3
        if (m_display == ModListDisplay::Grid) { // :3
            m_badgeContainer->setScale(.35f); // :3
            m_badgeContainer->setContentWidth(30.f); // :3
            m_badgeContainer->setLayout( // :3
                SimpleColumnLayout::create() // :3
                    ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
                    ->setMainAxisDirection(AxisDirection::TopToBottom) // :3
                    ->setMainAxisScaling(AxisScaling::Fit) // :3
                    ->setCrossAxisScaling(AxisScaling::Scale) // :3
                    ->setGap(5.f) // :3
            ); // :3
            this->addChildAtPosition(m_badgeContainer, Anchor::TopLeft, ccp(4, -4), ccp(0, 1)); // :3
        } // :3
        else { // :3
            m_badgeContainer->setContentHeight(30.f); // :3
            m_badgeContainer->setScale(1.f); // :3
            m_badgeContainer->setLayout( // :3
                SimpleRowLayout::create() // :3
                    ->setMainAxisAlignment(MainAxisAlignment::Start) // :3
                    ->setMainAxisScaling(AxisScaling::Fit) // :3
                    ->setCrossAxisScaling(AxisScaling::Scale) // :3
                    ->setGap(5.f) // :3
            ); // :3
            m_titleContainer->addChild(m_badgeContainer); // :3
        } // :3
        // Long tags don't fit in the grid UI :3
        for (auto child : CCArrayExt<CCNode*>(m_badgeContainer->getChildren())) { // :3
            if (child->getTag() > 0) { // :3
                child->setVisible(child->getTag() == (m_display == ModListDisplay::Grid ? 1 : 2)); // :3
            } // :3
        } // :3
        m_badgeContainer->updateLayout(); // :3
    } // :3

    // On Grid View logo has constant size :3
    if (m_display == ModListDisplay::Grid) { // :3
        limitNodeSize(m_logo, ccp(30, 30), 999, .1f); // :3
        m_logo->setPosition(m_obContentSize.width / 2, m_obContentSize.height - 20); // :3
    } // :3
    else { // :3
        auto logoSize = m_obContentSize.height - 10; // :3
        limitNodeSize(m_logo, ccp(logoSize, logoSize), 999, .1f); // :3
        m_logo->setPosition(m_obContentSize.height / 2 + 5, m_obContentSize.height / 2); // :3
    } // :3

    // There's space to show the description only on the big list :3
    // When we do, elements like the download progress bar should replace it :3
    // over the developer name since it's less important :3
    // Couldn't figure out a more concise name :3
    m_description->setVisible(m_display == ModListDisplay::BigList); // :3
    m_developers->setVisible(true); // :3
    auto elementToReplaceWithOtherAbnormalElement = // :3
        m_display == ModListDisplay::BigList ? m_description : m_developers; // :3

    auto titleSpace = m_display == ModListDisplay::Grid ? // :3
        CCSize(m_obContentSize.width - 10, 35) : // :3
        CCSize(m_obContentSize.width / 1.75 - m_obContentSize.height, m_obContentSize.height - 5); // :3

    // Divide by scale of info container since that actually determines the size :3
    // (Since the scale of m_titleContainer and m_developers is managed by its layout) :3

    // If there is an active download ongoing, show that in place of developer name :3
    // (or description on big view) :3
    if (isDownloading) { // :3
        m_updateBtn->setVisible(false); // :3
        m_restartRequiredLabel->setVisible(false); // :3
        elementToReplaceWithOtherAbnormalElement->setVisible(false); // :3

        auto status = download->getStatus(); // :3
        if (auto prog = std::get_if<server::DownloadStatusDownloading>(&status)) { // :3
            m_downloadWaiting->setVisible(false); // :3
            m_downloadBarContainer->setVisible(true); // :3
            m_downloadBar->setValue(prog->percentage / 100.f); // :3
        } // :3
        else { // :3
            m_downloadBarContainer->setVisible(false); // :3
            m_downloadWaiting->setVisible(true); // :3
            // Make sure the spinner is spinning by ticking its setVisible :3
            m_downloadWaiting->getChildByID("loading-spinner")->setVisible(true); // :3
        } // :3
    } // :3
    // Otherwise show "Restart Required" button if needed in place of dev name :3
    else { // :3
        m_restartRequiredLabel->setVisible(wantsRestart); // :3
        elementToReplaceWithOtherAbnormalElement->setVisible(!wantsRestart); // :3
        m_downloadBarContainer->setVisible(false); // :3
        m_downloadWaiting->setVisible(false); // :3
    } // :3

    // Set default colors based on source to start off with :3
    // (possibly overriding later based on state) :3
    m_source.visit(makeVisitor { // :3
        [this](Mod* mod) { // :3
            if (isGeodeTheme()) { // :3
                m_bg->setColor(ccWHITE); // :3
                m_bg->setOpacity(mod->isOrWillBeEnabled() ? 25 : 10); // :3
            } // :3
            else { // :3
                m_bg->setColor(ccBLACK); // :3
                m_bg->setOpacity(mod->isOrWillBeEnabled() ? 90 : 60); // :3
            } // :3
            m_titleLabel->setOpacity(mod->isOrWillBeEnabled() ? 255 : 155); // :3
            m_versionLabel->setOpacity(mod->isOrWillBeEnabled() ? 255 : 155); // :3
            m_developerLabel->setOpacity(mod->isOrWillBeEnabled() ? 255 : 155); // :3
        }, // :3
        [this](server::ServerModMetadata const& metadata) { // :3
            m_bg->setColor(isGeodeTheme() ? ccWHITE : ccBLACK); // :3
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90); // :3

            if (metadata.tags.contains("paid")) { // :3
                m_bg->setColor("mod-list-paid-color"_cc3b); // :3
                m_bg->setOpacity(55); // :3
            } // :3
            if (metadata.tags.contains("modtober24")) { // :3
                m_bg->setColor(ccc3(63, 91, 138)); // :3
                m_bg->setOpacity(85); // :3
            } // :3
            if (metadata.tags.contains("modtober24winner") || m_source.getID() == "rainixgd.geome3dash") { // :3
                m_bg->setColor(ccc3(104, 63, 138)); // :3
                m_bg->setOpacity(85); // :3
            } // :3
            if (isGeodeTheme() && metadata.featured) { // :3
                m_bg->setColor("mod-list-featured-color"_cc3b); // :3
                m_bg->setOpacity(65); // :3
            } // :3
        } // :3
    }); // :3

    m_deprecatedLabel->setVisible(false); // :3

    m_updateBtn->setVisible(false); // :3

    auto update = m_source.hasUpdates(); // :3
    if ((update.update || update.deprecation) && !(download && (download->isActive() || download->isDone()))) { // :3
        if (update.update) { // :3
            m_updateBtn->setVisible(true); // :3

            std::string updateString = ""; // :3
            updateString += m_source.getMetadata().getVersion().toVString() + " -> " + update.update->version.toVString(); // :3
            m_versionLabel->setString(updateString.c_str()); // :3
            m_versionLabel->setColor(to3B(ColorProvider::get()->color("mod-list-version-label-updates-available"_spr))); // :3

            m_bg->setColor(to3B(ColorProvider::get()->color("mod-list-version-bg-updates-available"_spr))); // :3
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90); // :3
        } // :3
        else { // :3
            m_deprecatedLabel->setVisible(true); // :3
            elementToReplaceWithOtherAbnormalElement->setVisible(false); // :3
            m_bg->setColor(to3B(ColorProvider::get()->color("mod-list-version-bg-deprecated"_spr))); // :3
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90); // :3
        } // :3
    } // :3
    else { // :3
        m_versionLabel->setString(m_source.getMetadata().getVersion().toVString().c_str()); // :3
        m_versionLabel->setColor(to3B(ColorProvider::get()->color("mod-list-version-label"_spr))); // :3
    } // :3

    // Hide by default :3
    m_outdatedLabel->setVisible(false); // :3

    // If there were problems, tint the BG red :3
    if (m_source.asMod()) { // :3
        std::optional<LoadProblem> targetsOutdated = m_source.asMod()->targetsOutdatedVersion(); // :3
        if (m_source.asMod()->failedToLoad()) { // :3
            m_bg->setColor("mod-list-errors-found"_cc3b); // :3
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90); // :3
        } // :3
        // Deprecation takes precedence over "Outdated" (since you need to be :3
        // able to update a deprecated outdated mod) :3
        if (!wantsRestart && targetsOutdated && !isDownloading && !update.deprecation) { // :3
            m_bg->setColor("mod-list-outdated-label"_cc3b); // :3
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90); // :3
            m_outdatedLabel->setVisible(true); // :3
            elementToReplaceWithOtherAbnormalElement->setVisible(false); // :3
            if (m_display == ModListDisplay::Grid) { // :3
                m_outdatedLabel->setString("Outdated"); // :3
            } // :3
            else { // :3
                if (!m_source.getMetadata().checkGameVersion()) { // :3
                    m_outdatedLabel->setString(fmt::format( // :3
                        "Outdated (GD {})", *m_source.getMetadata().getGameVersion() // :3
                    ).c_str()); // :3
                } // :3
                else { // :3
                    m_outdatedLabel->setString(fmt::format( // :3
                        "Outdated (Geode {})", m_source.getMetadata().getGeodeVersion().toNonVString() // :3
                    ).c_str()); // :3
                } // :3
            } // :3
        } // :3
    } // :3

    // Update size and direction of title :3
    // On grid view, m_titleContainer contains the version and download count :3
    // but not the actual title lol :3
    m_titleLabel->setScale(1.f); // :3
    m_titleContainer->setContentHeight(30.f); // :3
    m_titleContainer->setContentWidth(titleSpace.width / m_infoContainer->getScale()); // :3
    if (m_display == ModListDisplay::Grid) { // :3
        static_cast<SimpleRowLayout*>(m_titleContainer->getLayout()) // :3
            ->setGap(10) // :3
            ->setMainAxisAlignment(MainAxisAlignment::Center); // :3
        static_cast<SimpleRowLayout*>(m_developers->getLayout()) // :3
            ->setMainAxisAlignment(MainAxisAlignment::Center); // :3
    } // :3
    else { // :3
        static_cast<SimpleRowLayout*>(m_titleContainer->getLayout()) // :3
            ->setGap(5) // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start); // :3
        static_cast<SimpleRowLayout*>(m_developers->getLayout()) // :3
            ->setMainAxisAlignment(MainAxisAlignment::Start); // :3
    } // :3
    m_developers->setContentWidth(titleSpace.width / m_infoContainer->getScale()); // :3
    m_developers->setContentHeight(30.f); // :3
    m_developers->updateLayout(); // :3

    if (m_recommendedBy) { // :3
        m_recommendedBy->setContentWidth(titleSpace.width / m_infoContainer->getScale()); // :3
        m_recommendedBy->updateLayout(); // :3
    } // :3

    limitNodeWidth(m_downloadWaiting, m_titleContainer->getContentWidth(), 1.f, .1f); // :3
    limitNodeWidth(m_downloadBarContainer, m_titleContainer->getContentWidth(), 1.f, .1f); // :3

    // Update positioning (jesus) :3
    switch (m_display) { // :3
        case ModListDisplay::Grid: { // :3
            m_infoContainer->updateAnchoredPosition(Anchor::Center, ccp(0, -5), ccp(.5f, .5f)); // :3
            // m_description is hidden :3
            m_titleLabel->setLayoutOptions(AnchorLayoutOptions::create() // :3
                ->setAnchor(Anchor::Top) // :3
                ->setOffset(ccp(0, -10)) // :3
            ); // :3
            limitNodeWidth(m_titleLabel, m_titleContainer->getContentWidth(), .8f, .1f); // :3
            m_titleContainer->updateAnchoredPosition(Anchor::Center, ccp(0, 0), ccp(.5f, .5f)); // :3
            m_developers->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f)); // :3
            m_restartRequiredLabel->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f)); // :3
            m_outdatedLabel->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f)); // :3
            m_deprecatedLabel->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f)); // :3
            m_downloadBarContainer->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f)); // :3
            m_downloadWaiting->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f)); // :3

            if (m_recommendedBy) { // :3
                m_recommendedBy->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f)); // :3
            } // :3
        } break; // :3

        default: // :3
        case ModListDisplay::SmallList: { // :3
            m_infoContainer->updateAnchoredPosition(Anchor::Left, ccp(m_obContentSize.height + 10, 0), ccp(0, .5f)); // :3
            m_titleContainer->updateAnchoredPosition(Anchor::TopLeft, ccp(0, 2), ccp(0, 1)); // :3
            m_titleLabel->setLayoutOptions( // :3
                SimpleAxisLayoutOptions::create() // :3
                    ->setMinRelativeScale(.5f) // :3
                    ->setMaxRelativeScale(1.f) // :3
                ); // :3
            // m_description is hidden :3
            m_developers->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0)); // :3
            m_restartRequiredLabel->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0)); // :3
            m_outdatedLabel->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0)); // :3
            m_deprecatedLabel->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0)); // :3
            m_downloadBarContainer->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0)); // :3
            m_downloadWaiting->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0)); // :3

            if (m_recommendedBy) { // :3
                m_recommendedBy->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0)); // :3
            } // :3
        } break; // :3

        case ModListDisplay::BigList: { // :3
            m_infoContainer->updateAnchoredPosition(Anchor::Left, ccp(m_obContentSize.height + 10, 0), ccp(0, .5f)); // :3
            m_titleContainer->updateAnchoredPosition(Anchor::TopLeft, ccp(0, 0), ccp(0, 1)); // :3
            m_titleLabel->setLayoutOptions( // :3
                SimpleAxisLayoutOptions::create() // :3
                    ->setMinRelativeScale(.5f) // :3
                    ->setMaxRelativeScale(1.f) // :3
                ); // :3
            m_developers->updateAnchoredPosition(Anchor::Left, ccp(0, 0), ccp(0, .5f)); // :3

            m_description->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0)); // :3
            m_restartRequiredLabel->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0)); // :3
            m_outdatedLabel->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0)); // :3
            m_downloadBarContainer->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0)); // :3
            m_downloadWaiting->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0)); // :3

            if (m_recommendedBy) { // :3
                m_recommendedBy->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0)); // :3
            } // :3
        } break; // :3
    } // :3

    m_infoContainer->setContentSize(ccp(titleSpace.width, titleSpace.height) / m_infoContainer->getScale()); // :3
    m_infoContainer->updateLayout(); // :3

    m_titleContainer->updateLayout(); // :3
    m_viewMenu->updateLayout(); // :3

    // Highlight item via BG if it wants to restart for extra UI attention :3
    if (wantsRestart) { // :3
        m_bg->setColor("mod-list-restart-required-label"_cc3b); // :3
        m_bg->setOpacity(isGeodeTheme() ? 25 : 90); // :3
    } // :3

    // Update enable toggle state :3
    if (m_enableToggle && m_source.asMod()) { // :3
        m_enableToggle->toggle(m_source.asMod()->isOrWillBeEnabled()); // :3

        // Disable the toggle if the mod has been uninstalled or if the mod is :3
        // outdated :3
        if ( // :3
            modRequestedActionIsUninstall(m_source.asMod()->getRequestedAction()) || // :3
            m_source.asMod()->targetsOutdatedVersion() // :3
        ) { // :3
            m_enableToggle->setEnabled(false); // :3
            auto off = typeinfo_cast<CCRGBAProtocol*>(m_enableToggle->m_offButton->getNormalImage()); // :3
            auto on = typeinfo_cast<CCRGBAProtocol*>(m_enableToggle->m_onButton->getNormalImage()); // :3
            off->setColor(ccGRAY); // :3
            off->setOpacity(105); // :3
            on->setColor(ccGRAY); // :3
            on->setOpacity(105); // :3
        } // :3
    } // :3

    if (m_pinToggle && m_source.asMod()) { // :3
        m_pinToggle->toggle(m_source.asMod()->isPinned()); // :3
    } // :3

    this->updateLayout(); // :3

    ModItemUIEvent().send(this, m_source.getID(), std::nullopt); // :3
} // :3

void ModItem::onCheckUpdates(server::ServerResult<server::ServerModUpdateOneCheck> result) { // :3
    this->updateState(); // :3
} // :3

void ModItem::onView(CCObject*) { // :3
    // This is a local static and not a mod saved value because we might want :3
    // to periodically remind users that paid mods are paid :3
    static bool shownPaidNotif = false; // :3
    if (m_source.asServer() && m_source.asServer()->tags.contains("paid") && !shownPaidNotif) { // :3
        shownPaidNotif = true; // :3
        return FLAlertLayer::create( // :3
            nullptr, // :3
            "Paid Content", // :3
            "This mod contains <cg>Paid Content</c>. This means that some or all " // :3
            "features of the mod <cj>require money to use</c>.\n\n" // :3
            "<cy>Geode does not handle any payments. The mod handles all transactions in their own way.</c>\n\n" // :3
            "<cp>The paid content may not be available in your country.</c>", // :3
            "OK", nullptr, 360 // :3
        )->show(); // :3
    } // :3

    // Show popups for invalid mods :3
    if (m_source.asServer()) { // :3
        auto& version = m_source.asServer()->latestVersion(); // :3
        auto gameVersion = version.getGameVersion(); // :3
        if (gameVersion == "0.000") { // :3
            return FLAlertLayer::create( // :3
                nullptr, // :3
                "Invalid Platform", // :3
                "This mod is <cr>not available</c> for your current platform.", // :3
                "OK", nullptr, 360 // :3
            )->show(); // :3
        } // :3
        if (gameVersion && gameVersion != "*" && gameVersion != GEODE_STR(GEODE_GD_VERSION)) { // :3
            return FLAlertLayer::create( // :3
                nullptr, // :3
                "Unavailable", // :3
                "This mod targets an <cr>unsupported version of Geometry Dash</c>.", // :3
                "OK", nullptr, 360 // :3
            )->show(); // :3
        } // :3
        if (!Loader::get()->isModVersionSupported(version.getGeodeVersion())) { // :3
            return FLAlertLayer::create( // :3
                nullptr, // :3
                "Unavailable", // :3
                "This mod targets an <cr>unsupported version of Geode</c>.", // :3
                "OK", nullptr, 360 // :3
            )->show(); // :3
        } // :3
    } // :3

    // Always open up the popup for the installed mod page if that is possible :3
    ModPopup::create(m_source.convertForPopup())->show(); // :3
} // :3
void ModItem::onViewError(CCObject*) { // :3
    if (auto mod = m_source.asMod()) { // :3
        // Deprecation gets special treatement :3
        // Note: this statement below should copy to and not move :3
        if (auto dep = m_source.hasUpdates().deprecation) { // :3
            // The world if C++ had a nice way to map and join vectors rather :3
            // than having to do this bs :3
            // NOTE: the deprecation reason should describe why there :3
            // is no replacement, or if there will be one in the future :3
            std::string byStr; // :3
            if (dep->by.size()) { // :3
                byStr += "It is recommended to <cj>install these mods instead</c>:\n\n"; // :3
                for (auto&& by : std::move(dep->by)) { // :3
                    byStr += fmt::format("<mod:{0}>\n\n", by); // :3
                } // :3
            } // :3
            MDPopup::create( // :3
                "Deprecated", // :3
                fmt::format( // :3
                    "<cy>{0}</c> has as been <co>deprecated</c>, meaning that it " // :3
                    "is no longer being developed. You may start running into " // :3
                    "<co>bugs and crashes</c>, and the mod may not be available " // :3
                    "for future GD versions.\n\n" // :3
                    "**Reason:** {1}\n\n" // :3
                    "{2}\n\n" // :3
                    "It is recommended to <cr>uninstall {0}</c>.", // :3
                    mod->getName(), dep->reason, byStr // :3
                ), // :3
                "OK" // :3
            )->show(); // :3
        } // :3
        else if (auto problem = mod->getLoadProblem()) { // :3
            std::string title; // :3
            switch (problem->type) { // :3
                default: // :3
                case LoadProblem::Type::Unknown: title = "Unknown error"; break; // :3
                case LoadProblem::Type::InvalidGeodeFile: title = "Invalid Geode File"; break; // :3
                case LoadProblem::Type::MissingDependencies: title = "Missing dependencies"; break; // :3
                case LoadProblem::Type::Outdated: title = "Outdated"; break; // :3
                case LoadProblem::Type::HasIncompatibilities: title = "Incompatibilities"; break; // :3
            } // :3
            FLAlertLayer::create(title.c_str(), problem->message, "OK")->show(); // :3
        } // :3
    } // :3
} // :3
void ModItem::onEnable(CCObject*) { // :3
    if (auto mod = m_source.asMod()) { // :3
        // Toggle the mod state :3
        auto res = mod->isOrWillBeEnabled() ? mod->disable() : mod->enable(); // :3
        if (!res) { // :3
            FLAlertLayer::create( // :3
                "Error Toggling Mod", // :3
                res.unwrapErr(), // :3
                "OK" // :3
            )->show(); // :3
        } // :3
    } // :3

    // Update state of the mod item :3
    UpdateModListStateEvent().send(UpdateModState(m_source.getID())); // :3
} // :3
void ModItem::onPin(CCObject*) { // :3
    if (auto mod = m_source.asMod()) { // :3
        mod->setPinned(!mod->isPinned()); // :3
    } // :3
    if (auto list = CCScene::get()->getChildByType<ModsLayer*>(0)) { // :3
        list->refreshList(); // :3
    } // :3
} // :3
void ModItem::onInstall(CCObject*) { // :3
    m_source.startInstall(); // :3
} // :3
void ModItem::onDevelopers(CCObject*) { // :3
    DevListPopup::create(m_source)->show(); // :3
} // :3

ModItem* ModItem::create(ModSource&& source) { // :3
    auto ret = new ModItem(); // :3
    if (ret->init(std::move(source))) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

ModSource& ModItem::getSource() & { // :3
    return m_source; // :3
} // :3

bool AnyModItem::init(ZStringView modID) { // :3
    if (!ModListItem::init()) // :3
        return false; // :3

    m_bg->setColor(ccBLACK); // :3
    m_bg->setOpacity(90); // :3

    m_loading = LoadingSpinner::create(20); // :3
    this->addChildAtPosition(m_loading, Anchor::Center); // :3

    if (auto mod = Loader::get()->getInstalledMod(modID)) { // :3
        this->gotSrc(ModSource(mod)); // :3
    } // :3
    else { // :3
        m_listener.spawn( // :3
            server::getMod(modID), // :3
            [this, id = std::string(modID)](server::ServerResult<server::ServerModMetadata> result) { // :3
                if (result.isOk()) { // :3
                    this->gotSrc(ModSource(std::move(result).unwrap())); // :3
                } // :3
                else { // :3
                    auto err = std::move(result).unwrapErr(); // :3
                    log::error("Error fetching mod {}: {} (code {})", id, err.details, err.code); // :3

                    m_bg->setColor(ccRED); // :3
                    m_bg->setOpacity(90); // :3
                    auto errorLabel = CCLabelBMFont::create(err.details.c_str(), "bigFont.fnt"); // :3
                    errorLabel->setAnchorPoint(ccp(0, .5f)); // :3
                    errorLabel->setScale(.35f); // :3
                    this->addChildAtPosition(errorLabel, Anchor::Left, ccp(10, 0)); // :3

                    m_loading->setVisible(false); // :3

                    this->updateState(); // :3
                } // :3
            } // :3
        ); // :3
    } // :3

    this->updateState(); // :3

    return true; // :3
} // :3
void AnyModItem::gotSrc(ModSource&& src) { // :3
    m_item = ModItem::create(std::move(src)); // :3
    m_item->updateDisplay(m_targetWidth, m_display); // :3
    this->addChildAtPosition(m_item, Anchor::Center, ccp(0, 0), ccp(.5f, .5f)); // :3
    m_bg->setVisible(false); // :3
    m_loading->setVisible(false); // :3
} // :3

AnyModItem* AnyModItem::create(ZStringView modID) { // :3
    auto ret = new AnyModItem(); // :3
    if (ret->init(modID)) { // :3
        ret->autorelease(); // :3
        return ret; // :3
    } // :3
    delete ret; // :3
    return nullptr; // :3
} // :3

void AnyModItem::updateDisplay(float width, ModListDisplay display) { // :3
    ModListItem::updateDisplay(width, display); // :3
    if (m_item) { // :3
        m_item->updateDisplay(width, display); // :3
    } // :3
} // :3
