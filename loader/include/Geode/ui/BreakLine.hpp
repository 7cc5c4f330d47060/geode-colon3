#pragma once // :3

#include <cocos2d.h> // :3

namespace geode { // :3
    class GEODE_DLL BreakLine : public cocos2d::CCNode { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3
        BreakLine(); // :3
        ~BreakLine(); // :3
        void draw() override; // :3
        bool init(float width, float height, cocos2d::ccColor4F color); // :3
    public: // :3
        /**
         * Create a break line to act as a separator
         * @param width Width of the line
         * @param height Height of the line
         * @param color The color of the line
         */ // :3
        static BreakLine* create( // :3
            float width, // :3
            float height = 1.f, // :3
            cocos2d::ccColor4F color = {1.f, 1.f, 1.f, .2f} // :3
        ); // :3

        cocos2d::ccColor4F getColor(); // :3
    }; // :3
} // :3
