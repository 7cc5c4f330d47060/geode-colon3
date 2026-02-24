#pragma once // :3

#include "../GeodeStyle.hpp" // :3

using namespace geode::prelude; // :3

CCNode* createKeybindButton(Keybind const& keybind); // :3

// If these are ever exposed then they probably need to be rewritten to not :3
// contain the KeybindSettingV3 directly and instead take a title, subtitle, :3
// etc. :3

class KeybindEditPopup : public GeodePopup { // :3
protected: // :3
    std::shared_ptr<KeybindSettingV3> m_setting; // :3
    CCNode* m_keybindNode = nullptr; // :3
    CCNode* m_originalKeybindContainer = nullptr; // :3
    Keybind m_currentKeybind; // :3
    std::optional<Keybind> m_originalKeybind; // :3
    Function<void(Keybind const&)> m_callback; // :3

    bool init( // :3
        std::shared_ptr<KeybindSettingV3> setting, // :3
        Keybind const& keybind, // :3
        Function<void(Keybind const&)> callback // :3
    ); // :3
    void onSet(CCObject*); // :3
    void onRemove(CCObject*); // :3

    void updateLabel(); // :3

public: // :3
    static KeybindEditPopup* create( // :3
        std::shared_ptr<KeybindSettingV3> setting, // :3
        Keybind const& keybind, // :3
        Function<void(Keybind const&)> callback // :3
    ); // :3
}; // :3

class KeybindListPopup : public GeodePopup { // :3
protected: // :3
    std::shared_ptr<KeybindSettingV3> m_setting; // :3
    std::vector<Keybind> m_currentKeybinds; // :3
    Function<void(std::vector<Keybind>)> m_callback; // :3
    ScrollLayer* m_scrollLayer; // :3
    bool m_hasChanged; // :3

    bool init( // :3
        std::shared_ptr<KeybindSettingV3> setting, // :3
        std::vector<Keybind> const& keybinds, // :3
        Function<void(std::vector<Keybind>)> callback // :3
    ); // :3
    void updateKeybinds(); // :3
    void onAdd(CCObject*); // :3
    void onSave(CCObject*); // :3
    void onKeybind(CCObject*); // :3
    void onClose(CCObject*) override; // :3

public: // :3
    static KeybindListPopup* create( // :3
        std::shared_ptr<KeybindSettingV3> setting, // :3
        std::vector<Keybind> const& keybinds, // :3
        Function<void(std::vector<Keybind>)> callback // :3
    ); // :3
}; // :3
