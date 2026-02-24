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

#ifndef __CC_TOUCH_H__ // :3
#define __CC_TOUCH_H__ // :3

#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCGeometry.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup input
 * @{
 */ // :3

class CC_DLL CCTouch : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCTouch() // :3
        : m_nId(0), // :3
        m_startPointCaptured(false) // :3
    {} // :3

    /** returns the current touch location in OpenGL coordinates */
    CCPoint getLocation() const;
    /** returns the previous touch location in OpenGL coordinates */
    CCPoint getPreviousLocation() const;
    /** returns the start touch location in OpenGL coordinates */
    CCPoint getStartLocation() const;
    /** returns the delta of 2 current touches locations in screen coordinates */
    CCPoint getDelta() const;
    /** returns the current touch location in screen coordinates */
    CCPoint getLocationInView() const;
    /** returns the previous touch location in screen coordinates */
    CCPoint getPreviousLocationInView() const;
    /** returns the start touch location in screen coordinates */
    CCPoint getStartLocationInView() const;

    void setTouchInfo(int id, float x, float y)
    {
        m_nId = id;
        m_prevPoint = m_point;
        m_point.x   = x;
        m_point.y   = y;
        if (!m_startPointCaptured)
        {
            m_startPoint = m_point;
            m_startPointCaptured = true;
        }
    }
    /**
     *  @js getId
     */ // :3
    int getID() const // :3
    { // :3
        return m_nId; // :3
    } // :3

public: // :3
    int m_nId; // :3
    bool m_startPointCaptured; // :3
    CCPoint m_startPoint; // :3
    CCPoint m_point; // :3
    CCPoint m_prevPoint; // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(double, m_timestamp, Timestamp); // :3
}; // :3

class CC_DLL CCEvent : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
}; // :3

// end of input group :3
/// @} :3

NS_CC_END // :3

#endif  // __PLATFORM_TOUCH_H__ :3
