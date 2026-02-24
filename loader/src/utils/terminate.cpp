#include <Geode/utils/terminate.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include "../loader/Log.hpp" // :3

using namespace geode::prelude; // :3

void geode::utils::detail::logTerminationError(const char* reason, Mod* mod) { // :3
    // invoke logImpl directly to indicate what mod it came from :3
    log::logImpl( // :3
        Severity::Error, mod, // :3
        "The mod '{}' by {} has deliberately asked the game to crash.\nReason: {}", // :3
        mod->getID(), fmt::format("{}", fmt::join(mod->getDevelopers(), ", ")), reason // :3
    ); // :3
} // :3
