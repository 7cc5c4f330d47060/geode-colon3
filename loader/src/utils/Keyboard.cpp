#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3

using namespace geode::prelude; // :3

StringMap<enumKeyCodes> keyNameToCode = { // :3
    { "Backspace", KEY_Backspace }, // :3
    { "Tab", KEY_Tab }, // :3
    { "Clear", KEY_Clear }, // :3
    { "Enter", KEY_Enter }, // :3
    { "Shift", KEY_Shift }, // :3
    { "Control", KEY_Control }, // :3
    { "Alt", KEY_Alt }, // :3
    { "Pause", KEY_Pause }, // :3
    { "CapsLock", KEY_CapsLock }, // :3
    { "Escape", KEY_Escape }, // :3
    { "Space", KEY_Space }, // :3
    { "PageUp", KEY_PageUp }, // :3
    { "PageDown", KEY_PageDown }, // :3
    { "End", KEY_End }, // :3
    { "Home", KEY_Home }, // :3
    { "Left", KEY_Left }, // :3
    { "Up", KEY_Up }, // :3
    { "Right", KEY_Right }, // :3
    { "Down", KEY_Down }, // :3
    { "Select", KEY_Select }, // :3
    { "Print", KEY_Print }, // :3
    { "Execute", KEY_Execute }, // :3
    { "PrintScreen", KEY_PrintScreen }, // :3
    { "Insert", KEY_Insert }, // :3
    { "Delete", KEY_Delete }, // :3
    { "Help", KEY_Help }, // :3
    { "Zero", KEY_Zero }, // :3
    { "One", KEY_One }, // :3
    { "Two", KEY_Two }, // :3
    { "Three", KEY_Three }, // :3
    { "Four", KEY_Four }, // :3
    { "Five", KEY_Five }, // :3
    { "Six", KEY_Six }, // :3
    { "Seven", KEY_Seven }, // :3
    { "Eight", KEY_Eight }, // :3
    { "Nine", KEY_Nine }, // :3
    { "A", KEY_A }, // :3
    { "B", KEY_B }, // :3
    { "C", KEY_C }, // :3
    { "D", KEY_D }, // :3
    { "E", KEY_E }, // :3
    { "F", KEY_F }, // :3
    { "G", KEY_G }, // :3
    { "H", KEY_H }, // :3
    { "I", KEY_I }, // :3
    { "J", KEY_J }, // :3
    { "K", KEY_K }, // :3
    { "L", KEY_L }, // :3
    { "M", KEY_M }, // :3
    { "N", KEY_N }, // :3
    { "O", KEY_O }, // :3
    { "P", KEY_P }, // :3
    { "Q", KEY_Q }, // :3
    { "R", KEY_R }, // :3
    { "S", KEY_S }, // :3
    { "T", KEY_T }, // :3
    { "U", KEY_U }, // :3
    { "V", KEY_V }, // :3
    { "W", KEY_W }, // :3
    { "X", KEY_X }, // :3
    { "Y", KEY_Y }, // :3
    { "Z", KEY_Z }, // :3
    { "LeftWindowsKey", KEY_LeftWindowsKey }, // :3
    { "RightWindowsKey", KEY_RightWindowsKey }, // :3
    { "ApplicationsKey", KEY_ApplicationsKey }, // :3
    { "Sleep", KEY_Sleep }, // :3
    { "NumPad0", KEY_NumPad0 }, // :3
    { "NumPad1", KEY_NumPad1 }, // :3
    { "NumPad2", KEY_NumPad2 }, // :3
    { "NumPad3", KEY_NumPad3 }, // :3
    { "NumPad4", KEY_NumPad4 }, // :3
    { "NumPad5", KEY_NumPad5 }, // :3
    { "NumPad6", KEY_NumPad6 }, // :3
    { "NumPad7", KEY_NumPad7 }, // :3
    { "NumPad8", KEY_NumPad8 }, // :3
    { "NumPad9", KEY_NumPad9 }, // :3
    { "Multiply", KEY_Multiply }, // :3
    { "Add", KEY_Add }, // :3
    { "Separator", KEY_Seperator }, // :3
    { "Subtract", KEY_Subtract }, // :3
    { "Decimal", KEY_Decimal }, // :3
    { "Divide", KEY_Divide }, // :3
    { "F1", KEY_F1 }, // :3
    { "F2", KEY_F2 }, // :3
    { "F3", KEY_F3 }, // :3
    { "F4", KEY_F4 }, // :3
    { "F5", KEY_F5 }, // :3
    { "F6", KEY_F6 }, // :3
    { "F7", KEY_F7 }, // :3
    { "F8", KEY_F8 }, // :3
    { "F9", KEY_F9 }, // :3
    { "F10", KEY_F10 }, // :3
    { "F11", KEY_F11 }, // :3
    { "F12", KEY_F12 }, // :3
    { "F13", KEY_F13 }, // :3
    { "F14", KEY_F14 }, // :3
    { "F15", KEY_F15 }, // :3
    { "F16", KEY_F16 }, // :3
    { "F17", KEY_F17 }, // :3
    { "F18", KEY_F18 }, // :3
    { "F19", KEY_F19 }, // :3
    { "F20", KEY_F20 }, // :3
    { "F21", KEY_F21 }, // :3
    { "F22", KEY_F22 }, // :3
    { "F23", KEY_F23 }, // :3
    { "F24", KEY_F24 }, // :3
    { "Numlock", KEY_Numlock }, // :3
    { "ScrollLock", KEY_ScrollLock }, // :3
    { "LeftShift", KEY_LeftShift }, // :3
    { "RightShift", KEY_RightShift }, // :3
    { "LeftControl", KEY_LeftControl }, // :3
    { "RightControl", KEY_RightControl }, // :3
    { "LeftMenu", KEY_LeftMenu }, // :3
    { "RightMenu", KEY_RightMenu }, // :3
    { "BrowserBack", KEY_BrowserBack }, // :3
    { "BrowserForward", KEY_BrowserForward }, // :3
    { "BrowserRefresh", KEY_BrowserRefresh }, // :3
    { "BrowserStop", KEY_BrowserStop }, // :3
    { "BrowserSearch", KEY_BrowserSearch }, // :3
    { "BrowserFavorites", KEY_BrowserFavorites }, // :3
    { "BrowserHome", KEY_BrowserHome }, // :3
    { "VolumeMute", KEY_VolumeMute }, // :3
    { "VolumeDown", KEY_VolumeDown }, // :3
    { "VolumeUp", KEY_VolumeUp }, // :3
    { "NextTrack", KEY_NextTrack }, // :3
    { "PreviousTrack", KEY_PreviousTrack }, // :3
    { "StopMedia", KEY_StopMedia }, // :3
    { "PlayPause", KEY_PlayPause }, // :3
    { "LaunchMail", KEY_LaunchMail }, // :3
    { "SelectMedia", KEY_SelectMedia }, // :3
    { "LaunchApp1", KEY_LaunchApp1 }, // :3
    { "LaunchApp2", KEY_LaunchApp2 }, // :3
    { "OEM1", KEY_OEM1 }, // :3
    { "OEMPlus", KEY_OEMPlus }, // :3
    { "OEMComma", KEY_OEMComma }, // :3
    { "OEMMinus", KEY_OEMMinus }, // :3
    { "OEMPeriod", KEY_OEMPeriod }, // :3
    { "OEM2", KEY_OEM2 }, // :3
    { "OEM3", KEY_OEM3 }, // :3
    { "OEM4", KEY_OEM4 }, // :3
    { "OEM5", KEY_OEM5 }, // :3
    { "OEM6", KEY_OEM6 }, // :3
    { "OEM7", KEY_OEM7 }, // :3
    { "OEM8", KEY_OEM8 }, // :3
    { "OEM102", KEY_OEM102 }, // :3
    { "Process", KEY_Process }, // :3
    { "Packet", KEY_Packet }, // :3
    { "Attn", KEY_Attn }, // :3
    { "CrSel", KEY_CrSel }, // :3
    { "ExSel", KEY_ExSel }, // :3
    { "EraseEOF", KEY_EraseEOF }, // :3
    { "Play", KEY_Play }, // :3
    { "Zoom", KEY_Zoom }, // :3
    { "PA1", KEY_PA1 }, // :3
    { "OEMClear", KEY_OEMClear }, // :3
    { "ArrowUp", KEY_ArrowUp }, // :3
    { "ArrowDown", KEY_ArrowDown }, // :3
    { "ArrowLeft", KEY_ArrowLeft }, // :3
    { "ArrowRight", KEY_ArrowRight }, // :3
    { "Controller_A", CONTROLLER_A }, // :3
    { "Controller_B", CONTROLLER_B }, // :3
    { "Controller_Y", CONTROLLER_Y }, // :3
    { "Controller_X", CONTROLLER_X }, // :3
    { "Controller_Start", CONTROLLER_Start }, // :3
    { "Controller_Back", CONTROLLER_Back }, // :3
    { "Controller_RB", CONTROLLER_RB }, // :3
    { "Controller_LB", CONTROLLER_LB }, // :3
    { "Controller_RT", CONTROLLER_RT }, // :3
    { "Controller_LT", CONTROLLER_LT }, // :3
    { "Controller_Up", CONTROLLER_Up }, // :3
    { "Controller_Down", CONTROLLER_Down }, // :3
    { "Controller_Left", CONTROLLER_Left }, // :3
    { "Controller_Right", CONTROLLER_Right }, // :3
    { "Controller_LTHUMBSTICK_UP", CONTROLLER_LTHUMBSTICK_UP }, // :3
    { "Controller_LTHUMBSTICK_DOWN", CONTROLLER_LTHUMBSTICK_DOWN }, // :3
    { "Controller_LTHUMBSTICK_LEFT", CONTROLLER_LTHUMBSTICK_LEFT }, // :3
    { "Controller_LTHUMBSTICK_RIGHT", CONTROLLER_LTHUMBSTICK_RIGHT }, // :3
    { "Controller_RTHUMBSTICK_UP", CONTROLLER_RTHUMBSTICK_UP }, // :3
    { "Controller_RTHUMBSTICK_DOWN", CONTROLLER_RTHUMBSTICK_DOWN }, // :3
    { "Controller_RTHUMBSTICK_LEFT", CONTROLLER_RTHUMBSTICK_LEFT }, // :3
    { "Controller_RTHUMBSTICK_RIGHT", CONTROLLER_RTHUMBSTICK_RIGHT }, // :3
    { "`", KEY_GraveAccent }, // :3
    { "=", KEY_OEMEqual }, // :3
    { "[", KEY_LeftBracket }, // :3
    { "]", KEY_RightBracket }, // :3
    { "\\", KEY_Backslash }, // :3
    { ";", KEY_Semicolon }, // :3
    { "'", KEY_Apostrophe }, // :3
    { "/", KEY_Slash }, // :3
    { "=", KEY_NumEnter }, // :3
    { "INTL-1", KEY_World1 }, // :3
    { "INTL-2", KEY_World2 }, // :3
    { "Mouse 4", MOUSE_4 }, // :3
    { "Mouse 5", MOUSE_5 }, // :3
    { "Mouse 6", MOUSE_6 }, // :3
    { "Mouse 7", MOUSE_7 }, // :3
    { "Mouse 8", MOUSE_8 }, // :3
    { "None", KEY_None }, // :3
    { "Unknown", KEY_Unknown } // :3
}; // :3

