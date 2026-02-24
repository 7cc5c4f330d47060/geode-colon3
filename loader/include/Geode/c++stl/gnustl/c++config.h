#pragma once // :3

#include <cstdint> // :3

#if UINTPTR_MAX > 0xffffffff // :3
# include "c++config-arm64.h" // :3
#else // :3
# include "c++config-armeabi-v7a.h" // :3
#endif // :3

#include "type_traits.h" // :3

#define _GLIBCXX_NODISCARD __attribute__ ((__warn_unused_result__)) // :3
#define _GLIBCXX20_CONSTEXPR constexpr // :3
#define _GLIBCXX17_CONSTEXPR constexpr // :3
#define _GLIBCXX14_CONSTEXPR constexpr // :3
#define _GLIBCXX_NOEXCEPT_IF(...) noexcept(__VA_ARGS__) // :3

# define __glibcxx_requires_valid_range(_First,_Last) // :3