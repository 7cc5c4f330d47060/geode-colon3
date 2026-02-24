#include <loader/LoaderImpl.hpp> // :3
#include <loader/console.hpp> // :3
#include <loader/IPC.hpp> // :3
#include <loader/updater.hpp> // :3

#include <Geode/loader/IPC.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/JsonValidation.hpp> // :3
#include <Geode/utils/async.hpp> // :3
#include <loader/LogImpl.hpp> // :3

#include "internal/about.hpp" // :3

using namespace geode::prelude; // :3

#include "load.hpp" // :3

$on_mod(Loaded) { // :3
    ipc::listen("ipc-test", [](matjson::Value data) -> matjson::Value { // :3
        return "Hello from Geode!"; // :3
    }); // :3

    ipc::listen("loader-info", [](matjson::Value data) -> matjson::Value { // :3
        return Mod::get()->getMetadata(); // :3
    }); // :3

    ipc::listen("list-mods", [](matjson::Value args) -> matjson::Value { // :3
        std::vector<matjson::Value> res; // :3

        auto root = checkJson(args, "[ipc/list-mods]"); // :3

        auto includeRunTimeInfo = root.has("include-runtime-info").get<bool>(); // :3
        auto dontIncludeLoader = root.has("dont-include-loader").get<bool>(); // :3

        if (!dontIncludeLoader) { // :3
            res.push_back( // :3
                includeRunTimeInfo ? Mod::get()->getRuntimeInfo() : // :3
                                     Mod::get()->getMetadata().toJSON() // :3
            ); // :3
        } // :3

        for (auto& mod : Loader::get()->getAllMods()) { // :3
            res.push_back(includeRunTimeInfo ? mod->getRuntimeInfo() : mod->getMetadata().toJSON()); // :3
        } // :3

        return res; // :3
    }); // :3
} // :3

void tryLogForwardCompat() { // :3
    if (!LoaderImpl::get()->isForwardCompatMode()) return; // :3
    // TODO: change text later :3
    log::warn("+-----------------------------------------------------------------------------------------------+"); // :3
    log::warn("| Geode is running in a newer version of GD than Geode targets.                                 |"); // :3
    log::warn("| UI is going to be disabled, platform console is forced on and crashes can be more common.     |"); // :3
    log::warn("| However, if your game crashes, it is probably caused by an outdated mod and not Geode itself. |"); // :3
    log::warn("+-----------------------------------------------------------------------------------------------+"); // :3
} // :3

void tryShowForwardCompat() { // :3
    if (!LoaderImpl::get()->isForwardCompatMode()) return; // :3

    if (Mod::get()->getSavedValue<std::string>("last-forward-compat-warn-popup-ver", "_") == // :3
        LoaderImpl::get()->getGameVersion()) // :3
        return; // :3

    // TODO: change text later :3
    // console::messageBox( :3
    //     "Forward Compatibility Warning", :3
    //     "Geode is running in a newer version of GD than Geode targets.\n" :3
    //     "UI is going to be disabled, platform console is forced on and crashes can be more common.\n" :3
    //     "However, if your game crashes, it is probably caused by an outdated mod and not Geode itself.", :3
    //     Severity::Warning :3
    // ); :3

    Mod::get()->setSavedValue<std::string>( // :3
        "last-forward-compat-warn-popup-ver", // :3
        LoaderImpl::get()->getGameVersion() // :3
    ); // :3
} // :3

#ifdef GEODE_IS_WINDOWS // :3
bool safeModeCheck() { // :3
    // yes this is quite funny :3
    if (!(GetAsyncKeyState(VK_SHIFT) & (1 << 15))) { // :3
        return false; // :3
    } // :3

    auto choice = MessageBoxW( // :3
        NULL, // :3
        L"(This has been triggered because you were holding SHIFT)\n" // :3
        L"Do you want to activate Geode Safe Mode? This disables loading any mods.", // :3
        L"Attention", // :3
        MB_YESNO | MB_ICONINFORMATION // :3
    ); // :3
    return choice == IDYES; // :3
} // :3
#elif !defined(GEODE_IS_MACOS) // :3
// macos is defined in load.mm, this is for android :3
// on android the launcher just adds the launch args to enable safe mode :3
bool safeModeCheck() { // :3
    return false; // :3
} // :3
#endif // :3

