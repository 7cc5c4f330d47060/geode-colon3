/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009-2010 Ricardo Quesada
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


#ifndef __CC_TM_XML_PARSER__ // :3
#define __CC_TM_XML_PARSER__ // :3

#include "../cocoa/CCArray.h" // :3
#include "../cocoa/CCDictionary.h" // :3
#include "../cocoa/CCGeometry.h" // :3
#include "../platform/CCSAXParser.h" // :3

#include <string> // :3

NS_CC_BEGIN // :3

class CCTMXObjectGroup; // :3

/** @file
* Internal TMX parser
*
* IMPORTANT: These classed should not be documented using doxygen strings
* since the user should not use them.
*
*/ // :3

/**
 * @addtogroup tilemap_parallax_nodes
 * @{
 */ // :3

enum { // :3
    TMXLayerAttribNone = 1 << 0, // :3
    TMXLayerAttribBase64 = 1 << 1, // :3
    TMXLayerAttribGzip = 1 << 2, // :3
    TMXLayerAttribZlib = 1 << 3, // :3
}; // :3

enum { // :3
    TMXPropertyNone, // :3
    TMXPropertyMap, // :3
    TMXPropertyLayer, // :3
    TMXPropertyObjectGroup, // :3
    TMXPropertyObject, // :3
    TMXPropertyTile // :3
}; // :3

typedef enum ccTMXTileFlags_ { // :3
    kCCTMXTileHorizontalFlag        = 0x80000000, // :3
    kCCTMXTileVerticalFlag            = 0x40000000, // :3
    kCCTMXTileDiagonalFlag            = 0x20000000, // :3
    kCCFlipedAll                    = (kCCTMXTileHorizontalFlag|kCCTMXTileVerticalFlag|kCCTMXTileDiagonalFlag), // :3
    kCCFlippedMask                    = ~(kCCFlipedAll) // :3
} ccTMXTileFlags; // :3

// Bits on the far end of the 32-bit global tile ID (GID's) are used for tile flags :3

/** @brief CCTMXLayerInfo contains the information about the layers like:
- Layer name
- Layer size
- Layer opacity at creation time (it can be modified at runtime)
- Whether the layer is visible (if it's not visible, then the CocosNode won't be created)

This information is obtained from the TMX file.
*/ // :3
class CC_DLL CCTMXLayerInfo : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    CC_PROPERTY(CCDictionary*, m_pProperties, Properties); // :3
public: // :3
    gd::string         m_sName; // :3
    CCSize              m_tLayerSize; // :3
    unsigned int        *m_pTiles; // :3
    bool                m_bVisible; // :3
    unsigned char       m_cOpacity; // :3
    bool                m_bOwnTiles; // :3
    unsigned int        m_uMinGID; // :3
    unsigned int        m_uMaxGID; // :3
    CCPoint             m_tOffset; // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTMXLayerInfo, CCObject) // :3
    CCTMXLayerInfo(); // :3
    virtual ~CCTMXLayerInfo(); // :3
}; // :3

/** @brief CCTMXTilesetInfo contains the information about the tilesets like:
- Tileset name
- Tileset spacing
- Tileset margin
- size of the tiles
- Image used for the tiles
- Image size

This information is obtained from the TMX file.
*/ // :3
class CC_DLL CCTMXTilesetInfo : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    gd::string     m_sName; // :3
    unsigned int    m_uFirstGid; // :3
    CCSize          m_tTileSize; // :3
    unsigned int    m_uSpacing; // :3
    unsigned int    m_uMargin; // :3
    //! filename containing the tiles (should be spritesheet / texture atlas) :3
    gd::string     m_sSourceImage; // :3
    //! size in pixels of the image :3
    CCSize          m_tImageSize; // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTMXTilesetInfo, CCObject) // :3
    CCTMXTilesetInfo(); // :3
    virtual ~CCTMXTilesetInfo(); // :3
    CCRect rectForGID(unsigned int gid); // :3
}; // :3

