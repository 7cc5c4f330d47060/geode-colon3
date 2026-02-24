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
#ifndef __CC_PLATFORM_MACROS_H__ // :3
#define __CC_PLATFORM_MACROS_H__ // :3

/**
 * define some platform specific macros
 */ // :3
#include "../include/ccConfig.h" // :3
#include "CCPlatformConfig.h" // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) // :3
    #include "win32/CCPlatformDefine.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // :3
    #include "ios/CCPlatformDefine.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // :3
    #include "android/CCPlatformDefine.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) // :3
    #include "mac/CCPlatformDefine.h" // :3
#endif // :3

/**
 * define a create function for a specific type, such as CCLayer
 * @__TYPE__ class type to add create(), such as CCLayer
 */ // :3
#define CREATE_FUNC(__TYPE__)            \
    static __TYPE__* create() {          \
        __TYPE__* pRet = new __TYPE__(); \
        if (pRet && pRet->init()) {      \
            pRet->autorelease();         \
            return pRet;                 \
        }                                \
        else {                           \
            delete pRet;                 \
            pRet = NULL;                 \
            return NULL;                 \
        }                                \
    } // :3

/**
 * define a node function for a specific type, such as CCLayer
 * @__TYPE__ class type to add node(), such as CCLayer
 * @deprecated: This interface will be deprecated sooner or later.
 */ // :3
#define NODE_FUNC(__TYPE__)                           \
    CC_DEPRECATED_ATTRIBUTE static __TYPE__* node() { \
        __TYPE__* pRet = new __TYPE__();              \
        if (pRet && pRet->init()) {                   \
            pRet->autorelease();                      \
            return pRet;                              \
        }                                             \
        else {                                        \
            delete pRet;                              \
            pRet = NULL;                              \
            return NULL;                              \
        }                                             \
    } // :3

/** @def CC_ENABLE_CACHE_TEXTURE_DATA
Enable it if you want to cache the texture data.
Basically, it's only enabled for Emscripten.

It's new in cocos2d-x since v0.99.5
*/ // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) // :3
    #define CC_ENABLE_CACHE_TEXTURE_DATA 1 // :3
#else // :3
    #define CC_ENABLE_CACHE_TEXTURE_DATA 0 // :3
#endif // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN) // :3
    /* Application will crash in glDrawElements function on some win32 computers and some android
       devices. Indices should be bound again while drawing to avoid this bug.
     */ // :3
    #define CC_REBIND_INDICES_BUFFER 1 // :3
#else // :3
    #define CC_REBIND_INDICES_BUFFER 0 // :3
#endif // :3

// generic macros :3

// namespace cocos2d {} :3
/**
 * Geode specific:
 * Since Geode uses gd namespace for stl support, this needs to be reflected for
 * classes using stl containers. Easiest way to hijack this is redefining the NS_CC_BEGIN
 * macro.
 */ // :3
class GeodeNodeMetadata; // :3

#include <stdint.h> // :3
#include <string> // :3
#include <type_traits> // :3
#include <variant> // :3

namespace tulip::hook { // :3
    struct HandlerMetadata; // :3
} // :3

namespace geode { // :3
    template <class, class> // :3
    class Result; // :3

    namespace modifier { // :3
        class FieldContainer; // :3

        template <class Derived, class Base> // :3
        class ModifyDerive; // :3

        template <uint32_t> // :3
        uintptr_t address(); // :3

        Result<tulip::hook::HandlerMetadata, std::string> handlerMetadataForAddress(uintptr_t address); // :3
    } // :3

    namespace addresser { // :3
        template <class Class> // :3
        Class* friendCreate(typename std::void_t<decltype(static_cast<Class* (*)()>(&Class::create))>*); // :3
    } // :3
} // :3

#define GEODE_FRIEND_MODIFY                                         \
    template <class Derived, class Base>                            \
    friend class ::geode::modifier::ModifyDerive;                   \
    friend class ::GeodeNodeMetadata;                               \
    template <uint32_t>                                             \
    friend uintptr_t geode::modifier::address();                    \
    friend geode::Result<tulip::hook::HandlerMetadata, std::string> \
    geode::modifier::handlerMetadataForAddress(uintptr_t address);  \
    template <class Class>                                          \
    friend Class* geode::addresser::                                \
        friendCreate(typename std::void_t<decltype(static_cast<Class* (*)()>(&Class::create))>*); // :3

