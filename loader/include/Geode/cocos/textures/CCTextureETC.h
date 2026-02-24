/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org

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

#ifndef __CCETCTEXTURE_H__ // :3
#define __CCETCTEXTURE_H__ // :3

#include "../cocoa/CCObject.h" // :3
#include "../platform/CCPlatformMacros.h" // :3
#include "../platform/CCGL.h" // :3

NS_CC_BEGIN // :3
/**
 *  @js NA
 *  @lua NA
 */ // :3
class CC_DLL CCTextureETC : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTextureETC, CCObject) // :3
    CCTextureETC(); // :3
    virtual ~CCTextureETC(); // :3

    bool initWithFile(const char* file); // :3

    unsigned int getName() const; // :3
    unsigned int getWidth() const; // :3
    unsigned int getHeight() const; // :3

private: // :3
    bool loadTexture(const char* file); // :3

public: // :3
    GLuint _name; // :3
    unsigned int _width; // :3
    unsigned int _height; // :3
}; // :3

NS_CC_END // :3

#endif /* defined(__CCETCTEXTURE_H__) */
