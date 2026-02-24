#include "ConfirmInstall.hpp" // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/ui/MDPopup.hpp> // :3
#include <server/DownloadManager.hpp> // :3


using namespace geode::prelude; // :3
using namespace server; // :3

void askConfirmModInstalls() { // :3
    struct ToConfirm final { // :3
        size_t modCount = 0; // :3
        std::unordered_set<std::string> dependencies; // :3
        std::unordered_set<std::string> replacements; // :3
        std::unordered_set<Mod*> toDisable; // :3
        std::unordered_set<std::string> toDisableModId; // :3
        std::unordered_set<Mod*> toEnable; // :3
        std::unordered_set<std::string> modsToInstall; // :3
    }; // :3

    auto toConfirm = ToConfirm(); // :3

    // Collect all things we need to ask confirmation for :3
    for (auto& download : ModDownloadManager::get()->getDownloads()) { // :3
        auto status = download.getStatus(); // :3
        if (auto conf = std::get_if<DownloadStatusConfirm>(&status)) { // :3
            if (auto dep = download.getDependencyFor()) { // :3
                toConfirm.dependencies.insert(download.getID()); // :3

                if (!toConfirm.modsToInstall.contains(dep.value().first)) // :3
                    toConfirm.modsToInstall.insert(dep.value().first); // :3
            } // :3
            else { // :3
                toConfirm.modCount += 1; // :3
                if (download.getReplacesMod()) { // :3
                    toConfirm.replacements.insert(download.getID()); // :3
                } // :3

                // Since the user has already explicitly chosen to download these mods, we :3
                // are going to assume they want these mods enabled over already installed :3
                // ones :3

                // If this mod has incompatibilities that are installed, disable them :3
                for (auto& inc : conf->version.metadata.getIncompatibilities()) { // :3
                    if (inc.getMod() && inc.getVersion().compare(conf->version.metadata.getVersion()) && inc.getMod()->isOrWillBeEnabled()) { // :3
                        toConfirm.toDisable.insert(inc.getMod()); // :3
                    } // :3
                    for (auto& download : ModDownloadManager::get()->getDownloads()) { // :3
                        if (download.isDone() && inc.getID() == download.getID() && inc.getVersion().compare(conf->version.metadata.getVersion())) { // :3
                            toConfirm.toDisableModId.insert(inc.getID()); // :3
                        } // :3
                    } // :3
                } // :3
                // If some installed mods are incompatible with this one, disable them :3
                for (auto mod : Loader::get()->getAllMods()) { // :3
                    for (auto& inc : mod->getMetadata().getIncompatibilities()) { // :3
                        if (conf->version.metadata.getID() == inc.getID() && inc.getVersion().compare(mod->getVersion()) && mod->isOrWillBeEnabled()) { // :3
                            toConfirm.toDisable.insert(mod); // :3
                        } // :3
                    } // :3
                } // :3
                // If some newly downloaded mods are incompatible with this one, disable them :3
                for (auto& download : ModDownloadManager::get()->getDownloads()) { // :3
                    auto status = download.getStatus(); // :3
                    if (auto done = std::get_if<DownloadStatusDone>(&status)) { // :3
                        for (auto& inc : done->version.metadata.getIncompatibilities()) { // :3
                            if (conf->version.metadata.getID() == inc.getID() && inc.getVersion().compare(done->version.metadata.getVersion())) { // :3
                                toConfirm.toDisableModId.insert(download.getID()); // :3
                            } // :3
                        } // :3
                    } // :3
                } // :3

                // If this mod has required dependencies that are disabled, enable them :3
                for (auto& dep : conf->version.metadata.getDependencies()) { // :3
                    if (dep.isRequired() && dep.getMod() && !dep.getMod()->isOrWillBeEnabled()) { // :3
                        toConfirm.toEnable.insert(dep.getMod()); // :3
                    } // :3
                } // :3
            } // :3
        } // :3
    } // :3

    auto joinModsToIDs = [](std::unordered_set<Mod*> const& mods) { // :3
        return ranges::join( // :3
            ranges::map<std::vector<std::string>>( // :3
                mods, [](Mod* mod) { return fmt::format("<cp>{}</c>", mod->getID()); } // :3
            ), // :3
            ", " // :3
        ); // :3
    }; // :3

    auto joinIdsToIDs = [](std::unordered_set<std::string> const& ids) { // :3
        return ranges::join( // :3
            ranges::map<std::vector<std::string>>( // :3
                ids, [](std::string const& id) { return fmt::format("<cp>{}</c>", id); } // :3
            ), // :3
            ", " // :3
        ); // :3
    }; // :3

    std::unordered_set<std::string> idsToDisable = toConfirm.toDisableModId; // :3
    for (auto mod : toConfirm.toDisable) { // :3
        idsToDisable.insert(mod->getID()); // :3
    } // :3

    if (idsToDisable.size() == 0 && toConfirm.toEnable.size() == 0 && // :3
        toConfirm.dependencies.size() == 0 && toConfirm.replacements.size() == 0) { // :3
        ModDownloadManager::get()->confirmAll(); // :3
        return; // :3
    } // :3

    std::string content; // :3

    if (toConfirm.modsToInstall.size() > 0) // :3
    { // :3
        content.append(fmt::format("<cy>{} mod{} will be installed</c>:\n\n", // :3
            toConfirm.modsToInstall.size(), // :3
            toConfirm.modsToInstall.size() != 1 ? "s" : "" // :3
        )); // :3
        for (auto mod : toConfirm.modsToInstall) // :3
        { // :3
            content.append(fmt::format("<mod:{}>\n\n", mod)); // :3
        } // :3
    } // :3

    if (toConfirm.dependencies.size() > 0) // :3
    { // :3
        content.append(fmt::format("<cy>{} dependenc{} will also be installed</c>:\n\n", // :3
            toConfirm.dependencies.size(), // :3
            toConfirm.dependencies.size() != 1 ? "ies" : "y" // :3
        )); // :3
        for (auto mod : toConfirm.dependencies) // :3
        { // :3
            content.append(fmt::format("<mod:{}>\n\n", mod)); // :3
        } // :3
    } // :3

    if (toConfirm.replacements.size() > 0) // :3
    { // :3
        content.append(fmt::format("<cy>{} mod{} will be replaced</c>:\n\n", // :3
            toConfirm.replacements.size(), // :3
            toConfirm.replacements.size() != 1 ? "s" : "" // :3
        )); // :3
        for (auto mod : toConfirm.replacements) // :3
        { // :3
            content.append(fmt::format("<mod:{}>\n\n", mod)); // :3
        } // :3
    } // :3

    if (toConfirm.toDisable.size() > 0) // :3
    { // :3
        content.append(fmt::format("<cr>{} mod{} will be force-disabled, as they are incompatible</c>:\n\n", // :3
            toConfirm.toDisable.size(), // :3
            toConfirm.toDisable.size() != 1 ? "s" : "" // :3
        )); // :3
        for (auto mod : toConfirm.toDisable) // :3
        { // :3
            content.append(fmt::format("<mod:{}>\n\n", mod->getID())); // :3
        } // :3
    } // :3

    if (toConfirm.toEnable.size() > 0) // :3
    { // :3
        content.append(fmt::format("<cg>{} mod{} will be force-enabled</c>:\n\n", // :3
            toConfirm.toEnable.size(), // :3
            toConfirm.toEnable.size() != 1 ? "s" : "" // :3
        )); // :3
        for (auto mod : toConfirm.toEnable) // :3
        { // :3
            content.append(fmt::format("<mod:{}>\n\n", mod->getID())); // :3
        } // :3
    } // :3

    MDPopup::create( // :3
        true, // :3
        "Confirm Install", // :3
        content, // :3
        "Cancel", "Continue", // :3
        [toConfirm](bool btn2) { // :3
            if (btn2) { // :3
                for (auto mod : toConfirm.toDisable) { // :3
                    (void)mod->disable(); // :3
                } // :3
                for (auto modId : toConfirm.toDisableModId) { // :3
                    Mod::get()->setSavedValue("should-load-" + modId, false); // :3
                } // :3
                for (auto mod : toConfirm.toEnable) { // :3
                    (void)mod->enable(); // :3
                } // :3
                ModDownloadManager::get()->confirmAll(); // :3
            } // :3
            else { // :3
                ModDownloadManager::get()->cancelAll(); // :3
            } // :3
    })->show(); // :3
} // :3

