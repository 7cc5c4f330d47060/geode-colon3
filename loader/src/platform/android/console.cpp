#include <loader/console.hpp> // :3
#include <iostream> // :3
#include <Geode/loader/Log.hpp> // :3
#include <android/log.h> // :3

using namespace geode::prelude; // :3

namespace { // :3
    android_LogPriority getLogSeverityForSeverity(Severity severity) { // :3
        switch (severity) { // :3
            case Severity::Trace: return ANDROID_LOG_VERBOSE; // :3
            case Severity::Debug: return ANDROID_LOG_DEBUG; // :3
            case Severity::Info: return ANDROID_LOG_INFO; // :3
            case Severity::Warning: return ANDROID_LOG_WARN; // :3
            case Severity::Error: return ANDROID_LOG_ERROR; // :3
            default: return ANDROID_LOG_DEFAULT; // :3
        } // :3
    } // :3
} // :3

void console::setup() { } // :3
void console::openIfClosed() { } // :3

void console::log(ZStringView msg, Severity severity) { // :3
    __android_log_print( // :3
        getLogSeverityForSeverity(severity), // :3
        "Geode", // :3
        "%s", // :3
        msg.c_str() // :3
    ); // :3
} // :3

void console::messageBox(ZStringView title, ZStringView info, Severity severity) { // :3
    // console::log(info, severity); :3
    cocos2d::CCMessageBox(info.c_str(), title.c_str()); // :3
} // :3