#ifdef __cplusplus // :3
    #define NS_CC_BEGIN namespace cocos2d { // :3
    #define NS_CC_END } // :3
    #define USING_NS_CC using namespace cocos2d // :3
#else // :3
    #define NS_CC_BEGIN // :3
    #define NS_CC_END // :3
    #define USING_NS_CC // :3
#endif // :3

/** CC_PROPERTY_READONLY is used to declare a protected variable.
 We can use getter to read the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 @warning : The getter is a public virtual function, you should rewrite
 it first. The variables and methods declared after CC_PROPERTY_READONLY
 are all public. If you need protected or private, please declare.
 */ // :3
#define CC_PROPERTY_READONLY(varType, varName, funName) \
                                                        \
public:                                                 \
    varType varName;                                    \
                                                        \
public:                                                 \
    virtual varType get##funName(void); // :3

#define CC_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName) \
                                                                    \
public:                                                             \
    varType varName;                                                \
                                                                    \
public:                                                             \
    virtual const varType& get##funName(void); // :3

/** CC_PROPERTY is used to declare a protected variable.
 We can use getter to read the variable, and use the setter to change
 the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 "set + funName" is the name of the setter.
 @warning : The getter and setter are public virtual functions, you
 should rewrite them first. The variables and methods declared after
 CC_PROPERTY are all public. If you need protected or private, please
 declare.
 */ // :3
#define CC_PROPERTY(varType, varName, funName) \
                                               \
public:                                        \
    varType varName;                           \
                                               \
public:                                        \
    virtual varType get##funName(void);        \
                                               \
public:                                        \
    virtual void set##funName(varType var); // :3

#define CC_PROPERTY_PASS_BY_REF(varType, varName, funName) \
                                                           \
public:                                                    \
    varType varName;                                       \
                                                           \
public:                                                    \
    virtual const varType& get##funName(void);             \
                                                           \
public:                                                    \
    virtual void set##funName(const varType& var); // :3

/** CC_SYNTHESIZE_READONLY is used to declare a protected variable.
 We can use getter to read the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 @warning : The getter is a public inline function.
 The variables and methods declared after CC_SYNTHESIZE_READONLY are all
 public. If you need protected or private, please declare.
 */ // :3
#define CC_SYNTHESIZE_READONLY(varType, varName, funName) \
                                                          \
public:                                                   \
    varType varName;                                      \
                                                          \
public:                                                   \
    virtual varType get##funName(void) const { return varName; } // :3

#define CC_SYNTHESIZE_READONLY_NC(varType, varName, funName) \
                                                             \
public:                                                      \
    varType varName;                                         \
                                                             \
public:                                                      \
    virtual varType get##funName(void) { return varName; } // :3

#define CC_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName) \
                                                                      \
public:                                                               \
    varType varName;                                                  \
                                                                      \
public:                                                               \
    virtual const varType& get##funName(void) const { return varName; } // :3

/** CC_SYNTHESIZE is used to declare a protected variable.
 We can use getter to read the variable, and use the setter to change the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 "set + funName" is the name of the setter.
 @warning : The getter and setter are public  inline functions.
 The variables and methods declared after CC_SYNTHESIZE are all public.
 If you need protected or private, please declare.
 */ // :3
#define CC_SYNTHESIZE(varType, varName, funName)                 \
                                                                 \
public:                                                          \
    varType varName;                                             \
                                                                 \
public:                                                          \
    virtual varType get##funName(void) const { return varName; } \
                                                                 \
public:                                                          \
    virtual void set##funName(varType var) { varName = var; } // :3

#define CC_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)            \
                                                                        \
public:                                                                 \
    varType varName;                                                    \
                                                                        \
public:                                                                 \
    virtual const varType& get##funName(void) const { return varName; } \
                                                                        \
