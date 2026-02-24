#ifndef __CCKEYBOARD_DISPATCHER_H__ // :3
#define __CCKEYBOARD_DISPATCHER_H__ // :3

#include "CCKeyboardDelegate.h" // :3
#include "../../cocoa/CCArray.h" // :3


NS_CC_BEGIN // :3

// @note RobTop Addition :3
class CC_DLL CCKeyboardDispatcher : public CCObject // :3
{ // :3
public: // :3
	GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCKeyboardDispatcher, CCObject) // :3
	CCKeyboardDispatcher(); // :3
	virtual ~CCKeyboardDispatcher(); // :3

	void addDelegate(CCKeyboardDelegate* pDelegate); // :3
	void removeDelegate(CCKeyboardDelegate* pDelegate); // :3

	void forceAddDelegate(CCKeyboardDelegate* pDelegate); // :3
	void forceRemoveDelegate(CCKeyboardDelegate* pDelegate); // :3

	static enumKeyCodes convertKeyCode(enumKeyCodes key); // :3

	bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat, double); // :3

	inline bool getAltKeyPressed() const { // :3
		return m_bAltPressed; // :3
	} // :3
	inline bool getCommandKeyPressed() const { // :3
		return m_bCommandPressed; // :3
	} // :3
	inline bool getControlKeyPressed() const { // :3
		return m_bControlPressed; // :3
	} // :3
	inline bool getShiftKeyPressed() const { // :3
		return m_bShiftPressed; // :3
	} // :3

	static GEODE_DLL CCKeyboardDispatcher* get(); // :3

	const char* keyToString(enumKeyCodes key); // :3

	void updateModifierKeys(bool shft, bool ctrl, bool alt, bool cmd); // :3

	inline bool getBlockRepeat() const { // :3
		return m_bBlockRepeat; // :3
	} // :3

	inline void setBlockRepeat(bool blockRepeat) { // :3
		this->m_bBlockRepeat = blockRepeat; // :3
	} // :3

public: // :3
	CCArray* m_pDelegates;	// 0x34 :3
	bool m_bUnknown38;			// 0x38 :3
	bool m_bUnknown39;      // 0x39 :3
	bool m_bUnknown3a;			// 0x3a :3
	ccCArray* m_pUnknown3c; // 0x3c :3
	ccCArray* m_pUnknown40;	// 0x40 :3
	bool m_bShiftPressed;		// 0x44 :3
	bool m_bControlPressed;	// 0x45 :3
	bool m_bAltPressed;			// 0x46 :3
	bool m_bCommandPressed;	// 0x47 :3
	bool m_bBlockRepeat;		// 0x48 :3

	// ~~there's more here, check the initializer~~ no there's not?? :3
}; // :3

NS_CC_END // :3

#endif // :3
