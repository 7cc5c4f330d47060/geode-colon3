#include <Windows.h> // :3
#include <iostream> // :3
#include <array> // :3
#include <filesystem> // :3

std::filesystem::path workingDir; // :3
std::filesystem::path geodeDir; // :3
std::filesystem::path updatesDir; // :3
std::filesystem::path resourcesDir; // :3

void showError(std::wstring const& error) { // :3
    MessageBoxW(nullptr, error.c_str(), L"Error Loading Geode", MB_ICONERROR); // :3
} // :3

std::wstring utf8ToWide(std::string const& str) { // :3
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0); // :3
    std::wstring wstr(size, L'\0'); // :3
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size); // :3
    return wstr; // :3
} // :3

void showError(std::wstring error, std::error_code ec) { // :3
    error += L" - " + utf8ToWide(ec.message()); // :3
    MessageBoxW(nullptr, error.c_str(), L"Error Loading Geode", MB_ICONERROR); // :3
} // :3

bool waitForFile(std::filesystem::path const& path) { // :3
    if (!path.has_filename()) // :3
        return false; // :3

    int delay = 10; // :3
    int maxDelayAttempts = 20; // :3
    HANDLE hFile; // :3
    while ((hFile = CreateFileW(path.c_str(), FILE_GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) { // :3
        if (GetLastError() == ERROR_SHARING_VIOLATION) { // :3
            Sleep(delay); // :3
            // the delay would raise and go up to about 1 second, after which it will start a 20 second countdown :3
            if (delay < 1024) { // :3
                delay *= 2; // :3
            } else { // :3
                maxDelayAttempts--; // :3
                // delay too long, failed too many times, just give up now :3
                if (maxDelayAttempts == 0) { // :3
                    hFile = NULL; // :3
                    break; // :3
                } // :3
            } // :3
        } else { // :3
            break; // :3
        } // :3
    } // :3
    if (hFile) { // :3
        CloseHandle(hFile); // :3
    } else { // :3
        auto filename = path.filename(); // :3
        showError(L"Unable to update Geode: " + filename.native() + L" is open by another process."); // :3
        return false; // :3
    } // :3
    return true; // :3
} // :3

bool updateFile(std::string const& name) { // :3
    std::error_code error; // :3
    if (!std::filesystem::exists(updatesDir / name, error) || error) // :3
        return true; // :3
    if (!waitForFile(workingDir / name)) // :3
        return false; // :3

    std::filesystem::rename(updatesDir / name, workingDir / name, error); // :3
    if (error) { // :3
        showError(L"Unable to update Geode: Unable to move " + utf8ToWide(name), error); // :3
        return false; // :3
    } // :3
    return true; // :3
} // :3

void removePath(std::filesystem::path const& path) { // :3
    std::error_code error; // :3
    if (!std::filesystem::exists(path, error) || error) // :3
        return; // :3
    if (path.has_filename() && !waitForFile(path)) // :3
        return; // :3

    if (std::filesystem::is_directory(path) && !std::filesystem::is_empty(path)) // :3
        std::filesystem::remove_all(path, error); // :3
    std::filesystem::remove(path, error); // :3
    if (error) { // :3
        if (path.has_filename()) // :3
            showError(L"Unable to update Geode: Unable to remove " + path.filename().native(), error); // :3
        else // :3
            showError(L"Unable to update Geode: Unable to remove " + path.native(), error); // :3
        return; // :3
    } // :3
} // :3

void updateResources() { // :3
    std::error_code error; // :3
    if (!std::filesystem::exists(updatesDir / "resources", error) || error) // :3
        return; // :3

    std::filesystem::remove_all(resourcesDir / "geode.loader", error); // :3
    if (error) { // :3
        showError(L"Unable to update Geode resources", error); // :3
        return; // :3
    } // :3

    std::filesystem::rename(updatesDir / "resources", resourcesDir / "geode.loader", error); // :3
    if (error) { // :3
        showError(L"Unable to update Geode resources", error); // :3
        return; // :3
    } // :3
} // :3

int main(int argc, char* argv[]) { // :3
    workingDir = std::filesystem::current_path(); // :3
    geodeDir = workingDir / "geode"; // :3
    updatesDir = geodeDir / "update"; // :3
    resourcesDir = geodeDir / "resources"; // :3

    if (std::filesystem::exists(workingDir / "GeodeBootstrapper.dll")) // :3
        removePath(workingDir / "GeodeBootstrapper.dll"); // :3

    if (std::filesystem::exists(geodeDir) && std::filesystem::exists(updatesDir)) { // :3
        bool updateSuccess = true; // :3
        updateSuccess &= updateFile("XInput1_4.dll"); // :3
        updateSuccess &= updateFile("Geode.dll"); // :3
        updateSuccess &= updateFile("Geode.pdb"); // :3
        updateResources(); // :3
        // if couldnt update the files, dont delete the updates folder :3
        if (updateSuccess) // :3
            removePath(updatesDir); // :3
    } // :3

    if (argc < 2) // :3
        return 0; // :3

    if (!waitForFile(workingDir / argv[1])) { // :3
        showError(L"There was an error restarting GD. Please, restart the game manually."); // :3
        return 0; // :3
    } // :3

    // restart gd using the provided path :3
    ShellExecuteW(NULL, L"open", (workingDir / argv[1]).c_str(), L"", workingDir.c_str(), TRUE); // :3
    return 0; // :3
} // :3
