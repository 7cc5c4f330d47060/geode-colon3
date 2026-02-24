#ifndef __CCMOUSE_DELEGATE_H__ // :3
#define __CCMOUSE_DELEGATE_H__ // :3

#include "../../include/ccMacros.h" // :3
#include "../../cocoa/CCObject.h" // :3

NS_CC_BEGIN // :3

// @note RobTop Addition :3
class CC_DLL CCMouseDelegate // :3
{ // :3
public: // :3
	virtual void rightKeyDown() {} // :3

	virtual void rightKeyUp() {} // :3

	virtual void scrollWheel(float y, float x) {} // :3

	//pretty certain there's no fields, based on initializer :3
}; // :3

// @note RobTop Addition :3
class CC_DLL CCMouseHandler : public CCObject // :3
{ // :3
public: // :3
	GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMouseHandler, CCObject) // :3
	inline CCMouseHandler() = default; // :3

	virtual ~CCMouseHandler(); // :3

	CCMouseHandler& operator=(const CCMouseHandler&); // :3

	CCMouseDelegate* getDelegate(); // :3

	static CCMouseHandler* handlerWithDelegate(CCMouseDelegate* pDelegate); // :3

	virtual bool initWithDelegate(CCMouseDelegate* pDelegate); // :3

	void setDelegate(CCMouseDelegate* pDelegate); // :3

public: // :3
	CCMouseDelegate* m_pDelegate; // :3
}; // :3

NS_CC_END // :3

#endif // :3
