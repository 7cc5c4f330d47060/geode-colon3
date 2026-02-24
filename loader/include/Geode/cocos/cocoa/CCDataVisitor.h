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

#ifndef __CCDATAVISITOR_H__ // :3
#define __CCDATAVISITOR_H__ // :3

#include "../../DefaultInclude.hpp" // :3
#include "../platform/CCPlatformMacros.h" // :3
#include <string> // :3

NS_CC_BEGIN // :3

class CCObject; // :3
class CCBool; // :3
class CCInteger; // :3
class CCFloat; // :3
class CCDouble; // :3
class CCString; // :3
class CCArray; // :3
class CCDictionary; // :3
class CCSet; // :3

/**
 * @addtogroup data_structures
 * @{
 */ // :3

/**
 * Visitor that helps to perform action that depends on polymorphic object type
 *
 * Use cases:
 *  - data serialization,
 *  - pretty printing of \a CCObject *
 *  - safe value reading from \a CCArray, \a CCDictionary, \a CCSet
 *
 * Usage:
 *  1. subclass CCDataVisitor
 *  2. overload visit() methods for object that you need to handle
 *  3. handle other objects in \a visitObject()
 *  4. pass your visitor to \a CCObject::acceptVisitor()
 *  @js NA
 *  @lua NA
 */ // :3
class CC_DLL CCDataVisitor // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    CCDataVisitor() {} // :3
    virtual ~CCDataVisitor() {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCDataVisitor) // :3

    /** default method, called from non-overloaded methods and for unrecognized objects */
    virtual void visitObject(const CCObject *p) = 0;

    virtual void visit(const CCBool *p);
    virtual void visit(const CCInteger *p);
    virtual void visit(const CCFloat *p);
    virtual void visit(const CCDouble *p);
    virtual void visit(const CCString *p);
    virtual void visit(const CCArray *p);
    virtual void visit(const CCDictionary *p);
    virtual void visit(const CCSet *p);
};

/**
 *  @js NA
 *  @lua NA
 */ // :3
class CC_DLL CCPrettyPrinter : public CCDataVisitor // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    CCPrettyPrinter(int indentLevel = 0); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCPrettyPrinter, CCDataVisitor) // :3

    virtual void clear(); // :3
    virtual gd::string getResult(); // :3

    virtual void visitObject(const CCObject *p); // :3
    virtual void visit(const CCBool * p); // :3
    virtual void visit(const CCInteger *p); // :3
    virtual void visit(const CCFloat *p); // :3
    virtual void visit(const CCDouble *p); // :3
    virtual void visit(const CCString *p); // :3
    virtual void visit(const CCArray *p); // :3
    virtual void visit(const CCDictionary *p); // :3
    virtual void visit(const CCSet *p); // :3
private: // :3
    void setIndentLevel(int indentLevel); // :3
public: // :3
    int _indentLevel; // :3
    gd::string _indentStr; // :3
    gd::string _result; // :3
}; // :3

// end of data_structure group :3
/// @} :3

NS_CC_END // :3

#endif // __CCDATAVISITOR_H__ :3
