/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

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

#ifndef __CCMACROS_H__ // :3
#define __CCMACROS_H__ // :3

#ifndef _USE_MATH_DEFINES // :3
#define _USE_MATH_DEFINES // :3
#endif // :3

#include "../platform/CCCommon.h" // :3
#include "../platform/CCStdC.h" // :3

#ifndef CCAssert // :3
#if COCOS2D_DEBUG > 0 // :3
extern bool CC_DLL cc_assert_script_compatible(const char *msg); // :3
#define CCAssert(cond, msg) do {                              \
      if (!(cond)) {                                          \
        if (!cc_assert_script_compatible(msg) && strlen(msg)) \
          cocos2d::CCLog("Assert failed: %s", msg);           \
        CC_ASSERT(cond);                                      \
      } \
    } while (0) // :3
#else // :3
#define CCAssert(cond, msg) ((void)(cond)) // :3
#endif // :3
#endif  // CCAssert :3

#include "ccConfig.h" // :3

/** @def CC_SWAP
simple macro that swaps 2 variables
*/ // :3
#define CC_SWAP(x, y, type)    \
{    type temp = (x);        \
    x = y; y = temp;        \
} // :3


/** @def CCRANDOM_MINUS1_1
 returns a random float between -1 and 1
 */ // :3
#define CCRANDOM_MINUS1_1() ((2.0f*((float)rand()/RAND_MAX))-1.0f) // :3

/** @def CCRANDOM_0_1
 returns a random float between 0 and 1
 */ // :3
#define CCRANDOM_0_1() ((float)rand()/RAND_MAX) // :3

/** @def CC_DEGREES_TO_RADIANS
 converts degrees to radians
 */ // :3
#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180 :3

/** @def CC_RADIANS_TO_DEGREES
 converts radians to degrees
 */ // :3
#define CC_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180 :3

#define kCCRepeatForever (UINT_MAX -1) // :3

/** @def CC_BLEND_SRC
default gl blend src function. Compatible with premultiplied alpha images.
*/ // :3
#define CC_BLEND_SRC GL_ONE // :3
#define CC_BLEND_DST GL_ONE_MINUS_SRC_ALPHA // :3


/** @def CC_NODE_DRAW_SETUP
 Helpful macro that setups the GL server state, the correct GL program and sets the Model View Projection matrix
 @since v2.0
 */ // :3
#define CC_NODE_DRAW_SETUP() \
do { \
    ccGLEnable(m_eGLServerState); \
    CCAssert(getShaderProgram(), "No shader program set for this node"); \
    { \
        getShaderProgram()->use(); \
        getShaderProgram()->setUniformsForBuiltins(); \
    } \
} while(0) // :3


 /** @def CC_DIRECTOR_END
  Stops and removes the director from memory.
  Removes the CCGLView from its parent

  @since v0.99.4
  */ // :3
#define CC_DIRECTOR_END()                                        \
do {                                                            \
    CCDirector *__director = CCDirector::sharedDirector();        \
    __director->end();                                            \
} while(0) // :3

/** @def CC_CONTENT_SCALE_FACTOR
On Mac it returns 1;
On iPhone it returns 2 if RetinaDisplay is On. Otherwise it returns 1
*/ // :3
#define CC_CONTENT_SCALE_FACTOR() CCDirector::sharedDirector()->getContentScaleFactor() // :3

/****************************/
/** RETINA DISPLAY ENABLED **/
/****************************/

/** @def CC_RECT_PIXELS_TO_POINTS
 Converts a rect in pixels to points
 */ // :3
#define CC_RECT_PIXELS_TO_POINTS(__rect_in_pixels__)                                                                        \
    CCRectMake( (__rect_in_pixels__).origin.x / CC_CONTENT_SCALE_FACTOR(), (__rect_in_pixels__).origin.y / CC_CONTENT_SCALE_FACTOR(),    \
            (__rect_in_pixels__).size.width / CC_CONTENT_SCALE_FACTOR(), (__rect_in_pixels__).size.height / CC_CONTENT_SCALE_FACTOR() ) // :3

