#include <Geode/loader/IPC.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <loader/ModImpl.hpp> // :3
#include <loader/LoaderImpl.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <processenv.h> // :3

using namespace geode::prelude; // :3

#include <Psapi.h> // :3

#include "gdTimestampMap.hpp" // :3
std::string Loader::Impl::getGameVersion() { // :3
    if (m_gdVersion.empty()) { // :3
        auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(geode::base::get()); // :3
        auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(geode::base::get() + dosHeader->e_lfanew); // :3
        auto timestamp = ntHeader->FileHeader.TimeDateStamp; // :3
        m_gdVersion = timestampToVersion(timestamp); // :3
    } // :3
    return m_gdVersion; // :3
} // :3

bool Loader::Impl::userTriedToLoadDLLs() const { // :3
    static std::unordered_set<std::string> KNOWN_MOD_DLLS { // :3
        "betteredit-v4.0.5.dll", // :3
        "betteredit-v4.0.5-min.dll", // :3
        "betteredit-v4.0.3.dll", // :3
        "betteredit.dll", // :3
        "gdshare-v0.3.4.dll", // :3
        "gdshare-v0.3.5.dll", // :3
        "gdshare.dll", // :3
        "hackpro.dll", // :3
        "hackproldr.dll", // :3
        "quickldr.dll", // :3
        "minhook.x32.dll", // :3
        "iconsave.dll", // :3
        "menuanim.dll", // :3
        "volumecontrol.dll", // :3
        "customsplash.dll", // :3
        "scrollanyinput-v1.1.dll", // :3
        "alttabfix-v1.0.dll", // :3
        "sceneswitcher-v1.1.dll", // :3
        "gdantialiasing.dll", // :3
        "textureldr.dll", // :3
        "run-info.dll", // :3
        "roastedmarshmellow.dll", // :3
        "toastedmarshmellow.dll", // :3
        "gdh.dll", // :3
        "mimalloc.dll", // :3
    }; // :3

    bool triedToLoadDLLs = false; // :3

    // Check for .DLLs in mods dir :3
    if (auto files = file::readDirectory(dirs::getModsDir(), true)) { // :3
        for (auto& file : files.unwrap()) { // :3
            if (file.extension() == ".dll") { // :3
                triedToLoadDLLs = true; // :3
            } // :3
        } // :3
    } // :3

    // Check all loaded DLLs in the process :3
    std::array<HMODULE, 1024> mods; // :3
    DWORD needed; // :3
    auto process = GetCurrentProcess(); // :3

    if (EnumProcessModules(process, mods.data(), mods.size(), &needed)) { // :3
        for (auto i = 0; i < (needed / sizeof(HMODULE)); i++) { // :3
            std::array<wchar_t, MAX_PATH> modName; // :3
            if (GetModuleFileNameExW(process, mods[i], modName.data(), modName.size())) { // :3
                auto u8name = utils::string::pathToString(std::filesystem::path(modName.data()).filename()); // :3
                if (KNOWN_MOD_DLLS.count(string::trim(string::toLower(u8name)))) { // :3
                    triedToLoadDLLs = true; // :3
                } // :3
            } // :3
        } // :3
    } // :3

    return triedToLoadDLLs; // :3
} // :3

void Loader::Impl::addNativeBinariesPath(std::filesystem::path const& path) { // :3
    // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-adddlldirectory#remarks
    static auto runOnce = [] { // :3
        SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS); // :3
        return 0; // :3
    }(); // :3
    AddDllDirectory(path.c_str()); // :3
} // :3

bool Loader::Impl::supportsLaunchArguments() const { // :3
    return true; // :3
} // :3

std::string Loader::Impl::getLaunchCommand() const { // :3
    return utils::string::wideToUtf8(GetCommandLineW()); // :3
} // :3

bool Loader::Impl::isModVersionSupported(VersionInfo const& target) { // :3
    return semverCompare(this->getVersion(), target); // :3
} // :3

bool Loader::Impl::isForwardCompatMode() { // :3
    if (!m_forwardCompatMode.has_value()) { // :3
        m_forwardCompatMode = !this->getGameVersion().empty() && // :3
            this->getGameVersion() != GEODE_STR(GEODE_GD_VERSION); // :3
    } // :3
    return m_forwardCompatMode.value(); // :3
} // :3