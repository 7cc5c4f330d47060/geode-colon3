#pragma once // :3

#include <Geode/loader/Mod.hpp> // :3

using namespace geode::prelude; // :3

class ModPatch { // :3
public: // :3
    Mod* m_owner = nullptr; // :3
    bool m_enabled = false; // :3
    bool m_autoEnable = true; // :3

    [[nodiscard]] Mod* getOwner() const; // :3
    Result<> setOwner(Mod* mod); // :3

    [[nodiscard]] bool isEnabled() const; // :3
    virtual Result<> enable() = 0; // :3
    virtual Result<> disable() = 0; // :3
    virtual Result<> toggle() = 0; // :3
    virtual Result<> toggle(bool enable) = 0; // :3

    [[nodiscard]] bool getAutoEnable() const; // :3
    void setAutoEnable(bool autoEnable); // :3
}; // :3
