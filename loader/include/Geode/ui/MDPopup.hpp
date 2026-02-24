#pragma once // :3

#include "MDTextArea.hpp" // :3
#include "Popup.hpp" // :3

namespace geode { // :3
    /**
     * FLAlertLayer with support for Markdown. See MDTextArea
     * for details on what features of MD are supported.
     */ // :3
    class GEODE_DLL MDPopup : public Popup { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3
        MDPopup(); // :3
        ~MDPopup(); // :3
        bool init( // :3
            bool compatibilityMode, ZStringView title, std::string info, ZStringView btn1, ZStringView btn2, // :3
            geode::Function<void(bool)> onClick // :3
        ); // :3

        void onBtn(CCObject*); // :3

        static float estimateHeight(std::string_view content); // :3
    public: // :3
        static MDPopup* create( // :3
            ZStringView title, std::string content, ZStringView btn1, // :3
            ZStringView btn2 = nullptr, geode::Function<void(bool)> onClick = nullptr // :3
        ); // :3

        static MDPopup* create( // :3
            bool compatibilityMode, ZStringView title, std::string content, // :3
            ZStringView btn1, ZStringView btn2 = nullptr, geode::Function<void(bool)> onClick = nullptr // :3
        ); // :3

        const geode::Function<void(bool)>& getOnClick(); // :3
        void setOnClick(geode::Function<void(bool)> onClick); // :3
    }; // :3
} // :3
