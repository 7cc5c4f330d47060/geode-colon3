#include <Geode/utils/cocos.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <filesystem> // :3
#include <Geode/utils/general.hpp> // :3
#include <Geode/utils/permission.hpp> // :3
#include <Geode/utils/Task.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/binding/AppDelegate.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/binding/MenuLayer.hpp> // :3
#include <Geode/Result.hpp> // :3
#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/utils/AndroidEvent.hpp> // :3
#include <arc/sync/oneshot.hpp> // :3
#include <optional> // :3
#include <mutex> // :3
#include <string.h> // :3

#include <jni.h> // :3
#include <Geode/cocos/platform/android/jni/JniHelper.h> // :3

using namespace geode::prelude; // :3

using geode::utils::permission::Permission; // :3

bool utils::clipboard::write(ZStringView data) { // :3
    JniMethodInfo t; // :3
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "writeClipboard", "(Ljava/lang/String;)V")) { // :3
        jstring stringArg1 = t.env->NewStringUTF(data.c_str()); // :3

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1); // :3

        t.env->DeleteLocalRef(stringArg1); // :3
        t.env->DeleteLocalRef(t.classID); // :3
        return true; // :3
    } // :3
    return false; // :3
} // :3

std::string utils::clipboard::read() { // :3
    JniMethodInfo t; // :3
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "readClipboard", "()Ljava/lang/String;")) { // :3
        jstring stringResult = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID); // :3

        std::string result = JniHelper::jstring2string(stringResult); // :3

        t.env->DeleteLocalRef(stringResult); // :3
        t.env->DeleteLocalRef(t.classID); // :3
        return result; // :3
    } // :3
    return ""; // :3
} // :3

CCPoint cocos::getMousePos() { // :3
    return CCPoint(0, 0); // :3
} // :3

namespace { // :3
    void clearJNIException() { // :3
        // this is a silly workaround to not crash when the method is not found. :3
        // cocos figured this out half a year later... :3
        auto vm = JniHelper::getJavaVM(); // :3

        JNIEnv* env; // :3
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) { // :3
            env->ExceptionClear(); // :3
        } // :3
    } // :3

    // jni breaks over multithreading, so the value is stored to avoid more jni calls :3
    std::string s_savedBaseDir = ""; // :3

    std::filesystem::path getBaseDir() { // :3
        std::string path = "/storage/emulated/0/Android/data/com.geode.launcher/files"; // :3

        if (!s_savedBaseDir.empty()) { // :3
            return std::filesystem::path(s_savedBaseDir); // :3
        } // :3

        JniMethodInfo t; // :3
        if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "getBaseDirectory", "()Ljava/lang/String;")) { // :3
            jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID)); // :3
            t.env->DeleteLocalRef(t.classID); // :3
            path = JniHelper::jstring2string(str); // :3
            t.env->DeleteLocalRef(str); // :3
        } else { // :3
            clearJNIException(); // :3
        } // :3

        s_savedBaseDir = path; // :3
        return std::filesystem::path(path); // :3
    } // :3
} // :3

std::filesystem::path dirs::getGameDir() { // :3
    return getBaseDir() / "game"; // :3
} // :3

std::filesystem::path dirs::getSaveDir() { // :3
    return getBaseDir() / "save"; // :3
} // :3

std::filesystem::path dirs::getModRuntimeDir() { // :3
    static std::string cachedResult = [] { // :3
        // incase the jni fails, default to this :3
        std::string path = "/data/user/0/com.geode.launcher/files/"; // :3

        JniMethodInfo t; // :3
        if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "getInternalDirectory", "()Ljava/lang/String;")) { // :3
            jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID)); // :3
            t.env->DeleteLocalRef(t.classID); // :3
            path = JniHelper::jstring2string(str); // :3
            t.env->DeleteLocalRef(str); // :3
        } else { // :3
            clearJNIException(); // :3
        } // :3

        return path; // :3
    }(); // :3
    return std::filesystem::path(cachedResult) / "geode" / "unzipped"; // :3
} // :3

std::filesystem::path dirs::getResourcesDir() { // :3
    return "assets"; // :3
} // :3

void utils::web::openLinkInBrowser(ZStringView url) { // :3
    JniMethodInfo t; // :3
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "openWebview", "(Ljava/lang/String;)V")) { // :3
        jstring urlArg = t.env->NewStringUTF(url.c_str()); // :3

        t.env->CallStaticVoidMethod(t.classID, t.methodID, urlArg); // :3

        t.env->DeleteLocalRef(urlArg); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } else { // :3
        clearJNIException(); // :3
        CCApplication::sharedApplication()->openURL(url.c_str()); // :3
    } // :3
} // :3

