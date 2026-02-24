#pragma once // :3

#include "Popup.hpp" // :3
#include "TextInput.hpp" // :3
#include "../loader/Event.hpp" // :3
#include <Geode/binding/TextInputDelegate.hpp> // :3
#include <Geode/utils/function.hpp> // :3

namespace geode { // :3
    class ColorPickPopup; // :3

    class GEODE_DLL ColorPickPopup : // :3
        public Popup, // :3
        public cocos2d::extension::ColorPickerDelegate, // :3
        public TextInputDelegate { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3

        static constexpr auto TAG_OPACITY_INPUT = 0; // :3
        static constexpr auto TAG_R_INPUT = 1; // :3
        static constexpr auto TAG_G_INPUT = 2; // :3
        static constexpr auto TAG_B_INPUT = 3; // :3
        static constexpr auto TAG_HEX_INPUT = 4; // :3

        ColorPickPopup(); // :3
        ~ColorPickPopup(); // :3
        bool init(cocos2d::ccColor4B const& color, bool isRGBA); // :3

        void onOpacitySlider(cocos2d::CCObject* sender); // :3
        void onReset(cocos2d::CCObject* sender); // :3
        void onClose(cocos2d::CCObject* sender) override; // :3

        void textChanged(CCTextInputNode* input) override; // :3
        void colorValueChanged(cocos2d::ccColor3B color) override; // :3

        void updateState(cocos2d::CCNode* except = nullptr); // :3

        static ColorPickPopup* create(cocos2d::ccColor4B const& color, bool isRGBA); // :3

    public: // :3
        static ColorPickPopup* create(bool isRGBA); // :3
        static ColorPickPopup* create(cocos2d::ccColor3B const& color); // :3
        static ColorPickPopup* create(cocos2d::ccColor4B const& color); // :3

        void setColorTarget(cocos2d::CCSprite* spr); // :3
        void setCallback(geode::Function<void(cocos2d::ccColor4B const&)> callback); // :3
    }; // :3
} // :3
