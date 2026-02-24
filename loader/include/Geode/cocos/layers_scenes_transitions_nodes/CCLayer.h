/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

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

#ifndef __CCLAYER_H__ // :3
#define __CCLAYER_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "../include/CCProtocols.h" // :3
#include "../touch_dispatcher/CCTouchDelegateProtocol.h" // :3
#include "../platform/CCAccelerometerDelegate.h" // :3
#include "../keypad_dispatcher/CCKeypadDelegate.h" // :3

#include "../robtop/keyboard_dispatcher/CCKeyboardDelegate.h" // :3
#include "../robtop/mouse_dispatcher/CCMouseDelegate.h" // :3

#include "../cocoa/CCArray.h" // :3
#ifdef EMSCRIPTEN // :3
#include "../base_nodes/CCGLBufferedNode.h" // :3
#endif // EMSCRIPTEN :3

NS_CC_BEGIN // :3

typedef enum { // :3
	kCCTouchesAllAtOnce, // :3
	kCCTouchesOneByOne, // :3
} ccTouchesMode; // :3

/**
 * @addtogroup layer
 * @{
 */ // :3

class CCTouchScriptHandlerEntry; // :3

// :3
// CCLayer :3
// :3
/** @brief CCLayer is a subclass of CCNode that implements the TouchEventsDelegate protocol.

All features from CCNode are valid, plus the following new features:
- It can receive iPhone Touches
- It can receive Accelerometer input
 * @note Robtop Addition: added CCKeyboardDelegate and CCMouseDelegate
*/ // :3
class CC_DLL CCLayer : public CCNode, public CCTouchDelegate, public CCAccelerometerDelegate, public CCKeypadDelegate, public CCKeyboardDelegate, public CCMouseDelegate // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCLayer(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCLayer, CCNode) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCLayer(); // :3
    virtual bool init(); // :3

    /** create one layer */
    static CCLayer *create(void);
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
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnterTransitionDidFinish(); // :3

    // default implements are used to call script callback if exist :3
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); // :3
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); // :3
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); // :3
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent); // :3

    // default implements are used to call script callback if exist :3
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent); // :3
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent); // :3
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent); // :3
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual void didAccelerate(CCAcceleration* pAccelerationValue); // :3
    void registerScriptAccelerateHandler(int nHandler); // :3
    void unregisterScriptAccelerateHandler(void); // :3

    /** If isTouchEnabled, this method is called onEnter. Override it to change the
    way CCLayer receives touch events.
    ( Default: CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this,0); )
    Example:
    void CCLayer::registerWithTouchDispatcher()
    {
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,INT_MIN+1,true);
    }
    @since v0.8.0
    */ // :3
    virtual void registerWithTouchDispatcher(void); // :3

    /** Register script touch events handler */
    virtual void registerScriptTouchHandler(int nHandler, bool bIsMultiTouches = false, int nPriority = INT_MIN, bool bSwallowsTouches = false);
    /** Unregister script touch events handler */
    virtual void unregisterScriptTouchHandler(void);

    /** whether or not it will receive Touch events.
    You can enable / disable touch events with this property.
    Only the touches of this node will be affected. This "method" is not propagated to it's children.
    @since v0.8.1
    */ // :3
    virtual bool isTouchEnabled(); // :3
    virtual void setTouchEnabled(bool value); // :3

    virtual void setTouchMode(ccTouchesMode mode); // :3
    virtual int getTouchMode(); // :3

    /** priority of the touch events. Default is 0 */
    virtual void setTouchPriority(int priority);
    virtual int getTouchPriority();

    /** whether or not it will receive Accelerometer events
    You can enable / disable accelerometer events with this property.
    @since v0.8.1
    */ // :3
    virtual bool isAccelerometerEnabled(); // :3
    virtual void setAccelerometerEnabled(bool value); // :3
    virtual void setAccelerometerInterval(double interval); // :3

    /** whether or not it will receive keypad events
    You can enable / disable accelerometer events with this property.
    it's new in cocos2d-x
    */ // :3
    virtual bool isKeypadEnabled(); // :3
    virtual void setKeypadEnabled(bool value); // :3

    // @note RobTop Addition :3
    virtual bool isKeyboardEnabled(); // :3
    // @note RobTop Addition :3
    virtual void setKeyboardEnabled(bool value); // :3

    // @note RobTop Addition :3
    virtual bool isMouseEnabled(); // :3
    // @note RobTop Addition :3
    virtual void setMouseEnabled(bool value); // :3

    /** Register keypad events handler */
    void registerScriptKeypadHandler(int nHandler);
    /** Unregister keypad events handler */
    void unregisterScriptKeypadHandler(void);

    virtual void keyBackClicked(void);
    virtual void keyMenuClicked(void);

    // @note RobTop Addition
    void keyDown(enumKeyCodes, double);

    // 2.2 additions
    virtual void setPreviousPriority(int);
    virtual int getPreviousPriority();

    inline CCTouchScriptHandlerEntry* getScriptTouchHandlerEntry() { return m_pScriptTouchHandlerEntry; };
    inline CCScriptHandlerEntry* getScriptKeypadHandlerEntry() { return m_pScriptKeypadHandlerEntry; };
    inline CCScriptHandlerEntry* getScriptAccelerateHandlerEntry() { return m_pScriptAccelerateHandlerEntry; };
