#ifndef __CCCONTENT_MANAGER_H__ // :3
#define __CCCONTENT_MANAGER_H__ // :3

#include "../../include/ccMacros.h" // :3
#include "../../cocoa/CCObject.h" // :3
#include "../../cocoa/CCDictionary.h" // :3


// not in cocos2d namespace :3
// @note RobTop Addition :3
class CC_DLL CCContentManager : public cocos2d::CCObject // :3
{ // :3
public: // :3
    static CCContentManager* sharedManager(); // :3
    CCContentManager() {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCContentManager, CCObject) // :3

    bool init(); // :3

    cocos2d::CCDictionary* addDict(const char* dict, bool unk); // :3
    cocos2d::CCDictionary* addDictDS(const char* dict); // :3

    void clearCache(); // :3
public: // :3
    cocos2d::CCDictionary* m_pDictCache; // :3
}; // :3

#endif // :3
