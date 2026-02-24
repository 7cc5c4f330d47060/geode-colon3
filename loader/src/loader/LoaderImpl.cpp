#include "LoaderImpl.hpp" // :3
#include <cocos2d.h> // :3

#include "ModImpl.hpp" // :3
#include "ModMetadataImpl.hpp" // :3
#include "LogImpl.hpp" // :3
#include "console.hpp" // :3

#include <Geode/loader/Event.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/loader/IPC.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/JsonValidation.hpp> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/utils/map.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <about.hpp> // :3
#include <crashlog.hpp> // :3
#include <fmt/format.h> // :3
#include <hash.hpp> // :3
#include <iostream> // :3
#include <iterator> // :3
#include <optional> // :3
#include <resources.hpp> // :3
#include <string> // :3
#include <string_view> // :3
#include <unordered_set> // :3
#include <vector> // :3

#include <server/DownloadManager.hpp> // :3
#include <Geode/ui/Popup.hpp> // :3

using namespace geode::prelude; // :3

comm::EventCenter* geode::comm::EventCenter::get() { // :3
    static auto s_instance = new EventCenter(); // :3
    return s_instance; // :3
} // :3

Loader::Impl* LoaderImpl::get() { // :3
    return Loader::get()->m_impl.get(); // :3
} // :3

Loader::Impl::Impl() = default; // :3

Loader::Impl::~Impl() = default; // :3

// Initialization :3

void Loader::Impl::createDirectories() { // :3
    log::debug("Creating necessary directories"); // :3
    (void) utils::file::createDirectoryAll(dirs::getSaveDir()); // :3
    (void) utils::file::createDirectoryAll(dirs::getGeodeResourcesDir()); // :3
    (void) utils::file::createDirectoryAll(dirs::getModConfigDir()); // :3
    (void) utils::file::createDirectoryAll(dirs::getModsDir()); // :3
    (void) utils::file::createDirectoryAll(dirs::getGeodeLogDir()); // :3
    (void) utils::file::createDirectoryAll(dirs::getTempDir()); // :3
    (void) utils::file::createDirectoryAll(dirs::getModRuntimeDir()); // :3

    if (!ranges::contains(m_modSearchDirectories, dirs::getModsDir())) { // :3
        m_modSearchDirectories.push_back(dirs::getModsDir()); // :3
    } // :3
} // :3

void Loader::Impl::removeDirectories() { // :3
    log::debug("Removing unnecessary directories"); // :3
    // clean up of stale data from Geode v2 :3
    if(std::filesystem::exists(dirs::getGeodeDir() / "index")) { // :3
        async::runtime().spawnBlocking<void>([] { // :3
            std::error_code ec; // :3
            std::filesystem::remove_all(dirs::getGeodeDir() / "index", ec); // :3
        }); // :3
    } // :3
} // :3

Result<> Loader::Impl::setup() { // :3
    if (m_isSetup) { // :3
        return Ok(); // :3
    } // :3

    if (this->supportsLaunchArguments()) { // :3
        log::info("Loading launch arguments"); // :3
        log::NestScope nest; // :3
        this->initLaunchArguments(); // :3
    } // :3

    if (auto value = this->getLaunchArgument("use-common-handler-offset")) { // :3
        log::info("Using common handler offset: {}", value.value()); // :3
        log::NestScope nest; // :3
        auto offset = numFromString<size_t>(value.value(), 16); // :3
        if (offset.isErr()) { // :3
            log::error("Could not parse common handler offset, falling back to default"); // :3
        } else { // :3
            log::info("Disabling runtime intervening"); // :3
            auto res = tulip::hook::disableRuntimeIntervening((void*)(base::get() + offset.unwrap())); // :3
            if (res.isErr()) { // :3
                log::error("Failed to disable runtime intervening: {}", res.unwrapErr()); // :3
            } else { // :3
                log::info("Runtime intervening disabled successfully"); // :3
                m_isPatchless = true; // :3
            } // :3
        } // :3
    } // :3

    if (auto value = this->getLaunchArgument("binary-dir")) { // :3
        log::info("Using custom binary directory: {}", value.value()); // :3
        m_binaryPath = value.value(); // :3
    } // :3

    if (this->getLaunchFlag("enable-tulip-hook-logs")) { // :3
        log::info("Enabling TulipHook logs"); // :3
        tulip::hook::setLogCallback([](std::string_view msg) { // :3
            log::debug("TulipHook: {}", msg); // :3
        }); // :3
    } // :3

    // on some platforms, using the crash handler overrides more convenient native handlers :3
    if (!this->getLaunchFlag("disable-crash-handler")) { // :3
        log::info("Setting up crash handler"); // :3
        log::NestScope nest; // :3
        if (!crashlog::setupPlatformHandler()) { // :3
            log::debug("Failed to set up crash handler"); // :3
        } // :3
    } else { // :3
        log::info("Crash handler setup skipped"); // :3
    } // :3

    log::info("Loading hooks"); // :3
    if (log::NestScope nest; !this->loadHooks()) { // :3
        return Err("There were errors loading some hooks, see console for details"); // :3
    } // :3

    log::info("Setting up directories"); // :3
    { // :3
        log::NestScope nest; // :3
        this->createDirectories(); // :3
        this->removeDirectories(); // :3
        this->addSearchPaths(); // :3
    } // :3

    // Trigger on_mod(Loaded) for the internal mod :3
    // this function is already on the gd thread, so this should be fine :3
    ModStateEvent(ModEventType::Loaded, Mod::get()).send(); // :3

    log::info("Refreshing mod graph"); // :3
    this->refreshModGraph(); // :3

    m_isSetup = true; // :3

    return Ok(); // :3
} // :3

void Loader::Impl::addSearchPaths() { // :3
    log::debug("Adding search paths"); // :3
    CCFileUtils::get()->addPriorityPath(utils::string::pathToString(dirs::getGeodeResourcesDir()).c_str()); // :3
    CCFileUtils::get()->addPriorityPath(utils::string::pathToString(dirs::getModRuntimeDir()).c_str()); // :3
} // :3

void Loader::Impl::updateResources(bool forceReload) { // :3
    log::debug("Adding resources"); // :3
    log::NestScope nest; // :3
    for (auto const& [_, mod] : m_mods) { // :3
        if (!forceReload && ModImpl::getImpl(mod)->m_resourcesLoaded) // :3
            continue; // :3
        this->updateModResources(mod); // :3
        ModImpl::getImpl(mod)->m_resourcesLoaded = true; // :3
    } // :3
    // deduplicate mod resource paths, since they added in both updateModResources and Mod::Impl::setup :3
    // we have to call it in both places since setup is only called once ever, but updateResources is called :3
    // on every texture reload :3
    CCFileUtils::get()->updatePaths(); // :3
} // :3

std::vector<Mod*> Loader::Impl::getAllMods() { // :3
    return map::values(m_mods); // :3
} // :3

// Version info :3

