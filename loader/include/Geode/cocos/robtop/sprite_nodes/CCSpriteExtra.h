#ifndef __CCSPRITEEXTRA_H__ // :3
#define __CCSPRITEEXTRA_H__ // :3

#include "../../include/ccMacros.h" // :3
#include "../../sprite_nodes/CCSprite.h" // :3

NS_CC_BEGIN // :3

// @note RobTop Addition :3
class CC_DLL CCSpriteExtra : public CCSprite { // :3
public: // :3
    CCSpriteExtra() : m_fXOffset(0.0f), m_fYOffset(0.0f) {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSpriteExtra, CCSprite); // :3

    inline float getXOffset() const { return m_fXOffset; } // :3
    inline float getYOffset() const { return m_fYOffset; } // :3

    inline void setXOffset(float offset) { m_fXOffset = offset; } // :3
    inline void setYOffset(float offset) { m_fYOffset = offset; } // :3

protected: // :3
    float m_fXOffset; // :3
    float m_fYOffset; // :3
}; // :3

NS_CC_END // :3

#endif // __CCSPRITEEXTRA_H__ :3
