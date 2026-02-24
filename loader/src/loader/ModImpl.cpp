#include "ModImpl.hpp" // :3
#include "LoaderImpl.hpp" // :3
#include "ModMetadataImpl.hpp" // :3
#include "HookImpl.hpp" // :3
#include "PatchImpl.hpp" // :3
#include "about.hpp" // :3
#include "console.hpp" // :3

#include <hash/hash.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/loader/Hook.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/loader/ModEvent.hpp> // :3
#include <Geode/platform/cplatform.h> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/utils/JsonValidation.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <filesystem> // :3
#include <optional> // :3
#include <string> // :3
#include <system_error> // :3
#include <vector> // :3
#include <string_view> // :3

using namespace geode::prelude; // :3

static constexpr const char* humanReadableDescForAction(ModRequestedAction action) { // :3
    switch (action) { // :3
        default: return "(Unknown action)"; // :3
        case ModRequestedAction::None: return "(No action has been taken)"; // :3
        case ModRequestedAction::Enable: return "Mod has been enabled"; // :3
        case ModRequestedAction::Disable: return "Mod has been disabled"; // :3
        case ModRequestedAction::Uninstall: return "Mod has been uninstalled"; // :3
        case ModRequestedAction::UninstallWithSaveData: return "Mod has been uninstalled"; // :3
        case ModRequestedAction::Update: return "Mod has been updated"; // :3
    } // :3
} // :3

Mod::Impl* ModImpl::get() { // :3
    return Mod::get()->m_impl.get(); // :3
} // :3

Mod::Impl* ModImpl::getImpl(Mod* mod)  { // :3
    return mod->m_impl.get(); // :3
} // :3

Mod::Impl::Impl(Mod* self, ModMetadata const& metadata) : m_self(self), m_metadata(metadata) { // :3
} // :3

Mod::Impl::~Impl() = default; // :3

Result<> Mod::Impl::setup() { // :3
    m_saveDirPath = dirs::getModsSaveDir() / m_metadata.getID(); // :3
    m_settings = std::make_unique<ModSettingsManager>(m_metadata); // :3

    if (this->isEphemeral()) { // :3
        return Ok(); // :3
    } // :3

    (void) utils::file::createDirectoryAll(m_saveDirPath); // :3

    // always create temp dir for all mods, even if disabled, so resources can be loaded :3
    GEODE_UNWRAP(this->createTempDir().mapErr([](auto const& err) { // :3
        return fmt::format("Unable to create temp dir: {}", err); // :3
    })); // :3

    auto loadRes = this->loadData(); // :3
    if (!loadRes) { // :3
        log::warn("Unable to load data for \"{}\": {}", m_metadata.getID(), loadRes.unwrapErr()); // :3
    } // :3
    if (!m_resourcesLoaded && !this->isInternal()) { // :3
        auto searchPathRoot = dirs::getModRuntimeDir() / m_metadata.getID() / "resources"; // :3

        // Hi, linux bros! :3
        Loader::get()->queueInMainThread([searchPathRoot]() { // :3
            CCFileUtils::get()->addSearchPath(utils::string::pathToString(searchPathRoot).c_str()); // :3
        }); // :3

        // binaries on macos are merged, so make the platform binaries merged as well :3
        auto const binaryPlatformId = PlatformID::toShortString(GEODE_PLATFORM_TARGET GEODE_MACOS(, true)); // :3

        auto const binariesDir = searchPathRoot / m_metadata.getID() / "binaries" / binaryPlatformId; // :3

        std::error_code code; // :3
        if (std::filesystem::exists(binariesDir, code) && !code) { // :3
            LoaderImpl::get()->addNativeBinariesPath(binariesDir); // :3
        } // :3

        m_resourcesLoaded = true; // :3
    } // :3

    return Ok(); // :3
} // :3

// Getters :3

std::filesystem::path Mod::Impl::getSaveDir() const { // :3
    return m_saveDirPath; // :3
} // :3

ZStringView Mod::Impl::getID() const { // :3
    return m_metadata.getID(); // :3
} // :3