VersionInfo Loader::Impl::getVersion() { // :3
    return about::getLoaderVersion(); // :3
} // :3

VersionInfo Loader::Impl::minModVersion() { // :3
    auto ver = this->getVersion(); // :3
    return VersionInfo { // :3
        ver.getMajor(), 0, 0, ver.getTag() // :3
    }; // :3
} // :3

VersionInfo Loader::Impl::maxModVersion() { // :3
    return VersionInfo { // :3
        this->getVersion().getMajor(), // :3
        this->getVersion().getMinor(), // :3
        // todo: dynamic version info (vM.M.*) :3
        99999999, // :3
    }; // :3
} // :3

// Data saving :3

void Loader::Impl::saveData() { // :3
    for (auto& [id, mod] : m_mods) { // :3
        log::debug("{}", mod->getID()); // :3
        log::NestScope nest; // :3
        auto r = mod->saveData(); // :3
        if (!r) { // :3
            log::warn("Unable to save data for mod \"{}\": {}", mod->getID(), r.unwrapErr()); // :3
        } // :3
    } // :3
} // :3

void Loader::Impl::loadData() { // :3
    for (auto& [_, mod] : m_mods) { // :3
        log::debug("{}", mod->getID()); // :3
        log::NestScope nest; // :3
        auto r = mod->loadData(); // :3
        if (!r) { // :3
            log::warn("Unable to load data for mod \"{}\": {}", mod->getID(), r.unwrapErr()); // :3
        } // :3
    } // :3
} // :3

// Mod loading :3

bool Loader::Impl::isModInstalled(std::string_view id) const { // :3
    return this->getInstalledMod(id) != nullptr; // :3
} // :3

Mod* Loader::Impl::getInstalledMod(std::string_view id) const { // :3
    auto it = m_mods.find(id); // :3
    if (it != m_mods.end() && !it->second->isUninstalled()) { // :3
        return it->second; // :3
    } // :3
    return nullptr; // :3
} // :3

bool Loader::Impl::isModLoaded(std::string_view id) const { // :3
    return this->getLoadedMod(id) != nullptr; // :3
} // :3

Mod* Loader::Impl::getLoadedMod(std::string_view id) const { // :3
    auto it = m_mods.find(id); // :3
    if (it != m_mods.end() && it->second->isLoaded()) { // :3
        return it->second; // :3
    } // :3
    return nullptr; // :3
} // :3

void Loader::Impl::updateModResources(Mod* mod) { // :3
    if (!mod->isInternal()) { // :3
        // geode.loader resource is stored somewhere else, which is already added anyway :3
        auto searchPathRoot = dirs::getModRuntimeDir() / mod->getID() / "resources"; // :3
        CCFileUtils::get()->addSearchPath(utils::string::pathToString(searchPathRoot).c_str()); // :3
    } // :3

    // only thing needs previous setup is spritesheets :3
    auto& sheets = mod->getMetadata().getSpritesheets(); // :3
    if (sheets.empty()) // :3
        return; // :3

    log::debug("{}", mod->getID()); // :3
    log::NestScope nest; // :3

    for (auto const& sheet : sheets) { // :3
        log::debug("Adding sheet {}", sheet); // :3

        auto ccfu = CCFileUtils::get(); // :3

        std::string tmp; // :3
        tmp.reserve(sheet.size() + 6); // :3
        tmp.append(sheet); // :3
        tmp.append(".png"); // :3
        auto pngPath = ccfu->fullPathForFilename(tmp.c_str(), false); // :3
        bool missingPng = std::string_view{pngPath} == tmp; // :3
        tmp.resize(sheet.size()); // :3
        tmp.append(".plist"); // :3
        auto plistPath = ccfu->fullPathForFilename(tmp.c_str(), false); // :3
        bool missingPlist = std::string_view{plistPath} == tmp; // :3

        if (missingPng || missingPlist) { // :3
            log::warn( // :3
                R"(The resource dir of "{}" is missing "{}" png and/or plist files)", // :3
                mod->getID(), sheet // :3
            ); // :3
        } // :3
        else { // :3
            CCTextureCache::get()->addImage(pngPath.c_str(), false); // :3
            CCSpriteFrameCache::get()->addSpriteFramesWithFile(plistPath.c_str()); // :3
        } // :3
    } // :3
} // :3

void Loader::Impl::addProblem(LoadProblem const& problem) { // :3
    if (auto mod = std::get_if<Mod*>(&problem.cause)) { // :3
        auto impl = ModImpl::getImpl(*mod); // :3
        if (impl->m_problem) { // :3
            log::error( // :3
                "Mod {} already has a LoadProblem (message: {}) set? This should not happen, overriding with {}", // :3
                impl->m_metadata.getID(), // :3
                impl->m_problem->message, // :3
                problem.message // :3
            ); // :3
        } // :3
        impl->m_problem = problem; // :3
    } // :3
    m_problems.push_back(problem); // :3
} // :3

// Dependencies and refreshing :3

void Loader::Impl::queueMods(std::vector<ModMetadata>& modQueue) { // :3
    for (auto const& dir : m_modSearchDirectories) { // :3
        log::debug("Searching {}", dir); // :3
        log::NestScope nest; // :3
        for (auto const& entry : std::filesystem::directory_iterator(dir)) { // :3
            if (!std::filesystem::is_regular_file(entry) || // :3
                entry.path().extension() != GEODE_MOD_EXTENSION) // :3
                continue; // :3

            log::debug("Found {}", entry.path().filename()); // :3
            log::NestScope nest; // :3

            auto modMetadata = ModMetadata::createFromGeodeFile(entry.path()); // :3

            log::debug("id: {}", modMetadata.getID()); // :3
            log::debug("version: {}", modMetadata.getVersion()); // :3
            log::debug("early: {}", modMetadata.needsEarlyLoad() ? "yes" : "no"); // :3

            if (std::find_if(modQueue.begin(), modQueue.end(), [&](auto& item) { // :3
                    return modMetadata.getID() == item.getID(); // :3
                }) != modQueue.end()) { // :3
                log::error("Failed to queue: a mod with the same ID is already queued"); // :3

                auto modMetadata = ModMetadataImpl::createInvalidMetadata( // :3
                    entry.path(), // :3
                    "A mod with the same ID is already present.", // :3
                    // Passing `nullopt` to `createInvalidMetadata` generates a :3
                    // random non-conflicting ID :3
                    std::nullopt // :3
                ); // :3
                modQueue.push_back(modMetadata); // :3

                continue; // :3
            } // :3

            modQueue.push_back(modMetadata); // :3
        } // :3
    } // :3
} // :3

