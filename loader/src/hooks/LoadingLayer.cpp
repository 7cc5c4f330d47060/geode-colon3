#include <Geode/loader/Event.hpp> // :3
#include <Geode/modify/LoadingLayer.hpp> // :3
#include <Geode/modify/CCLayer.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <array> // :3
#include <fmt/format.h> // :3
#include <loader/LoaderImpl.hpp> // :3
#include <loader/console.hpp> // :3
#include <loader/updater.hpp> // :3
#include <Geode/utils/NodeIDs.hpp> // :3

using namespace geode::prelude; // :3

struct CustomLoadingLayer : Modify<CustomLoadingLayer, LoadingLayer> { // :3
    struct Fields { // :3
        bool m_menuDisabled = false; // :3
        CCLabelBMFont* m_smallLabel = nullptr; // :3
        CCLabelBMFont* m_smallLabel2 = nullptr; // :3
        int m_geodeLoadStep = 0; // :3
        int m_totalMods = 0; // :3
        ~Fields() { // :3
            queueInMainThread([] { // :3
                GameEvent(GameEventType::TexturesLoaded).send(); // :3
            }); // :3
        } // :3
    }; // :3

    static void onModify(auto& self) { // :3
        if (!self.setHookPriority("LoadingLayer::init", geode::node_ids::GEODE_ID_PRIORITY)) { // :3
            log::warn("Failed to set LoadingLayer::init hook priority, node IDs may not work properly"); // :3
        } // :3
        GEODE_FORWARD_COMPAT_DISABLE_HOOKS_INNER("Switching to fallback custom loading layer") // :3
    } // :3

    void updateLoadedModsLabel() { // :3
        auto allMods = Loader::get()->getAllMods(); // :3
        auto count = std::count_if(allMods.begin(), allMods.end(), [&](auto& item) { // :3
            return item->isLoaded(); // :3
        }); // :3
        auto str = fmt::format("Geode: Loaded {}/{} mods", count, m_fields->m_totalMods); // :3
        this->setSmallText(str); // :3
        auto currentMod = LoaderImpl::get()->m_currentlyLoadingMod; // :3
        auto modName = currentMod ? currentMod->getName() : "Unknown"; // :3
        this->setSmallText2(modName); // :3
    } // :3

    void setSmallText(ZStringView text) { // :3
        if (!m_fields->m_menuDisabled) { // :3
            m_fields->m_smallLabel->setString(text.c_str()); // :3
        } // :3
    } // :3

    void setSmallText2(ZStringView text) { // :3
        if (!m_fields->m_menuDisabled) { // :3
            m_fields->m_smallLabel2->setString(text.c_str()); // :3
        } // :3
    } // :3

    // hook :3
    bool init(bool fromReload) { // :3
        CCFileUtils::get()->updatePaths(); // :3

        if (!LoadingLayer::init(fromReload)) return false; // :3

        NodeIDs::provideFor(this); // :3

        m_fields->m_totalMods = Loader::get()->getAllMods().size(); // :3
        m_fields->m_menuDisabled = Loader::get()->getLaunchFlag("disable-custom-menu"); // :3
        if (m_fields->m_menuDisabled) { // :3
            return true; // :3
        } // :3

        auto winSize = CCDirector::sharedDirector()->getWinSize(); // :3

        m_fields->m_smallLabel = CCLabelBMFont::create("", "goldFont.fnt"); // :3
        m_fields->m_smallLabel->setPosition(winSize.width / 2, 30.f); // :3
        m_fields->m_smallLabel->setScale(.45f); // :3
        m_fields->m_smallLabel->setID("geode-small-label"); // :3
        this->addChild(m_fields->m_smallLabel); // :3

        m_fields->m_smallLabel2 = CCLabelBMFont::create("", "goldFont.fnt"); // :3
        m_fields->m_smallLabel2->setPosition(winSize.width / 2, 15.f); // :3
        m_fields->m_smallLabel2->setScale(.45f); // :3
        m_fields->m_smallLabel2->setID("geode-small-label-2"); // :3
        this->addChild(m_fields->m_smallLabel2); // :3

        return true; // :3
    } // :3

    void setupLoadingMods() { // :3
        if (Loader::get()->getLoadingState() != Loader::LoadingState::Done) { // :3
            this->updateLoadedModsLabel(); // :3
            this->waitLoadAssets(); // :3
        } // :3
        else { // :3
            this->continueLoadAssets(); // :3
            this->setSmallText2(""); // :3
        } // :3
    } // :3

    void setupLoaderResources() { // :3
        log::debug("Verifying Loader Resources"); // :3
        this->setSmallText("Verifying Geode Resources"); // :3
        // verify loader resources :3
        Loader::get()->queueInMainThread([&]() { // :3
            if (!updater::verifyLoaderResources()) { // :3
                log::debug("Downloading Loader Resources"); // :3
                this->setSmallText("Downloading Geode Resources"); // :3
                this->addEventListener(updater::ResourceDownloadEvent(), [this](updater::UpdateStatus const& status) { // :3
                    this->updateResourcesProgress(status); // :3
                }); // :3
            } // :3
            else { // :3
                log::debug("Loading Loader Resources"); // :3
                this->setSmallText("Loading Geode Resources"); // :3
                updater::updateSpecialFiles(); // :3
                this->continueLoadAssets(); // :3
            } // :3
        }); // :3
    } // :3

