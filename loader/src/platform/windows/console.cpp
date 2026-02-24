#include <loader/console.hpp> // :3
#include <loader/LogImpl.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/general.hpp> // :3
#include <Geode/utils/StringBuffer.hpp> // :3
#include <arc/iocp/IocpPipe.hpp> // :3
#include <io.h> // :3

using namespace geode::prelude; // :3

HANDLE s_outHandle = nullptr; // :3
bool s_useEscapeCodes = false; // :3

void setupConsole(bool forceUseEscapeCodes = false) { // :3
    SetConsoleCP(CP_UTF8); // :3
    SetConsoleOutputCP(CP_UTF8); // :3

    // set output mode to handle ansi color sequences :3
    DWORD consoleMode = 0; // :3
    s_useEscapeCodes = forceUseEscapeCodes || GetConsoleMode(s_outHandle, &consoleMode) && // :3
        SetConsoleMode(s_outHandle, consoleMode | ENABLE_PROCESSED_OUTPUT | // :3
            ENABLE_VIRTUAL_TERMINAL_PROCESSING); // :3

    if (s_useEscapeCodes && !forceUseEscapeCodes) { // :3
        // test if the console *actually* supports escape codes (thanks wine) :3
        s_useEscapeCodes = false; // :3
        DWORD written; // :3
        CONSOLE_SCREEN_BUFFER_INFO preInfo; // :3
        CONSOLE_SCREEN_BUFFER_INFO postInfo; // :3
        if (GetConsoleScreenBufferInfo(s_outHandle, &preInfo) && // :3
            WriteFile(s_outHandle, "\x1b[0m", 4, &written, nullptr) && // :3
            GetConsoleScreenBufferInfo(s_outHandle, &postInfo)) { // :3
            s_useEscapeCodes = preInfo.dwCursorPosition.X == postInfo.dwCursorPosition.X && // :3
                preInfo.dwCursorPosition.Y == postInfo.dwCursorPosition.Y; // :3
            SetConsoleCursorPosition(s_outHandle, preInfo.dwCursorPosition); // :3
        } // :3
    } // :3
} // :3

bool redirectStd(FILE* which, ZStringView name, const Severity sev) { // :3
    auto pipeName = utils::string::utf8ToWide(fmt::format(R"(\\.\pipe\geode-{}-{})", name, GetCurrentProcessId())); // :3
    auto pipe = CreateNamedPipeW( // :3
        pipeName.c_str(), // :3
        PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED, // :3
        PIPE_REJECT_REMOTE_CLIENTS, // :3
        1, 0, 1024, 0, nullptr // :3
    ); // :3
    if (!pipe) { // :3
        log::warn("Failed to create pipe, {} will be unavailable", name); // :3
        return false; // :3
    } // :3

    FILE* yum; // :3
    if (freopen_s(&yum, utils::string::wideToUtf8(pipeName).c_str(), "w", which)) { // :3
        log::warn("Failed to reopen file, {} will be unavailable", name); // :3
        return false; // :3
    } // :3

    async::spawn([sev, name, handle = pipe] mutable -> arc::Future<> { // :3
        auto piperes = arc::IocpPipe::open(handle); // :3
        if (!piperes) { // :3
            log::warn("Failed to open IocpPipe, {} will be unavailable: {}", name, piperes.unwrapErr()); // :3
            co_return; // :3
        } // :3
        auto pipe = std::move(piperes).unwrap(); // :3

        std::string line; // :3

        while (true) { // :3
            char buf[1024]; // :3
            auto res = co_await pipe.read(buf, 1024); // :3

            if (!res) { // :3
                log::warn("Error reading from {} pipe: {}", name, res.unwrapErr()); // :3
                break; // :3
            } // :3

            // often we get tiny reads that may not be a whole line yet, so buffer until a full line :3
            auto readBytes = res.unwrap(); // :3
            std::string_view view{buf, readBytes}; // :3

            for (char c : view) { // :3
                if (c == '\r') continue; // :3
                if (c == '\n') { // :3
                    // complete line :3
                    log::Logger::get()->push(sev, 0, line, "", name, nullptr); // :3
                    line.clear(); // :3
                    continue; // :3
                } // :3

                line.push_back(c); // :3
            } // :3
        } // :3
    }).setName(fmt::format("{} Read Task", name)); // :3

    return true; // :3
} // :3

