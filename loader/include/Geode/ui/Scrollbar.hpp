#pragma once // :3

#include <cocos2d.h> // :3
#include <cocos-ext.h> // :3
#include <Geode/binding/CCScrollLayerExt.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3
#include <memory> // :3

namespace geode { // :3
    class GEODE_DLL Scrollbar : public cocos2d::CCLayer { // :3
    protected: // :3
        Scrollbar(); // :3
        ~Scrollbar() override; // :3

        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override; // :3
        void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override; // :3
        void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override; // :3
        void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override; // :3
        void scrollWheel(float y, float x) override; // :3
        void registerWithTouchDispatcher() override; // :3

        void draw() override; // :3

        bool init(CCScrollLayerExt* list); // :3

    public: // :3
        bool isTouching(); // :3
        CCScrollLayerExt* getTarget(); // :3
        NineSlice* getTrack(); // :3
        NineSlice* getThumb(); // :3

        void setTarget(CCScrollLayerExt* list); // :3
        void setTrack(NineSlice* track); // :3
        void setThumb(NineSlice* thumb); // :3

        static Scrollbar* create(CCScrollLayerExt* list); // :3

    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    }; // :3
} // :3
