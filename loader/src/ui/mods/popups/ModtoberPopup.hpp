#pragma once // :3

#include <Geode/ui/Popup.hpp> // :3
#include "../GeodeStyle.hpp" // :3

using namespace geode::prelude; // :3

class ModtoberPopup : public GeodePopup { // :3
protected: // :3
    bool init() override; // :3

    void onDiscord(CCObject*); // :3

public: // :3
    static ModtoberPopup* create(); // :3
}; // :3