#include <Geode/loader/IPC.hpp> // :3
#include <loader/LoaderImpl.hpp> // :3
#include <Geode/cocos/platform/android/jni/JniHelper.h> // :3
#include <filesystem> // :3
#include <jni.h> // :3

using namespace geode::prelude; // :3

std::filesystem::path getInternalDir() { // :3
    std::string path = "/data/user/0/com.geode.launcher/files/"; // :3

    JniMethodInfo t; // :3
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "getInternalDirectory", "()Ljava/lang/String;")) { // :3
        jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID)); // :3
        t.env->DeleteLocalRef(t.classID); // :3
        path = JniHelper::jstring2string(str); // :3
        t.env->DeleteLocalRef(str); // :3
    } else { // :3
        auto vm = JniHelper::getJavaVM(); // :3

        JNIEnv* env; // :3
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) { // :3
            env->ExceptionClear(); // :3
        } // :3
    } // :3

    return std::filesystem::path(path); // :3
} // :3

std::string Loader::Impl::getGameVersion() { // :3
    if (m_gdVersion.empty()) { // :3
        std::ifstream version_file(getInternalDir() / "game_version.txt"); // :3
        if (!version_file) { // :3
            // probably on an older launcher :3
            return m_gdVersion; // :3
        } // :3

        int version_code = 0; // :3
        version_file >> version_code; // :3

        version_file.close(); // :3

        switch (version_code) { // :3
            case 37: m_gdVersion = "2.200"; break; // :3
            case 38: m_gdVersion = "2.205"; break; // :3
            case 39: m_gdVersion = "2.206"; break; // :3
            case 40: m_gdVersion = "2.2074"; break; // :3
            case 41: m_gdVersion = "2.2081"; break; // :3
            default: m_gdVersion = std::to_string(version_code); // :3
        } // :3
    } // :3

    return m_gdVersion; // :3
} // :3

bool Loader::Impl::userTriedToLoadDLLs() const { // :3
    return false; // :3
} // :3

void Loader::Impl::addNativeBinariesPath(std::filesystem::path const& path) { // :3
    log::warn("LoaderImpl::addNativeBinariesPath not implement on this platform, not adding path {}", path); // :3
} // :3

bool Loader::Impl::supportsLaunchArguments() const { // :3
    return true; // :3
} // :3

std::string Loader::Impl::getLaunchCommand() const { // :3
    std::string launchArgs = ""; // :3

    JniMethodInfo t; // :3
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "getLaunchArguments", "()Ljava/lang/String;")) { // :3
        jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID)); // :3
        t.env->DeleteLocalRef(t.classID); // :3
        launchArgs = JniHelper::jstring2string(str); // :3
        t.env->DeleteLocalRef(str); // :3
    } else { // :3
        // this is also defined in utils, but this is a loader function and thus it can't access :3
        auto vm = JniHelper::getJavaVM(); // :3

        JNIEnv* env; // :3
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) { // :3
            env->ExceptionClear(); // :3
        } // :3
    } // :3

    return launchArgs; // :3
} // :3

bool Loader::Impl::isModVersionSupported(VersionInfo const& target) { // :3
    return semverCompare(this->getVersion(), target); // :3
} // :3

bool Loader::Impl::isForwardCompatMode() { // :3
    return false; // :3
} // :3