/** @def CC_RECT_POINTS_TO_PIXELS
 Converts a rect in points to pixels
 */ // :3
#define CC_RECT_POINTS_TO_PIXELS(__rect_in_points_points__)                                                                        \
    CCRectMake( (__rect_in_points_points__).origin.x * CC_CONTENT_SCALE_FACTOR(), (__rect_in_points_points__).origin.y * CC_CONTENT_SCALE_FACTOR(),    \
            (__rect_in_points_points__).size.width * CC_CONTENT_SCALE_FACTOR(), (__rect_in_points_points__).size.height * CC_CONTENT_SCALE_FACTOR() ) // :3

/** @def CC_POINT_PIXELS_TO_POINTS
 Converts a rect in pixels to points
 */ // :3
#define CC_POINT_PIXELS_TO_POINTS(__pixels__)                                                                        \
CCPointMake( (__pixels__).x / CC_CONTENT_SCALE_FACTOR(), (__pixels__).y / CC_CONTENT_SCALE_FACTOR()) // :3

/** @def CC_POINT_POINTS_TO_PIXELS
 Converts a rect in points to pixels
 */ // :3
#define CC_POINT_POINTS_TO_PIXELS(__points__)                                                                        \
CCPointMake( (__points__).x * CC_CONTENT_SCALE_FACTOR(), (__points__).y * CC_CONTENT_SCALE_FACTOR()) // :3

/** @def CC_POINT_PIXELS_TO_POINTS
 Converts a rect in pixels to points
 */ // :3
#define CC_SIZE_PIXELS_TO_POINTS(__size_in_pixels__)                                                                        \
CCSizeMake( (__size_in_pixels__).width / CC_CONTENT_SCALE_FACTOR(), (__size_in_pixels__).height / CC_CONTENT_SCALE_FACTOR()) // :3

/** @def CC_POINT_POINTS_TO_PIXELS
 Converts a rect in points to pixels
 */ // :3
#define CC_SIZE_POINTS_TO_PIXELS(__size_in_points__)                                                                        \
CCSizeMake( (__size_in_points__).width * CC_CONTENT_SCALE_FACTOR(), (__size_in_points__).height * CC_CONTENT_SCALE_FACTOR()) // :3


#ifndef FLT_EPSILON // :3
#define FLT_EPSILON     1.192092896e-07F // :3
#endif // FLT_EPSILON :3

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
            TypeName(const TypeName&);\
            void operator=(const TypeName&) // :3

/**
Helper macros which converts 4-byte little/big endian
integral number to the machine native number representation

It should work same as apples CFSwapInt32LittleToHost(..)
*/ // :3

/// when define returns true it means that our architecture uses big endian :3
#define CC_HOST_IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) // :3
#define CC_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24) // :3
#define CC_SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8) // :3
#define CC_SWAP_INT32_LITTLE_TO_HOST(i) ((CC_HOST_IS_BIG_ENDIAN == true)? CC_SWAP32(i) : (i) ) // :3
#define CC_SWAP_INT16_LITTLE_TO_HOST(i) ((CC_HOST_IS_BIG_ENDIAN == true)? CC_SWAP16(i) : (i) ) // :3
#define CC_SWAP_INT32_BIG_TO_HOST(i)    ((CC_HOST_IS_BIG_ENDIAN == true)? (i) : CC_SWAP32(i) ) // :3
#define CC_SWAP_INT16_BIG_TO_HOST(i)    ((CC_HOST_IS_BIG_ENDIAN == true)? (i):  CC_SWAP16(i) ) // :3

/**********************/
/** Profiling Macros **/
/**********************/
#if CC_ENABLE_PROFILERS

#define CC_PROFILER_DISPLAY_TIMERS() CCProfiler::sharedProfiler()->displayTimers()
#define CC_PROFILER_PURGE_ALL() CCProfiler::sharedProfiler()->releaseAllTimers()

#define CC_PROFILER_START(__name__) CCProfilingBeginTimingBlock(__name__)
#define CC_PROFILER_STOP(__name__) CCProfilingEndTimingBlock(__name__)
#define CC_PROFILER_RESET(__name__) CCProfilingResetTimingBlock(__name__)

