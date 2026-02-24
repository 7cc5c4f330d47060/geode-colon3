#include <Geode/utils/AndroidEvent.hpp> // :3
#include <Geode/utils/Keyboard.hpp> // :3
#include <Geode/Prelude.hpp> // :3
#include <cocos2d.h> // :3
#include <android/keycodes.h> // :3
#include <jni.h> // :3
#include <unordered_map> // :3

using namespace geode::prelude; // :3

namespace { // :3
    std::unordered_map<int, cocos2d::enumKeyCodes> g_keymap = { // :3
        {AKEYCODE_SPACE, cocos2d::KEY_Space}, // :3
        {AKEYCODE_COMMA, cocos2d::KEY_OEMComma}, // :3
        {AKEYCODE_PLUS, cocos2d::KEY_OEMPlus}, // :3
        {AKEYCODE_MINUS, cocos2d::KEY_OEMMinus}, // :3
        {AKEYCODE_PERIOD, cocos2d::KEY_OEMPeriod}, // :3
        {AKEYCODE_0, cocos2d::KEY_Zero}, // :3
        {AKEYCODE_1, cocos2d::KEY_One}, // :3
        {AKEYCODE_2, cocos2d::KEY_Two}, // :3
        {AKEYCODE_3, cocos2d::KEY_Three}, // :3
        {AKEYCODE_4, cocos2d::KEY_Four}, // :3
        {AKEYCODE_5, cocos2d::KEY_Five}, // :3
        {AKEYCODE_6, cocos2d::KEY_Six}, // :3
        {AKEYCODE_7, cocos2d::KEY_Seven}, // :3
        {AKEYCODE_8, cocos2d::KEY_Eight}, // :3
        {AKEYCODE_9, cocos2d::KEY_Nine}, // :3
        {AKEYCODE_A, cocos2d::KEY_A}, // :3
        {AKEYCODE_B, cocos2d::KEY_B}, // :3
        {AKEYCODE_C, cocos2d::KEY_C}, // :3
        {AKEYCODE_D, cocos2d::KEY_D}, // :3
        {AKEYCODE_E, cocos2d::KEY_E}, // :3
        {AKEYCODE_F, cocos2d::KEY_F}, // :3
        {AKEYCODE_G, cocos2d::KEY_G}, // :3
        {AKEYCODE_H, cocos2d::KEY_H}, // :3
        {AKEYCODE_I, cocos2d::KEY_I}, // :3
        {AKEYCODE_J, cocos2d::KEY_J}, // :3
        {AKEYCODE_K, cocos2d::KEY_K}, // :3
        {AKEYCODE_L, cocos2d::KEY_L}, // :3
        {AKEYCODE_M, cocos2d::KEY_M}, // :3
        {AKEYCODE_N, cocos2d::KEY_N}, // :3
        {AKEYCODE_O, cocos2d::KEY_O}, // :3
        {AKEYCODE_P, cocos2d::KEY_P}, // :3
        {AKEYCODE_Q, cocos2d::KEY_Q}, // :3
        {AKEYCODE_R, cocos2d::KEY_R}, // :3
        {AKEYCODE_S, cocos2d::KEY_S}, // :3
        {AKEYCODE_T, cocos2d::KEY_T}, // :3
        {AKEYCODE_U, cocos2d::KEY_U}, // :3
        {AKEYCODE_V, cocos2d::KEY_V}, // :3
        {AKEYCODE_W, cocos2d::KEY_W}, // :3
        {AKEYCODE_X, cocos2d::KEY_X}, // :3
        {AKEYCODE_Z, cocos2d::KEY_Z}, // :3
        {AKEYCODE_Y, cocos2d::KEY_Y}, // :3

        {AKEYCODE_ESCAPE, cocos2d::KEY_Escape}, // :3
        {AKEYCODE_ENTER, cocos2d::KEY_Enter}, // :3
        {AKEYCODE_DEL, cocos2d::KEY_Backspace}, // :3
        {AKEYCODE_FORWARD_DEL, cocos2d::KEY_Delete}, // :3
        {AKEYCODE_DPAD_LEFT, cocos2d::KEY_Left}, // :3
        {AKEYCODE_DPAD_RIGHT, cocos2d::KEY_Right}, // :3
        {AKEYCODE_DPAD_UP, cocos2d::KEY_Up}, // :3
        {AKEYCODE_DPAD_DOWN, cocos2d::KEY_Down}, // :3
        {AKEYCODE_F1, cocos2d::KEY_F1}, // :3
        {AKEYCODE_F2, cocos2d::KEY_F2}, // :3
        {AKEYCODE_F3, cocos2d::KEY_F3}, // :3
        {AKEYCODE_F4, cocos2d::KEY_F4}, // :3
        {AKEYCODE_F5, cocos2d::KEY_F5}, // :3
        {AKEYCODE_F6, cocos2d::KEY_F6}, // :3
        {AKEYCODE_F7, cocos2d::KEY_F7}, // :3
        {AKEYCODE_F8, cocos2d::KEY_F8}, // :3
        {AKEYCODE_F9, cocos2d::KEY_F9}, // :3
        {AKEYCODE_F10, cocos2d::KEY_F10}, // :3
        {AKEYCODE_F11, cocos2d::KEY_F11}, // :3
        {AKEYCODE_F12, cocos2d::KEY_F12}, // :3
        {AKEYCODE_SHIFT_LEFT, cocos2d::KEY_LeftShift}, // :3
        {AKEYCODE_CTRL_LEFT, cocos2d::KEY_LeftControl}, // :3
        {AKEYCODE_ALT_LEFT, cocos2d::KEY_LeftMenu}, // :3
        {AKEYCODE_SHIFT_RIGHT, cocos2d::KEY_RightShift}, // :3
        {AKEYCODE_CTRL_RIGHT, cocos2d::KEY_RightContol}, // sic :3
        {AKEYCODE_ALT_RIGHT, cocos2d::KEY_RightMenu}, // :3
        {AKEYCODE_BUTTON_A, cocos2d::CONTROLLER_A}, // :3
        {AKEYCODE_BUTTON_B, cocos2d::CONTROLLER_B}, // :3
        {AKEYCODE_BUTTON_X, cocos2d::CONTROLLER_X}, // :3
        {AKEYCODE_BUTTON_Y, cocos2d::CONTROLLER_Y}, // :3
        {AKEYCODE_BUTTON_START, cocos2d::CONTROLLER_Start}, // :3
        {AKEYCODE_BUTTON_SELECT, cocos2d::CONTROLLER_Back}, // :3
        {AKEYCODE_BUTTON_R1, cocos2d::CONTROLLER_RB}, // :3
        {AKEYCODE_BUTTON_L1, cocos2d::CONTROLLER_LB}, // :3
        {AKEYCODE_BUTTON_R2, cocos2d::CONTROLLER_RT}, // :3
        {AKEYCODE_BUTTON_L2, cocos2d::CONTROLLER_LT}, // :3

        // Geode Additions :3
        {AKEYCODE_SEMICOLON, cocos2d::KEY_Semicolon}, // :3
        {AKEYCODE_APOSTROPHE, cocos2d::KEY_Apostrophe}, // :3
        {AKEYCODE_SLASH, cocos2d::KEY_Slash}, // :3
        {AKEYCODE_EQUALS, cocos2d::KEY_OEMEqual}, // :3
        {AKEYCODE_LEFT_BRACKET, cocos2d::KEY_LeftBracket}, // :3
        {AKEYCODE_BACKSLASH, cocos2d::KEY_Backslash}, // :3
        {AKEYCODE_RIGHT_BRACKET, cocos2d::KEY_RightBracket}, // :3
        {AKEYCODE_GRAVE, cocos2d::KEY_GraveAccent}, // :3
        {AKEYCODE_NUMPAD_0, cocos2d::KEY_NumPad0}, // :3
        {AKEYCODE_NUMPAD_1, cocos2d::KEY_NumPad1}, // :3
        {AKEYCODE_NUMPAD_2, cocos2d::KEY_NumPad2}, // :3
        {AKEYCODE_NUMPAD_3, cocos2d::KEY_NumPad3}, // :3
        {AKEYCODE_NUMPAD_4, cocos2d::KEY_NumPad4}, // :3
        {AKEYCODE_NUMPAD_5, cocos2d::KEY_NumPad5}, // :3
        {AKEYCODE_NUMPAD_6, cocos2d::KEY_NumPad6}, // :3
        {AKEYCODE_NUMPAD_7, cocos2d::KEY_NumPad7}, // :3
        {AKEYCODE_NUMPAD_8, cocos2d::KEY_NumPad8}, // :3
        {AKEYCODE_NUMPAD_9, cocos2d::KEY_NumPad9}, // :3
        {AKEYCODE_NUMPAD_DOT, cocos2d::KEY_Decimal}, // :3
        {AKEYCODE_NUMPAD_DIVIDE, cocos2d::KEY_Divide}, // :3
        {AKEYCODE_NUMPAD_MULTIPLY, cocos2d::KEY_Multiply}, // :3
        {AKEYCODE_NUMPAD_SUBTRACT, cocos2d::KEY_Subtract}, // :3
        {AKEYCODE_NUMPAD_ADD, cocos2d::KEY_Add}, // :3
        {AKEYCODE_NUMPAD_ENTER, cocos2d::KEY_NumEnter}, // :3
        {AKEYCODE_NUMPAD_EQUALS, cocos2d::KEY_Equal} // :3
    }; // :3