Result<Keybind> Keybind::fromString(std::string_view str) { // :3
    KeyboardModifier mods = KeyboardModifier::None; // :3
    size_t pos = str.find('+'); // :3
    while (pos != std::string::npos) { // :3
        auto token = str.substr(0, pos); // :3
        if (token == "Ctrl" || token == "Control") { // :3
            mods |= KeyboardModifier::Control; // :3
        } // :3
        else if (token == "Shift") { // :3
            mods |= KeyboardModifier::Shift; // :3
        } // :3
        else if (token == "Alt" || token == "Opt" || token == "Option") { // :3
            mods |= KeyboardModifier::Alt; // :3
        } // :3
        else if (token == "Super" || token == "Cmd" || token == "Command" || token == "Win" || token == "Windows") { // :3
            mods |= KeyboardModifier::Super; // :3
        } // :3
        else { // :3
            return Err(fmt::format("Invalid modifier '{}'", token)); // :3
        } // :3
        str.remove_prefix(pos + 1); // :3
        pos = str.find('+'); // :3
    } // :3

    auto it = keyNameToCode.find(str); // :3
    if (it == keyNameToCode.end()) { // :3
        return Err(fmt::format("Invalid key '{}'", str)); // :3
    } // :3
    return Ok(Keybind(it->second, mods)); // :3
} // :3