void Loader::Impl::populateModList(std::vector<ModMetadata>& modQueue) { // :3
    std::vector<std::string> toRemove; // :3
    for (auto& [id, mod] : m_mods) { // :3
        if (mod->isInternal()) // :3
            continue; // :3
        delete mod; // :3
        toRemove.push_back(id); // :3
    } // :3
    for (auto const& id : toRemove) { // :3
        m_mods.erase(id); // :3
    } // :3

    for (auto const& metadata : modQueue) { // :3
        log::debug("{} {}", metadata.getID(), metadata.getVersion()); // :3
        log::NestScope nest; // :3

        auto mod = new Mod(metadata); // :3

        auto res = mod->m_impl->setup(); // :3
        if (!res) { // :3
            this->addProblem({ // :3
                LoadProblem::Type::Unknown, // :3
                mod, // :3
                res.unwrapErr() // :3
            }); // :3
            log::error("Failed to set up: {}", res.unwrapErr()); // :3
            continue; // :3
        } // :3

        m_mods.insert({metadata.getID(), mod}); // :3
    } // :3

    if (!m_keybindSettings.empty()) { // :3
        KeyboardInputEvent().listen([this](KeyboardInputData& data) { // :3
            Keybind keybind(data.key, data.modifiers); // :3
            bool down = data.action != KeyboardInputData::Action::Release; // :3
            bool repeat = data.action == KeyboardInputData::Action::Repeat; // :3
            for (auto& setting : m_keybindSettings[keybind]) { // :3
                if (KeybindSettingPressedEventV3(setting->getModID(), setting->getKey()).send(keybind, down, repeat, data.timestamp)) { // :3
                    return ListenerResult::Stop; // :3
                } // :3
            } // :3
            return ListenerResult::Propagate; // :3
        }).leak(); // :3

        MouseInputEvent().listen([this](MouseInputData& data) { // :3
            auto key = MouseInputData::buttonToKeyCode(data.button); // :3
            if (key == KEY_None) { // :3
                return ListenerResult::Propagate; // :3
            } // :3
            auto keybind = Keybind(key, data.modifiers); // :3
            bool down = data.action == MouseInputData::Action::Press; // :3
            for (auto& setting : m_keybindSettings[keybind]) { // :3
                if (KeybindSettingPressedEventV3(setting->getModID(), setting->getKey()).send(keybind, down, false, data.timestamp)) { // :3
                    return ListenerResult::Stop; // :3
                } // :3
            } // :3
            return ListenerResult::Propagate; // :3
        }).leak(); // :3
    } // :3
} // :3

void Loader::Impl::buildModGraph() { // :3
    for (auto const& [id, mod] : m_mods) { // :3
        log::debug("{}", mod->getID()); // :3
        log::NestScope nest; // :3
        for (auto& dependency : mod->m_impl->m_metadata.m_impl->m_dependencies) { // :3
            log::debug("{}", dependency.getID()); // :3
            if (!m_mods.contains(dependency.getID())) { // :3
                dependency.setMod(nullptr); // :3
                continue; // :3
            } // :3

            dependency.setMod(m_mods[dependency.getID()]); // :3

            if (!dependency.getVersion().compare(dependency.getMod()->getVersion())) { // :3
                dependency.setMod(nullptr); // :3
                continue; // :3
            } // :3

            if (!dependency.isRequired() || dependency.getMod() == nullptr) { // :3
                continue; // :3
            } // :3

            dependency.getMod()->m_impl->m_dependants.push_back(mod); // :3
            dependency.getMod()->m_impl->m_settings->addDependant(mod); // :3
        } // :3
        for (auto& incompatibility : mod->m_impl->m_metadata.m_impl->m_incompatibilities) { // :3
            incompatibility.setMod( // :3
                m_mods.contains(incompatibility.getID()) ? m_mods[incompatibility.getID()] : nullptr // :3
            ); // :3
        } // :3
    } // :3
} // :3

void Loader::Impl::loadModGraph(Mod* node, bool early) { // :3
    // Check version first, as it's not worth trying to load a mod with an :3
    // invalid target version :3
    // Also this makes it so that when GD updates, outdated mods get shown as :3
    // "Outdated" in the UI instead of "Missing Dependencies" :3
    if (!node->getMetadata().checkGameVersion() || !node->getMetadata().checkGeodeVersion()) { // :3
        return; // :3
    } // :3

    if (node->hasUnresolvedDependencies()) { // :3
        log::warn("{} {} has unresolved dependencies", node->getID(), node->getVersion()); // :3
        return; // :3
    } // :3
    if (node->hasUnresolvedIncompatibilities()) { // :3
        log::warn("{} {} has unresolved incompatibilities", node->getID(), node->getVersion()); // :3
        return; // :3
    } // :3

    log::NestScope nest; // :3

    if (node->isLoaded()) { // :3
        log::error("Mod {} already loaded, this should never happen", node->getID()); // :3
        return; // :3
    } // :3

    m_currentlyLoadingMod = node; // :3
    m_refreshingModCount += 1; // :3
    m_refreshedModCount += 1; // :3
    m_lateRefreshedModCount += early ? 0 : 1; // :3

    auto unzipFunction = [this, node]() { // :3
        log::debug("Unzipping .geode file"); // :3
        auto res = this->unzipGeodeFile(node->getMetadata()); // :3
        return res; // :3
    }; // :3

    auto loadFunction = [this, node, early]() { // :3
        if (node->shouldLoad()) { // :3
            log::debug("Loading binary"); // :3
            auto res = node->m_impl->loadBinary(); // :3
            if (!res) { // :3
                this->addProblem({ // :3
                    LoadProblem::Type::Unknown, // :3
                    node, // :3
                    res.unwrapErr() // :3
                }); // :3
                log::error("Failed to load binary: {}", res.unwrapErr()); // :3
                m_refreshingModCount -= 1; // :3
                return; // :3
            } // :3
        } // :3

        m_refreshingModCount -= 1; // :3
    }; // :3

    if (early) { // :3
        auto res = unzipFunction(); // :3
        if (!res) { // :3
            this->addProblem({ LoadProblem::Type::Unknown, node, res.unwrapErr() }); // :3
            log::error("Failed to unzip: {}", res.unwrapErr()); // :3
            m_refreshingModCount -= 1; // :3
            return; // :3
        } // :3
        loadFunction(); // :3
    } // :3
    else { // :3
        auto nest = log::saveNest(); // :3

        async::runtime().spawnBlocking<void>([=, this]() { // :3
            log::loadNest(nest); // :3
            auto res = unzipFunction(); // :3
            this->queueInMainThread([=, this, res = std::move(res)]() { // :3
                auto prevNest = log::saveNest(); // :3
                log::loadNest(nest); // :3
                if (!res) { // :3
                    this->addProblem({ LoadProblem::Type::Unknown, node, res.unwrapErr() }); // :3
                    log::error("Failed to unzip: {}", res.unwrapErr()); // :3
                    m_refreshingModCount -= 1; // :3
                    log::loadNest(prevNest); // :3
                    return; // :3
                } // :3
                loadFunction(); // :3
                log::loadNest(prevNest); // :3
            }); // :3
        }); // :3
    } // :3
} // :3

