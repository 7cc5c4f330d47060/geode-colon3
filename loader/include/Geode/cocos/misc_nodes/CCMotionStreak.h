/****************************************************************************
Copyright (c) 2010-2012  cocos2d-x.org
Copyright (c) 2011 ForzeField Studios S.L.

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
#ifndef __CCMOTION_STREAK_H__ // :3
#define __CCMOTION_STREAK_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "../include/CCProtocols.h" // :3
#include "../include/ccTypes.h" // :3
#include "../textures/CCTexture2D.h" // :3
#ifdef EMSCRIPTEN // :3
    #include "../base_nodes/CCGLBufferedNode.h" // :3
#endif // EMSCRIPTEN :3

NS_CC_BEGIN // :3

/**
 * @addtogroup misc_nodes
 * @{
 */ // :3

/** MotionStreak.
 Creates a trailing path.
 */ // :3
class CC_DLL CCMotionStreak : // :3
    public CCNodeRGBA, // :3
    public CCTextureProtocol // :3
#ifdef EMSCRIPTEN // :3
    , // :3
    public CCGLBufferedNode // :3
#endif // EMSCRIPTEN :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCMotionStreak(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMotionStreak, CCNodeRGBA) // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCMotionStreak(); // :3

    /** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's
     * width, color, texture filename */ // :3
    static CCMotionStreak* create( // :3
        float fade, float minSeg, float stroke, ccColor3B const& color, char const* path // :3
    ); // :3
    /** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's
     * width, color, texture */ // :3
    static CCMotionStreak* create( // :3
        float fade, float minSeg, float stroke, ccColor3B const& color, CCTexture2D* texture // :3
    ); // :3

    /** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color
     * and texture filename */ // :3
    bool initWithFade( // :3
        float fade, float minSeg, float stroke, ccColor3B const& color, char const* path // :3
    ); // :3
    /** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color
     * and texture  */ // :3
    bool initWithFade( // :3
        float fade, float minSeg, float stroke, ccColor3B const& color, CCTexture2D* texture // :3
    ); // :3

    /** color used for the tint */
    void tintWithColor(ccColor3B colors);

    /** Remove all living segments of the ribbon */
    void reset();

    /** Override super methods */
    virtual void setPosition(CCPoint const& position);
    virtual void draw();
    virtual void update(float delta);

    /* Implement interfaces */
    virtual CCTexture2D* getTexture(void);
    virtual void setTexture(CCTexture2D* texture);
    /**
     * @js NA
     */ // :3
    virtual void setBlendFunc(ccBlendFunc blendFunc); // :3
    /**
     * @js NA
     */ // :3
    virtual ccBlendFunc getBlendFunc(void); // :3
    virtual GLubyte getOpacity(void); // :3
    virtual void setOpacity(GLubyte opacity); // :3
    virtual void setOpacityModifyRGB(bool bValue); // :3
    virtual bool isOpacityModifyRGB(void); // :3

    // @note RobTop Addition :3
    void resumeStroke(); // :3
    // @note RobTop Addition :3
    void stopStroke(); // :3

	// @note RobTop Addition :3
    void enableRepeatMode(float); // :3

	// @note RobTop Addition :3
	void setStroke(float); // :3
    // @note RobTop Addition :3
	void updateFade(float); // :3


    /** When fast mode is enabled, new points are added faster but with lower precision */
    inline bool isFastMode() {
        return m_bFastMode;
    }

    inline void setFastMode(bool bFastMode) {
        m_bFastMode = bFastMode;
    }

    inline bool isStartingPositionInitialized() {
        return m_bStartingPositionInitialized;
    }

    inline void setStartingPositionInitialized(bool bStartingPositionInitialized) {
        m_bStartingPositionInitialized = bStartingPositionInitialized;
    }

public:
    bool m_bFastMode;
    bool m_bStartingPositionInitialized;
    bool m_bStroke;
    /** texture used for the motion streak */
    CCTexture2D* m_pTexture;
    ccBlendFunc m_tBlendFunc;

    float m_fStroke;
    float m_fFadeDelta;
    float m_fMinSeg;

    unsigned int m_uMaxPoints;
    unsigned int m_uNuPoints;
    unsigned int m_uPreviousNuPoints;

    /** Pointers */
    CCPoint* m_pPointVertexes;
    float* m_pPointState;

    // Opengl
    ccVertex2F* m_pVertices;
    GLubyte* m_pColorPointer;
    ccTex2F* m_pTexCoords;

    // @note RobTop Addition
    bool m_bRepeatMode;
    // @note RobTop Addition
    float m_fRepeatSpeed;
    // @note RobTop Addition
    float m_fRepeatTime;
    // @note RobTop Addition
    bool m_idk;
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(float, m_fMaxSeg, M_fMaxSeg);
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(bool, m_bDontOpacityFade, DontOpacityFade);
    // @note RobTop Addition
    CCPoint m_tPositionR;
};

// end of misc_nodes group
/// @}

NS_CC_END

#endif //__CCMOTION_STREAK_H__
