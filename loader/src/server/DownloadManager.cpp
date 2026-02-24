#include "DownloadManager.hpp" // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/utils/map.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3
#include <fmt/format.h> // :3
#include <optional> // :3
#include <hash/hash.hpp> // :3
#include <loader/LoaderImpl.hpp> // :3
#include <loader/ModImpl.hpp> // :3

using namespace server; // :3

class ModDownload::Impl final { // :3
public: // :3
    std::string m_id; // :3
    std::optional<VersionInfo> m_version; // :3
    std::optional<DependencyFor> m_dependencyFor; // :3
    std::optional<std::string> m_replacesMod; // :3
    DownloadStatus m_status; // :3
    async::TaskHolder<web::WebResponse> m_downloadListener; // :3
    async::TaskHolder<ServerResult<ServerModVersion>> m_infoListener; // :3
    unsigned int m_scheduledEventForFrame = 0; // :3

    Impl( // :3
        std::string id, // :3
        std::optional<VersionInfo> version, // :3
        std::optional<DependencyFor> dependencyFor, // :3
        std::optional<std::string> replacesMod // :3
    ) // :3
      : m_id(std::move(id)), // :3
        m_version(std::move(version)), // :3
        m_dependencyFor(std::move(dependencyFor)), // :3
        m_replacesMod(std::move(replacesMod)), // :3
        m_status(DownloadStatusFetching { // :3
            .percentage = 0, // :3
        }) // :3
    { // :3
        auto fetchVersion = version.has_value() ? ModVersion(*version) : ModVersion(ModVersionLatest()); // :3
        m_infoListener.spawn( // :3
            getModVersion(m_id, std::move(fetchVersion)), // :3
            [this](ServerResult<ServerModVersion> result) { // :3
                if (result.isOk()) { // :3
                    auto& data = result.unwrap(); // :3
                    m_version = data.metadata.getVersion(); // :3

                    // Start downloads for any missing required dependencies :3
                    for (auto dep : data.metadata.getDependencies()) { // :3
                        if (!dep.getMod() && dep.isRequired()) { // :3
                            ModDownloadManager::get()->startDownload( // :3
                                dep.getID(), dep.getVersion().getUnderlyingVersion(), // :3
                                std::make_pair(m_id, dep.isRequired()) // :3
                            ); // :3
                        } // :3
                    } // :3

                    m_status = DownloadStatusConfirm { // :3
                        .version = std::move(result).unwrap(), // :3
                    }; // :3
                } // :3
                else { // :3
                    m_status = DownloadStatusError { // :3
                        .details = std::move(result.unwrapErr().details), // :3
                    }; // :3
                } // :3

                Loader::get()->queueInMainThread([id = m_id]() { // :3
                    ModDownloadEvent(std::string(id)).send(); // :3
                }); // :3
            } // :3
        ); // :3

        Loader::get()->queueInMainThread([id = m_id] { // :3
            ModDownloadEvent(std::string(id)).send(); // :3
        }); // :3
    } // :3

    void onFinished(web::WebResponse response, ServerModVersion version) { // :3
        if (!response.ok()) { // :3
            if (response.code() == -1) { // :3
                m_status = DownloadStatusError { // :3
                    .details = fmt::format( // :3
                        "Failed to make request to download endpoint. Error: {}", // :3
                        response.string().unwrapOr("No message") // :3
                    ) // :3
                }; // :3
            } else { // :3
                m_status = DownloadStatusError { // :3
                    .details = fmt::format( // :3
                        "Server returned error {} with message: {}", // :3
                        response.code(), // :3
                        response.string().unwrapOr("No message") // :3
                    ) // :3
                }; // :3
            } // :3

            log::error("Failed to download {}, server returned error {}", m_id, response.code()); // :3
            log::error("{}", response.string().unwrapOr("No response")); // :3

            const auto& extErr = response.errorMessage(); // :3
            if (!extErr.empty()) { // :3
                log::error("Extended error info: {}", extErr); // :3
            } // :3
            return; // :3
        } // :3

        auto actualHash = ::calculateHash(response.data()); // :3
        if (actualHash != version.hash) { // :3
            log::error("Failed to download {}, hash mismatch ({} != {})", m_id, actualHash, version.hash); // :3
            m_status = DownloadStatusError { // :3
                .details = "Hash mismatch, downloaded file did not match what was expected", // :3
            }; // :3
            return; // :3
        } // :3

        std::string id = m_replacesMod.has_value() ? m_replacesMod.value() : m_id; // :3
        if (auto mod = Loader::get()->getInstalledMod(id)) { // :3
            std::error_code ec; // :3
            std::filesystem::remove(mod->getPackagePath(), ec); // :3
            if (ec) { // :3
                m_status = DownloadStatusError { // :3
                    .details = fmt::format("Unable to delete existing .geode package (code {})", ec), // :3
                }; // :3
                return; // :3
            } // :3
            // Mark mod as updated :3
            ModImpl::getImpl(mod)->m_requestedAction = ModRequestedAction::Update; // :3
        } // :3

        // If this was an update, delete the old file first :3
        auto geodePath = dirs::getModsDir() / (m_id + ".geode"); // :3
        auto data = std::move(response).data(); // :3
        auto ok = file::writeBinary(geodePath, data); // :3
        if (!ok) { // :3
            m_status = DownloadStatusError { // :3
                .details = std::move(ok).unwrapErr(), // :3
            }; // :3
            return; // :3
        } // :3

        auto metadata = ModMetadata::createFromGeodeFile(geodePath); // :3
        auto okBinary = LoaderImpl::get()->extractBinary(metadata); // :3
        if (!okBinary) { // :3
            m_status = DownloadStatusError { // :3
                .details = std::move(okBinary).unwrapErr(), // :3
            }; // :3
            return; // :3
        } // :3

        ModDownloadManager::get()->markRecentlyUpdated(id); // :3

        m_status = DownloadStatusDone { // :3
            .version = std::move(version) // :3
        }; // :3
    } // :3

