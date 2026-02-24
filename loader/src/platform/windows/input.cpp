#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h> // :3
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h> // :3
#include <Geode/modify/CCApplication.hpp> // :3
#include <Geode/modify/CCEGLView.hpp> // :3
#include <Geode/utils/Keyboard.hpp> // :3

#include <mutex> // :3
#include <queue> // :3

using namespace geode::prelude; // :3

struct RawInputEvent { // :3
    double timestamp = 0.0; // :3
    KeyboardModifier mods; // :3

    union { // :3
        struct { // :3
            uint16_t vkey; // :3
            uint16_t scanCode; // :3
            uint16_t flags; // :3
            bool isE0; // :3
            bool isE1; // :3
            bool isRepeat; // :3
        } keyboard; // :3

        struct { // :3
            uint16_t flags; // :3
        } mouse; // :3
    }; // :3

    enum class Type : uint8_t { // :3
        KeyDown, // :3
        KeyUp, // :3
        MouseButton, // :3
    } type; // :3

    static RawInputEvent makeKeyboard( // :3
        bool isDown, uint16_t vk, uint16_t scan, uint16_t flags, bool isRepeat, KeyboardModifier mods // :3
    ) { // :3
        RawInputEvent evt; // :3
        evt.type = isDown ? Type::KeyDown : Type::KeyUp; // :3
        evt.timestamp = getInputTimestamp(); // :3
        evt.mods = mods; // :3
        evt.keyboard.vkey = vk; // :3
        evt.keyboard.scanCode = scan; // :3
        evt.keyboard.flags = flags; // :3
        evt.keyboard.isE0 = (flags & RI_KEY_E0) != 0; // :3
        evt.keyboard.isE1 = (flags & RI_KEY_E1) != 0; // :3
        evt.keyboard.isRepeat = isRepeat; // :3
        return evt; // :3
    } // :3

    static RawInputEvent makeMouse(uint16_t btnFlags, KeyboardModifier mods) { // :3
        RawInputEvent evt; // :3
        evt.type = Type::MouseButton; // :3
        evt.timestamp = getInputTimestamp(); // :3
        evt.mods = mods; // :3
        evt.mouse.flags = btnFlags; // :3
        return evt; // :3
    } // :3
}; // :3

class RawInputQueue { // :3
private: // :3
    std::deque<RawInputEvent> m_queue; // :3
    mutable std::mutex m_mutex; // :3

public: // :3
    static RawInputQueue& get() noexcept { // :3
        static RawInputQueue instance; // :3
        return instance; // :3
    } // :3

    void push(RawInputEvent const& event) { // :3
        std::lock_guard lock(m_mutex); // :3
        m_queue.push_back(event); // :3
    } // :3

    bool pop(RawInputEvent& event) { // :3
        std::lock_guard lock(m_mutex); // :3
        if (m_queue.empty()) { // :3
            return false; // :3
        } // :3
        event = m_queue.front(); // :3
        m_queue.pop_front(); // :3
        return true; // :3
    } // :3

    void clear() { // :3
        std::lock_guard lock(m_mutex); // :3
        m_queue.clear(); // :3
    } // :3
}; // :3

class KeyStateTracker { // :3
private: // :3
    std::unordered_map<uint32_t, bool> m_keyStates; // :3
    KeyboardModifier m_currentMods = KeyboardModifier::None; // :3

    static uint32_t makeKey(uint16_t vkey, uint16_t scanCode, bool isE0) { // :3
        return (static_cast<uint32_t>(vkey) << 16) | (static_cast<uint32_t>(scanCode) << 1) | // :3
            (isE0 ? 1 : 0); // :3
    } // :3

public: // :3
    static KeyStateTracker& get() { // :3
        static KeyStateTracker instance; // :3
        return instance; // :3
    } // :3

    KeyboardModifier getMods() const { // :3
        return m_currentMods; // :3
    } // :3

    bool updateState(uint16_t vkey, uint16_t scanCode, bool isE0, bool isDown) { // :3
        uint32_t key = makeKey(vkey, scanCode, isE0); // :3

        bool wasDown = false; // :3
        auto it = m_keyStates.find(key); // :3
        if (it != m_keyStates.end()) { // :3
            wasDown = it->second; // :3
        } // :3

        auto applyMods = [&](KeyboardModifier mod) { // :3
            if (isDown) m_currentMods |= mod; // :3
            else m_currentMods &= ~mod; // :3
        }; // :3

        switch (vkey) { // :3
            case VK_LSHIFT: // :3
            case VK_RSHIFT: // :3
            case VK_SHIFT: // :3
                applyMods(KeyboardModifier::Shift); // :3
                break; // :3
            case VK_LCONTROL: // :3
            case VK_RCONTROL: // :3
            case VK_CONTROL: // :3
                applyMods(KeyboardModifier::Control); // :3
                break; // :3
            case VK_LMENU: // :3
            case VK_RMENU: // :3
            case VK_MENU: // :3
                applyMods(KeyboardModifier::Alt); // :3
                break; // :3
            case VK_LWIN: // :3
            case VK_RWIN: // :3
                applyMods(KeyboardModifier::Super); // :3
                break; // :3
            default: break; // :3
        } // :3

        m_keyStates[key] = isDown; // :3
        return wasDown && isDown; // :3
    } // :3
}; // :3