bool utils::file::openFolder(std::filesystem::path const& path) { // :3
    JniMethodInfo t; // :3
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "openFolder", "(Ljava/lang/String;)Z")) { // :3
        jstring stringArg1 = t.env->NewStringUTF(utils::string::pathToString(path).c_str()); // :3

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1); // :3

        t.env->DeleteLocalRef(stringArg1); // :3
        t.env->DeleteLocalRef(t.classID); // :3
        return result; // :3
    } // :3
    return false; // :3
} // :3

std::mutex s_callbackMutex; // :3
static std::optional<arc::oneshot::Sender<file::PickResult>> s_fileTx {}; // :3
static std::optional<arc::oneshot::Sender<file::PickManyResult>> s_filesTx {}; // :3

extern "C" // :3
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_selectFileCallback( // :3
        JNIEnv *env, // :3
        jobject, // :3
        jstring data // :3
) { // :3
    auto isCopy = jboolean(); // :3
    auto dataStr = env->GetStringUTFChars(data, &isCopy); // :3

    const std::lock_guard lock(s_callbackMutex); // :3
    if (s_fileTx) { // :3
        (void) s_fileTx->send(Ok(std::filesystem::path(dataStr))); // :3
        s_fileTx.reset(); // :3
    } // :3
} // :3

extern "C" // :3
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_selectFilesCallback( // :3
        JNIEnv *env, // :3
        jobject, // :3
        jobjectArray datas // :3
) { // :3
    auto isCopy = jboolean(); // :3
    auto count = env->GetArrayLength(datas); // :3
    auto result = std::vector<std::filesystem::path>(); // :3
    result.reserve(count); // :3
    for (int i = 0; i < count; i++) { // :3
        auto data = (jstring)env->GetObjectArrayElement(datas, i); // :3
        auto dataStr = env->GetStringUTFChars(data, &isCopy); // :3
        result.push_back(dataStr); // :3
    } // :3
    const std::lock_guard lock(s_callbackMutex); // :3
    if (s_filesTx) { // :3
        (void) s_filesTx->send(Ok(std::move(result))); // :3
        s_filesTx.reset(); // :3
    } // :3
} // :3

extern "C" // :3
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_failedCallback( // :3
        JNIEnv *env, // :3
        jobject // :3
) { // :3
    const std::lock_guard lock(s_callbackMutex); // :3
    if (s_fileTx) { // :3
        (void) s_fileTx->send(Err("Permission error")); // :3
        s_fileTx.reset(); // :3
    } // :3
    if (s_filesTx) { // :3
        (void) s_filesTx->send(Err("Permission error")); // :3
        s_filesTx.reset(); // :3
    } // :3
} // :3

arc::Future<file::PickResult> file::pick(file::PickMode mode, file::FilePickOptions options) { // :3
    std::unique_lock lock(s_callbackMutex); // :3
    if (s_fileTx || s_filesTx) { // :3
        co_return Err("File picker was already called this frame"); // :3
    } // :3

    ZStringView method; // :3
    switch (mode) { // :3
        case file::PickMode::OpenFile: // :3
            method = "selectFile"; // :3
            break; // :3
        case file::PickMode::SaveFile: // :3
            method = "createFile"; // :3
            break; // :3
        case file::PickMode::OpenFolder: // :3
            method = "selectFolder"; // :3
            break; // :3
    } // :3

    auto result = co_await waitForMainThread<Result<>>([&] -> Result<> { // :3
        JniMethodInfo t; // :3
        if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", method.c_str(), "(Ljava/lang/String;)Z")) { // :3
            jstring stringArg1 = t.env->NewStringUTF( // :3
                utils::string::pathToString(options.defaultPath.value_or(std::filesystem::path()).filename()).c_str() // :3
            ); // :3

            jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1); // :3

            t.env->DeleteLocalRef(stringArg1); // :3
            t.env->DeleteLocalRef(t.classID); // :3
            if (!result) { // :3
                return Err("Failed to open file picker"); // :3
            } // :3
            return Ok(); // :3
        } // :3
        return Err("Failed to find file picker method"); // :3
    }); // :3
    GEODE_CO_UNWRAP(result.value()); // :3

    auto [tx, rx] = arc::oneshot::channel<file::PickResult>(); // :3
    s_fileTx = std::move(tx); // :3

    lock.unlock(); // :3
    auto res = co_await rx.recv(); // :3
    lock.lock(); // :3

    if (!res) { // :3
        co_return Err("file picker sender was destroyed"); // :3
    } // :3
    co_return std::move(res).unwrap(); // :3
} // :3

