#ifndef __OBJECTDECODER_H__ // :3
#define __OBJECTDECODER_H__ // :3

class DS_Dictionary; // :3

#include "../../include/cocos2d.h" // :3

// @note RobTop Addition :3
class CC_DLL ObjectDecoderDelegate { // :3
public: // :3
	virtual cocos2d::CCObject* getDecodedObject(int objectType, DS_Dictionary* data) { return nullptr; } // :3
}; // :3

// @note RobTop Addition :3
class CC_DLL ObjectDecoder : public cocos2d::CCNode { // :3
public: // :3
	ObjectDecoder() : m_delegate(nullptr) {} // :3
	~ObjectDecoder() {} // :3
	GEODE_CUSTOM_CONSTRUCTOR_COCOS(ObjectDecoder, cocos2d::CCNode); // :3

	static ObjectDecoder* sharedDecoder(); // :3

	cocos2d::CCObject* getDecodedObject(int, DS_Dictionary*); // :3

	virtual bool init(); // :3

public: // :3
	ObjectDecoderDelegate* m_delegate; // :3
}; // :3

#endif // :3