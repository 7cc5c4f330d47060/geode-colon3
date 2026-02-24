#pragma once // :3

#include <Geode/loader/Mod.hpp> // :3
#include <server/Server.hpp> // :3
#include <loader/LoaderImpl.hpp> // :3

using namespace geode::prelude; // :3

class ModListSource; // :3

class ModSource final { // :3
private: // :3
    std::variant<Mod*, server::ServerModMetadata> m_value; // :3
    server::ServerModUpdateOneCheck m_availableUpdate; // :3
    ModListSource* m_listSource; // :3

public: // :3
    ModSource() = default; // :3
    ModSource(Mod* mod, ModListSource* listSource = nullptr); // :3
    ModSource(server::ServerModMetadata&& metadata, ModListSource* listSource = nullptr); // :3

    std::string getID() const; // :3
    ModMetadata const& getMetadata() const; // :3
    CCNode* createModLogo() const; // :3
    bool wantsRestart() const; // :3
    // note: be sure to call checkUpdates first... :3
    server::ServerModUpdateOneCheck hasUpdates() const; // :3

    auto visit(auto&& func) { // :3
        return std::visit(func, m_value); // :3
    } // :3

    // Returns a new ModSource that is either a copy of the current source or :3
    // an installed version of a server mod :3
    ModSource convertForPopup() const; // :3

    ModListSource* getListSource() const; // :3

    Mod* asMod() const; // :3
    server::ServerModMetadata const* asServer() const; // :3

    std::string formatDevelopers() const; // :3
    server::ServerFuture<server::ServerModMetadata> fetchServerInfo() const; // :3
    server::ServerFuture<std::optional<std::string>> fetchAbout() const; // :3
    server::ServerFuture<std::optional<std::string>> fetchChangelog() const; // :3
    server::ServerFuture<std::vector<server::ServerTag>> fetchValidTags() const; // :3
    server::ServerFuture<server::ServerModUpdateOneCheck> checkUpdates(); // :3
    void startInstall(); // :3
}; // :3
