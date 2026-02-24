#include <Windows.h> // :3
#include <string> // :3
#include <array> // :3
#include <vector> // :3
#include <filesystem> // :3

struct XINPUT_STATE; // :3
struct XINPUT_CAPABILITIES; // :3
struct XINPUT_VIBRATION; // :3

constexpr static auto MAX_PATH_CHARS = 32768u; // :3

static HMODULE getXInput() { // :3
    static auto xinput = []() -> HMODULE { // :3
        std::wstring path(MAX_PATH_CHARS, L'\0'); // :3
        auto size = GetSystemDirectoryW(path.data(), path.size()); // :3
        if (size) { // :3
            path.resize(size); // :3
            return LoadLibraryW((path + L"\\XInput1_4.dll").c_str()); // :3
        } // :3
        return NULL; // :3
    }(); // :3

    return xinput; // :3
} // :3

static FARPROC getFP(const std::string& sym) { // :3
    if (auto xinput = getXInput()) // :3
        return GetProcAddress(xinput, sym.c_str()); // :3

    return NULL; // :3
} // :3

#pragma comment(linker, "/export:XInputGetState,@2") // :3
extern "C" DWORD XInputGetState(DWORD dwUserIndex, XINPUT_STATE *pState) { // :3
    static auto fp = getFP("XInputGetState"); // :3
    if (fp) { // :3
        using FPType = decltype(&XInputGetState); // :3
        return reinterpret_cast<FPType>(fp)(dwUserIndex, pState); // :3
    } // :3

    return ERROR_DEVICE_NOT_CONNECTED; // :3
} // :3

#pragma comment(linker, "/export:XInputSetState,@3") // :3
extern "C" DWORD XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) { // :3
    static auto fp = getFP("XInputSetState"); // :3
    if (fp) { // :3
        using FPType = decltype(&XInputSetState); // :3
        return reinterpret_cast<FPType>(fp)(dwUserIndex, pVibration); // :3
    } // :3

    return ERROR_DEVICE_NOT_CONNECTED; // :3
} // :3

#pragma comment(linker, "/export:XInputGetCapabilities,@4") // :3
extern "C" DWORD XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES *pCapabilities) { // :3
    static auto fp = getFP("XInputGetCapabilities"); // :3
    if (fp) { // :3
        using FPType = decltype(&XInputGetCapabilities); // :3
        return reinterpret_cast<FPType>(fp)(dwUserIndex, dwFlags, pCapabilities); // :3
    } // :3

    return ERROR_DEVICE_NOT_CONNECTED; // :3
} // :3

static std::wstring getErrorString(DWORD error) { // :3
    return L"Could not load Geode! Error code: " + std::to_wstring(error); // :3
} // :3

static DWORD errorThread(LPVOID param) { // :3
    constexpr wchar_t REDIST_ERROR[] = L"Could not load Geode!\n" // :3
        "This is likely due to an outdated redist package.\n" // :3
        "Do you want to update Microsoft Visual C++ Redistributable 2022 to try to fix this issue?"; // :3
    constexpr wchar_t ALT_REDIST_ERROR[] = L"Could not load Geode!\n\n" // :3
        "Please **delete** the following files from your Geometry Dash directory and try again: "; // :3
    const DWORD error = reinterpret_cast<DWORD64>(param); // :3

    if (error == ERROR_DLL_INIT_FAILED) { // :3

        std::array<std::wstring, 4> msvcpDlls = { // :3
            L"msvcp140.dll", // :3
            L"msvcp140d.dll", // :3
            L"vcruntime140.dll", // :3
            L"vcruntime140d.dll" // :3
        }; // :3

        std::vector<std::wstring> foundDlls; // :3
        for(auto dll : msvcpDlls) { // :3
            if(std::filesystem::exists(dll)) { // :3
                foundDlls.push_back(dll); // :3
            } // :3
        } // :3

        if(foundDlls.empty()) { // :3
            const auto choice = MessageBoxW(NULL, REDIST_ERROR, L"Load failed", MB_YESNO | MB_ICONWARNING); // :3
            if (choice == IDYES) // :3
                ShellExecuteW(NULL, L"open", L"https://aka.ms/vs/17/release/vc_redist.x64.exe", NULL, NULL, SW_SHOWNORMAL);
        } else { // :3
            std::wstring files = ALT_REDIST_ERROR; // :3
            bool first = true; // :3
            for(auto dll : foundDlls) { // :3
                if(!first) files += L", "; // :3
                files += dll; // :3
                first = false; // :3
            } // :3
            const auto choice = MessageBoxW(NULL, files.c_str(), L"Load failed", MB_OK | MB_ICONWARNING); // :3
        } // :3

    } else { // :3
        MessageBoxW(NULL, getErrorString(error).c_str(), L"Load failed" , MB_OK | MB_ICONWARNING); // :3
    } // :3

    return 0u; // :3
} // :3

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) { // :3
    if (reason == DLL_PROCESS_ATTACH) { // :3
        DisableThreadLibraryCalls(module); // :3

        // This is UB. :3
        if (LoadLibraryW(L"Geode.dll") == NULL) { // :3
            const auto param = reinterpret_cast<LPVOID>(static_cast<DWORD64>(GetLastError())); // :3
            CreateThread(NULL, 0, &errorThread, param, 0, NULL); // :3
        } // :3
    } // :3

    return TRUE; // :3
} // :3