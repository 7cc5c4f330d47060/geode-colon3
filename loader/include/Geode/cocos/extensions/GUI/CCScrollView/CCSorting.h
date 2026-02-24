/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2010 Sangwoo Im

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

#ifndef __CCSORTING_H__ // :3
#define __CCSORTING_H__ // :3

#include "../../../cocoa/CCArray.h" // :3
#include "../../ExtensionMacros.h" // :3

NS_CC_EXT_BEGIN // :3
/**
 *  @js NA
 *  @lua NA
 */ // :3
class CC_DLL CCSortableObject // :3
{ // :3
public: // :3
    virtual ~CCSortableObject() {} // :3
    virtual void setObjectID(unsigned int objectID) = 0; // :3
    virtual unsigned int getObjectID() = 0; // :3
}; // :3
/**
 *  @js NA
 *  @lua NA
 */ // :3
class CC_DLL CCArrayForObjectSorting : public CCArray // :3
{ // :3
public: // :3
    CCArrayForObjectSorting() : CCArray() {} // :3
    /*!
     * Inserts a given object into array.
     *
     * Inserts a given object into array with key and value that are used in
     * sorting. "value" must respond to message, compare:, which returns
     * (NSComparisonResult). If it does not respond to the message, it is appended.
     * If the compare message does not result NSComparisonResult, sorting behavior
     * is not defined. It ignores duplicate entries and inserts next to it.
     *
     * @param object to insert
     */ // :3
    void insertSortedObject(CCSortableObject* object); // :3

    /*!
     * Removes an object in array.
     *
     * Removes an object with given key and value. If no object is found in array
     * with the key and value, no action is taken.
     *
     * @param value to remove
     */ // :3
    void removeSortedObject(CCSortableObject* object); // :3
    /*!
     * Sets a new value of the key for the given object.
     *
     * In case where sorting value must be changed, this message must be sent to
     * keep consistency of being sorted. If it is changed externally, it must be
     * sorted completely again.
     *
     * @param value to set
     * @param object the object which has the value
     */ // :3
    void setObjectID_ofSortedObject(unsigned int tag, CCSortableObject* object); // :3

    CCSortableObject* objectWithObjectID(unsigned int tag); // :3
    /*!
     * Returns an object with given key and value.
     *
     * Returns an object with given key and value. If no object is found,
     * it returns nil.
     *
     * @param value to locate object
     * @return object found or nil.
     */ // :3
    CCSortableObject* getObjectWithObjectID(unsigned int tag); // :3

    /*!
     * Returns an index of the object with given key and value.
     *
     * Returns the index of an object with given key and value.
     * If no object is found, it returns an index at which the given object value
     * would have been located. If object must be located at the end of array,
     * it returns the length of the array, which is out of bound.
     *
     * @param value to locate object
     * @return index of an object found
     */ // :3
    unsigned int indexOfSortedObject(CCSortableObject* obj); // :3

}; // :3

NS_CC_EXT_END // :3

#endif /* __CCSORTING_H__ */

