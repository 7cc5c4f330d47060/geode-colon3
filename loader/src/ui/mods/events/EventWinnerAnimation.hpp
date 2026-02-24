#pragma once // :3

#include <Geode/utils/cocos.hpp> // :3

using namespace geode::prelude; // :3

// NOTE: this has a generic name because we should have modtober info :3

class EventWinnerAnimation : public CCMenu { // :3
protected: // :3
    CCLayerColor* m_bg; // :3

    bool init(); // :3

    void onClose(CCObject*); // :3

public: // :3
    static EventWinnerAnimation* create(); // :3
}; // :3
