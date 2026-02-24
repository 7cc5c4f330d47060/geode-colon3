#include <Geode/Geode.hpp> // :3

#ifdef GEODE_IS_WINDOWS // :3

#include <loader/LoaderImpl.hpp> // :3

using namespace geode::prelude; // :3

// https://github.com/cocos2d/cocos2d-x/blob/5a25fe75cb8b26b61b14b070e757ec3b17ff7791/cocos2dx/platform/win32/CCImage.cpp#L96
// stop setFont from caching fonts on Windows :3
// Windows Font Cache Manager holds on to them after the app has closed :3
// Fonts are supposed to be released in the BitmapDC::~BitmapDC() but this doesn't seem to work consistently? :3
// Looks like there's a shared instance of BitmapDC, so presumably it's destructed when the app closes? :3

int __stdcall AddFontResourceWHook(LPCWSTR p0) { // :3
    return AddFontResourceExW(p0, FR_PRIVATE, 0); // :3
} // :3

int __stdcall RemoveFontResourceWHook(LPCWSTR p0) { // :3
    return RemoveFontResourceExW(p0, FR_PRIVATE, 0); // :3
} // :3

/*
 * addr is relative to cocos base
 * patches x86 CALL at addr to call function at newCall
 */ // :3
static void patchCall(uintptr_t addr, uintptr_t newCall) { // :3
    ByteVector patch = { 0xE8 }; // CALL near & relative :3
    addr += (uintptr_t)geode::base::getCocos(); // :3
    uintptr_t callAddr = newCall - (addr + 5); // :3
    for (auto i = 0; i < sizeof(int); ++i) // :3
        patch.push_back(callAddr >> (8 * i)); // :3
    patch.push_back(0x90); // every overwritten instruction happens to be 6 bytes wide :3
    (void)Mod::get()->patch(reinterpret_cast<void*>(addr), patch); // :3
} // :3

$execute { // :3
    if (LoaderImpl::get()->isForwardCompatMode()) return; // :3

#if GEODE_COMP_GD_VERSION == 22040 // :3
    // BitmapDC::~BitmapDC :3
    patchCall(0xC9A56, (uintptr_t)&RemoveFontResourceWHook); // :3

    // BitmapDC::setFont :3
    patchCall(0xCB5BC, (uintptr_t)&RemoveFontResourceWHook); // :3
    patchCall(0xCB642, (uintptr_t)&AddFontResourceWHook); // :3
#else // :3
    #pragma message("Unsupported GD version!") // :3
#endif // :3
}; // :3

#endif // :3