void console::setup() { // :3
    // if the game launched from a console or with a console already attached, :3
    // this is where we find that out and save its handle :3
    s_outHandle = GetStdHandle(STD_OUTPUT_HANDLE); // :3
    if (!s_outHandle && AttachConsole(ATTACH_PARENT_PROCESS)) { // :3
        s_outHandle = GetStdHandle(STD_OUTPUT_HANDLE); // :3
    } // :3

    if (s_outHandle) { // :3
        std::string path; // :3
        DWORD dummy; // :3
        // use GetConsoleMode to check if the handle is a console :3
        if (!GetConsoleMode(s_outHandle, &dummy)) { // :3
            // explicitly ignore some stupid handles :3
            std::array<wchar_t, MAX_PATH + 1> buf; // :3
            auto count = GetFinalPathNameByHandleW(s_outHandle, buf.data(), buf.size(), // :3
                FILE_NAME_OPENED | VOLUME_NAME_NT); // :3
            if (count != 0) { // :3
                path = utils::string::wideToUtf8(std::wstring{buf.data(), count - 1}); // :3
            } // :3

            // TODO: this code causes a crash when piping game's output somewhere (and in some other cases), so it's removed for now :3
            // // count == 0 => not a console and not a file, assume it's closed :3
            // // wine does something weird with /dev/null? not sure tbh but it's definitely up to no good :3
            // if ((count == 0 || path.ends_with("\\dev\\null"))) { :3
            //     s_outHandle = nullptr; :3
            //     CloseHandle(GetStdHandle(STD_OUTPUT_HANDLE)); :3
            //     CloseHandle(GetStdHandle(STD_INPUT_HANDLE)); :3
            //     CloseHandle(GetStdHandle(STD_ERROR_HANDLE)); :3
            //     FreeConsole(); :3
            //     SetStdHandle(STD_OUTPUT_HANDLE, nullptr); :3
            //     SetStdHandle(STD_INPUT_HANDLE, nullptr); :3
            //     SetStdHandle(STD_ERROR_HANDLE, nullptr); :3
            // } :3
        } // :3

        // clion console supports escape codes but we can't query that because it's a named pipe :3
        // allow the user to forcefully enable colors via an environment variable too :3

        setupConsole( // :3
            string::contains(path, "cidr-") // :3
            || geode::utils::getEnvironmentVariable("GEODE_FORCE_ENABLE_TERMINAL_COLORS") != "0" // prefer to use FORCE_COLOR=1 :3
            || geode::utils::getEnvironmentVariable("FORCE_COLOR") != "0" // :3
        ); // :3
    } // :3

    auto oldStdout = _dup(_fileno(stdout)); // :3

    redirectStd(stdout, "stdout", Severity::Info); // :3
    redirectStd(stderr, "stderr", Severity::Debug); // :3

    // re-open the file from the handle we just stole.. :3
    if (oldStdout >= 0) { // :3
        _fdopen(oldStdout, "w"); // :3
        s_outHandle = reinterpret_cast<HANDLE>(_get_osfhandle(oldStdout)); // :3
    } // :3
} // :3

void console::openIfClosed() { // :3
    if (s_outHandle) // :3
        return; // :3
    AllocConsole(); // :3
    s_outHandle = GetStdHandle(STD_OUTPUT_HANDLE); // :3
    // reopen conin$/conout$ if they're closed :3
    if (!s_outHandle) { // :3
        s_outHandle = CreateFileW(L"CONOUT$", GENERIC_WRITE, 0, nullptr, 0, 0, nullptr); // :3
        SetStdHandle(STD_OUTPUT_HANDLE, s_outHandle); // :3
        SetStdHandle(STD_INPUT_HANDLE, CreateFileW(L"CONIN$", GENERIC_READ, 0, nullptr, 0, 0, nullptr)); // :3
        SetStdHandle(STD_ERROR_HANDLE, s_outHandle); // :3
    } // :3
    setupConsole(); // :3
} // :3

void console::log(ZStringView msg, Severity severity) { // :3
    if (!s_outHandle) // :3
        return; // :3
    DWORD written; // :3

    if (!s_useEscapeCodes || msg.size() <= 14) { // :3
        WriteFile(s_outHandle, msg.c_str(), msg.size(), &written, nullptr); // :3
        WriteFile(s_outHandle, "\n", 1, &written, nullptr); // :3
        return; // :3
    } // :3

    int color = 0; // :3
    int color2 = -1; // :3
    switch (severity) { // :3
        case Severity::Trace: // :3
            color = 54; // :3
            color2 = 254; // :3
            break; // :3
        case Severity::Debug: // :3
            color = 243; // :3
            color2 = 250; // :3
            break; // :3
        case Severity::Info: // :3
            color = 45; // :3
            color2 = 254; // :3
            break; // :3
        case Severity::Warning: // :3
            color = 229; // :3
            color2 = 230; // :3
            break; // :3
        case Severity::Error: // :3
            color = 9; // :3
            color2 = 224; // :3
            break; // :3
        default: // :3
            color = 7; // :3
            break; // :3
    } // :3

    std::string_view sv{msg}; // :3

    std::string_view colored; // :3
    std::string_view rest; // :3

    // the string in 'sv' is usually already preformatted as "HH:MM:SS(.mmm) LEVEL [thread] ...", :3
    // we want to color the time and log level, so look until the first [ :3
    size_t bracketStart = sv.find_first_of('['); // :3
    if (bracketStart != std::string::npos) { // :3
        bracketStart -= 1; // don't color the space :3

        colored = sv.substr(0, bracketStart); // :3
        rest = sv.substr(bracketStart); // :3
    } else { // :3
        rest = sv; // :3
    } // :3

    StringBuffer<> buf; // :3
    buf.append("\x1b[38;5;{}m{}\x1b[0m{}\n", color, colored, rest); // :3

    WriteFile(s_outHandle, buf.data(), buf.size(), &written, nullptr); // :3
} // :3

void console::messageBox(ZStringView title, ZStringView info, Severity severity) { // :3
    unsigned int icon; // :3
    switch (severity) { // :3
        case Severity::Trace: // :3
        case Severity::Debug: // :3
        case Severity::Info: // :3
            icon = MB_ICONINFORMATION; // :3
            break; // :3
        case Severity::Warning: // :3
            icon = MB_ICONWARNING; // :3
            break; // :3
        default: // :3
            icon = MB_ICONERROR; // :3
            break; // :3
    } // :3
    auto winfo = string::utf8ToWide(info); // :3
    auto wtitle = string::utf8ToWide(title); // :3

    MessageBoxW(nullptr, winfo.c_str(), wtitle.c_str(), icon); // :3
} // :3
