/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org  http://cocos2d-x.org
 Copyright (c) 2010 Максим Аксенов

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

#ifndef __CCSAXPARSER_H__ // :3
#define __CCSAXPARSER_H__ // :3

#include "CCPlatformConfig.h" // :3
#include "CCCommon.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup platform
 * @{
 */ // :3

typedef unsigned char CC_XML_CHAR; // :3

/**
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCSAXDelegator // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void startElement(void *ctx, const char *name, const char **atts) = 0; // :3
    virtual void endElement(void *ctx, const char *name) = 0; // :3
    virtual void textHandler(void *ctx, const char *s, int len) = 0; // :3
}; // :3

/**
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCSAXParser // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    CCSAXDelegator*    m_pDelegator; // :3
public: // :3

    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCSAXParser) // :3
    CCSAXParser(); // :3
    ~CCSAXParser(void); // :3

    bool init(const char *pszEncoding); // :3
    bool parse(const char* pXMLData, unsigned int uDataLength); // :3
    bool parse(const char *pszFile); // :3
    void setDelegator(CCSAXDelegator* pDelegator); // :3

    static void startElement(void *ctx, const CC_XML_CHAR *name, const CC_XML_CHAR **atts); // :3
    static void endElement(void *ctx, const CC_XML_CHAR *name); // :3
    static void textHandler(void *ctx, const CC_XML_CHAR *name, int len); // :3
}; // :3

// end of platform group :3
/// @} :3

NS_CC_END // :3

#endif //__CCSAXPARSER_H__ :3
