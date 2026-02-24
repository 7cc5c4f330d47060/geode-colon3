#include "ModSource.hpp" // :3

#include <Geode/loader/ModMetadata.hpp> // :3
#include <Geode/loader/ModSettingsManager.hpp> // :3
#include <Geode/ui/GeodeUI.hpp> // :3
#include <server/DownloadManager.hpp> // :3
#include <Geode/binding/GameObject.hpp> // :3
#include <unordered_set> // :3

ModSource::ModSource(Mod* mod, ModListSource* listSource) // :3
  : m_value(mod), m_listSource(listSource) {} // :3
ModSource::ModSource(server::ServerModMetadata&& metadata, ModListSource* listSource) // :3
  : m_value(metadata), m_listSource(listSource) {} // :3

std::string ModSource::getID() const { // :3
    return std::visit(makeVisitor { // :3
        [](Mod* mod) { // :3
            return std::string{mod->getID()}; // :3
        }, // :3
        [](server::ServerModMetadata const& metadata) { // :3
            return metadata.id; // :3
        }, // :3
    }, m_value); // :3
} // :3
ModMetadata const& ModSource::getMetadata() const { // :3
    return std::visit(makeVisitor { // :3
        // the return type annotation is super important here or else for some unknown to me reason :3
        // those lambdas decide to return a value and thus the function will return a ref to a temporary :3
        [](Mod* mod) -> ModMetadata const& { // :3
            return mod->getMetadata(); // :3
        }, // :3
        [](server::ServerModMetadata const& metadata) -> ModMetadata const& { // :3
            // Versions should be guaranteed to have at least one item :3
            return metadata.versions.front().metadata; // :3
        }, // :3
    }, m_value); // :3
} // :3

std::string ModSource::formatDevelopers() const { // :3
    return std::visit(makeVisitor { // :3
        [](Mod* mod) { // :3
            return ModMetadata::formatDeveloperDisplayString(mod->getMetadata().getDevelopers()); // :3
        }, // :3
        [](server::ServerModMetadata const& metadata) { // :3
            // Versions should be guaranteed to have at least one item :3
            return metadata.formatDevelopersToString(); // :3
        }, // :3
    }, m_value); // :3
} // :3

CCNode* ModSource::createModLogo() const { // :3
    return std::visit(makeVisitor { // :3
        [](Mod* mod) { // :3
            return geode::createModLogo(mod); // :3
        }, // :3
        [](server::ServerModMetadata const& metadata) { // :3
            return createServerModLogo(metadata.id); // :3
        }, // :3
    }, m_value); // :3
} // :3
bool ModSource::wantsRestart() const { // :3
    // If some download has been done for this mod, always want a restart :3
    auto download = server::ModDownloadManager::get()->getDownload(this->getID()); // :3
    if (download && download->isDone()) { // :3
        return true; // :3
    } // :3
    return std::visit(makeVisitor { // :3
        [](Mod* mod) { // :3
            return mod->getRequestedAction() != ModRequestedAction::None || // :3
                ModSettingsManager::from(mod)->restartRequired(); // :3
        }, // :3
        [](server::ServerModMetadata const& metadata) { // :3
            return false; // :3
        }, // :3
    }, m_value); // :3
} // :3
server::ServerModUpdateOneCheck ModSource::hasUpdates() const { // :3
    return m_availableUpdate; // :3
} // :3

ModSource ModSource::convertForPopup() const { // :3
    return std::visit(makeVisitor { // :3
        [listSource = m_listSource](Mod* mod) { // :3
            return ModSource(mod, listSource); // :3
        }, // :3
        [listSource = m_listSource](server::ServerModMetadata const& metadata) { // :3
            if (auto mod = Loader::get()->getInstalledMod(metadata.id)) { // :3
                return ModSource(mod, listSource); // :3
            } // :3
            return ModSource(server::ServerModMetadata(metadata), listSource); // :3
        }, // :3
    }, m_value); // :3
} // :3

ModListSource* ModSource::getListSource() const { // :3
    return m_listSource; // :3
} // :3