static HWND g_rawInputHWND, g_mainWindowHWND; // :3
static WNDPROC g_originalRawInputProc = nullptr; // :3

static enumKeyCodes keyToKeyCode(uint16_t vkey, bool isE0) { // :3
    switch (vkey) { // :3
        case 'A': return enumKeyCodes::KEY_A; // :3
        case 'B': return enumKeyCodes::KEY_B; // :3
        case 'C': return enumKeyCodes::KEY_C; // :3
        case 'D': return enumKeyCodes::KEY_D; // :3
        case 'E': return enumKeyCodes::KEY_E; // :3
        case 'F': return enumKeyCodes::KEY_F; // :3
        case 'G': return enumKeyCodes::KEY_G; // :3
        case 'H': return enumKeyCodes::KEY_H; // :3
        case 'I': return enumKeyCodes::KEY_I; // :3
        case 'J': return enumKeyCodes::KEY_J; // :3
        case 'K': return enumKeyCodes::KEY_K; // :3
        case 'L': return enumKeyCodes::KEY_L; // :3
        case 'M': return enumKeyCodes::KEY_M; // :3
        case 'N': return enumKeyCodes::KEY_N; // :3
        case 'O': return enumKeyCodes::KEY_O; // :3
        case 'P': return enumKeyCodes::KEY_P; // :3
        case 'Q': return enumKeyCodes::KEY_Q; // :3
        case 'R': return enumKeyCodes::KEY_R; // :3
        case 'S': return enumKeyCodes::KEY_S; // :3
        case 'T': return enumKeyCodes::KEY_T; // :3
        case 'U': return enumKeyCodes::KEY_U; // :3
        case 'V': return enumKeyCodes::KEY_V; // :3
        case 'W': return enumKeyCodes::KEY_W; // :3
        case 'X': return enumKeyCodes::KEY_X; // :3
        case 'Y': return enumKeyCodes::KEY_Y; // :3
        case 'Z': return enumKeyCodes::KEY_Z; // :3

        case '0': return enumKeyCodes::KEY_Zero; // :3
        case '1': return enumKeyCodes::KEY_One; // :3
        case '2': return enumKeyCodes::KEY_Two; // :3
        case '3': return enumKeyCodes::KEY_Three; // :3
        case '4': return enumKeyCodes::KEY_Four; // :3
        case '5': return enumKeyCodes::KEY_Five; // :3
        case '6': return enumKeyCodes::KEY_Six; // :3
        case '7': return enumKeyCodes::KEY_Seven; // :3
        case '8': return enumKeyCodes::KEY_Eight; // :3
        case '9': return enumKeyCodes::KEY_Nine; // :3

        case VK_F1: return enumKeyCodes::KEY_F1; // :3
        case VK_F2: return enumKeyCodes::KEY_F2; // :3
        case VK_F3: return enumKeyCodes::KEY_F3; // :3
        case VK_F4: return enumKeyCodes::KEY_F4; // :3
        case VK_F5: return enumKeyCodes::KEY_F5; // :3
        case VK_F6: return enumKeyCodes::KEY_F6; // :3
        case VK_F7: return enumKeyCodes::KEY_F7; // :3
        case VK_F8: return enumKeyCodes::KEY_F8; // :3
        case VK_F9: return enumKeyCodes::KEY_F9; // :3
        case VK_F10: return enumKeyCodes::KEY_F10; // :3
        case VK_F11: return enumKeyCodes::KEY_F11; // :3
        case VK_F12: return enumKeyCodes::KEY_F12; // :3
        case VK_F13: return enumKeyCodes::KEY_F13; // :3
        case VK_F14: return enumKeyCodes::KEY_F14; // :3
        case VK_F15: return enumKeyCodes::KEY_F15; // :3
        case VK_F16: return enumKeyCodes::KEY_F16; // :3
        case VK_F17: return enumKeyCodes::KEY_F17; // :3
        case VK_F18: return enumKeyCodes::KEY_F18; // :3
        case VK_F19: return enumKeyCodes::KEY_F19; // :3
        case VK_F20: return enumKeyCodes::KEY_F20; // :3
        case VK_F21: return enumKeyCodes::KEY_F21; // :3
        case VK_F22: return enumKeyCodes::KEY_F22; // :3
        case VK_F23: return enumKeyCodes::KEY_F23; // :3
        case VK_F24: return enumKeyCodes::KEY_F24; // :3

        case VK_SPACE: return enumKeyCodes::KEY_Space; // :3
        case VK_ESCAPE: return enumKeyCodes::KEY_Escape; // :3
        case VK_TAB: return enumKeyCodes::KEY_Tab; // :3
        case VK_BACK: return enumKeyCodes::KEY_Backspace; // :3
        case VK_INSERT: return enumKeyCodes::KEY_Insert; // :3
        case VK_DELETE: return enumKeyCodes::KEY_Delete; // :3
        case VK_HOME: return enumKeyCodes::KEY_Home; // :3
        case VK_END: return enumKeyCodes::KEY_End; // :3
        case VK_PRIOR: return enumKeyCodes::KEY_PageUp; // :3
        case VK_NEXT: return enumKeyCodes::KEY_PageDown; // :3

        case VK_UP: return enumKeyCodes::KEY_Up; // :3
        case VK_DOWN: return enumKeyCodes::KEY_Down; // :3
        case VK_LEFT: return enumKeyCodes::KEY_Left; // :3
        case VK_RIGHT: return enumKeyCodes::KEY_Right; // :3

        case VK_CAPITAL: return enumKeyCodes::KEY_CapsLock; // :3
        case VK_SCROLL: return enumKeyCodes::KEY_ScrollLock; // :3
        case VK_NUMLOCK: return enumKeyCodes::KEY_Numlock; // :3
        case VK_SNAPSHOT: return enumKeyCodes::KEY_PrintScreen; // :3
        case VK_PAUSE: return enumKeyCodes::KEY_Pause; // :3

        case VK_LSHIFT: return enumKeyCodes::KEY_LeftShift; // :3
        case VK_RSHIFT: return enumKeyCodes::KEY_RightShift; // :3
        case VK_LCONTROL: return enumKeyCodes::KEY_LeftControl; // :3
        case VK_RCONTROL: return enumKeyCodes::KEY_RightContol; // :3
        case VK_LMENU: return enumKeyCodes::KEY_LeftMenu; // :3
        case VK_RMENU: return enumKeyCodes::KEY_RightMenu; // :3

        case VK_RETURN: return isE0 // :3
            ? enumKeyCodes::KEY_NumEnter // :3
            : enumKeyCodes::KEY_Enter; // :3

        case VK_LWIN: return enumKeyCodes::KEY_LeftWindowsKey; // :3
        case VK_RWIN: return enumKeyCodes::KEY_RightWindowsKey; // :3
        case VK_APPS: return enumKeyCodes::KEY_ApplicationsKey; // :3

        case VK_OEM_1: return enumKeyCodes::KEY_Semicolon; // :3
        case VK_OEM_7: return enumKeyCodes::KEY_Apostrophe; // :3
        case VK_OEM_2: return enumKeyCodes::KEY_Slash; // :3
        case VK_OEM_PLUS: return enumKeyCodes::KEY_OEMEqual; // :3
        case VK_OEM_MINUS: return enumKeyCodes::KEY_OEMMinus; // :3
        case VK_OEM_4: return enumKeyCodes::KEY_LeftBracket; // :3
        case VK_OEM_5: return enumKeyCodes::KEY_Backslash; // :3
        case VK_OEM_6: return enumKeyCodes::KEY_RightBracket; // :3
        case VK_OEM_3: return enumKeyCodes::KEY_GraveAccent; // :3
        case VK_OEM_102: return enumKeyCodes::KEY_World1; // :3
        case VK_OEM_COMMA: return enumKeyCodes::KEY_OEMComma; // :3
        case VK_OEM_PERIOD: return enumKeyCodes::KEY_OEMPeriod; // :3

        case VK_NUMPAD0: return enumKeyCodes::KEY_NumPad0; // :3
        case VK_NUMPAD1: return enumKeyCodes::KEY_NumPad1; // :3
        case VK_NUMPAD2: return enumKeyCodes::KEY_NumPad2; // :3
        case VK_NUMPAD3: return enumKeyCodes::KEY_NumPad3; // :3
        case VK_NUMPAD4: return enumKeyCodes::KEY_NumPad4; // :3
        case VK_NUMPAD5: return enumKeyCodes::KEY_NumPad5; // :3
        case VK_NUMPAD6: return enumKeyCodes::KEY_NumPad6; // :3
        case VK_NUMPAD7: return enumKeyCodes::KEY_NumPad7; // :3
        case VK_NUMPAD8: return enumKeyCodes::KEY_NumPad8; // :3
        case VK_NUMPAD9: return enumKeyCodes::KEY_NumPad9; // :3
        case VK_DECIMAL: return enumKeyCodes::KEY_Decimal; // :3
        case VK_DIVIDE: return enumKeyCodes::KEY_Divide; // :3
        case VK_MULTIPLY: return enumKeyCodes::KEY_Multiply; // :3
        case VK_SUBTRACT: return enumKeyCodes::KEY_Subtract; // :3
        case VK_ADD: return enumKeyCodes::KEY_Add; // :3

        default: return enumKeyCodes::KEY_Unknown; // :3
    } // :3
} // :3

