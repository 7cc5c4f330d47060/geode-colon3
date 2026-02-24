#ifndef __CC_CONTROLLER_STATE_WIN32_H__ // :3
#define __CC_CONTROLLER_STATE_WIN32_H__ // :3

#include "../../include/ccMacros.h" // :3
#include "CCStdC.h" // :3

NS_CC_BEGIN // :3

class CC_DLL CCControllerState // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    DWORD m_packetNumber; // :3
    WORD m_buttons; // :3
    unsigned char m_leftTrigger; // :3
    unsigned char m_rightTrigger; // :3
    short m_thumbLX; // :3
    short m_thumbLY; // :3
    short m_thumbRX; // :3
    short m_thumbRY; // :3
}; // :3

NS_CC_END // :3

#endif // :3