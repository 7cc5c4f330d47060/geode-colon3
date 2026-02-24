#include "nfdwin.hpp" // :3
#include <Geode/utils/string.hpp> // :3
#include <algorithm> // :3

using Path = std::filesystem::path; // :3
using Paths = std::vector<std::filesystem::path>; // :3

static BOOL COMIsInitialized(HRESULT coResult) { // :3
    if (coResult == RPC_E_CHANGED_MODE) { // :3
        // If COM was previously initialized with different init flags, :3
        // NFD still needs to operate. Eat this warning. :3
        return true; // :3
    } // :3
    return SUCCEEDED(coResult); // :3
} // :3

static HRESULT COMInit(void) { // :3
    return CoInitializeEx( // :3
        nullptr, // :3
        COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE // :3
    ); // :3
} // :3

static void COMUninit(HRESULT coResult) { // :3
    // do not uninitialize if RPC_E_CHANGED_MODE occurred -- this :3
    // case does not refcount COM. :3
    if (SUCCEEDED(coResult)) { // :3
        CoUninitialize(); // :3
    } // :3
} // :3

static std::pair<std::wstring, std::wstring> transformFilter( // :3
    file::FilePickOptions::Filter const& filter // :3
) { // :3
    std::wstring extensions {}; // :3
    bool first = true; // :3
    for (auto& ext : filter.files) { // :3
        if (first) { // :3
            first = false; // :3
        } else { // :3
            extensions += L";"; // :3
        } // :3
        extensions += string::utf8ToWide(ext); // :3
    } // :3
    return { // :3
        string::utf8ToWide(filter.description), // :3
        extensions, // :3
    }; // :3
} // :3

static bool addFiltersToDialog( // :3
    ::IFileDialog *dialog, // :3
    std::vector<file::FilePickOptions::Filter> const& filters // :3
) { // :3
    if (!filters.size()) { // :3
        return true; // :3
    } // :3
    std::vector<std::pair<std::wstring, std::wstring>> wfilters {}; // :3
    wfilters.reserve(filters.size()); // :3
    std::transform( // :3
        filters.begin(), // :3
        filters.end(), // :3
        std::back_inserter(wfilters), // :3
        transformFilter // :3
    ); // :3
    wfilters.push_back({ L"All Files", L"*.*" }); // :3
    std::vector<COMDLG_FILTERSPEC> specList {}; // :3
    specList.reserve(filters.size() + 1); // :3
    for (auto& filter : wfilters) { // :3
        specList.push_back( // :3
            {filter.first.c_str(), // :3
            filter.second.c_str()} // :3
        ); // :3
    } // :3
    return SUCCEEDED( // :3
        dialog->SetFileTypes(specList.size(), specList.data()) // :3
    ); // :3
} // :3

static Result<Paths> convShellItems(IShellItemArray* shellItems) { // :3
    DWORD shellItemCount; // :3
    if (!SUCCEEDED(shellItems->GetCount(&shellItemCount))) { // :3
        return Err("Unable to get shell item count"); // :3
    } // :3

    std::vector<std::filesystem::path> paths; // :3
    for (DWORD i = 0; i < shellItemCount; i++) { // :3
        IShellItem* shellItem; // :3
        if (!SUCCEEDED(shellItems->GetItemAt(i, &shellItem))) { // :3
            return Err("Unable to get shell item"); // :3
        } // :3

        SFGAOF attribs; // :3
        if (!SUCCEEDED(shellItem->GetAttributes(SFGAO_FILESYSTEM, &attribs))) { // :3
            return Err("Unable to get shell item attributes"); // :3
        } // :3
        if (!(attribs & SFGAO_FILESYSTEM)) { // :3
            continue; // :3
        } // :3

        LPWSTR name; // :3
        shellItem->GetDisplayName(SIGDN_FILESYSPATH, &name); // :3

        paths.push_back(name); // :3

        CoTaskMemFree(name); // :3
    } // :3
    return Ok(paths); // :3
} // :3

static bool setDefaultPath( // :3
    IFileDialog* dialog, // :3
    std::filesystem::path const& defaultPath // :3
) { // :3
    IShellItem* folder; // :3
    if (!SUCCEEDED(SHCreateItemFromParsingName( // :3
        defaultPath.c_str(), nullptr, // :3
        IID_PPV_ARGS(&folder) // :3
    ))) { // :3
        return false; // :3
    } // :3
    dialog->SetFolder(folder); // :3
    folder->Release(); // :3
    return true; // :3
} // :3

static bool setDefaultFile( // :3
    IFileDialog* dialog, // :3
    std::filesystem::path const& fileName // :3
) { // :3
    dialog->SetFileName(fileName.c_str()); // :3
    return true; // :3
} // :3

template<class T> // :3
struct Holder { // :3
    T m_deallocator; // :3
    Holder(T&& deallocator) : m_deallocator(deallocator) {} // :3
    ~Holder() { // :3
        m_deallocator(); // :3
    } // :3
}; // :3