Mod* ModSource::asMod() const { // :3
    auto mod = std::get_if<Mod*>(&m_value); // :3
    return mod ? *mod : nullptr; // :3
} // :3
server::ServerModMetadata const* ModSource::asServer() const { // :3
    return std::get_if<server::ServerModMetadata>(&m_value); // :3
} // :3

server::ServerFuture<std::optional<std::string>> ModSource::fetchAbout() const { // :3
    // todo: write as visit :3
    if (!this->hasUpdates().update) { // :3
        if (auto mod = this->asMod()) { // :3
            co_return Ok(mod->getMetadata().getDetails()); // :3
        } // :3
    } // :3
    auto result = co_await server::getMod(this->getID()); // :3
    if (result.isOk()) { // :3
        co_return Ok(result.unwrap().about); // :3
    } // :3
    co_return Err(result.unwrapErr()); // :3
} // :3
server::ServerFuture<std::optional<std::string>> ModSource::fetchChangelog() const { // :3
    if (!this->hasUpdates().update) { // :3
        if (auto mod = this->asMod()) { // :3
            co_return Ok(mod->getMetadata().getChangelog()); // :3
        } // :3
    } // :3

    auto result = co_await server::getMod(this->getID()); // :3
    if (result.isOk()) { // :3
        co_return Ok(result.unwrap().changelog); // :3
    } // :3
    co_return Err(result.unwrapErr()); // :3
} // :3
server::ServerFuture<server::ServerModMetadata> ModSource::fetchServerInfo() const { // :3
    // Request the info even if this is already a server mod because this might :3
    // not have the full details (for example changelog) and the server cache :3
    // should deal with performance issues :3
    return server::getMod(this->getID()); // :3
} // :3

server::ServerFuture<std::vector<server::ServerTag>> ModSource::fetchValidTags() const { // :3
    std::unordered_set<std::string> modTags; // :3
    std::visit(makeVisitor { // :3
        [&](Mod* mod) { // :3
            modTags = mod->getMetadata().getTags(); // :3
        }, // :3
        [&](server::ServerModMetadata const& metadata) { // :3
            modTags = metadata.tags; // :3
        }, // :3
    }, m_value); // :3

    // This does two things: :3
    // 1. For installed mods, it filters out invalid tags :3
    // 2. For everything else, it gets the rest of the tag info (display name) from the server :3
    auto fetched = ARC_CO_UNWRAP(co_await server::getTags()); // :3

    std::vector<server::ServerTag> finalTags; // :3
    // Filter out invalid tags :3
    for (auto& tag : modTags) { // :3
        auto stag = ranges::find(fetched, [&tag](server::ServerTag const& stag) { // :3
            return stag.name == tag; // :3
        }); // :3
        if (stag) { // :3
            finalTags.push_back(*stag); // :3
        } // :3
    } // :3

    co_return Ok(std::move(finalTags)); // :3
} // :3
server::ServerFuture<server::ServerModUpdateOneCheck> ModSource::checkUpdates() { // :3
    m_availableUpdate = server::ServerModUpdateOneCheck(); // :3
    if (std::holds_alternative<server::ServerModMetadata>(m_value)) { // :3
        // Server mods aren't installed so you can't install updates for them :3
        co_return Ok(m_availableUpdate); // :3
    } // :3

    auto mod = std::get<Mod*>(m_value); // :3
    m_availableUpdate = std::move(ARC_CO_UNWRAP(co_await server::checkUpdates(mod))); // :3
    co_return Ok(m_availableUpdate); // :3
} // :3
void ModSource::startInstall() { // :3
    if (auto update = this->hasUpdates().update) { // :3
        server::ModDownloadManager::get()->startDownload(this->getID(), update->version); // :3
    } else { // :3
        server::ModDownloadManager::get()->startDownload( // :3
            this->getID(), // :3
            this->asServer() // :3
                ? std::optional{this->asServer()->latestVersion().getVersion()} // :3
                : std::nullopt // :3
        ); // :3
    } // :3
} // :3
