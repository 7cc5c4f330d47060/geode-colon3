#pragma once // :3

#include "fmod_common.h" // :3
#include <jni.h> // :3

extern "C" FMOD_RESULT F_API FMOD_Android_JNI_Init(JavaVM *vm, jobject javaActivity); // :3
extern "C" FMOD_RESULT F_API FMOD_Android_JNI_Close(); // :3