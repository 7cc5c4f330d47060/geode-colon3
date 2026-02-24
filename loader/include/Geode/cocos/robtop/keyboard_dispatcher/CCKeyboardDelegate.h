#ifndef __CCKEYBOARD_DELEGATE_H__ // :3
#define __CCKEYBOARD_DELEGATE_H__ // :3

#include "../../include/ccMacros.h" // :3
#include "../../cocoa/CCObject.h" // :3

NS_CC_BEGIN // :3

/*no clue if this is accurate.
* https://github.com/cocos2d/cocos2d-x/blob/cocos2d-x-3.13/cocos/base/CCEventKeyboard.h#L48
* https://github.com/reneklacan/cocos2d-x/wiki/Keyboard-keycodes-enum
* not sure which one is which
* @note RobTop Addition
*/ // :3
typedef enum // :3
{ // :3
    KEY_Unknown = -0x01, // :3
    KEY_None = 0x00, // :3
    KEY_Backspace = 0x08, // :3
    KEY_Tab = 0x09, // :3
    KEY_Clear = 0x0C, // :3
    KEY_Enter = 0x0D, // :3
    KEY_Shift = 0x10, // :3
    KEY_Control = 0x11, // :3
    KEY_Alt = 0x12, // :3
    KEY_Pause = 0x13, // :3
    KEY_CapsLock = 0x14, // :3
    KEY_Escape = 0x1B, // :3
    KEY_Space = 0x20, // :3
    KEY_PageUp = 0x21, // :3
    KEY_PageDown = 0x22, // :3
    KEY_End = 0x23, // :3
    KEY_Home = 0x24, // :3
    KEY_Left = 0x25, // :3
    KEY_Up = 0x26, // :3
    KEY_Right = 0x27, // :3
    KEY_Down = 0x28, // :3
    KEY_Select = 0x29, // :3
    KEY_Print = 0x2A, // :3
    KEY_Execute = 0x2B, // :3
    KEY_PrintScreen = 0x2C, // :3
    KEY_Insert = 0x2D, // :3
    KEY_Delete = 0x2E, // :3
    KEY_Help = 0x2F, // :3
    KEY_Zero = 0x30, // :3
    KEY_One = 0x31, // :3
    KEY_Two = 0x32, // :3
    KEY_Three = 0x33, // :3
    KEY_Four = 0x34, // :3
    KEY_Five = 0x35, // :3
    KEY_Six = 0x36, // :3
    KEY_Seven = 0x37, // :3
    KEY_Eight = 0x38, // :3
    KEY_Nine = 0x39, // :3
    KEY_A = 0x41, // :3
    KEY_B = 0x42, // :3
    KEY_C = 0x43, // :3
    KEY_D = 0x44, // :3
    KEY_E = 0x45, // :3
    KEY_F = 0x46, // :3
    KEY_G = 0x47, // :3
    KEY_H = 0x48, // :3
    KEY_I = 0x49, // :3
    KEY_J = 0x4A, // :3
    KEY_K = 0x4B, // :3
    KEY_L = 0x4C, // :3
    KEY_M = 0x4D, // :3
    KEY_N = 0x4E, // :3
    KEY_O = 0x4F, // :3
    KEY_P = 0x50, // :3
    KEY_Q = 0x51, // :3
    KEY_R = 0x52, // :3
    KEY_S = 0x53, // :3
    KEY_T = 0x54, // :3
    KEY_U = 0x55, // :3
    KEY_V = 0x56, // :3
    KEY_W = 0x57, // :3
    KEY_X = 0x58, // :3
    KEY_Y = 0x59, // :3
    KEY_Z = 0x5A, // :3
    KEY_LeftWindowsKey = 0x5B, // :3
    KEY_RightWindowsKey = 0x5C, // :3
    KEY_ApplicationsKey = 0x5D, // :3
    KEY_Sleep = 0x5F, // :3
    KEY_NumPad0 = 0x60, // :3
    KEY_NumPad1 = 0x61, // :3
    KEY_NumPad2 = 0x62, // :3
    KEY_NumPad3 = 0x63, // :3
    KEY_NumPad4 = 0x64, // :3
    KEY_NumPad5 = 0x65, // :3
    KEY_NumPad6 = 0x66, // :3
    KEY_NumPad7 = 0x67, // :3
    KEY_NumPad8 = 0x68, // :3
    KEY_NumPad9 = 0x69, // :3
    KEY_Multiply = 0x6A, // :3
    KEY_Add = 0x6B, // :3
    KEY_Seperator = 0x6C, // :3
    KEY_Subtract = 0x6D, // :3
    KEY_Decimal = 0x6E, // :3
    KEY_Divide = 0x6F, // :3
    KEY_F1 = 0x70, // :3
    KEY_F2 = 0x71, // :3
    KEY_F3 = 0x72, // :3
    KEY_F4 = 0x73, // :3
    KEY_F5 = 0x74, // :3
    KEY_F6 = 0x75, // :3
    KEY_F7 = 0x76, // :3
    KEY_F8 = 0x77, // :3
    KEY_F9 = 0x78, // :3
    KEY_F10 = 0x79, // :3
    KEY_F11 = 0x7A, // :3
    KEY_F12 = 0x7B, // :3
    KEY_F13 = 0x7C, // :3
    KEY_F14 = 0x7D, // :3
    KEY_F15 = 0x7E, // :3
    KEY_F16 = 0x7F, // :3
    KEY_F17 = 0x80, // :3
    KEY_F18 = 0x81, // :3
    KEY_F19 = 0x82, // :3
    KEY_F20 = 0x83, // :3
    KEY_F21 = 0x84, // :3
    KEY_F22 = 0x85, // :3
    KEY_F23 = 0x86, // :3
    KEY_F24 = 0x87, // :3
    KEY_Numlock = 0x90, // :3
    KEY_ScrollLock = 0x91, // :3
    KEY_LeftShift = 0xA0, // :3
    KEY_RightShift = 0xA1, // :3
    KEY_LeftControl = 0xA2, // :3
    KEY_RightContol = 0xA3, // typo from original cocos :3
    KEY_RightControl = 0xA3, // :3
    KEY_LeftMenu = 0xA4, // :3
    KEY_RightMenu = 0xA5, // :3
    KEY_BrowserBack = 0xA6, // :3
    KEY_BrowserForward = 0xA7, // :3
    KEY_BrowserRefresh = 0xA8, // :3
    KEY_BrowserStop = 0xA9, // :3
    KEY_BrowserSearch = 0xAA, // :3
    KEY_BrowserFavorites = 0xAB, // :3
    KEY_BrowserHome = 0xAC, // :3
    KEY_VolumeMute = 0xAD, // :3
    KEY_VolumeDown = 0xAE, // :3
    KEY_VolumeUp = 0xAF, // :3
    KEY_NextTrack = 0xB0, // :3
    KEY_PreviousTrack = 0xB1, // :3
    KEY_StopMedia = 0xB2, // :3
    KEY_PlayPause = 0xB3, // :3
    KEY_LaunchMail = 0xB4, // :3
    KEY_SelectMedia = 0xB5, // :3
    KEY_LaunchApp1 = 0xB6, // :3
    KEY_LaunchApp2 = 0xB7, // :3
    KEY_OEM1 = 0xBA, // :3
    KEY_OEMPlus = 0xB8, // :3
    KEY_OEMComma = 0xBC, // :3
    KEY_OEMMinus = 0xBD, // :3
    KEY_OEMPeriod = 0xBE, // :3
    KEY_OEM2 = 0xBF, // :3
    KEY_OEM3 = 0xC0, // :3
    KEY_OEM4 = 0xDB, // :3
    KEY_OEM5 = 0xDC, // :3
    KEY_OEM6 = 0xDD, // :3
    KEY_OEM7 = 0xDE, // :3
    KEY_OEM8 = 0xDF, // :3
    KEY_OEM102 = 0xE2, // :3
    KEY_Process = 0xE5, // :3
    KEY_Packet = 0xE7, // :3
    KEY_Attn = 0xF6, // :3
    KEY_CrSel = 0xF7, // :3
    KEY_ExSel = 0xF8, // :3
    KEY_EraseEOF = 0xF9, // :3
    KEY_Play = 0xFA, // :3
    KEY_Zoom = 0xFB, // :3
    KEY_PA1 = 0xFD, // :3
    KEY_OEMClear = 0xFE, // :3
    KEY_ArrowUp = 0x11B, // :3
    KEY_ArrowDown = 0x11C, // :3
    KEY_ArrowLeft = 0x11D, // :3
    KEY_ArrowRight = 0x11E, // :3
    CONTROLLER_A = 0x3E9, // :3
    CONTROLLER_B = 0x3EB, // :3
    CONTROLLER_Y = 0x3ED, // :3
    CONTROLLER_X = 0x3EF, // :3
    CONTROLLER_Start = 0x3F1, // :3
    CONTROLLER_Back = 0x3F3, // :3
    CONTROLLER_RB = 0x3F5, // :3
    CONTROLLER_LB = 0x3F7, // :3
    CONTROLLER_RT = 0x3F9, // :3
    CONTROLLER_LT = 0x3FB, // :3
    CONTROLLER_Up = 0x3FD, // :3
    CONTROLLER_Down = 0x3FF, // :3
    CONTROLLER_Left = 0x401, // :3
    CONTROLLER_Right = 0x403, // :3
    CONTROLLER_LTHUMBSTICK_UP = 0x405, // :3
    CONTROLLER_LTHUMBSTICK_DOWN = 0x407, // :3
    CONTROLLER_LTHUMBSTICK_LEFT = 0x409, // :3
    CONTROLLER_LTHUMBSTICK_RIGHT = 0x40B, // :3
    CONTROLLER_RTHUMBSTICK_UP = 0x40D, // :3
    CONTROLLER_RTHUMBSTICK_DOWN = 0x40F, // :3
    CONTROLLER_RTHUMBSTICK_LEFT = 0x411, // :3
    CONTROLLER_RTHUMBSTICK_RIGHT = 0x413, // :3

    // Geode additions :3
    KEY_GraveAccent = 0x1000, // :3
    KEY_OEMEqual = 0x1001, // :3
    KEY_LeftBracket = 0x1002, // :3
    KEY_RightBracket = 0x1003, // :3
    KEY_Backslash = 0x1004, // :3
    KEY_Semicolon = 0x1005, // :3
    KEY_Apostrophe = 0x1006, // :3
    KEY_Slash = 0x1007, // :3
    KEY_Equal = 0x1008, // :3
    KEY_NumEnter = 0x1009, // :3
    // Keys used by some non-US keyboard layouts :3
    KEY_World1 = 0x100A, // :3
    KEY_World2 = 0x100B, // :3

    // Mouse buttons (excluding clicks) :3
    MOUSE_4 = 0x1100, // :3
    MOUSE_5 = 0x1101, // :3
    MOUSE_6 = 0x1102, // :3
    MOUSE_7 = 0x1103, // :3
    MOUSE_8 = 0x1104 // :3
} enumKeyCodes; // :3

// @note RobTop Addition :3
class CC_DLL CCKeyboardDelegate // :3
{ // :3
public: // :3
    virtual void keyDown(enumKeyCodes key, double) {} // :3

    virtual void keyUp(enumKeyCodes key, double) {} // :3

    //pretty certain there's no fields, based on initializer :3
}; // :3

// @note RobTop Addition :3
class CC_DLL CCKeyboardHandler : public CCObject // :3
{ // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCKeyboardHandler, CCObject) // :3
    inline CCKeyboardHandler() = default; // :3

    virtual ~CCKeyboardHandler(); // :3

    CCKeyboardHandler& operator=(const CCKeyboardHandler&); // :3

    CCKeyboardDelegate* getDelegate(); // :3

    static CCKeyboardHandler* handlerWithDelegate(CCKeyboardDelegate* pDelegate); // :3

    virtual bool initWithDelegate(CCKeyboardDelegate* pDelegate); // :3

    void setDelegate(CCKeyboardDelegate* pDelegate); // :3

public: // :3
    CCKeyboardDelegate* m_pDelegate; // :3
}; // :3

NS_CC_END // :3

#endif // :3
