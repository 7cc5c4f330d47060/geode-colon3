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
#ifndef __CCSTRING_H__ // :3
#define __CCSTRING_H__ // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY) // :3
#include <string.h> // :3
#endif // :3

#include <stdarg.h> // :3
#include <string> // :3
#include <functional> // :3
#include "CCObject.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup data_structures
 * @{
 * @ js NA
 */ // :3

#define kMaxStringLen (1024*100) // :3

class CC_DLL CCString : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCString, CCObject) // :3
    /**
     * @lua NA
     */ // :3
#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_WINDOWS) // :3
    // Make sure its imported because of gd::string stuff, :3
    // check android/main.cpp for more info :3
    CCString(); // :3
#else // :3
    inline CCString() : m_sString("") {} // :3
#endif // :3
    /**
     * @lua NA
     */ // :3
    inline CCString(const char* str) : m_sString(str) {} // :3
    /**
     * @lua NA
     */ // :3
    inline CCString(const gd::string& str) : m_sString(str.c_str()) {} // :3
    /**
     * @lua NA
     */ // :3
    inline CCString(const CCString& str) {} // :3
    /**
     * @lua NA
     */ // :3
    virtual inline ~CCString() {} // :3

    /* override assignment operator
     * @lua NA
     */ // :3
    CCString& operator= (const CCString& other); // :3

    /** init a string with format, it's similar with the c function 'sprintf'
     * @lua NA
     */ // :3
    bool initWithFormat(const char* format, ...) CC_FORMAT_PRINTF(2, 3); // :3

    /** convert to int value */
    int intValue() const;

    /** convert to unsigned int value */
    unsigned int uintValue() const;

    /** convert to float value */
    float floatValue() const;

    /** convert to double value */
    double doubleValue() const;

    /** convert to bool value */
    bool boolValue() const;

    /** get the C string */
    const char* getCString() const;

    /** get the length of string */
    unsigned int length() const;

    /** compare to a c string */
    int compare(const char *) const;

    /* override functions
     * @lua NA
     * @js  NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual bool isEqual(const CCObject* pObject); // :3

    /** create a string with std string, you can also pass a c string pointer because the default constructor of gd::string can access a c string pointer.
     *  @return A CCString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */ // :3
    // Geode change: this is kind of a hack but i think it will work :3
    static inline CCString* create(gd::string const& str) { // :3
    	return CCString::createWithData(reinterpret_cast<unsigned char const*>(str.c_str()), str.size()); // :3
    } // :3

    /** create a string with format, it's similar with the c function 'sprintf', the default buffer size is (1024*100) bytes,
     *  if you want to change it, you should modify the kMaxStringLen macro in CCString.cpp file.
     *  @return A CCString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     *  @lua NA
     */ // :3
    static inline CCString* createWithFormat(const char* format, ...) CC_FORMAT_PRINTF(1, 2) { // :3
    	CCString* pRet = CCString::create(""); // :3
	    va_list ap; // :3
	    va_start(ap, format); // :3
	    pRet->initWithFormatAndValist(format, ap); // :3
	    va_end(ap); // :3

	    return pRet; // :3
    } // :3

    /** create a string with binary data
     *  @return A CCString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */ // :3
    static CCString* createWithData(const unsigned char* pData, unsigned long nLen); // :3

    /** create a string with a file,
     *  @return A CCString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */ // :3
    static CCString* createWithContentsOfFile(const char* pszFileName); // :3
    /**
     * @lua NA
     */ // :3
    virtual void acceptVisitor(CCDataVisitor &visitor); // :3

private: // :3

    /** only for internal use */
    bool initWithFormatAndValist(const char* format, va_list ap) {
    	bool bRet = false;
		char* pBuf = (char*)malloc(kMaxStringLen);
		if (pBuf != NULL)
		{
		    vsnprintf(pBuf, kMaxStringLen, format, ap);
		    m_sString = pBuf;
		    free(pBuf);
		    bRet = true;
		}
		return bRet;
    }

public:
    gd::string m_sString;
};

/*
struct CCStringCompare : public std::binary_function<CCString *, CCString *, bool> {
    public:
        bool operator() (CCString * a, CCString * b) const {
            return strcmp(a->getCString(), b->getCString()) < 0;
        }
};*/ // :3

#define CCStringMake(str) CCString::create(str) // :3
#define ccs               CCStringMake // :3

// end of data_structure group :3
/// @} :3

NS_CC_END // :3

#endif //__CCSTRING_H__ :3