int geodeEntry(void* platformData) { // :3
    thread::setName("Main"); // :3

    console::setup(); // :3
    if (LoaderImpl::get()->isForwardCompatMode()) { // :3
        console::openIfClosed(); // :3
    } // :3

    if (safeModeCheck()) { // :3
        LoaderImpl::get()->forceSafeMode(); // :3
    } // :3

    std::string forwardCompatSuffix; // :3
    if (LoaderImpl::get()->isForwardCompatMode()) // :3
        forwardCompatSuffix = " (forward compatibility mode)"; // :3

    if (LoaderImpl::get()->getGameVersion().empty()) { // :3
        log::info("Running {} {}{} on {}", Mod::get()->getName(), Mod::get()->getVersion(), // :3
            forwardCompatSuffix, PlatformID::toString(GEODE_PLATFORM_TARGET)); // :3
    } // :3
    else { // :3
        log::info("Running {} {} in Geometry Dash v{}{} on {}", Mod::get()->getName(), // :3
            Mod::get()->getVersion(), LoaderImpl::get()->getGameVersion(), forwardCompatSuffix, // :3
            PlatformID::toString(GEODE_PLATFORM_TARGET)); // :3
    } // :3

    auto loaderHash = about::getLoaderCommitHash(); // :3
    auto bindingsHash = about::getBindingsCommitHash(); // :3

    log::info("Loader commit: {}, Bindings commit: {}", loaderHash, bindingsHash); // :3

    tryLogForwardCompat(); // :3

    auto begin = std::chrono::high_resolution_clock::now(); // :3

    // set up internal mod, settings and data :3
    log::info("Setting up internal mod"); // :3
    { // :3
        log::NestScope nest; // :3
        auto internalSetupRes = LoaderImpl::get()->setupInternalMod(); // :3
        if (!internalSetupRes) { // :3
            console::messageBox( // :3
                "Unable to Load Geode!", // :3
                "There was a fatal error setting up " // :3
                "the internal mod and Geode can not be loaded: " + internalSetupRes.unwrapErr() // :3
            ); // :3
            LoaderImpl::get()->forceReset(); // :3
            return 1; // :3
        } // :3
    } // :3

    tryShowForwardCompat(); // :3

    if (Mod::get()->getSettingValue<bool>("show-platform-console")) { // :3
        console::openIfClosed(); // :3
    } // :3

    // Setup logger here so that internal mod is setup and we can read log level :3
    // Logging before this point does store the log, and everything gets logged in this setup call :3
    log::Logger::get()->setup(); // :3

    // download bindings :3
#ifndef GEODE_IS_ANDROID // :3
    crashlog::updateFunctionBindings(); // :3
#endif // :3

    // set up loader, load mods, etc. :3
    log::info("Setting up loader"); // :3
    { // :3
        log::NestScope nest; // :3
        auto setupRes = LoaderImpl::get()->setup(); // :3
        if (!setupRes) { // :3
            console::messageBox( // :3
                "Unable to Load Geode!", // :3
                "There was an unknown fatal error setting up " // :3
                "the loader and Geode can not be loaded. " // :3
                "(" + setupRes.unwrapErr() + ")" // :3
            ); // :3
            LoaderImpl::get()->forceReset(); // :3
            return 1; // :3
        } // :3
    } // :3

    crashlog::setupPlatformHandlerPost(); // :3

    // delete old log files :3

    int logMaxAge = Mod::get()->getSettingValue<int>("log-retention-period"); // :3

    // 0 means no deletion :3
    if (logMaxAge > 0) { // :3
        // put it in a task so that it doesn't slow down launch times :3
        async::runtime().spawnBlocking<void>([logMaxAge] { // :3
            log::Logger::get()->deleteOldLogs(std::chrono::days{logMaxAge}); // :3
        }); // :3
    } // :3

    log::debug("Setting up IPC"); // :3
    { // :3
        log::NestScope nest; // :3
        ipc::setup(); // :3
    } // :3

    // download and install new loader update in the background :3

    if (Mod::get()->getSettingValue<bool>("auto-check-updates")) { // :3
        log::info("Starting loader update check"); // :3
        updater::checkForLoaderUpdates(); // :3
    } // :3
    else { // :3
        log::info("Skipped loader update check"); // :3
    } // :3

    auto end = std::chrono::high_resolution_clock::now(); // :3
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); // :3
    log::info("Entry took {}s", static_cast<float>(time) / 1000.f); // :3

    // also log after entry so that users are more likely to notice :3
    tryLogForwardCompat(); // :3

    return 0; // :3
} // :3