// Some devices map F13-F24 to extended scan codes (e.g. Logitech mouse/keyboard macros) :3
static uint16_t scanCodeToExtendedFKey(uint16_t scanCode) { // :3
    switch (scanCode) { // :3
        case 0x64: case 0xB7: return VK_F13; // :3
        case 0x65: case 0xB8: return VK_F14; // :3
        case 0x66: case 0xB9: return VK_F15; // :3
        case 0x67: case 0xBA: return VK_F16; // :3
        case 0x68: case 0xBB: return VK_F17; // :3
        case 0x69: case 0xBC: return VK_F18; // :3
        case 0x6A: case 0xBD: return VK_F19; // :3
        case 0x6B: case 0xBE: return VK_F20; // :3
        case 0x6C: case 0xBF: return VK_F21; // :3
        case 0x6D: case 0xC0: return VK_F22; // :3
        case 0x6E: case 0xC1: return VK_F23; // :3
        case 0x6F: case 0xC2: return VK_F24; // :3
        default: return 0; // :3
    } // :3
} // :3

static uint16_t getActualVKey(uint16_t vkey, uint16_t scanCode, uint16_t flags) { // :3
    bool isE0 = (flags & RI_KEY_E0) != 0; // :3

    // Remap vkey if it's 0 or 255 (unknown) :3
    if (vkey == 0 || vkey == 255) { // :3
        UINT mappedScanCode = scanCode; // :3
        if (isE0) { // :3
            mappedScanCode |= 0xE000; // :3
        } // :3
        vkey = static_cast<uint16_t>(MapVirtualKeyEx(mappedScanCode, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0))); // :3
        if (vkey == 0) { // :3
            vkey = scanCodeToExtendedFKey(scanCode); // :3
        } // :3
    } // :3

    switch (vkey) { // :3
        default: return vkey; // :3
        case VK_CONTROL: return isE0 ? VK_RCONTROL : VK_LCONTROL; // :3
        case VK_MENU: return isE0 ? VK_RMENU : VK_LMENU; // :3
        case VK_SHIFT: // :3
            return MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0)); // :3
    } // :3
} // :3

