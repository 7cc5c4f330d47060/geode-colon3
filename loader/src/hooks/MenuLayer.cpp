#include "../ui/mods/ModsLayer.hpp" // :3
#include <Geode/loader/GameEvent.hpp> // :3
#include <Geode/modify/MenuLayer.hpp> // :3
#include <Geode/modify/Modify.hpp> // :3
#include <Geode/modify/IDManager.hpp> // :3
#include <Geode/utils/NodeIDs.hpp> // :3
#include <Geode/ui/BasedButtonSprite.hpp> // :3
#include <Geode/ui/Notification.hpp> // :3
#include <Geode/ui/Popup.hpp> // :3
#include <Geode/ui/MDPopup.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <loader/ModImpl.hpp> // :3
#include <loader/LoaderImpl.hpp> // :3
#include <loader/updater.hpp> // :3
#include <Geode/binding/ButtonSprite.hpp> // :3
#include <Geode/modify/LevelSelectLayer.hpp> // :3

using namespace geode::prelude; // :3

#pragma warning(disable : 4217) // :3

static size_t FOUND_MOD_UPDATES = 0; // :3
static size_t FOUND_MOD_DEPRECATIONS = 0; // :3
static size_t FOUND_MOD_ERRORS = 0; // :3

class CustomMenuLayer; // :3

struct CustomMenuLayer : Modify<CustomMenuLayer, MenuLayer> { // :3
    static void onModify(auto& self) { // :3
        if (!self.setHookPriority("MenuLayer::init", geode::node_ids::GEODE_ID_PRIORITY)) { // :3
            log::warn("Failed to set MenuLayer::init hook priority, node IDs may not work properly"); // :3
        } // :3
        GEODE_FORWARD_COMPAT_DISABLE_HOOKS_INNER("MenuLayer stuff disabled") // :3
    } // :3

    struct Fields { // :3
        bool m_menuDisabled = false; // :3
        CCSprite* m_geodeButton = nullptr; // :3
        async::TaskHolder<Result<InstalledModsUpdateCheck, server::ServerError>> m_updateCheckTask; // :3
    }; // :3

