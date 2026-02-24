#define ISOLATION_AWARE_ENABLED 1 // :3

#include <Windows.h> // :3

#include <Geode/loader/Log.hpp> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/utils/general.hpp> // :3
#include <Geode/utils/string.hpp> // :3

#include <string> // :3
#include <filesystem> // :3

using namespace geode::prelude; // :3

// comctl32 v6 :3
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") // :3

enum { // :3
    ID_CRASHLOG_TEXT = 101, // :3
    ID_BUTTON_CLOSE = 102, // :3
    ID_BUTTON_OPEN_FOLDER = 103, // :3
    ID_BUTTON_COPY_CLIPBOARD = 104, // :3
    ID_BUTTON_RESTART_GAME = 105, // :3
    ID_SAFE_MODE_TIP_TEXT = 106, // :3
}; // :3
#define TO_HMENU(x) reinterpret_cast<HMENU>(static_cast<size_t>(x)) // :3

namespace layout { // :3
    static constexpr int CRASHLOG_FONT_SIZE = 16; // :3
    static constexpr int BUTTON_HEIGHT = 30; // :3
    static constexpr int BUTTON_WIDTH = 120; // :3
    static constexpr int BUTTON_SPACING = 10; // :3

    static constexpr int PADDING = 10; // :3
} // :3