/** @brief CCTMXMapInfo contains the information about the map like:
- Map orientation (hexagonal, isometric or orthogonal)
- Tile size
- Map size

And it also contains:
- Layers (an array of TMXLayerInfo objects)
- Tilesets (an array of TMXTilesetInfo objects)
- ObjectGroups (an array of TMXObjectGroupInfo objects)

This information is obtained from the TMX file.

*/ // :3
class CC_DLL CCTMXMapInfo : public CCObject, public CCSAXDelegator // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /// map orientation :3
    CC_SYNTHESIZE_NV(int,    m_nOrientation, Orientation); // :3
    /// map width & height :3
    CC_SYNTHESIZE_NV_PASS_BY_REF(CCSize, m_tMapSize, MapSize); // :3
    /// tiles width & height :3
    CC_SYNTHESIZE_NV_PASS_BY_REF(CCSize, m_tTileSize, TileSize); // :3
    /// Layers :3
    CC_PROPERTY(CCArray*, m_pLayers, Layers); // :3
    /// tilesets :3
    CC_PROPERTY(CCArray*, m_pTilesets, Tilesets); // :3
    /// ObjectGroups :3
    CC_PROPERTY(CCArray*, m_pObjectGroups, ObjectGroups); // :3
    /// parent element :3
    CC_SYNTHESIZE_NV(int, m_nParentElement, ParentElement); // :3
    /// parent GID :3
    CC_SYNTHESIZE_NV(unsigned int, m_uParentGID, ParentGID); // :3
    /// layer attribs :3
    CC_SYNTHESIZE_NV(int, m_nLayerAttribs, LayerAttribs); // :3
    /// is storing characters? :3
    CC_SYNTHESIZE_NV(bool, m_bStoringCharacters, StoringCharacters); // :3
    /// properties :3
    CC_PROPERTY(CCDictionary*, m_pProperties, Properties); // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTMXMapInfo, CCObject) // :3
    /**
     * @js  ctor
     * @lua NA
     */ // :3
    CCTMXMapInfo(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCTMXMapInfo(); // :3
    /** creates a TMX Format with a tmx file */
    static CCTMXMapInfo * formatWithTMXFile(const char *tmxFile);
    /** creates a TMX Format with an XML string and a TMX resource path */
    static CCTMXMapInfo * formatWithXML(const char* tmxString, const char* resourcePath);
    /** initializes a TMX format with a  tmx file
     * @lua NA
     */ // :3
    bool initWithTMXFile(const char *tmxFile); // :3
    /** initializes a TMX format with an XML string and a TMX resource path
     * @lua NA
     */ // :3
    bool initWithXML(const char* tmxString, const char* resourcePath); // :3
    /** initializes parsing of an XML file, either a tmx (Map) file or tsx (Tileset) file */
    bool parseXMLFile(const char *xmlFilename);
    /* initializes parsing of an XML string, either a tmx (Map) string or tsx (Tileset) string */
    bool parseXMLString(const char *xmlString);

    CCDictionary* getTileProperties();
    void setTileProperties(CCDictionary* tileProperties);

    /** implement pure virtual methods of CCSAXDelegator
     *  @js NA
     */ // :3
    void startElement(void *ctx, const char *name, const char **atts); // :3
    /**
     *  @js NA
     */ // :3
    void endElement(void *ctx, const char *name); // :3
    /**
     * @js NA
     */ // :3
    void textHandler(void *ctx, const char *ch, int len); // :3

    inline const char* getCurrentString(){ return m_sCurrentString.c_str(); } // :3
    inline void setCurrentString(const char *currentString){ m_sCurrentString = currentString; } // :3
    inline const char* getTMXFileName(){ return m_sTMXFileName.c_str(); } // :3
    inline void setTMXFileName(const char *fileName){ m_sTMXFileName = fileName; } // :3
private: // :3
    void internalInit(const char* tmxFileName, const char* resourcePath); // :3
public: // :3
    //! tmx filename :3
    gd::string m_sTMXFileName; // :3
    // tmx resource path :3
    gd::string m_sResources; // :3
    //! current string :3
    gd::string m_sCurrentString; // :3
    //! tile properties :3
    CCDictionary* m_pTileProperties; // :3
    unsigned int m_uCurrentFirstGID; // :3
}; // :3

// end of tilemap_parallax_nodes group :3
/// @} :3

NS_CC_END // :3

#endif // :3