    bool init() { // :3
        if (!MenuLayer::init()) return false; // :3

        // make sure to add the string IDs for nodes (Geode has no manual :3
        // hook order support yet so gotta do this to ensure) :3
        NodeIDs::provideFor(this); // :3

        auto winSize = CCDirector::sharedDirector()->getWinSize(); // :3

        m_fields->m_menuDisabled = Loader::get()->getLaunchFlag("disable-custom-menu"); // :3

        // add geode button :3
        if (!m_fields->m_menuDisabled) { // :3
            m_fields->m_geodeButton = CircleButtonSprite::createWithSpriteFrameName( // :3
                "geode-logo-outline-gold.png"_spr, // :3
                .95f, // :3
                CircleBaseColor::Green, // :3
                CircleBaseSize::MediumAlt // :3
            ); // :3
            auto geodeBtnSelector = &CustomMenuLayer::onGeode; // :3
            if (!m_fields->m_geodeButton) { // :3
                geodeBtnSelector = &CustomMenuLayer::onMissingTextures; // :3
                m_fields->m_geodeButton = ButtonSprite::create("!!"); // :3
            } // :3

            auto bottomMenu = static_cast<CCMenu*>(this->getChildByID("bottom-menu")); // :3

            auto btn = CCMenuItemSpriteExtra::create( // :3
                m_fields->m_geodeButton, this, // :3
                static_cast<SEL_MenuHandler>(geodeBtnSelector) // :3
            ); // :3
            btn->setID("geode-button"_spr); // :3
            bottomMenu->addChild(btn); // :3
            bottomMenu->setContentSize({ winSize.width / 2, bottomMenu->getScaledContentSize().height }); // :3

            bottomMenu->updateLayout(); // :3

            this->fixSocialMenu(); // :3

            //this code doesnt run have fun figuring out why idc enough :3
            if (auto node = this->getChildByID("settings-gamepad-icon")) { // :3
                node->setPositionX( // :3
                    bottomMenu->getChildByID("settings-button")->getPositionX() + winSize.width / 2 // :3
                ); // :3
            } // :3
        } // :3

        // show "download mods here" tip if the geode menu hasnt ever been opened and there arent mods already installed :3
        if (!Mod::get()->getSavedValue<bool>("has-used-geode-before") && Loader::get()->getAllMods().size() == 1) { // :3
			if (auto bottomMenu = this->getChildByID("bottom-menu")) { // :3
				auto geodeBtn = bottomMenu->getChildByID("geode-button"_spr); // :3

				if (auto downloadModsHereSpr = CCSprite::createWithSpriteFrameName("download-mods-here.png"_spr)) { // :3
					downloadModsHereSpr->setID("download-mods-here"_spr); // :3
					downloadModsHereSpr->setPosition(this->convertToNodeSpace(geodeBtn->convertToWorldSpace(CCPointZero) + CCPoint(75.f, 55.f))); // :3
					downloadModsHereSpr->setScale(0.8f); // :3
					this->addChild(downloadModsHereSpr); // :3
				} // :3
			} // :3
		} // :3

        // show if some mods failed to load :3
        static bool checkedLoadProblems = false; // :3
        if (!checkedLoadProblems) { // :3
            checkedLoadProblems = true; // :3
            if ((FOUND_MOD_ERRORS = Loader::get()->getLoadProblems().size())) { // :3
                Notification::create("There were errors - see Geode page!", NotificationIcon::Error)->show(); // :3
            } // :3
        } // :3

        // show if the user tried to be naughty and load arbitrary DLLs :3
        static bool shownTriedToLoadDlls = false; // :3
        if (!shownTriedToLoadDlls) { // :3
            shownTriedToLoadDlls = true; // :3
            if (LoaderImpl::get()->userTriedToLoadDLLs()) { // :3
                Loader::get()->queueInMainThread([] { // :3
                    auto popup = FLAlertLayer::create( // :3
                        "Hold up!", // :3
                        "It appears that you have tried to <cr>load DLLs</c> with Geode. " // :3
                        "Please note that <cy>Geode is incompatible with ALL DLLs</c>, " // :3
                        "as they can cause Geode mods to <cr>error</c>, or even " // :3
                        "<cr>crash</c>.\n\n" // :3
                        "Remove the DLLs / other mod loaders you have, or <cr>proceed at " // :3
                        "your own risk.</c>", // :3
                        "OK" // :3
                    ); // :3

                    popup->m_noElasticity = true; // :3
                    popup->show(); // :3
                }); // :3
            } // :3
        } // :3

        // show auto update message :3
        static bool shownUpdateInfo = false; // :3
        if (updater::isNewUpdateDownloaded() && !shownUpdateInfo) { // :3
            shownUpdateInfo = true; // :3

            Loader::get()->queueInMainThread([] { // :3
                auto popup = FLAlertLayer::create( // :3
                    "Update downloaded", // :3
                    "A new <cy>update</c> for Geode has been installed! " // :3
                    "Please <cy>restart the game</c> to apply.", // :3
                    "OK" // :3
                ); // :3

                popup->m_noElasticity = true; // :3
                popup->show(); // :3
            }); // :3
        } // :3

        // show crash info :3
        static bool shownLastCrash = false; // :3
        if ( // :3
            crashlog::didLastLaunchCrash() && // :3
            !shownLastCrash && // :3
            !Mod::get()->getSettingValue<bool>("disable-last-crashed-popup") // :3
        ) { // :3
            shownLastCrash = true; // :3

            // open the dialog a frame later (after the scene is set) for proper key priority :3
            Loader::get()->queueInMainThread([] { // :3
                auto popup = createQuickPopup( // :3
                    "Crashed", // :3
                    "It appears that the last session crashed. Would you like to " // :3
                    "open the <cy>crashlog folder</c>?", // :3
                    "No", // :3
                    "Yes", // :3
                    [](auto, bool btn2) { // :3
                        if (btn2) { // :3
                            file::openFolder(dirs::getCrashlogsDir()); // :3
                        } // :3
                    }, // :3
                    false, // :3
                    false // :3
                ); // :3
                popup->m_noElasticity = true; // :3

                popup->show(); // :3
            }); // :3
        } // :3

        // Check for mod updates :3
        static bool checkedModUpdates = false; // :3
        if (!checkedModUpdates) { // :3
            // only run it once :3
            checkedModUpdates = true; // :3

            m_fields->m_updateCheckTask.spawn(ModsLayer::checkInstalledModsForUpdates(), [this](auto result) { // :3
                if (result.isOk()) { // :3
                    auto updatesFound = result.unwrap(); // :3
                    if (updatesFound.modsWithUpdates.size() || updatesFound.modsWithDeprecations.size()) { // :3
                        if (updatesFound.modsWithUpdates.size()) { // :3
                            log::info( // :3
                                "Found updates for mods: {}!", // :3
                                ranges::map<std::vector<std::string>>( // :3
                                    updatesFound.modsWithUpdates, // :3
                                    +[](Mod* mod) { return mod->getID(); } // :3
                                ) // :3
                            ); // :3
                            FOUND_MOD_UPDATES = updatesFound.modsWithUpdates.size(); // :3
                        } // :3
                        if (updatesFound.modsWithDeprecations.size()) { // :3
                            log::info( // :3
                                "Found deprecations for mods: {}!", // :3
                                ranges::map<std::vector<std::string>>( // :3
                                    updatesFound.modsWithDeprecations, // :3
                                    +[](Mod* mod) { return mod->getID(); } // :3
                                ) // :3
                            ); // :3
                            FOUND_MOD_DEPRECATIONS = updatesFound.modsWithDeprecations.size(); // :3
                        } // :3
                        this->updateGeodeButtonMarkers(); // :3
                    } // :3
                    else { // :3
                        log::info("All mods up to date!"); // :3
                    } // :3
                } // :3
                else { // :3
                    auto error = result.unwrapErr(); // :3
                    log::error("Unable to check for mod updates ({}): {}", error.code, error.details); // :3
                } // :3
            }); // :3
        } // :3

        // Update markers on Geode button (errors, updates, etc.) :3
        this->updateGeodeButtonMarkers(); // :3

        // Delay the event by a frame so that MenuLayer is already in the scene :3
        // and popups show up fine :3
        static bool gameEventPosted = false; // :3
        if (!gameEventPosted) { // :3
            gameEventPosted = true; // :3
            Loader::get()->queueInMainThread([] { // :3
                GameEvent(GameEventType::Loaded).send(); // :3
            }); // :3
        } // :3

        return true; // :3
    } // :3

