/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 James Chen

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

#ifndef __CCEditBoxIMPLIOS_H__ // :3
#define __CCEditBoxIMPLIOS_H__ // :3

#include "../../../include/cocos2d.h" // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // :3

#include "../../ExtensionMacros.h" // :3
#include "CCEditBoxImpl.h" // :3

#import <Foundation/Foundation.h> // :3
#import <UIKit/UIKit.h> // :3

@interface CustomUITextField : UITextField // :3
{ // :3
} // :3

@end // :3


@interface EditBoxImplIOS : NSObject <UITextFieldDelegate> // :3
{ // :3
    CustomUITextField* textField_; // :3
    void* editBox_; // :3
    BOOL editState_; // :3
} // :3

@property(nonatomic, retain) UITextField* textField; // :3
@property(nonatomic, readonly, getter = isEditState) BOOL editState; // :3
@property(nonatomic, assign) void* editBox; // :3

-(id) initWithFrame: (CGRect) frameRect editBox: (void*) editBox; // :3
-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance; // :3
-(void) setPosition:(CGPoint) pos; // :3
-(void) setContentSize:(CGSize) size; // :3
-(void) visit; // :3
-(void) openKeyboard; // :3
-(void) closeKeyboard; // :3

@end // :3

NS_CC_EXT_BEGIN // :3

class CC_DLL CCEditBox; // :3
/**
 *  @js NA
 *  @lua NA
 */ // :3
class CC_DLL CCEditBoxImplIOS : public CCEditBoxImpl // :3
{ // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCEditBoxImplIOS, CCEditBoxImpl) // :3
    CCEditBoxImplIOS() {} // :3

    CCEditBoxImplIOS(CCEditBox* pEditText); // :3
    virtual ~CCEditBoxImplIOS(); // :3

    virtual bool initWithSize(const CCSize& size); // :3
    virtual void setFont(const char* pFontName, int fontSize); // :3
    virtual void setFontColor(const ccColor3B& color); // :3
    virtual void setPlaceholderFont(const char* pFontName, int fontSize); // :3
    virtual void setPlaceholderFontColor(const ccColor3B& color); // :3
    virtual void setInputMode(EditBoxInputMode inputMode); // :3
    virtual void setInputFlag(EditBoxInputFlag inputFlag); // :3
    virtual void setMaxLength(int maxLength); // :3
    virtual int  getMaxLength(); // :3
    virtual void setReturnType(KeyboardReturnType returnType); // :3
    virtual bool isEditing(); // :3

    virtual void setText(const char* pText); // :3
    virtual const char* getText(void); // :3
    virtual void setPlaceHolder(const char* pText); // :3
    virtual void setPosition(const CCPoint& pos); // :3
    virtual void setVisible(bool visible); // :3
    virtual void setContentSize(const CCSize& size); // :3
	virtual void setAnchorPoint(const CCPoint& anchorPoint); // :3
    virtual void visit(void); // :3
	virtual void onEnter(void); // :3
    virtual void doAnimationWhenKeyboardMove(float duration, float distance); // :3
    virtual void openKeyboard(); // :3
    virtual void closeKeyboard(); // :3

	virtual void onEndEditing(); // :3

private: // :3
	void			initInactiveLabels(const CCSize& size); // :3
	void			setInactiveText(const char* pText); // :3
	void			adjustTextFieldPosition(); // :3
    void            placeInactiveLabels(); // :3

public: // :3
    CCLabelTTF*     m_pLabel; // :3
    CCLabelTTF*     m_pLabelPlaceHolder; // :3
    CCSize          m_tContentSize; // :3
    CCPoint         m_obPosition; // :3
    CCPoint         m_obAnchorPoint; // :3
    EditBoxImplIOS* m_systemControl; // :3
    int             m_nMaxTextLength; // :3
    bool            m_bInRetinaMode; // :3
}; // :3


NS_CC_EXT_END // :3


#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) */

#endif /* __CCEditBoxIMPLIOS_H__ */

