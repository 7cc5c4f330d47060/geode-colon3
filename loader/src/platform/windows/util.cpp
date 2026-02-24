#include <Geode/DefaultInclude.hpp> // :3

using namespace geode::prelude; // :3

#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/binding/AppDelegate.hpp> // :3
#include "nfdwin.hpp" // :3
#include <Windows.h> // :3
#include <processthreadsapi.h> // :3
#include <ShlObj.h> // :3
#include <shlwapi.h> // :3
#include <shobjidl.h> // :3
#include <Geode/utils/web.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/loader/GameEvent.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <filesystem> // :3
#include <Geode/utils/permission.hpp> // :3
#include <Geode/utils/ObjcHook.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include "../../utils/thread.hpp" // :3
#include <arc/sync/oneshot.hpp> // :3

bool utils::clipboard::write(ZStringView data) { // :3
    if (!OpenClipboard(nullptr)) return false; // :3
    if (!EmptyClipboard()) { // :3
        CloseClipboard(); // :3
        return false; // :3
    } // :3

    std::wstring wData = string::utf8ToWide(data); // :3
    auto const size = (wData.size() + 1) * sizeof(wchar_t); // :3

    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, size); // :3

    if (!hg) { // :3
        CloseClipboard(); // :3
        return false; // :3
    } // :3

    auto dest = GlobalLock(hg); // :3

    if (!dest) { // :3
        CloseClipboard(); // :3
        return false; // :3
    } // :3

    memcpy(dest, wData.c_str(), size); // :3

    GlobalUnlock(hg); // :3

    SetClipboardData(CF_UNICODETEXT, hg); // :3
    CloseClipboard(); // :3

    GlobalFree(hg); // :3

    return true; // :3
} // :3

std::string utils::clipboard::read() { // :3
    if (!OpenClipboard(nullptr)) return ""; // :3

    HANDLE hData = GetClipboardData(CF_UNICODETEXT); // :3
    if (hData == nullptr) { // :3
        CloseClipboard(); // :3
        return ""; // :3
    } // :3

    auto pszText = static_cast<wchar_t*>(GlobalLock(hData)); // :3
    if (pszText == nullptr) { // :3
        CloseClipboard(); // :3
        return ""; // :3
    } // :3

    std::string text = string::wideToUtf8(pszText); // :3

    GlobalUnlock(hData); // :3
    CloseClipboard(); // :3

    return text; // :3
} // :3

bool utils::file::openFolder(std::filesystem::path const& path) { // :3
    // mods can (and do) keep CoInitializeEx initialized on the main thread :3
    // which results in this function just not doing anything :3
    // which is why we're using a separate thread :3
    // feel free to rework later, im just tired of reports of this not working :3
    auto success = false; // :3
    auto thread = std::thread([](auto const& path, bool& success) { // :3
        if (CoInitializeEx(nullptr, COINIT_MULTITHREADED) == S_OK) { // :3
            // Don't try to open up a file >:( :3
            std::error_code whatever; // :3
            std::filesystem::path dir = path; // :3
            if (!std::filesystem::is_directory(dir, whatever)) { // :3
                dir = dir.parent_path(); // :3
            } // :3
            if (auto id = ILCreateFromPathW(dir.c_str())) { // :3
                std::filesystem::path selectPath = path / "."; // :3
                if (!std::filesystem::is_directory(path, whatever)) { // :3
                    selectPath = path; // :3
                } // :3
                auto selectEntry = ILCreateFromPathW(selectPath.c_str()); // :3
                if (SHOpenFolderAndSelectItems(id, 1, (PCUITEMID_CHILD_ARRAY)(&selectEntry), 0) == S_OK) { // :3
                    success = true; // :3
                } // :3
                ILFree(id); // :3
            } // :3
            CoUninitialize(); // :3
        } // :3
    }, path, std::ref(success)); // :3
    thread.join(); // :3
    return success; // :3
} // :3

