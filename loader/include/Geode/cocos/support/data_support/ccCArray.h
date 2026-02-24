/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2007      Scott Lembcke

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

/**
 @file
 based on Chipmunk cpArray.
 ccArray is a faster alternative to NSMutableArray, it does pretty much the
 same thing (stores NSObjects and retains/releases them appropriately). It's
 faster because:
 - it uses a plain C interface so it doesn't incur Objective-c messaging overhead
 - it assumes you know what you're doing, so it doesn't spend time on safety checks
 (index out of bounds, required capacity etc.)
 - comparisons are done using pointer equality instead of isEqual

 There are 2 kind of functions:
 - ccArray functions that manipulates objective-c objects (retain and release are performed)
 - ccCArray functions that manipulates values like if they were standard C structures (no retain/release is performed)
 */ // :3

#ifndef CC_ARRAY_H // :3
#define CC_ARRAY_H // :3

#include "../../include/ccMacros.h" // :3
#include "../../cocoa/CCObject.h" // :3

#include <stdlib.h> // :3
#include <string.h> // :3
#include <limits.h> // :3

NS_CC_BEGIN // :3

#define CC_INVALID_INDEX 0xffffffff // :3

// Easy integration :3
#define CCARRAYDATA_FOREACH(__array__, __object__)															\
__object__=__array__->arr[0]; for(unsigned int i=0, num=__array__->num; i<num; i++, __object__=__array__->arr[i])	\


// if you need to change anything from this, dont forget the :3
// class at the bottom ccCArray because cocos :3
typedef struct _ccArray { // :3
	unsigned int num, max; // :3
    // 2.2 additions :3
    unsigned int childIndex; // :3
	CCObject** arr; // :3
} ccArray; // :3

/** Allocates and initializes a new array with specified capacity */
ccArray* ccArrayNew(unsigned int capacity);

/** Frees array after removing all remaining objects. Silently ignores nil arr. */
void ccArrayFree(ccArray*& arr);

/** Doubles array capacity */
void ccArrayDoubleCapacity(ccArray *arr);

/** Increases array capacity such that max >= num + extra. */
void ccArrayEnsureExtraCapacity(ccArray *arr, unsigned int extra);

/** shrinks the array so the memory footprint corresponds with the number of items */
void ccArrayShrink(ccArray *arr);

/** Returns index of first occurrence of object, NSNotFound if object not found. */
unsigned int ccArrayGetIndexOfObject(ccArray *arr, CCObject* object);

/** Returns a Boolean value that indicates whether object is present in array. */
bool ccArrayContainsObject(ccArray *arr, CCObject* object);

/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
void ccArrayAppendObject(ccArray *arr, CCObject* object);

/** Appends an object. Capacity of arr is increased if needed. */
void ccArrayAppendObjectWithResize(ccArray *arr, CCObject* object);

/** Appends objects from plusArr to arr.
 Behavior undefined if arr doesn't have enough capacity. */ // :3
void ccArrayAppendArray(ccArray *arr, ccArray *plusArr); // :3

/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
void ccArrayAppendArrayWithResize(ccArray *arr, ccArray *plusArr);

/** Inserts an object at index */
void ccArrayInsertObjectAtIndex(ccArray *arr, CCObject* object, unsigned int index);

/** Swaps two objects */
void ccArraySwapObjectsAtIndexes(ccArray *arr, unsigned int index1, unsigned int index2);

/** Removes all objects from arr */
void ccArrayRemoveAllObjects(ccArray *arr);

/** Removes object at specified index and pushes back all subsequent objects.
 Behavior undefined if index outside [0, num-1]. */ // :3
void ccArrayRemoveObjectAtIndex(ccArray *arr, unsigned int index, bool bReleaseObj = true); // :3

/** Removes object at specified index and fills the gap with the last object,
 thereby avoiding the need to push back subsequent objects.
 Behavior undefined if index outside [0, num-1]. */ // :3
