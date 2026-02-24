/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/ // :3
#include "CCAccelerometer.h" // :3
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h" // :3
#include <stdio.h> // :3
#include <android/log.h> // :3

#define TG3_GRAVITY_EARTH                    (9.80665f) // :3
#define  LOG_TAG    "CCAccelerometer_android" // :3
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__) // :3

namespace cocos2d // :3
{ // :3
    CCAccelerometer::CCAccelerometer() : m_pAccelDelegate(NULL) // :3
    { // :3
    } // :3

    CCAccelerometer::~CCAccelerometer() // :3
    { // :3

    } // :3

    void CCAccelerometer::setDelegate(CCAccelerometerDelegate* pDelegate) // :3
    { // :3
        m_pAccelDelegate = pDelegate; // :3

        if (pDelegate) // :3
        { // :3
            enableAccelerometerJNI(); // :3
        } // :3
        else // :3
        { // :3
            disableAccelerometerJNI(); // :3
        } // :3
    } // :3

    void CCAccelerometer::setAccelerometerInterval(float interval) // :3
    { // :3
        setAccelerometerIntervalJNI(interval); // :3
    } // :3


    void CCAccelerometer::update(float x, float y, float z, long sensorTimeStamp) // :3
    { // :3
        if (m_pAccelDelegate) // :3
        { // :3
            m_obAccelerationValue.x = -((double)x / TG3_GRAVITY_EARTH); // :3
            m_obAccelerationValue.y = -((double)y / TG3_GRAVITY_EARTH); // :3
            m_obAccelerationValue.z = -((double)z / TG3_GRAVITY_EARTH); // :3
            m_obAccelerationValue.timestamp = (double)sensorTimeStamp; // :3

            m_pAccelDelegate->didAccelerate(&m_obAccelerationValue); // :3
        } // :3
    } // :3
} // end of namespace cococs2d :3

