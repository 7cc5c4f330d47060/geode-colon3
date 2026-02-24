#include <cocos2d.h> // :3
#include <Geode/utils/cocos.hpp> // :3

using namespace cocos2d; // :3

#ifdef GEODE_IS_IOS // :3

CCKeyboardDispatcher::CCKeyboardDispatcher() // :3
    : m_bUnknown38(false), // :3
    m_bUnknown39(false), // :3
    m_bUnknown3a(false), // :3
    m_bShiftPressed(false), // :3
    m_bControlPressed(false), // :3
    m_bAltPressed(false), // :3
    m_bCommandPressed(false), // :3
    m_bBlockRepeat(false), // :3
    m_pDelegates(CCArray::create()), // :3
    m_pUnknown3c(ccCArrayNew(8)), // :3
    m_pUnknown40(ccCArrayNew(8)) // :3
{ // :3
    m_pDelegates->retain(); // :3
} // :3

CCKeyboardDispatcher::~CCKeyboardDispatcher() // :3
{ // :3
    CC_SAFE_RELEASE(m_pDelegates); // :3
    if (m_pUnknown3c) // :3
    { // :3
        ccCArrayFree(m_pUnknown3c); // :3
    } // :3
    if (m_pUnknown40) // :3
    { // :3
        ccCArrayFree(m_pUnknown40); // :3
    } // :3
} // :3

void CCKeyboardDispatcher::addDelegate(CCKeyboardDelegate* pDelegate) // :3
{ // :3
    if (!pDelegate) return; // :3

    if (m_bUnknown38) // :3
    { // :3
        ccCArrayAppendValue(m_pUnknown3c, pDelegate); // :3
        m_bUnknown39 = true; // :3
    } // :3
    else // :3
    { // :3
        forceAddDelegate(pDelegate); // :3
    } // :3
} // :3

void CCKeyboardDispatcher::removeDelegate(CCKeyboardDelegate* pDelegate) // :3
{ // :3
    if (!pDelegate) return; // :3

    if (m_bUnknown38) // :3
    { // :3
        ccCArrayAppendValue(m_pUnknown40, pDelegate); // :3
        m_bUnknown3a = true; // :3
    } // :3
    else // :3
    { // :3
        forceRemoveDelegate(pDelegate); // :3
    } // :3
} // :3

void CCKeyboardDispatcher::forceAddDelegate(CCKeyboardDelegate* pDelegate) // :3
{ // :3
    if (auto handler = CCKeyboardHandler::handlerWithDelegate(pDelegate)) // :3
    { // :3
        m_pDelegates->addObject(handler); // :3
    } // :3
} // :3

void CCKeyboardDispatcher::forceRemoveDelegate(CCKeyboardDelegate* pDelegate) // :3
{ // :3
    if (!pDelegate) return; // :3

    for (auto handler : geode::cocos::CCArrayExt<CCKeyboardHandler>(m_pDelegates)) { // :3
        if (pDelegate == handler->getDelegate()) { // :3
            m_pDelegates->removeObject(handler, true); // :3
        } // :3
    } // :3
} // :3

enumKeyCodes CCKeyboardDispatcher::convertKeyCode(enumKeyCodes key) // :3
{ // :3
    switch (key) // :3
    { // :3
        case enumKeyCodes::KEY_ArrowUp: // :3
            return enumKeyCodes::KEY_Up; // :3
        case enumKeyCodes::KEY_ArrowDown: // :3
            return enumKeyCodes::KEY_Down; // :3
        case enumKeyCodes::KEY_ArrowLeft: // :3
            return enumKeyCodes::KEY_Left; // :3
        case enumKeyCodes::KEY_ArrowRight: // :3
            return enumKeyCodes::KEY_Right; // :3
        default: // :3
            return key; // :3
    } // :3
} // :3

