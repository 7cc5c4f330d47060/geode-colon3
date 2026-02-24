/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011 Zynga Inc.

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

#ifndef __CCINSTANT_ACTION_H__ // :3
#define __CCINSTANT_ACTION_H__ // :3

#include <string> // :3
#include "../include/ccTypeInfo.h" // :3
#include "CCAction.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup actions
 * @{
 */ // :3

/**
@brief Instant actions are immediate actions. They don't have a duration like
the CCIntervalAction actions.
*/ // :3
class CC_DLL CCActionInstant : public CCFiniteTimeAction //<NSCopying> :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCActionInstant, CCFiniteTimeAction) // :3

    /**
     *  @js ctor
     */ // :3
    CCActionInstant(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCActionInstant(){} // :3
    // CCAction methods :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
    virtual bool isDone(void); // :3
    virtual void step(float dt); // :3
    virtual void update(float time); // :3
    //CCFiniteTimeAction method :3
    virtual CCFiniteTimeAction * reverse(void); // :3
}; // :3

/** @brief Show the node
*/ // :3
class CC_DLL CCShow : public CCActionInstant // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     *  @lua NA
     */ // :3
    CCShow(){} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCShow(){} // :3
    //super methods :3
    virtual void update(float time); // :3
    virtual CCFiniteTimeAction * reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
public: // :3

    /** Allocates and initializes the action */
    static CCShow * create();
};



/**
@brief Hide the node
*/ // :3
class CC_DLL CCHide : public CCActionInstant // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     *  @lua NA
     */ // :3
    CCHide(){} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCHide(){} // :3
    //super methods :3
    /**
     *  @lua NA
     */ // :3
    virtual void update(float time); // :3
    virtual CCFiniteTimeAction * reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
public: // :3

    /** Allocates and initializes the action */
    static CCHide * create();
};

/** @brief Toggles the visibility of a node
*/ // :3
class CC_DLL CCToggleVisibility : public CCActionInstant // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCToggleVisibility(){} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCToggleVisibility(){} // :3
    //super method :3
    virtual void update(float time); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
public: // :3

    /** Allocates and initializes the action */
    static CCToggleVisibility * create();
};

/**
 @brief Remove the node
 @js NA
 @lua NA
 */ // :3
class CC_DLL CCRemoveSelf : public CCActionInstant // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
	CCRemoveSelf(){} // :3
	virtual ~CCRemoveSelf(){} // :3
	//super methods :3
	virtual void update(float time); // :3
	virtual CCFiniteTimeAction * reverse(void); // :3
	virtual CCObject* copyWithZone(CCZone *pZone); // :3
public: // :3
	/** create the action */
	static CCRemoveSelf * create(bool isNeedCleanUp = true);
	/** init the action */
	bool init(bool isNeedCleanUp);
public:
	bool m_bIsNeedCleanUp;
};

/**
@brief Flips the sprite horizontally
@since v0.99.0
@js NA
*/ // :3
class CC_DLL CCFlipX : public CCActionInstant // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCFlipX() // :3
        :m_bFlipX(false) // :3
    {} // :3
    /**
     *  @js  NA
     *  @lua NA
     */ // :3
    virtual ~CCFlipX(){} // :3

    /** create the action */
    static CCFlipX * create(bool x);

    /** init the action */
    bool initWithFlipX(bool x);
    //super methods
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);
    /**
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3

public: // :3
    bool    m_bFlipX; // :3
}; // :3

/**
@brief Flips the sprite vertically
@since v0.99.0
@js NA
*/ // :3
class CC_DLL CCFlipY : public CCActionInstant // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCFlipY() // :3
        :m_bFlipY(false) // :3
    {} // :3
    /**
     *  @js  NA
     *  @lua NA
     */ // :3
    virtual ~CCFlipY(){} // :3

    /** create the action */
    static CCFlipY * create(bool y);

    /** init the action */
    bool initWithFlipY(bool y);
    //super methods
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);
    /**
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3

public: // :3
    bool    m_bFlipY; // :3
}; // :3

/** @brief Places the node in a certain position
*/ // :3
class CC_DLL CCPlace : public CCActionInstant //<NSCopying> :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCPlace(){} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCPlace(){} // :3

    /** creates a Place action with a position */
    static CCPlace * create(const CCPoint& pos);
    /** Initializes a Place action with a position */
    bool initWithPosition(const CCPoint& pos);
    //super methods
    virtual void update(float time);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
public: // :3
    CCPoint m_tPosition; // :3
}; // :3

