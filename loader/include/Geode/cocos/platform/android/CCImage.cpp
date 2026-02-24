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

//#define COCOS2D_DEBUG 1 :3

#define __CC_PLATFORM_IMAGE_CPP__ // :3
#include "platform/CCImageCommon_cpp.h" // :3
#include "../platform/CCPlatformMacros.h" // :3
#include "../platform/CCImage.h" // :3
#include "../platform/CCFileUtils.h" // :3
#include "jni/JniHelper.h" // :3

#include <android/log.h> // :3
#include <string.h> // :3
#include <jni.h> // :3

// prototype :3
void swapAlphaChannel(unsigned int *pImageMemory, unsigned int numPixels); // :3

NS_CC_BEGIN // :3

class BitmapDC // :3
{ // :3
public: // :3

    BitmapDC() // :3
    : m_pData(NULL) // :3
    , m_nWidth(0) // :3
    , m_nHeight(0) // :3
    { // :3
    } // :3

    ~BitmapDC(void) // :3
    { // :3
        if (m_pData) // :3
        { // :3
            delete [] m_pData; // :3
        } // :3
    } // :3

    bool getBitmapFromJavaShadowStroke(	const char *text, // :3
    									int nWidth, // :3
    									int nHeight, // :3
    									CCImage::ETextAlign eAlignMask, // :3
    									const char * pFontName, // :3
    									float fontSize, // :3
    									float textTintR 		= 1.0, // :3
    									float textTintG 		= 1.0, // :3
    									float textTintB 		= 1.0, // :3
    									bool shadow 			= false, // :3
    									float shadowDeltaX 		= 0.0, // :3
    									float shadowDeltaY 		= 0.0, // :3
    									float shadowBlur 		= 0.0, // :3
    									float shadowIntensity 	= 0.0, // :3
    									bool stroke 			= false, // :3
    									float strokeColorR 		= 0.0, // :3
    									float strokeColorG 		= 0.0, // :3
    									float strokeColorB 		= 0.0, // :3
    									float strokeSize 		= 0.0 ) // :3
    { // :3
           JniMethodInfo methodInfo; // :3
           if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/lib/Cocos2dxBitmap", "createTextBitmapShadowStroke", // :3
               "(Ljava/lang/String;Ljava/lang/String;IFFFIIIZFFFZFFFF)V")) // :3
           { // :3
               CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__); // :3
               return false; // :3
           } // :3



           // Do a full lookup for the font path using CCFileUtils in case the given font name is a relative path to a font file asset, :3
           // or the path has been mapped to a different location in the app package: :3
           gd::string fullPathOrFontName = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFontName); // :3

		   // If the path name returned includes the 'assets' dir then that needs to be removed, because the android.content.Context :3
		   // requires this portion of the path to be omitted for assets inside the app package. :3
		   if (fullPathOrFontName.find("assets/") == 0) // :3
		   { // :3
               fullPathOrFontName = fullPathOrFontName.substr(strlen("assets/"));	// Chop out the 'assets/' portion of the path. :3
           } // :3

           /**create bitmap
            * this method call Cococs2dx.createBitmap()(java code) to create the bitmap, the java code
            * will call Java_org_cocos2dx_lib_Cocos2dxBitmap_nativeInitBitmapDC() to init the width, height
            * and data.
            * use this approach to decrease the jni call number
           */ // :3
           jstring jstrText = methodInfo.env->NewStringUTF(text); // :3
           jstring jstrFont = methodInfo.env->NewStringUTF(fullPathOrFontName.c_str()); // :3

           methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrText, // :3
               jstrFont, (int)fontSize, textTintR, textTintG, textTintB, eAlignMask, nWidth, nHeight, shadow, shadowDeltaX, -shadowDeltaY, shadowBlur, stroke, strokeColorR, strokeColorG, strokeColorB, strokeSize); // :3

           methodInfo.env->DeleteLocalRef(jstrText); // :3
           methodInfo.env->DeleteLocalRef(jstrFont); // :3
           methodInfo.env->DeleteLocalRef(methodInfo.classID); // :3

           return true; // :3
    } // :3


    bool getBitmapFromJava(const char *text, int nWidth, int nHeight, CCImage::ETextAlign eAlignMask, const char * pFontName, float fontSize) // :3
    { // :3
    	return  getBitmapFromJavaShadowStroke(	text, nWidth, nHeight, eAlignMask, pFontName, fontSize ); // :3
    } // :3

    // ARGB -> RGBA :3
    inline unsigned int swapAlpha(unsigned int value) // :3
    { // :3
        return ((value << 8 & 0xffffff00) | (value >> 24 & 0x000000ff)); // :3
    } // :3

public: // :3
    int m_nWidth; // :3
    int m_nHeight; // :3
    unsigned char *m_pData; // :3
    JNIEnv *env; // :3
}; // :3