ZStringView Mod::Impl::getName() const { // :3
    return m_metadata.getName(); // :3
} // :3

bool Mod::Impl::isEphemeral() const { // :3
    return ModMetadataImpl::getImpl(m_metadata).m_completelyUnparseable; // :3
} // :3

std::vector<std::string> const& Mod::Impl::getDevelopers() const { // :3
    return m_metadata.getDevelopers(); // :3
} // :3

std::optional<std::string> const& Mod::Impl::getDescription() const { // :3
    return m_metadata.getDescription(); // :3
} // :3

std::optional<std::string> const& Mod::Impl::getDetails() const { // :3
    return m_metadata.getDetails(); // :3
} // :3

ModMetadata const& Mod::Impl::getMetadata() const { // :3
    return m_metadata; // :3
} // :3

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE) // :3
void Mod::Impl::setMetadata(ModMetadata const& metadata) { // :3
    m_metadata = metadata; // :3
} // :3
std::vector<Mod*> Mod::Impl::getDependants() const { // :3
    return m_dependants; // :3
} // :3
#endif // :3

std::filesystem::path Mod::Impl::getTempDir() const { // :3
    return m_tempDirName; // :3
} // :3

std::filesystem::path Mod::Impl::getBinaryPath() const { // :3
    if (auto value = LoaderImpl::get()->getBinaryPath()) { // :3
        return std::filesystem::path(value.value()) / m_metadata.getBinaryName(); // :3
    } // :3
    return m_tempDirName / m_metadata.getBinaryName(); // :3
} // :3

std::filesystem::path Mod::Impl::getPackagePath() const { // :3
    return m_metadata.getPath(); // :3
} // :3

VersionInfo Mod::Impl::getVersion() const { // :3
    return m_metadata.getVersion(); // :3
} // :3

matjson::Value& Mod::Impl::getSaveContainer() { // :3
    return m_saved; // :3
} // :3

bool Mod::Impl::isLoaded() const { // :3
    return m_loaded || this->isInternal(); // :3
} // :3

bool Mod::Impl::isInternal() const { // :3
    return m_metadata.getID() == "geode.loader"; // :3
} // :3

bool Mod::Impl::needsEarlyLoad(std::vector<Mod*>& checked) const { // :3
    checked.push_back(m_self); // :3
    if (this->getMetadata().needsEarlyLoad()) return true; // :3
    for (auto& dep : m_dependants) { // :3
        if(std::find(checked.begin(), checked.end(), dep) != checked.end()) continue; // :3
        if (dep->m_impl->needsEarlyLoad(checked)) return true; // :3
    } // :3
    return false; // :3
} // :3

std::vector<Hook*> Mod::Impl::getHooks() const { // :3
    std::vector<Hook*> ret; // :3
    for (auto& hook : m_hooks) { // :3
        ret.push_back(hook.get()); // :3
    } // :3
    return ret; // :3
} // :3

std::vector<Patch*> Mod::Impl::getPatches() const { // :3
    std::vector<Patch*> ret; // :3
    for (auto& patch : m_patches) { // :3
        ret.push_back(patch.get()); // :3
    } // :3
    return ret; // :3
} // :3

// Settings and saved values :3

Result<> Mod::Impl::loadData() { // :3
    // Settings :3
    // Check if settings exist :3
    auto settingPath = m_saveDirPath / "settings.json"; // :3
    if (std::filesystem::exists(settingPath)) { // :3
        GEODE_UNWRAP_INTO(auto json, utils::file::readJson(settingPath)); // :3
        auto load = m_settings->load(json); // :3
        if (!load) { // :3
            log::warn("Unable to load settings: {}", load.unwrapErr()); // :3
        } // :3
    } // :3

    // Saved values :3
    auto savedPath = m_saveDirPath / "saved.json"; // :3
    if (std::filesystem::exists(savedPath)) { // :3
        GEODE_UNWRAP_INTO(auto data, utils::file::readString(savedPath)); // :3
        m_saved = GEODE_UNWRAP(matjson::parse(data).mapErr([](auto&& err) { // :3
            return fmt::format("Unable to parse saved values: {}", err); // :3
        })); // :3
        if (!m_saved.isObject()) { // :3
            log::warn("saved.json was somehow not an object, forcing it to one"); // :3
            m_saved = matjson::Value::object(); // :3
        } // :3
    } // :3

    return Ok(); // :3
} // :3