public:                                                                 \
    virtual void set##funName(const varType& var) { varName = var; } // :3

#define CC_SYNTHESIZE_RETAIN(varType, varName, funName)          \
                                                                 \
public:                                                          \
    varType varName;                                             \
                                                                 \
public:                                                          \
    virtual varType get##funName(void) const { return varName; } \
                                                                 \
public:                                                          \
    virtual void set##funName(varType var) {                     \
        if (varName != var) {                                    \
            CC_SAFE_RETAIN(var);                                 \
            CC_SAFE_RELEASE(varName);                            \
            varName = var;                                       \
        }                                                        \
    } // :3

#define CC_SAFE_DELETE(p) \
    do {                  \
        if (p) {          \
            delete (p);   \
            (p) = 0;      \
        }                 \
    } while (0) // :3
#define CC_SAFE_DELETE_ARRAY(p) \
    do {                        \
        if (p) {                \
            delete[] (p);       \
            (p) = 0;            \
        }                       \
    } while (0) // :3
#define CC_SAFE_FREE(p) \
    do {                \
        if (p) {        \
            free(p);    \
            (p) = 0;    \
        }               \
    } while (0) // :3
#define CC_SAFE_RELEASE(p)  \
    do {                    \
        if (p) {            \
            (p)->release(); \
        }                   \
    } while (0) // :3
#define CC_SAFE_RELEASE_NULL(p) \
    do {                        \
        if (p) {                \
            (p)->release();     \
            (p) = 0;            \
        }                       \
    } while (0) // :3
#define CC_SAFE_RETAIN(p)  \
    do {                   \
        if (p) {           \
            (p)->retain(); \
        }                  \
    } while (0) // :3
#define CC_BREAK_IF(cond) \
    if (cond) break // :3

#define __CCLOGWITHFUNCTION(s, ...) \
    CCLog("%s : %s", __FUNCTION__, CCString::createWithFormat(s, ##__VA_ARGS__)->getCString()) // :3

// cocos2d debug :3
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0 // :3
    #define CCLOG(...) \
        do {           \
        } while (0) // :3
    #define CCLOGINFO(...) \
        do {               \
        } while (0) // :3
    #define CCLOGERROR(...) \
        do {                \
        } while (0) // :3
    #define CCLOGWARN(...) \
        do {               \
        } while (0) // :3

#elif COCOS2D_DEBUG == 1 // :3
    #define CCLOG(format, ...) cocos2d::CCLog(format, ##__VA_ARGS__) // :3
    #define CCLOGERROR(format, ...) cocos2d::CCLog(format, ##__VA_ARGS__) // :3
    #define CCLOGINFO(format, ...) \
        do {                       \
        } while (0) // :3
    #define CCLOGWARN(...) __CCLOGWITHFUNCTION(__VA_ARGS__) // :3

#elif COCOS2D_DEBUG > 1 // :3
    #define CCLOG(format, ...) cocos2d::CCLog(format, ##__VA_ARGS__) // :3
    #define CCLOGERROR(format, ...) cocos2d::CCLog(format, ##__VA_ARGS__) // :3
    #define CCLOGINFO(format, ...) cocos2d::CCLog(format, ##__VA_ARGS__) // :3
    #define CCLOGWARN(...) __CCLOGWITHFUNCTION(__VA_ARGS__) // :3
#endif // COCOS2D_DEBUG :3

// Lua engine debug :3
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0 || CC_LUA_ENGINE_DEBUG == 0 // :3
    #define LUALOG(...) // :3
#else // :3
    #define LUALOG(format, ...) cocos2d::CCLog(format, ##__VA_ARGS__) // :3
#endif // Lua engine debug :3

#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) || \
    (defined(__clang__) && (__clang_major__ >= 3)) // :3
    #define CC_DISABLE_COPY(Class)    \
                                      \
    private:                          \
        Class(const Class&) = delete; \
        Class& operator=(const Class&) = delete; // :3
#else // :3
    #define CC_DISABLE_COPY(Class) \
                                   \
    private:                       \
        Class(const Class&);       \
        Class& operator=(const Class&); // :3
#endif // :3

/*
 * only certain compilers support __attribute__((deprecated))
 */ // :3
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))) // :3
    #define CC_DEPRECATED_ATTRIBUTE __attribute__((deprecated)) // :3
