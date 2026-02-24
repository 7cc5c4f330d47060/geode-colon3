#pragma once // :3

#include "../DefaultInclude.hpp" // :3
#include <cocos2d.h> // :3

namespace geode { // :3
    class GEODE_DLL OverlayManager final : public cocos2d::CCNode { // :3
    public: // :3
        static OverlayManager* get(); // :3
    }; // :3
} // :3