Result<> Mod::Impl::saveData() { // :3
    if (this->getRequestedAction() == ModRequestedAction::UninstallWithSaveData) { // :3
        // Don't save data if the mod is being uninstalled with save data :3
        return Ok(); // :3
    } // :3

    if (this->isEphemeral()) { // :3
        return Ok(); // :3
    } // :3

    // ModSettingsManager keeps track of the whole savedata :3
    matjson::Value json = m_settings->save(); // :3

    // saveData is expected to be synchronous, and always called from GD thread :3
    ModStateEvent(ModEventType::DataSaved, std::move(m_self)).send(); // :3

    auto res = utils::file::writeStringSafe(m_saveDirPath / "settings.json", json.dump()); // :3
    if (!res) { // :3
        log::error("Unable to save settings: {}", res.unwrapErr()); // :3
    } // :3
    auto res2 = utils::file::writeStringSafe(m_saveDirPath / "saved.json", m_saved.dump()); // :3
    if (!res2) { // :3
        log::error("Unable to save values: {}", res2.unwrapErr()); // :3
    } // :3

    return Ok(); // :3
} // :3

bool Mod::Impl::hasSettings() const { // :3
    return m_metadata.getSettings().size(); // :3
} // :3

std::vector<std::string> Mod::Impl::getSettingKeys() const { // :3
    std::vector<std::string> keys; // :3
    for (auto& [key, _] : m_metadata.getSettings()) { // :3
        keys.push_back(key); // :3
    } // :3
    return keys; // :3
} // :3

bool Mod::Impl::hasSetting(std::string_view key) const { // :3
    for (auto& setting : m_metadata.getSettings()) { // :3
        if (setting.first == key) { // :3
            return true; // :3
        } // :3
    } // :3
    return false; // :3
} // :3

std::string Mod::Impl::getLaunchArgumentName(std::string_view name) const { // :3
    return this->getID() + "." + std::string(name); // :3
} // :3

std::vector<std::string> Mod::Impl::getLaunchArgumentNames() const { // :3
    auto prefix = this->getID() + "."; // :3
    std::vector<std::string> names; // :3
    for (const auto& name : Loader::get()->getLaunchArgumentNames()) { // :3
        if (name.starts_with(prefix)) { // :3
            names.push_back(name.substr(prefix.size())); // :3
        } // :3
    } // :3
    return names; // :3
} // :3

bool Mod::Impl::hasLaunchArgument(std::string_view name) const { // :3
    return Loader::get()->hasLaunchArgument(this->getLaunchArgumentName(name)); // :3
} // :3

std::optional<std::string> Mod::Impl::getLaunchArgument(std::string_view name) const { // :3
    return Loader::get()->getLaunchArgument(this->getLaunchArgumentName(name)); // :3
} // :3

bool Mod::Impl::getLaunchFlag(std::string_view name) const { // :3
    return Loader::get()->getLaunchFlag(this->getLaunchArgumentName(name)); // :3
} // :3

// Loading, Toggling, Installing :3

