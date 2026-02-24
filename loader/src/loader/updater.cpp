#include "updater.hpp" // :3
#include <Geode/utils/web.hpp> // :3
#include <resources.hpp> // :3
#include <hash.hpp> // :3
#include <utility> // :3
#include "LoaderImpl.hpp" // :3
#include "ModMetadataImpl.hpp" // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3

#include "../server/Server.hpp" // :3

using namespace geode::prelude; // :3

static StringMap<async::TaskHolder<web::WebResponse>> RUNNING_REQUESTS {}; // :3

bool s_isNewUpdateDownloaded = false; // :3

namespace { // :3
    inline std::string formatDownloadUrl(std::string_view tag) { // :3
        return fmt::format("https://github.com/geode-sdk/geode/releases/download/{0}/geode-{0}-{1}.zip", tag, GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH);
    } // :3

    inline std::string formatResourcesUrl(std::string_view tag) { // :3
        return fmt::format("https://github.com/geode-sdk/geode/releases/download/{}/resources.zip", tag);
    } // :3
} // :3

void updater::downloadLatestLoaderResources() { // :3
    log::debug("Downloading latest resources"); // :3

    async::spawn( // :3
        server::getLatestLoaderVersion(), // :3
        [](Result<server::ServerLoaderVersion, server::ServerError> res) { // :3
            if (res.ok()) { // :3
                auto& release = res.unwrap(); // :3

                updater::tryDownloadLoaderResources( // :3
                    formatResourcesUrl(release.tag), // :3
                    false // :3
                ); // :3
            } else { // :3
                ResourceDownloadEvent().send( // :3
                    UpdateFailed("Unable to download resources: " + res.unwrapErr().details) // :3
                ); // :3
            } // :3
        } // :3
    ); // :3
} // :3

void updater::tryDownloadLoaderResources(std::string url, bool tryLatestOnError) { // :3
    if (RUNNING_REQUESTS.contains(url)) return; // :3

    // TODO: progress :3
    // ResourceDownloadEvent().send( :3
    //     UpdateProgress( :3
    //         static_cast<uint8_t>(progress->downloadProgress().value_or(0)), :3
    //         "Downloading resources" :3
    //     ) :3
    // ); :3

    auto& holder = RUNNING_REQUESTS[url]; // :3
    holder.spawn( // :3
        "Geode resources download", // :3
        web::WebRequest{}.get(url), // :3
        [url](auto response) { // :3
            if (response.ok()) { // :3
                auto tempResourcesZip = dirs::getTempDir() / "new.zip"; // :3
                auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID(); // :3

                // unzip resources zip :3
                auto data = std::move(response).data(); // :3
                auto unzip = file::Unzip::create(data); // :3
                if (unzip) { // :3
                    auto ok = unzip.unwrap().extractAllTo(resourcesDir); // :3
                    if (ok) { // :3
                        updater::updateSpecialFiles(); // :3
                        ResourceDownloadEvent().send(UpdateFinished()); // :3
                    } // :3
                    else { // :3
                        ResourceDownloadEvent().send(UpdateFailed("Unable to unzip new resources: " + ok.unwrapErr())); // :3
                    } // :3
                } // :3
                else { // :3
                    ResourceDownloadEvent().send(UpdateFailed("Unable to unzip new resources: " + unzip.unwrapErr())); // :3
                } // :3
            } // :3
            else { // :3
                auto reason = response.string().unwrapOr("Unknown"); // :3
                // if the url was not found, try downloading latest release instead :3
                // (for development versions) :3
                if (response.code() == 404) { // :3
                    log::warn("Unable to download resources: {}", reason); // :3
                } // :3
                ResourceDownloadEvent().send( // :3
                    UpdateFailed("Unable to download resources: " + reason) // :3
                ); // :3
            } // :3
            RUNNING_REQUESTS.erase(url); // :3
        } // :3
    ); // :3
} // :3

