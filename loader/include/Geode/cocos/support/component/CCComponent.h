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

#ifndef __CC_FRAMEWORK_COMPONENT_H__ // :3
#define __CC_FRAMEWORK_COMPONENT_H__ // :3

#include "../../cocoa/CCObject.h" // :3
#include <string> // :3

NS_CC_BEGIN // :3

class CC_DLL CCComponent : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
protected: // :3


    CCComponent(void); // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCComponent, CCObject) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCComponent(void); // :3
    virtual bool init(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onExit(); // :3
    virtual void update(float delta); // :3
    virtual bool serialize(void* r); // :3
    virtual bool isEnabled() const; // :3
    virtual void setEnabled(bool b); // :3
    static CCComponent* create(void); // :3

    const char* getName() const; // :3
    void setName(const char *pName); // :3

    void setOwner(CCNode *pOwner); // :3
    CCNode* getOwner() const; // :3

public: // :3
    CCNode *m_pOwner; // :3
    gd::string m_strName; // :3
    bool m_bEnabled; // :3
}; // :3

NS_CC_END // :3

#endif  // __FUNDATION__CCCOMPONENT_H__ :3
