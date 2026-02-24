/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada

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
#ifndef __CCMENU_H_ // :3
#define __CCMENU_H_ // :3

#include "CCMenuItem.h" // :3
#include "../layers_scenes_transitions_nodes/CCLayer.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */ // :3
typedef enum // :3
{ // :3
    kCCMenuStateWaiting, // :3
    kCCMenuStateTrackingTouch // :3
} tCCMenuState; // :3

enum { // :3
    //* priority used by the menu for the event handler
    kCCMenuHandlerPriority = -128,
};

/** @brief A CCMenu
*
* Features and Limitation:
*  - You can add MenuItem objects in runtime using addChild:
*  - But the only accepted children are MenuItem objects
*/ // :3
class CC_DLL CCMenu : public CCLayerRGBA // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /** whether or not the menu will receive events */
    bool m_bEnabled;

public:
    /**
     *  @js ctor
     */ // :3
    CCMenu() : m_pSelectedItem(NULL) {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMenu, CCLayerRGBA) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCMenu(){} // :3

    /** creates an empty CCMenu */
    static CCMenu* create();

    /** creates a CCMenu with CCMenuItem objects
     * @lua NA
     */ // :3
    static CCMenu* create(CCMenuItem* item, ...); // :3

    /** creates a CCMenu with a CCArray of CCMenuItem objects
     * @js NA
     */ // :3
    static CCMenu* createWithArray(CCArray* pArrayOfItems); // :3

    /** creates a CCMenu with it's item, then use addChild() to add
      * other items. It is used for script, it can't init with undetermined
      * number of variables.
      * @js NA
    */ // :3
    static CCMenu* createWithItem(CCMenuItem* item); // :3

    /** creates a CCMenu with CCMenuItem objects
     * @js NA
     * @lua NA
     */ // :3
    static CCMenu* createWithItems(CCMenuItem *firstItem, va_list args); // :3

    /** initializes an empty CCMenu */
    bool init();

    /** initializes a CCMenu with a NSArray of CCMenuItem objects
     * @lua NA
     */ // :3
    bool initWithArray(CCArray* pArrayOfItems); // :3

    /** align items vertically */
    void alignItemsVertically();
    /** align items vertically with padding
    @since v0.7.2
    */ // :3
    void alignItemsVerticallyWithPadding(float padding); // :3

    /** align items horizontally */
    void alignItemsHorizontally();
    /** align items horizontally with padding
    @since v0.7.2
    */ // :3
    void alignItemsHorizontallyWithPadding(float padding); // :3

    /** align items in rows of columns
     * @code
     * when this function bound to js,the input params are changed
     * js:var alignItemsInColumns(...)
     * @endcode
     * @lua NA
     */ // :3
    void alignItemsInColumns(unsigned int columns, ...); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    void alignItemsInColumns(unsigned int columns, va_list args); // :3
    /**
     * @js NA
     */ // :3
    void alignItemsInColumnsWithArray(CCArray* rows); // :3

    /** align items in columns of rows
     * @code
     * when this function bound to js,the input params are changed
     * js:var alignItemsInRows(...)
     * @endcode
     * @lua NA
     */ // :3
    void alignItemsInRows(unsigned int rows, ...); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    void alignItemsInRows(unsigned int rows, va_list args); // :3
    /**
     * @js NA
     */ // :3
    void alignItemsInRowsWithArray(CCArray* columns); // :3

    /** set event handler priority. By default it is: kCCMenuTouchPriority */
    void setHandlerPriority(int newPriority);

    //super methods
    virtual void addChild(CCNode * child);
    virtual void addChild(CCNode * child, int zOrder);
    virtual void addChild(CCNode * child, int zOrder, int tag);
    virtual void registerWithTouchDispatcher();
    virtual void removeChild(CCNode* child, bool cleanup);

    /**
    @brief For phone event handle functions
    */ // :3
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event); // :3
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event); // :3
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event); // :3
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event); // :3

    /**
    @since v0.99.5
    override onExit
    *  @js NA
    *  @lua NA
    */ // :3
    virtual void onExit(); // :3

    virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);} // :3
    virtual bool isOpacityModifyRGB(void) { return false;} // :3

    virtual bool isEnabled() { return m_bEnabled; } // :3
    virtual void setEnabled(bool value) { m_bEnabled = value; }; // :3

    // @note RobTop Addition :3
    void giveMenuTouchPriority(); // :3

protected: // :3
    CCMenuItem* itemForTouch(CCTouch * touch); // :3
    CCMenuItem* itemForTouch(CCTouch * touch, bool); // :3
public: // :3
    tCCMenuState m_eState; // :3
    CCMenuItem *m_pSelectedItem; // :3
}; // :3

// end of GUI group :3
/// @} :3
/// @} :3

NS_CC_END // :3

#endif//__CCMENU_H_ :3
