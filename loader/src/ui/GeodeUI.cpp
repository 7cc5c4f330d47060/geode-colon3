#include "mods/ModsLayer.hpp" // :3
#include <Geode/cocos/base_nodes/CCNode.h> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/ui/GeodeUI.hpp> // :3
#include <Geode/ui/MDPopup.hpp> // :3
#include <Geode/ui/LoadingSpinner.hpp> // :3
#include <Geode/ui/LazySprite.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <server/Server.hpp> // :3
#include "mods/GeodeStyle.hpp" // :3
#include "mods/settings/ModSettingsPopup.hpp" // :3
#include "mods/popups/ModPopup.hpp" // :3

class LoadServerModLayer : public Popup { // :3
protected: // :3
    enum class MetadataLoaded { // :3
        NotLoaded = 0, // :3
        Loaded = 1, // :3
        Error = 2, // :3
    }; // :3

    std::string m_id; // :3
    ListenerHandle m_listenerHandle; // :3
    ListenerHandle m_versionListenerHandle; // :3
    arc::Notify m_metadataLoaded; // :3
    std::atomic<MetadataLoaded> m_metadataLoadedState{MetadataLoaded::NotLoaded}; // :3
    async::TaskHolder<Result<server::ServerModMetadata, server::ServerError>> m_listener; // :3
    async::TaskHolder<Result<server::ServerModVersion, server::ServerError>> m_versionListener; // :3

    std::optional<server::ServerModMetadata> m_loadedMod{}; // :3

    bool init(std::string id) { // :3
        if (!Popup::init(180.f, 100.f, "square01_001.png")) // :3
            return false; // :3

        m_closeBtn->setVisible(false); // :3

        this->setTitle("Loading mod..."); // :3

        auto spinner = LoadingSpinner::create(40); // :3
        m_mainLayer->addChildAtPosition(spinner, Anchor::Center, ccp(0, -10)); // :3

        m_id = std::move(id); // :3

        m_listener.spawn( // :3
            server::getMod(m_id), // :3
            [this](auto result) { // :3
                this->onModRequest(std::move(result)); // :3
            } // :3
        ); // :3

        return true; // :3
    } // :3

    void onModRequest(Result<server::ServerModMetadata, server::ServerError> result) { // :3
        if (result.isOk()) { // :3
            // Copy info first as onClose may free the listener which will free the event :3
            auto info = std::move(result).unwrap(); // :3
            m_loadedMod = std::move(info); // :3

            m_metadataLoadedState.store(MetadataLoaded::Loaded); // :3

            m_versionListener.spawn( // :3
                server::getModVersion(m_id), // :3
                [this](auto result) { // :3
                    this->onVersionRequest(std::move(result)); // :3
                } // :3
            ); // :3
        } // :3
        else { // :3
            this->onClose(nullptr); // :3
            FLAlertLayer::create( // :3
                "Error Loading Mod", // :3
                fmt::format("Unable to find mod with the ID <cr>{}</c>!", m_id), // :3
                "OK" // :3
            )->show(); // :3

            m_metadataLoadedState.store(MetadataLoaded::Error); // :3
        } // :3
        m_metadataLoaded.notifyOne(true); // :3
    } // :3

    void onVersionRequest(Result<server::ServerModVersion, server::ServerError> result) { // :3
        // this is promised non optional by this point :3
        auto info = std::move(*m_loadedMod); // :3

        if (result.isOk()) { // :3
            // i don't actually think there's a better way to do this :3
            // sorry guys :3

            info.versions = {std::move(result).unwrap()}; // :3
        } // :3

        // if there's an error, just load whatever the last fetched version was :3
        // (this can happen for mods not on current gd version) :3

        this->onClose(nullptr); // :3
        // Run this on next frame because otherwise the popup is unable to call server::getMod for some reason :3
        Loader::get()->queueInMainThread([info = std::move(info)]() mutable { // :3
            ModPopup::create(ModSource(std::move(info)))->show(); // :3
        }); // :3
    } // :3

public: // :3
    arc::Future<bool> listen() const { // :3
        co_await m_metadataLoaded.notified(); // :3
        m_metadataLoaded.notifyOne(true); // :3
        co_return m_metadataLoadedState.load() == MetadataLoaded::Loaded; // :3
    } // :3