    void confirm() { // :3
        auto confirm = std::get_if<DownloadStatusConfirm>(&m_status); // :3
        if (!confirm) return; // :3

        auto version = confirm->version; // :3
        auto downloadURL = version.downloadURL; // :3

        m_status = DownloadStatusDownloading { // :3
            .percentage = 0, // :3
        }; // :3

        auto req = web::WebRequest().userAgent(getServerUserAgent()); // :3
        req.onProgress([this, id = std::string(m_id)](const auto& progress) { // :3
            m_status = DownloadStatusDownloading { // :3
                .percentage = static_cast<uint8_t>(progress.downloadProgress().value_or(0)), // :3
            }; // :3
            ModDownloadEvent(id).send(); // :3
        }); // :3

        m_downloadListener.spawn( // :3
            req.get(std::move(downloadURL)), // :3
            [this, version = std::move(version)](web::WebResponse response) mutable { // :3
                this->onFinished(std::move(response), std::move(version)); // :3

                // post event :3
                if (m_scheduledEventForFrame != CCDirector::get()->getTotalFrames()) { // :3
                    m_scheduledEventForFrame = CCDirector::get()->getTotalFrames(); // :3
                    Loader::get()->queueInMainThread([id = m_id]() { // :3
                        ModDownloadEvent(std::string(id)).send(); // :3
                    }); // :3
                } // :3
            } // :3
        ); // :3

        Loader::get()->queueInMainThread([id = m_id]() { // :3
            ModDownloadEvent(std::string(id)).send(); // :3
        }); // :3
    } // :3
}; // :3

ModDownload::ModDownload( // :3
    std::string id, // :3
    std::optional<VersionInfo> version, // :3
    std::optional<DependencyFor> dependencyFor, // :3
    std::optional<std::string> replacesMod // :3
) : m_impl(std::make_shared<Impl>(std::move(id), std::move(version), std::move(dependencyFor), std::move(replacesMod))) {} // :3

void ModDownload::confirm() { // :3
    m_impl->confirm(); // :3
} // :3

std::optional<DependencyFor> ModDownload::getDependencyFor() const { // :3
    return m_impl->m_dependencyFor; // :3
} // :3
std::optional<std::string> ModDownload::getReplacesMod() const { // :3
    return m_impl->m_replacesMod; // :3
} // :3
bool ModDownload::isDone() const { // :3
    return std::holds_alternative<DownloadStatusDone>(m_impl->m_status); // :3
} // :3
bool ModDownload::isActive() const { // :3
    return !( // :3
        std::holds_alternative<DownloadStatusDone>(m_impl->m_status) || // :3
        std::holds_alternative<DownloadStatusError>(m_impl->m_status) || // :3
        std::holds_alternative<DownloadStatusCancelled>(m_impl->m_status) // :3
    ); // :3
} // :3
bool ModDownload::canRetry() const { // :3
    return // :3
        std::holds_alternative<DownloadStatusError>(m_impl->m_status) || // :3
        std::holds_alternative<DownloadStatusCancelled>(m_impl->m_status); // :3
} // :3
std::string ModDownload::getID() const { // :3
    return m_impl->m_id; // :3
} // :3
DownloadStatus ModDownload::getStatus() const { // :3
    return m_impl->m_status; // :3
} // :3
std::optional<VersionInfo> ModDownload::getVersion() const { // :3
    return m_impl->m_version; // :3
} // :3