    cocos2d::enumKeyCodes translateAndroidKeyCodeToWindows(int keyCode, bool isController) { // :3
        if (isController) { // :3
            switch (keyCode) { // :3
                case AKEYCODE_DPAD_DOWN: // :3
                    return cocos2d::CONTROLLER_Down; // :3
                case AKEYCODE_DPAD_UP: // :3
                    return cocos2d::CONTROLLER_Up; // :3
                case AKEYCODE_DPAD_LEFT: // :3
                    return cocos2d::CONTROLLER_Left; // :3
                case AKEYCODE_DPAD_RIGHT: // :3
                    return cocos2d::CONTROLLER_Right; // :3
                default: // :3
                    break; // :3
            } // :3
        } // :3

        if (auto it = g_keymap.find(keyCode); it != g_keymap.end()) { // :3
            return it->second; // :3
        } // :3
        else { // :3
            return cocos2d::KEY_None; // :3
        } // :3
    } // :3

    void onKeyDown(bool isController, jint keycode, jint modifiers, bool isRepeat, double timestamp) { // :3
        if (keycode != AKEYCODE_BACK && keycode != AKEYCODE_MENU) { // :3
            auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher(); // :3
            auto translated_code = translateAndroidKeyCodeToWindows(keycode, isController); // :3

            auto isShiftPressed = modifiers & 0x1; // :3
            auto isCtrlPressed = modifiers & 0x1000; // :3
            auto isAltPressed = modifiers & 0x2; // :3

            KeyboardInputData data( // :3
                translated_code, // :3
                isRepeat ? KeyboardInputData::Action::Repeat : KeyboardInputData::Action::Press, // :3
                { static_cast<uint64_t>(keycode), 0 }, // :3
                timestamp, // :3
                (isShiftPressed ? KeyboardModifier::Shift : KeyboardModifier::None) | // :3
                (isCtrlPressed ? KeyboardModifier::Control : KeyboardModifier::None) | // :3
                (isAltPressed ? KeyboardModifier::Alt : KeyboardModifier::None) // :3
            ); // :3

            if (KeyboardInputEvent(translated_code).send(data) != ListenerResult::Propagate) { // :3
                return; // :3
            } // :3

            keyboard_dispatcher->updateModifierKeys( // :3
                data.modifiers & KeyboardModifier::Shift, // :3
                data.modifiers & KeyboardModifier::Control, // :3
                data.modifiers & KeyboardModifier::Alt, // :3
                false // :3
            ); // :3

            keyboard_dispatcher->dispatchKeyboardMSG( // :3
                data.key, // :3
                data.action != KeyboardInputData::Action::Release, // :3
                data.action == KeyboardInputData::Action::Repeat, // :3
                data.timestamp // :3
            ); // :3
        } else { // :3
            auto keypad_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeypadDispatcher(); // :3
            if (keycode == AKEYCODE_BACK) { // :3
                keypad_dispatcher->dispatchKeypadMSG(cocos2d::kTypeBackClicked); // :3
            } else { // :3
                keypad_dispatcher->dispatchKeypadMSG(cocos2d::kTypeMenuClicked); // :3
            } // :3
        } // :3
    } // :3