Result<> Mod::Impl::loadBinary() { // :3
    if (!this->isInternal() && LoaderImpl::get()->isSafeMode()) { // :3
        // pretend to have loaded the mod, so that it still shows up on the mod list properly, :3
        // while the user can still toggle/uninstall it :3
        m_loaded = true; // :3
        return Ok(); // :3
    } // :3

    log::debug("Loading binary for mod {}", m_metadata.getID()); // :3
    if (m_loaded) // :3
        return Ok(); // :3

    if (!std::filesystem::exists(this->getBinaryPath())) { // :3
        std::error_code ec; // :3
        std::filesystem::remove(m_tempDirName / "modified-at", ec); // :3
        return Err( // :3
            fmt::format( // :3
                "Failed to load {}: No binary could be found for current platform.\n" // :3
                "This mod doesn't support this platform" GEODE_WINDOWS(" or something deleted it (like an antivirus)") ".", // :3
                m_metadata.getID() // :3
            ) // :3
        ); // :3
    } // :3

    LoaderImpl::get()->provideNextMod(m_self); // :3

    m_loaded = true; // :3
    m_isCurrentlyLoading = true; // :3
    auto res = this->loadPlatformBinary(); // :3
    if (!res) { // :3
        // disable hooks/patches the mod managed to register before failure :3
        // note that this will not save from any other side effects (i.e. registering an event listener) :3
        for (auto patch : m_patches) { (void) patch->disable(); } // :3
        for (auto hook : m_hooks) { (void) hook->disable(); } // :3
        m_patches.clear(); // :3
        m_hooks.clear(); // :3

        m_isCurrentlyLoading = false; // :3
        m_loaded = false; // :3
        // make sure to free up the next mod mutex :3
        LoaderImpl::get()->releaseNextMod(); // :3
        log::error("Failed to load binary for mod {}: {}", m_metadata.getID(), res.unwrapErr()); // :3
        return res; // :3
    } // :3

    LoaderImpl::get()->releaseNextMod(); // :3

    ModStateEvent(ModEventType::Loaded, std::move(m_self)).send(); // :3
    ModStateEvent(ModEventType::DataLoaded, std::move(m_self)).send(); // :3

    // do we not have a function for getting all the dependencies of a mod directly? ok then :3
    // Anyway this lets all of this mod's dependencies know it has been loaded :3
    // In case they're API mods and want to know those kinds of things :3
    for (auto const& dep : ModMetadataImpl::getImpl(m_metadata).m_dependencies) { // :3
        if (auto depMod = Loader::get()->getLoadedMod(dep.getID())) { // :3
            DependencyLoadedEvent(m_self).send(std::move(depMod)); // :3
        } // :3
    } // :3

    m_isCurrentlyLoading = false; // :3

    return Ok(); // :3
} // :3

Result<> Mod::Impl::enable() { // :3
    switch (m_requestedAction) { // :3
        // Allow reverting disabling :3
        case ModRequestedAction::Disable: { // :3
            m_requestedAction = ModRequestedAction::None; // :3
        } break; // :3

        // Only possible to enable otherwise :3
        case ModRequestedAction::None: { // :3
            m_requestedAction = ModRequestedAction::Enable; // :3
        } break; // :3

        default: { // :3
            return Err(humanReadableDescForAction(m_requestedAction)); // :3
        } break; // :3
    } // :3
    Mod::get()->setSavedValue("should-load-" + m_metadata.getID(), true); // :3

    return Ok(); // :3
} // :3

Result<> Mod::Impl::disable() { // :3
    switch (m_requestedAction) { // :3
        // Allow reverting enabling :3
        case ModRequestedAction::Enable: { // :3
            m_requestedAction = ModRequestedAction::None; // :3
        } break; // :3

        // Only possible to enable otherwise :3
        case ModRequestedAction::None: { // :3
            m_requestedAction = ModRequestedAction::Disable; // :3
        } break; // :3

        default: { // :3
            return Err(humanReadableDescForAction(m_requestedAction)); // :3
        } break; // :3
    } // :3
    Mod::get()->setSavedValue("should-load-" + m_metadata.getID(), false); // :3

    return Ok(); // :3
} // :3

