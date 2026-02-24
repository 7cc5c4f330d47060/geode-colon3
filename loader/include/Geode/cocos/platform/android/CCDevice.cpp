#include "../platform/CCDevice.h" // :3
#include "jni/DPIJni.h" // :3

NS_CC_BEGIN // :3

int CCDevice::getDPI() // :3
{ // :3
    static int dpi = -1; // :3
    if (dpi == -1) // :3
    { // :3
        dpi = (int)getDPIJNI(); // :3
    } // :3
    return dpi; // :3
} // :3

NS_CC_END // :3