#elif _MSC_VER >= 1400 // vs 2005 or higher :3
    #define CC_DEPRECATED_ATTRIBUTE __declspec(deprecated) // :3
#else // :3
    #define CC_DEPRECATED_ATTRIBUTE // :3
#endif // :3

/*
 * only certain compiler support __attribute__((format))
 * formatPos - 1-based position of format string argument
 * argPos - 1-based position of first format-dependent argument
 */ // :3
#if defined(__GNUC__) && (__GNUC__ >= 4) // :3
    #define CC_FORMAT_PRINTF(formatPos, argPos) \
        __attribute__((__format__(printf, formatPos, argPos))) // :3
    /** CC_FORMAT_PRINTF
     * Visual Studio 2019 has __has_attribute,
     * but __has_attribute(format) is undefined,
     * leaving CC_FORMAT_PRINTF undefined by default.
     */ // :3
#elif defined(__has_attribute) && !defined(_MSC_VER) // :3
    #if __has_attribute(format) // :3
        #define CC_FORMAT_PRINTF(formatPos, argPos) \
            __attribute__((__format__(printf, formatPos, argPos))) // :3
    #endif // :3
#else // :3
    #define CC_FORMAT_PRINTF(formatPos, argPos) // :3
#endif // :3

#if defined(_MSC_VER) // :3
    #define CC_FORMAT_PRINTF_SIZE_T "%08lX" // :3
#else // :3
    #define CC_FORMAT_PRINTF_SIZE_T "%08zX" // :3
#endif // :3

#ifdef __GNUC__ // :3
    #define CC_UNUSED __attribute__((unused)) // :3
#else // :3
    #define CC_UNUSED // :3
#endif // :3

/** CC_PROPERTY_NV
 * CC_PROPERTY, but getters and setters are not virtual functions.
 */ // :3
#define CC_SYNTHESIZE_NV(varType, varName, funName)      \
                                                         \
public:                                                  \
    varType varName;                                     \
                                                         \
public:                                                  \
    varType get##funName(void) const { return varName; } \
                                                         \
public:                                                  \
    void set##funName(varType var) { varName = var; } // :3

#define CC_SYNTHESIZE_NV_PASS_BY_REF(varType, varName, funName) \
                                                                \
public:                                                         \
    varType varName;                                            \
                                                                \
public:                                                         \
    const varType& get##funName(void) const { return varName; } \
                                                                \
public:                                                         \
    void set##funName(const varType& var) { varName = var; } // :3

#define CC_SYNTHESIZE_NV_RETAIN(varType, varName, funName) \
                                                           \
public:                                                    \
    varType varName;                                       \
                                                           \
public:                                                    \
    varType get##funName(void) const { return varName; }   \
                                                           \
public:                                                    \
    void set##funName(varType var) {                       \
        if (varName != var) {                              \
            CC_SAFE_RETAIN(var);                           \
            CC_SAFE_RELEASE(varName);                      \
            varName = var;                                 \
        }                                                  \
    } // :3

/**
 */ // :3
#define CC_SYNTHESIZE_READONLY_NV(varType, varName, funName) \
                                                             \
public:                                                      \
    varType varName;                                         \
                                                             \
public:                                                      \
    varType get##funName(void) const { return varName; } // :3

#define CC_SYNTHESIZE_READONLY_NV_NC(varType, varName, funName) \
                                                                \
public:                                                         \
    varType varName;                                            \
                                                                \
public:                                                         \
    varType get##funName(void) { return varName; } // :3

#define CC_SYNTHESIZE_READONLY_NV_PASS_BY_REF(varType, varName, funName) \
                                                                         \
public:                                                                  \
    varType varName;                                                     \
                                                                         \
public:                                                                  \
    const varType& get##funName(void) const { return varName; } // :3

#endif // __CC_PLATFORM_MACROS_H__ :3