public:
    bool m_bTouchEnabled;
    bool m_bAccelerometerEnabled;
    bool m_bKeypadEnabled;
    // @note RobTop Addition
    bool m_bKeyboardEnabled;
    // @note RobTop Addition
    bool m_bMouseEnabled;

public:
    // Script touch events handler
    CCTouchScriptHandlerEntry* m_pScriptTouchHandlerEntry;
    CCScriptHandlerEntry* m_pScriptKeypadHandlerEntry;
    CCScriptHandlerEntry* m_pScriptAccelerateHandlerEntry;

    int m_nTouchPriority;
    ccTouchesMode m_eTouchMode;

    // 2.2 additions
    int m_uPreviousPriority; // no idea

private:
    int  excuteScriptTouchHandler(int nEventType, CCTouch *pTouch);
    int  excuteScriptTouchHandler(int nEventType, CCSet *pTouches);
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#pragma mark -
#pragma mark CCLayerRGBA
#endif

/** CCLayerRGBA is a subclass of CCLayer that implements the CCRGBAProtocol protocol using a solid color as the background.

 All features from CCLayer are valid, plus the following new features that propagate into children that conform to the CCRGBAProtocol:
 - opacity
 - RGB colors
 @since 2.1
 */ // :3
class CC_DLL CCLayerRGBA : public CCLayer, public CCRGBAProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    CREATE_FUNC(CCLayerRGBA); // :3
    /**
     *  @js ctor
     */ // :3
    CCLayerRGBA(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCLayerRGBA, CCLayer) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCLayerRGBA(); // :3

    virtual bool init(); // :3

    virtual GLubyte getOpacity(); // :3
    virtual GLubyte getDisplayedOpacity(); // :3
    virtual void setOpacity(GLubyte opacity); // :3
    virtual void updateDisplayedOpacity(GLubyte parentOpacity); // :3
    virtual bool isCascadeOpacityEnabled(); // :3
    virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled); // :3

    virtual const ccColor3B& getColor(); // :3
    virtual const ccColor3B& getDisplayedColor(); // :3
    virtual void setColor(const ccColor3B& color); // :3
    virtual void updateDisplayedColor(const ccColor3B& parentColor); // :3
    virtual bool isCascadeColorEnabled(); // :3
    virtual void setCascadeColorEnabled(bool cascadeColorEnabled); // :3

    virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);} // :3
    virtual bool isOpacityModifyRGB() { return false; } // :3
protected: // :3
	GLubyte		_displayedOpacity, _realOpacity; // :3
	ccColor3B	_displayedColor, _realColor; // :3
	bool		_cascadeOpacityEnabled, _cascadeColorEnabled; // :3
}; // :3

// :3
// CCLayerColor :3
// :3
/** @brief CCLayerColor is a subclass of CCLayer that implements the CCRGBAProtocol protocol.

All features from CCLayer are valid, plus the following new features:
- opacity
- RGB colors
*/ // :3
class CC_DLL CCLayerColor : public CCLayerRGBA, public CCBlendProtocol // :3
#ifdef EMSCRIPTEN // :3
, public CCGLBufferedNode // :3
#endif // EMSCRIPTEN :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
protected: // :3


    ccVertex2F m_pSquareVertices[4]; // :3
    ccColor4F  m_pSquareColors[4]; // :3

public: // :3
    /**
     *  @js ctor
     */ // :3
    CCLayerColor(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCLayerColor, CCLayerRGBA) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCLayerColor(); // :3

    virtual void draw(); // :3
    virtual void setContentSize(const CCSize & var); // :3

    static CCLayerColor* create(); // :3

    /** creates a CCLayer with color, width and height in Points */
    static CCLayerColor * create(const ccColor4B& color, GLfloat width, GLfloat height);
    /** creates a CCLayer with color. Width and height are the window size. */
    static CCLayerColor * create(const ccColor4B& color);

    virtual bool init();
    /** initializes a CCLayer with color, width and height in Points */
    virtual bool initWithColor(const ccColor4B& color, GLfloat width, GLfloat height);
    /** initializes a CCLayer with color. Width and height are the window size. */
    virtual bool initWithColor(const ccColor4B& color);

    /** change width in Points*/
    void changeWidth(GLfloat w);
    /** change height in Points*/
    void changeHeight(GLfloat h);
    /** change width and height in Points
    @since v0.8
    */ // :3
    void changeWidthAndHeight(GLfloat w ,GLfloat h); // :3

    /** BlendFunction. Conforms to CCBlendProtocol protocol */
    CC_PROPERTY(ccBlendFunc, m_tBlendFunc, BlendFunc)

    virtual void setColor(const ccColor3B &color);
    virtual void setOpacity(GLubyte opacity);

	void addToVertices(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::CCPoint);
	void setVertices(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::CCPoint);


protected:
    virtual void updateColor();
};

