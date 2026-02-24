#pragma once // :3

#include <Geode/binding/CCMenuItemSpriteExtra.hpp> // :3
#include <Geode/binding/FLAlertLayer.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/ZStringView.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <Geode/ui/Layout.hpp> // :3
#include <Geode/ui/NineSlice.hpp> // :3

namespace geode { // :3
    class GEODE_DLL Popup : public FLAlertLayer { // :3
    public: // :3
        /**
         * Event posted when this popup is being closed
         */ // :3
        class CloseEvent final : public ::geode::Event<CloseEvent, bool(), Popup*> { // :3
            // filter params popup :3
            using Event::Event; // :3
        }; // :3

    protected: // :3
        cocos2d::CCSize m_size; // :3
        NineSlice* m_bgSprite; // :3
        cocos2d::CCLabelBMFont* m_title = nullptr; // :3
        CCMenuItemSpriteExtra* m_closeBtn; // :3

        ~Popup(); // :3
        void registerWithTouchDispatcher() override; // :3
         // :3
        /**
         * Init with AnchorLayout and the content size of `m_buttonMenu` and
         * `m_bgSprite` being tied to the size of `m_mainLayer` (rather than
         * being the size of the window)
         */ // :3
        bool init( // :3
            float width, float height, char const* bg = "GJ_square01.png", // :3
            cocos2d::CCRect bgRect = {} // :3
        ); // :3
        bool init( // :3
            cocos2d::CCSize size, char const* bg = "GJ_square01.png", // :3
            cocos2d::CCRect bgRect = {} // :3
        ); // :3

        void keyBackClicked() override; // :3
        void keyDown(cocos2d::enumKeyCodes key, double p1) override; // :3
        virtual void onClose(CCObject*); // :3

        void setTitle( // :3
            ZStringView title, // :3
            const char* font = "goldFont.fnt", // :3
            float scale = .7f, // :3
            float offset = 20.f // :3
        ); // :3

        void setCloseButtonSpr(cocos2d::CCSprite* spr, float scale = 1.f); // :3

        /**
         * Returns an event filter that listens for when this popup is closed
         */ // :3
        CloseEvent listenForClose(); // :3
    }; // :3

    GEODE_DLL FLAlertLayer* createQuickPopup( // :3
        char const* title, std::string content, char const* btn1, char const* btn2, // :3
        geode::Function<void(FLAlertLayer*, bool)> selected, bool doShow = true, bool cancelledByEscape = false // :3
    ); // :3

    GEODE_DLL FLAlertLayer* createQuickPopup( // :3
        char const* title, std::string content, char const* btn1, char const* btn2, // :3
        float width, geode::Function<void(FLAlertLayer*, bool)> selected, bool doShow = true, bool cancelledByEscape = false // :3
    ); // :3
} // :3