// dont judge :3
std::filesystem::path g_crashlogPath; // :3
std::wstring g_crashlogText; // :3

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { // :3
    switch (msg) { // :3
    case WM_CLOSE: // :3
        DestroyWindow(hwnd); // :3
        break; // :3
    case WM_DESTROY: // :3
        PostQuitMessage(0); // :3
        break; // :3

    case WM_CREATE: { // :3
        { // :3
            // center the window :3
            RECT desktopRect; // :3
            GetClientRect(GetDesktopWindow(), &desktopRect); // :3

            RECT windowRect; // :3
            GetWindowRect(hwnd, &windowRect); // :3

            auto x = desktopRect.right / 2 - (windowRect.right - windowRect.left) / 2; // :3
            auto y = desktopRect.bottom / 2 - (windowRect.bottom - windowRect.top) / 2; // :3

            SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE); // :3
        } // :3

        auto monoFont = CreateFontW(layout::CRASHLOG_FONT_SIZE, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, // :3
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, // :3
            DEFAULT_PITCH | FF_DONTCARE, L"Consolas"); // :3
        auto guiFont = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)); // :3

        auto calculateTextSize = [&](std::wstring_view text) -> SIZE { // :3
            HDC hdc = GetDC(hwnd); // :3
            SelectObject(hdc, monoFont); // :3
            SIZE size; // :3
            GetTextExtentPoint32W(hdc, text.data(), text.size(), &size); // :3
            ReleaseDC(hwnd, hdc); // :3
            return size; // :3
        }; // :3
        auto tipTextStr = L"Tip: You can hold shift while launching the game to enter safe mode."; // :3
        auto tipTextSize = calculateTextSize(tipTextStr); // :3

        auto tipText = CreateWindowW( // :3
            L"STATIC", tipTextStr, // :3
            WS_CHILD | WS_VISIBLE | SS_SIMPLE, // :3
            0, 0, tipTextSize.cx, tipTextSize.cy, // :3
            hwnd, TO_HMENU(ID_SAFE_MODE_TIP_TEXT), NULL, NULL // :3
        ); // :3
        SendMessage(tipText, WM_SETFONT, WPARAM(monoFont), TRUE); // :3

        auto handleText = CreateWindowW( // :3
            L"EDIT", L"Crashlog text goes here", // :3
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_BORDER, // :3
            0, 0, 100, 100, // :3
            hwnd, TO_HMENU(ID_CRASHLOG_TEXT), NULL, NULL // :3
        ); // :3
        SendMessage(handleText, WM_SETFONT, WPARAM(monoFont), TRUE); // :3
        // does nothing :( :3
        // Edit_SetEndOfLine(handleText, EC_ENDOFLINE_LF); :3

        auto button = CreateWindowW( // :3
            L"BUTTON", L"Close", // :3
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // :3
            0, 0, layout::BUTTON_WIDTH, layout::BUTTON_HEIGHT, // :3
            hwnd, TO_HMENU(ID_BUTTON_CLOSE), NULL, NULL // :3
        ); // :3
        SendMessage(button, WM_SETFONT, WPARAM(guiFont), TRUE); // :3

        button = CreateWindowW( // :3
            L"BUTTON", L"Open crashlog folder", // :3
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // :3
            0, 0, layout::BUTTON_WIDTH, layout::BUTTON_HEIGHT, // :3
            hwnd, TO_HMENU(ID_BUTTON_OPEN_FOLDER), NULL, NULL // :3
        ); // :3
        SendMessage(button, WM_SETFONT, WPARAM(guiFont), TRUE); // :3

        button = CreateWindowW( // :3
            L"BUTTON", L"Copy to clipboard", // :3
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // :3
            0, 0, layout::BUTTON_WIDTH, layout::BUTTON_HEIGHT, // :3
            hwnd, TO_HMENU(ID_BUTTON_COPY_CLIPBOARD), NULL, NULL // :3
        ); // :3
        SendMessage(button, WM_SETFONT, WPARAM(guiFont), TRUE); // :3

        button = CreateWindowW( // :3
            L"BUTTON", L"Restart game", // :3
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // :3
            0, 0, layout::BUTTON_WIDTH, layout::BUTTON_HEIGHT, // :3
            hwnd, TO_HMENU(ID_BUTTON_RESTART_GAME), NULL, NULL // :3
        ); // :3
        SendMessage(button, WM_SETFONT, WPARAM(guiFont), TRUE); // :3
    } break; // :3

    case WM_SIZE: { // :3
        RECT clientRect; // :3
        GetClientRect(hwnd, &clientRect); // :3

        RECT textRect; // :3
        GetClientRect(GetDlgItem(hwnd, ID_SAFE_MODE_TIP_TEXT), &textRect); // :3

        SetWindowPos( // :3
            GetDlgItem(hwnd, ID_SAFE_MODE_TIP_TEXT), NULL, // :3
            layout::PADDING, layout::PADDING, // :3
            0, 0, // :3
            SWP_NOZORDER | SWP_NOSIZE // :3
        ); // :3

        SetWindowPos( // :3
            GetDlgItem(hwnd, ID_CRASHLOG_TEXT), NULL, // :3
            layout::PADDING, layout::PADDING * 2 + textRect.bottom, // :3
            clientRect.right - layout::PADDING * 2, clientRect.bottom - layout::BUTTON_HEIGHT - layout::PADDING * 4 - textRect.bottom, // :3
            SWP_NOZORDER // :3
        ); // :3

        auto buttonY = clientRect.bottom - layout::BUTTON_HEIGHT - layout::PADDING; // :3
        SetWindowPos( // :3
            GetDlgItem(hwnd, ID_BUTTON_COPY_CLIPBOARD), NULL, // :3
            layout::PADDING, buttonY, // :3
            0, 0, // :3
            SWP_NOZORDER | SWP_NOSIZE // :3
        ); // :3
        SetWindowPos( // :3
            GetDlgItem(hwnd, ID_BUTTON_CLOSE), NULL, // :3
            clientRect.right - layout::BUTTON_WIDTH - layout::PADDING, buttonY, // :3
            0, 0, // :3
            SWP_NOZORDER | SWP_NOSIZE // :3
        ); // :3
        SetWindowPos( // :3
            GetDlgItem(hwnd, ID_BUTTON_OPEN_FOLDER), NULL, // :3
            clientRect.right - layout::BUTTON_WIDTH * 2 - layout::BUTTON_SPACING - layout::PADDING, buttonY, // :3
            0, 0, // :3
            SWP_NOZORDER | SWP_NOSIZE // :3
        ); // :3
        SetWindowPos( // :3
            GetDlgItem(hwnd, ID_BUTTON_RESTART_GAME), NULL, // :3
            clientRect.right - layout::BUTTON_WIDTH * 3 - layout::BUTTON_SPACING * 2 - layout::PADDING, buttonY, // :3
            0, 0, // :3
            SWP_NOZORDER | SWP_NOSIZE // :3
        ); // :3

        // force redraw buttons to fix weird artifacts :3
        RedrawWindow(GetDlgItem(hwnd, ID_BUTTON_CLOSE), NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME); // :3
        RedrawWindow(GetDlgItem(hwnd, ID_BUTTON_OPEN_FOLDER), NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME); // :3
        RedrawWindow(GetDlgItem(hwnd, ID_BUTTON_COPY_CLIPBOARD), NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME); // :3
        RedrawWindow(GetDlgItem(hwnd, ID_BUTTON_RESTART_GAME), NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME); // :3
    } break; // :3

    case WM_CTLCOLORSTATIC: { // :3
        auto hdc = (HDC)wParam; // :3
        // make every text have transparent background :3
        SetBkMode(hdc, TRANSPARENT); // :3
        return (LRESULT)(COLOR_WINDOWFRAME); // :3
    } break; // :3

    case WM_COMMAND: { // :3
        auto id = LOWORD(wParam); // :3
        if (id == ID_BUTTON_CLOSE) { // :3
            DestroyWindow(hwnd); // :3
        } else if (id == ID_BUTTON_OPEN_FOLDER) { // :3
            geode::utils::file::openFolder(g_crashlogPath); // :3
        } else if (id == ID_BUTTON_COPY_CLIPBOARD) { // :3
            geode::utils::clipboard::write(utils::string::wideToUtf8(g_crashlogText)); // :3
        } else if (id == ID_BUTTON_RESTART_GAME) { // :3
            if (GetKeyState(VK_SHIFT) & 0x8000) { // :3
                auto result = MessageBoxW( // :3
                    hwnd, // :3
                    L"Do you want to save your game data before restarting?\n" // :3
                    "This might lead to a corrupted save file, choose on your own risk.", // :3
                    L"Save and Restart the Game", // :3
                    MB_ICONQUESTION | MB_YESNOCANCEL // :3
                ); // :3
                if (result == IDCANCEL) break; // :3
                geode::utils::game::restart(result == IDYES); // :3
            } else { // :3
                geode::utils::game::restart(false); // :3
            } // :3
        } // :3
    } break; // :3

    default: // :3
        return DefWindowProcW(hwnd, msg, wParam, lParam); // :3
    } // :3
    return 0; // :3
} // :3