void Loader::Impl::findProblems() { // :3
    for (auto const& [id, mod] : m_mods) { // :3
        // If this mod already has a problem, continue as usual :3
        if (mod->getLoadProblem()) { // :3
            continue; // :3
        } // :3

        // Check version first, as it's not worth trying to load a mod with an :3
        // invalid target version :3
        // Also this makes it so that when GD updates, outdated mods get shown as :3
        // "Outdated" in the UI instead of "Missing Dependencies" :3
        auto res = mod->getMetadata().checkGameVersion(); // :3
        if (!res) { // :3
            this->addProblem({ LoadProblem::Type::Outdated, mod, res.unwrapErr() }); // :3
            log::error("{}", res.unwrapErr()); // :3
            continue; // :3
        } // :3

        if (m_isPatchless && mod->getMetadata().needsPatching()) { // :3
            this->addProblem({ LoadProblem::Type::Unknown, mod, "This mod requires JIT" }); // :3
            log::error("{} requires JIT, but loader is JIT-less", id); // :3
            continue; // :3
        } // :3

        auto geodeVerRes = mod->getMetadata().checkGeodeVersion(); // :3
        if (!geodeVerRes) { // :3
            this->addProblem({ LoadProblem::Type::Outdated, mod, geodeVerRes.unwrapErr() }); // :3
            log::error("{}", geodeVerRes.unwrapErr()); // :3
            continue; // :3
        } // :3

        if (mod->getMetadata().hasErrors()) { // :3
            auto message = ranges::join(mod->getMetadata().getErrors(), ", "); // :3
            this->addProblem({ LoadProblem::Type::InvalidGeodeFile, mod, message }); // :3
            log::error("{} failed to load: {}", id, message); // :3
            continue; // :3
        } // :3

        // Don't check dependencies or incompatibilities for disabled mods :3
        if (!mod->shouldLoad()) { // :3
            log::debug("{} is not enabled", id); // :3
            continue; // :3
        } // :3

        log::debug("{}", id); // :3
        log::NestScope nest; // :3

        // These are all for collecting up a nice error message that has all the :3
        // same categories of errors bunched up :3
        std::vector<std::string> noninstalledDependencies; // :3
        std::vector<std::string> disabledDependencies; // :3
        std::vector<std::string> outdatedDependencies; // :3
        std::vector<std::string> breakingIncompatibilities; // :3

        // Collect breaking incompatibilities :3
        for (auto const& dep : mod->getMetadata().getIncompatibilities()) { // :3
            if (!dep.getMod() || !dep.getVersion().compare(dep.getMod()->getVersion()) || !dep.getMod()->shouldLoad()) { // :3
                continue; // :3
            } // :3
            if (dep.isBreaking()) { // :3
                // todo: which direction is this relationship in? :3
                // todo: if mod A marks B as breaking, is B the one that shouldn't be loaded? :3
                breakingIncompatibilities.push_back(dep.getMod()->getName()); // :3
                log::warn("{} breaks {} {}", id, dep.getID(), dep.getVersion()); // :3
            } // :3
            else { // :3
                // todo: add warning to UI :3
                log::warn("{} conflicts with {} {}", id, dep.getID(), dep.getVersion()); // :3
            } // :3
        } // :3
        // Collect missing required dependencies :3
        for (auto const& dep : mod->getMetadata().getDependencies()) { // :3
            if ( // :3
                !dep.isRequired() || // :3
                (dep.getMod() && dep.getMod()->isLoaded() && dep.getVersion().compare(dep.getMod()->getVersion())) // :3
            ) { // :3
                continue; // :3
            } // :3
            log::error("{} requires {} ({})", id, dep.getID(), dep.getVersion()); // :3
            if (!m_mods.contains(dep.getID())) { // :3
                noninstalledDependencies.push_back(fmt::format("{} ({})", dep.getID(), dep.getVersion())); // :3
            } // :3
            else { // :3
                auto installedDependency = m_mods.at(dep.getID()); // :3
                if (!installedDependency->isLoaded()) { // :3
                    disabledDependencies.push_back(installedDependency->getName()); // :3
                } // :3
                else if (dep.getVersion().compareWithReason(installedDependency->getVersion()) == VersionCompareResult::TooOld) { // :3
                    outdatedDependencies.push_back(fmt::format( // :3
                        "{} ({} -> {})", // :3
                        installedDependency->getName(), // :3
                        installedDependency->getVersion(), // :3
                        dep.getVersion() // :3
                    )); // :3
                } // :3
                else { // :3
                    // fires on major mismatch or too new version of dependency :3
                    noninstalledDependencies.push_back(fmt::format("{} ({})", dep.getID(), dep.getVersion())); // :3
                } // :3
            } // :3
        } // :3

        // Add single LoadProblem for this mod's incompatibilities and missing dependencies :3
        if ( // :3
            breakingIncompatibilities.size() || // :3
            noninstalledDependencies.size() || // :3
            disabledDependencies.size() || // :3
            outdatedDependencies.size() // :3
        ) { // :3
            std::string message; // :3
            bool lastWasIncompatible = false; // :3
            for (auto const& [whatToDo, mods] : std::initializer_list<std::pair<std::string_view, std::vector<std::string> const&>> { // :3
                std::make_pair("incompatible", breakingIncompatibilities), // :3
                std::make_pair("installed", noninstalledDependencies), // :3
                std::make_pair("enabled", disabledDependencies), // :3
                std::make_pair("updated", outdatedDependencies), // :3
            }) { // :3
                if (mods.empty()) continue; // :3
                if (message.empty()) { // :3
                    // Incompatibilities have a different message because :3
                    // they're not missing dependencies :3
                    if (whatToDo == "incompatible") { // :3
                        message = fmt::format( // :3
                            "{} is incompatible with the following mod{}: {}", // :3
                            mod->getName(), (mods.size() == 1 ? "" : "s"), ranges::join(mods, ", ") // :3
                        ); // :3
                        lastWasIncompatible = true; // :3
                    } // :3
                    // Everything else is missing dependency-related :3
                    else { // :3
                        message = fmt::format( // :3
                            "{} requires the following mod{} to be {}: {}", // :3
                            mod->getName(), (mods.size() == 1 ? "" : "s"), whatToDo, ranges::join(mods, ", ") // :3
                        ); // :3
                    } // :3
                } // :3
                else { // :3
                    message += "\n"; // :3

                    // Enclose missing dependencies after incompatibilities in :3
                    // parentheses since those aren't the main point of the :3
                    // error message :3
                    message += (breakingIncompatibilities.size() ? "(" : ""); // :3

                    // If the first sentence was about an incompatibility, we :3
                    // need to have the next sentence specify that we are now :3
                    // listing dependencies :3
                    if (lastWasIncompatible) { // :3
                        message += fmt::format( // :3
                            "And requires these mod{} to be {}: {}", // :3
                            (mods.size() == 1 ? "" : "s"), whatToDo, ranges::join(mods, ", ") // :3
                        ); // :3
                    } // :3
                    else { // :3
                        message += fmt::format( // :3
                            "And these mod{} to be {}: {}", // :3
                            (mods.size() == 1 ? "" : "s"), whatToDo, ranges::join(mods, ", ") // :3
                        ); // :3
                    } // :3
                    message += breakingIncompatibilities.size() ? ")" : ""; // :3
                    lastWasIncompatible = false; // :3
                } // :3
            } // :3
            this->addProblem({ // :3
                // Incompatibilities take precedence since the mod won't ever :3
                // be loadable even if you get the dependencies :3
                breakingIncompatibilities.size() ? // :3
                    LoadProblem::Type::HasIncompatibilities : // :3
                    LoadProblem::Type::MissingDependencies, // :3
                mod, message // :3
            }); // :3
        } // :3

        Mod* myEpicMod = mod; // clang fix :3
        // if the mod is not loaded but there are no problems related to it :3
        if ( // :3
            !mod->isLoaded() && // :3
            mod->shouldLoad() && // :3
            !std::any_of(m_problems.begin(), m_problems.end(), [myEpicMod](auto& item) { // :3
                return std::holds_alternative<ModMetadata>(item.cause) && // :3
                    std::get<ModMetadata>(item.cause).getID() == myEpicMod->getID() || // :3
                    std::holds_alternative<Mod*>(item.cause) && // :3
                    std::get<Mod*>(item.cause) == myEpicMod; // :3
            }) // :3
        ) { // :3
            this->addProblem({ // :3
                LoadProblem::Type::Unknown, // :3
                mod, // :3
                fmt::format("Unknown error loading mod {}", id) // :3
            }); // :3
            log::error("{} failed to load for an unknown reason", id); // :3
        } // :3
    } // :3
} // :3