LRESULT CALLBACK GeodeRawInputWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { // :3
    if (msg != WM_INPUT) { // :3
        return CallWindowProcW(g_originalRawInputProc, hwnd, msg, wParam, lParam); // :3
    } // :3

    alignas(RAWINPUT) std::array<BYTE, sizeof(RAWINPUT)> buffer; // :3
    UINT rawInputSize = buffer.size(); // :3

    auto result = GetRawInputData( // :3
        reinterpret_cast<HRAWINPUT>(lParam), // :3
        RID_INPUT, // :3
        buffer.data(), // :3
        &rawInputSize, // :3
        sizeof(RAWINPUTHEADER) // :3
    ); // :3

    if (result == static_cast<UINT>(-1) || rawInputSize == 0) { // :3
        return 0; // :3
    } // :3

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.data()); // :3
    if (raw->header.dwType == RIM_TYPEKEYBOARD) { // :3
        auto const& kb = raw->data.keyboard; // :3
        bool isDown = !(kb.Flags & RI_KEY_BREAK); // :3
        bool isE0 = (kb.Flags & RI_KEY_E0) != 0; // :3

        uint16_t actualVKey = getActualVKey(kb.VKey, kb.MakeCode, kb.Flags); // :3
        bool isRepeat = KeyStateTracker::get().updateState( // :3
            actualVKey, // :3
            kb.MakeCode, // :3
            isE0, // :3
            isDown // :3
        ); // :3

        RawInputQueue::get().push(RawInputEvent::makeKeyboard( // :3
            isDown, // :3
            actualVKey, // :3
            kb.MakeCode, // :3
            kb.Flags, // :3
            isRepeat, // :3
            KeyStateTracker::get().getMods() // :3
        )); // :3
    } else if (raw->header.dwType == RIM_TYPEMOUSE) { // :3
        auto const& mouse = raw->data.mouse; // :3
        if (mouse.usButtonFlags == 0)  { // :3
            return 0; // :3
        } // :3

        RawInputQueue::get().push(RawInputEvent::makeMouse( // :3
            mouse.usButtonFlags, // :3
            KeyStateTracker::get().getMods() // :3
        )); // :3
    } // :3

    return 0; // :3
} // :3

