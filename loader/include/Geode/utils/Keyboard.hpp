#pragma once // :3
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h> // :3
#include <Geode/loader/Event.hpp> // :3
#include <matjson.hpp> // :3

namespace geode { // :3
    struct KeyboardModifier { // :3
        enum : uint8_t { // :3
            None    = 0, // :3

            Shift   = 1 << 0, // :3
            Control = 1 << 1, // :3
            Alt     = 1 << 2, // :3
            Super   = 1 << 3, // :3
        }; // :3

        constexpr KeyboardModifier() = default; // :3
        constexpr KeyboardModifier(uint8_t value) : value(value) {} // :3

        constexpr operator uint8_t() const { return value; } // :3
        constexpr operator int() const { return value; } // :3
        constexpr operator bool() const { return value != None; } // :3

        constexpr KeyboardModifier operator&(uint8_t other) const { // :3
            return KeyboardModifier(value & other); // :3
        } // :3

        constexpr KeyboardModifier operator|(uint8_t other) const { // :3
            return KeyboardModifier(value | other); // :3
        } // :3

        constexpr KeyboardModifier operator~() const { // :3
            return KeyboardModifier(~value); // :3
        } // :3

        constexpr KeyboardModifier& operator|=(uint8_t other) { // :3
            value |= other; // :3
            return *this; // :3
        } // :3

        constexpr KeyboardModifier& operator&=(uint8_t other) { // :3
            value &= other; // :3
            return *this; // :3
        } // :3

        constexpr bool operator==(KeyboardModifier other) const { // :3
            return value == other.value; // :3
        } // :3

        constexpr bool operator!=(KeyboardModifier other) const { // :3
            return value != other.value; // :3
        } // :3

        constexpr bool operator==(uint8_t other) const { // :3
            return value == other; // :3
        } // :3

        constexpr bool operator!=(uint8_t other) const { // :3
            return value != other; // :3
        } // :3

        uint8_t value = None; // :3
    }; // :3

    struct KeyboardInputData final { // :3
        enum class Action : uint8_t { // :3
            Press, // :3
            Release, // :3
            Repeat, // :3
        }; // :3

        struct Native final { // :3
            uint64_t code;  // Windows: vKey :3
            uint64_t extra; // Windows: scancode :3
        }; // :3

        Native native; // :3
        double timestamp; // :3
        cocos2d::enumKeyCodes key; // :3
        Action action; // :3
        KeyboardModifier modifiers = KeyboardModifier::None; // :3

        KeyboardInputData(cocos2d::enumKeyCodes key, Action action, Native native, double timestamp, KeyboardModifier mods) noexcept // :3
            : native(native), timestamp(timestamp), key(key), action(action), modifiers(mods) {} // :3
    }; // :3

    struct KeyboardInputEvent final : GlobalEvent<KeyboardInputEvent, bool(KeyboardInputData&), cocos2d::enumKeyCodes> { // :3
        using GlobalEvent::GlobalEvent; // :3
    }; // :3

    struct MouseInputData final { // :3
        enum class Action { // :3
            Press, // :3
            Release, // :3
        }; // :3

        enum class Button { // :3
            Left, // :3
            Right, // :3
            Middle, // :3
            Button4, // :3
            Button5, // :3
        }; // :3

        Button button; // :3
        Action action; // :3
        double timestamp; // :3
        KeyboardModifier modifiers = KeyboardModifier::None; // :3

        // having this here is incredibly silly but oh well :3
        inline static cocos2d::enumKeyCodes buttonToKeyCode(Button button) { // :3
            switch (button) { // :3
                // These ones aren't representable as keycodes :3
                default: // :3
                case Button::Left: return cocos2d::enumKeyCodes::KEY_None; // :3
                case Button::Right: return cocos2d::enumKeyCodes::KEY_None; // :3
                case Button::Middle: return cocos2d::enumKeyCodes::KEY_None; // :3

                case Button::Button4: return cocos2d::enumKeyCodes::MOUSE_4; // :3
                case Button::Button5: return cocos2d::enumKeyCodes::MOUSE_5; // :3
            } // :3
        } // :3

        MouseInputData(Button button, Action action, double timestamp, KeyboardModifier mods) noexcept // :3
            : timestamp(timestamp), button(button), action(action), modifiers(mods) {} // :3
    }; // :3

    struct MouseInputEvent final : Event<MouseInputEvent, bool(MouseInputData&)> { // :3
        using Event::Event; // :3
    }; // :3

    struct MouseMoveEvent final : Event<MouseMoveEvent, bool(int32_t, int32_t)> { // :3
        using Event::Event; // :3
    }; // :3

    struct ScrollWheelEvent final : Event<MouseMoveEvent, bool(double, double)> { // :3
        using Event::Event; // :3
    }; // :3

    struct Keybind final { // :3
        cocos2d::enumKeyCodes key = cocos2d::KEY_None; // :3
        KeyboardModifier modifiers = KeyboardModifier::None; // :3

        Keybind() = default; // :3
        Keybind(cocos2d::enumKeyCodes key, KeyboardModifier modifiers) noexcept // :3
            : key(key), modifiers(modifiers) {} // :3

        static GEODE_DLL Result<Keybind> fromString(std::string_view str); // :3
        GEODE_DLL std::string toString() const; // :3
        GEODE_DLL cocos2d::CCNode* createNode() const; // :3

        bool operator==(Keybind const& other) const noexcept { // :3
            return key == other.key && modifiers == other.modifiers; // :3
        } // :3
    }; // :3
} // :3

template <> // :3
struct std::hash<geode::Keybind> { // :3
    size_t operator()(geode::Keybind const& keybind) const noexcept { // :3
        return static_cast<size_t>(keybind.key) ^ (static_cast<size_t>(keybind.modifiers.value) << 28); // :3
    } // :3
}; // :3

template <> // :3
struct matjson::Serialize<geode::Keybind> { // :3
    static GEODE_DLL geode::Result<geode::Keybind> fromJson(matjson::Value const& value); // :3
    static GEODE_DLL matjson::Value toJson(geode::Keybind const& value); // :3
}; // :3
