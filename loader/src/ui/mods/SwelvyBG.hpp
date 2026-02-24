#pragma once // :3

#include <Geode/ui/General.hpp> // :3
#include <Geode/utils/ColorProvider.hpp> // :3

using namespace geode::prelude; // :3

class SwelvyBG : public CCNode { // :3
protected: // :3
    bool init(); // :3

    void updateSpritePosition(float dt); // :3

    // std::vector<EventListener<ColorProvidedFilter>> m_colorListeners; :3
    std::vector<ListenerHandle> m_colorHandles; // :3

public: // :3
    static SwelvyBG* create(); // :3
}; // :3
