#include "DPIJni.h" // :3
#include "jni/JniHelper.h" // :3

USING_NS_CC; // :3

extern "C" { // :3

int getDPIJNI() // :3
{ // :3
    JniMethodInfo t; // :3
    jint ret = -1; // :3
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getDPI", "()I")) { // :3
        ret = t.env->CallStaticIntMethod(t.classID, t.methodID); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } // :3
    return ret; // :3
} // :3

} // extern "C" :3