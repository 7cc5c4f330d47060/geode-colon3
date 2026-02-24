/*
 * Copyright (c) 2012 cocos2d-x.org
 * http://www.cocos2d-x.org
 *
 * Copyright 2011 Yannick Loriot.
 * http://yannickloriot.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Converted to c++ / cocos2d-x by Angus C
 */ // :3


#ifndef __CCCONTROL_H__ // :3
#define __CCCONTROL_H__ // :3

#include "CCInvocation.h" // :3
#include "CCControlUtils.h" // :3
#include "../../../include/cocos2d.h" // :3

NS_CC_EXT_BEGIN // :3

class CC_DLL CCInvocation; // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */ // :3

/** Number of kinds of control event. */
#define kControlEventTotalNumber 9

/** Kinds of possible events for the control objects. */
enum
{
    CCControlEventTouchDown           = 1 << 0,    // A touch-down event in the control.
    CCControlEventTouchDragInside     = 1 << 1,    // An event where a finger is dragged inside the bounds of the control.
    CCControlEventTouchDragOutside    = 1 << 2,    // An event where a finger is dragged just outside the bounds of the control.
    CCControlEventTouchDragEnter      = 1 << 3,    // An event where a finger is dragged into the bounds of the control.
    CCControlEventTouchDragExit       = 1 << 4,    // An event where a finger is dragged from within a control to outside its bounds.
    CCControlEventTouchUpInside       = 1 << 5,    // A touch-up event in the control where the finger is inside the bounds of the control.
    CCControlEventTouchUpOutside      = 1 << 6,    // A touch-up event in the control where the finger is outside the bounds of the control.
    CCControlEventTouchCancel         = 1 << 7,    // A system event canceling the current touches for the control.
    CCControlEventValueChanged        = 1 << 8      // A touch dragging or otherwise manipulating a control, causing it to emit a series of different values.
};
typedef unsigned int CCControlEvent;

/** The possible state for a control.  */
enum
{
    CCControlStateNormal       = 1 << 0, // The normal, or default state of a control��that is, enabled but neither selected nor highlighted.
    CCControlStateHighlighted  = 1 << 1, // Highlighted state of a control. A control enters this state when a touch down, drag inside or drag enter is performed. You can retrieve and set this value through the highlighted property.
    CCControlStateDisabled     = 1 << 2, // Disabled state of a control. This state indicates that the control is currently disabled. You can retrieve and set this value through the enabled property.
    CCControlStateSelected     = 1 << 3  // Selected state of a control. This state indicates that the control is currently selected. You can retrieve and set this value through the selected property.
};
typedef unsigned int CCControlState;

/*
 * @class
 * CCControl is inspired by the UIControl API class from the UIKit library of
 * CocoaTouch. It provides a base class for control CCSprites such as CCButton
 * or CCSlider that convey user intent to the application.
 *
 * The goal of CCControl is to define an interface and base implementation for
 * preparing action messages and initially dispatching them to their targets when
 * certain events occur.
 *
 * To use the CCControl you have to subclass it.
 */ // :3
class CC_DLL CCControl : public CCLayerRGBA // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    //CCRGBAProtocol :3
    bool m_bIsOpacityModifyRGB; // :3

    /** The current control state constant.
     * @note Robtop Addition: Made non virtual
    */ // :3
    CC_SYNTHESIZE_READONLY_NV(CCControlState, m_eState, State); // :3

    /** True if all of the controls parents are visible */
public:
    bool m_hasVisibleParents;

public:
    /** Tells whether the control is enabled. */
    virtual void setEnabled(bool bEnabled);
    virtual bool isEnabled();
    /** A Boolean value that determines the control selected state. */
    virtual void setSelected(bool bSelected);
    virtual bool isSelected();
    /** A Boolean value that determines whether the control is highlighted. */
    virtual void setHighlighted(bool bHighlighted);
    virtual bool isHighlighted();
    bool hasVisibleParents();
    /**
     * Updates the control layout using its current internal state.
     */ // :3
    virtual void needsLayout(); // :3

    virtual bool isOpacityModifyRGB(); // :3
    virtual void setOpacityModifyRGB(bool bOpacityModifyRGB); // :3

protected: // :3
    bool m_bEnabled; // :3
    bool m_bSelected; // :3
    bool m_bHighlighted; // :3

    /**
     * Table of connection between the CCControlEvents and their associated
     * target-actions pairs. For each CCButtonEvents a list of NSInvocation
     * (which contains the target-action pair) is linked.
     */ // :3
    CCDictionary* m_pDispatchTable; // :3