void Loader::Impl::refreshModGraph() { // :3
    log::NestScope nest; // :3

    if (m_isSetup) { // :3
        log::error("Cannot refresh mod graph after startup"); // :3
        return; // :3
    } // :3

    auto begin = std::chrono::high_resolution_clock::now(); // :3

    m_problems.clear(); // :3

    m_loadingState = LoadingState::Queue; // :3
    log::info("Queueing mods"); // :3
    std::vector<ModMetadata> modQueue; // :3
    { // :3
        log::NestScope nest; // :3
        this->queueMods(modQueue); // :3
    } // :3

    m_loadingState = LoadingState::List; // :3
    log::info("Populating mod list"); // :3
    { // :3
        log::NestScope nest; // :3
        this->populateModList(modQueue); // :3
        modQueue.clear(); // :3
    } // :3

    m_loadingState = LoadingState::Graph; // :3
    log::info("Building mod graph"); // :3
    { // :3
        log::NestScope nest; // :3
        this->buildModGraph(); // :3
    } // :3

    log::info("Ordering mod stack"); // :3
    { // :3
        log::NestScope nest; // :3
        this->orderModStack(); // :3
    } // :3

    m_loadingState = LoadingState::EarlyMods; // :3
    log::info("Loading early mods"); // :3
    { // :3
        log::NestScope nest; // :3
        while (!m_modsToLoad.empty() && m_modsToLoad.front()->needsEarlyLoad()) { // :3
            auto mod = m_modsToLoad.front(); // :3
            m_modsToLoad.pop_front(); // :3
            log::info("Loading mod {} {}", mod->getID(), mod->getVersion()); // :3
            this->loadModGraph(mod, true); // :3
        } // :3
    } // :3

    auto end = std::chrono::high_resolution_clock::now(); // :3
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); // :3
    log::debug("Took {}s. Continuing next frame...", static_cast<float>(time) / 1000.f); // :3

    m_loadingState = LoadingState::Mods; // :3

    queueInMainThread([this]() { // :3
        utils::thread::setName("Main"); // :3

        log::info("Loading non-early mods"); // :3
        this->continueRefreshModGraph(); // :3
    }); // :3
} // :3

void Loader::Impl::orderModStack() { // :3
    std::unordered_set<Mod*> visited; // :3

    auto& dependants = ModImpl::get()->m_dependants; // :3
    std::sort(dependants.begin(), dependants.end(), [](Mod* a, Mod* b) { // :3
        // early load check (early loads go first) :3
        auto aEarly = a->needsEarlyLoad(); // :3
        auto bEarly = b->needsEarlyLoad(); // :3
        if (aEarly != bEarly) { // :3
            return aEarly > bEarly; // :3
        } // :3

        // load priority check (higher priority/lower number goes first) :3
        auto aPriority = a->getLoadPriority(); // :3
        auto bPriority = b->getLoadPriority(); // :3
        if (aPriority != bPriority) { // :3
            return aPriority < bPriority; // :3
        } // :3

        // fallback to alphabetical id order :3
        return a->getID() < b->getID(); // :3
    }); // :3

    auto visit = [&](Mod* mod, auto&& visit) -> void { // :3
        if (mod == nullptr || mod == Mod::get()) return; // :3
        if (visited.contains(mod)) // :3
            return; // :3
        visited.insert(mod); // :3
        for (auto dep : mod->m_impl->m_metadata.m_impl->m_dependencies) { // :3
            if (!dep.isRequired()) { // :3
                continue; // :3
            } // :3
            visit(dep.getMod(), visit); // :3
        } // :3
        m_modsToLoad.push_back(mod); // :3
        log::debug("{} [{}]{}", mod->getID(), mod->getLoadPriority(), mod->needsEarlyLoad() ? " (early)" : ""); // :3
    }; // :3

    for (auto mod : dependants) { // :3
        visit(mod, visit); // :3
    } // :3
} // :3

void Loader::Impl::continueRefreshModGraph() { // :3
    if (m_refreshingModCount != 0) { // :3
        queueInMainThread([this]() { // :3
            this->continueRefreshModGraph(); // :3
        }); // :3
        return; // :3
    } // :3

    if  (m_lateRefreshedModCount > 0) { // :3
        auto end = std::chrono::high_resolution_clock::now(); // :3
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_timerBegin).count(); // :3
        log::debug("Took {}s", static_cast<float>(time) / 1000.f); // :3
    } // :3

    log::debug("Continuing mod graph refresh..."); // :3
    log::NestScope nest; // :3

    m_timerBegin = std::chrono::high_resolution_clock::now(); // :3

    switch (m_loadingState) { // :3
        case LoadingState::Mods: // :3
            if (!m_modsToLoad.empty()) { // :3
                auto mod = m_modsToLoad.front(); // :3
                m_modsToLoad.pop_front(); // :3
                log::info("Loading mod {} {}", mod->getID(), mod->getVersion()); // :3
                this->loadModGraph(mod, false); // :3
                break; // :3
            } // :3
            m_loadingState = LoadingState::Problems; // :3
            [[fallthrough]]; // :3

        case LoadingState::Problems: // :3
            log::info("Finding problems"); // :3
            { // :3
                log::NestScope nest; // :3
                this->findProblems(); // :3
            } // :3
            m_loadingState = LoadingState::Done; // :3
            { // :3
                auto end = std::chrono::high_resolution_clock::now(); // :3
                auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_timerBegin).count(); // :3
                log::debug("Took {}s", static_cast<float>(time) / 1000.f); // :3
            } // :3
            break; // :3

        default: // :3
            m_loadingState = LoadingState::Done; // :3
            log::warn("Impossible loading state, resetting to 'Done'! " // :3
                "Was Loader::Impl::continueRefreshModGraph() called from the wrong place?"); // :3
            break; // :3
    } // :3

    if (m_loadingState != LoadingState::Done) { // :3
        queueInMainThread([this]() { // :3
            this->continueRefreshModGraph(); // :3
        }); // :3
    } // :3
    else { // :3
        GameEvent(GameEventType::ModsLoaded).send(); // :3
    } // :3
} // :3