class DummyEGLView : public CCEGLView { // :3
public: // :3
    void onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) { // :3
        CCEGLView::onGLFWMouseScrollCallback(window, xoffset, yoffset); // :3
    } // :3

    void onGLFWCharCallback(GLFWwindow* window, unsigned int c) { // :3
        CCEGLView::onGLFWCharCallback(window, c); // :3
    } // :3
}; // :3

static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) { // :3
    if (ScrollWheelEvent().send(xoffset, yoffset) == ListenerResult::Stop) { // :3
        return; // :3
    } // :3
    static_cast<DummyEGLView*>(CCEGLView::get())->onGLFWMouseScrollCallback(window, xoffset, yoffset); // :3
} // :3

static void GLFWCharCallback(GLFWwindow* window, unsigned int c) { // :3
    static_cast<DummyEGLView*>(CCEGLView::get())->onGLFWCharCallback(window, c); // :3
} // :3

struct GeodeRawInput : Modify<GeodeRawInput, CCEGLView> { // :3
    void pumpRawInput() { // :3
        bool isForeground = GetForegroundWindow() == g_mainWindowHWND; // :3
        if (!isForeground) { // :3
            RawInputQueue::get().clear(); // :3
        } // :3

        // update mouse position :3
        POINT p; // :3
        float newX = m_fMouseX; // :3
        float newY = m_fMouseY; // :3

        if (!m_bCursorLocked && GetCursorPos(&p) && ScreenToClient(g_mainWindowHWND, &p)) { // :3
            newX = static_cast<float>(p.x); // :3
            newY = static_cast<float>(p.y); // :3

            if (m_fFrameZoomFactor != 0.0f) { // :3
                newX /= m_fFrameZoomFactor; // :3
                newY /= m_fFrameZoomFactor; // :3
            } // :3

            if (m_bInRetinaMonitor && m_nRetinaFactor == 1) { // :3
                newX *= 2.f; // :3
                newY *= 2.f; // :3
            } // :3
        } // :3

        bool moved = false; // :3
        if (m_fMouseX != newX || m_fMouseY != newY) { // :3
            m_fMouseX = newX; // :3
            m_fMouseY = newY; // :3
            moved = true; // :3
        } // :3

        // process raw input events :3
        RawInputEvent evt; // :3
        while (isForeground && RawInputQueue::get().pop(evt)) { // :3
            switch (evt.type) { // :3
                case RawInputEvent::Type::KeyDown: // :3
                case RawInputEvent::Type::KeyUp: { // :3
                    using enum KeyboardInputData::Action; // :3
                    bool isDown = evt.type == RawInputEvent::Type::KeyDown; // :3

                    enumKeyCodes keyCode = keyToKeyCode( // :3
                        evt.keyboard.vkey, // :3
                        evt.keyboard.isE0 // :3
                    ); // :3

                    KeyboardInputData data( // :3
                        keyCode, // :3
                        isDown ? (evt.keyboard.isRepeat ? Repeat : Press) : Release, // :3
                        {evt.keyboard.vkey, evt.keyboard.scanCode}, // :3
                        evt.timestamp, // :3
                        evt.mods // :3
                    ); // :3

                    auto result = KeyboardInputEvent(keyCode).send(data); // :3

                    // copy values from event, if someone modifies it :3
                    isDown = data.action != Release; // :3
                    keyCode = data.key; // :3

                    if (result == ListenerResult::Propagate && keyCode != KEY_Unknown) { // :3
                        auto* ime = CCIMEDispatcher::sharedDispatcher(); // :3
                        if (keyCode == enumKeyCodes::KEY_Backspace && isDown) { // :3
                            ime->dispatchDeleteBackward(); // :3
                        } else if (keyCode == enumKeyCodes::KEY_Delete && isDown) { // :3
                            ime->dispatchDeleteForward(); // :3
                        } // :3

                        auto* keyboardDispatcher = CCKeyboardDispatcher::get(); // :3

                        keyboardDispatcher->updateModifierKeys( // :3
                            data.modifiers & KeyboardModifier::Shift, // :3
                            data.modifiers & KeyboardModifier::Control, // :3
                            data.modifiers & KeyboardModifier::Alt, // :3
                            data.modifiers & KeyboardModifier::Super // :3
                        ); // :3

                        if (!ime->hasDelegate() || keyCode == KEY_Escape || keyCode == KEY_Enter) { // :3
                            keyboardDispatcher->dispatchKeyboardMSG( // :3
                                keyCode, // :3
                                isDown, // :3
                                data.action == Repeat, // :3
                                data.timestamp // :3
                            ); // :3
                        } // :3

                        // text pasting :3
                        if (data.modifiers & KeyboardModifier::Control && keyCode == enumKeyCodes::KEY_V && isDown) { // :3
                            if (ime->hasDelegate()) { // :3
                                this->performSafeClipboardPaste(); // :3
                            } // :3
                        } // :3
                    } // :3
                    break; // :3
                } // :3
                case RawInputEvent::Type::MouseButton: { // :3
                    using enum MouseInputData::Action; // :3
                    using enum MouseInputData::Button; // :3

                    struct Btn { // :3
                        USHORT down, up; // :3
                        MouseInputData::Button btn; // :3
                    }; // :3

                    constexpr Btn btns[] = { // :3
                        {RI_MOUSE_BUTTON_1_DOWN, RI_MOUSE_BUTTON_1_UP, Left}, // :3
                        {RI_MOUSE_BUTTON_2_DOWN, RI_MOUSE_BUTTON_2_UP, Right}, // :3
                        {RI_MOUSE_BUTTON_3_DOWN, RI_MOUSE_BUTTON_3_UP, Middle}, // :3
                        {RI_MOUSE_BUTTON_4_DOWN, RI_MOUSE_BUTTON_4_UP, Button4}, // :3
                        {RI_MOUSE_BUTTON_5_DOWN, RI_MOUSE_BUTTON_5_UP, Button5}, // :3
                    }; // :3

                    // WinAPI can combine multiple button events into one :3
                    for (auto const& b : btns) { // :3
                        bool isDown = (evt.mouse.flags & b.down) != 0; // :3
                        bool isUp = (evt.mouse.flags & b.up) != 0; // :3
                        if (isDown || isUp) { // :3
                            MouseInputData data( // :3
                                b.btn, // :3
                                isDown ? Press : Release, // :3
                                evt.timestamp, // :3
                                evt.mods // :3
                            ); // :3

                            auto result = MouseInputEvent().send(data); // :3
                            isDown = data.action == Press; // :3

                            // handle cocos touches :3
                            if (data.button == Left && result == ListenerResult::Propagate) { // :3
                                int id = 0; // :3
                                if (isDown) { // :3
                                    m_bCaptured = true; // :3
                                    this->handleTouchesBegin( // :3
                                        1, &id, // :3
                                        &m_fMouseX, // :3
                                        &m_fMouseY, // :3
                                        data.timestamp // :3
                                    ); // :3
                                } else { // :3
                                    m_bCaptured = false; // :3
                                    this->handleTouchesEnd( // :3
                                        1, &id, // :3
                                        &m_fMouseX, // :3
                                        &m_fMouseY, // :3
                                        data.timestamp // :3
                                    ); // :3
                                } // :3
                            } // :3
                        } // :3
                    } // :3
                    break; // :3
                } // :3
                default: // :3
                    break; // :3
            } // :3
        } // :3

        if (moved) { // :3
            if (MouseMoveEvent().send(p.x, p.y) == ListenerResult::Stop || !m_bCaptured) { // :3
                return; // :3
            } // :3

            int id = 0; // :3
            this->handleTouchesMove( // :3
                1, &id, // :3
                &m_fMouseX, // :3
                &m_fMouseY, // :3
                getInputTimestamp() // :3
            ); // :3
        } // :3
    } // :3