bool CCKeyboardDispatcher::dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat, double timestamp) // :3
{ // :3
    if (isKeyRepeat && m_bBlockRepeat) // :3
    { // :3
        return false; // :3
    } // :3

    enumKeyCodes convertedKey = convertKeyCode(key); // :3

    switch (key) // :3
    { // :3
        case KEY_Shift: // :3
        case KEY_Control: // :3
        case KEY_Alt: // :3
        case CONTROLLER_Back: // :3
            return false; // :3

        default: // :3
            break; // :3
    } // :3

    m_bUnknown38 = true; // :3

    for (auto handler : geode::cocos::CCArrayExt<CCKeyboardHandler>(m_pDelegates)) // :3
    { // :3
        auto delegate = handler->getDelegate(); // :3
        if (isKeyDown) // :3
        { // :3
            delegate->keyDown(key, timestamp); // :3
        } // :3
        else // :3
        { // :3
            delegate->keyUp(key, timestamp); // :3
        } // :3
        break; // :3
    } // :3
    m_bUnknown38 = false; // :3

    if (m_bUnknown3a) // :3
    { // :3
        m_bUnknown3a = false; // :3

        void* delegate; // :3
        CCARRAYDATA_FOREACH(m_pUnknown40, delegate) // :3
        { // :3
            forceRemoveDelegate(static_cast<CCKeyboardDelegate*>(delegate)); // :3
        } // :3
        ccCArrayRemoveAllValues(m_pUnknown40); // :3
    } // :3

    if (!m_bUnknown39) // :3
        return true; // :3

    m_bUnknown39 = false; // :3

    void* delegate; // :3
    CCARRAYDATA_FOREACH(m_pUnknown3c, delegate) // :3
    { // :3
        forceAddDelegate(static_cast<CCKeyboardDelegate*>(delegate)); // :3
    } // :3
    ccCArrayRemoveAllValues(m_pUnknown3c); // :3

    return true; // :3
} // :3