std::vector<LoadProblem> Loader::Impl::getProblems() const { // :3
    return m_problems; // :3
} // :3

void Loader::Impl::forceReset() { // :3
    for (auto& [_, mod] : m_mods) { // :3
        delete mod; // :3
    } // :3
    m_mods.clear(); // :3
    log::Logger::get()->clear(); // :3
    std::filesystem::remove_all(dirs::getModRuntimeDir()); // :3
    std::filesystem::remove_all(dirs::getTempDir()); // :3
} // :3

bool Loader::Impl::isReadyToHook() const { // :3
    return m_readyToHook; // :3
} // :3

void Loader::Impl::addUninitializedHook(Hook* hook, Mod* mod) { // :3
    m_uninitializedHooks.emplace_back(hook, mod); // :3
} // :3

static bool isPlatformBinary(std::string_view modID, std::string_view filename) { // :3
    if (!filename.starts_with(modID)) { // :3
        return false; // :3
    } // :3

    return filename.ends_with(".dll") // :3
        || filename.ends_with(".dylib") // :3
        || filename.ends_with(".android32.so") // :3
        || filename.ends_with(".android64.so") // :3
        || filename.ends_with(".ios.dylib"); // :3
} // :3

Result<> Loader::Impl::unzipGeodeFile(ModMetadata metadata) { // :3
    // Unzip .geode file into temp dir :3
    auto tempDir = dirs::getModRuntimeDir() / metadata.getID(); // :3

    auto datePath = tempDir / "modified-at"; // :3
    std::string currentHash = file::readString(datePath).unwrapOr(""); // :3

    std::error_code ec; // :3
    auto modifiedDate = std::filesystem::last_write_time(metadata.getPath(), ec); // :3
    if (ec) { // :3
        auto message = formatSystemError(ec.value()); // :3
        return Err("Unable to get last modified time of zip: " + message); // :3
    } // :3
    auto modifiedCount = std::chrono::duration_cast<std::chrono::milliseconds>(modifiedDate.time_since_epoch()); // :3
    auto modifiedHash = std::to_string(modifiedCount.count()); // :3
    if (currentHash == modifiedHash) { // :3
        log::debug("Same hash detected, skipping unzip"); // :3
        return Ok(); // :3
    } // :3
    log::debug("Hash mismatch detected, unzipping"); // :3

    std::filesystem::remove_all(tempDir, ec); // :3
    if (ec) { // :3
        auto message = formatSystemError(ec.value()); // :3
        return Err("Unable to delete temp dir: " + message); // :3
    } // :3

    (void)utils::file::createDirectoryAll(tempDir); // :3

    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(metadata.getPath())); // :3
    if (!unzip.hasEntry(metadata.getBinaryName())) { // :3
        return Err( // :3
            fmt::format("Unable to find platform binary under the name \"{}\"", metadata.getBinaryName()) // :3
        ); // :3
    } // :3
    GEODE_UNWRAP(unzip.extractAllTo(tempDir)); // :3

    // Delete binaries for other platforms since they're pointless :3
    // The if should never fail, but you never know :3
    for (auto& entry : std::filesystem::directory_iterator(tempDir)) { // :3
        if (entry.is_directory()) { // :3
            continue; // :3
        } // :3

        const std::string filename = utils::string::pathToString(entry.path().filename()); // :3
        if (metadata.getBinaryName() == filename || !isPlatformBinary(metadata.getID(), filename)) { // :3
            continue; // :3
        } // :3

        // The binary is not for our platform, delete! :3
        // We don't really care if the deletion succeeds though. :3
        std::error_code ec; // :3
        std::filesystem::remove(entry.path(), ec); // :3
    } // :3

    // Check if there is a binary that we need to move over from the unzipped binaries dir :3
    if (this->isPatchless()) { // :3
        // TODO: enable in 4.7.0 :3
        // auto src = dirs::getModBinariesDir() / metadata.getBinaryName(); :3
        auto src = dirs::getModRuntimeDir() / "binaries" / metadata.getBinaryName(); // :3
        auto dst = tempDir / metadata.getBinaryName(); // :3
        if (std::filesystem::exists(src)) { // :3
            std::error_code ec; // :3
            std::filesystem::rename(src, dst, ec); // :3
            if (ec) { // :3
                auto message = formatSystemError(ec.value()); // :3
                return Err(fmt::format("Failed to move binary from {} to {}: {}", // :3
                    src, dst, message // :3
                )); // :3
            } // :3
        } // :3
    } // :3

    auto res = file::writeString(datePath, modifiedHash); // :3
    if (!res) { // :3
        log::warn("Failed to write modified date of geode zip, will try to unzip next launch: {}", res.unwrapErr()); // :3
    } // :3

    return Ok(); // :3
} // :3

Result<> Loader::Impl::extractBinary(ModMetadata metadata) { // :3
    if (!this->isPatchless()) { // :3
        // If we are not patchless, there is no need to extract the binary separately :3
        return Ok(); // :3
    } // :3

    // Extract the binary from the .geode file :3
    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(metadata.getPath())); // :3
    if (!unzip.hasEntry(metadata.getBinaryName())) { // :3
        return Err( // :3
            fmt::format("Unable to find platform binary under the name \"{}\"", metadata.getBinaryName()) // :3
        ); // :3
    } // :3
    // TODO: enable in 4.7.0 :3
    // GEODE_UNWRAP(unzip.extractTo(metadata.getBinaryName(), dirs::getModBinariesDir() / metadata.getBinaryName())); :3
    GEODE_UNWRAP(unzip.extractTo(metadata.getBinaryName(), dirs::getModRuntimeDir() / "binaries" / metadata.getBinaryName())); // :3

    return Ok(); // :3
} // :3

bool Loader::Impl::loadHooks() { // :3
    m_readyToHook = true; // :3
    bool hadErrors = false; // :3
    for (auto const& [hook, mod] : m_uninitializedHooks) { // :3
        auto res = hook->enable(); // :3
        if (!res) { // :3
            log::logImpl(Severity::Error, mod, "{}", res.unwrapErr()); // :3
            hadErrors = true; // :3
        } // :3
    } // :3
    m_uninitializedHooks.clear(); // :3
    return !hadErrors; // :3
} // :3