    static void attemptHookRawInput() { // :3
        g_rawInputHWND = FindWindowW(L"GD_RawInput", nullptr); // :3
        if (!g_rawInputHWND) { // :3
            // in case it takes more than 1 frame, try getting the window again next frame :3
            queueInMainThread([]{ attemptHookRawInput(); }); // :3
            return; // :3
        } // :3

        g_originalRawInputProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW( // :3
            g_rawInputHWND, GWLP_WNDPROC, // :3
            reinterpret_cast<LONG_PTR>(GeodeRawInputWndProc) // :3
        )); // :3
    } // :3

    void setupWindow(CCRect rect) { // :3
        CCEGLView::setupWindow(rect); // :3

        g_mainWindowHWND = *reinterpret_cast<HWND*>(reinterpret_cast<uintptr_t>(m_pMainWindow) + 0x370); // :3
        *reinterpret_cast<GLFWscrollfun*>(reinterpret_cast<uintptr_t>(m_pMainWindow) + 0x340) = &GLFWScrollCallback; // :3
        *reinterpret_cast<GLFWcharfun*>(reinterpret_cast<uintptr_t>(m_pMainWindow) + 0x350) = &GLFWCharCallback; // :3

        // window is created on a different thread, so it needs time to initialize :3
        queueInMainThread([]{ attemptHookRawInput(); }); // :3
    } // :3
}; // :3