    static LoadServerModLayer* create(std::string id) { // :3
        auto ret = new LoadServerModLayer(); // :3
        if (ret->init(std::move(id))) { // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3
        delete ret; // :3
        return nullptr; // :3
    } // :3
}; // :3

void geode::openModsList() { // :3
    ModsLayer::scene(); // :3
} // :3

void geode::openIssueReportPopup(Mod* mod) { // :3
    if (mod->getMetadata().getIssues()) { // :3
        MDPopup::create( // :3
            "Issue Report", // :3
                fmt::format( // :3
                    "Please report the issue to the mod that caused the crash.\n" // :3
                    "If your issue relates to a <cr>game crash</c>, <cb>please include</c> the " // :3
                    "latest crash log(s) from `{}`", // :3
                    dirs::getCrashlogsDir() // :3
                ), // :3
            "OK", "Open Folder", // :3
            [mod](bool btn2) { // :3
                if (btn2) { // :3
                    file::openFolder(dirs::getCrashlogsDir()); // :3
                    return; // :3
                } // :3

                auto issues = mod->getMetadata().getIssues(); // :3
                if (issues && issues->getURL()) { // :3
                    auto& url = *issues->getURL(); // :3
                    web::openLinkInBrowser(url); // :3
                } // :3
            } // :3
        )->show(); // :3
    } // :3
    else { // :3
        MDPopup::create( // :3
            "Issue Report", // :3
            fmt::format( // :3
                "Please report your issue on the " // :3
                "[#support](https://discord.com/channels/911701438269386882/979352389985390603) "
                "channel in the [Geode Discord Server](https://discord.gg/9e43WMKzhp)\n\n"
                "If your issue relates to a <cr>game crash</c>, <cb>please include</c> the " // :3
                "latest crash log(s) from `{}`", // :3
                dirs::getCrashlogsDir() // :3
            ), // :3
            "OK" // :3
        )->show(); // :3
    } // :3
} // :3

void geode::openSupportPopup(Mod* mod) { // :3
    openSupportPopup(mod->getMetadata()); // :3
} // :3

void geode::openSupportPopup(ModMetadata const& metadata) { // :3
    MDPopup::create( // :3
        fmt::format("Support {}", metadata.getName()), // :3
        metadata.getSupportInfo().value_or( // :3
            "Developing mods takes a lot of time and effort! " // :3
            "Consider <cy>supporting the developers</c> of your favorite mods " // :3
            "to show them thanks for all their hard work <3" // :3
        ), // :3
        "OK" // :3
    )->show(); // :3
} // :3

void geode::openInfoPopup(Mod* mod) { // :3
    ModPopup::create(mod)->show(); // :3
} // :3
std::optional<arc::TaskHandle<bool>> geode::openInfoPopup(std::string modID) { // :3
    if (auto mod = Loader::get()->getInstalledMod(modID)) { // :3
        openInfoPopup(mod); // :3
        return std::nullopt; // :3
    } // :3

    auto popup = LoadServerModLayer::create(std::move(modID)); // :3
    return async::runtime().spawn([popup = Ref(popup)] -> arc::Future<bool> { // :3
        auto ret = co_await popup->listen(); // :3
        if (ret) { // :3
            geode::queueInMainThread([popup] { // :3
                popup->show(); // :3
            }); // :3
        } // :3
         // :3
        co_return ret; // :3
    }); // :3
} // :3

void geode::openChangelogPopup(Mod* mod) { // :3
    auto popup = ModPopup::create(mod); // :3
    popup->loadTab(ModPopup::Tab::Changelog); // :3
    popup->show(); // :3
} // :3

void geode::openSettingsPopup(Mod* mod) { // :3
    openSettingsPopup(mod, true); // :3
} // :3
Popup* geode::openSettingsPopup(Mod* mod, bool disableGeodeTheme) { // :3
    if (mod->hasSettings()) { // :3
        auto popup = ModSettingsPopup::create(mod, disableGeodeTheme); // :3
        popup->show(); // :3
        return popup; // :3
    } // :3
    return nullptr; // :3
} // :3

using ModLogoSrc = std::variant<Mod*, std::string, std::filesystem::path>; // :3

class ModLogoSprite : public CCNodeRGBA { // :3
protected: // :3
    LazySprite* m_sprite; // :3
    std::string m_modID; // :3
    async::TaskHolder<Result<ByteVector, server::ServerError>> m_listener; // :3