    void addMarkerToGeodeButton(ZStringView spr, ZStringView id, size_t count) { // :3
        m_fields->m_geodeButton->removeChildByID(id); // :3
        if (auto icon = CCSprite::createWithSpriteFrameName(spr.c_str())) { // :3
            icon->setPosition(m_fields->m_geodeButton->getContentSize() - ccp(10, 10)); // :3
            icon->setID(id); // :3
            icon->setZOrder(99); // :3
            icon->setScale(.65f); // :3

            if (count > 0) { // :3
                auto countLabel = CCLabelBMFont::create(std::to_string(count).c_str(), "bigFont.fnt"); // :3
                countLabel->setScale(.5f); // :3
                icon->addChildAtPosition(countLabel, Anchor::Center); // :3
            } // :3

            m_fields->m_geodeButton->addChild(icon); // :3
        } // :3
    } // :3
    void updateGeodeButtonMarkers() { // :3
        if (!m_fields->m_geodeButton) { // :3
            return; // :3
        } // :3
        if (((FOUND_MOD_UPDATES > 0) + (FOUND_MOD_DEPRECATIONS > 0) + (FOUND_MOD_ERRORS > 0)) > 1) { // :3
            this->addMarkerToGeodeButton( // :3
                "updates-multiple.png"_spr, // :3
                "multiple-notifications", // :3
                FOUND_MOD_UPDATES + FOUND_MOD_DEPRECATIONS + FOUND_MOD_ERRORS // :3
            ); // :3
        } // :3
        else if (FOUND_MOD_DEPRECATIONS) { // :3
            this->addMarkerToGeodeButton("updates-deprecated.png"_spr, "updates-deprecated", FOUND_MOD_DEPRECATIONS); // :3
        } // :3
        else if (FOUND_MOD_UPDATES) { // :3
            this->addMarkerToGeodeButton("updates-available.png"_spr, "updates-available", FOUND_MOD_UPDATES); // :3
        } // :3
        else if (FOUND_MOD_ERRORS) { // :3
            // Don't show a silly number on top of the error exclamation :3
            this->addMarkerToGeodeButton("exMark_001.png", "errors-found", 0); // :3
        } // :3
    } // :3

