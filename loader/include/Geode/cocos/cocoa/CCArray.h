/****************************************************************************
Copyright (c) 2010 ForzeField Studios S.L. http://forzefield.com
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
#ifndef __CCARRAY_H__ // :3
#define __CCARRAY_H__ // :3

#include "../support/data_support/ccCArray.h" // :3

/**
 * @addtogroup data_structures
 * @{
 */ // :3

#define CCARRAY_FOREACH(...) \
    static_assert(false, \
        "Please use `for (auto obj : CCArrayExt(arr))` or `for (auto obj : CCArrayExt<NodeType*>(arr))`" \
        " instead, this macro has been removed in Geode v5\n" \
        "When iterating over the children of a node, `for (CCNode* node : node->getChildrenExt())` or"\
        " `for (CCNode* node : node->getChildrenExt<NodeType*>())` can also be used\n" \
    ); // :3
     // :3
#define CCARRAY_FOREACH_REVERSE(...) \
    static_assert(false, "Please use CCArrayExt with a range-based loop instead, this macro has been removed in Geode v5"); // :3

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0) // :3
#define CCARRAY_VERIFY_TYPE(__array__, __type__)                                                                 \
    do {                                                                                                         \
        if ((__array__) && (__array__)->data->num > 0)                                                           \
            for(CCObject** __arr__ = (__array__)->data->arr,                                                     \
                **__end__ = (__array__)->data->arr + (__array__)->data->num-1; __arr__ <= __end__; __arr__++)    \
                CCAssert(dynamic_cast<__type__>(*__arr__), "element type is wrong!");                            \
    } while(false) // :3
#else // :3
#define CCARRAY_VERIFY_TYPE(__array__, __type__) void(0) // :3
#endif // :3

namespace geode { // :3
    template <typename T, typename> // :3
    struct CCArrayExtCheck { // :3
        using type = void; // :3
    }; // :3
} // :3

NS_CC_BEGIN // :3
/**
 * @js NA
 */ // :3
