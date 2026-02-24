#ifndef __CCFONTSPRITE_H__ // :3
#define __CCFONTSPRITE_H__ // :3

#include "../../include/ccMacros.h" // :3
#include "CCSpriteExtra.h" // :3

NS_CC_BEGIN // :3

// @note RobTop Addition :3
class CC_DLL CCFontSprite : public CCSpriteExtra { // :3
public: // :3
    CCFontSprite() // :3
    : m_bUseInstant(false) // :3
    , m_fInstantTime(0.0f) // :3
    , m_fDelay(0.0f) // :3
    , m_fShakesPerSecond(0.0f) // :3
    , m_fShakeIntensity(0.0f) // :3
    , m_fShakeElapsed(0.0f) // :3
    , m_nShakeIndex(0) // :3
    {} // :3
    virtual ~CCFontSprite() {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCFontSprite, CCSpriteExtra); // :3

public: // :3
    bool m_bUseInstant; // :3
    float m_fInstantTime; // :3
    float m_fDelay; // :3
    float m_fShakesPerSecond; // :3
    float m_fShakeIntensity; // :3
    float m_fShakeElapsed; // :3
    int m_nShakeIndex; // :3
    CCPoint m_obShakePosition; // :3
}; // :3

NS_CC_END // :3

#endif // __CCFONTSPRITE_H__ :3
