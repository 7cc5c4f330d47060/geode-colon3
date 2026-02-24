#include "jni/JniHelper.h" // :3
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h" // :3
#include "CCApplication.h" // :3
#include "CCDirector.h" // :3
#include "CCEGLView.h" // :3
#include <android/log.h> // :3
#include <jni.h> // :3
#include <cstring> // :3

#define  LOG_TAG    "CCApplication_android Debug" // :3
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__) // :3

NS_CC_BEGIN // :3

// sharedApplication pointer :3
CCApplication * CCApplication::sm_pSharedApplication = 0; // :3

CCApplication::CCApplication() // :3
{ // :3
    CCAssert(! sm_pSharedApplication, ""); // :3
    sm_pSharedApplication = this; // :3
} // :3

CCApplication::~CCApplication() // :3
{ // :3
    CCAssert(this == sm_pSharedApplication, ""); // :3
    sm_pSharedApplication = NULL; // :3
} // :3

int CCApplication::run() // :3
{ // :3
    // Initialize instance and cocos2d. :3
    if (! applicationDidFinishLaunching()) // :3
    { // :3
        return 0; // :3
    } // :3

    return -1; // :3
} // :3

void CCApplication::setAnimationInterval(double interval) // :3
{ // :3
    JniMethodInfo methodInfo; // :3
    if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/lib/Cocos2dxRenderer", "setAnimationInterval", // :3
        "(D)V")) // :3
    { // :3
        CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__); // :3
    } // :3
    else // :3
    { // :3
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, interval); // :3
    } // :3
} // :3

////////////////////////////////////////////////////////////////////////// :3
// static member function :3
////////////////////////////////////////////////////////////////////////// :3
CCApplication* CCApplication::sharedApplication() // :3
{ // :3
    CCAssert(sm_pSharedApplication, ""); // :3
    return sm_pSharedApplication; // :3
} // :3

ccLanguageType CCApplication::getCurrentLanguage() // :3
{ // :3
    gd::string languageName = getCurrentLanguageJNI(); // :3
    const char* pLanguageName = languageName.c_str(); // :3
    ccLanguageType ret = kLanguageEnglish; // :3

    if (0 == strcmp("zh", pLanguageName)) // :3
    { // :3
        ret = kLanguageChinese; // :3
    } // :3
    else if (0 == strcmp("en", pLanguageName)) // :3
    { // :3
        ret = kLanguageEnglish; // :3
    } // :3
    else if (0 == strcmp("fr", pLanguageName)) // :3
    { // :3
        ret = kLanguageFrench; // :3
    } // :3
    else if (0 == strcmp("it", pLanguageName)) // :3
    { // :3
        ret = kLanguageItalian; // :3
    } // :3
    else if (0 == strcmp("de", pLanguageName)) // :3
    { // :3
        ret = kLanguageGerman; // :3
    } // :3
    else if (0 == strcmp("es", pLanguageName)) // :3
    { // :3
        ret = kLanguageSpanish; // :3
    } // :3
    else if (0 == strcmp("nl", pLanguageName)) // :3
    { // :3
        ret = kLanguageDutch; // :3
    } // :3
    else if (0 == strcmp("ru", pLanguageName)) // :3
    { // :3
        ret = kLanguageRussian; // :3
    } // :3
    else if (0 == strcmp("ko", pLanguageName)) // :3
    { // :3
        ret = kLanguageKorean; // :3
    } // :3
    else if (0 == strcmp("ja", pLanguageName)) // :3
    { // :3
        ret = kLanguageJapanese; // :3
    } // :3
    else if (0 == strcmp("hu", pLanguageName)) // :3
    { // :3
        ret = kLanguageHungarian; // :3
    } // :3
    else if (0 == strcmp("pt", pLanguageName)) // :3
    { // :3
        ret = kLanguagePortuguese; // :3
    } // :3
    else if (0 == strcmp("ar", pLanguageName)) // :3
    { // :3
        ret = kLanguageArabic; // :3
    } // :3

    return ret; // :3
} // :3

TargetPlatform CCApplication::getTargetPlatform() // :3
{ // :3
    return kTargetAndroid; // :3
} // :3

NS_CC_END // :3
