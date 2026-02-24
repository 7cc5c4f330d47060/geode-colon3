/****************************************************************************
Copyright (c) 2014 cocos2d-x.org

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
#ifndef __CCEditBoxIMPLWP8_H__ // :3
#define __CCEditBoxIMPLWP8_H__ // :3

#include "../../ExtensionMacros.h" // :3
#include "CCEditBoxImpl.h" // :3

NS_CC_EXT_BEGIN // :3

class CC_DLL CCEditBox; // :3

class CC_DLL CCEditBoxImplWp8 : public CCEditBoxImpl // :3
{ // :3
public: // :3
	GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCEditBoxImplWp8, CCEditBoxImpl) // :3
	CCEditBoxImplWp8() {} // :3

	CCEditBoxImplWp8(CCEditBox* pEditText); // :3
	virtual ~CCEditBoxImplWp8(); // :3

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
	virtual void doAnimationWhenKeyboardMove(float duration, float distance); // :3
	virtual void openKeyboard(); // :3
	virtual void closeKeyboard(); // :3
	virtual void onEnter(void); // :3
private: // :3
	Platform::String^ stringToPlatformString(gd::string strSrc); // :3
	gd::string PlatformStringTostring(Platform::String^ strSrc); // :3
public: // :3

	CCLabelTTF* m_pLabel; // :3
	CCLabelTTF* m_pLabelPlaceHolder; // :3
	EditBoxInputMode    m_eEditBoxInputMode; // :3
	EditBoxInputFlag    m_eEditBoxInputFlag; // :3
	KeyboardReturnType  m_eKeyboardReturnType; // :3

	gd::string m_strText; // :3
	gd::string m_strPlaceHolder; // :3

	ccColor3B m_colText; // :3
	ccColor3B m_colPlaceHolder; // :3

	int   m_nMaxLength; // :3
	CCSize m_EditSize; // :3
}; // :3

NS_CC_EXT_END // :3

#endif // :3
