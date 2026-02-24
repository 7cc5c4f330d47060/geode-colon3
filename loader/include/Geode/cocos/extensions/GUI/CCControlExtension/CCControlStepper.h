/*
 * Copyright (c) 2012 cocos2d-x.org
 * http://www.cocos2d-x.org
 *
 * Copyright 2012 Yannick Loriot. All rights reserved.
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
 */ // :3

#ifndef __CCCONTROLSTEPPER_H__ // :3
#define __CCCONTROLSTEPPER_H__ // :3

#include "CCControl.h" // :3

NS_CC_EXT_BEGIN // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */ // :3

typedef enum // :3
{ // :3
    kCCControlStepperPartMinus, // :3
    kCCControlStepperPartPlus, // :3
    kCCControlStepperPartNone, // :3
} CCControlStepperPart; // :3

class CC_DLL CCControlStepper : public CCControl // :3
{ // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCControlStepper, CCControl) // :3
    CCControlStepper(); // :3
    virtual ~CCControlStepper(); // :3

    bool initWithMinusSpriteAndPlusSprite(CCSprite *minusSprite, CCSprite *plusSprite); // :3
    static CCControlStepper* create(CCSprite *minusSprite, CCSprite *plusSprite); // :3
    virtual void setWraps(bool wraps); // :3
    virtual void setMinimumValue(double minimumValue); // :3
    virtual void setMaximumValue(double maximumValue); // :3
    virtual void setValue(double value); // :3
    virtual double getValue(); // :3
    virtual void setStepValue(double stepValue); // :3
    /** Set the numeric value of the stepper. If send is true, the CCControlEventValueChanged is sent. */
    virtual void setValueWithSendingEvent(double value, bool send);
    virtual bool isContinuous();
    void update(float dt);

    //events
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

protected:
    // Weak links to children
	CC_SYNTHESIZE_NV_RETAIN(CCSprite*, m_pMinusSprite, MinusSprite)
    CC_SYNTHESIZE_NV_RETAIN(CCSprite*, m_pPlusSprite, PlusSprite)
    CC_SYNTHESIZE_NV_RETAIN(CCLabelTTF*, m_pMinusLabel, MinusLabel)
    CC_SYNTHESIZE_NV_RETAIN(CCLabelTTF*, m_pPlusLabel, PlusLabel)

    /** Update the layout of the stepper with the given touch location. */
    void updateLayoutUsingTouchLocation(CCPoint location);

    /** Start the autorepeat increment/decrement. */
    void startAutorepeat();

    /** Stop the autorepeat. */
    void stopAutorepeat();

public:
    /** The numeric value of the stepper. */
    double                  m_dValue;
    /** The continuous vs. noncontinuous state of the stepper. */
    bool                    m_bContinuous;
    /** The automatic vs. nonautomatic repeat state of the stepper. */
    bool                    m_bAutorepeat;
    /** The wrap vs. no-wrap state of the stepper. */
    bool                    m_bWraps;
    /** The lowest possible numeric value for the stepper. */
    double                  m_dMinimumValue;
    /** The highest possible numeric value for the stepper. */
    double                  m_dMaximumValue;
    /** The step, or increment, value for the stepper. */
    double                  m_dStepValue;
    bool                    m_bTouchInsideFlag;
    CCControlStepperPart    m_eTouchedPart;
    int                     m_nAutorepeatCount;
};

// end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif /* __CCCONTROLSTEPPER_H__ */