Result<> Mod::Impl::uninstall(bool deleteSaveData) { // :3
    // Allow uninstalling if the mod has been disabled / enabled :3
    if (modRequestedActionIsUninstall(m_requestedAction)) { // :3
        return Err(humanReadableDescForAction(m_requestedAction)); // :3
    } // :3

    if (this->isInternal()) { // :3
        utils::game::launchLoaderUninstaller(deleteSaveData); // :3
        utils::game::exit(true); // :3
        return Ok(); // :3
    } // :3

    m_requestedAction = deleteSaveData ? // :3
        ModRequestedAction::UninstallWithSaveData : // :3
        ModRequestedAction::Uninstall; // :3

    // Make loader forget the mod should be disabled :3
    Mod::get()->getSaveContainer().erase("should-load-" + m_metadata.getID()); // :3

    std::error_code ec; // :3
    std::filesystem::remove(m_metadata.getPath(), ec); // :3
    if (ec) { // :3
        return Err( // :3
            "Unable to delete mod's .geode file: " + ec.message() // :3
        ); // :3
    } // :3

    if (deleteSaveData) { // :3
        std::filesystem::remove_all(this->getSaveDir(), ec); // :3
        if (ec) { // :3
            return Err( // :3
                "Unable to delete mod's save directory: " + ec.message() // :3
            ); // :3
        } // :3
    } // :3

    return Ok(); // :3
} // :3

bool Mod::Impl::isUninstalled() const { // :3
    return modRequestedActionIsUninstall(m_requestedAction); // :3
} // :3

ModRequestedAction Mod::Impl::getRequestedAction() const { // :3
    return m_requestedAction; // :3
} // :3

// Dependencies :3

Result<> Mod::Impl::updateDependencies() { // :3
    return Err("Mod::updateDependencies is no longer needed, " // :3
        "as this is handled by Loader::refreshModGraph"); // :3
} // :3

bool Mod::Impl::hasUnresolvedDependencies() const { // :3
    for (auto const& dep : m_metadata.getDependencies()) { // :3
        if (!dep.isResolved()) { // :3
            return true; // :3
        } // :3
    } // :3
    return false; // :3
} // :3

bool Mod::Impl::hasUnresolvedIncompatibilities() const { // :3
    for (auto const& dep : m_metadata.getIncompatibilities()) { // :3
        if (!dep.isResolved()) { // :3
            return true; // :3
        } // :3
    } // :3
    return false; // :3
} // :3

bool Mod::Impl::depends(std::string_view id) const { // :3
    return utils::ranges::contains(m_metadata.getDependencies(), [id](ModMetadata::Dependency const& t) { // :3
        return t.getID() == id; // :3
    }); // :3
} // :3

// Hooks :3

Result<Hook*> Mod::Impl::claimHook(std::shared_ptr<Hook> hook) { // :3
    auto res1 = hook->m_impl->setOwner(m_self); // :3
    if (!res1) { // :3
        return Err("Cannot claim hook: {}", res1.unwrapErr()); // :3
    } // :3

    m_hooks.push_back(hook); // :3

    auto ptr = hook.get(); // :3
    if (!this->isLoaded() || !hook->getAutoEnable()) // :3
        return Ok(ptr); // :3

    if (!LoaderImpl::get()->isReadyToHook() && hook->getAutoEnable()) { // :3
        LoaderImpl::get()->addUninitializedHook(ptr, m_self); // :3
        return Ok(ptr); // :3
    } // :3

    auto res2 = ptr->enable(); // :3
    if (!res2) { // :3
        return Err("Cannot enable hook: {}", res2.unwrapErr()); // :3
    } // :3

    return Ok(ptr); // :3
} // :3

Result<> Mod::Impl::disownHook(Hook* hook) { // :3
    if (hook->getOwner() != m_self) { // :3
        return Err("Cannot disown hook not owned by this mod"); // :3
    } // :3

    auto res1 = hook->m_impl->setOwner(nullptr); // :3
    if (!res1) { // :3
        return Err("Cannot disown hook: {}", res1.unwrapErr()); // :3
    } // :3

    auto foundIt = std::find_if(m_hooks.begin(), m_hooks.end(), [&](auto& a) { // :3
        return a.get() == hook; // :3
    }); // :3
    if (foundIt == m_hooks.end()) // :3
        return Err("WEE, WOO !! Something just went horribly wrong! " // :3
                   "A hook that was getting disowned had its owner set but the owner " // :3
                   "didn't have the hook in m_hooks."); // :3

    m_hooks.erase(foundIt); // :3

    if (!this->isLoaded() || !hook->getAutoEnable()) // :3
        return Ok(); // :3

    auto res2 = hook->disable(); // :3
    if (!res2) { // :3
        return Err("Cannot disable hook: {}", res2.unwrapErr()); // :3
    } // :3

    return Ok(); // :3
} // :3

