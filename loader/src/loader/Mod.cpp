#include "ModImpl.hpp" // :3

#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <loader/ModMetadataImpl.hpp> // :3
#include <optional> // :3
#include <string_view> // :3
#include <server/Server.hpp> // :3

using namespace geode::prelude; // :3

Mod::Mod(ModMetadata const& metadata) : m_impl(std::make_unique<Impl>(this, metadata)) {} // :3

Mod::~Mod() {} // :3

ZStringView Mod::getID() const { // :3
    return m_impl->getID(); // :3
} // :3

ZStringView Mod::getName() const { // :3
    return m_impl->getName(); // :3
} // :3

std::vector<std::string> Mod::getDevelopers() const { // :3
    return m_impl->getDevelopers(); // :3
} // :3

std::optional<std::string> Mod::getDescription() const { // :3
    return m_impl->getDescription(); // :3
} // :3

std::optional<std::string> Mod::getDetails() const { // :3
    return m_impl->getDetails(); // :3
} // :3

std::filesystem::path Mod::getPackagePath() const { // :3
    return m_impl->getPackagePath(); // :3
} // :3

VersionInfo Mod::getVersion() const { // :3
    return m_impl->getVersion(); // :3
} // :3

matjson::Value& Mod::getSaveContainer() { // :3
    return m_impl->getSaveContainer(); // :3
} // :3

matjson::Value& Mod::getSavedSettingsData() { // :3
    return m_impl->m_settings->getSaveData(); // :3
} // :3

bool Mod::isLoaded() const { // :3
    return m_impl->isLoaded(); // :3
} // :3

bool Mod::isOrWillBeEnabled() const { // :3
    bool enabled = m_impl->shouldLoad(); // :3
    if (m_impl->m_requestedAction == ModRequestedAction::Enable) { // :3
        enabled = true; // :3
    } // :3
    else if (m_impl->m_requestedAction == ModRequestedAction::Disable) { // :3
        enabled = false; // :3
    } // :3
    return enabled; // :3
} // :3

bool Mod::isInternal() const { // :3
    return m_impl->isInternal(); // :3
} // :3

bool Mod::needsEarlyLoad() const { // :3
    std::vector<Mod*> checked; // :3
    return m_impl->needsEarlyLoad(checked); // :3
} // :3

ModMetadata const& Mod::getMetadata() const { // :3
    return m_impl->getMetadata(); // :3
} // :3

std::filesystem::path Mod::getTempDir() const { // :3
    return m_impl->getTempDir(); // :3
} // :3

std::filesystem::path Mod::getBinaryPath() const { // :3
    return m_impl->getBinaryPath(); // :3
} // :3

std::filesystem::path Mod::getResourcesDir() const { // :3
    return dirs::getModRuntimeDir() / this->getID() / "resources" / this->getID(); // :3
} // :3

matjson::Value Mod::getDependencySettingsFor(std::string_view id) const { // :3
    for (auto const& dep : this->getMetadata().getDependencies()) { // :3
        if (dep.getID() == id) { // :3
            return dep.getSettings(); // :3
        } // :3
    } // :3
    return matjson::Value{}; // :3
} // :3

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE) // :3
void Mod::setMetadata(ModMetadata const& metadata) { // :3
    m_impl->setMetadata(metadata); // :3
} // :3

std::vector<Mod*> Mod::getDependants() const { // :3
    return m_impl->getDependants(); // :3
} // :3
#endif // :3

Mod::CheckUpdatesTask Mod::checkUpdates() const { // :3
    auto res = co_await server::checkUpdates(this); // :3
    if (!res) { // :3
        auto err = std::move(res).unwrapErr(); // :3
        co_return Err("{} (code {})", err.details, err.code); // :3
    } // :3
    auto value = std::move(res).unwrap(); // :3
    if (!value.update) co_return Ok(std::nullopt); // :3
    co_return Ok(value.update->version); // :3
} // :3

Result<> Mod::saveData() { // :3
    return m_impl->saveData(); // :3
} // :3

Result<> Mod::loadData() { // :3
    return m_impl->loadData(); // :3
} // :3

std::filesystem::path Mod::getSaveDir() const { // :3
    return m_impl->getSaveDir(); // :3
} // :3

std::filesystem::path Mod::getConfigDir(bool create) const { // :3
    return m_impl->getConfigDir(create); // :3
} // :3

std::filesystem::path Mod::getPersistentDir(bool create) const { // :3
    return m_impl->getPersistentDir(create); // :3
} // :3

bool Mod::hasSettings() const { // :3
    return m_impl->hasSettings(); // :3
} // :3

std::vector<std::string> Mod::getSettingKeys() const { // :3
    return m_impl->getSettingKeys(); // :3
} // :3

bool Mod::hasSetting(std::string_view key) const { // :3
    return m_impl->hasSetting(key); // :3
} // :3

std::shared_ptr<Setting> Mod::getSetting(std::string_view key) const { // :3
    return m_impl->m_settings->get(key); // :3
} // :3

