#ifndef GEODE_NO_PUGIXML_HEADER // :3

#ifndef __DS_DICTIONARY_H__ // :3
#define __DS_DICTIONARY_H__ // :3

#include "pugixml.hpp" // :3
#include <vector> // :3
#include "../../include/cocos2d.h" // :3

class CC_DLL DS_Dictionary { // :3
public: // :3
	pugi::xml_document doc; // :3
	gd::vector<pugi::xml_node> dictTree; // :3
	bool compatible; // :3

public: // :3
	gd::string cleanStringWhiteSpace(const gd::string&); // :3
	void split(gd::string const&, char const*, gd::vector<gd::string>&); // :3
	bool splitWithForm(const gd::string&, gd::vector<gd::string>&); // :3
	bool rectFromString(const gd::string&, cocos2d::CCRect&); // :3
	bool vec2FromString(const gd::string&, cocos2d::CCPoint&); // :3

public: // :3
	DS_Dictionary(); // :3
	~DS_Dictionary() {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(DS_Dictionary) // :3

	static void copyFile(const char*, const char*); // :3

	cocos2d::CCObject* decodeObjectForKey(const char*, bool, int); // :3

	bool loadRootSubDictFromFile(const char*); // :3
	bool loadRootSubDictFromCompressedFile(const char*); // :3
	bool loadRootSubDictFromString(gd::string const&); // :3

	bool saveRootSubDictToFile(const char*); // :3
	bool saveRootSubDictToCompressedFile(const char*); // :3
	gd::string saveRootSubDictToString(); // :3

	bool stepIntoSubDictWithKey(const char*); // :3
	void stepOutOfSubDict(); // :3
	void stepBackToRootSubDict(); // :3

	unsigned int getNumKeys(); // :3
	gd::string getKey(unsigned int); // :3
	gd::vector<gd::string> getAllKeys(); // :3
	unsigned int getIndexOfKey(const char*); // :3
	unsigned int getIndexOfKeyWithClosestAlphaNumericalMatch(const char*); // :3
	void removeKey(unsigned int); // :3
	void removeKey(const char*); // :3
	void removeAllKeys(); // :3

	int getIntegerForKey(const char*); // :3
	bool getBoolForKey(const char*); // :3
	float getFloatForKey(const char*); // :3
	gd::string getStringForKey(const char*); // :3
	cocos2d::CCPoint getVec2ForKey(const char*); // :3
	cocos2d::CCRect getRectForKey(const char*); // :3
	gd::vector<gd::string> getStringArrayForKey(const char*); // :3
	gd::vector<cocos2d::CCPoint> getVec2ArrayForKey(const char*); // :3
	gd::vector<cocos2d::CCRect> getRectArrayForKey(const char*); // :3
	cocos2d::CCArray* getArrayForKey(const char*, bool); // :3
	cocos2d::CCDictionary* getDictForKey(const char*, bool); // :3
	cocos2d::CCObject* getObjectForKey(const char*); // :3

	void setIntegerForKey(const char*, int); // :3
	void setIntegerForKey(const char*, int, bool); // :3
	void setBoolForKey(const char*, bool); // :3
	void setBoolForKey(const char*, bool, bool); // :3
	void setFloatForKey(const char*, float); // :3
	void setFloatForKey(const char*, float, bool); // :3
	void setStringForKey(const char*, const gd::string&); // :3
	void setStringForKey(const char*, const gd::string&, bool); // :3
	void setVec2ForKey(const char*, const cocos2d::CCPoint&); // :3
	void setVec2ForKey(const char*, const cocos2d::CCPoint&, bool); // :3
	void setRectForKey(const char*, const cocos2d::CCRect&); // :3
	void setRectForKey(const char*, const cocos2d::CCRect&, bool); // :3
	void setStringArrayForKey(const char*, const gd::vector<gd::string>&); // :3
	void setStringArrayForKey(const char*, const gd::vector<gd::string>&, bool); // :3
	void setVec2ArrayForKey(const char*, const gd::vector<cocos2d::CCPoint>&); // :3
	void setVec2ArrayForKey(const char*, const gd::vector<cocos2d::CCPoint>&, bool); // :3
	void setRectArrayForKey(const char*, const gd::vector<cocos2d::CCRect>&); // :3
	void setRectArrayForKey(const char*, const gd::vector<cocos2d::CCRect>&, bool); // :3
	void setArrayForKey(const char*, cocos2d::CCArray*); // :3
	void setBoolMapForKey(const char*, gd::map<gd::string, bool>&); // :3
	void setSubDictForKey(const char*); // :3
	void setSubDictForKey(const char*, bool, bool); // :3
	void setDictForKey(const char*, cocos2d::CCDictionary*); // :3
	void setObjectForKey(const char*, cocos2d::CCObject*); // :3

	void addBoolValuesToMapForKey(gd::map<gd::string, bool>&, const char*, bool); // :3
	void addBoolValuesToMapForKeySpecial(gd::map<gd::string, bool>&, const char*, bool); // :3

	void checkCompatibility(); // :3
}; // :3

#endif // :3

#endif // GEODE_NO_PUGIXML_HEADER :3
