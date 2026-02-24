/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2009      Leonardo Kasperavičius
Copyright (c) 2011      Zynga Inc.

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
#ifndef __CC_PARTICLE_SYSTEM_QUAD_H__ // :3
#define __CC_PARTICLE_SYSTEM_QUAD_H__ // :3

#include  "CCParticleSystem.h" // :3

NS_CC_BEGIN // :3

class CCSpriteFrame; // :3

struct ParticleStruct // :3
{ // :3
    int TotalParticles = 0; // :3
    float Duration = .0f; // :3
    float Life = .0f; // :3
    float LifeVar = .0f; // :3
    int EmissionRate = 0; // :3
    int Angle = 0; // :3
    int AngleVar = 0; // :3
    int Speed = 0; // :3
    int SpeedVar = 0; // :3
    int PosVarX = 0; // :3
    int PosVarY = 0; // :3
    int GravityX = 0; // :3
    int GravityY = 0; // :3
    int RadialAccel = 0; // :3
    int RadialAccelVar = 0; // :3
    int TangentialAccel = 0; // :3
    int TangentialAccelVar = 0; // :3

    int StartSize = 0; // :3
    int StartSizeVar = 0; // :3
    int StartSpin = 0; // :3
    int StartSpinVar = 0; // :3
    float StartColorR = .0f; // :3
    float StartColorVarR = .0f; // :3
    float StartColorG = .0f; // :3
    float StartColorVarG = .0f; // :3
    float StartColorB = .0f; // :3
    float StartColorVarB = .0f; // :3
    float StartColorA = .0f; // :3
    float StartColorVarA = .0f; // :3

    int EndSize = 0; // :3
    int EndSizeVar = 0; // :3
    int EndSpin = 0; // :3
    int EndSpinVar = 0; // :3
    float EndColorR = .0f; // :3
    float EndColorVarR = .0f; // :3
    float EndColorG = .0f; // :3
    float EndColorVarG = .0f; // :3
    float EndColorB = .0f; // :3
    float EndColorVarB = .0f; // :3
    float EndColorA = .0f; // :3
    float EndColorVarA = .0f; // :3

    float FadeInTime = .0f; // :3
    float FadeInTimeVar = .0f; // :3
    float FadeOutTime = .0f; // :3
    float FadeOutTimeVar = .0f; // :3

    int StartRadius = 0; // :3
    int StartRadiusVar = 0; // :3
    int EndRadius = 0; // :3
    int EndRadiusVar = 0; // :3
    int RotatePerSecond = 0; // :3
    int RotatePerSecondVar = 0; // :3

    int EmitterMode = 0; // :3
    int PositionType = 0; // :3
    bool isBlendAdditive = false; // :3
    bool startSpinEqualToEndSpin = false; // :3
    bool rotationIsDir = false; // :3
    bool dynamicRotationIsDir = false; // :3
    int customParticleIdx = 0; // :3
    bool uniformColorMode = false; // :3

    float frictionPos = .0f; // :3
    float frictionPosVar = .0f; // :3

    float respawn = .0f; // :3
    float respawnVar = .0f; // :3

    bool orderSensitive = false; // :3
    bool startSizeEqualToEndSize = false; // :3
    bool startRadiusEqualToEndRadius = false; // :3

    bool startRGBVarSync = false; // :3
    bool endRGBVarSync = false; // :3

    float frictionSize = .0f; // :3
    float frictionSizeVar = .0f; // :3

    float frictionRot = .0f; // :3
    float frictionRotVar = .0f; // :3

    gd::string sFrame; // :3
}; // :3

/**
 * @addtogroup particle_nodes
 * @{
 */ // :3

/** @brief CCParticleSystemQuad is a subclass of CCParticleSystem

It includes all the features of ParticleSystem.

Special features and Limitations:
- Particle size can be any float number.
- The system can be scaled
- The particles can be rotated
- It supports subrects
- It supports batched rendering since 1.1
@since v0.8
*/ // :3
class CC_DLL CCParticleSystemQuad : public CCParticleSystem // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    ccV3F_C4B_T2F_Quad    *m_pQuads;        // quads to be rendered :3
    GLushort            *m_pIndices;    // indices :3

#if CC_TEXTURE_ATLAS_USE_VAO // :3
    GLuint                m_uVAOname; // :3
#endif // :3

    GLuint                m_pBuffersVBO[2]; //0: vertex  1: indices :3

    // @note Robtop Addition :3
    CCRect m_tTextureRect; // :3
    // @note Robtop Addition :3
    ccColor4B m_tQuadColor; // :3
    // @note Robtop Addition :3
    GLushort m_uParticleIdx; // :3
    // @note Robtop Addition :3
    GLubyte m_uOpacity; // :3

public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleSystemQuad(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCParticleSystemQuad, CCParticleSystem) // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleSystemQuad(); // :3

    /** creates an initializes a CCParticleSystemQuad from a plist file.
    This plist files can be created manually or with Particle Designer:
    */ // :3
    static CCParticleSystemQuad * create(const char *plistFile); // :3

    /** initializes the indices for the vertices*/
    void initIndices();

    /** initializes the texture with a rectangle measured Points */
    void initTexCoordsWithRect(const CCRect& rect);

    /** Sets a new CCSpriteFrame as particle.
    WARNING: this method is experimental. Use setTexture:withRect instead.
    @since v0.99.4
    */ // :3
    void setDisplayFrame(CCSpriteFrame *spriteFrame); // :3

    /** Sets a new texture with a rect. The rect is in Points.
    @since v0.99.4
    */ // :3
    void setTextureWithRect(CCTexture2D *texture, const CCRect& rect); // :3
    /** super methods
     *  @js NA
     *  @lua NA
     */ // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles, bool); // :3
    /**
     * @js NA
     */ // :3
    virtual void setTexture(CCTexture2D* texture); // :3
    /**
     * @js NA
     */ // :3
    virtual void updateQuadWithParticle(tCCParticle* particle, const CCPoint& newPosition); // :3
    /**
     * @js NA
     */ // :3
    virtual void postStep(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual void draw(); // :3
    /**
     * @js NA
     */ // :3
    virtual void setBatchNode(CCParticleBatchNode* batchNode); // :3
    /**
     * @js NA
     */ // :3
    virtual void setTotalParticles(unsigned int tp); // :3

    /** listen the event that coming to foreground on Android
     *  @js NA
     *  @lua NA
     */ // :3
    void listenBackToForeground(CCObject *obj); // :3

    static CCParticleSystemQuad * create(); // :3
    static CCParticleSystemQuad * create(const char*, bool); // :3
    static CCParticleSystemQuad * createWithTotalParticles(unsigned int numberOfParticles, bool); // :3

	GLubyte getOpacity() { return m_uOpacity; } // :3
	void setOpacity(GLubyte opacity) { m_uOpacity = opacity; } // :3

	void updateTexCoords(); // :3

private: // :3
#if CC_TEXTURE_ATLAS_USE_VAO // :3
    void setupVBOandVAO(); // :3
#else // :3
    void setupVBO(); // :3
#endif // :3
    bool allocMemory(); // :3
}; // :3

// end of particle_nodes group :3
/// @} :3

NS_CC_END // :3

#endif //__CC_PARTICLE_SYSTEM_QUAD_H__ :3