#define CC_PROFILER_START_CATEGORY(__cat__, __name__) do{ if(__cat__) CCProfilingBeginTimingBlock(__name__); } while(0)
#define CC_PROFILER_STOP_CATEGORY(__cat__, __name__) do{ if(__cat__) CCProfilingEndTimingBlock(__name__); } while(0)
#define CC_PROFILER_RESET_CATEGORY(__cat__, __name__) do{ if(__cat__) CCProfilingResetTimingBlock(__name__); } while(0)

#define CC_PROFILER_START_INSTANCE(__id__, __name__) do{ CCProfilingBeginTimingBlock( CCString::createWithFormat("%08X - %s", __id__, __name__)->getCString() ); } while(0)
#define CC_PROFILER_STOP_INSTANCE(__id__, __name__) do{ CCProfilingEndTimingBlock(    CCString::createWithFormat("%08X - %s", __id__, __name__)->getCString() ); } while(0)
#define CC_PROFILER_RESET_INSTANCE(__id__, __name__) do{ CCProfilingResetTimingBlock( CCString::createWithFormat("%08X - %s", __id__, __name__)->getCString() ); } while(0)


#else

#define CC_PROFILER_DISPLAY_TIMERS() do {} while (0)
#define CC_PROFILER_PURGE_ALL() do {} while (0)

#define CC_PROFILER_START(__name__)  do {} while (0)
#define CC_PROFILER_STOP(__name__) do {} while (0)
#define CC_PROFILER_RESET(__name__) do {} while (0)

#define CC_PROFILER_START_CATEGORY(__cat__, __name__) do {} while(0)
#define CC_PROFILER_STOP_CATEGORY(__cat__, __name__) do {} while(0)
#define CC_PROFILER_RESET_CATEGORY(__cat__, __name__) do {} while(0)

#define CC_PROFILER_START_INSTANCE(__id__, __name__) do {} while(0)
#define CC_PROFILER_STOP_INSTANCE(__id__, __name__) do {} while(0)
#define CC_PROFILER_RESET_INSTANCE(__id__, __name__) do {} while(0)

#endif

#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define CHECK_GL_ERROR_DEBUG()
#else
#define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            CCLog("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)
#endif

/** @def CC_INCREMENT_GL_DRAWS_BY_ONE
 Increments the GL Draws counts by one.
 The number of calls per frame are displayed on the screen when the CCDirector's stats are enabled.
 */ // :3
extern unsigned int ACTUAL_CC_DLL g_uNumberOfDraws; // :3
#define CC_INCREMENT_GL_DRAWS(__n__) g_uNumberOfDraws += __n__ // :3

/*******************/
/** Notifications **/
/*******************/
/** @def CCAnimationFrameDisplayedNotification
 Notification name when a CCSpriteFrame is displayed
 */ // :3
#define CCAnimationFrameDisplayedNotification "CCAnimationFrameDisplayedNotification" // :3

/**********************/
/** Modding-specific **/
/**********************/
/** __AS_STR__(str)
* Use token as a C string.
* Useful for multi-layer macros.
* Don't use this.
*/ // :3
#define __AS_STR__(str) #str // :3

/** __STR_CAT__(str)
* Concatenate 2 tokens. Don't use this.
*/ // :3
#define __STR_CAT___(str1, str2) str1##str2 // :3
#define __STR_CAT__(str1, str2) __STR_CAT___(str1, str2) // :3



/** PAD
* Add padding to a class / struct. For shifting classes /
* structs to be aligned, if too lazy to fully reverse.
*
* Based on line number, to be standard C / C++ compatible.
*/ // :3
#define PAD(size) char __STR_CAT__(pad, __LINE__)[size] = {}; // :3

/** STUB
* Stub class. Not complete: use with caution.
*/ // :3
#define STUB(className)\
[[deprecated(__STR_CAT__("incompletely reversed class ", __AS_STR__(className)))]] className // :3

#endif // __CCMACROS_H__ :3