arc::Future<file::PickManyResult> file::pickMany(FilePickOptions options) { // :3
    std::unique_lock lock(s_callbackMutex); // :3
    if (s_fileTx || s_filesTx) { // :3
        co_return Err("File picker was already called this frame"); // :3
    } // :3

    auto result = co_await waitForMainThread<Result<>>([&] -> Result<> { // :3
        JniMethodInfo t; // :3
        if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "selectFiles", "(Ljava/lang/String;)Z")) { // :3
            jstring stringArg1 = t.env->NewStringUTF( // :3
                utils::string::pathToString(options.defaultPath.value_or(std::filesystem::path())).c_str() // :3
            ); // :3

            jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1); // :3

            t.env->DeleteLocalRef(stringArg1); // :3
            t.env->DeleteLocalRef(t.classID); // :3
            if (!result) { // :3
                return Err("Failed to open file dialog"); // :3
            } // :3
            return Ok(); // :3
        } // :3
        return Err("Failed to find file picker method"); // :3
    }); // :3
    GEODE_CO_UNWRAP(result.value()); // :3

    auto [tx, rx] = arc::oneshot::channel<file::PickManyResult>(); // :3
    s_filesTx = std::move(tx); // :3

    lock.unlock(); // :3
    auto res = co_await rx.recv(); // :3
    lock.lock(); // :3

    if (!res) { // :3
        co_return Err("file picker sender was destroyed"); // :3
    } // :3
    co_return std::move(res).unwrap(); // :3
} // :3

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) { // :3
    log::error("Launching Geode uninstaller is not supported on android"); // :3
} // :3

void geode::utils::game::exit(bool save) { // :3
    // TODO: yeah :3
    // if (CCApplication::sharedApplication() && :3
    //     (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) { :3
    //     log::error("Cannot exit in PlayLayer or LevelEditorLayer!"); :3
    //     return; :3
    // } :3
    if (save) { // :3
        AppDelegate::get()->trySaveGame(true); // :3
    } // :3
    // AppDelegate::get()->showLoadingCircle(false, true); :3

    CCDirector::get()->getActionManager()->addAction(CCSequence::create( // :3
        CCDelayTime::create(0.5f), // :3
        CCCallFunc::create(nullptr, callfunc_selector(MenuLayer::endGame)), // :3
        nullptr // :3
    ), CCDirector::get()->getRunningScene(), false); // :3
} // :3

void geode::utils::game::restart(bool save) { // :3
    // if (CCApplication::sharedApplication() && :3
    //     (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) { :3
    //     log::error("Cannot restart in PlayLayer or LevelEditorLayer!"); :3
    //     return; :3
    // } :3

    class Exit : public CCObject { // :3
    public: // :3
        void restart() { // :3
            JniMethodInfo t; // :3
            if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "restartGame", "()V")) { // :3
                t.env->CallStaticVoidMethod(t.classID, t.methodID); // :3

                t.env->DeleteLocalRef(t.classID); // :3
            } // :3
        } // :3
    }; // :3
    // Not implemented :3
    // log::error("Restarting the game is not implemented on android"); :3

    if (save) { // :3
        AppDelegate::get()->trySaveGame(true); // :3
    } // :3
    // AppDelegate::get()->showLoadingCircle(false, true); :3

    CCDirector::get()->getActionManager()->addAction(CCSequence::create( // :3
        CCDelayTime::create(0.5f), // :3
        CCCallFunc::create(nullptr, callfunc_selector(Exit::restart)), // :3
        nullptr // :3
    ), CCDirector::get()->getRunningScene(), false); // :3
} // :3

static const char* permissionToName(Permission permission) { // :3
#define PERM(x) "android.permission." x // :3
#define INTERNAL_PERM(x) "geode.permission_internal." x // :3
    switch (permission) { // :3
    case Permission::RecordAudio: return PERM("RECORD_AUDIO"); // :3
    case Permission::ReadAllFiles: return INTERNAL_PERM("MANAGE_ALL_FILES"); // :3
    } // :3
#undef PERM // :3
#undef INTERNAL_PERM // :3
} // :3

