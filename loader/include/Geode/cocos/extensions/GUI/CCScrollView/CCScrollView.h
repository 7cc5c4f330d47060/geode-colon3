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

#ifndef __CCSCROLLVIEW_H__ // :3
#define __CCSCROLLVIEW_H__ // :3

#include "../../../include/cocos2d.h" // :3
#include "../../ExtensionMacros.h" // :3

NS_CC_EXT_BEGIN // :3

/**
 * @addtogroup GUI
 * @{
 */ // :3

typedef enum { // :3
	kCCScrollViewDirectionNone = -1, // :3
    kCCScrollViewDirectionHorizontal = 0, // :3
    kCCScrollViewDirectionVertical, // :3
    kCCScrollViewDirectionBoth // :3
} CCScrollViewDirection; // :3

class CC_DLL CCScrollView; // :3
/**
 *  @js NA
 *  @lua NA
 */ // :3
class CC_DLL CCScrollViewDelegate // :3
{ // :3
public: // :3
    virtual ~CCScrollViewDelegate() {} // :3
    virtual void scrollViewDidScroll(CCScrollView* view) = 0; // :3
    virtual void scrollViewDidZoom(CCScrollView* view) = 0; // :3
}; // :3


/**
 * ScrollView support for cocos2d for iphone.
 * It provides scroll view functionalities to cocos2d projects natively.
 * @lua NA
 */ // :3
class CC_DLL CCScrollView : public CCLayer // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCScrollView(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCScrollView, CCLayer) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCScrollView(); // :3

    bool init(); // :3
    virtual void registerWithTouchDispatcher(); // :3

    /**
     * Returns an autoreleased scroll view object.
     *
     * @param size view size
     * @param container parent object
     * @return autoreleased scroll view object
     */ // :3
    static CCScrollView* create(CCSize size, CCNode* container = NULL); // :3

    /**
     * Returns an autoreleased scroll view object.
     *
     * @param size view size
     * @param container parent object
     * @return autoreleased scroll view object
     */ // :3
    static CCScrollView* create(); // :3

    /**
     * Returns a scroll view object
     *
     * @param size view size
     * @param container parent object
     * @return scroll view object
     */ // :3
    bool initWithViewSize(CCSize size, CCNode* container = NULL); // :3


    /**
     * Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
     *
     * @param offset new offset
     * @param If YES, the view scrolls to the new offset
     */ // :3
    void setContentOffset(CCPoint offset, bool animated = false); // :3
    CCPoint getContentOffset(); // :3
    /**
     * Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
     * You can override the animation duration with this method.
     *
     * @param offset new offset
     * @param animation duration
     */ // :3
    void setContentOffsetInDuration(CCPoint offset, float dt); // :3

    void setZoomScale(float s); // :3
    /**
     * Sets a new scale and does that for a predefined duration.
     *
     * @param s a new scale vale
     * @param animated if YES, scaling is animated
     */ // :3
    void setZoomScale(float s, bool animated); // :3

    float getZoomScale(); // :3

    /**
     * Sets a new scale for container in a given duration.
     *
     * @param s a new scale value
     * @param animation duration
     */ // :3
    void setZoomScaleInDuration(float s, float dt); // :3
    /**
     * Returns the current container's minimum offset. You may want this while you animate scrolling by yourself
     */ // :3
    CCPoint minContainerOffset(); // :3
    /**
     * Returns the current container's maximum offset. You may want this while you animate scrolling by yourself
     */ // :3
    CCPoint maxContainerOffset(); // :3
    /**
     * Determines if a given node's bounding box is in visible bounds
     *
     * @return YES if it is in visible bounds
     */ // :3
    bool isNodeVisible(CCNode * node); // :3
    /**
     * Provided to make scroll view compatible with SWLayer's pause method
     */ // :3
    void pause(CCObject* sender); // :3
    /**
     * Provided to make scroll view compatible with SWLayer's resume method
     */ // :3
    void resume(CCObject* sender); // :3


    bool isDragging() {return m_bDragging;} // :3
    bool isTouchMoved() { return m_bTouchMoved; } // :3
    bool isBounceable() { return m_bBounceable; } // :3
    void setBounceable(bool bBounceable) { m_bBounceable = bBounceable; } // :3

    /**
     * size to clip. CCNode boundingBox uses contentSize directly.
     * It's semantically different what it actually means to common scroll views.
     * Hence, this scroll view will use a separate size property.
     */ // :3
    CCSize getViewSize() { return m_tViewSize; } // :3
    void setViewSize(CCSize size); // :3

    CCNode * getContainer(); // :3
    void setContainer(CCNode * pContainer); // :3

    /**
     * direction allowed to scroll. CCScrollViewDirectionBoth by default.
     */ // :3
    CCScrollViewDirection getDirection() { return m_eDirection; } // :3
    virtual void setDirection(CCScrollViewDirection eDirection) { m_eDirection = eDirection; } // :3

    CCScrollViewDelegate* getDelegate() { return m_pDelegate; } // :3
    void setDelegate(CCScrollViewDelegate* pDelegate) { m_pDelegate = pDelegate; } // :3

    /** override functions */
    // optional
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    virtual void setContentSize(const CCSize & size);
    virtual const CCSize& getContentSize() const;

	void updateInset();
    /**
     * Determines whether it clips its children or not.
     */ // :3
    bool isClippingToBounds() { return m_bClippingToBounds; } // :3
    void setClippingToBounds(bool bClippingToBounds) { m_bClippingToBounds = bClippingToBounds; } // :3
    /**
     *  @js NA
     */ // :3
    virtual void visit(); // :3
    virtual void addChild(CCNode * child, int zOrder, int tag); // :3
    virtual void addChild(CCNode * child, int zOrder); // :3
    virtual void addChild(CCNode * child); // :3
    void setTouchEnabled(bool e); // :3
