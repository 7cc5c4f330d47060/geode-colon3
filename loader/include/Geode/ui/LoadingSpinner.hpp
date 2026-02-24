#pragma once // :3

#include <cocos2d.h> // :3

namespace geode { // :3
    /**
     * An eternally spinning loading circle. Essentially just a more convenient
     * alternative to RobTop's `LoadingCircle` class, as this one respects its
     * content size and is a lot more stripped down (not requiring a `show`
     * method or anything - it just works!)
     */ // :3
    class GEODE_DLL LoadingSpinner final : public cocos2d::CCNodeRGBA { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3
        LoadingSpinner(); // :3
        ~LoadingSpinner(); // :3

        bool init(float size); // :3
        void spin(); // :3
    public: // :3
        /**
         * Create a loading circle
         * @param size The diameter of the circle in Cocos units
         */ // :3
        static LoadingSpinner* create(float size); // :3

        void setVisible(bool visible) override; // :3
        void setContentSize(const cocos2d::CCSize& size) override; // :3
         // :3
        cocos2d::CCSprite* getSpinner(); // :3
    }; // :3
} // :3
