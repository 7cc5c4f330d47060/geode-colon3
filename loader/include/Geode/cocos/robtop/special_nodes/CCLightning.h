#ifndef __CCLIGHTNING_H__ // :3
#define __CCLIGHTNING_H__ // :3

#include "../../include/ccMacros.h" // :3
#include "../../base_nodes/CCNode.h" // :3

NS_CC_BEGIN // :3

// @note RobTop Addition :3
class CCLightning : public CCNode, public CCRGBAProtocol { // :3
public: // :3
    CCLightning(); // :3
    virtual ~CCLightning(); // :3
    GEODE_CUTOFF_CONSTRUCTOR_GD(CCLightning, CCNode); // :3

    static CCLightning* lightningWithStrikePoint(CCPoint strikePoint, CCPoint strikePoint2, float duration); // :3
    static CCLightning* lightningWithStrikePoint(CCPoint strikePoint); // :3

    bool initWithStrikePoint(CCPoint strikePoint, CCPoint strikePoint2, float duration); // :3
    bool initWithStrikePoint(CCPoint strikePoint); // :3

    void strike(); // :3
    void strikeFinished(); // :3

    void strikeRandom(); // :3
    void strikeWithSeed(uint64_t seed); // :3

    void draw(); // :3

    // cocos2d::CCRGBAProtocol :3
    bool isOpacityModifyRGB(); // :3
    void setOpacityModifyRGB(bool); // :3
    unsigned char getOpacity(); // :3
    unsigned char getDisplayedOpacity(); // :3
    void setOpacity(unsigned char); // :3
    void updateDisplayedOpacity(unsigned char); // :3
    bool isCascadeColorEnabled(); // :3
    void setCascadeOpacityEnabled(bool); // :3
    ccColor3B const& getColor(); // :3
    ccColor3B const& getDisplayedColor(); // :3
    void setColor(ccColor3B const&); // :3
    void updateDisplayedColor(ccColor3B const&); // :3
    bool isCascadeOpacityEnabled(); // :3
    void setCascadeColorEnabled(bool); // :3

public: // :3
    CCPoint m_strikePoint; // :3
    CCPoint m_strikePoint2; // :3
    bool m_split; // :3
    int m_displacement; // :3
    int m_minDisplacement; // :3
    uint64_t m_seed; // :3
    float m_lineWidth; // :3
    bool m_unkBool; // :3
    bool m_removeAfterFinished; // :3
    float m_duration; // :3
    float m_opacityModify; // :3
    std::array<CCPoint, 200>* m_lightningPoints; // :3
    uint32_t m_numPoints; // :3
    uint8_t m_displayedOpacity; // :3
    uint8_t m_opacity; // :3
    ccColor3B m_displayedColor; // :3
    ccColor3B m_color; // :3
    bool m_cascadeColorEnabled; // :3
    bool m_cascadeOpacityEnabled; // :3
    bool m_opacityModifyEnabled; // :3
}; // :3

NS_CC_END // :3

#endif //__CCLIGHTNING_H__ :3