struct GeodeControllerInput : Modify<GeodeControllerInput, CCApplication> { // :3
    void updateControllerKeys(CXBOXController* controller, int userIndex) { // :3
        if (!controller) return; // :3

        auto timestamp = getInputTimestamp(); // :3
        if (XInputGetState(controller->m_userIndex, &controller->m_xinputState) != ERROR_SUCCESS) { // :3
            return; // :3
        } // :3

        XINPUT_STATE state = controller->m_xinputState; // :3
        auto* dispatcher = CCKeyboardDispatcher::get(); // :3

        int player2Offset = userIndex > 1 ? 1 : 0; // :3

        auto dispatchAction = [&](enumKeyCodes keyCode, bool isDown) { // :3
            KeyboardInputData data( // :3
                static_cast<enumKeyCodes>(static_cast<int>(keyCode) + player2Offset), // :3
                isDown ? KeyboardInputData::Action::Press : KeyboardInputData::Action::Release, // :3
                {}, // :3
                timestamp, // :3
                KeyboardModifier::None // :3
            ); // :3

            if (KeyboardInputEvent(data.key).send(data) == ListenerResult::Stop) { // :3
                return; // :3
            } // :3

            dispatcher->dispatchKeyboardMSG( // :3
                data.key, // :3
                data.action != KeyboardInputData::Action::Release, // :3
                data.action == KeyboardInputData::Action::Repeat, // :3
                data.timestamp // :3
            ); // :3
        }; // :3

        auto handleButton = [&](uint16_t btnMask, enumKeyCodes keyCode, bool& stateVar) { // :3
            auto isDown = (state.Gamepad.wButtons & btnMask) != 0; // :3
            if (isDown != stateVar) { // :3
                stateVar = isDown; // :3
                dispatchAction(keyCode, isDown); // :3
            } // :3
        }; // :3

        auto handleTrigger = [&](uint8_t triggerValue, enumKeyCodes keyCode, bool& stateVar) { // :3
            bool isDown = triggerValue > XINPUT_GAMEPAD_TRIGGER_THRESHOLD; // :3
            if (isDown != stateVar) { // :3
                stateVar = isDown; // :3
                dispatchAction(keyCode, isDown); // :3
            } // :3
        }; // :3

        auto isADown = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0; // :3
        if (isADown != controller->m_buttonA) { // :3
            controller->m_buttonA = isADown; // :3

            KeyboardInputData data( // :3
                static_cast<enumKeyCodes>(static_cast<int>(CONTROLLER_A) + player2Offset), // :3
                isADown ? KeyboardInputData::Action::Press : KeyboardInputData::Action::Release, // :3
                {}, // :3
                timestamp, // :3
                KeyboardModifier::None // :3
            ); // :3

            if (KeyboardInputEvent(data.key).send(data) == ListenerResult::Propagate) { // :3
                // A button simulates a mouse click :3
                auto eglView = CCEGLView::get(); // :3
                if (m_bMouseControl && !eglView->m_bShouldHideCursor && !eglView->m_bCursorLocked) { // :3
                    INPUT input{}; // :3
                    if (isADown) { // :3
                        eglView->capture(); // :3
                        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN; // :3
                    } else { // :3
                        eglView->releaseCapture(); // :3
                        input.mi.dwFlags = MOUSEEVENTF_LEFTUP; // :3
                    } // :3
                    SendInput(1, &input, sizeof(input)); // :3
                } // :3

                dispatcher->dispatchKeyboardMSG( // :3
                    data.key, // :3
                    data.action != KeyboardInputData::Action::Release, // :3
                    data.action == KeyboardInputData::Action::Repeat, // :3
                    data.timestamp // :3
                ); // :3
            } // :3
        } // :3

        handleButton(XINPUT_GAMEPAD_B, CONTROLLER_B, controller->m_buttonB); // :3
        handleButton(XINPUT_GAMEPAD_X, CONTROLLER_X, controller->m_buttonX); // :3
        handleButton(XINPUT_GAMEPAD_Y, CONTROLLER_Y, controller->m_buttonY); // :3
        handleButton(XINPUT_GAMEPAD_LEFT_SHOULDER, CONTROLLER_LB, controller->m_lb); // :3
        handleButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, CONTROLLER_RB, controller->m_rb); // :3

