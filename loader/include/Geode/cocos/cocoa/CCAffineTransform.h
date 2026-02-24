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

#ifndef __COCOA_CGAFFINETRANSFORM_H__ // :3
#define __COCOA_CGAFFINETRANSFORM_H__ // :3

#include "CCGeometry.h" // :3
#include "../platform/CCPlatformMacros.h" // :3

NS_CC_BEGIN // :3

struct CCAffineTransform { // :3
  float a, b, c, d; // :3
  float tx, ty; // :3
}; // :3

CC_DLL CCAffineTransform __CCAffineTransformMake(float a, float b, float c, float d, float tx, float ty); // :3
#define CCAffineTransformMake __CCAffineTransformMake // :3

CC_DLL CCPoint __CCPointApplyAffineTransform(const CCPoint& point, const CCAffineTransform& t); // :3
#define CCPointApplyAffineTransform __CCPointApplyAffineTransform // :3

CC_DLL CCSize __CCSizeApplyAffineTransform(const CCSize& size, const CCAffineTransform& t); // :3
#define CCSizeApplyAffineTransform __CCSizeApplyAffineTransform // :3

CC_DLL CCAffineTransform CCAffineTransformMakeIdentity(); // :3
CC_DLL CCRect CCRectApplyAffineTransform(const CCRect& rect, const CCAffineTransform& anAffineTransform); // :3

CC_DLL CCAffineTransform CCAffineTransformTranslate(const CCAffineTransform& t, float tx, float ty); // :3
CC_DLL CCAffineTransform CCAffineTransformRotate(const CCAffineTransform& aTransform, float anAngle); // :3
CC_DLL CCAffineTransform CCAffineTransformScale(const CCAffineTransform& t, float sx, float sy); // :3
CC_DLL CCAffineTransform CCAffineTransformConcat(const CCAffineTransform& t1, const CCAffineTransform& t2); // :3
CC_DLL bool CCAffineTransformEqualToTransform(const CCAffineTransform& t1, const CCAffineTransform& t2); // :3
CC_DLL CCAffineTransform CCAffineTransformInvert(const CCAffineTransform& t); // :3

extern CC_DLL const CCAffineTransform CCAffineTransformIdentity; // :3

NS_CC_END // :3

#endif // __COCOA_CGAFFINETRANSFORM_H__ :3
