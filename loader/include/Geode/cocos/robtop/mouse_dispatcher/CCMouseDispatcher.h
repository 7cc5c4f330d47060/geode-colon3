#ifndef __CCMOUSE_DISPATCHER_H__ // :3
#define __CCMOUSE_DISPATCHER_H__ // :3

#include "CCMouseDelegate.h" // :3
#include "../../cocoa/CCArray.h" // :3

NS_CC_BEGIN // :3

// @note RobTop Addition :3
class CC_DLL CCMouseDispatcher : public CCObject // :3
{ // :3
	GEODE_FRIEND_MODIFY // :3
public: // :3
	GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMouseDispatcher, CCObject) // :3
	CCMouseDispatcher(); // :3
	virtual ~CCMouseDispatcher(); // :3

	void addDelegate(CCMouseDelegate* pDelegate); // :3

	void removeDelegate(CCMouseDelegate* pDelegate); // :3

	void forceAddDelegate(CCMouseDelegate* pDelegate); // :3

	void forceRemoveDelegate(CCMouseDelegate* pDelegate); // :3

	bool dispatchScrollMSG(float y, float x); // :3

public: // :3
	CCArray* m_pMouseHandlers; // :3
	bool m_bLocked; // :3
	bool m_bToAdd; // :3
	bool m_bToRemove; // :3
	ccCArray* m_pHandlersToAdd; // :3
	ccCArray* m_pHandlersToRemove; // :3
}; // :3

NS_CC_END // :3

#endif // :3
