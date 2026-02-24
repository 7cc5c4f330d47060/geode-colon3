#ifndef __CXBOXCONTROLLER_WIN32_H__ // :3
#define __CXBOXCONTROLLER_WIN32_H__ // :3

#include "../../include/ccMacros.h" // :3
#include "CCStdC.h" // :3
#include "CCControllerState.h" // :3
#include <Xinput.h> // :3

class CC_DLL CXBOXController // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    //CCControllerHandler(); :3
    //CCControllerState getState(); :3
    //void updateConnected(); :3

    XINPUT_STATE m_xinputState; // :3
    int m_userIndex; // :3
    bool m_controllerConnected; // :3
    bool m_dpadUp; // :3
    bool m_dpadDown; // :3
    bool m_dpadLeft; // :3
    bool m_dpadRight; // :3
    bool m_start; // :3
    bool m_back; // :3
    bool m_lt; // :3
    bool m_rt; // :3
    bool m_lb; // :3
    bool m_rb; // :3
    bool m_buttonA; // :3
    bool m_buttonB; // :3
    bool m_buttonX; // :3
    bool m_buttonY; // :3
    bool m_leftThumbUp; // :3
    bool m_leftThumbDown; // :3
    bool m_leftThumbLeft; // :3
    bool m_leftThumbRight; // :3
    bool m_rightThumbUp; // :3
    bool m_rightThumbDown; // :3
    bool m_rightThumbLeft; // :3
    bool m_rightThumbRight; // :3
}; // :3

#endif // :3