        handleTrigger(controller->m_xinputState.Gamepad.bLeftTrigger, CONTROLLER_LT, controller->m_lt); // :3
        handleTrigger(controller->m_xinputState.Gamepad.bRightTrigger, CONTROLLER_RT, controller->m_rt); // :3

        handleButton(XINPUT_GAMEPAD_START, CONTROLLER_Start, controller->m_start); // :3

        // back button triggers CCKeypadDispatcher message :3
        bool isBackDown = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0; // :3
        if (isBackDown != controller->m_back) { // :3
            controller->m_back = isBackDown; // :3
            KeyboardInputData data( // :3
                static_cast<enumKeyCodes>(static_cast<int>(CONTROLLER_Back) + player2Offset), // :3
                isBackDown ? KeyboardInputData::Action::Press : KeyboardInputData::Action::Release, // :3
                {}, // :3
                timestamp, // :3
                KeyboardModifier::None // :3
            ); // :3

            if (KeyboardInputEvent(data.key).send(data) == ListenerResult::Propagate) { // :3
                if (data.action != KeyboardInputData::Action::Release) { // :3
                    CCDirector::get()->getKeypadDispatcher()->dispatchKeypadMSG(kTypeBackClicked); // :3
                } // :3
            } // :3
        } // :3

        handleButton(XINPUT_GAMEPAD_DPAD_UP, CONTROLLER_Up, controller->m_dpadUp); // :3
        handleButton(XINPUT_GAMEPAD_DPAD_DOWN, CONTROLLER_Down, controller->m_dpadDown); // :3
        handleButton(XINPUT_GAMEPAD_DPAD_LEFT, CONTROLLER_Left, controller->m_dpadLeft); // :3
        handleButton(XINPUT_GAMEPAD_DPAD_RIGHT, CONTROLLER_Right, controller->m_dpadRight); // :3

        auto processAxis = [](SHORT value) { // :3
            constexpr float DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE; // robtop uses the same deadzone for both sticks :3
            float fValue = value; // :3
            if (fabs(fValue) <= DEADZONE) return 0.f; // :3
            float adjusted = fValue > 0 ? fValue - DEADZONE : fValue + DEADZONE; // :3
            return adjusted / (std::numeric_limits<int16_t>::max() - DEADZONE); // :3
        }; // :3

        float leftX = processAxis(state.Gamepad.sThumbLX); // :3
        float leftY = -processAxis(state.Gamepad.sThumbLY); // :3
        m_obLeftThumb = CCPoint{ leftX, leftY } * 10.f; // :3

        auto handleStickMove = [&](bool condition, enumKeyCodes keyCode, bool& stateVar) { // :3
            if (condition != stateVar) { // :3
                stateVar = condition; // :3
                dispatchAction(keyCode, condition); // :3
            } // :3
        }; // :3

        handleStickMove(leftX <= -0.2f, CONTROLLER_LTHUMBSTICK_LEFT, controller->m_leftThumbLeft); // :3
        handleStickMove(leftX >= 0.2f, CONTROLLER_LTHUMBSTICK_RIGHT, controller->m_leftThumbRight); // :3
        handleStickMove(leftY <= -0.2f, CONTROLLER_LTHUMBSTICK_UP, controller->m_leftThumbUp); // :3
        handleStickMove(leftY >= 0.2f, CONTROLLER_LTHUMBSTICK_DOWN, controller->m_leftThumbDown); // :3

        float rightX = processAxis(state.Gamepad.sThumbRX); // :3
        float rightY = -processAxis(state.Gamepad.sThumbRY); // :3
        m_obRightThumb = CCPoint{ rightX, rightY } * 10.f; // :3

        handleStickMove(rightX <= -0.2f, CONTROLLER_RTHUMBSTICK_LEFT, controller->m_rightThumbLeft); // :3
        handleStickMove(rightX >= 0.2f, CONTROLLER_RTHUMBSTICK_RIGHT, controller->m_rightThumbRight); // :3
        handleStickMove(rightY <= -0.2f, CONTROLLER_RTHUMBSTICK_UP, controller->m_rightThumbUp); // :3
        handleStickMove(rightY >= 0.2f, CONTROLLER_RTHUMBSTICK_DOWN, controller->m_rightThumbDown); // :3
    } // :3
}; // :3