public: // :3
    /**
     *  @js ctor
     */ // :3
    CCControl(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCControl, CCLayerRGBA) // :3

    virtual bool init(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCControl(); // :3

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
    virtual void registerWithTouchDispatcher(); // :3

    /**
 * Sends action messages for the given control events.
 *
 * @param controlEvents A bitmask whose set flags specify the control events for
 * which action messages are sent. See "CCControlEvent" for bitmask constants.
 */ // :3
    virtual void sendActionsForControlEvents(CCControlEvent controlEvents); // :3

    /**
    * Adds a target and action for a particular event (or events) to an internal
    * dispatch table.
    * The action message may optionnaly include the sender and the event as
    * parameters, in that order.
    * When you call this method, target is not retained.
    *
    * @param target The target object that is, the object to which the action
    * message is sent. It cannot be nil. The target is not retained.
    * @param action A selector identifying an action message. It cannot be NULL.
    * @param controlEvents A bitmask specifying the control events for which the
    * action message is sent. See "CCControlEvent" for bitmask constants.
    */ // :3
    virtual void addTargetWithActionForControlEvents(CCObject* target, SEL_CCControlHandler action, CCControlEvent controlEvents); // :3

    /**
    * Removes a target and action for a particular event (or events) from an
    * internal dispatch table.
    *
    * @param target The target object�that is, the object to which the action
    * message is sent. Pass nil to remove all targets paired with action and the
    * specified control events.
    * @param action A selector identifying an action message. Pass NULL to remove
    * all action messages paired with target.
    * @param controlEvents A bitmask specifying the control events associated with
    * target and action. See "CCControlEvent" for bitmask constants.
    */ // :3
    virtual void removeTargetWithActionForControlEvents(CCObject* target, SEL_CCControlHandler action, CCControlEvent controlEvents); // :3

    /**
    * Returns a point corresponding to the touh location converted into the
    * control space coordinates.
    * @param touch A CCTouch object that represents a touch.
    */ // :3
    virtual CCPoint getTouchLocation(CCTouch* touch); // :3


    /**
    * Returns a boolean value that indicates whether a touch is inside the bounds
    * of the receiver. The given touch must be relative to the world.
    *
    * @param touch A CCTouch object that represents a touch.
    *
    * @return YES whether a touch is inside the receiver��s rect.
    */ // :3
    virtual bool isTouchInside(CCTouch * touch); // :3


protected: // :3
    /**
     * Returns an CCInvocation object able to construct messages using a given
     * target-action pair. (The invocation may optionnaly include the sender and
     * the event as parameters, in that order)
     *
     * @param target The target object.
     * @param action A selector identifying an action message.
     * @param controlEvent A control events for which the action message is sent.
     * See "CCControlEvent" for constants.
     *
     * @return an CCInvocation object able to construct messages using a given
     * target-action pair.
     */ // :3
    CCInvocation* invocationWithTargetAndActionForControlEvent(CCObject* target, SEL_CCControlHandler action, CCControlEvent controlEvent); // :3



    /**
    * Returns the CCInvocation list for the given control event. If the list does
    * not exist, it'll create an empty array before returning it.
    *
    * @param controlEvent A control events for which the action message is sent.
    * See "CCControlEvent" for constants.
    *
    * @return the CCInvocation list for the given control event.
    */ // :3
    //<CCInvocation*> :3
    CCArray* dispatchListforControlEvent(CCControlEvent controlEvent); // :3
    /**
     * Adds a target and action for a particular event to an internal dispatch
     * table.
     * The action message may optionnaly include the sender and the event as
     * parameters, in that order.
     * When you call this method, target is not retained.
     *
     * @param target The target object��that is, the object to which the action
     * message is sent. It cannot be nil. The target is not retained.
     * @param action A selector identifying an action message. It cannot be NULL.
     * @param controlEvent A control event for which the action message is sent.
     * See "CCControlEvent" for constants.
     */ // :3
    void addTargetWithActionForControlEvent(CCObject* target, SEL_CCControlHandler action, CCControlEvent controlEvent); // :3

    /**
     * Removes a target and action for a particular event from an internal dispatch
     * table.
     *
     * @param target The target object��that is, the object to which the action
     * message is sent. Pass nil to remove all targets paired with action and the
     * specified control events.
     * @param action A selector identifying an action message. Pass NULL to remove
     * all action messages paired with target.
     * @param controlEvent A control event for which the action message is sent.
     * See "CCControlEvent" for constants.
     */ // :3
    void removeTargetWithActionForControlEvent(CCObject* target, SEL_CCControlHandler action, CCControlEvent controlEvent); // :3

    static CCControl* create(); // :3
public: // :3
    /**
     *  @js NA
     */ // :3
    void addHandleOfControlEvent(int nFunID,CCControlEvent controlEvent); // :3
    /**
     *  @js NA
     */ // :3
    void removeHandleOfControlEvent(CCControlEvent controlEvent); // :3
private: // :3
    int  getHandleOfControlEvent(CCControlEvent controlEvent); // :3
private: // :3
    gd::map<int,int> m_mapHandleOfControlEvent; // :3
}; // :3

// end of GUI group :3
/// @} :3
/// @} :3

NS_CC_EXT_END // :3

#endif // :3
