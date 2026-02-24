/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

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

#ifndef __CC_SET_H__ // :3
#define __CC_SET_H__ // :3

#include <set> // :3
#include "CCObject.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup data_structures
 * @{
 */ // :3

typedef gd::set<CCObject *>::iterator CCSetIterator; // :3

class CC_DLL CCSet : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js  ctor
     * @lua NA
     */ // :3
    CCSet(void); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSet, CCObject) // :3
    /**
     * @lua NA
     */ // :3
    // CCSet(const CCSet &rSetObject); :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCSet(void); // :3

    /**
    * @brief Create and return a new empty set.
    * @lua NA
    */ // :3
    static CCSet * create(); // :3

    /**
    *@brief Return a copy of the CCSet, it will copy all the elements.
    */ // :3
    CCSet* copy(); // :3
    /**
    *@brief It is the same as copy().
    *@lua NA
    */ // :3
    CCSet* mutableCopy(); // :3
    /**
    *@brief Return the number of elements the CCSet contains.
    */ // :3
    int count(); // :3
    /**
    *@brief Add a element into CCSet, it will retain the element.
    */ // :3
    void addObject(CCObject *pObject); // :3
    /**
    *@brief Remove the given element, nothing todo if no element equals pObject.
    */ // :3
    void removeObject(CCObject *pObject); // :3
    /**
     *@brief Remove all elements of the set
     */ // :3
    void removeAllObjects(); // :3
    /**
    *@brief Check if CCSet contains a element equals pObject.
    */ // :3
    bool containsObject(CCObject *pObject); // :3
    /**
    *@brief Return the iterator that points to the first element.
    *@js NA
    *@lua NA
    */ // :3
    CCSetIterator begin(); // :3
    /**
    *@brief Return the iterator that points to the position after the last element.
    *@js NA
    *@lua NA
    */ // :3
    CCSetIterator end(); // :3
    /**
    *@brief Return the first element if it contains elements, or null if it doesn't contain any element.
    */ // :3
    CCObject* anyObject(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual void acceptVisitor(CCDataVisitor &visitor); // :3

public: // :3
    gd::set<CCObject *> *m_pSet; // :3
}; // :3

// end of data_structure group :3
/// @} :3

NS_CC_END // :3

#endif // __CC_SET_H__ :3

