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

#ifndef __CCGEMETRY_H__ // :3
#define __CCGEMETRY_H__ // :3

#include "../platform/CCPlatformMacros.h" // :3
#include "CCObject.h" // :3
#include <math.h> // :3

// why :3
#if !defined(FLT_EPSILON) // :3
#define FLT_EPSILON 1.19209290E-07F // decimal constant :3
#endif // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup data_structures
 * @{
 */ // :3

// for CCPoint assignement operator and copy constructor :3
class CCSize; // :3

/**
 * @js NA
 */ // :3
class CC_DLL CCPoint // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    float x; // :3
    float y; // :3
    // alk modifications: all defined :3

    inline constexpr CCPoint() : x(0), y(0) {} // :3
    inline constexpr CCPoint(float x, float y) : x(x), y(y) {} // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCPoint(const CCPoint& other) : x(other.x), y(other.y) {} // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCPoint(const CCSize& size); // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCPoint& operator= (const CCPoint& other) { // :3
        setPoint(other.x, other.y); // :3
        return *this; // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCPoint& operator= (const CCSize& size); // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCPoint operator+(const CCPoint& right) const { // :3
        return CCPoint(this->x + right.x, this->y + right.y); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCPoint operator-(const CCPoint& right) const { // :3
        return CCPoint(this->x - right.x, this->y - right.y); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCPoint operator-() const { // :3
        return CCPoint(-x, -y); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCPoint operator*(float a) const { // :3
        return CCPoint(this->x * a, this->y * a); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCPoint operator/(float a) const { // :3
        return CCPoint(this->x / a, this->y / a); // :3
    } // :3


    // camila modification :3
    inline constexpr CCPoint operator*(const CCPoint& right) const { // :3
        return CCPoint(x * right.x, y * right.y); // :3
    } // :3
    inline constexpr CCPoint operator/(const CCPoint& right) const { // :3
        return CCPoint(x / right.x, y / right.y); // :3
    } // :3
    //314 :3

    /**
     * @lua NA
     */ // :3
    inline constexpr void setPoint(float x, float y) { // :3
        this->x = x; // :3
        this->y = y; // :3
    } // :3

    inline constexpr bool equals(const CCPoint& target) const { // :3
        return this->fuzzyEquals(target, FLT_EPSILON); // :3
    } // :3

    /** @returns if points have fuzzy equality which means equal with some degree of variance.
     * @since v2.1.4
     * @lua NA
     */ // :3
    inline constexpr bool fuzzyEquals(const CCPoint& b, float var) const { // :3
        if(x - var <= b.x && b.x <= x + var) // :3
            if(y - var <= b.y && b.y <= y + var) // :3
                return true; // :3
        return false; // :3
    } // :3

    /** Calculates distance between point an origin
     * @return float
     * @since v2.1.4
     * @lua NA
     */ // :3
    inline float getLength() const { // :3
        return sqrtf(x*x + y*y); // :3
    }; // :3

    /** Calculates the square length of a CCPoint (not calling sqrt() )
     * @return float
     * @since v2.1.4
     * @lua NA
     */ // :3
    inline constexpr float getLengthSq() const { // :3
        return dot(*this); //x*x + y*y; :3
    }; // :3

    /** Calculates the square distance between two points (not calling sqrt() )
     @return float
     @since v2.1.4
    */ // :3
    inline constexpr float getDistanceSq(const CCPoint& other) const { // :3
        return (*this - other).getLengthSq(); // :3
    }; // :3

    /** Calculates the distance between two points
     @return float
     @since v2.1.4
     */ // :3
    inline float getDistance(const CCPoint& other) const { // :3
        return (*this - other).getLength(); // :3
    }; // :3

    /** @returns the angle in radians between this vector and the x axis
     @since v2.1.4
    */ // :3
    inline float getAngle() const { // :3
        return atan2f(y, x); // :3
    }; // :3

    /** @returns the angle in radians between two vector directions
     @since v2.1.4
    */ // :3
    inline float getAngle(const CCPoint& other) const { // :3
        CCPoint a2 = normalize(); // :3
        CCPoint b2 = other.normalize(); // :3
        float angle = atan2f(a2.cross(b2), a2.dot(b2)); // :3
        if(fabs(angle) < FLT_EPSILON) return 0.f; // :3
        return angle; // :3
    } // :3

    /** Calculates dot product of two points.
     @return float
     @since v2.1.4
     */ // :3
    inline constexpr float dot(const CCPoint& other) const { // :3
        return x*other.x + y*other.y; // :3
    }; // :3

    /** Calculates cross product of two points.
     @return float
     @since v2.1.4
     */ // :3
    inline constexpr float cross(const CCPoint& other) const { // :3
        return x*other.y - y*other.x; // :3
    }; // :3

    /** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
     @return CCPoint
     @since v2.1.4
     */ // :3
    inline constexpr CCPoint getPerp() const { // :3
        return CCPoint(-y, x); // :3
    }; // :3

    /** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
     @return CCPoint
     @since v2.1.4
     */ // :3
    inline constexpr CCPoint getRPerp() const { // :3
        return CCPoint(y, -x); // :3
    }; // :3

    /** Calculates the projection of this over other.
     @return CCPoint
     @since v2.1.4
     */ // :3
    inline constexpr CCPoint project(const CCPoint& other) const { // :3
        return other * (dot(other)/other.dot(other)); // :3
    }; // :3

    /** Complex multiplication of two points ("rotates" two points).
     @return CCPoint vector with an angle of this.getAngle() + other.getAngle(),
     and a length of this.getLength() * other.getLength().
     @since v2.1.4
     */ // :3
    inline constexpr CCPoint rotate(const CCPoint& other) const { // :3
        return CCPoint(x*other.x - y*other.y, x*other.y + y*other.x); // :3
    }; // :3

    /** Unrotates two points.
     @return CCPoint vector with an angle of this.getAngle() - other.getAngle(),
     and a length of this.getLength() * other.getLength().
     @since v2.1.4
     */ // :3
    inline constexpr CCPoint unrotate(const CCPoint& other) const { // :3
        return CCPoint(x*other.x + y*other.y, y*other.x - x*other.y); // :3
    }; // :3

    /** Returns point multiplied to a length of 1.
     * If the point is 0, it returns (1, 0)
     @return CCPoint
     @since v2.1.4
     */ // :3
    inline CCPoint normalize() const { // :3
        float length = getLength(); // :3
        if(length == 0.) return CCPoint(1.f, 0); // :3
        return *this / getLength(); // :3
    }; // :3

    /** Linear Interpolation between two points a and b
     @returns
        alpha == 0 ? a
        alpha == 1 ? b
        otherwise a value between a..b
     @since v2.1.4
     */ // :3
    inline constexpr CCPoint lerp(const CCPoint& other, float alpha) const { // :3
        return *this * (1.f - alpha) + other * alpha; // :3
    }; // :3

    /** Rotates a point counter clockwise by the angle around a pivot
     @param pivot is the pivot, naturally
     @param angle is the angle of rotation ccw in radians
     @returns the rotated point
     @since v2.1.4
     */ // :3
    inline CCPoint rotateByAngle(const CCPoint& pivot, float angle) const { // :3
        return pivot + (*this - pivot).rotate(CCPoint::forAngle(angle)); // :3
    } // :3

    static inline CCPoint forAngle(const float a) // :3
    { // :3
    	return CCPoint(cosf(a), sinf(a)); // :3
    } // :3

	constexpr void add(int idx, float val) { // :3
        idx == 0 ? x += val : y += val; // :3
    } // :3
	constexpr float at(int idx) { // :3
        return idx == 0 ? x : y; // :3
    } // :3
	constexpr bool isZero() const { // :3
        return x == 0.f && y == 0.f; // :3
    } // :3
	constexpr void set(int idx, float val) { // :3
        idx == 0 ? x = val : y = val; // :3
    } // :3
	constexpr void swap() { // :3
        std::swap(x, y); // :3
    } // :3

}; // :3

/**
 * @js NA
 */ // :3
class CC_DLL CCSize // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    float width; // :3
    float height; // :3

public: // :3
    inline constexpr CCSize() : width(0), height(0) {} // :3
    inline constexpr CCSize(float width, float height) : width(width), height(height) {} // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCSize(const CCSize& other) : width(other.width), height(other.height) {} // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCSize(const CCPoint& point) : width(point.x), height(point.y) {} // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCSize& operator= (const CCSize& other) { // :3
        setSize(other.width, other.height); // :3
        return *this; // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCSize& operator= (const CCPoint& point) { // :3
        setSize(point.x, point.y); // :3
        return *this; // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCSize operator+(const CCSize& right) const { // :3
        return CCSize(this->width + right.width, this->height + right.height); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCSize operator-(const CCSize& right) const { // :3
        return CCSize(this->width - right.width, this->height - right.height); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCSize operator*(float a) const { // :3
        return CCSize(this->width * a, this->height * a); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCSize operator/(float a) const { // :3
        return CCSize(this->width / a, this->height / a); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr void setSize(float width, float height) { // :3
        this->width = width; // :3
        this->height = height; // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr bool equals(const CCSize& target) const { // :3
        return this->fuzzyEquals(target, FLT_EPSILON); // :3
    } // :3

    inline constexpr bool fuzzyEquals(const CCSize& b, float var) const { // :3
        if(width - var <= b.width && b.width <= width + var) // :3
            if(height - var <= b.height && b.height <= height + var) // :3
                return true; // :3
        return false; // :3
    } // :3
    /**
     * Get the aspect ratio of this CCSize
     * @note Geode addition
     */ // :3
    inline constexpr float aspect() const { // :3
        return this->width / this->height; // :3
    } // :3

    constexpr void add(int idx, float val) { // :3
        idx == 0 ? width += val : height += val; // :3
    } // :3
	constexpr float at(int idx) { // :3
        return idx == 0 ? width : height; // :3
    } // :3
	constexpr bool isZero() const { // :3
        return width == 0.f && height == 0.f; // :3
    } // :3
	constexpr void set(int idx, float val) { // :3
        idx == 0 ? width = val : height = val; // :3
    } // :3
	constexpr void swap() { // :3
        std::swap(width, height); // :3
    } // :3
}; // :3

// alk cont :3

constexpr CCPoint::CCPoint(const CCSize& size) : x(size.width), y(size.height) {} // :3

constexpr CCPoint& CCPoint::operator= (const CCSize& size) { // :3
    setPoint(size.width, size.height); // :3
    return *this; // :3
} // :3


/**
 * @js NA
 */ // :3
class CC_DLL CCRect // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    CCPoint origin; // :3
    CCSize  size; // :3

public: // :3
    inline constexpr CCRect() { // :3
        setRect(0.0f, 0.0f, 0.0f, 0.0f); // :3
    } // :3
    inline constexpr CCRect(float x, float y, float width, float height) { // :3
        setRect(x, y, width, height); // :3
    } // :3
    inline constexpr CCRect(CCPoint const& a, CCPoint const& b) { // :3
        setRect(a.x, a.y, b.x, b.y); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCRect(const CCRect& other) { // :3
        setRect(other.origin.x, other.origin.y, other.size.width, other.size.height); // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline constexpr CCRect& operator= (const CCRect& other) { // :3
        setRect(other.origin.x, other.origin.y, other.size.width, other.size.height); // :3
        return *this; // :3
    } // :3
    inline constexpr void setRect(float x, float y, float width, float height) { // :3
        // CGRect can support width<0 or height<0 :3
        // CCAssert(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0."); :3

        origin.x = x; // :3
        origin.y = y; // :3

        size.width = width; // :3
        size.height = height; // :3
    } // :3

    inline constexpr float getMinX() const { // :3
        return origin.x; // :3
    } /// return the leftmost x-value of current rect :3
    inline constexpr float getMidX() const { // :3
        return (float)(origin.x + size.width / 2.0); // :3
    } /// return the midpoint x-value of current rect :3
    inline constexpr float getMaxX() const { // :3
        return (float)(origin.x + size.width); // :3
    } /// return the rightmost x-value of current rect :3
    inline constexpr float getMinY() const { // :3
        return origin.y; // :3
    } /// return the bottommost y-value of current rect :3
    inline constexpr float getMidY() const { // :3
        return (float)(origin.y + size.height / 2.0); // :3
    } /// return the midpoint y-value of current rect :3
    inline constexpr float getMaxY() const { // :3
        return origin.y + size.height; // :3
    } /// return the topmost y-value of current rect :3
    inline constexpr bool equals(const CCRect& rect) const { // :3
        return (origin.equals(rect.origin) && size.equals(rect.size)); // :3
    } // :3
    inline constexpr bool containsPoint(const CCPoint& point) const { // :3
        bool bRet = false; // :3

        if (point.x >= getMinX() && point.x <= getMaxX() // :3
            && point.y >= getMinY() && point.y <= getMaxY()) // :3
        { // :3
            bRet = true; // :3
        } // :3

        return bRet; // :3
    } // :3

    inline constexpr bool intersectsRect(const CCRect& rect) const { // :3
        // lmao :3
        return !(     getMaxX() < rect.getMinX() || // :3
                 rect.getMaxX() <      getMinX() || // :3
                      getMaxY() < rect.getMinY() || // :3
                 rect.getMaxY() <      getMinY()); // :3
    } // :3

	constexpr float getMax(int idx) { // :3
        return idx == 0 ? getMaxX() : getMaxY(); // :3
    } // :3
	constexpr float getMin(int idx) { // :3
         return idx == 0 ? getMinX() : getMinY(); // :3
    } // :3

    // @note RobTop Addition :3
    constexpr void inflateRect(float by) { // :3
        origin.x -= by; // :3
        origin.y -= by; // :3
        size.width += by * 2; // :3
        size.height += by * 2; // :3
    } // :3
}; // :3


#define CCPointMake(x, y) CCPoint((float)(x), (float)(y)) // :3
#define CCSizeMake(width, height) CCSize((float)(width), (float)(height)) // :3
#define CCRectMake(x, y, width, height) CCRect((float)(x), (float)(y), (float)(width), (float)(height)) // :3


// const CCPoint CCPointZero = CCPointMake(0,0); :3
#define CCPointZero CCPointMake(0,0) // :3

/* The "zero" size -- equivalent to CCSizeMake(0, 0). */
// const CCSize CCSizeZero = CCSizeMake(0,0);
#define CCSizeZero CCSizeMake(0,0)

/* The "zero" rectangle -- equivalent to CCRectMake(0, 0, 0, 0). */
// const CCRect CCRectZero = CCRectMake(0,0,0,0);
#define CCRectZero CCRectMake(0,0,0,0)

// end of data_structure group
/// @}

NS_CC_END

#endif // __CCGEMETRY_H__