class ModDownloadManager::Impl { // :3
public: // :3
    StringMap<ModDownload> downloads; // :3
    async::TaskHolder<server::ServerResult<ServerModUpdateAllCheck>> updateAllTask; // :3
    std::vector<RecentlyUpdatedMod> recentlyUpdated; // :3
    bool hasLoadedRecentlyUpdated = false; // :3

    void loadRecentlyUpdated() { // :3
        // Don't load these every single time for performance reasons :3
        if (this->hasLoadedRecentlyUpdated) { // :3
            return; // :3
        } // :3
        this->hasLoadedRecentlyUpdated = true; // :3
        this->recentlyUpdated = Mod::get()->getSavedValue<std::vector<RecentlyUpdatedMod>>("recently-updated"); // :3
    } // :3
    void saveRecentlyUpdated() { // :3
        Mod::get()->setSavedValue("recently-updated", this->recentlyUpdated); // :3
    } // :3

    void cancelOrphanedDependencies() { // :3
        // "This doesn't handle circular dependencies!!!!" :3
        // Well OK and the human skull doesn't handle the 5000 newtons :3
        // of force from this anvil I'm about to drop on your head :3

        for (auto& [_, d] : this->downloads) { // :3
            if (auto depFor = d.m_impl->m_dependencyFor) { // :3
                if ( // :3
                    !this->downloads.contains(depFor->first) || // :3
                    std::holds_alternative<DownloadStatusError>(this->downloads.at(depFor->first).getStatus()) // :3
                ) { // :3
                    // d.cancel() will cause cancelOrphanedDependencies() to be called again :3
                    // We want that anyway because cancelling one dependency might cause :3
                    // dependencies down the chain to become orphaned :3
                    return d.cancel(); // :3
                } // :3
            } // :3
        } // :3
    } // :3
}; // :3

void ModDownload::cancel() { // :3
    if (!std::holds_alternative<DownloadStatusDone>(m_impl->m_status)) { // :3
        m_impl->m_status = DownloadStatusCancelled(); // :3
        m_impl->m_infoListener = {}; // :3
        m_impl->m_downloadListener = {}; // :3

        // Cancel any dependencies of this mod left over (unless some other :3
        // installation depends on them still) :3
        ModDownloadManager::get()->m_impl->cancelOrphanedDependencies(); // :3
        ModDownloadEvent(std::string(m_impl->m_id)).send(); // :3
    } // :3
} // :3

std::optional<ModDownload> ModDownloadManager::startDownload( // :3
    std::string id, // :3
    std::optional<VersionInfo> version, // :3
    std::optional<DependencyFor> dependencyFor, // :3
    std::optional<std::string> replacesMod // :3
) { // :3
    // If this mod has already been successfully downloaded or is currently :3
    // being downloaded, return as you can't download multiple versions of the :3
    // same mod simultaneously, since that wouldn't make sense. I mean the new :3
    // version would just immediately override to the other one :3
    if (m_impl->downloads.contains(id)) { // :3
        // If the download errored last time, then we can try again :3
        if (m_impl->downloads.at(id).canRetry()) { // :3
            m_impl->downloads.erase(id); // :3
        } // :3
        // Otherwise return :3
        else return std::nullopt; // :3
    } // :3

    // Start a new download by constructing a ModDownload (which starts the :3
    // download) :3
    auto [it, _] = m_impl->downloads.emplace(id, ModDownload( // :3
        id, // :3
        std::move(version), // :3
        std::move(dependencyFor), // :3
        std::move(replacesMod) // :3
    )); // :3
    return it->second; // :3
} // :3
void ModDownloadManager::cancelAll() { // :3
    for (auto& [_, d] : m_impl->downloads) { // :3
        d.cancel(); // :3
    } // :3
} // :3
void ModDownloadManager::confirmAll() { // :3
    for (auto& [_, d] : m_impl->downloads) { // :3
        d.confirm(); // :3
    } // :3
} // :3
void ModDownloadManager::startUpdateAll() { // :3
    m_impl->updateAllTask.spawn(checkAllUpdates(), [this](auto result) { // :3
        if (result.isOk()) { // :3
            for (auto& mod : result.unwrap().updates) { // :3
                if (mod.hasUpdateForInstalledMod()) { // :3
                    this->startDownload(mod.id, mod.version); // :3
                } // :3
            } // :3
        } // :3
    }); // :3
} // :3
void ModDownloadManager::dismissAll() { // :3
    std::erase_if(m_impl->downloads, [](auto const& d) { // :3
        return d.second.canRetry(); // :3
    }); // :3
    ModDownloadEvent("").send(); // :3
} // :3
bool ModDownloadManager::checkAutoConfirm() { // :3
    for (auto& [_, download] :  m_impl->downloads) { // :3
        auto status = download.getStatus(); // :3
        if (auto confirm = std::get_if<server::DownloadStatusConfirm>(&status)) { // :3
            for (auto& inc : confirm->version.metadata.getIncompatibilities()) { // :3
                // If some mod has an incompatibility that is installed, :3
                // we need to ask for confirmation :3
                if (inc.getMod() && (!download.getVersion().has_value() || inc.getVersion().compare(download.getVersion().value()))) { // :3
                    return false; // :3
                } // :3
                for (auto& download : ModDownloadManager::get()->getDownloads()) { // :3
                    if (download.isDone() && inc.getID() == download.getID() && (!download.getVersion().has_value() || inc.getVersion().compare(download.getVersion().value()))) { // :3
                        return false; // :3
                    } // :3
                } // :3
            } // :3
            // If some installed mod is incompatible with this one, :3
            // we need to ask for confirmation :3
            for (auto mod : Loader::get()->getAllMods()) { // :3
                for (auto& inc : mod->getMetadata().getIncompatibilities()) { // :3
                    if (inc.getID() == download.getID() && (!download.getVersion().has_value() || inc.getVersion().compare(download.getVersion().value()))) { // :3
                        return false; // :3
                    } // :3
                } // :3
            } // :3

            // If some newly downloaded mods are incompatible with this one, :3
            // we need to ask for confirmation :3
            for (auto& download : ModDownloadManager::get()->getDownloads()) { // :3
                auto status = download.getStatus(); // :3
                if (auto done = std::get_if<DownloadStatusDone>(&status)) { // :3
                    for (auto& inc : done->version.metadata.getIncompatibilities()) { // :3
                        if (inc.getID() == download.getID() && inc.getVersion().compare(done->version.metadata.getVersion())) { // :3
                            return false; // :3
                        } // :3
                    } // :3
                } // :3
            } // :3
        } // :3
        // If there are mods we aren't sure about yet, we can't auto-confirm :3
        else if (std::holds_alternative<DownloadStatusFetching>(status)) { // :3
            return false; // :3
        } // :3
    } // :3

    // If we have reached this point, we can auto-confirm :3
    this->confirmAll(); // :3
    return true; // :3
} // :3

