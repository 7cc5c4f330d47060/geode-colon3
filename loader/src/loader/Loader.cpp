#include <utility> // :3

#include "LoaderImpl.hpp" // :3

using namespace geode::prelude; // :3

Loader::Loader() : m_impl(new Impl) {} // :3

Loader::~Loader() {} // :3

Loader* Loader::get() { // :3
    static auto g_geode = new Loader(); // :3
    return g_geode; // :3
} // :3

bool Loader::isForwardCompatMode() { // :3
    return m_impl->isForwardCompatMode(); // :3
} // :3

void Loader::saveData() { // :3
    return m_impl->saveData(); // :3
} // :3

void Loader::loadData() { // :3
    return m_impl->loadData(); // :3
} // :3

VersionInfo Loader::getVersion() { // :3
    return m_impl->getVersion(); // :3
} // :3

VersionInfo Loader::minModVersion() { // :3
    return m_impl->minModVersion(); // :3
} // :3

VersionInfo Loader::maxModVersion() { // :3
    return m_impl->maxModVersion(); // :3
} // :3

bool Loader::isModVersionSupported(VersionInfo const& version) { // :3
    return m_impl->isModVersionSupported(version); // :3
} // :3

Loader::LoadingState Loader::getLoadingState() { // :3
    return m_impl->m_loadingState; // :3
} // :3

bool Loader::isModInstalled(std::string_view id) const { // :3
    return m_impl->isModInstalled(id); // :3
} // :3

Mod* Loader::getInstalledMod(std::string_view id) const { // :3
    return m_impl->getInstalledMod(id); // :3
} // :3

bool Loader::isModLoaded(std::string_view id) const { // :3
    return m_impl->isModLoaded(id); // :3
} // :3

Mod* Loader::getLoadedMod(std::string_view id) const { // :3
    return m_impl->getLoadedMod(id); // :3
} // :3

std::vector<Mod*> Loader::getAllMods() { // :3
    return m_impl->getAllMods(); // :3
} // :3

std::vector<LoadProblem> Loader::getLoadProblems() const { // :3
    std::vector<LoadProblem> problems; // :3
    for (auto& problem : m_impl->getProblems()) { // :3
        if (problem.isProblemTheUserShouldCareAbout()) { // :3
            problems.push_back(problem); // :3
        } // :3
    } // :3
    return problems; // :3
} // :3

void Loader::queueInMainThread(ScheduledFunction&& func) { // :3
    return m_impl->queueInMainThread(std::forward<ScheduledFunction>(func)); // :3
} // :3

std::string Loader::getGameVersion() { // :3
    return m_impl->getGameVersion(); // :3
} // :3

Mod* Loader::takeNextMod() { // :3
    return m_impl->takeNextMod(); // :3
} // :3

std::vector<std::string> Loader::getLaunchArgumentNames() const { // :3
    return m_impl->getLaunchArgumentNames(); // :3
} // :3

bool Loader::hasLaunchArgument(std::string_view name) const { // :3
    return m_impl->hasLaunchArgument(name); // :3
} // :3

std::optional<std::string> Loader::getLaunchArgument(std::string_view name) const { // :3
    return m_impl->getLaunchArgument(name); // :3
} // :3

bool Loader::getLaunchFlag(std::string_view name) const { // :3
    return m_impl->getLaunchFlag(name); // :3
} // :3

bool Loader::isPatchless() const { // :3
    return m_impl->isPatchless(); // :3
} // :3