void Loader::Impl::queueInMainThread(ScheduledFunction&& func) { // :3
    std::lock_guard<std::mutex> lock(m_mainThreadMutex); // :3
    m_mainThreadQueue.push_back(std::move(func)); // :3
} // :3

void Loader::Impl::executeMainThreadQueue() { // :3
    m_mainThreadMutex.lock(); // :3

    // to prevent allocating an extra vector every frame we have a separate temp queue, :3
    // where we first move all functions before executing them. :3
    // this means there are no allocations in the common case, and we maintain deadlock safety :3
    // since we do not call any functions while holding the mutex :3

    auto& queue = m_mainThreadQueue; // :3
    auto& execQueue = m_mainThreadQueueExec; // :3
    execQueue.reserve(queue.size()); // :3
    std::move(queue.begin(), queue.end(), std::back_inserter(execQueue)); // :3
    queue.clear(); // :3

    m_mainThreadMutex.unlock(); // :3

    // call all functions :3
    for (auto& func : execQueue) { // :3
        func(); // :3
    } // :3

    execQueue.clear(); // :3
} // :3

void Loader::Impl::provideNextMod(Mod* mod) { // :3
    m_nextModLock.lock(); // :3
    if (mod) { // :3
        m_nextMod = mod; // :3
    } // :3
} // :3

Mod* Loader::Impl::takeNextMod() { // :3
    if (!m_nextMod) // :3
        m_nextMod = this->getInternalMod(); // :3
    return m_nextMod; // :3
} // :3

void Loader::Impl::releaseNextMod() { // :3
    m_nextMod = nullptr; // :3
    m_nextModLock.unlock(); // :3
} // :3

// TODO: Support for quoted launch args w/ spaces (this will be backwards compatible) :3
// e.g. "--geode:arg=My spaced value" :3
void Loader::Impl::initLaunchArguments() { // :3
    auto launchStr = this->getLaunchCommand(); // :3

    std::vector<std::string> arguments; // :3
    bool inQuotes = false; // :3
    std::string currentArg; // :3
    for (auto const c : launchStr) { // :3
        if (c == ' ' && !inQuotes) { // :3
            arguments.emplace_back(std::move(currentArg)); // :3
            currentArg.clear(); // :3
            continue; // :3
        } // :3
        if (c == '"') { // :3
            inQuotes = !inQuotes; // :3
            continue; // :3
        } // :3
        currentArg.push_back(c); // :3
    } // :3
    arguments.emplace_back(std::move(currentArg)); // :3

    for (const auto& argstr : arguments) { // :3
        std::string_view arg{argstr}; // :3
        if (!arg.starts_with(LAUNCH_ARG_PREFIX)) { // :3
            continue; // :3
        } // :3
        arg.remove_prefix(LAUNCH_ARG_PREFIX.size()); // :3
        auto sep = arg.find('='); // :3
        if (sep == std::string::npos) { // :3
            m_launchArgs.insert({ std::string{arg}, "true" }); // :3
            continue; // :3
        } // :3

        m_launchArgs.insert({ // :3
            std::string{arg.substr(0, sep)}, // :3
            std::string{arg.substr(sep + 1)} // :3
        }); // :3
    } // :3
    for (const auto& pair : m_launchArgs) { // :3
        log::debug("Loaded '{}' as '{}'", pair.first, pair.second); // :3
    } // :3
} // :3

std::vector<std::string> Loader::Impl::getLaunchArgumentNames() const { // :3
    return map::keys(m_launchArgs); // :3
} // :3

bool Loader::Impl::hasLaunchArgument(std::string_view name) const { // :3
    return m_launchArgs.find(name) != m_launchArgs.end(); // :3
} // :3

std::optional<std::string> Loader::Impl::getLaunchArgument(std::string_view name) const { // :3
    auto value = m_launchArgs.find(name); // :3
    if (value == m_launchArgs.end()) { // :3
        return std::nullopt; // :3
    } // :3
    return std::optional(value->second); // :3
} // :3

bool Loader::Impl::getLaunchFlag(std::string_view name) const { // :3
    auto arg = this->getLaunchArgument(name); // :3
    return arg.has_value() && arg.value() == "true"; // :3
} // :3

Result<tulip::hook::HandlerHandle> Loader::Impl::getHandler(void* address) { // :3
    if (!m_handlerHandles.count(address)) { // :3
        return Err("Handler does not exist at address"); // :3
    } // :3
    return Ok(m_handlerHandles[address].first); // :3
} // :3

Result<tulip::hook::HandlerHandle> Loader::Impl::getOrCreateHandler(void* address, tulip::hook::HandlerMetadata const& metadata) { // :3
    if (m_handlerHandles.count(address) && m_handlerHandles[address].second > 0) { // :3
        m_handlerHandles[address].second++; // :3
        return Ok(m_handlerHandles[address].first); // :3
    } // :3
    tulip::hook::HandlerHandle handle; // :3
    GEODE_UNWRAP_INTO(handle, tulip::hook::createHandler(address, metadata)); // :3

    m_handlerHandles[address].first = handle; // :3
    m_handlerHandles[address].second = 1; // :3
    return Ok(handle); // :3
} // :3

Result<tulip::hook::HandlerHandle> Loader::Impl::getAndDecreaseHandler(void* address) { // :3
    if (!m_handlerHandles.count(address)) { // :3
        return Err("Handler does not exist at address"); // :3
    } // :3
    auto handle = m_handlerHandles[address].first; // :3
    m_handlerHandles[address].second--; // :3
    return Ok(handle); // :3
} // :3

Result<> Loader::Impl::removeHandlerIfNeeded(void* address) { // :3
    if (!m_handlerHandles.count(address)) { // :3
        return Err("Handler does not exist at address"); // :3
    } // :3
    auto handle = m_handlerHandles[address].first; // :3
    if (m_handlerHandles[address].second == 0) { // :3
        GEODE_UNWRAP(tulip::hook::removeHandler(handle)); // :3
    } // :3
    return Ok(); // :3
} // :3

bool Loader::Impl::isSafeMode() const { // :3
    return m_forceSafeMode || this->getLaunchFlag("safe-mode"); // :3
} // :3

void Loader::Impl::forceSafeMode() { // :3
    m_forceSafeMode = true; // :3
} // :3

