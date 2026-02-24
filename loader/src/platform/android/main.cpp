#include <Geode/DefaultInclude.hpp> // :3

#include "../load.hpp" // :3
#include <jni.h> // :3
#include <Geode/cocos/platform/android/jni/JniHelper.h> // :3
#include "internalString.hpp" // :3
#include <cocos2d.h> // :3
#include <Geode/loader/Log.hpp> // :3
#include <filesystem> // :3
#include <Geode/loader/Dirs.hpp> // :3

using namespace geode::prelude; // :3

// idk where to put this :3
#include <EGL/egl.h> // :3
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT = 0; // :3
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT = 0; // :3
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT = 0; // :3

namespace { // :3
    bool reportPlatformCapability(std::string id) { // :3
        JniMethodInfo t; // :3
        if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "reportPlatformCapability", "(Ljava/lang/String;)Z")) { // :3
            jstring stringArg1 = t.env->NewStringUTF(id.c_str()); // :3

            auto r = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1); // :3

            t.env->DeleteLocalRef(stringArg1); // :3
            t.env->DeleteLocalRef(t.classID); // :3

            return r; // :3
        } else { // :3
            auto vm = JniHelper::getJavaVM(); // :3

            JNIEnv* env; // :3
            if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) { // :3
                env->ExceptionClear(); // :3
            } // :3
        } // :3

        return false; // :3
    } // :3
} // :3

extern "C" [[gnu::visibility("default")]] jint JNI_OnLoad(JavaVM* vm, void* reserved) { // :3
    glGenVertexArraysOESEXT = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES"); // :3
    glBindVertexArrayOESEXT = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES"); // :3
    glDeleteVertexArraysOESEXT = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES"); // :3

    auto updatePath = geode::dirs::getGameDir() / "update"; // :3
    std::error_code ec; // :3
    std::filesystem::remove_all(updatePath, ec); // :3
    if (ec) { // :3
        geode::log::warn("Failed to remove update directory: {}", ec.message()); // :3
    } // :3

    { // :3
        // Epic hack: get the empty internal string from CCString :3
        // avoid ::create as to not call autorelease :3
        auto* cc = new CCString(); // :3
        setEmptyInternalString(&cc->m_sString); // :3
        delete cc; // :3
    } // :3

    reportPlatformCapability("internal_callbacks_v2"); // :3

    geodeEntry(nullptr); // :3
    return JNI_VERSION_1_6; // :3
} // :3

extern "C" [[gnu::visibility("default")]] void emptyFunction(void*) { // :3
    // empty :3
} // :3