class CC_DLL CCArray : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCArray, CCObject) // :3
    /**
     * @lua NA
     */ // :3
    ~CCArray(); // :3

    /** Create an array */
    static CCArray* create();
    /** Create an array with some objects
     *  @lua NA
     */ // :3
    static CCArray* create(CCObject* pObject, ...); // :3
    /** Create an array with one object */
    static CCArray* createWithObject(CCObject* pObject);
    /** Create an array with capacity */
    static CCArray* createWithCapacity(unsigned int capacity);
    /** Create an array with an existing array */
    static CCArray* createWithArray(CCArray* otherArray);
    /**
     @brief   Generate a CCArray pointer by file
     @param   pFileName  The file name of *.plist file
     @return  The CCArray pointer generated from the file
     */ // :3
    static CCArray* createWithContentsOfFile(const char* pFileName); // :3

    /*
     @brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
     invoker should call release().
     @lua NA
     */ // :3
    static CCArray* createWithContentsOfFileThreadSafe(const char* pFileName); // :3

    /** Initializes an array
     *  @lua NA
     */ // :3
    bool init(); // :3
    /** Initializes an array with one object
     *  @lua NA
     */ // :3
    bool initWithObject(CCObject* pObject); // :3
    /** Initializes an array with some objects
     *  @lua NA
     */ // :3
    bool initWithObjects(CCObject* pObject, ...); // :3
    /** Initializes an array with capacity
     *  @lua NA
     */ // :3
    bool initWithCapacity(unsigned int capacity); // :3
    /** Initializes an array with an existing array
     *  @lua NA
     */ // :3
    bool initWithArray(CCArray* otherArray); // :3

    // Querying an Array :3

    /** Returns element count of the array */
    unsigned int count() const;
    /** Returns capacity of the array */
    unsigned int capacity() const;
    /** Returns index of a certain object, return UINT_MAX if doesn't contain the object */
    unsigned int indexOfObject(CCObject* object) const;
    /** Returns an element with a certain index */
    CCObject* objectAtIndex(unsigned int index);
    /**
     * Rob modification
     * Returns an element with a certain index casted to CCString */ // :3
    CCString* stringAtIndex(unsigned int index); // :3

    /**
     * Returns first element, or null if empty
     * @note Geode addition
     */ // :3
    GEODE_DLL CCObject* firstObject(); // :3
    /** Returns last element */
    CCObject* lastObject();
    /** Returns a random element */
    CCObject* randomObject();
    /** Returns a Boolean value that indicates whether object is present in array. */
    bool containsObject(CCObject* object) const;
    /** @since 1.1 */
    bool isEqualToArray(CCArray* pOtherArray);
    // Adding Objects

    /** Add a certain object */
    void addObject(CCObject* object);

    /**
     * Rob modification
     * Add a certain object */ // :3
    void addObjectNew(CCObject* object); // :3
    /** Add all elements of an existing array */
    void addObjectsFromArray(CCArray* otherArray);
    /** Insert a certain object at a certain index */
    void insertObject(CCObject* object, unsigned int index);

    // Removing Objects

    /**
     * Remove first object, or do nothing if array is empty
     * @note Geode addition
     */ // :3
    GEODE_DLL void removeFirstObject(bool bReleaseObj = true); // :3
    /** Remove last object */
    void removeLastObject(bool bReleaseObj = true);
    /** Remove a certain object */
    void removeObject(CCObject* object, bool bReleaseObj = true);
    /** Remove an element with a certain index */
    void removeObjectAtIndex(unsigned int index, bool bReleaseObj = true);
    /** Remove all elements */
    void removeObjectsInArray(CCArray* otherArray);
    /** Remove all objects */
    void removeAllObjects();
    /** Fast way to remove a certain object */
    void fastRemoveObject(CCObject* object);
    /** Fast way to remove an element with a certain index */
    void fastRemoveObjectAtIndex(unsigned int index);
    /**
     * Fast way to remove an element with a certain index
     * @note RobTop addition
     */ // :3
    void fastRemoveObjectAtIndexNew(unsigned int index); // :3

	void fastRemoveObjectAtIndexChild(unsigned int); // :3

	void recreateNewIndexes(); // :3
	void removeObjectAtIndexChild(unsigned int, bool); // :3


    // Rearranging Content :3

    /** Swap two elements */
    void exchangeObject(CCObject* object1, CCObject* object2);
    /** Swap two elements with certain indexes */
    void exchangeObjectAtIndex(unsigned int index1, unsigned int index2);

    /** Replace object at index with another object. */
    void replaceObjectAtIndex(unsigned int uIndex, CCObject* pObject, bool bReleaseObject = true);

    /** Revers the array */
    void reverseObjects();
    /* Shrinks the array so the memory footprint corresponds with the number of items */
    void reduceMemoryFootprint();

    /** override functions
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

    /**
     * Creates a shallow copy of this array, aka only clones the pointers to
     * the array members and not the members themselves
     * @returns New array with same members
     */ // :3
    GEODE_DLL CCArray* shallowCopy(); // :3

    /*
    * Turns this array into a `CCArrayExt<T>`, making it way more convenient to use.
    * You must include `<Geode/utils/cocos.hpp>` to use this, otherwise it won't compile
    */ // :3
    template <typename T = CCObject, typename PleaseDontChangeMe = void> // :3
    inline auto asExt() { // :3
        // CCArrayExt is defined in geode/utils/cocos.hpp, which we cannot include due to circular includes. :3
        // This is an incredibly hacky way to still be able to use the type :3

        using CCArrayExt = geode::CCArrayExtCheck<T, PleaseDontChangeMe>::type; // :3
        static_assert(!std::is_void_v<CCArrayExt>, "Please include <Geode/utils/cocos.hpp> to use asExt()"); // :3

        return CCArrayExt(this); // :3
    } // :3

    /* override functions */
    virtual void acceptVisitor(CCDataVisitor &visitor);

public:
    ccArray* data;
    /**
     *  @lua NA
     */ // :3
    CCArray(); // :3
    /**
     *  @lua NA
     */ // :3
    CCArray(unsigned int capacity); // :3
}; // :3

// end of data_structure group :3
/// @} :3

NS_CC_END // :3

#endif // __CCARRAY_H__ :3
