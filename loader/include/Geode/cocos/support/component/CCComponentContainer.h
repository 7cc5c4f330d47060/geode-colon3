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

#ifndef __CC_FRAMEWORK_COMCONTAINER_H__ // :3
#define __CC_FRAMEWORK_COMCONTAINER_H__ // :3

#include "../../cocoa/CCDictionary.h" // :3

NS_CC_BEGIN // :3

class CCComponent; // :3
class CCNode; // :3
/**
 *  @js NA
 *  @lua NA
 */ // :3
class CC_DLL CCComponentContainer // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
protected: // :3


    CCComponentContainer(CCNode *pNode); // :3

public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCComponentContainer) // :3
    CCComponentContainer() {} // :3

    virtual ~CCComponentContainer(void); // :3
    virtual CCComponent* get(const char *pName) const; // :3
    virtual bool add(CCComponent *pCom); // :3
    virtual bool remove(const char *pName); // :3
    virtual bool remove(CCComponent *pCom); // :3
    virtual void removeAll(); // :3
    virtual void visit(float fDelta); // :3
public: // :3
    bool isEmpty() const; // :3

private: // :3
    void alloc(void); // :3

public: // :3
    CCDictionary *m_pComponents;        ///< Dictionary of components :3
    CCNode *m_pOwner; // :3

    friend class CCNode; // :3
}; // :3

NS_CC_END // :3

#endif  // __FUNDATION__CCCOMPONENT_H__ :3