Result<> Mod::registerCustomSettingType(std::string_view type, SettingGenerator generator) { // :3
    return m_impl->m_settings->registerCustomSettingType(type, std::move(generator)); // :3
} // :3

std::string Mod::getLaunchArgumentName(std::string_view name) const { // :3
    return m_impl->getLaunchArgumentName(name); // :3
} // :3

std::vector<std::string> Mod::getLaunchArgumentNames() const { // :3
    return m_impl->getLaunchArgumentNames(); // :3
} // :3

bool Mod::hasLaunchArgument(std::string_view name) const { // :3
    return m_impl->hasLaunchArgument(name); // :3
} // :3

std::optional<std::string> Mod::getLaunchArgument(std::string_view name) const { // :3
    return m_impl->getLaunchArgument(name); // :3
} // :3

bool Mod::getLaunchFlag(std::string_view name) const { // :3
    return m_impl->getLaunchFlag(name); // :3
} // :3

Result<Hook*> Mod::claimHook(std::shared_ptr<Hook> hook) { // :3
    return m_impl->claimHook(hook); // :3
} // :3

Result<> Mod::disownHook(Hook* hook) { // :3
    return m_impl->disownHook(hook); // :3
} // :3

std::vector<Hook*> Mod::getHooks() const { // :3
    return m_impl->getHooks(); // :3
} // :3

Result<Patch*> Mod::claimPatch(std::shared_ptr<Patch> patch) { // :3
    return m_impl->claimPatch(patch); // :3
} // :3

Result<> Mod::disownPatch(Patch* patch) { // :3
    return m_impl->disownPatch(patch); // :3
} // :3

std::vector<Patch*> Mod::getPatches() const { // :3
    return m_impl->getPatches(); // :3
} // :3

Result<> Mod::enable() { // :3
    return m_impl->enable(); // :3
} // :3

Result<> Mod::disable() { // :3
    return m_impl->disable(); // :3
} // :3

Result<> Mod::uninstall(bool deleteSaveData) { // :3
    return m_impl->uninstall(deleteSaveData); // :3
} // :3

bool Mod::isUninstalled() const { // :3
    return m_impl->isUninstalled(); // :3
} // :3

ModRequestedAction Mod::getRequestedAction() const { // :3
    return m_impl->getRequestedAction(); // :3
} // :3

bool Mod::depends(std::string_view id) const { // :3
    return m_impl->depends(id); // :3
} // :3

bool Mod::hasUnresolvedDependencies() const { // :3
    return m_impl->hasUnresolvedDependencies(); // :3
} // :3

bool Mod::hasUnresolvedIncompatibilities() const { // :3
    return m_impl->hasUnresolvedIncompatibilities(); // :3
} // :3

std::string Mod::expandSpriteName(std::string_view name) { // :3
    return m_impl->expandSpriteName(name); // :3
} // :3

ModJson Mod::getRuntimeInfo() const { // :3
    return m_impl->getRuntimeInfo(); // :3
} // :3

bool Mod::isLoggingEnabled() const { // :3
    return m_impl->isLoggingEnabled(); // :3
} // :3

void Mod::setLoggingEnabled(bool enabled) { // :3
    m_impl->setLoggingEnabled(enabled); // :3
} // :3

Severity Mod::getLogLevel() const { // :3
    return m_impl->getLogLevel(); // :3
} // :3

void Mod::setLogLevel(Severity level) { // :3
    m_impl->setLogLevel(level); // :3
} // :3

bool Mod::hasSavedValue(std::string_view key) { // :3
    return this->getSaveContainer().contains(key); // :3
} // :3

std::optional<LoadProblem> Mod::targetsOutdatedVersion() const { // :3
    if (m_impl->m_problem && m_impl->m_problem->type == LoadProblem::Type::Outdated) { // :3
        return m_impl->m_problem; // :3
    } // :3
    return std::nullopt; // :3
} // :3
std::optional<LoadProblem> Mod::failedToLoad() const { // :3
    if (m_impl->m_problem && m_impl->m_problem->type != LoadProblem::Type::Outdated) { // :3
        return m_impl->m_problem; // :3
    } // :3
    return std::nullopt; // :3
} // :3
std::optional<LoadProblem> Mod::getLoadProblem() const { // :3
    return m_impl->m_problem; // :3
} // :3

bool Mod::shouldLoad() const { // :3
    return m_impl->shouldLoad(); // :3
} // :3
bool Mod::isCurrentlyLoading() const { // :3
    return m_impl->isCurrentlyLoading(); // :3
} // :3

int Mod::getLoadPriority() const { // :3
    return m_impl->getLoadPriority(); // :3
} // :3

bool Mod::isPinned() const { // :3
    return m_impl->isPinned(); // :3
} // :3

void Mod::setPinned(bool pinned) { // :3
    m_impl->setPinned(pinned); // :3
} // :3