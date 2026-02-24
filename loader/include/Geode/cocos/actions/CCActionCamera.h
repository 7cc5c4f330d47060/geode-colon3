/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada

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

#ifndef __CCCAMERA_ACTION_H__ // :3
#define __CCCAMERA_ACTION_H__ // :3

#include "CCActionInterval.h" // :3

NS_CC_BEGIN // :3

class CCCamera; // :3

/**
 * @addtogroup actions
 * @{
 */ // :3

/**
@brief Base class for CCCamera actions
@ingroup Actions
*/ // :3
class CC_DLL CCActionCamera : public CCActionInterval //<NSCopying> :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCActionCamera, CCActionInterval) // :3
    /**
     *  @js ctor
     */ // :3
    CCActionCamera() // :3
        :m_fCenterXOrig(0) // :3
        ,m_fCenterYOrig(0) // :3
        ,m_fCenterZOrig(0) // :3
        ,m_fEyeXOrig(0) // :3
        ,m_fEyeYOrig(0) // :3
        ,m_fEyeZOrig(0) // :3
        ,m_fUpXOrig(0) // :3
        ,m_fUpYOrig(0) // :3
        ,m_fUpZOrig(0) // :3
    {} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCActionCamera(){} // :3
    // super methods :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual CCActionInterval * reverse(); // :3
public: // :3
    float m_fCenterXOrig; // :3
    float m_fCenterYOrig; // :3
    float m_fCenterZOrig; // :3

    float m_fEyeXOrig; // :3
    float m_fEyeYOrig; // :3
    float m_fEyeZOrig; // :3

    float m_fUpXOrig; // :3
    float m_fUpYOrig; // :3
    float m_fUpZOrig; // :3
}; // :3

/**
@brief CCOrbitCamera action
Orbits the camera around the center of the screen using spherical coordinates
@ingroup Actions
*/ // :3
class CC_DLL CCOrbitCamera : public CCActionCamera //<NSCopying> :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCOrbitCamera() // :3
        : m_fRadius(0.0) // :3
        , m_fDeltaRadius(0.0) // :3
        , m_fAngleZ(0.0) // :3
        , m_fDeltaAngleZ(0.0) // :3
        , m_fAngleX(0.0) // :3
        , m_fDeltaAngleX(0.0) // :3
        , m_fRadZ(0.0) // :3
        , m_fRadDeltaZ(0.0) // :3
        , m_fRadX(0.0) // :3
        , m_fRadDeltaX(0.0) // :3
    {} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    ~CCOrbitCamera(){} // :3

    /** creates a CCOrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX */
    static CCOrbitCamera* create(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX);

    /** initializes a CCOrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX */
    bool initWithDuration(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX);
    /** positions the camera according to spherical coordinates */
    void sphericalRadius(float *r, float *zenith, float *azimuth);
    // super methods
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void update(float time); // :3

public: // :3
    float m_fRadius; // :3
    float m_fDeltaRadius; // :3
    float m_fAngleZ; // :3
    float m_fDeltaAngleZ; // :3
    float m_fAngleX; // :3
    float m_fDeltaAngleX; // :3

    float m_fRadZ; // :3
    float m_fRadDeltaZ; // :3
    float m_fRadX; // :3
    float m_fRadDeltaX; // :3
}; // :3

// end of actions group :3
/// @} :3

NS_CC_END // :3

#endif //__CCCAMERA_ACTION_H__ :3