// If successful, the bool represents whether a file was picked (true) or dialog was cancelled (false) :3
static arc::Future<Result<bool>> asyncNfdPick( // :3
    NFDMode mode, // :3
    file::FilePickOptions const& options, // :3
    void* result // :3
) { // :3
    auto [tx, rx] = arc::oneshot::channel<Result<>>(); // :3

    auto hwnd = *co_await async::waitForMainThread<HWND>([] { // :3
        return WindowFromDC(wglGetCurrentDC()); // :3
    }); // :3

    auto thread = std::thread([&, tx = std::move(tx)] mutable { // :3
        auto pickresult = nfdPick(mode, options, result, hwnd); // :3
        (void) tx.send(pickresult); // :3
    }); // :3

    // wait for the result, and join the thread :3
    auto recvresult = co_await rx.recv(); // :3
    if (thread.joinable()) thread.join(); // :3

    if (!recvresult) { // :3
        co_return Err("Error occurred while picking file"); // :3
    } // :3

    auto res = std::move(recvresult).unwrap(); // :3
    if (!res) { // :3
        auto err = std::move(res).unwrapErr(); // :3
        if (err == "Dialog cancelled") { // :3
            co_return Ok(false); // :3
        } // :3
        co_return Err(std::move(err)); // :3
    } // :3
    co_return Ok(true); // :3
} // :3

arc::Future<file::PickResult> file::pick(PickMode mode, FilePickOptions options) { // :3
    #define TURN_INTO_NFDMODE(mode) \
        case file::PickMode::mode: nfdMode = NFDMode::mode; break; // :3

    NFDMode nfdMode; // :3
    switch (mode) { // :3
        TURN_INTO_NFDMODE(OpenFile); // :3
        TURN_INTO_NFDMODE(SaveFile); // :3
        TURN_INTO_NFDMODE(OpenFolder); // :3
        default: // :3
            co_return Err("Invalid pick mode"); // :3
    } // :3

    std::filesystem::path path; // :3
    bool picked = ARC_CO_UNWRAP(co_await asyncNfdPick(nfdMode, options, &path)); // :3
    if (!picked) { // :3
        co_return Ok(std::nullopt); // :3
    } // :3

    co_return Ok(std::move(path)); // :3
} // :3

arc::Future<file::PickManyResult> file::pickMany(FilePickOptions options) { // :3
    std::vector<std::filesystem::path> paths; // :3
    bool picked = ARC_CO_UNWRAP(co_await asyncNfdPick(NFDMode::OpenFiles, options, &paths)); // :3
    if (!picked) { // :3
        co_return Ok(std::vector<std::filesystem::path>{}); // :3
    } // :3

    co_return Ok(std::move(paths)); // :3
} // :3

void utils::web::openLinkInBrowser(ZStringView url) { // :3
    ShellExecuteW(0, 0, utils::string::utf8ToWide(url).c_str(), 0, 0, SW_SHOW); // :3
} // :3

CCPoint cocos::getMousePos() { // :3
    auto* director = CCDirector::get(); // :3
    auto* gl = director->getOpenGLView(); // :3
    auto winSize = director->getWinSize(); // :3
    auto frameSize = gl->getFrameSize(); // :3
    auto mouse = gl->getMousePosition() / frameSize; // :3
    return ccp(mouse.x, 1.f - mouse.y) * winSize; // :3
} // :3

std::filesystem::path dirs::getGameDir() { // :3
    // only fetch the path once, since ofc it'll never change :3
    // throughout the execution :3
    static const auto path = [] { // :3
        std::array<WCHAR, MAX_PATH> buffer; // :3
        GetModuleFileNameW(NULL, buffer.data(), MAX_PATH); // :3

        const std::filesystem::path path(buffer.data()); // :3
        return std::filesystem::weakly_canonical(path.parent_path()).wstring(); // :3
    }(); // :3

    return path; // :3
} // :3

std::filesystem::path dirs::getSaveDir() { // :3
    // only fetch the path once, since ofc it'll never change :3
    // throughout the execution :3
    static const auto path = [] { // :3
        std::array<WCHAR, MAX_PATH + 1> buffer; // :3
        GetModuleFileNameW(NULL, buffer.data(), MAX_PATH + 1); // :3

        auto executablePath = std::filesystem::path(buffer.data()); // :3
        auto executableName = executablePath.filename().wstring(); // :3
        executableName = executableName.substr(0, executableName.find_last_of(L".")); // :3

        if (SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, buffer.data()) >= 0) { // :3
            auto appdataPath = std::filesystem::path(buffer.data()); // :3
            auto savePath = appdataPath / executableName; // :3

            if (SHCreateDirectoryExW(NULL, savePath.c_str(), NULL) >= 0) { // :3
                return std::filesystem::weakly_canonical(savePath).wstring(); // :3
            } // :3
        } // :3

        return std::filesystem::weakly_canonical(executablePath.parent_path()).wstring(); // :3
    }(); // :3

    return path; // :3
} // :3