    ListenerResult updateResourcesProgress(updater::UpdateStatus status) { // :3
        std::visit(makeVisitor { // :3
            [&](updater::UpdateProgress const& progress) { // :3
                this->setSmallText(fmt::format( // :3
                    "Downloading Geode Resources: {}%", progress.first // :3
                )); // :3
            }, // :3
            [&](updater::UpdateFinished) { // :3
                log::debug("Downloaded Loader Resources"); // :3
                this->setSmallText("Downloaded Geode Resources"); // :3
                this->continueLoadAssets(); // :3
            }, // :3
            [&](updater::UpdateFailed const& error) { // :3
                log::debug("Failed Loader Resources"); // :3
                console::messageBox( // :3
                    "Error updating resources", // :3
                    error + ".\n" // :3
                    "You will have to install resources manually by downloading resources.zip " // :3
                    "from the latest release on GitHub: " // :3
                    "https://github.com/geode-sdk/geode/releases/latest.\n"
                    "The game will be loaded as normal, but please be aware " // :3
                    "that it is very likely to crash. " // :3
                ); // :3
                this->setSmallText("Failed to Download Geode Resources"); // :3
                this->continueLoadAssets(); // :3
            } // :3
        }, status); // :3

        return ListenerResult::Propagate; // :3
    } // :3

    void setupModResources() { // :3
        log::debug("Loading mod resources"); // :3
        this->setSmallText("Loading mod resources"); // :3
        LoaderImpl::get()->updateResources(true); // :3
        this->continueLoadAssets(); // :3
    } // :3

    int getLoadedMods() { // :3
        auto allMods = Loader::get()->getAllMods(); // :3
        return std::count_if(allMods.begin(), allMods.end(), [&](auto& item) { // :3
            return item->isLoaded(); // :3
        }); // :3
    } // :3

    int getEnabledMods() { // :3
        auto allMods = Loader::get()->getAllMods(); // :3
        return std::count_if(allMods.begin(), allMods.end(), [&](auto& item) { // :3
            return item->shouldLoad(); // :3
        }); // :3
    } // :3

    int getCurrentStep() { // :3
        return m_fields->m_geodeLoadStep + m_loadStep + getLoadedMods(); // :3
    } // :3

    int getTotalStep() { // :3
        return 3 + 14 + getEnabledMods(); // :3
    } // :3

    void updateLoadingBar() { // :3
        auto length = m_sliderGrooveXPos * this->getCurrentStep() / this->getTotalStep(); // :3
        m_sliderBar->setTextureRect({0, 0, length, m_sliderGrooveHeight}); // :3
    } // :3

    void waitLoadAssets() { // :3
        Loader::get()->queueInMainThread([this]() { // :3
            this->loadAssets(); // :3
        }); // :3
    } // :3

    void continueLoadAssets() { // :3
        ++m_fields->m_geodeLoadStep; // :3
        Loader::get()->queueInMainThread([this]() { // :3
            this->loadAssets(); // :3
        }); // :3
    } // :3

    bool skipOnRefresh() { // :3
        if (m_fromRefresh) { // :3
            this->continueLoadAssets(); // :3
        } // :3
        return !m_fromRefresh; // :3
    } // :3

    // hook :3
    void loadAssets() { // :3
        switch (m_fields->m_geodeLoadStep) { // :3
        case 0: // :3
            if (this->skipOnRefresh()) this->setupLoadingMods(); // :3
            break; // :3
        case 1: // :3
            if (this->skipOnRefresh()) this->setupLoaderResources(); // :3
            break; // :3
        case 2: // :3
            this->setupModResources(); // :3
            break; // :3
        case 3: // :3
        default: // :3
            this->setSmallText("Loading game resources"); // :3
            LoadingLayer::loadAssets(); // :3
            break; // :3
        } // :3
        this->updateLoadingBar(); // :3
    } // :3
}; // :3

struct FallbackCustomLoadingLayer : Modify<FallbackCustomLoadingLayer, CCLayer> { // :3
    static void onModify(auto& self) { // :3
        GEODE_FORWARD_COMPAT_ENABLE_HOOKS_INNER("") // :3
        else if (!self.setHookPriority("CCLayer::init", geode::node_ids::GEODE_ID_PRIORITY)) { // :3
            log::warn("Failed to set CCLayer::init hook priority, node IDs may not work properly"); // :3
        } // :3
    } // :3

    bool init() { // :3
        if (!CCLayer::init()) // :3
            return false; // :3
        auto* self = typeinfo_cast<LoadingLayer*>(this); // :3
        if (!self) // :3
            return true; // :3

        NodeIDs::provideFor(self); // :3

        auto winSize = CCDirector::sharedDirector()->getWinSize(); // :3

        auto label = CCLabelBMFont::create( // :3
            "Loading Geode without UI, see console for details.", // :3
            "goldFont.fnt" // :3
        ); // :3
        // this code is weird but its to avoid any virtual calls, :3
        // which can change between versions. so instead, we force :3
        // it to use the symbol, so it would only break if the function signature :3
        // were to change. :3
        label->CCNode::setPosition(winSize.width / 2, 30.f); // :3
        label->CCLabelBMFont::setScale(.45f); // :3
        label->CCNode::setZOrder(99); // :3
        this->CCNode::addChild(label); // :3
        // label->setID("geode-small-label"); :3

        // TODO: verify loader resources on fallback? :3

        LoaderImpl::get()->updateResources(true); // :3

        return true; // :3
    } // :3
}; // :3