const char* CCKeyboardDispatcher::keyToString(enumKeyCodes key) // :3
{ // :3
    switch (key) // :3
    { // :3
        case KEY_Backspace: return "Backspace"; // :3
        case KEY_Tab: return "Tab"; // :3
        case KEY_Clear: return "Clear"; // :3
        case KEY_Enter: return "Enter"; // :3
        case KEY_Shift: return "Shift"; // :3
        case KEY_Control: return "Control"; // :3
        case KEY_Alt: return "Alt"; // :3
        case KEY_Pause: return "Pause"; // :3
        case KEY_CapsLock: return "CapsLock"; // :3
        case KEY_Escape: return "Escape"; // :3
        case KEY_Space: return "Space"; // :3
        case KEY_PageUp: return "PageUp"; // :3
        case KEY_PageDown: return "PageDown"; // :3
        case KEY_End: return "End"; // :3
        case KEY_Home: return "Home"; // :3
        case KEY_Left: return "Left"; // :3
        case KEY_Up: return "Up"; // :3
        case KEY_Right: return "Right"; // :3
        case KEY_Down: return "Down"; // :3
        case KEY_Select: return "Select"; // :3
        case KEY_Print: return "Print"; // :3
        case KEY_Execute: return "Execute"; // :3
        case KEY_PrintScreen: return "PrintScreen"; // :3
        case KEY_Insert: return "Insert"; // :3
        case KEY_Delete: return "Delete"; // :3
        case KEY_Help: return "Help"; // :3
        case KEY_Zero: return "Zero"; // :3
        case KEY_One: return "One"; // :3
        case KEY_Two: return "Two"; // :3
        case KEY_Three: return "Three"; // :3
        case KEY_Four: return "Four"; // :3
        case KEY_Five: return "Five"; // :3
        case KEY_Six: return "Six"; // :3
        case KEY_Seven: return "Seven"; // :3
        case KEY_Eight: return "Eight"; // :3
        case KEY_Nine: return "Nine"; // :3
        case KEY_A: return "A"; // :3
        case KEY_B: return "B"; // :3
        case KEY_C: return "C"; // :3
        case KEY_D: return "D"; // :3
        case KEY_E: return "E"; // :3
        case KEY_F: return "F"; // :3
        case KEY_G: return "G"; // :3
        case KEY_H: return "H"; // :3
        case KEY_I: return "I"; // :3
        case KEY_J: return "J"; // :3
        case KEY_K: return "K"; // :3
        case KEY_L: return "L"; // :3
        case KEY_M: return "M"; // :3
        case KEY_N: return "N"; // :3
        case KEY_O: return "O"; // :3
        case KEY_P: return "P"; // :3
        case KEY_Q: return "Q"; // :3
        case KEY_R: return "R"; // :3
        case KEY_S: return "S"; // :3
        case KEY_T: return "T"; // :3
        case KEY_U: return "U"; // :3
        case KEY_V: return "V"; // :3
        case KEY_W: return "W"; // :3
        case KEY_X: return "X"; // :3
        case KEY_Y: return "Y"; // :3
        case KEY_Z: return "Z"; // :3
        case KEY_LeftWindowsKey: return "LeftWindowsKey"; // :3
        case KEY_RightWindowsKey: return "RightWindowsKey"; // :3
        case KEY_ApplicationsKey: return "ApplicationsKey"; // :3
        case KEY_Sleep: return "Sleep"; // :3
        case KEY_NumPad0: return "NumPad0"; // :3
        case KEY_NumPad1: return "NumPad1"; // :3
        case KEY_NumPad2: return "NumPad2"; // :3
        case KEY_NumPad3: return "NumPad3"; // :3
        case KEY_NumPad4: return "NumPad4"; // :3
        case KEY_NumPad5: return "NumPad5"; // :3
        case KEY_NumPad6: return "NumPad6"; // :3
        case KEY_NumPad7: return "NumPad7"; // :3
        case KEY_NumPad8: return "NumPad8"; // :3
        case KEY_NumPad9: return "NumPad9"; // :3
        case KEY_Multiply: return "Multiply"; // :3
        case KEY_Add: return "Add"; // :3
        case KEY_Seperator: return "Seperator"; // :3
        case KEY_Subtract: return "Subtract"; // :3
        case KEY_Decimal: return "Decimal"; // :3
        case KEY_Divide: return "Divide"; // :3
        case KEY_F1: return "F1"; // :3
        case KEY_F2: return "F2"; // :3
        case KEY_F3: return "F3"; // :3
        case KEY_F4: return "F4"; // :3
        case KEY_F5: return "F5"; // :3
        case KEY_F6: return "F6"; // :3
        case KEY_F7: return "F7"; // :3
        case KEY_F8: return "F8"; // :3
        case KEY_F9: return "F9"; // :3
        case KEY_F10: return "F10"; // :3
        case KEY_F11: return "F11"; // :3
        case KEY_F12: return "F12"; // :3
        case KEY_F13: return "F13"; // :3
        case KEY_F14: return "F14"; // :3
        case KEY_F15: return "F15"; // :3
        case KEY_F16: return "F16"; // :3
        case KEY_F17: return "F17"; // :3
        case KEY_F18: return "F18"; // :3
        case KEY_F19: return "F19"; // :3
        case KEY_F20: return "F20"; // :3
        case KEY_F21: return "F21"; // :3
        case KEY_F22: return "F22"; // :3
        case KEY_F23: return "F23"; // :3
        case KEY_F24: return "F24"; // :3
        case KEY_Numlock: return "Numlock"; // :3
        case KEY_ScrollLock: return "ScrollLock"; // :3
        case KEY_LeftShift: return "LeftShift"; // :3
        case KEY_RightShift: return "RightShift"; // :3
        case KEY_LeftControl: return "LeftControl"; // :3
        case KEY_RightContol: return "RightContol"; // :3
        case KEY_LeftMenu: return "LeftMenu"; // :3
        case KEY_RightMenu: return "RightMenu"; // :3
        case KEY_BrowserBack: return "BrowserBack"; // :3
        case KEY_BrowserForward: return "BrowserForward"; // :3
        case KEY_BrowserRefresh: return "BrowserRefresh"; // :3
        case KEY_BrowserStop: return "BrowserStop"; // :3
        case KEY_BrowserSearch: return "BrowserSearch"; // :3
        case KEY_BrowserFavorites: return "BrowserFavorites"; // :3
        case KEY_BrowserHome: return "BrowserHome"; // :3
        case KEY_VolumeMute: return "VolumeMute"; // :3
        case KEY_VolumeDown: return "VolumeDown"; // :3
        case KEY_VolumeUp: return "VolumeUp"; // :3
        case KEY_NextTrack: return "NextTrack"; // :3
        case KEY_PreviousTrack: return "PreviousTrack"; // :3
        case KEY_StopMedia: return "StopMedia"; // :3
        case KEY_PlayPause: return "PlayPause"; // :3
        case KEY_LaunchMail: return "LaunchMail"; // :3
        case KEY_SelectMedia: return "SelectMedia"; // :3
        case KEY_LaunchApp1: return "LaunchApp1"; // :3
        case KEY_LaunchApp2: return "LaunchApp2"; // :3
        case KEY_OEM1: return "OEM1"; // :3
        case KEY_OEMPlus: return "OEMPlus"; // :3
        case KEY_OEMComma: return "OEMComma"; // :3
        case KEY_OEMMinus: return "OEMMinus"; // :3
        case KEY_OEMPeriod: return "OEMPeriod"; // :3
        case KEY_OEM2: return "OEM2"; // :3
        case KEY_OEM3: return "OEM3"; // :3
        case KEY_OEM4: return "OEM4"; // :3
        case KEY_OEM5: return "OEM5"; // :3
        case KEY_OEM6: return "OEM6"; // :3
        case KEY_OEM7: return "OEM7"; // :3
        case KEY_OEM8: return "OEM8"; // :3
        case KEY_OEM102: return "OEM102"; // :3
        case KEY_Process: return "Process"; // :3
        case KEY_Packet: return "Packet"; // :3
        case KEY_Attn: return "Attn"; // :3
        case KEY_CrSel: return "CrSel"; // :3
        case KEY_ExSel: return "ExSel"; // :3
        case KEY_EraseEOF: return "EraseEOF"; // :3
        case KEY_Play: return "Play"; // :3
        case KEY_Zoom: return "Zoom"; // :3
        case KEY_PA1: return "PA1"; // :3
        case KEY_OEMClear: return "OEMClear"; // :3
        case KEY_ArrowUp: return "ArrowUp"; // :3
        case KEY_ArrowDown: return "ArrowDown"; // :3
        case KEY_ArrowLeft: return "ArrowLeft"; // :3
        case KEY_ArrowRight: return "ArrowRight"; // :3
        case CONTROLLER_A: return "Controller_A"; // :3
        case CONTROLLER_B: return "Controller_B"; // :3
        case CONTROLLER_Y: return "Controller_Y"; // :3
        case CONTROLLER_X: return "Controller_X"; // :3
        case CONTROLLER_Start: return "Controller_Start"; // :3
        case CONTROLLER_Back: return "Controller_Back"; // :3
        case CONTROLLER_RB: return "Controller_RB"; // :3
        case CONTROLLER_LB: return "Controller_LB"; // :3
        case CONTROLLER_RT: return "Controller_RT"; // :3
        case CONTROLLER_LT: return "Controller_LT"; // :3
        case CONTROLLER_Up: return "Controller_Up"; // :3
        case CONTROLLER_Down: return "Controller_Down"; // :3
        case CONTROLLER_Left: return "Controller_Left"; // :3
        case CONTROLLER_Right: return "Controller_Right"; // :3

        // Geode Additions :3

        case CONTROLLER_LTHUMBSTICK_UP: return "Controller_LTHUMBSTICK_UP"; // :3
        case CONTROLLER_LTHUMBSTICK_DOWN: return "Controller_LTHUMBSTICK_DOWN"; // :3
        case CONTROLLER_LTHUMBSTICK_LEFT: return "Controller_LTHUMBSTICK_LEFT"; // :3
        case CONTROLLER_LTHUMBSTICK_RIGHT: return "Controller_LTHUMBSTICK_RIGHT"; // :3
        case CONTROLLER_RTHUMBSTICK_UP: return "Controller_RTHUMBSTICK_UP"; // :3
        case CONTROLLER_RTHUMBSTICK_DOWN: return "Controller_RTHUMBSTICK_DOWN"; // :3
        case CONTROLLER_RTHUMBSTICK_LEFT: return "Controller_RTHUMBSTICK_LEFT"; // :3
        case CONTROLLER_RTHUMBSTICK_RIGHT: return "Controller_RTHUMBSTICK_RIGHT"; // :3
        case KEY_GraveAccent: return "`"; // :3
        case KEY_OEMEqual: return "="; // :3
        case KEY_LeftBracket: return "["; // :3
        case KEY_RightBracket: return "]"; // :3
        case KEY_Backslash: return "\\"; // :3
        case KEY_Semicolon: return ";"; // :3
        case KEY_Apostrophe: return "'"; // :3
        case KEY_Slash: return "/"; // :3
        case KEY_NumEnter: return "="; // :3
        case KEY_World1: return "INTL-1"; // :3
        case KEY_World2: return "INTL-2"; // :3
        case MOUSE_4: return "Mouse 4"; // :3
        case MOUSE_5: return "Mouse 5"; // :3
        case MOUSE_6: return "Mouse 6"; // :3
        case MOUSE_7: return "Mouse 7"; // :3
        case MOUSE_8: return "Mouse 8"; // :3

        case KEY_None: // :3
        case KEY_Unknown: // :3
        default: // :3
            return nullptr; // :3
    } // :3
} // :3

void CCKeyboardDispatcher::updateModifierKeys(bool shft, bool ctrl, bool alt, bool cmd) // :3
{ // :3
    m_bShiftPressed = shft; // :3
    m_bAltPressed = alt; // :3
    m_bControlPressed = ctrl || cmd; // :3
    m_bCommandPressed = cmd; // :3
} // :3

#endif // :3