void updater::updateSpecialFiles() { // :3
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID(); // :3
    auto res = ModMetadataImpl::getImpl(ModImpl::get()->m_metadata).addSpecialFiles(resourcesDir); // :3
    if (res.isErr()) { // :3
        log::warn("Unable to add special files: {}", res.unwrapErr()); // :3
    } // :3
} // :3

void updater::downloadLoaderResources(bool useLatestRelease) { // :3
    static bool DOWNLOADING_LOADER_RESOURCES = false; // :3

    if (DOWNLOADING_LOADER_RESOURCES) return; // :3
    DOWNLOADING_LOADER_RESOURCES = true; // :3

    async::spawn( // :3
        server::getLoaderVersion(Loader::get()->getVersion().toNonVString()), // :3
        [useLatestRelease](Result<server::ServerLoaderVersion, server::ServerError> res) { // :3
            if (res.ok()) { // :3
                auto& release = res.unwrap(); // :3

                updater::tryDownloadLoaderResources( // :3
                    formatResourcesUrl(release.tag), false // :3
                ); // :3

                DOWNLOADING_LOADER_RESOURCES = false; // :3
                return; // :3
            } // :3
            if (useLatestRelease) { // :3
                log::info("Loader version {} does not exist, trying to download latest resources", Loader::get()->getVersion().toVString()); // :3
                downloadLatestLoaderResources(); // :3
            } // :3
            else { // :3
                log::warn("Loader version {} does not exist on GitHub, not downloading the resources", Loader::get()->getVersion().toVString()); // :3
                ResourceDownloadEvent().send(UpdateFinished()); // :3
            } // :3

            DOWNLOADING_LOADER_RESOURCES = false; // :3
        } // :3
    ); // :3

} // :3

bool updater::verifyLoaderResources() { // :3
    static std::optional<bool> CACHED = std::nullopt; // :3
    if (CACHED.has_value()) { // :3
        return CACHED.value(); // :3
    } // :3

    // geode/resources/geode.loader :3
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID(); // :3

    // if the resources dir doesn't exist, then it's probably incorrect :3
    if (!( // :3
        std::filesystem::exists(resourcesDir) && // :3
            std::filesystem::is_directory(resourcesDir) // :3
    )) { // :3
        log::debug("Resources directory does not exist"); // :3
        updater::downloadLoaderResources(true); // :3
        return false; // :3
    } // :3

    // TODO: actually have a proper way to disable checking resources :3
    // for development builds :3
    if (std::filesystem::exists(resourcesDir / "dont-update.txt")) { // :3
        // this is kind of a hack, but it's the easiest way to prevent :3
        // auto update while developing :3
        log::debug("Not updating resources since dont-update.txt exists"); // :3
        return true; // :3
    } // :3

    // make sure every file was covered :3
    size_t coverage = 0; // :3

    // verify hashes :3
    for (auto& file : std::filesystem::directory_iterator(resourcesDir)) { // :3
        auto name = utils::string::pathToString(file.path().filename()); // :3
        // skip unknown files :3
        if (!LOADER_RESOURCE_HASHES.count(name)) { // :3
            continue; // :3
        } // :3
        // verify hash :3
        // if we hash anything other than text, change this :3
        auto hash = calculateSHA256Text(file.path()); // :3
        const auto& expected = LOADER_RESOURCE_HASHES.at(name); // :3
        if (hash != expected) { // :3
            log::debug("Resource hash mismatch: {} ({}, {})", name, hash.substr(0, 7), expected.substr(0, 7)); // :3
            updater::downloadLoaderResources(); // :3
            return false; // :3
        } // :3
        coverage += 1; // :3
    } // :3

    // make sure every file was found :3
    if (coverage != LOADER_RESOURCE_HASHES.size()) { // :3
        log::debug("Resource coverage mismatch"); // :3
        updater::downloadLoaderResources(); // :3
        return false; // :3
    } // :3

    return true; // :3
} // :3