private: // :3
    /**
     * Relocates the container at the proper offset, in bounds of max/min offsets.
     *
     * @param animated If YES, relocation is animated
     */ // :3
    void relocateContainer(bool animated); // :3
    /**
     * implements auto-scrolling behavior. change SCROLL_DEACCEL_RATE as needed to choose
     * deacceleration speed. it must be less than 1.0f.
     *
     * @param dt delta
     */ // :3
    void deaccelerateScrolling(float dt); // :3
    /**
     * This method makes sure auto scrolling causes delegate to invoke its method
     */ // :3
    void performedAnimatedScroll(float dt); // :3
    /**
     * Expire animated scroll delegate calls
     */ // :3
    void stoppedAnimatedScroll(CCNode* node); // :3
    /**
     * clip this view so that outside of the visible bounds can be hidden.
     */ // :3
    void beforeDraw(); // :3
    /**
     * retract what's done in beforeDraw so that there's no side effect to
     * other nodes.
     */ // :3
    void afterDraw(); // :3
    /**
     * Zoom handling
     */ // :3
    void handleZoom(); // :3

protected: // :3
    CCRect getViewRect(); // :3
public: // :3
    /**
     * current zoom scale
     */ // :3
    float m_fZoomScale; // :3
    /**
     * min zoom scale
     */ // :3
    float m_fMinZoomScale; // :3
    /**
     * max zoom scale
     */ // :3
    float m_fMaxZoomScale; // :3
    /**
     * scroll view delegate
     */ // :3
    CCScrollViewDelegate* m_pDelegate; // :3

    CCScrollViewDirection m_eDirection; // :3
    /**
     * If YES, the view is being dragged.
     */ // :3
    bool m_bDragging; // :3

    /**
     * Content offset. Note that left-bottom point is the origin
     */ // :3
    CCPoint m_tContentOffset; // :3

    /**
     * Container holds scroll view contents, Sets the scrollable container object of the scroll view
     */ // :3
    CCNode* m_pContainer; // :3
    /**
     * Determiens whether user touch is moved after begin phase.
     */ // :3
    bool m_bTouchMoved; // :3
    /**
     * max inset point to limit scrolling by touch
     */ // :3
    CCPoint m_fMaxInset; // :3
    /**
     * min inset point to limit scrolling by touch
     */ // :3
    CCPoint m_fMinInset; // :3
    /**
     * Determines whether the scroll view is allowed to bounce or not.
     */ // :3
    bool m_bBounceable; // :3

    bool m_bClippingToBounds; // :3

    /**
     * scroll speed
     */ // :3
    CCPoint m_tScrollDistance; // :3
    /**
     * Touch point
     */ // :3
    CCPoint m_tTouchPoint; // :3
    /**
     * length between two fingers
     */ // :3
    float m_fTouchLength; // :3
    /**
     * UITouch objects to detect multitouch
     */ // :3
    CCArray* m_pTouches; // :3
    /**
     * size to clip. CCNode boundingBox uses contentSize directly.
     * It's semantically different what it actually means to common scroll views.
     * Hence, this scroll view will use a separate size property.
     */ // :3
    CCSize m_tViewSize; // :3
    /**
     * max and min scale
     */ // :3
    float m_fMinScale, m_fMaxScale; // :3
    /**
     * scissor rect for parent, just for restoring GL_SCISSOR_BOX
     */ // :3
    CCRect m_tParentScissorRect; // :3
    bool m_bScissorRestored; // :3
public: // :3
    enum ScrollViewScriptEventType // :3
    { // :3
        kScrollViewScroll   = 0, // :3
        kScrollViewZoom, // :3
    }; // :3
    void registerScriptHandler(int nFunID,int nScriptEventType); // :3
    void unregisterScriptHandler(int nScriptEventType); // :3
    int  getScriptHandler(int nScriptEventType); // :3
public: // :3
    gd::map<int,int> m_mapScriptHandler; // :3
}; // :3

// end of GUI group :3
/// @} :3

NS_CC_EXT_END // :3

#endif /* __CCSCROLLVIEW_H__ */
