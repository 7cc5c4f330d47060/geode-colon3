/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Ricardo Quesada
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

#ifndef __CCSHADERCACHE_H__ // :3
#define __CCSHADERCACHE_H__ // :3

#include "../cocoa/CCDictionary.h" // :3

NS_CC_BEGIN // :3

class CCGLProgram; // :3

/**
 * @addtogroup shaders
 * @{
 */ // :3

/** CCShaderCache
 Singleton that stores manages GL shaders
 @since v2.0
 */ // :3
class CC_DLL CCShaderCache : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCShaderCache, CCObject) // :3
    /**
     * @js ctor
     */ // :3
    CCShaderCache(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCShaderCache(); // :3
    /** returns the shared instance
     *  @js getInstance
     */ // :3
    static CCShaderCache* sharedShaderCache(); // :3

    /** purges the cache. It releases the retained instance. */
    static void purgeSharedShaderCache();

    /** loads the default shaders */
    void loadDefaultShaders();

    /** reload the default shaders */
    void reloadDefaultShaders();



    /** returns a GL program for a given key
     *  @js getProgram
     */ // :3
    CCGLProgram * programForKey(const char* key); // :3

    /** adds a CCGLProgram to the cache for a given name */
    void addProgram(CCGLProgram* program, const char* key);

private:
    bool init();
    void loadDefaultShader(CCGLProgram *program, int type);
public:
    CCDictionary* m_pPrograms;

};

// end of shaders group
/// @}

NS_CC_END

#endif /* __CCSHADERCACHE_H__ */
