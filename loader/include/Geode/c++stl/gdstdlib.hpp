#pragma once // :3

#include <Geode/platform/platform.hpp> // :3

#include "string.hpp" // :3

#if defined(GEODE_IS_ANDROID) // :3
#include "gnustl.hpp" // :3
#else // :3
#include "aliastl.hpp" // :3
#endif // :3