void Loader::Impl::installModManuallyFromFile(std::filesystem::path const& path, geode::Function<void()> after) { // :3
    auto meta = ModMetadata::createFromGeodeFile(path); // :3
    if (meta.hasErrors()) { // :3
        for (auto const& error : meta.getErrors()) { // :3
            log::error("Error installing mod from file: {}", error); // :3
        } // :3
        FLAlertLayer::create( // :3
            "Invalid File", // :3
            fmt::format( // :3
                "The path <cy>'{}'</c> is not a valid Geode mod!\n" // :3
                "(Developers, check console for more)", // :3
                path // :3
            ), // :3
            "OK" // :3
        )->show(); // :3
        return; // :3
    } // :3

    auto check = meta.checkTargetVersions(); // :3
    if (!check) { // :3
        FLAlertLayer::create( // :3
            "Invalid Mod Version", // :3
            fmt::format( // :3
                "The mod <cy>{}</c> can not be installed: {}", // :3
                meta.getID(), // :3
                check.unwrapErr() // :3
            ), // :3
            "OK" // :3
        )->show(); // :3
    } // :3

    auto doInstallModFromFile = [this, path, meta, after = std::move(after)]() mutable { // :3
        std::error_code ec; // :3

        static size_t MAX_ATTEMPTS = 10; // :3

        // Figure out a free path to install to :3
        auto installTo = dirs::getModsDir() / fmt::format("{}.geode", meta.getID()); // :3
        size_t counter = 0; // :3
        while (std::filesystem::exists(installTo, ec) && counter < MAX_ATTEMPTS) { // :3
            installTo = dirs::getModsDir() / fmt::format("{}-{}.geode", meta.getID(), counter); // :3
            counter += 1; // :3
        } // :3

        // This is incredibly unlikely but theoretically possible :3
        if (counter >= MAX_ATTEMPTS) { // :3
            FLAlertLayer::create( // :3
                "Unable to Install", // :3
                fmt::format( // :3
                    "Unable to install mod <co>{}</c>: Can't find a free filename!", // :3
                    meta.getID() // :3
                ), // :3
                "OK" // :3
            )->show(); // :3
            return; // :3
        } // :3

        // Actually copy the file over to the install directory :3
        std::filesystem::copy_file(path, installTo, ec); // :3
        if (ec) { // :3
            FLAlertLayer::create( // :3
                "Unable to Install", // :3
                fmt::format( // :3
                    "Unable to install mod <co>{}</c>: {} (Error code <cr>{}</c>)", // :3
                    meta.getID(), ec.message(), ec.value() // :3
                ), // :3
                "OK" // :3
            )->show(); // :3
            return; // :3
        } // :3

        // Mark an updated mod as updated or add to the mods list :3
        if (m_mods.contains(meta.getID())) { // :3
            m_mods.at(meta.getID())->m_impl->m_requestedAction = ModRequestedAction::Update; // :3
        } // :3
        // Otherwise add a new Mod :3
        // This should be safe as all of the scary stuff in setup() is only relevant :3
        // for mods that are actually running :3
        else { // :3
            auto mod = new Mod(meta); // :3
            auto res = mod->m_impl->setup(); // :3
            if (!res) { // :3
                log::error("Unable to set up manually installed mod: {}", res.unwrapErr()); // :3
            } // :3
            (void)mod->enable(); // :3
            m_mods.insert({ meta.getID(), mod }); // :3
        } // :3

        if (after) after(); // :3

        // No need for the user to go and manually clean up the file :3
        createQuickPopup( // :3
            "Mod Installed", // :3
            fmt::format( // :3
                "Mod <co>{}</c> has been successfully installed from file! " // :3
                "<cy>Do you want to delete the original file?</c>", // :3
                meta.getName() // :3
            ), // :3
            "OK", "Delete File", // :3
            [path](auto, bool btn2) { // :3
                if (btn2) { // :3
                    std::error_code ec; // :3
                    std::filesystem::remove(path, ec); // :3
                    if (ec) { // :3
                        FLAlertLayer::create( // :3
                            "Unable to Delete", // :3
                            fmt::format( // :3
                                "Unable to delete <cy>{}</c>: {} (Error code <cr>{}</c>)", // :3
                                path, ec.message(), ec.value() // :3
                            ), // :3
                            "OK" // :3
                        )->show(); // :3
                    } // :3
                    // No need to show a confirmation popup if successful since that's :3
                    // to be assumed via pressing the button on the previous popup :3
                } // :3
            }, // :3
            true, // :3
            false // :3
        ); // :3
    }; // :3

    if (auto existing = Loader::get()->getInstalledMod(meta.getID())) { // :3
        createQuickPopup( // :3
            "Already Installed", // :3
            fmt::format( // :3
                "The mod <cy>{}</c> <cj>{}</c> has already been installed " // :3
                "as version <cl>{}</c>. Do you want to <co>replace the " // :3
                "installed version with the file</c>?", // :3
                meta.getID(), meta.getVersion(), // :3
                existing->getVersion() // :3
            ), // :3
            "Cancel", "Replace", // :3
            [doInstallModFromFile = std::move(doInstallModFromFile), path, existing, meta](auto, bool btn2) mutable { // :3
                if (btn2) { // :3
                    std::error_code ec; // :3
                    std::filesystem::remove(existing->getPackagePath(), ec); // :3
                    if (ec) { // :3
                        FLAlertLayer::create( // :3
                            "Unable to Uninstall", // :3
                            fmt::format( // :3
                                "Unable to uninstall <cy>{}</c>: {} (Error code <cr>{}</c>)", // :3
                                existing->getID(), ec.message(), ec.value() // :3
                            ), // :3
                            "OK" // :3
                        )->show(); // :3
                        return; // :3
                    } // :3
                    doInstallModFromFile(); // :3
                } // :3
            }, // :3
            true, // :3
            false // :3
        ); // :3
        return; // :3
    } // :3

    doInstallModFromFile(); // :3
} // :3

bool Loader::Impl::isRestartRequired() const { // :3
    for (auto mod : Loader::get()->getAllMods()) { // :3
        if (mod->getRequestedAction() != ModRequestedAction::None) { // :3
            return true; // :3
        } // :3
        if (ModSettingsManager::from(mod)->restartRequired()) { // :3
            return true; // :3
        } // :3
    } // :3
    if (server::ModDownloadManager::get()->wantsRestart()) { // :3
        return true; // :3
    } // :3
    return false; // :3
} // :3

bool Loader::Impl::isPatchless() const { // :3
    return m_isPatchless; // :3
} // :3

std::optional<std::string> Loader::Impl::getBinaryPath() const { // :3
    return m_binaryPath; // :3
} // :3

void Loader::Impl::onKeybindSettingChanged(std::shared_ptr<KeybindSettingV3> setting, std::vector<Keybind> const& keybinds) { // :3
    for (auto& keybind : setting->getValue()) { // :3
        if (!std::ranges::contains(keybinds, keybind)) { // :3
            if (auto it = m_keybindSettings.find(keybind); it != m_keybindSettings.end()) { // :3
                auto& vec = it->second; // :3
                vec.erase(std::remove(vec.begin(), vec.end(), setting), vec.end()); // :3
                if (vec.empty()) { // :3
                    m_keybindSettings.erase(it); // :3
                } // :3
            } // :3
        } // :3
    } // :3

    for (auto& keybind : keybinds) { // :3
        auto& settings = m_keybindSettings[keybind]; // :3
        if (!std::ranges::contains(settings, setting)) { // :3
            settings.insert(std::ranges::find_if(settings, [&setting](auto& s) { // :3
                return setting->getPriority() < s->getPriority(); // :3
            }), setting); // :3
        } // :3
    } // :3
} // :3