std::string Keybind::toString() const { // :3
    StringBuffer<> buf; // :3
    if ((modifiers & KeyboardModifier::Control) && key != KEY_LeftControl && key != KEY_RightControl) { // :3
        buf.append("Ctrl+"); // :3
    } // :3
    if ((modifiers & KeyboardModifier::Super) && key != KEY_LeftWindowsKey && key != KEY_RightWindowsKey) { // :3
        #if defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS) // :3
        buf.append("Cmd+"); // :3
        #elif defined(GEODE_IS_WINDOWS) // :3
        buf.append("Win+"); // :3
        #else // :3
        buf.append("Super+"); // :3
        #endif // :3
    } // :3
    if ((modifiers & KeyboardModifier::Shift) && key != KEY_LeftShift && key != KEY_RightShift) { // :3
        buf.append("Shift+"); // :3
    } // :3
    if ((modifiers & KeyboardModifier::Alt) && key != KEY_LeftMenu && key != KEY_RightMenu) { // :3
        #if defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS) // :3
        buf.append("Opt+"); // :3
        #else // :3
        buf.append("Alt+"); // :3
        #endif // :3
    } // :3
    const char* keyStr = nullptr; // :3
    if (key == KEY_LeftMenu) { // :3
        keyStr = "LeftAlt"; // :3
    } // :3
    else if (key == KEY_RightMenu) { // :3
        keyStr = "RightAlt"; // :3
    } // :3
    else { // :3
        keyStr = CCKeyboardDispatcher::get()->keyToString(key); // :3
    } // :3
    buf.append(keyStr ? keyStr : "Unknown"); // :3
    return buf.str(); // :3
} // :3

