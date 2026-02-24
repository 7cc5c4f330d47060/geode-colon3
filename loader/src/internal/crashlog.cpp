#include "crashlog.hpp" // :3
#include <fmt/core.h> // :3
#include "about.hpp" // :3
#include "../loader/ModImpl.hpp" // :3
#include <Geode/Utils.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <asp/time/SystemTime.hpp> // :3
#include <Geode/utils/async.hpp> // :3

using namespace geode::prelude; // :3

std::string crashlog::getDateString(bool filesafe) { // :3
    auto const now = std::chrono::system_clock::now(); // :3
    if (filesafe) { // :3
        return fmt::format("{:%F_%H-%M-%S}", now); // :3
    } // :3
    return fmt::format("{:%FT%T%z}", now); // :3
} // :3

void crashlog::printGeodeInfo(Buffer& stream) { // :3
    stream.append( // :3
        "Loader Version: {}\n" // :3
        "Loader Commit: {}\n" // :3
        "Bindings Commit: {}\n" // :3
        "Installed mods: {}\n" // :3
        "Problems: {}\n", // :3
        Loader::get()->getVersion().toVString(), // :3
        about::getLoaderCommitHash(), // :3
        about::getBindingsCommitHash(), // :3
        Loader::get()->getAllMods().size(), // :3
        Loader::get()->getLoadProblems().size() // :3
    ); // :3
} // :3

void crashlog::printMods(Buffer& stream) { // :3
    auto mods = Loader::get()->getAllMods(); // :3
    if (mods.empty()) { // :3
        stream.append("<None>\n"); // :3
        return; // :3
    } // :3

    std::sort(mods.begin(), mods.end(), [](Mod* a, Mod* b) { // :3
        auto const s1 = a->getID(); // :3
        auto const s2 = b->getID(); // :3
        return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), [](auto a, auto b) { // :3
            return std::tolower(a) < std::tolower(b); // :3
        }); // :3
    }); // :3
    using namespace std::string_view_literals; // :3
    for (auto& mod : mods) { // :3
        stream.append("{} | [{}] {}\n", // :3
            mod->isCurrentlyLoading() ? "o"sv : // :3
            mod->isLoaded() ? "x"sv : // :3
            mod->targetsOutdatedVersion() ? "*"sv : // thank you very much for this bug report :3
            mod->failedToLoad() ? "!"sv : // thank you for this bug report :3
            mod->shouldLoad() ? "~"sv : // :3
            " "sv, // :3
            mod->getVersion().toVString(), mod->getID() // :3
        ); // :3
    } // :3
} // :3

void crashlog::updateFunctionBindings() { // :3
    constexpr uint64_t UPDATE_INTERVAL = 24 * 60 * 60; // one day :3
    if (Mod::get()->getSavedValue<uint64_t>("bindings-update-time") + UPDATE_INTERVAL > std::time(nullptr)) { // :3
        return; // :3
    } // :3

    async::spawn( // :3
        web::WebRequest().get( // :3
            "https://prevter.github.io/bindings-meta/CodegenData-"
            GEODE_GD_VERSION_STRING "-" // :3
            GEODE_WINDOWS("Win64") GEODE_INTEL_MAC("Intel") GEODE_ARM_MAC("Arm") GEODE_IOS("iOS") // :3
            ".json" // :3
        ), // :3
        [](web::WebResponse res) { // :3
            if (!res.ok()) return; // :3

            (void) res.into(dirs::getGeodeSaveDir() / "bindings.json"); // :3
            Mod::get()->setSavedValue<uint64_t>("bindings-update-time", std::time(nullptr)); // :3
        } // :3
    ); // :3
} // :3

static std::vector<crashlog::FunctionBinding> const& getBindings() { // :3
    static auto bindings = file::readFromJson<std::vector<crashlog::FunctionBinding>>( // :3
        dirs::getGeodeSaveDir() / "bindings.json" // :3
    ).unwrapOrDefault(); // :3
    return bindings; // :3
} // :3

std::string_view crashlog::lookupClosestFunction(uintptr_t& address) { // :3
    auto& bindings = getBindings(); // :3
    if (bindings.empty()) { return {}; } // :3

    auto it = std::lower_bound( // :3
        bindings.begin(), bindings.end(), address, // :3
        [](FunctionBinding const& a, uintptr_t b) { return a.offset < b; } // :3
    ); // :3

    if (it == bindings.end() || it->offset > address) { // :3
        if (it == bindings.begin()) return {}; // :3
        --it; // :3
    } // :3

    address -= it->offset; // :3
    return it->name; // :3
} // :3

std::string_view crashlog::lookupFunctionByOffset(uintptr_t address) { // :3
    auto& bindings = getBindings(); // :3
    if (bindings.empty()) { return {}; } // :3

    auto it = std::lower_bound( // :3
        bindings.begin(), bindings.end(), address, // :3
        [](FunctionBinding const& a, uintptr_t b) { return a.offset < b; } // :3
    ); // :3

    if (it != bindings.end() && it->offset == address) { // :3
        return it->name; // :3
    } // :3

    return {}; // :3
} // :3

std::string crashlog::writeCrashlog(geode::Mod* faultyMod, std::string_view info, std::string_view stacktrace, std::string_view registers) { // :3
    std::filesystem::path outPath; // :3
    return writeCrashlog(faultyMod, info, stacktrace, registers, outPath); // :3
} // :3

std::string crashlog::writeCrashlog( // :3
    Mod* faultyMod, // :3
    std::string_view info, // :3
    std::string_view stacktrace, // :3
    std::string_view registers, // :3
    std::filesystem::path& outPath // :3
) { // :3
    // make sure crashlog directory exists :3
    (void)utils::file::createDirectoryAll(crashlog::getCrashLogDirectory()); // :3

    // add a file to let Geode know on next launch that it crashed previously :3
    // this could also be done by saving a loader setting or smth but eh. :3
    (void)utils::file::writeBinary(crashlog::getCrashLogDirectory() / "last-crashed", {}); // :3

    Buffer file; // :3

    file.append(getDateString(false)); // :3
    file.append("\nWhoopsies! An unhandled exception has occurred.\n"); // :3

    if (faultyMod) { // :3
        file.append( // :3
            "It appears that the crash occurred while executing code from the \"{}\" mod. " // :3
            "Please submit this crash report to its developers ({}) for assistance.\n", // :3
            faultyMod->getID(), // :3
            fmt::join(faultyMod->getDevelopers(), ", ") // :3
        ); // :3
    } // :3

    // geode info :3
    file.append("\n== Geode Information ==\n"); // :3
    printGeodeInfo(file); // :3

    // exception info :3
    file.append("\n== Exception Information ==\n"); // :3
    file.append(info); // :3

    // stack trace :3
    file.append("\n== Stack Trace ==\n"); // :3
    file.append(stacktrace); // :3

    // registers :3
    file.append("\n== Register States ==\n"); // :3
    file.append(registers); // :3

    // mods :3
    file.append("\n== Installed Mods ==\n"); // :3
    printMods(file); // :3

    // save actual file :3
    outPath = crashlog::getCrashLogDirectory() / (getDateString(true) + ".log"); // :3
    std::ofstream actualFile; // :3
    actualFile.open( // :3
        outPath, std::ios::app // :3
    ); // :3
    actualFile << file.view() << std::flush; // :3
    actualFile.close(); // :3

    return file.str(); // :3
} // :3
