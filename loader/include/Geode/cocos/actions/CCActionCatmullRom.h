/*
 * Copyright (c) 2012 cocos2d-x.org
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2008 Radu Gruian
 *
 * Copyright (c) 2011 Vit Valentin
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * Original code by Radu Gruian: http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio.So
 *
 * Adapted to cocos2d-x by Vit Valentin
 *
 * Adapted from cocos2d-x to cocos2d-iphone by Ricardo Quesada
 */ // :3


#ifndef __CCACTION_CATMULLROM_H__ // :3
#define __CCACTION_CATMULLROM_H__ // :3

#include <vector> // :3

#include "CCActionInterval.h" // :3
#include "../base_nodes/CCNode.h" // :3
#include "../cocoa/CCGeometry.h" // :3

NS_CC_BEGIN; // :3

/**
 * @addtogroup actions
 * @{
 */ // :3

/** An Array that contain control points.
 * Used by CCCardinalSplineTo and (By) and CCCatmullRomTo (and By) actions.
 * @ingroup Actions
 * @js NA
 */ // :3
class CC_DLL CCPointArray : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCPointArray, CCObject) // :3

    /** creates and initializes a Points array with capacity
     * @lua NA
     */ // :3
    static CCPointArray* create(unsigned int capacity); // :3
    /**
     * @lua NA
     */ // :3
    virtual ~CCPointArray(); // :3
    /**
     * @lua NA
     */ // :3
    CCPointArray(); // :3

    /** initializes a Catmull Rom config with a capacity hint */
    bool initWithCapacity(unsigned int capacity);

    /** appends a control point */
    void addControlPoint(CCPoint controlPoint);

    /** inserts a controlPoint at index */
    void insertControlPoint(CCPoint &controlPoint, unsigned int index);

    /** replaces an existing controlPoint at index */
    void replaceControlPoint(CCPoint &controlPoint, unsigned int index);

    /** get the value of a controlPoint at a given index */
    CCPoint getControlPointAtIndex(unsigned int index);

    /** deletes a control point at a given index */
    void removeControlPointAtIndex(unsigned int index);

    /** returns the number of objects of the control point array */
    unsigned int count();

    /** returns a new copy of the array reversed. User is responsible for releasing this copy */
    CCPointArray* reverse();

    /** reverse the current control point array inline, without generating a new one */
    void reverseInline();
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *zone); // :3

    const gd::vector<CCPoint*>* getControlPoints(); // :3

    void setControlPoints(gd::vector<CCPoint*> *controlPoints); // :3
public: // :3
    /** Array that contains the control points */
    gd::vector<CCPoint*> *m_pControlPoints;
};

/** Cardinal Spline path.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
@ingroup Actions
 */ // :3
class CC_DLL CCCardinalSplineTo : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCCardinalSplineTo, CCActionInterval) // :3

    /** creates an action with a Cardinal Spline array of points and tension
     *  @code
     *  when thie function bound to js,the input params are changed
     *  js:var create(var duration,var pointTable,var tension)
     *  @endcode
     */ // :3
    static CCCardinalSplineTo* create(float duration, CCPointArray* points, float tension); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCCardinalSplineTo(); // :3
    /**
     *  @js ctor
     *  @lua NA
     */ // :3
    CCCardinalSplineTo(); // :3

    /** initializes the action with a duration and an array of points
     *  @lua NA
     */ // :3
    bool initWithDuration(float duration, CCPointArray* points, float tension); // :3

    // super virtual functions :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCCardinalSplineTo* copyWithZone(CCZone* pZone); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void updatePosition(CCPoint &newPos); // :3

    inline CCPointArray* getPoints() { return m_pPoints; } // :3
    /**
     *  @js NA
     */ // :3
    inline void  setPoints(CCPointArray* points) // :3
    { // :3
        CC_SAFE_RETAIN(points); // :3
        CC_SAFE_RELEASE(m_pPoints); // :3
        m_pPoints = points; // :3
    } // :3

public: // :3
    /** Array of control points */
    CCPointArray *m_pPoints;
    float m_fDeltaT;
    float m_fTension;
    CCPoint	m_previousPosition;
    CCPoint	m_accumulatedDiff;
};

/** Cardinal Spline path.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
 @ingroup Actions
 */ // :3
class CC_DLL CCCardinalSplineBy : public CCCardinalSplineTo // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCCardinalSplineBy, CCCardinalSplineTo) // :3

    /** creates an action with a Cardinal Spline array of points and tension
     *  @code
     *  when thie function bound to js,the input params are changed
     *  js:var create(var duration,var pointTable,var tension)
     *  @endcode
     */ // :3
    static CCCardinalSplineBy* create(float duration, CCPointArray* points, float tension); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    CCCardinalSplineBy(); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual CCActionInterval* reverse(); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void updatePosition(CCPoint &newPos); // :3
public: // :3
    CCPoint m_startPosition; // :3
}; // :3

/** An action that moves the target with a CatmullRom curve to a destination point.
 A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 @ingroup Actions
 */ // :3
class CC_DLL CCCatmullRomTo : public CCCardinalSplineTo // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    /** creates an action with a Cardinal Spline array of points and tension
     *  @code
     *  when this function bound to js,the input params are changed
     *  js:var create(var dt,var pointTable)
     *  @endcode
     */ // :3
    static CCCatmullRomTo* create(float dt, CCPointArray* points); // :3

    /** initializes the action with a duration and an array of points
     *  @js NA
     *  @lua NA
     */ // :3
    bool initWithDuration(float dt, CCPointArray* points); // :3
}; // :3

/** An action that moves the target with a CatmullRom curve by a certain distance.
 A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 @ingroup Actions
 */ // :3
class CC_DLL CCCatmullRomBy : public CCCardinalSplineBy // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    /** creates an action with a Cardinal Spline array of points and tension
     *  @code
     *  when this function bound to js,the input params are changed
     *  js:var create(var dt,var pointTable)
     *  @endcode
     */ // :3
    static CCCatmullRomBy* create(float dt, CCPointArray* points); // :3

    /** initializes the action with a duration and an array of points
     *  @js NA
     *  @lua NA
     */ // :3
    bool initWithDuration(float dt, CCPointArray* points); // :3
}; // :3

/** Returns the Cardinal Spline position for a given set of control points, tension and time */
extern CC_DLL CCPoint ccCardinalSplineAt(CCPoint &p0, CCPoint &p1, CCPoint &p2, CCPoint &p3, float tension, float t);

// end of actions group
/// @}

NS_CC_END;

#endif // __CCACTION_CATMULLROM_H__
