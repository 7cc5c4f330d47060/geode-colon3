#include <Geode/loader/Loader.hpp> // :3

using namespace geode::prelude; // :3

#include <Geode/modify/AppDelegate.hpp> // :3
#include <Geode/modify/CCApplication.hpp> // :3

namespace { // :3
    void saveModData() { // :3
        log::info("Saving mod data..."); // :3
        log::NestScope nest; // :3

        auto begin = std::chrono::high_resolution_clock::now(); // :3

        (void)Loader::get()->saveData(); // :3

        auto end = std::chrono::high_resolution_clock::now(); // :3
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); // :3
        log::info("Took {}s", static_cast<float>(time) / 1000.f); // :3
    } // :3
} // :3

struct SaveLoader : Modify<SaveLoader, AppDelegate> { // :3
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("save moved to CCApplication::gameDidSave()") // :3
    void trySaveGame(bool p0) { // :3
        saveModData(); // :3
        return AppDelegate::trySaveGame(p0); // :3
    } // :3
}; // :3

#ifdef GEODE_IS_WINDOWS // :3

struct FallbackSaveLoader : Modify<FallbackSaveLoader, CCApplication> { // :3
    GEODE_FORWARD_COMPAT_ENABLE_HOOKS("") // :3
    void gameDidSave() { // :3
        saveModData(); // :3
        return CCApplication::gameDidSave(); // :3
    } // :3
}; // :3

#endif // :3

#ifdef GEODE_IS_ANDROID // :3

#include <Geode/modify/FileOperation.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3

// redirects the save path to what geode knows, in case launcher's fopen hook fails :3
struct FileOperationOverride : Modify<FileOperationOverride, FileOperation> { // :3
    static gd::string getFilePath() { // :3
        return utils::string::pathToString(dirs::getSaveDir()) + "/"; // :3
    } // :3
}; // :3

#endif // :3
