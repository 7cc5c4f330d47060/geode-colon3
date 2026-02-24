#include <Geode/DefaultInclude.hpp> // :3

#ifndef GEODE_IS_IOS // :3

#include <Geode/cocos/include/cocos2d.h> // :3
#include <Geode/modify/CCKeyboardDispatcher.hpp> // :3

using namespace geode::prelude; // :3

class $modify(CCKeyboardDispatcher) { // :3
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("CCKeyboardDispatcher new keys") // :3

    const char* keyToString(enumKeyCodes key) { // :3
        if (key == KEY_C) return "C"; // :3

        if (key < 0x405) { // :3
            return CCKeyboardDispatcher::keyToString(key); // :3
        } // :3

        switch (key) { // :3
            case CONTROLLER_LTHUMBSTICK_UP: // :3
                return "Controller_LTHUMBSTICK_UP"; // :3
            case CONTROLLER_LTHUMBSTICK_DOWN: // :3
                return "Controller_LTHUMBSTICK_DOWN"; // :3
            case CONTROLLER_LTHUMBSTICK_LEFT: // :3
                return "Controller_LTHUMBSTICK_LEFT"; // :3
            case CONTROLLER_LTHUMBSTICK_RIGHT: // :3
                return "Controller_LTHUMBSTICK_RIGHT"; // :3
            case CONTROLLER_RTHUMBSTICK_UP: // :3
                return "Controller_RTHUMBSTICK_UP"; // :3
            case CONTROLLER_RTHUMBSTICK_DOWN: // :3
                return "Controller_RTHUMBSTICK_DOWN"; // :3
            case CONTROLLER_RTHUMBSTICK_LEFT: // :3
                return "Controller_RTHUMBSTICK_LEFT"; // :3
            case CONTROLLER_RTHUMBSTICK_RIGHT: // :3
                return "Controller_RTHUMBSTICK_RIGHT"; // :3
            case KEY_GraveAccent: // :3
                return "`"; // :3
            case KEY_OEMEqual: // :3
                return "="; // :3
            case KEY_LeftBracket: // :3
                return "["; // :3
            case KEY_RightBracket: // :3
                return "]"; // :3
            case KEY_Backslash: // :3
                return "\\"; // :3
            case KEY_Semicolon: // :3
                return ";"; // :3
            case KEY_Apostrophe: // :3
                return "'"; // :3
            case KEY_Slash: // :3
                return "/"; // :3
            case KEY_Equal: // :3
                return "="; // :3
            case KEY_NumEnter: // :3
                return "NumEnter"; // :3
            case KEY_World1: // :3
                return "INTL-1"; // :3
            case KEY_World2: // :3
                return "INTL-2"; // :3
            case MOUSE_4: // :3
                return "Mouse 4"; // :3
            case MOUSE_5: // :3
                return "Mouse 5"; // :3
            case MOUSE_6: // :3
                return "Mouse 6"; // :3
            case MOUSE_7: // :3
                return "Mouse 7"; // :3
            case MOUSE_8: // :3
                return "Mouse 8"; // :3
            default: // :3
                return CCKeyboardDispatcher::keyToString(KEY_Unknown); // :3
        } // :3
    } // :3
}; // :3

#endif // :3