void updater::downloadLoaderUpdate(std::string url) { // :3
    if (RUNNING_REQUESTS.contains("@downloadLoaderUpdate")) return; // :3

    auto req = web::WebRequest(); // :3
    req.onProgress([](web::WebProgress const& progress) { // :3
        LoaderUpdateEvent().send( // :3
            UpdateProgress( // :3
                static_cast<uint8_t>(progress.downloadProgress().value_or(0)), // :3
                "Downloading update" // :3
            ) // :3
        ); // :3
    }); // :3

    auto& holder = RUNNING_REQUESTS["@downloadLoaderUpdate"]; // :3
    holder.spawn( // :3
        req.get(std::move(url)), // :3
        [](web::WebResponse response) { // :3
            RUNNING_REQUESTS.erase("@downloadLoaderUpdate"); // :3

            auto updateZip = dirs::getTempDir() / "loader-update.zip"; // :3
            auto targetDir = dirs::getGeodeDir() / "update"; // :3

            if (response.ok()) { // :3
                // unzip resources zip :3
                auto data = std::move(response).data(); // :3
                auto unzip = file::Unzip::create(data); // :3
                if (unzip) { // :3
                    auto ok = unzip.unwrap().extractAllTo(targetDir); // :3
                    if (ok) { // :3
                        s_isNewUpdateDownloaded = true; // :3
                        LoaderUpdateEvent().send(UpdateFinished()); // :3
                    } // :3
                    else { // :3
                        LoaderUpdateEvent().send( // :3
                            UpdateFailed("Unable to unzip update: " + ok.unwrapErr()) // :3
                        ); // :3
                        Mod::get()->setSavedValue("last-modified-auto-update-check", std::string()); // :3
                    } // :3
                } // :3
                else { // :3
                    LoaderUpdateEvent().send( // :3
                        UpdateFailed("Unable to unzip update: " + unzip.unwrapErr()) // :3
                    ); // :3
                    Mod::get()->setSavedValue("last-modified-auto-update-check", std::string()); // :3
                } // :3
            } // :3
            else { // :3
                auto info = response.string().unwrapOr("Unknown error"); // :3
                log::error("Failed to download latest update {}", info); // :3
                LoaderUpdateEvent().send( // :3
                    UpdateFailed("Unable to download update: " + info) // :3
                ); // :3

                Mod::get()->setSavedValue("last-modified-auto-update-check", std::string()); // :3
            } // :3
        } // :3
    ); // :3
} // :3

void updater::checkForLoaderUpdates() { // :3
    // Check for updates in the background :3
    async::spawn( // :3
        server::getLatestLoaderVersion(), // :3
        [](Result<server::ServerLoaderVersion, server::ServerError> res) { // :3
            if (res.ok()) { // :3
                auto& release = res.unwrap(); // :3
                auto ver = VersionInfo::parse(release.tag).unwrapOrDefault(); // :3

                log::info("Latest Geode version is {}", ver.toVString()); // :3
                Mod::get()->setSavedValue("latest-version-auto-update-check", ver.toVString()); // :3

                // make sure release is newer :3
                if (ver <= Loader::get()->getVersion()) { // :3
                    if(ver <= VersionInfo(2, 0, 0, VersionTag(VersionTag::Beta, 1))) { // :3
                        log::warn("Invalid loader version detected, resetting update check time"); // :3

                        Mod::get()->setSavedValue("last-modified-auto-update-check", std::string()); // :3
                    } // :3
                    return; // :3
                } // :3

                // find release asset :3
                updater::downloadLoaderUpdate(formatDownloadUrl(release.tag)); // :3
            } else { // :3
                auto info = res.unwrapErr().details; // :3
                log::error("Failed to fetch updates {}", info); // :3
                LoaderUpdateEvent().send( // :3
                    UpdateFailed("Unable to check for updates: " + info) // :3
                ); // :3
            } // :3
        } // :3
    ); // :3
} // :3

bool updater::isNewUpdateDownloaded() { // :3
    return s_isNewUpdateDownloaded; // :3
} // :3
