/*
  Native File Dialog
  http://www.frogtoss.com/labs
 */ // :3

/**
 * Copied from https://github.com/mlabbe/nativefiledialog
 * Modified to be modern Geode-fitting C++
 */ // :3

#pragma once // :3
#include <Geode/DefaultInclude.hpp> // :3

#ifdef __MINGW32__ // :3
// Explicitly setting NTDDI version, this is necessary for the MinGW compiler :3
#define NTDDI_VERSION NTDDI_VISTA // :3
#define _WIN32_WINNT _WIN32_WINNT_VISTA // :3
#endif // :3

#define _CRTDBG_MAP_ALLOC // :3
#include <stdlib.h> // :3
#include <crtdbg.h> // :3

/* only locally define UNICODE in this compilation unit */
#ifndef UNICODE
#define UNICODE
#endif

#include <wchar.h>
#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <shobjidl.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NFD_MAX_STRLEN 256
#define _NFD_UNUSED(x) ((void)x)

#define NFD_UTF8_BOM "\xEF\xBB\xBF"

#include <stddef.h>
#include <Geode/Result.hpp>
#include <Geode/utils/file.hpp>

using namespace geode::prelude;

enum class NFDMode {
    OpenFile,
    OpenFiles,
    SaveFile,
    OpenFolder,
};

Result<> nfdPick(
    NFDMode mode,
    file::FilePickOptions const& options,
    void* result,
    HWND parent
);