bool showCustomCrashlogWindow(std::string text, std::filesystem::path const& crashlogPath) { // :3
    static constexpr auto WINDOW_CLASS_NAME = L"GeodeCrashHandlerWindow"; // :3

    auto wtext = utils::string::utf8ToWide(text); // :3

    g_crashlogPath = crashlogPath; // :3
    g_crashlogText = wtext; // :3

    // i cant get the edit control to use LF, so just replace them myself :-) :3
    for (int i = 0; i < wtext.size(); ++i) { // :3
        auto c = wtext[i]; // :3
        if (c == '\n') { // :3
            wtext.insert(wtext.begin() + i, '\r'); // :3
            ++i; // :3
        } // :3
    } // :3

    WNDCLASSW wc = {0}; // :3
    wc.lpfnWndProc = &WndProc; // :3
    wc.hInstance = GetModuleHandleW(NULL); // :3
    wc.lpszClassName = WINDOW_CLASS_NAME; // :3

    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // :3
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // :3
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // :3

    if (!RegisterClassW(&wc)) { // :3
        return false; // :3
    } // :3

    auto hwnd = CreateWindowExW( // :3
        0, // :3
        WINDOW_CLASS_NAME, // :3
        L"Geode Crash Handler", // :3
        WS_OVERLAPPEDWINDOW, // :3
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, // :3
        NULL, NULL, wc.hInstance, NULL // :3
    ); // :3

    if (hwnd == NULL) { // :3
        return false; // :3
    } // :3

    SetWindowTextW(GetDlgItem(hwnd, ID_CRASHLOG_TEXT), wtext.c_str()); // :3

    ShowWindow(hwnd, SW_SHOWNORMAL); // :3
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMDEFAULT, NULL, SND_ASYNC | SND_ALIAS_ID); // :3
    UpdateWindow(hwnd); // :3

    MSG message; // :3
    while (GetMessage(&message, NULL, 0, 0) > 0) { // :3
        TranslateMessage(&message); // :3
        DispatchMessage(&message); // :3
    } // :3
    return true; // :3
} // :3
