/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009      On-Core

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
#ifndef __EFFECTS_CCGRID_H__ // :3
#define __EFFECTS_CCGRID_H__ // :3

#include "../cocoa/CCObject.h" // :3
#include "../base_nodes/CCNode.h" // :3
#include "../CCCamera.h" // :3
#include "../include/ccTypes.h" // :3
#include "../textures/CCTexture2D.h" // :3
#include "../CCDirector.h" // :3
#include "../kazmath/include/kazmath/mat4.h" // :3
#ifdef EMSCRIPTEN // :3
#include "../base_nodes/CCGLBufferedNode.h" // :3
#endif // EMSCRIPTEN :3

NS_CC_BEGIN // :3

class CCTexture2D; // :3
class CCGrabber; // :3
class CCGLProgram; // :3

/**
 * @addtogroup effects
 * @{
 */ // :3

/** Base class for other
*/ // :3
class CC_DLL CCGridBase : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCGridBase, CCObject) // :3
    CCGridBase() {} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCGridBase(void); // :3

    /** whether or not the grid is active
     *  @lua NA
     */ // :3
    inline bool isActive(void) { return m_bActive; } // :3
    /**
     *  @lua NA
     */ // :3
    void setActive(bool bActive); // :3

    /** number of times that the grid will be reused
     *  @lua NA
     */ // :3
    inline int getReuseGrid(void) { return m_nReuseGrid; } // :3
    /**
     *  @lua NA
     */ // :3
    inline void setReuseGrid(int nReuseGrid) { m_nReuseGrid = nReuseGrid; } // :3

    /** size of the grid
     *  @lua NA
     */ // :3
    inline const CCSize& getGridSize(void) { return m_sGridSize; } // :3
    /**
     *  @lua NA
     */ // :3
    inline void setGridSize(const CCSize& gridSize) { m_sGridSize = gridSize; } // :3

    /** pixels between the grids
     *  @lua NA
     */ // :3
    inline const CCPoint& getStep(void) { return m_obStep; } // :3
    /**
     *  @lua NA
     */ // :3
    inline void setStep(const CCPoint& step) { m_obStep = step; } // :3

    /** is texture flipped
     * @lua NA
     */ // :3
    inline bool isTextureFlipped(void) { return m_bIsTextureFlipped; } // :3
    /**
     *  @lua NA
     */ // :3
    void setTextureFlipped(bool bFlipped); // :3
    /**
     *  @lua NA
     */ // :3
    bool initWithSize(const CCSize& gridSize, CCTexture2D *pTexture, bool bFlipped); // :3
    /**
     *  @lua NA
     */ // :3
    bool initWithSize(const CCSize& gridSize); // :3
    /**
     *  @lua NA
     */ // :3
    void beforeDraw(void); // :3
    /**
     *  @lua NA
     */ // :3
    void afterDraw(CCNode *pTarget); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void blit(void); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void reuse(void); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void calculateVertexPoints(void); // :3

public: // :3

    /** create one Grid
     * @lua NA
     */ // :3
    static CCGridBase* create(const CCSize& gridSize, CCTexture2D *texture, bool flipped); // :3
    /** create one Grid
     * @lua NA
     */ // :3
    static CCGridBase* create(const CCSize& gridSize); // :3
    /**
     *  @lua NA
     */ // :3
    void set2DProjection(void); // :3

public: // :3
    bool m_bActive; // :3
    int  m_nReuseGrid; // :3
    CCSize m_sGridSize; // :3
    CCTexture2D *m_pTexture; // :3
    CCPoint m_obStep; // :3
    CCGrabber *m_pGrabber; // :3
    bool m_bIsTextureFlipped; // :3
    CCGLProgram* m_pShaderProgram; // :3
    ccDirectorProjection m_directorProjection; // :3
}; // :3

/**
 CCGrid3D is a 3D grid implementation. Each vertex has 3 dimensions: x,y,z
 @js NA
 @lua NA
 */ // :3
class CC_DLL CCGrid3D : public CCGridBase // :3
#ifdef EMSCRIPTEN // :3
, public CCGLBufferedNode // :3
#endif // EMSCRIPTEN :3
{ // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCGrid3D, CCGridBase) // :3
    CCGrid3D(); // :3
    ~CCGrid3D(void); // :3

    /** returns the vertex at a given position */
    ccVertex3F vertex(const CCPoint& pos);
    /** returns the original (non-transformed) vertex at a given position */
    ccVertex3F originalVertex(const CCPoint& pos);
    /** sets a new vertex at a given position */
    void setVertex(const CCPoint& pos, const ccVertex3F& vertex);

    virtual void blit(void);
    virtual void reuse(void);
    virtual void calculateVertexPoints(void);

public:
    /** create one Grid */
    static CCGrid3D* create(const CCSize& gridSize, CCTexture2D *pTexture, bool bFlipped);
    /** create one Grid */
    static CCGrid3D* create(const CCSize& gridSize);

protected:
    GLvoid *m_pTexCoordinates;
    GLvoid *m_pVertices;
    GLvoid *m_pOriginalVertices;
    GLushort *m_pIndices;
};

/**
 CCTiledGrid3D is a 3D grid implementation. It differs from Grid3D in that
 the tiles can be separated from the grid.
 @js NA
 @lua NA
*/ // :3
class CC_DLL CCTiledGrid3D : public CCGridBase // :3
#ifdef EMSCRIPTEN // :3
, public CCGLBufferedNode // :3
#endif // EMSCRIPTEN :3
{ // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTiledGrid3D, CCGridBase) // :3
    CCTiledGrid3D(); // :3
    ~CCTiledGrid3D(void); // :3

    /** returns the tile at the given position */
    ccQuad3 tile(const CCPoint& pos);
    /** returns the original tile (untransformed) at the given position */
    ccQuad3 originalTile(const CCPoint& pos);
    /** sets a new tile */
    void setTile(const CCPoint& pos, const ccQuad3& coords);

    virtual void blit(void);
    virtual void reuse(void);
    virtual void calculateVertexPoints(void);

public:

    /** create one Grid */
    static CCTiledGrid3D* create(const CCSize& gridSize, CCTexture2D *pTexture, bool bFlipped);
    /** create one Grid */
    static CCTiledGrid3D* create(const CCSize& gridSize);

protected:
    GLvoid *m_pTexCoordinates;
    GLvoid *m_pVertices;
    GLvoid *m_pOriginalVertices;
    GLushort *m_pIndices;
};

// end of effects group
/// @}

NS_CC_END

#endif // __EFFECTS_CCGRID_H__