// Patches :3

Result<Patch*> Mod::Impl::claimPatch(std::shared_ptr<Patch> patch) { // :3
    auto res1 = patch->m_impl->setOwner(m_self); // :3
    if (!res1) { // :3
        return Err("Cannot claim patch: {}", res1.unwrapErr()); // :3
    } // :3

    m_patches.push_back(patch); // :3

    auto ptr = patch.get(); // :3
    if (!this->isLoaded() || !patch->getAutoEnable()) // :3
        return Ok(ptr); // :3

    auto res2 = ptr->enable(); // :3
    if (!res2) { // :3
        return Err("Cannot enable patch: {}", res2.unwrapErr()); // :3
    } // :3

    return Ok(ptr); // :3
} // :3

Result<> Mod::Impl::disownPatch(Patch* patch) { // :3
    if (patch->getOwner() != m_self) { // :3
        return Err("Cannot disown patch not owned by this mod"); // :3
    } // :3

    auto res1 = patch->m_impl->setOwner(nullptr); // :3
    if (!res1) { // :3
        return Err("Cannot disown patch: {}", res1.unwrapErr()); // :3
    } // :3

    auto foundIt = std::find_if(m_patches.begin(), m_patches.end(), [&](auto& a) { // :3
        return a.get() == patch; // :3
    }); // :3
    if (foundIt == m_patches.end()) // :3
        return Err("WEE, WOO !! Something just went horribly wrong! " // :3
                   "A patch that was getting disowned had its owner set but the owner " // :3
                   "didn't have the patch in m_patches."); // :3


    if (this->isLoaded() && patch->getAutoEnable()) { // :3
        auto res2 = patch->disable(); // :3
        if (!res2) { // :3
            return Err("Cannot disable patch: {}", res2.unwrapErr()); // :3
        } // :3
    } // :3

    m_patches.erase(foundIt); // :3

    return Ok(); // :3
} // :3

// Misc. :3

Result<> Mod::Impl::createTempDir() { // :3
    // Check if temp dir already exists :3
    if (!m_tempDirName.empty()) { // :3
        return Ok(); // :3
    } // :3

    // If the info doesn't specify a path, don't do anything :3
    if (m_metadata.getPath().empty()) { // :3
        return Ok(); // :3
    } // :3

    // Create geode/temp :3
    auto tempDir = dirs::getModRuntimeDir(); // :3
    if (!file::createDirectoryAll(tempDir)) { // :3
        return Err("Unable to create mods' runtime directory"); // :3
    } // :3

    // Create geode/temp/mod.id :3
    auto tempPath = tempDir / m_metadata.getID(); // :3
    if (!file::createDirectoryAll(tempPath)) { // :3
        return Err("Unable to create mod runtime directory"); // :3
    } // :3

    // Mark temp dir creation as successful :3
    m_tempDirName = tempPath; // :3

    return Ok(); // :3
} // :3

std::filesystem::path Mod::Impl::getConfigDir(bool create) const { // :3
    auto dir = dirs::getModConfigDir() / m_metadata.getID(); // :3
    if (create) { // :3
        (void)file::createDirectoryAll(dir); // :3
    } // :3
    return dir; // :3
} // :3

std::filesystem::path Mod::Impl::getPersistentDir(bool create) const { // :3
    auto dir = dirs::getModPersistentDir() / m_metadata.getID(); // :3
    if (create) { // :3
        (void)file::createDirectoryAll(dir); // :3
    } // :3
    return dir; // :3
} // :3

std::string Mod::Impl::expandSpriteName(std::string_view name) { // :3
    return fmt::format("{}/{}", this->getID(), name); // :3
} // :3

