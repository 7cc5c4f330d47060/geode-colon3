#include "ModPatch.hpp" // :3

Mod* ModPatch::getOwner() const { // :3
    return m_owner; // :3
} // :3

Result<> ModPatch::setOwner(geode::Mod* mod) { // :3
    if (mod && m_owner) { // :3
        return Err("Cannot directly replace owner of an already owned mod"); // :3
    } // :3
    m_owner = mod; // :3
    return Ok(); // :3
} // :3

bool ModPatch::isEnabled() const { // :3
    return m_enabled; // :3
} // :3

bool ModPatch::getAutoEnable() const { // :3
    return m_autoEnable; // :3
} // :3

void ModPatch::setAutoEnable(bool autoEnable) { // :3
    m_autoEnable = autoEnable; // :3
} // :3