static BitmapDC& sharedBitmapDC() // :3
{ // :3
    static BitmapDC s_BmpDC; // :3
    return s_BmpDC; // :3
} // :3

bool CCImage::initWithString( // :3
                               const char *    pText, // :3
                               int             nWidth/* = 0*/,
                               int             nHeight/* = 0*/,
                               ETextAlign      eAlignMask/* = kAlignCenter*/,
                               const char *    pFontName/* = nil*/,
                               int             nSize/* = 0*/)
{
    bool bRet = false;

    do
    {
        CC_BREAK_IF(! pText);

        BitmapDC &dc = sharedBitmapDC();

        CC_BREAK_IF(! dc.getBitmapFromJava(pText, nWidth, nHeight, eAlignMask, pFontName, nSize));

        // assign the dc.m_pData to m_pData in order to save time
        m_pData = dc.m_pData;
        CC_BREAK_IF(! m_pData);

        m_nWidth    = (short)dc.m_nWidth;
        m_nHeight   = (short)dc.m_nHeight;
        m_bHasAlpha = true;
        m_bPreMulti = true;
        m_nBitsPerComponent = 8;

        bRet = true;
    } while (0);

    return bRet;
}

bool CCImage::initWithStringShadowStroke(
                                         const char * pText,
                                         int         nWidth ,
                                         int         nHeight ,
                                         ETextAlign eAlignMask ,
                                         const char * pFontName ,
                                         int          nSize ,
                                         float        textTintR,
                                         float        textTintG,
                                         float        textTintB,
                                         bool shadow,
                                         float shadowOffsetX,
                                         float shadowOffsetY,
                                         float shadowOpacity,
                                         float shadowBlur,
                                         bool  stroke,
                                         float strokeR,
                                         float strokeG,
                                         float strokeB,
                                         float strokeSize)
{
	 bool bRet = false;
	    do
	    {
	        CC_BREAK_IF(! pText);

	        BitmapDC &dc = sharedBitmapDC();


	        CC_BREAK_IF(! dc.getBitmapFromJavaShadowStroke(pText, nWidth, nHeight, eAlignMask, pFontName,
	        											   nSize, textTintR, textTintG, textTintB, shadow,
	        											   shadowOffsetX, shadowOffsetY, shadowBlur, shadowOpacity,
	        											   stroke, strokeR, strokeG, strokeB, strokeSize ));


	        // assign the dc.m_pData to m_pData in order to save time
	        m_pData = dc.m_pData;

	        CC_BREAK_IF(! m_pData);

	        m_nWidth    = (short)dc.m_nWidth;
	        m_nHeight   = (short)dc.m_nHeight;
	        m_bHasAlpha = true;
	        m_bPreMulti = true;
	        m_nBitsPerComponent = 8;

	        // swap the alpha channel (ARGB to RGBA)
	        swapAlphaChannel((unsigned int *)m_pData, (m_nWidth * m_nHeight) );

	        // ok
	        bRet = true;

	    } while (0);

	    return bRet;
}

NS_CC_END

// swap the alpha channel in an 32 bit image (from ARGB to RGBA)
void swapAlphaChannel(unsigned int *pImageMemory, unsigned int numPixels)
{
	for(int c = 0; c < numPixels; ++c, ++pImageMemory)
	{
		// copy the current pixel
		unsigned int currenPixel =  (*pImageMemory);
		// swap channels and store back
		char *pSource = (char *) 	&currenPixel;
		*pImageMemory = (pSource[0] << 24) | (pSource[3]<<16) | (pSource[2]<<8) | pSource[1];
	}
}

// this method is called by Cocos2dxBitmap
extern "C"
{
    /**
    * this method is called by java code to init width, height and pixels data
    */ // :3
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxBitmap_nativeInitBitmapDC(JNIEnv*  env, jobject thiz, int width, int height, jbyteArray pixels) // :3
    { // :3
        int size = width * height * 4; // :3
        cocos2d::BitmapDC& bitmapDC = cocos2d::sharedBitmapDC(); // :3
        bitmapDC.m_nWidth = width; // :3
        bitmapDC.m_nHeight = height; // :3
        bitmapDC.m_pData = new unsigned char[size]; // :3
        env->GetByteArrayRegion(pixels, 0, size, (jbyte*)bitmapDC.m_pData); // :3

        // swap data :3
        unsigned int *tempPtr = (unsigned int*)bitmapDC.m_pData; // :3
        unsigned int tempdata = 0; // :3
        for (int i = 0; i < height; ++i) // :3
        { // :3
            for (int j = 0; j < width; ++j) // :3
            { // :3
                tempdata = *tempPtr; // :3
                *tempPtr++ = bitmapDC.swapAlpha(tempdata); // :3
            } // :3
        } // :3
    } // :3
}; // :3