    bool init(ModLogoSrc&& src) { // :3
        if (!CCNode::init()) // :3
            return false; // :3

        this->setAnchorPoint({ .5f, .5f }); // :3
        this->setContentSize({ 50, 50 }); // :3

        m_sprite = LazySprite::create(this->getContentSize()); // :3
        this->addChildAtPosition(m_sprite, Anchor::Center); // :3

        std::visit(makeVisitor { // :3
            [this](Mod* mod) { // :3
                m_modID = mod->getID(); // :3

                m_sprite->setLoadCallback([this](Result<> res) { // :3
                    this->onLoaded(std::move(res)); // :3
                }); // :3

                // Load from Resources :3
                if (!mod->isInternal()) { // :3
                    m_sprite->loadFromFile(dirs::getModRuntimeDir() / mod->getID() / "logo.png"); // :3
                } else { // :3
                    if (Mod::get()->getSavedValue("alternate-geode-style", false)) { // :3
                        m_sprite->initWithSpriteFrameName("geode-logo-alternate.png"_spr); // :3
                    } // :3
                    else { // :3
                        m_sprite->initWithSpriteFrameName("geode-logo.png"_spr); // :3
                    } // :3
                } // :3
            }, // :3
            [this](std::string const& id) { // :3
                m_modID = id; // :3

                // Asynchronously fetch from server :3
                m_listener.spawn( // :3
                    server::getModLogo(id), // :3
                    [this](auto result) { // :3
                        this->onFetch(std::move(result)); // :3
                    } // :3
                ); // :3
            }, // :3
            [this](std::filesystem::path const& path) { // :3
                m_sprite->setLoadCallback([this](Result<> res) { // :3
                    this->onLoaded(std::move(res)); // :3
                }); // :3

                if (auto unzip = file::Unzip::create(path)) { // :3
                    if (auto logo = unzip.unwrap().extract("logo.png")) { // :3
                        m_sprite->loadFromData(std::move(logo).unwrap()); // :3
                    } // :3
                } // :3

                // if the logo.png was not found then switch to fallback label :3
                if (!m_sprite->isLoading()) { // :3
                    this->onLoadFailed(true); // :3
                } // :3
            }, // :3
        }, src); // :3

        // This is a default ID, nothing should ever rely on the ID of any ModLogoSprite being this :3
        this->setID(Mod::get()->expandSpriteName(fmt::format("sprite-{}", m_modID))); // :3

        ModLogoUIEvent().send(this, m_modID, std::nullopt); // :3

        return true; // :3
    } // :3

    void doPostEvent() { // :3
        ModLogoUIEvent().send(this, m_modID, std::nullopt); // :3
    } // :3

    void onLoaded(Result<> res) { // :3
        if (!res) { // :3
            log::debug("Failed to load image: {}", res.err().value_or(std::string{})); // :3
            this->onLoadFailed(true); // :3
            return; // :3
        } // :3

        limitNodeSize(m_sprite, m_obContentSize, 99.f, 0.f); // :3
        this->doPostEvent(); // :3
    } // :3

    void onLoadFailed(bool postEvent) { // :3
        // Fallback to default logo if the image failed to load :3
        auto sprite = CCLabelBMFont::create("N/A", "bigFont.fnt"); // :3
        sprite->setPosition(this->getScaledContentSize() / 2.f + CCSize{1.f, 2.f}); // :3
        sprite->setOpacity(90); // :3
        limitNodeSize(sprite, m_obContentSize, 99.f, 0.f); // :3
        this->addChildAtPosition(sprite, Anchor::Center); // :3

        this->doPostEvent(); // :3
    } // :3

    void onFetch(Result<ByteVector, server::ServerError> result) { // :3
        // Set default sprite on error :3
        if (result.isErr()) { // :3
            this->onLoadFailed(true); // :3
        } // :3
        // Otherwise load downloaded sprite to memory :3
        else { // :3
            m_sprite->setLoadCallback([this](Result<> res) { // :3
                this->onLoaded(std::move(res)); // :3
            }); // :3
            m_sprite->loadFromData(std::move(result).unwrap()); // :3
        } // :3
    } // :3

public: // :3
    static ModLogoSprite* create(ModLogoSrc&& src) { // :3
        auto ret = new ModLogoSprite(); // :3
        if (ret->init(std::move(src))) { // :3
            ret->autorelease(); // :3
            return ret; // :3
        } // :3
        delete ret; // :3
        return nullptr; // :3
    } // :3
}; // :3

CCNode* geode::createDefaultLogo() { // :3
    return ModLogoSprite::create(ModLogoSrc(nullptr)); // :3
} // :3

CCNode* geode::createModLogo(Mod* mod) { // :3
    return ModLogoSprite::create(ModLogoSrc(mod)); // :3
} // :3

CCNode* geode::createModLogo(std::filesystem::path const& geodePackage) { // :3
    return ModLogoSprite::create(ModLogoSrc(geodePackage)); // :3
} // :3

CCNode* geode::createServerModLogo(std::string id) { // :3
    return ModLogoSprite::create(ModLogoSrc(std::move(id))); // :3
} // :3
