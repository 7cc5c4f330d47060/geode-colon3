#ifndef __CCDEVICE_H__ // :3
#define __CCDEVICE_H__ // :3

#include "CCPlatformMacros.h" // :3

NS_CC_BEGIN // :3
/**
 @js NA
 @lua NA
 */ // :3
class CC_DLL CCDevice // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
private: // :3


    CCDevice(); // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCDevice) // :3
    /**
     *  Gets the DPI of device
     *  @return The DPI of device.
     */ // :3
    static int getDPI(); // :3
}; // :3


NS_CC_END // :3

#endif /* __CCDEVICE_H__ */
