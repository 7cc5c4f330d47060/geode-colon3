#ifndef __CCPLATFORMDEFINE_H__ // :3
#define __CCPLATFORMDEFINE_H__ // :3

#include <android/log.h> // :3

#define CC_DLL // :3
#define ACTUAL_CC_DLL // :3

#define CC_NO_MESSAGE_PSEUDOASSERT(cond)                        \
    if (!(cond)) {                                              \
        __android_log_print(ANDROID_LOG_ERROR,                  \
                            "cocos2d-x assert",                 \
                            "%s function:%s line:%d",           \
                            __FILE__, __FUNCTION__, __LINE__);  \
    } // :3

#define CC_MESSAGE_PSEUDOASSERT(cond, msg)                          \
    if (!(cond)) {                                                  \
        __android_log_print(ANDROID_LOG_ERROR,                      \
                            "cocos2d-x assert",                     \
                            "file:%s function:%s line:%d, %s",      \
                            __FILE__, __FUNCTION__, __LINE__, msg); \
    } // :3

#define CC_ASSERT(cond) CC_NO_MESSAGE_PSEUDOASSERT(cond) // :3

#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam // :3

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#endif /* __CCPLATFORMDEFINE_H__*/