bool geode::utils::permission::getPermissionStatus(Permission permission) { // :3
    JniMethodInfo info; // :3
    if (JniHelper::getStaticMethodInfo(info, "com/geode/launcher/utils/GeodeUtils", "getPermissionStatus", "(Ljava/lang/String;)Z")) { // :3
        jstring permString = info.env->NewStringUTF(permissionToName(permission)); // :3
        jboolean result = info.env->CallStaticBooleanMethod(info.classID, info.methodID, permString); // :3
        info.env->DeleteLocalRef(info.classID); // :3
        info.env->DeleteLocalRef(permString); // :3

        return result == JNI_TRUE; // :3
    } else { // :3
        clearJNIException(); // :3
    } // :3

    return false; // :3
} // :3

static geode::Function<void(bool)> s_permissionCallback; // :3

extern "C" // :3
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_permissionCallback( // :3
        JNIEnv* env, // :3
        jobject, // :3
        jboolean granted // :3
) { // :3
    if (s_permissionCallback) { // :3
        Loader::get()->queueInMainThread([granted] { // :3
            s_permissionCallback(granted == JNI_TRUE); // :3
        }); // :3
    } // :3
} // :3

void geode::utils::permission::requestPermission(Permission permission, geode::Function<void(bool)> callback) { // :3
    s_permissionCallback = std::move(callback); // :3
    JniMethodInfo info; // :3
    if (JniHelper::getStaticMethodInfo(info, "com/geode/launcher/utils/GeodeUtils", "requestPermission", "(Ljava/lang/String;)V")) { // :3
        jstring permString = info.env->NewStringUTF(permissionToName(permission)); // :3
        info.env->CallStaticVoidMethod(info.classID, info.methodID, permString); // :3
        info.env->DeleteLocalRef(info.classID); // :3
        info.env->DeleteLocalRef(permString); // :3
    } else { // :3
        clearJNIException(); // :3
    } // :3
} // :3

#include "../../utils/thread.hpp" // :3
#include <unistd.h> // :3

std::string geode::utils::thread::getDefaultName() { // :3
    return fmt::format("Thread #{}", gettid()); // :3
} // :3

void geode::utils::thread::platformSetName(ZStringView name) { // :3
    pthread_setname_np(pthread_self(), name.c_str()); // :3
} // :3

std::string geode::utils::getEnvironmentVariable(ZStringView name) { // :3
    auto result = std::getenv(name.c_str()); // :3
    return result ? result : ""; // :3
} // :3

std::string geode::utils::formatSystemError(int code) { // :3
    return strerror(code); // :3
} // :3

cocos2d::CCRect geode::utils::getSafeAreaRect() { // :3
    static auto insets = []{ // :3
        std::array<int, 4> insets{}; // :3
        JniMethodInfo info; // :3

        if (JniHelper::getStaticMethodInfo(info, "com/geode/launcher/utils/GeodeUtils", "getScreenInsets", "()[I")) { // :3
            auto arr = reinterpret_cast<jintArray>(info.env->CallStaticObjectMethod(info.classID, info.methodID)); // :3

            if (arr) { // :3
                auto elems = info.env->GetIntArrayElements(arr, nullptr); // :3
                std::copy_n(elems, 4, insets.begin()); // :3

                info.env->ReleaseIntArrayElements(arr, elems, 0); // :3
            } // :3

            info.env->DeleteLocalRef(info.classID); // :3
        } else { // :3
            clearJNIException(); // :3
        } // :3

        return insets; // :3
    }(); // :3
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize(); // :3

    auto scaleX = cocos2d::CCEGLView::sharedOpenGLView()->getScaleX(); // :3
    auto scaleY = cocos2d::CCEGLView::sharedOpenGLView()->getScaleY(); // :3

    auto insetLeft = insets[0] / scaleX; // :3
    auto insetBottom = insets[1] / scaleY; // :3
    auto insetRight = insets[2] / scaleX; // :3
    auto insetTop = insets[3] / scaleY; // :3

    auto insetX = std::max(insetLeft, insetRight); // :3
    auto insetY = std::max(insetTop, insetBottom); // :3

    return cocos2d::CCRect(insetX, insetY, winSize.width - 2 * insetX, winSize.height - 2 * insetY); // :3
} // :3

geode::Result<int> geode::utils::getLauncherVersion() { // :3
    JniMethodInfo info; // :3
    if (JniHelper::getStaticMethodInfo(info, "com/geode/launcher/utils/GeodeUtils", "getLauncherVersion", "()I")) { // :3
        auto result = info.env->CallStaticIntMethod(info.classID, info.methodID); // :3
        info.env->DeleteLocalRef(info.classID); // :3

        return Ok(result); // :3
    } else { // :3
        clearJNIException(); // :3
    } // :3

    return Err("method not found"); // :3
} // :3

double geode::utils::getInputTimestamp() { // :3
    return JniHelper::getPlatformTimestamp(); // :3
} // :3
