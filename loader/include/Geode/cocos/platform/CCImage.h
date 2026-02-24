/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#ifndef __CC_IMAGE_H__ // :3
#define __CC_IMAGE_H__ // :3

#include "../cocoa/CCObject.h" // :3

NS_CC_BEGIN // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) // :3
class CCFreeTypeFont; // :3
#endif // :3

/**
 * @addtogroup platform
 * @{
 */ // :3

class CC_DLL CCImage : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     @js ctor
     */ // :3
    CCImage(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCImage, CCObject) // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    ~CCImage(); // :3

    typedef enum // :3
    { // :3
        kFmtJpg = 0, // :3
        kFmtPng, // :3
        kFmtTiff, // :3
        kFmtWebp, // :3
        kFmtRawData, // :3
        kFmtUnKnown // :3
    }EImageFormat; // :3

    typedef enum // :3
    { // :3
        kAlignCenter        = 0x33, ///< Horizontal center and vertical center. :3
        kAlignTop           = 0x13, ///< Horizontal center and vertical top. :3
        kAlignTopRight      = 0x12, ///< Horizontal right and vertical top. :3
        kAlignRight         = 0x32, ///< Horizontal right and vertical center. :3
        kAlignBottomRight   = 0x22, ///< Horizontal right and vertical bottom. :3
        kAlignBottom        = 0x23, ///< Horizontal center and vertical bottom. :3
        kAlignBottomLeft    = 0x21, ///< Horizontal left and vertical bottom. :3
        kAlignLeft          = 0x31, ///< Horizontal left and vertical center. :3
        kAlignTopLeft       = 0x11, ///< Horizontal left and vertical top. :3
    }ETextAlign; // :3

    /**
    @brief  Load the image from the specified path.
    @param strPath   the absolute file path.
    @param imageType the type of image, currently only supporting two types.
    @return  true if loaded correctly.
    */ // :3
    bool initWithImageFile(const char * strPath, EImageFormat imageType = kFmtPng); // :3

    /*
     @brief The same result as with initWithImageFile, but thread safe. It is caused by
            loadImage() in CCTextureCache.cpp.
     @param fullpath  full path of the file.
     @param imageType the type of image, currently only supporting two types.
     @return  true if loaded correctly.
     */ // :3
    bool initWithImageFileThreadSafe(const char *fullpath, EImageFormat imageType = kFmtPng); // :3

    /**
    @brief  Load image from stream buffer.

    @warning kFmtRawData only supports RGBA8888.
    @param pBuffer  stream buffer which holds the image data.
    @param nLength  data length expressed in (number of) bytes.
    @param nWidth, nHeight, nBitsPerComponent are used for kFmtRawData.
    @return true if loaded correctly.
    @js NA
    */ // :3
    bool initWithImageData(void * pData, // :3
                           int nDataLen, // :3
                           EImageFormat eFmt = kFmtUnKnown, // :3
                           int nWidth = 0, // :3
                           int nHeight = 0, // :3
                           int nBitsPerComponent = 8, // :3
                           int whoknows = 0); // :3

    /**
    @brief    Create image with specified string.
    @param  pText       the text the image will show (cannot be nil).
    @param  nWidth      the image width, if 0, the width will match the text's width.
    @param  nHeight     the image height, if 0, the height will match the text's height.
    @param  eAlignMask  the test Alignment
    @param  pFontName   the name of the font used to draw the text. If nil, use the default system font.
    @param  nSize       the font size, if 0, use the system default size.
    @js NA
    */ // :3
    bool initWithString( // :3
        const char *    pText, // :3
        int             nWidth = 0, // :3
        int             nHeight = 0, // :3
        ETextAlign      eAlignMask = kAlignCenter, // :3
        const char *    pFontName = 0, // :3
        int             nSize = 0); // :3

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // :3
        /**
         * @js NA
         * @lua NA
         */ // :3
        bool initWithStringShadowStroke( // :3
                                            const char *    pText, // :3
                                            int             nWidth      = 0, // :3
                                            int             nHeight     = 0, // :3
                                            ETextAlign      eAlignMask  = kAlignCenter, // :3
                                            const char *    pFontName   = 0, // :3
                                            int             nSize       = 0, // :3
                                            float           textTintR   = 1, // :3
                                            float           textTintG   = 1, // :3
                                            float           textTintB   = 1, // :3
                                            bool shadow                 = false, // :3
                                            float shadowOffsetX         = 0.0, // :3
                                            float shadowOffsetY         = 0.0, // :3
                                            float shadowOpacity         = 0.0, // :3
                                            float shadowBlur            = 0.0, // :3
                                            bool  stroke                =  false, // :3
                                            float strokeR               = 1, // :3
                                            float strokeG               = 1, // :3
                                            float strokeB               = 1, // :3
                                            float strokeSize            = 1 // :3

                                        ); // :3

    #endif // :3


    unsigned char *   getData()               { return m_pData; } // :3
    int               getDataLen()            { return m_nWidth * m_nHeight; } // :3


    bool hasAlpha()                     { return m_bHasAlpha;   } // :3
    bool isPremultipliedAlpha()         { return m_bPreMulti;   } // :3


    /**
    @brief    Save CCImage data to the specified file, with specified format.
    @param    pszFilePath        the file's absolute path, including file suffix.
    @param    bIsToRGB        whether the image is saved as RGB format.
    */ // :3
    bool saveToFile(const char *pszFilePath, bool bIsToRGB = true); // :3

    CC_SYNTHESIZE_READONLY_NV(unsigned short,   m_nWidth,       Width); // :3
    CC_SYNTHESIZE_READONLY_NV(unsigned short,   m_nHeight,      Height); // :3
    CC_SYNTHESIZE_READONLY_NV(int,     m_nBitsPerComponent,   BitsPerComponent); // :3

protected: // :3
    bool _initWithJpgData(void *pData, int nDatalen); // :3
    bool _initWithPngData(void *pData, int nDatalen); // :3
    bool _initWithTiffData(void *pData, int nDataLen); // :3
    bool _initWithWebpData(void *pData, int nDataLen); // :3
    // @warning kFmtRawData only support RGBA8888 :3
    bool _initWithRawData(void *pData, int nDatalen, int nWidth, int nHeight, int nBitsPerComponent, bool bPreMulti); // :3

    bool _saveImageToPNG(const char *pszFilePath, bool bIsToRGB = true); // :3
    bool _saveImageToJPG(const char *pszFilePath); // :3
public: // :3
    unsigned char *m_pData; // :3
    bool m_bHasAlpha; // :3
    bool m_bPreMulti; // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) // :3
    CCFreeTypeFont* m_ft; // :3
#endif // :3

private: // :3
    // noncopyable :3
    // CCImage(const CCImage&    rImg); :3
    // CCImage & operator=(const CCImage&); :3


}; // :3

// end of platform group :3
/// @} :3

NS_CC_END // :3

#endif    // __CC_IMAGE_H__ :3