//
// CCLayerGradient
//
/** @brief CCLayerGradient is a subclass of CCLayerColor that draws gradients across the background.

All features from CCLayerColor are valid, plus the following new features:
- direction
- final color
- interpolation mode

Color is interpolated between the startColor and endColor along the given
vector (starting at the origin, ending at the terminus).  If no vector is
supplied, it defaults to (0, -1) -- a fade from top to bottom.

If 'compressedInterpolation' is disabled, you will not see either the start or end color for
non-cardinal vectors; a smooth gradient implying both end points will be still
be drawn, however.

If ' compressedInterpolation' is enabled (default mode) you will see both the start and end colors of the gradient.

@since v0.99.5
*/ // :3
class CC_DLL CCLayerGradient : public CCLayerColor // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCLayerGradient, CCLayerColor) // :3
    CCLayerGradient() {} // :3


    /** Creates a full-screen CCLayer with a gradient between start and end. */
    static CCLayerGradient* create(const ccColor4B& start, const ccColor4B& end);

    /** Creates a full-screen CCLayer with a gradient between start and end in the direction of v. */
    static CCLayerGradient* create(const ccColor4B& start, const ccColor4B& end, const CCPoint& v);

    virtual bool init();

    // @note RobTop Addition
    virtual void visit();

    virtual void updateColor();
    /** Initializes the CCLayer with a gradient between start and end.
     *  @js init
     */ // :3
    virtual bool initWithColor(const ccColor4B& start, const ccColor4B& end); // :3

    /** Initializes the CCLayer with a gradient between start and end in the direction of v.
     *  @js init
     */ // :3
    virtual bool initWithColor(const ccColor4B& start, const ccColor4B& end, const CCPoint& v); // :3

    CC_PROPERTY_PASS_BY_REF(ccColor3B, m_startColor, StartColor) // :3
    CC_PROPERTY_PASS_BY_REF(ccColor3B, m_endColor, EndColor) // :3
    CC_PROPERTY(GLubyte, m_cStartOpacity, StartOpacity) // :3
    CC_PROPERTY(GLubyte, m_cEndOpacity, EndOpacity) // :3
    CC_PROPERTY_PASS_BY_REF(CCPoint, m_AlongVector, Vector) // :3

	void setValues(cocos2d::_ccColor3B const&, unsigned char, cocos2d::_ccColor3B const&, unsigned char, cocos2d::CCPoint const&); // :3


    /** Whether or not the interpolation will be compressed in order to display all the colors of the gradient both in canonical and non canonical vectors
    Default: YES
    */ // :3
protected: // :3
    bool m_bCompressedInterpolation; // :3
    CC_SYNTHESIZE_NV(bool, m_bShouldPremultiply, ShouldPremultiply) // :3
public: // :3
    virtual void setCompressedInterpolation(bool bCompressedInterpolation); // :3
    virtual bool isCompressedInterpolation(); // :3

    static CCLayerGradient* create(); // :3
}; // :3


/** @brief CCMultipleLayer is a CCLayer with the ability to multiplex it's children.
Features:
- It supports one or more children
- Only one children will be active a time
*/ // :3
class CC_DLL CCLayerMultiplex : public CCLayer // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
protected: // :3


    unsigned int m_nEnabledLayer; // :3
    CCArray*     m_pLayers; // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCLayerMultiplex, CCLayer) // :3
    /**
     * @js ctor
     * @lua NA
     */ // :3
    CCLayerMultiplex(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCLayerMultiplex(); // :3
    /**
     * @js NA
     */ // :3
    static CCLayerMultiplex* create(); // :3

    /** creates a CCMultiplexLayer with an array of layers.
     * @since v2.1
     * @js NA
     */ // :3
    static CCLayerMultiplex* createWithArray(CCArray* arrayOfLayers); // :3

    /** creates a CCLayerMultiplex with one or more layers using a variable argument list.
     * @lua NA
     */ // :3
    static CCLayerMultiplex * create(CCLayer* layer, ... ); // :3

    /**
     * lua script can not init with undetermined number of variables
     * so add these functions to be used with lua.
     */ // :3
    static CCLayerMultiplex * createWithLayer(CCLayer* layer); // :3

    void addLayer(CCLayer* layer); // :3

    /** initializes a MultiplexLayer with one or more layers using a variable argument list.
     *  @js NA
     *  @lua NA
     */ // :3
    bool initWithLayers(CCLayer* layer, va_list params); // :3
    /** switches to a certain layer indexed by n.
    The current (old) layer will be removed from it's parent with 'cleanup:YES'.
    */ // :3

    /** initializes a CCMultiplexLayer with an array of layers
     *  @since v2.1
     *  @lua NA
     */ // :3
    bool initWithArray(CCArray* arrayOfLayers); // :3

    void switchTo(unsigned int n); // :3
    /** release the current layer and switches to another layer indexed by n.
    The current (old) layer will be removed from it's parent with 'cleanup:YES'.
    */ // :3
    void switchToAndReleaseMe(unsigned int n); // :3
}; // :3


// end of layer group :3
/// @} :3

NS_CC_END // :3

#endif // __CCLAYER_H__ :3