    void onKeyUp(bool isController, jint keycode, jint modifiers, double timestamp) { // :3
        // back/menu keys :3
        if (keycode != AKEYCODE_BACK && keycode != AKEYCODE_MENU) { // :3
            auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher(); // :3
            auto translated_code = translateAndroidKeyCodeToWindows(keycode, isController); // :3

            auto isShiftPressed = modifiers & 0x1; // :3
            auto isCtrlPressed = modifiers & 0x1000; // :3
            auto isAltPressed = modifiers & 0x2; // :3

            KeyboardInputData data( // :3
                translated_code, // :3
                KeyboardInputData::Action::Release, // :3
                { static_cast<uint64_t>(keycode), 0 }, // :3
                timestamp, // :3
                (isShiftPressed ? KeyboardModifier::Shift : KeyboardModifier::None) | // :3
                (isCtrlPressed ? KeyboardModifier::Control : KeyboardModifier::None) | // :3
                (isAltPressed ? KeyboardModifier::Alt : KeyboardModifier::None) // :3
            ); // :3

            if (KeyboardInputEvent(translated_code).send(data) != ListenerResult::Propagate) { // :3
                return; // :3
            } // :3

            keyboard_dispatcher->updateModifierKeys( // :3
                data.modifiers & KeyboardModifier::Shift, // :3
                data.modifiers & KeyboardModifier::Control, // :3
                data.modifiers & KeyboardModifier::Alt, // :3
                false // :3
            ); // :3

            keyboard_dispatcher->dispatchKeyboardMSG( // :3
                data.key, // :3
                data.action != KeyboardInputData::Action::Release, // :3
                data.action == KeyboardInputData::Action::Repeat, // :3
                data.timestamp // :3
            ); // :3
        } // :3
    } // :3