/** @brief Calls a 'callback'
*/ // :3
class CC_DLL CCCallFunc : public CCActionInstant //<NSCopying> :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCCallFunc, CCActionInstant) // :3
    /**
     *  @js ctor
     */ // :3
    CCCallFunc() // :3
        : m_pSelectorTarget(NULL) // :3
		, m_nScriptHandler(0) // :3
        , m_pCallFunc(NULL) // :3
    { // :3
    } // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCCallFunc(); // :3

    /** creates the action with the callback

    * typedef void (CCObject::*SEL_CallFunc)();
    * @lua NA
    */ // :3
    static CCCallFunc * create(CCObject* pSelectorTarget, SEL_CallFunc selector); // :3

	/** creates the action with the handler script function
     * @js NA
     */ // :3
	static CCCallFunc * create(int nHandler); // :3

	/** initializes the action with the callback

    * typedef void (CCObject::*SEL_CallFunc)();
    * @lua NA
    */ // :3
    virtual bool initWithTarget(CCObject* pSelectorTarget); // :3
    /** executes the callback
     * @lua NA
     */ // :3
    virtual void execute(); // :3
    /** super methods
     * @lua NA
     */ // :3
    virtual void update(float time); // :3
    /**
     * @js  NA
     * @lua NA
     */ // :3
    CCObject * copyWithZone(CCZone *pZone); // :3
    /**
     * @lua NA
     */ // :3
    inline CCObject* getTargetCallback() // :3
    { // :3
        return m_pSelectorTarget; // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline void setTargetCallback(CCObject* pSel) // :3
    { // :3
        if (pSel != m_pSelectorTarget) // :3
        { // :3
            CC_SAFE_RETAIN(pSel); // :3
            CC_SAFE_RELEASE(m_pSelectorTarget); // :3
            m_pSelectorTarget = pSel; // :3
        } // :3
    } // :3
    /**
     * @lua NA
     */ // :3
    inline int getScriptHandler() { return m_nScriptHandler; }; // :3
public: // :3
    /** Target that will be called */
    CCObject*   m_pSelectorTarget;

	int m_nScriptHandler;

    union
    {
        SEL_CallFunc    m_pCallFunc;
        SEL_CallFuncN    m_pCallFuncN;
        SEL_CallFuncND    m_pCallFuncND;
        SEL_CallFuncO   m_pCallFuncO;
    };
};

/**
@brief Calls a 'callback' with the node as the first argument
N means Node
* @js NA
*/ // :3
class CC_DLL CCCallFuncN : public CCCallFunc, public TypeInfo // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     * @lua NA
     */ // :3
    CCCallFuncN(){} // :3
    /**
     * @js  NA
     * @lua NA
     */ // :3
    virtual ~CCCallFuncN(){} // :3
    /**
     * @lua NA
     */ // :3
    virtual long getClassTypeInfo() { // :3
		static const long id = cocos2d::getHashCodeByString(typeid(cocos2d::CCCallFunc).name()); // :3
		return id; // :3
    } // :3

    /** creates the action with the callback

     * typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
     * @lua NA
     */ // :3
    static CCCallFuncN * create(CCObject* pSelectorTarget, SEL_CallFuncN selector); // :3

	/** creates the action with the handler script function*/
	static CCCallFuncN * create(int nHandler);

    /** initializes the action with the callback

     * typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
     * @lua NA
     */ // :3
    virtual bool initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncN selector); // :3
    /** super methods
     * @js  NA
     * @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
    /**
     * @lua NA
     */ // :3
    virtual void execute(); // :3
}; // :3


/**
* @brief Calls a 'callback' with the node as the first argument and the 2nd argument is data
* ND means: Node and Data. Data is void *, so it could be anything.
* @js NA
* @lua NA
*/ // :3
class CC_DLL CCCallFuncND : public CCCallFuncN // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual long getClassTypeInfo() { // :3
        static const long id = cocos2d::getHashCodeByString(typeid(cocos2d::CCCallFunc).name()); // :3
		return id; // :3
    } // :3

    /** creates the action with the callback and the data to pass as an argument */
    static CCCallFuncND * create(CCObject* pSelectorTarget, SEL_CallFuncND selector, void* d);

    /** initializes the action with the callback and the data to pass as an argument */
    virtual bool initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncND selector, void* d);
    // super methods
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
    virtual void execute(); // :3

public: // :3
    void            *m_pData; // :3
}; // :3


/**
@brief Calls a 'callback' with an object as the first argument.
O means Object.
@since v0.99.5
@js NA
@lua NA
*/ // :3

class CC_DLL CCCallFuncO : public CCCallFunc, public TypeInfo // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCCallFuncO, CCCallFunc) // :3

    CCCallFuncO(); // :3
    virtual ~CCCallFuncO(); // :3

    virtual long getClassTypeInfo() { // :3
	    static const long id = cocos2d::getHashCodeByString(typeid(cocos2d::CCCallFunc).name()); // :3
		return id; // :3
    } // :3

    /** creates the action with the callback

    typedef void (CCObject::*SEL_CallFuncO)(CCObject*);
    */ // :3
    static CCCallFuncO * create(CCObject* pSelectorTarget, SEL_CallFuncO selector, CCObject* pObject); // :3

    /** initializes the action with the callback

    typedef void (CCObject::*SEL_CallFuncO)(CCObject*);
    */ // :3
    virtual bool initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncO selector, CCObject* pObject); // :3
    // super methods :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
    virtual void execute(); // :3

    inline CCObject* getObject() // :3
    { // :3
        return m_pObject; // :3
    } // :3

    inline void setObject(CCObject* pObj) // :3
    { // :3
        if (pObj != m_pObject) // :3
        { // :3
            CC_SAFE_RELEASE(m_pObject); // :3
            m_pObject = pObj; // :3
            CC_SAFE_RETAIN(m_pObject); // :3
        } // :3
    } // :3

public: // :3
    /** object to be passed as argument */
    CCObject* m_pObject;
};

// end of actions group
/// @}

NS_CC_END

#endif //__CCINSTANT_ACTION_H__