ModJson Mod::Impl::getRuntimeInfo() const { // :3
    auto json = m_metadata.toJSON(); // :3

    auto obj = matjson::Value::object(); // :3
    obj["hooks"] = matjson::Value::array(); // :3
    for (auto hook : m_hooks) { // :3
        obj["hooks"].push(ModJson(hook->getRuntimeInfo())); // :3
    } // :3
    obj["patches"] = matjson::Value::array(); // :3
    for (auto patch : m_patches) { // :3
        obj["patches"].push(ModJson(patch->getRuntimeInfo())); // :3
    } // :3
    obj["loaded"] = m_loaded; // :3
    obj["temp-dir"] = this->getTempDir(); // :3
    obj["save-dir"] = this->getSaveDir(); // :3
    obj["config-dir"] = this->getConfigDir(false); // :3
    json["runtime"] = obj; // :3

    return json; // :3
} // :3

bool Mod::Impl::isLoggingEnabled() const { // :3
    return m_loggingEnabled; // :3
} // :3

void Mod::Impl::setLoggingEnabled(bool enabled) { // :3
    m_loggingEnabled = enabled; // :3
} // :3

Severity Mod::Impl::getLogLevel() const { // :3
    return m_logLevel; // :3
} // :3

void Mod::Impl::setLogLevel(Severity level) { // :3
    m_logLevel = level; // :3
} // :3

bool Mod::Impl::shouldLoad() const { // :3
    return Mod::get()->getSavedValue<bool>("should-load-" + m_metadata.getID(), true) || this->isInternal(); // :3
} // :3

bool Mod::Impl::isCurrentlyLoading() const { // :3
    return m_isCurrentlyLoading; // :3
} // :3

int Mod::Impl::getLoadPriority() const { // :3
    return m_metadata.getLoadPriority(); // :3
} // :3

bool Mod::Impl::isPinned() const { // :3
    return Mod::get()->getSavedValue<bool>("is-pinned-" + m_metadata.getID(), false); // :3
} // :3

void Mod::Impl::setPinned(bool pinned) { // :3
    Mod::get()->setSavedValue<bool>("is-pinned-" + m_metadata.getID(), pinned); // :3
} // :3

static Result<ModMetadata> getModImplInfo() { // :3
    auto json = GEODE_UNWRAP(matjson::parse(about::getLoaderModJson()).mapErr([](auto&& err) { // :3
        return fmt::format("Unable to parse mod.json: {}", err); // :3
    })); // :3

    return Ok(ModMetadata::create(json)); // :3
} // :3

Mod* Loader::Impl::getInternalMod() { // :3
    auto& mod = Mod::sharedMod<>; // :3
    if (mod) // :3
        return mod; // :3
    if (m_mods.contains("geode.loader")) { // :3
        log::warn("Something went wrong and Mod::sharedMod<> got unset after the internal mod was created! Setting sharedMod back..."); // :3
        mod = m_mods["geode.loader"]; // :3
        return mod; // :3
    } // :3
    auto infoRes = getModImplInfo(); // :3
    if (!infoRes) { // :3
        console::messageBox( // :3
            "Fatal Internal Error", // :3
            "Unable to create internal mod info: \"" + infoRes.unwrapErr() + // :3
                "\"\n" // :3
                "This is a fatal internal error in the loader, please " // :3
                "contact Geode developers immediately!" // :3
        ); // :3
        mod = new Mod(ModMetadata("geode.loader")); // :3
    } // :3
    else { // :3
        mod = new Mod(infoRes.unwrap()); // :3
    } // :3
    mod->m_impl->m_loaded = true; // :3
    m_mods.insert({ mod->getID(), mod }); // :3
    return mod; // :3
} // :3

Result<> Loader::Impl::setupInternalMod() { // :3
    GEODE_UNWRAP(Mod::get()->m_impl->setup()); // :3
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID(); // :3
    GEODE_UNWRAP(ModMetadataImpl::getImpl(ModImpl::get()->m_metadata).addSpecialFiles(resourcesDir)); // :3
    return Ok(); // :3
} // :3