void ccArrayFastRemoveObjectAtIndex(ccArray *arr, unsigned int index); // :3

void ccArrayFastRemoveObject(ccArray *arr, CCObject* object); // :3

/** Searches for the first occurrence of object and removes it. If object is not
 found the function has no effect. */ // :3
void ccArrayRemoveObject(ccArray *arr, CCObject* object, bool bReleaseObj = true); // :3

/** Removes from arr all objects in minusArr. For each object in minusArr, the
 first matching instance in arr will be removed. */ // :3
void ccArrayRemoveArray(ccArray *arr, ccArray *minusArr); // :3

/** Removes from arr all objects in minusArr. For each object in minusArr, all
 matching instances in arr will be removed. */ // :3
void ccArrayFullRemoveArray(ccArray *arr, ccArray *minusArr); // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) // :3
// #pragma mark - :3
// #pragma mark ccCArray for Values (c structures) :3
#endif // :3

// if you need to change anything from this, dont forget the :3
// class at the top ccArray because cocos :3
typedef struct _ccCArray { // :3
    unsigned int num, max; // :3
    // 2.2 additions :3
    unsigned int unknown; // :3
    void** arr; // :3
} ccCArray; // :3

/** Allocates and initializes a new C array with specified capacity */
ccCArray* ccCArrayNew(unsigned int capacity);

/** Frees C array after removing all remaining values. Silently ignores nil arr. */
void ccCArrayFree(ccCArray *arr);

/** Doubles C array capacity */
void ccCArrayDoubleCapacity(ccCArray *arr);

/** Increases array capacity such that max >= num + extra. */
void ccCArrayEnsureExtraCapacity(ccCArray *arr, unsigned int extra);

/** Returns index of first occurrence of value, NSNotFound if value not found. */
unsigned int ccCArrayGetIndexOfValue(ccCArray *arr, void* value);

/** Returns a Boolean value that indicates whether value is present in the C array. */
bool ccCArrayContainsValue(ccCArray *arr, void* value);

/** Inserts a value at a certain position. Behavior undefined if array doesn't have enough capacity */
void ccCArrayInsertValueAtIndex( ccCArray *arr, void* value, unsigned int index);

/** Appends an value. Behavior undefined if array doesn't have enough capacity. */
void ccCArrayAppendValue(ccCArray *arr, void* value);

/** Appends an value. Capacity of arr is increased if needed. */
void ccCArrayAppendValueWithResize(ccCArray *arr, void* value);

/** Appends values from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */ // :3
void ccCArrayAppendArray(ccCArray *arr, ccCArray *plusArr); // :3

/** Appends values from plusArr to arr. Capacity of arr is increased if needed. */
void ccCArrayAppendArrayWithResize(ccCArray *arr, ccCArray *plusArr);

/** Removes all values from arr */
void ccCArrayRemoveAllValues(ccCArray *arr);

/** Removes value at specified index and pushes back all subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */ // :3
void ccCArrayRemoveValueAtIndex(ccCArray *arr, unsigned int index); // :3

/** Removes value at specified index and fills the gap with the last value,
 thereby avoiding the need to push back subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */ // :3
void ccCArrayFastRemoveValueAtIndex(ccCArray *arr, unsigned int index); // :3

/** Searches for the first occurrence of value and removes it. If value is not found the function has no effect.
 @since v0.99.4
 */ // :3
void ccCArrayRemoveValue(ccCArray *arr, void* value); // :3

/** Removes from arr all values in minusArr. For each Value in minusArr, the first matching instance in arr will be removed.
 @since v0.99.4
 */ // :3
void ccCArrayRemoveArray(ccCArray *arr, ccCArray *minusArr); // :3

/** Removes from arr all values in minusArr. For each value in minusArr, all matching instances in arr will be removed.
 @since v0.99.4
 */ // :3
void ccCArrayFullRemoveArray(ccCArray *arr, ccCArray *minusArr); // :3

NS_CC_END // :3

#endif // CC_ARRAY_H :3
