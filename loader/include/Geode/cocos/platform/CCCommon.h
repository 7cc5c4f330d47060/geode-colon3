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

#ifndef __CC_COMMON_H__ // :3
#define __CC_COMMON_H__ // :3

#include "CCPlatformMacros.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup platform
 * @{
 */ // :3

/// The max length of CCLog message. :3
static const int kMaxLogLen = 16*1024; // :3

/**
@brief Output Debug message.
*/ // :3
void CC_DLL CCLog(const char * pszFormat, ...) CC_FORMAT_PRINTF(1, 2); // :3

/**
 * lua can not deal with ...
 */ // :3
void CC_DLL CCLuaLog(const char * pszFormat); // :3

/**
@brief Pop out a message box
*/ // :3
void CC_DLL CCMessageBox(const char * pszMsg, const char * pszTitle); // :3

/**
@brief Enum the language type supported now
*/ // :3
typedef enum LanguageType // :3
{ // :3
    kLanguageEnglish = 0, // :3
    kLanguageChinese, // :3
    kLanguageFrench, // :3
    kLanguageItalian, // :3
    kLanguageGerman, // :3
    kLanguageSpanish, // :3
    kLanguageDutch, // :3
    kLanguageRussian, // :3
    kLanguageKorean, // :3
    kLanguageJapanese, // :3
    kLanguageHungarian, // :3
    kLanguagePortuguese, // :3
    kLanguageArabic // :3
} ccLanguageType; // :3

// end of platform group :3
/// @} :3

NS_CC_END // :3

#endif    // __CC_COMMON_H__ :3