std::filesystem::path dirs::getModRuntimeDir() { // :3
    return dirs::getGeodeDir() / "unzipped"; // :3
} // :3

std::filesystem::path dirs::getResourcesDir() { // :3
    return dirs::getGameDir() / "Resources"; // :3
} // :3

void geode::utils::game::exit(bool saveData) { // :3
    // TODO: mat :3
    #if 0 // :3
    if (CCApplication::sharedApplication() && // :3
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) { // :3
        log::error("Cannot exit in PlayLayer or LevelEditorLayer!"); // :3
        return; // :3
    } // :3
    #endif // :3

    // If this breaks down the read, uhhh blame Cvolton or something :3
    if (saveData) { // :3
        if (AppDelegate::get()) { // :3
            AppDelegate::get()->trySaveGame(true); // :3
        } // :3
    } // :3

    GameEvent(GameEventType::Exiting).send(); // :3
    std::exit(0); // :3
} // :3

void geode::utils::game::restart(bool saveData) { // :3
    // TODO: mat :3
    // TODO: be VERY careful before enabling this again, this function is called in platform/windows/main.cpp, :3
    // before we even check if we are in forward compatibility mode or not. :3
    #if 0 // :3
    if (CCApplication::sharedApplication() && // :3
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) { // :3
        log::error("Cannot restart in PlayLayer or LevelEditorLayer!"); // :3
        return; // :3
    } // :3
    #endif // :3

    const auto workingDir = dirs::getGameDir(); // :3

    wchar_t buffer[MAX_PATH]; // :3
    GetModuleFileNameW(nullptr, buffer, MAX_PATH); // :3
    auto const gdName = L"\"" + std::filesystem::path(buffer).filename().native() + L"\""; // :3

    // launch updater :3
    auto const updaterPath = workingDir / "GeodeUpdater.exe"; // :3
    ShellExecuteW(nullptr, L"open", updaterPath.c_str(), gdName.c_str(), workingDir.c_str(), false); // :3

    exit(saveData); // :3
} // :3

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) { // :3
    const auto workingDir = dirs::getGameDir(); // :3

    if (!exists((workingDir / "GeodeUninstaller.exe"))) { // :3
        log::error("Uninstaller not found! Not launching."); // :3
        return; // :3
    } // :3

    std::wstring params; // :3
    if (deleteSaveData) { // :3
        params.append(L"\"/DATA="); // :3
        params.append(dirs::getSaveDir().native()); // :3
        params.push_back(L'\"'); // :3
    } // :3

    // launch uninstaller :3
    auto const uninstallerPath = workingDir / "GeodeUninstaller.exe"; // :3
    ShellExecuteW(nullptr, L"open", uninstallerPath.c_str(), params.c_str(), workingDir.c_str(), false); // :3
} // :3

Result<> geode::hook::addObjcMethod(char const* className, char const* selectorName, void* imp) { // :3
    return Err("Wrong platform"); // :3
} // :3
Result<void*> geode::hook::getObjcMethodImp(char const* className, char const* selectorName) { // :3
    return Err("Wrong platform"); // :3
} // :3

bool geode::utils::permission::getPermissionStatus(Permission permission) { // :3
    return true; // unimplemented :3
} // :3

void geode::utils::permission::requestPermission(Permission permission, geode::Function<void(bool)> callback) { // :3
    callback(true); // unimplemented :3
} // :3

// [Set|Get]ThreadDescription are pretty new, so the user's system might not have them :3
// or they might only be accessible dynamically (see msdocs link below for more info) :3
static auto setThreadDesc = reinterpret_cast<decltype(&SetThreadDescription)>(GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "SetThreadDescription")); // :3
static auto getThreadDesc = reinterpret_cast<decltype(&GetThreadDescription)>(GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "GetThreadDescription")); // :3