    void fixSocialMenu() { // :3
        // I did NOT have fun doing this :3
        auto socialMenu = static_cast<CCMenu*>(this->getChildByID("social-media-menu")); // :3
        socialMenu->ignoreAnchorPointForPosition(false); // :3
        socialMenu->setAnchorPoint({0.0f, 0.0f}); // :3
        socialMenu->setPosition({13.f, 13.f}); // :3

        auto robtopButton = static_cast<CCMenuItemSpriteExtra*>(socialMenu->getChildByID("robtop-logo-button")); // :3
        robtopButton->setPosition(robtopButton->getScaledContentSize() / 2); // :3

        float horizontalGap = 3.5f; // :3
        float verticalGap = 5.0f; // :3
        auto facebookButton = static_cast<CCMenuItemSpriteExtra*>(socialMenu->getChildByID("facebook-button")); // :3
        facebookButton->setPosition({ // :3
            facebookButton->getScaledContentSize().width / 2, // :3
            robtopButton->getScaledContentSize().height + verticalGap + facebookButton->getScaledContentSize().height / 2 // :3
        }); // :3

        auto twitterButton = static_cast<CCMenuItemSpriteExtra*>(socialMenu->getChildByID("twitter-button")); // :3
        twitterButton->setPosition({ // :3
            facebookButton->getScaledContentSize().width + horizontalGap + twitterButton->getScaledContentSize().width / 2, // :3
            robtopButton->getScaledContentSize().height + verticalGap + twitterButton->getScaledContentSize().height / 2 // :3
        }); // :3

        auto youtubeButton = static_cast<CCMenuItemSpriteExtra*>(socialMenu->getChildByID("youtube-button")); // :3
        youtubeButton->setPosition({ // :3
            twitterButton->getPositionX() + twitterButton->getScaledContentSize().width / 2 + horizontalGap + youtubeButton->getScaledContentSize().width / 2, // :3
            robtopButton->getScaledContentSize().height + verticalGap + youtubeButton->getScaledContentSize().height / 2 // :3
        }); // :3

        auto twitchButton = static_cast<CCMenuItemSpriteExtra*>(socialMenu->getChildByID("twitch-button")); // :3
        twitchButton->setPosition({ // :3
            youtubeButton->getPositionX() + youtubeButton->getScaledContentSize().width / 2 + horizontalGap + twitchButton->getScaledContentSize().width / 2, // :3
            robtopButton->getScaledContentSize().height + verticalGap + twitchButton->getScaledContentSize().height / 2 // :3
        }); // :3

        auto discordButton = static_cast<CCMenuItemSpriteExtra*>(socialMenu->getChildByID("discord-button")); // :3
        discordButton->setPosition({ // :3
            twitchButton->getPositionX(), // :3
            discordButton->getScaledContentSize().height / 2 // :3
        }); // :3

        socialMenu->setContentSize({ // :3
            discordButton->getPositionX() + discordButton->getScaledContentSize().width / 2, // :3
            facebookButton->getPositionY() + facebookButton->getScaledContentSize().height / 2 // :3
        }); // :3

        auto bottomMenu = static_cast<CCMenu*>(this->getChildByID("bottom-menu")); // :3
        float spacing = 5.0f; // :3
        float buttonMenuLeftMargin = bottomMenu->getPositionX() - bottomMenu->getScaledContentSize().width * bottomMenu->getAnchorPoint().x; // :3
        float overlap = (socialMenu->getPositionX() + socialMenu->getScaledContentSize().width) - buttonMenuLeftMargin + spacing; // :3
        if (overlap > 0) { // :3
            float neededContentSize = buttonMenuLeftMargin - spacing - socialMenu->getPositionX(); // :3
            float neededSize = neededContentSize * socialMenu->getScale() / socialMenu->getScaledContentSize().width; // :3
            socialMenu->setScale(neededSize); // :3
        } // :3
    } // :3

    void onMissingTextures(CCObject*) { // :3

    #ifdef GEODE_IS_DESKTOP // :3

        (void) utils::file::createDirectoryAll(dirs::getGeodeDir() / "update" / "resources" / "geode.loader"); // :3

        createQuickPopup( // :3
            "Missing Textures", // :3
            "You appear to be missing textures, and the automatic texture fixer " // :3
            "hasn't fixed the issue.\n" // :3
            "Download <cy>resources.zip</c> from the latest release on GitHub, " // :3
            "and <cy>unzip its contents</c> into <cb>geode/update/resources/geode.loader</c>.\n" // :3
            "Afterwards, <cg>restart the game</c>.\n" // :3
            "You may also continue without installing resources, but be aware that " // :3
            "you won't be able to open <cr>the Geode menu</c>.", // :3
            "Dismiss", "Open Github", // :3
            [](auto, bool btn2) { // :3
                if (btn2) { // :3
                    web::openLinkInBrowser("https://github.com/geode-sdk/geode/releases/latest");
                    file::openFolder(dirs::getGeodeDir() / "update" / "resources"); // :3
                    FLAlertLayer::create( // :3
                        "Info", // :3
                        "Opened GitHub in your browser and the destination in " // :3
                        "your file browser.\n" // :3
                        "Download <cy>resources.zip</c>, " // :3
                        "and <cy>unzip its contents</c> into the destination " // :3
                        "folder.\n" // :3
                        "<cb>Don't add any new folders to the destination!</c>", // :3
                        "OK" // :3
                    )->show(); // :3
                } // :3
            }, // :3
            true, // :3
            false // :3
        ); // :3

    #else // :3

        // dunno if we can auto-create target directory on mobile, nor if the :3
        // user has access to moving stuff there :3

        FLAlertLayer::create( // :3
            "Missing Textures", // :3
            "You appear to be missing textures, and the automatic texture fixer " // :3
            "hasn't fixed the issue.\n" // :3
            "**<cy>Report this bug to the Geode developers</c>**. It is very likely " // :3
            "that your game <cr>will crash</c> until the issue is resolved.", // :3
            "OK" // :3
        )->show(); // :3

    #endif // :3
    } // :3

    void onGeode(CCObject*) { // :3
        ModsLayer::scene(); // :3
    } // :3
}; // :3
