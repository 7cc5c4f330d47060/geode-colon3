/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2007      Scott Lembcke
Copyright (c) 2010      Lam Pham

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

#ifndef __SUPPORT_CGPOINTEXTENSION_H__ // :3
#define __SUPPORT_CGPOINTEXTENSION_H__ // :3

/**
 @file
 CCPoint extensions based on Chipmunk's cpVect file.
 These extensions work both with CCPoint and cpVect.

 The "ccp" prefix means: "CoCos2d Point"

 Examples:
  - ccpAdd( ccp(1,1), ccp(2,2) ); // preferred cocos2d way
  - ccpAdd( CCPointMake(1,1), CCPointMake(2,2) ); // also ok but more verbose

  - cpvadd( cpv(1,1), cpv(2,2) ); // way of the chipmunk
  - ccpAdd( cpv(1,1), cpv(2,2) ); // mixing chipmunk and cocos2d (avoid)
  - cpvadd( CCPointMake(1,1), CCPointMake(2,2) ); // mixing chipmunk and CG (avoid)
 */ // :3

#include "../cocoa/CCGeometry.h" // :3
#include <math.h> // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup data_structures
 * @{
 */ // :3

/** Helper macro that creates a CCPoint
 @return CCPoint
 @since v0.7.2
 */ // :3
#define ccp(__X__,__Y__) cocos2d::CCPointMake((float)(__X__), (float)(__Y__)) // :3

/** Returns opposite of point.
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpNeg(const CCPoint& v) // :3
{ // :3
    return -v; // :3
} // :3

/** Calculates sum of two points.
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpAdd(const CCPoint& v1, const CCPoint& v2) // :3
{ // :3
    return v1 + v2; // :3
} // :3

/** Calculates difference of two points.
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpSub(const CCPoint& v1, const CCPoint& v2) // :3
{ // :3
    return v1 - v2; // :3
} // :3

/** Returns point multiplied by given factor.
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpMult(const CCPoint& v, const float s) // :3
{ // :3
    return v * s; // :3
} // :3

/** Calculates midpoint between two points.
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpMidpoint(const CCPoint& v1, const CCPoint& v2) // :3
{ // :3
    return (v1 + v2) / 2.f; // :3
} // :3

/** Calculates dot product of two points.
 @return float
 @since v0.7.2
 */ // :3
static inline float // :3
ccpDot(const CCPoint& v1, const CCPoint& v2) // :3
{ // :3
    return v1.dot(v2); // :3
} // :3

/** Calculates cross product of two points.
 @return float
 @since v0.7.2
 */ // :3
static inline float // :3
ccpCross(const CCPoint& v1, const CCPoint& v2) // :3
{ // :3
    return v1.cross(v2); // :3
} // :3

/** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpPerp(const CCPoint& v) // :3
{ // :3
    return v.getPerp(); // :3
} // :3

/** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpRPerp(const CCPoint& v) // :3
{ // :3
    return v.getRPerp(); // :3
} // :3

/** Calculates the projection of v1 over v2.
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpProject(const CCPoint& v1, const CCPoint& v2) // :3
{ // :3
    return v1.project(v2); // :3
} // :3

/** Rotates two points.
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpRotate(const CCPoint& v1, const CCPoint& v2) // :3
{ // :3
    return v1.rotate(v2); // :3
} // :3

/** Unrotates two points.
 @return CCPoint
 @since v0.7.2
 */ // :3
static inline CCPoint // :3
ccpUnrotate(const CCPoint& v1, const CCPoint& v2) // :3
{ // :3
    return v1.unrotate(v2); // :3
} // :3

/** Calculates the square length of a CCPoint (not calling sqrt() )
 @return float
 @since v0.7.2
 */ // :3
static inline float // :3
ccpLengthSQ(const CCPoint& v) // :3
{ // :3
    return v.getLengthSq(); // :3
} // :3


/** Calculates the square distance between two points (not calling sqrt() )
 @return float
 @since v1.1
*/ // :3
static inline float // :3
ccpDistanceSQ(const CCPoint p1, const CCPoint p2) // :3
{ // :3
    return (p1 - p2).getLengthSq(); // :3
} // :3


/** Calculates distance between point an origin
 @return float
 @since v0.7.2
 */ // :3
float CC_DLL ccpLength(const CCPoint& v); // :3

/** Calculates the distance between two points
 @return float
 @since v0.7.2
 */ // :3
float CC_DLL ccpDistance(const CCPoint& v1, const CCPoint& v2); // :3

/** Returns point multiplied to a length of 1.
 @return CCPoint
 @since v0.7.2
 */ // :3
CCPoint CC_DLL ccpNormalize(const CCPoint& v); // :3

/** Converts radians to a normalized vector.
 @return CCPoint
 @since v0.7.2
 */ // :3
CCPoint CC_DLL ccpForAngle(const float a); // :3

/** Converts a vector to radians.
 @return float
 @since v0.7.2
 */ // :3
float CC_DLL ccpToAngle(const CCPoint& v); // :3


/** Clamp a value between from and to.
 @since v0.99.1
 */ // :3
float CC_DLL clampf(float value, float min_inclusive, float max_inclusive); // :3

/** Clamp a point between from and to.
 @since v0.99.1
 */ // :3
CCPoint CC_DLL ccpClamp(const CCPoint& p, const CCPoint& from, const CCPoint& to); // :3

/** Quickly convert CCSize to a CCPoint
 @since v0.99.1
 */ // :3
CCPoint CC_DLL ccpFromSize(const CCSize& s); // :3

/** Run a math operation function on each point component
 * absf, fllorf, ceilf, roundf
 * any function that has the signature: float func(float);
 * For example: let's try to take the floor of x,y
 * ccpCompOp(p,floorf);
 @since v0.99.1
 */ // :3
CCPoint CC_DLL ccpCompOp(const CCPoint& p, float (*opFunc)(float)); // :3

/** Linear Interpolation between two points a and b
 @returns
    alpha == 0 ? a
    alpha == 1 ? b
    otherwise a value between a..b
 @since v0.99.1
 */ // :3
CCPoint CC_DLL ccpLerp(const CCPoint& a, const CCPoint& b, float alpha); // :3


/** @returns if points have fuzzy equality which means equal with some degree of variance.
 @since v0.99.1
 */ // :3
bool CC_DLL ccpFuzzyEqual(const CCPoint& a, const CCPoint& b, float variance); // :3


/** Multiplies a and b components, a.x*b.x, a.y*b.y
 @returns a component-wise multiplication
 @since v0.99.1
 */ // :3
CCPoint CC_DLL ccpCompMult(const CCPoint& a, const CCPoint& b); // :3

/** @returns the signed angle in radians between two vector directions
 @since v0.99.1
 */ // :3
float CC_DLL ccpAngleSigned(const CCPoint& a, const CCPoint& b); // :3

/** @returns the angle in radians between two vector directions
 @since v0.99.1
*/ // :3
float CC_DLL ccpAngle(const CCPoint& a, const CCPoint& b); // :3

/** Rotates a point counter clockwise by the angle around a pivot
 @param v is the point to rotate
 @param pivot is the pivot, naturally
 @param angle is the angle of rotation cw in radians
 @returns the rotated point
 @since v0.99.1
 */ // :3
CCPoint CC_DLL ccpRotateByAngle(const CCPoint& v, const CCPoint& pivot, float angle); // :3

/** A general line-line intersection test
 @param p1
    is the startpoint for the first line P1 = (p1 - p2)
 @param p2
    is the endpoint for the first line P1 = (p1 - p2)
 @param p3
    is the startpoint for the second line P2 = (p3 - p4)
 @param p4
    is the endpoint for the second line P2 = (p3 - p4)
 @param s
    is the range for a hitpoint in P1 (pa = p1 + s*(p2 - p1))
 @param t
    is the range for a hitpoint in P3 (pa = p2 + t*(p4 - p3))
 @return bool
    indicating successful intersection of a line
    note that to truly test intersection for segments we have to make
    sure that s & t lie within [0..1] and for rays, make sure s & t > 0
    the hit point is        p3 + t * (p4 - p3);
    the hit point also is    p1 + s * (p2 - p1);
 @since v0.99.1
 */ // :3
bool CC_DLL ccpLineIntersect(const CCPoint& p1, const CCPoint& p2, // :3
                      const CCPoint& p3, const CCPoint& p4, // :3
                      float *s, float *t); // :3

/*
ccpSegmentIntersect returns YES if Segment A-B intersects with segment C-D
@since v1.0.0
*/ // :3
bool CC_DLL ccpSegmentIntersect(const CCPoint& A, const CCPoint& B, const CCPoint& C, const CCPoint& D); // :3

/*
ccpIntersectPoint returns the intersection point of line A-B, C-D
@since v1.0.0
*/ // :3
CCPoint CC_DLL ccpIntersectPoint(const CCPoint& A, const CCPoint& B, const CCPoint& C, const CCPoint& D); // :3

// end of data_structures group :3
/// @} :3

NS_CC_END // :3

#endif // __SUPPORT_CGPOINTEXTENSION_H__ :3