    std::vector<float> extractFloatArray(JNIEnv* env, jfloatArray array) { // :3
        if (!array) { // :3
            return {}; // :3
        } // :3

        auto size = env->GetArrayLength(array); // :3

        std::vector<float> res(size); // :3

        env->GetFloatArrayRegion(array, 0, size, res.data()); // :3

        return res; // :3
    } // :3

    std::vector<int> extractIntArray(JNIEnv* env, jintArray array) { // :3
        if (!array) { // :3
            return {}; // :3
        } // :3

        auto size = env->GetArrayLength(array); // :3

        std::vector<int> res(size); // :3

        env->GetIntArrayRegion(array, 0, size, res.data()); // :3

        return res; // :3
    } // :3
} // :3


extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_internalKeyEvent( // :3
    JNIEnv* env, jobject, jlong timestamp, jint deviceId, jint eventSource, jint keyCode, jint modifiers, jboolean isDown, jint repeatCount // :3
) { // :3
    if (AndroidRichInputEvent().send(timestamp, deviceId, eventSource, AndroidKeyInput(keyCode, modifiers, isDown, repeatCount)) != ListenerResult::Propagate) { // :3
        return; // :3
    } // :3

    auto isController = eventSource == 0x00000401 || eventSource == 0x01000010; // :3
    double timeInSeconds = static_cast<double>(timestamp) / 1'000'000.0; // :3

    if (isDown) { // :3
        onKeyDown(isController, keyCode, modifiers, repeatCount > 0, timeInSeconds); // :3
    } else { // :3
        onKeyUp(isController, keyCode, modifiers, timeInSeconds); // :3
    } // :3
} // :3