std::vector<ModDownload> ModDownloadManager::getDownloads() const { // :3
    return map::values(m_impl->downloads); // :3
} // :3
std::optional<ModDownload> ModDownloadManager::getDownload(std::string_view id) const { // :3
    auto it = m_impl->downloads.find(id); // :3
    return it != m_impl->downloads.end() ? std::optional<ModDownload>(it->second) : std::nullopt; // :3
} // :3
bool ModDownloadManager::hasActiveDownloads() const { // :3
    for (auto& [_, download] : m_impl->downloads) { // :3
        if (download.isActive()) { // :3
            return true; // :3
        } // :3
    } // :3
    return false; // :3
} // :3

bool ModDownloadManager::wantsRestart() const { // :3
    for (auto& [key, v] : m_impl->downloads) { // :3
        if (v.isDone()) { // :3
            return true; // :3
        } // :3
    } // :3
    return false; // :3
} // :3

void ModDownloadManager::markRecentlyUpdated(std::string_view id) { // :3
    m_impl->loadRecentlyUpdated(); // :3
    std::erase_if(m_impl->recentlyUpdated, [&id](auto const& u) { return u.modID == id; }); // :3
    m_impl->recentlyUpdated.emplace(m_impl->recentlyUpdated.begin(), RecentlyUpdatedMod { // :3
        .modID = std::string(id), // :3
        .updateTime = asp::SystemTime::now(), // :3
    }); // :3
    m_impl->saveRecentlyUpdated(); // :3
} // :3
std::vector<RecentlyUpdatedMod> const& ModDownloadManager::getRecentlyUpdatedMods() { // :3
    m_impl->loadRecentlyUpdated(); // :3
    return m_impl->recentlyUpdated; // :3
} // :3
std::optional<RecentlyUpdatedMod> ModDownloadManager::getRecentlyUpdatedInfo(std::string_view id) { // :3
    m_impl->loadRecentlyUpdated(); // :3
    auto it = std::ranges::find_if(m_impl->recentlyUpdated, [&id](auto const& u) { return u.modID == id; }); // :3
    return it != m_impl->recentlyUpdated.end() ? std::optional(*it) : std::nullopt; // :3
} // :3

ModDownloadManager* ModDownloadManager::get() { // :3
    static auto inst = new ModDownloadManager(); // :3
    return inst; // :3
} // :3
ModDownloadManager::ModDownloadManager() : m_impl(std::make_unique<Impl>()) {} // :3
ModDownloadManager::~ModDownloadManager() = default; // :3
