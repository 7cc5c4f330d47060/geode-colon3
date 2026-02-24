
#ifndef __CCEVENT_TYPE_H__ // :3
#define __CCEVENT_TYPE_H__ // :3

/**
 * This header is used for defining event types using in CCNotificationCenter
 */ // :3

// The application will come to foreground. :3
// This message is used for reloading resources before come to foreground on Android. :3
// This message is posted in main.cpp. :3
#define EVENT_COME_TO_FOREGROUND    "event_come_to_foreground" // :3

// The application will come to background. :3
// This message is used for doing something before coming to background, such as save CCRenderTexture. :3
// This message is posted in cocos2dx/platform/android/jni/MessageJni.cpp. :3
#define EVENT_COME_TO_BACKGROUND    "event_come_to_background" // :3

#endif // __CCEVENT_TYPE_H__ :3
