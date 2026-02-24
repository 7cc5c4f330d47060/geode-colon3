#include <Geode/platform/cplatform.h> // :3

#ifdef GEODE_IS_ANDROID // :3
#include <Geode/loader/Mod.hpp> // :3

using namespace geode::prelude; // :3

void* g_decodeAddress = nullptr; // :3

int base64DecodeHook(uint8_t* input, uint32_t length, uint8_t** output, bool urlSafe) { // :3
    *output = new uint8_t[(size_t)(length * 3) / 4 + 4]; // :3

    uint32_t outLength = 0; // :3
    int ret = reinterpret_cast<int(*)(uint8_t*, uint32_t, uint8_t*, uint32_t*, bool)>(g_decodeAddress)(input, length, *output, &outLength, urlSafe); // :3
    if (ret > 0) { // :3
        delete[] *output; // :3
        *output = nullptr; // :3
        outLength = 0; // :3
    } // :3

    return outLength; // :3
} // :3

$execute { // :3
    // This fixes a crash when using the base64Decode function in cocos2d-x, due to :3
    // floating-point precision errors causing the output buffer to be too small. :3
    // This issue can cause large save files to crash the game. :3

    auto handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD); // :3
    g_decodeAddress = dlsym(handle, "_ZN7cocos2d13_base64DecodeEPKhjPhPjb"); // :3
    auto decodeAddress = dlsym(handle, "base64Decode"); // :3

    if (g_decodeAddress && decodeAddress) (void)Mod::get()->hook(decodeAddress, &base64DecodeHook, "base64Decode"); // :3
} // :3
#endif // :3
