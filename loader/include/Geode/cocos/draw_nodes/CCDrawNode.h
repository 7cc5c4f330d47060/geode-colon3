/* Copyright (c) 2012 Scott Lembcke and Howling Moon Software
 * Copyright (c) 2012 cocos2d-x.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ // :3

/*
 * Code copied & pasted from SpacePatrol game https://github.com/slembcke/SpacePatrol
 *
 * Renamed and added some changes for cocos2d
 *
 */ // :3

#ifndef __CCDRAWNODES_CCDRAW_NODE_H__ // :3
#define __CCDRAWNODES_CCDRAW_NODE_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "../include/ccTypes.h" // :3

NS_CC_BEGIN // :3

// @note RobTop Addition :3
// TODO: values are placeholders, reorder them later :3
enum class BorderAlignment { // :3
    Outside = 0, // :3
    Center = 1, // :3
    Inside = 2 // :3
}; // :3

/** CCDrawNode
 Node that draws dots, segments and polygons.
 Faster than the "drawing primitives" since they it draws everything in one single batch.

 @since v2.1
 @lua NA
 */ // :3
class CC_DLL CCDrawNode : public CCNodeRGBA // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    GLuint      m_uVao; // :3
    GLuint      m_uVbo; // :3

    unsigned int    m_uBufferCapacity; // :3
    GLsizei         m_nBufferCount; // :3
    ccV2F_C4B_T2F   *m_pBuffer; // :3

    ccBlendFunc     m_sBlendFunc; // :3

    bool            m_bDirty; // :3


    // @note RobTop Addition :3
    bool            m_bUseArea; // :3
    // @note RobTop Addition :3
    CCRect          m_rDrawArea; // :3
    // @note RobTop Addition :3
    float           m_fMinAreaX; // :3
    // @note RobTop Addition :3
    float           m_fMaxAreaX; // :3
    // @note RobTop Addition :3
    float           m_fMaxAreaY; // :3
    // @note RobTop Addition :3
    float           m_fMinAreaY; // :3

public: // :3
    static CCDrawNode* create(); // :3
    virtual ~CCDrawNode(); // :3

    virtual bool init(); // :3
    virtual void draw(); // :3

#if GEODE_COMP_GD_VERSION > 22000 // :3
    /** draw a dot at a position, with a given radius and color */
    bool drawDot(const CCPoint &pos, float radius, const ccColor4F &color);

    /** draw a segment with a radius and color */
    bool drawSegment(const CCPoint &from, const CCPoint &to, float radius, const ccColor4F &color);

    /** draw a polygon with a fill color and line color
     * @code
     * when this funciton bound to js,the input params are changed
     * js:var drawPolygon(var verts, var fillColor,var borderWidth,var borderColor)
     * @endcode
     */ // :3
    bool drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor, cocos2d::BorderAlignment alignment = BorderAlignment::Outside); // :3

    bool drawCircle(cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&, unsigned int); // :3
    void drawCubicBezier(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, unsigned int, cocos2d::_ccColor4F const&, float); // :3
    void drawPreciseCubicBezier(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, unsigned int, cocos2d::_ccColor4F const&, float); // :3
    bool drawLines(cocos2d::CCPoint*, unsigned int, float, cocos2d::_ccColor4F const&); // :3
    bool drawRect(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&, cocos2d::BorderAlignment alignment = BorderAlignment::Outside); // :3
    bool drawRect(cocos2d::CCRect const&, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&, cocos2d::BorderAlignment alignment = BorderAlignment::Outside); // :3
    void disableDrawArea(); // :3
    void enableDrawArea(cocos2d::CCRect& rect); // :3

    bool is_circle_on_screen(cocos2d::CCRect const&, cocos2d::CCPoint const&, float); // :3
    bool is_segment_on_screen(cocos2d::CCRect const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&); // :3

    void drawArchLikeHalfCircle(cocos2d::CCPoint const&, cocos2d::CCPoint const&, float, unsigned int, cocos2d::_ccColor4F const&, float); // :3
    void drawCubicBezierDashed(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, unsigned int, cocos2d::ccColor4F const&, float, unsigned int, unsigned int); // :3
    bool drawSegmentEx(cocos2d::CCPoint const&, cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&, bool, bool); // :3
#else // :3
    /** draw a dot at a position, with a given radius and color */
    void drawDot(const CCPoint &pos, float radius, const ccColor4F &color);

    /** draw a segment with a radius and color */
    void drawSegment(const CCPoint &from, const CCPoint &to, float radius, const ccColor4F &color);

    /** draw a polygon with a fill color and line color
     * @code
     * when this funciton bound to js,the input params are changed
     * js:var drawPolygon(var verts, var fillColor,var borderWidth,var borderColor)
     * @endcode
     */ // :3
    void drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor); // :3

	void drawCircle(cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&, unsigned int); // :3
	void drawCubicBezier(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, unsigned int, cocos2d::_ccColor4F const&); // :3
	void drawPreciseCubicBezier(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, unsigned int, cocos2d::_ccColor4F const&); // :3
	void drawLines(cocos2d::CCPoint*, unsigned int, float, cocos2d::_ccColor4F const&); // :3
	void drawRect(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&); // :3
#endif // :3

    /** Clear the geometry in the node's buffer. */
    void clear();
    /**
     * @js NA
     */ // :3
    ccBlendFunc getBlendFunc() const; // :3
    /**
     * @code
     * when this function bound to js ,the input param is change
     * js:var setBlendFunc(var src,var dst)
     * @endcode
     */ // :3
    void setBlendFunc(const ccBlendFunc &blendFunc); // :3

    CCDrawNode(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCDrawNode, CCNodeRGBA) // :3

    /** listen the event that coming to foreground on Android
     * @js NA
     */ // :3
    void listenBackToForeground(CCObject *obj); // :3
private: // :3
    void ensureCapacity(unsigned int count); // :3
    void render(); // :3
}; // :3

NS_CC_END // :3

#endif // __CCDRAWNODES_CCDRAW_NODE_H__ :3