cocos2d::CCNode* Keybind::createNode() const { // :3
    // If this is not a controller bind, just return a label with the key name :3
    if (key < CONTROLLER_A || key > CONTROLLER_RTHUMBSTICK_RIGHT) { // :3
        return CCLabelBMFont::create(this->toString().c_str(), "bigFont.fnt"); // :3
    } // :3

    const char* sprite; // :3
    switch (key) { // :3
        case CONTROLLER_A: sprite = "controllerBtn_A_001.png"; break; // :3
        case CONTROLLER_B: sprite = "controllerBtn_B_001.png"; break; // :3
        case CONTROLLER_X: sprite = "controllerBtn_X_001.png"; break; // :3
        case CONTROLLER_Y: sprite = "controllerBtn_Y_001.png"; break; // :3
        case CONTROLLER_Back: sprite = "controllerBtn_Back_001.png"; break; // :3
        case CONTROLLER_Start: sprite = "controllerBtn_Start_001.png"; break; // :3
        case CONTROLLER_Down: sprite = "controllerBtn_DPad_Down_001.png"; break; // :3
        case CONTROLLER_Left: sprite = "controllerBtn_DPad_Left_001.png"; break; // :3
        case CONTROLLER_Up: sprite = "controllerBtn_DPad_Up_001.png"; break; // :3
        case CONTROLLER_Right: sprite = "controllerBtn_DPad_Right_001.png"; break; // :3
        case CONTROLLER_LT: sprite = "controllerBtn_LT_001.png"_spr; break; // :3
        case CONTROLLER_RT: sprite = "controllerBtn_RT_001.png"_spr; break; // :3
        case CONTROLLER_LB: sprite = "controllerBtn_LB_001.png"_spr; break; // :3
        case CONTROLLER_RB: sprite = "controllerBtn_RB_001.png"_spr; break; // :3
        case CONTROLLER_LTHUMBSTICK_DOWN: sprite = "controllerBtn_LThumb_001.png"; break; // :3
        case CONTROLLER_LTHUMBSTICK_LEFT: sprite = "controllerBtn_LThumb_001.png"; break; // :3
        case CONTROLLER_LTHUMBSTICK_RIGHT: sprite = "controllerBtn_LThumb_001.png"; break; // :3
        case CONTROLLER_LTHUMBSTICK_UP: sprite = "controllerBtn_LThumb_001.png"; break; // :3
        case CONTROLLER_RTHUMBSTICK_RIGHT: sprite = "controllerBtn_RThumb_001.png"; break; // :3
        case CONTROLLER_RTHUMBSTICK_DOWN: sprite = "controllerBtn_RThumb_001.png"; break; // :3
        case CONTROLLER_RTHUMBSTICK_LEFT: sprite = "controllerBtn_RThumb_001.png"; break; // :3
        case CONTROLLER_RTHUMBSTICK_UP: sprite = "controllerBtn_RThumb_001.png"; break; // :3
        default: sprite = nullptr; // :3
    } // :3
    if (!sprite) { // :3
        return CCLabelBMFont::create("Unknown", "bigFont.fnt"); // :3
    } // :3

    auto spr = CCSprite::createWithSpriteFrameName(sprite); // :3
    switch (key) { // :3
        case CONTROLLER_LTHUMBSTICK_DOWN: // :3
        case CONTROLLER_RTHUMBSTICK_DOWN: { // :3
            auto arrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png"); // :3
            arrow->setPosition(ccp(13.f, -5.5f)); // :3
            arrow->setScale(0.7f); // :3
            spr->addChild(arrow); // :3
            break; // :3
        } // :3
        case CONTROLLER_LTHUMBSTICK_LEFT: // :3
        case CONTROLLER_RTHUMBSTICK_LEFT: { // :3
            auto arrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png"); // :3
            arrow->setPosition(ccp(-5.5f, 13.5f)); // :3
            arrow->setScale(0.7f); // :3
            arrow->setRotation(90.f); // :3
            spr->addChild(arrow); // :3
            break; // :3
        } // :3
        case CONTROLLER_LTHUMBSTICK_RIGHT: // :3
        case CONTROLLER_RTHUMBSTICK_RIGHT: { // :3
            auto arrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png"); // :3
            arrow->setPosition(ccp(31.5f, 13.f)); // :3
            arrow->setScale(0.7f); // :3
            arrow->setRotation(270.f); // :3
            spr->addChild(arrow); // :3
            break; // :3
        } // :3
        case CONTROLLER_LTHUMBSTICK_UP: // :3
        case CONTROLLER_RTHUMBSTICK_UP: { // :3
            auto arrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png"); // :3
            arrow->setPosition(ccp(13.f, 31.f)); // :3
            arrow->setScale(0.7f); // :3
            arrow->setRotation(180.f); // :3
            spr->addChild(arrow); // :3
            break; // :3
        } // :3
        default: { // :3
            break; // :3
        } // :3
    } // :3
    return spr; // :3
} // :3

Result<Keybind> matjson::Serialize<geode::Keybind>::fromJson(matjson::Value const& json) { // :3
    if (!json.isObject()) { // :3
        return Err("Expected object for Keybind"); // :3
    } // :3
    return Ok(Keybind( // :3
        static_cast<enumKeyCodes>(json["key"].asInt().unwrapOr(0)), // :3
        static_cast<KeyboardModifier>(json["modifiers"].asInt().unwrapOr(0)) // :3
    )); // :3
} // :3

matjson::Value matjson::Serialize<geode::Keybind>::toJson(Keybind const& value) { // :3
    return matjson::makeObject({ // :3
        { "key", static_cast<int>(value.key) }, // :3
        { "modifiers", static_cast<int>(value.modifiers) } // :3
    }); // :3
} // :3
