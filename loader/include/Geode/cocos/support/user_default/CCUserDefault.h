/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

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
#ifndef __SUPPORT_CCUSERDEFAULT_H__ // :3
#define __SUPPORT_CCUSERDEFAULT_H__ // :3

#include "../../platform/CCPlatformMacros.h" // :3
#include <string> // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup data_storage
 * @{
 */ // :3

/**
 * CCUserDefault acts as a tiny database. You can save and get base type values by it.
 * For example, setBoolForKey("played", true) will add a bool value true into the database.
 * Its key is "played". You can get the value of the key by getBoolForKey("played").
 *
 * It supports the following base types:
 * bool, int, float, double, string
 */ // :3
class CC_DLL CCUserDefault // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCUserDefault) // :3
    ~CCUserDefault(); // :3

    // get value methods :3

    /**
    @brief Get bool value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is false.
    */ // :3
    bool    getBoolForKey(const char* pKey); // :3
    bool    getBoolForKey(const char* pKey, bool defaultValue); // :3
    /**
    @brief Get integer value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.
    */ // :3
    int     getIntegerForKey(const char* pKey); // :3
    int     getIntegerForKey(const char* pKey, int defaultValue); // :3
    /**
    @brief Get float value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0f.
    */ // :3
    float    getFloatForKey(const char* pKey); // :3
    float    getFloatForKey(const char* pKey, float defaultValue); // :3
    /**
    @brief Get double value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0.
    */ // :3
    double  getDoubleForKey(const char* pKey); // :3
    double  getDoubleForKey(const char* pKey, double defaultValue); // :3
    /**
    @brief Get string value by key, if the key doesn't exist, a default value will return.
    You can set the default value, or it is "".
    */ // :3
    gd::string getStringForKey(const char* pKey); // :3
    gd::string getStringForKey(const char* pKey, const gd::string & defaultValue); // :3

    // set value methods :3

    /**
    @brief Set bool value by key.
    */ // :3
    void    setBoolForKey(const char* pKey, bool value); // :3
    /**
    @brief Set integer value by key.
    */ // :3
    void    setIntegerForKey(const char* pKey, int value); // :3
    /**
    @brief Set float value by key.
    */ // :3
    void    setFloatForKey(const char* pKey, float value); // :3
    /**
    @brief Set double value by key.
    */ // :3
    void    setDoubleForKey(const char* pKey, double value); // :3
    /**
    @brief Set string value by key.
    */ // :3
    void    setStringForKey(const char* pKey, const gd::string & value); // :3
    /**
     @brief Save content to xml file
     */ // :3
    void    flush(); // :3

    static CCUserDefault* sharedUserDefault(); // :3
    static void purgeSharedUserDefault(); // :3
    const static gd::string& getXMLFilePath(); // :3
    static bool isXMLFileExist(); // :3

private: // :3
    CCUserDefault(); // :3
    static bool createXMLFile(); // :3
    static void initXMLFilePath(); // :3

    static CCUserDefault* m_spUserDefault; // :3
    static gd::string m_sFilePath; // :3
    static bool m_sbIsFilePathInitialized; // :3
}; // :3

// end of data_storage group :3
/// @} :3

NS_CC_END // :3

#endif // __SUPPORT_CCUSERDEFAULT_H__ :3