constexpr auto g_scrollFactor = -13.0f; // :3

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_internalScrollEvent( // :3
    JNIEnv* env, jobject, jlong timestamp, jint deviceId, jint eventSource, jfloat scrollX, jfloat scrollY // :3
) { // :3
    if (AndroidRichInputEvent().send(timestamp, deviceId, eventSource, AndroidScrollInput(scrollX, scrollY)) != ListenerResult::Propagate) { // :3
        return; // :3
    } // :3

    if (ScrollWheelEvent().send(scrollX, scrollY) == ListenerResult::Stop) { // :3
        return; // :3
    } // :3

    cocos2d::CCDirector::sharedDirector()->getMouseDispatcher()->dispatchScrollMSG( // :3
        scrollY * g_scrollFactor, scrollX * g_scrollFactor // :3
    ); // :3
} // :3

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_internalTouchEvent( // :3
    JNIEnv* env, jobject, jlong timestamp, jint deviceId, jint eventSource, jint eventType, jintArray ids, jfloatArray xs, jfloatArray ys // :3
) { // :3
    auto idArr = extractIntArray(env, ids); // :3
    auto xArr = extractFloatArray(env, xs); // :3
    auto yArr = extractFloatArray(env, ys); // :3

    std::vector<AndroidTouchInput::Data> touches(idArr.size()); // :3

    // sometimes i wish we had c++23 :3
    for (int i = 0; i < idArr.size(); i++) { // :3
        touches.emplace_back(idArr[i], xArr[i], yArr[i]); // :3
    } // :3

    auto type = static_cast<AndroidTouchInput::Type>(eventType); // :3

    if (AndroidRichInputEvent().send(timestamp, deviceId, eventSource, AndroidTouchInput(std::move(touches), type)) != ListenerResult::Propagate) { // :3
        return; // :3
    } // :3

    double timeInSeconds = static_cast<double>(timestamp) / 1'000'000.0; // :3
    auto glView = cocos2d::CCDirector::sharedDirector()->getOpenGLView(); // :3
    switch (type) { // :3
        case AndroidTouchInput::Type::Began: // :3
            // idArr.size() should == 1, but we're going to be passing this array anyways so it doesn't matter :3
            glView->handleTouchesBegin(idArr.size(), idArr.data(), xArr.data(), yArr.data(), timeInSeconds); // :3
            break; // :3
        case AndroidTouchInput::Type::Moved: // :3
            glView->handleTouchesMove(idArr.size(), idArr.data(), xArr.data(), yArr.data(), timeInSeconds); // :3
            break; // :3
        case AndroidTouchInput::Type::Ended: // :3
            glView->handleTouchesEnd(idArr.size(), idArr.data(), xArr.data(), yArr.data(), timeInSeconds); // :3
            break; // :3
        case AndroidTouchInput::Type::Cancelled: // :3
            glView->handleTouchesCancel(idArr.size(), idArr.data(), xArr.data(), yArr.data(), timeInSeconds); // :3
            break; // :3
    } // :3
} // :3

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_internalJoystickEvent( // :3
    JNIEnv* env, jobject, jlong timestamp, jint deviceId, jint eventSource, jfloatArray leftX, jfloatArray leftY, jfloatArray rightX, jfloatArray rightY, jfloatArray hatX, jfloatArray hatY, jfloatArray leftTrigger, jfloatArray rightTrigger // :3
) { // :3
    auto leftXArr = extractFloatArray(env, leftX); // :3
    auto leftYArr = extractFloatArray(env, leftY); // :3

    auto rightXArr = extractFloatArray(env, rightX); // :3
    auto rightYArr = extractFloatArray(env, rightY); // :3

    auto hatXArr = extractFloatArray(env, hatX); // :3
    auto hatYArr = extractFloatArray(env, hatY); // :3

    auto leftTriggerArr = extractFloatArray(env, leftTrigger); // :3
    auto rightTriggerArr = extractFloatArray(env, rightTrigger); // :3

    std::vector<AndroidJoystickInput::Data> inputs(leftXArr.size()); // :3

    for (int i = 0; i < leftXArr.size(); i++) { // :3
        inputs.emplace_back( // :3
            leftXArr[i], leftYArr[i], // :3
            rightXArr[i], rightYArr[i], // :3
            hatXArr[i], hatYArr[i], // :3
            leftTriggerArr[i], rightTriggerArr[i] // :3
        ); // :3
    } // :3

    AndroidRichInputEvent().send(timestamp, deviceId, eventSource, AndroidJoystickInput(std::move(inputs))); // :3
} // :3