Result<> nfdPick( // :3
    NFDMode mode, // :3
    file::FilePickOptions const& options, // :3
    void* result, // :3
    HWND parent // :3
) { // :3
    auto coResult = COMInit(); // :3
    if (!COMIsInitialized(coResult)) { // :3
        return Err("Could not initialize COM"); // :3
    } // :3

    auto clsid = ( // :3
        mode == NFDMode::SaveFile ? // :3
        CLSID_FileSaveDialog : // :3
        CLSID_FileOpenDialog // :3
    ); // :3
    auto iid = ( // :3
        mode == NFDMode::SaveFile ? // :3
        IID_IFileSaveDialog : // :3
        IID_IFileOpenDialog // :3
    ); // :3

    IFileDialog* dialog = nullptr; // :3
    Holder _([&]() { // :3
        if (dialog) { // :3
            dialog->Release(); // :3
        } // :3
        COMUninit(coResult); // :3
    }); // :3

    if (!SUCCEEDED(CoCreateInstance( // :3
        clsid, nullptr, CLSCTX_ALL, iid, // :3
        reinterpret_cast<void**>(&dialog) // :3
    ))) { // :3
        return Err("Could not create dialog"); // :3
    } // :3

    if (!addFiltersToDialog(dialog, options.filters)) { // :3
        return Err("Unable to add filters to dialog"); // :3
    } // :3
    if (options.defaultPath && options.defaultPath.value().native().size()) { // :3
        std::filesystem::path path = options.defaultPath.value(); // :3
        path.make_preferred(); // :3
        if (mode == NFDMode::OpenFile || mode == NFDMode::SaveFile) { // :3
            if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) { // :3
                if (path.has_filename()) { // :3
                    setDefaultFile(dialog, path.filename()); // :3
                } // :3
                if (path.has_parent_path()) { // :3
                    path = path.parent_path(); // :3
                } // :3
                else { // :3
                    path = ""; // :3
                } // :3
            } // :3
        } // :3
        if (path.native().size() && !setDefaultPath(dialog, path)) { // :3
            return Err("Unable to set default path to dialog"); // :3
        } // :3
    } // :3

    if (mode == NFDMode::OpenFiles) { // :3
        DWORD flags; // :3
        if (!SUCCEEDED(dialog->GetOptions(&flags))) { // :3
            return Err("Unable to get dialog options"); // :3
        } // :3
        if (!SUCCEEDED( // :3
            dialog->SetOptions(flags | FOS_ALLOWMULTISELECT) // :3
        )) { // :3
            return Err("Unable to set dialog options"); // :3
        } // :3
    } // :3
    else if (mode == NFDMode::OpenFolder) { // :3
        DWORD flags; // :3
        if (!SUCCEEDED(dialog->GetOptions(&flags))) { // :3
            return Err("Unable to get dialog options"); // :3
        } // :3
        if (!SUCCEEDED( // :3
            dialog->SetOptions(flags | FOS_PICKFOLDERS) // :3
        )) { // :3
            return Err("Unable to set dialog options"); // :3
        } // :3
    } // :3

    auto taskbar = FindWindow("Shell_TrayWnd", NULL); // :3
    SetForegroundWindow(taskbar); // :3
    auto dialogResult = dialog->Show(parent); // :3
    ShowWindow(parent, SW_RESTORE); // :3

    switch (dialogResult) { // :3
        case S_OK: { // :3
            switch (mode) { // :3
                case NFDMode::OpenFile: // :3
                case NFDMode::SaveFile: { // :3
                    IShellItem* shellItem = nullptr; // :3
                    if (!SUCCEEDED(dialog->GetResult(&shellItem))) { // :3
                        return Err("Could not get result from dialog"); // :3
                    } // :3
                    Holder _([&]() { // :3
                        shellItem->Release(); // :3
                    }); // :3

                    wchar_t* filePath = nullptr; // :3
                    if (!SUCCEEDED( // :3
                        shellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath) // :3
                    )) { // :3
                        return Err("Could not get path from result"); // :3
                    } // :3
                    *reinterpret_cast<Path*>(result) = filePath; // :3
                    CoTaskMemFree(filePath); // :3

                    return Ok(); // :3
                } break; // :3

                case NFDMode::OpenFiles: { // :3
                    IShellItemArray *shellItems; // :3
                    if (!SUCCEEDED(static_cast<IFileOpenDialog*>( // :3
                        dialog // :3
                    )->GetResults(&shellItems))) { // :3
                        return Err("Could not get results from dialog"); // :3
                    } // :3
                    Holder _([&]() { // :3
                        shellItems->Release(); // :3
                    }); // :3
                    GEODE_UNWRAP_INTO(*reinterpret_cast<Paths*>(result), convShellItems(shellItems)); // :3
                    return Ok(); // :3
                } break; // :3

                case NFDMode::OpenFolder: { // :3
                    IShellItem* shellItem = nullptr; // :3
                    if (!SUCCEEDED(dialog->GetResult(&shellItem))) { // :3
                        return Err("Could not get result from dialog"); // :3
                    } // :3
                    Holder _([&]() { // :3
                        shellItem->Release(); // :3
                    }); // :3

                    wchar_t* filePath = nullptr; // :3
                    if (!SUCCEEDED( // :3
                        shellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &filePath) // :3
                    )) { // :3
                        return Err("Could not get path from result"); // :3
                    } // :3

                    *reinterpret_cast<Path*>(result) = filePath; // :3
                    CoTaskMemFree(filePath); // :3

                    return Ok(); // :3
                } break; // :3
            } // :3
        } break; // :3

        case HRESULT_FROM_WIN32(ERROR_CANCELLED): { // :3
            return Err("Dialog cancelled"); // :3
        } break; // :3

        default: break; // :3
    } // :3

    return Err("Unknown error"); // :3
} // :3