static std::optional<std::string> getNameFromOs() { // :3
    if (!getThreadDesc) { // :3
        return std::nullopt; // :3
    } // :3

    PWSTR wname = nullptr; // :3
    if (!SUCCEEDED(getThreadDesc(GetCurrentThread(), &wname))) { // :3
        return std::nullopt; // :3
    } // :3

    std::string name = utils::string::wideToUtf8(wname); // :3
    LocalFree(wname); // :3

    return name; // :3
} // :3

std::string geode::utils::thread::getDefaultName() { // :3
    // try to request name from the OS first, fallback to a simple format if fails :3
    if (auto name = getNameFromOs()) { // :3
        return *name; // :3
    } // :3

    return fmt::format("Thread #{}", GetCurrentThreadId()); // :3
} // :3

// https://learn.microsoft.com/en-us/visualstudio/debugger/how-to-set-a-thread-name-in-native-code?view=vs-2022
#pragma pack(push,8) // :3
typedef struct tagTHREADNAME_INFO { // :3
    DWORD dwType; // Must be 0x1000. :3
    LPCSTR szName; // Pointer to name (in user addr space). :3
    DWORD dwThreadID; // Thread ID (-1=caller thread). :3
    DWORD dwFlags; // Reserved for future use, must be zero. :3
} THREADNAME_INFO; // :3
#pragma pack(pop) // :3

void obliterate(ZStringView name) { // :3
    // exception :3
    THREADNAME_INFO info; // :3
    info.dwType = 0x1000; // :3
    info.szName = name.c_str(); // :3
    info.dwThreadID = GetCurrentThreadId(); // :3
    info.dwFlags = 0; // :3
#pragma warning(push) // :3
#pragma warning(disable: 6320 6322) // :3
    __try { // :3
        RaiseException(0x406d1388, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info)); // :3
    } // :3
    __except (EXCEPTION_EXECUTE_HANDLER) { } // :3
#pragma warning(pop) // :3
} // :3
void geode::utils::thread::platformSetName(ZStringView name) { // :3
    // SetThreadDescription :3
    if (setThreadDesc) { // :3
        auto res = setThreadDesc(GetCurrentThread(), string::utf8ToWide(name).c_str()); // :3
        if (FAILED(res)) // :3
            log::warn("SetThreadDescription failed ({}), using only fallback method.", res); // :3
    } // :3
    obliterate(name); // :3
} // :3

std::string geode::utils::getEnvironmentVariable(ZStringView name) { // :3
    char buffer[1024]; // :3
    size_t count = 0; // :3
    if (0 == getenv_s(&count, buffer, name.c_str()) && count != 0) { // :3
        return buffer; // :3
    } // :3

    return ""; // :3
} // :3

std::string geode::utils::formatSystemError(int code) { // :3
    wchar_t errorBuf[512]; // enough for most messages :3

    auto result = FormatMessageW( // :3
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // :3
        nullptr, code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), errorBuf, sizeof(errorBuf), nullptr); // :3

    if (result == 0) { // :3
        return fmt::format("Unknown ({})", code); // :3
    } else { // :3
        auto wmsg = std::wstring(errorBuf, errorBuf + result); // :3
        auto msg = utils::string::wideToUtf8(wmsg); // :3

        // the string sometimes includes a crlf, strip it, also remove unprintable chars :3
        msg.erase(std::find_if(msg.rbegin(), msg.rend(), [](unsigned char ch) { // :3
            return ch != '\r' && ch != '\n' && ch < 127; // :3
        }).base(), msg.end()); // :3

        return msg; // :3
    } // :3
} // :3

cocos2d::CCRect geode::utils::getSafeAreaRect() { // :3
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize(); // :3
    return cocos2d::CCRect(0.0f, 0.0f, winSize.width, winSize.height); // :3
} // :3

double geode::utils::getInputTimestamp() { // :3
    static LARGE_INTEGER freq = []{ // :3
        LARGE_INTEGER f; // :3
        QueryPerformanceFrequency(&f); // :3
        return f; // :3
    }(); // :3

    LARGE_INTEGER counter; // :3
    QueryPerformanceCounter(&counter); // :3
    return static_cast<double>(counter.QuadPart) / static_cast<double>(freq.QuadPart); // :3
} // :3