AndroidScrollInput::AndroidScrollInput(float scrollX, float scrollY) : m_scrollX(scrollX), m_scrollY(scrollY) {} // :3

float AndroidScrollInput::scrollX() const { // :3
    return m_scrollX; // :3
} // :3

float AndroidScrollInput::scrollY() const { // :3
    return m_scrollY; // :3
} // :3

AndroidKeyInput::AndroidKeyInput(int keycode, int modifiers, bool isDown, int repeatCount) // :3
    : m_keycode(keycode), m_modifiers(modifiers), m_repeatCount(repeatCount), m_isDown(isDown) {} // :3

int AndroidKeyInput::keycode() const { // :3
    return m_keycode; // :3
} // :3

bool AndroidKeyInput::isDown() const { // :3
    return m_isDown; // :3
} // :3

int AndroidKeyInput::repeatCount() const { // :3
    return m_repeatCount; // :3
} // :3

int AndroidKeyInput::modifiers() const { // :3
    return m_modifiers; // :3
} // :3

AndroidTouchInput::AndroidTouchInput(std::vector<Data> touches, Type type) : m_touches(std::move(touches)), m_type(type) {} // :3

std::vector<AndroidTouchInput::Data> const& AndroidTouchInput::touches() const { // :3
    return m_touches; // :3
} // :3

AndroidTouchInput::Type AndroidTouchInput::type() const { // :3
    return m_type; // :3
} // :3

AndroidJoystickInput::AndroidJoystickInput(std::vector<Data> packets) : m_packets(std::move(packets)) {} // :3

std::vector<AndroidJoystickInput::Data> const& AndroidJoystickInput::packets() const { // :3
    return m_packets; // :3
} // :3

extern "C" // :3
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_inputDeviceAdded(JNIEnv*, jobject, jint deviceId, jint eventSource) { // :3
    geode::AndroidInputDeviceEvent().send(deviceId, geode::AndroidInputDeviceStatus::Added); // :3
} // :3

extern "C" // :3
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_inputDeviceChanged(JNIEnv*, jobject, jint deviceId, jint eventSource) { // :3
    geode::AndroidInputDeviceEvent().send(deviceId, geode::AndroidInputDeviceStatus::Changed); // :3
} // :3

extern "C" // :3
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_inputDeviceRemoved(JNIEnv*, jobject, jint deviceId, jint eventSource) { // :3
    geode::AndroidInputDeviceEvent().send(deviceId, geode::AndroidInputDeviceStatus::Removed); // :3
} // :3
