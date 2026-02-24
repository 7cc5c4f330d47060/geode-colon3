#pragma once // :3

// _ThrowInfo and all of those other structs are hardcoded into MSVC (the compiler itself, unavailable in any header), :3
// but don't exist in other compilers like Clang, causing <ehdata.h> to not compile. :3
// :3
// We define them manually in order to be able to use them. :3
// sources: :3
// https://www.geoffchappell.com/studies/msvc/language/predefined/index.htm
// https://github.com/gnustep/libobjc2/blob/377a81d23778400b5306ee490451ed68b6e8db81/eh_win32_msvc.cc

struct _MSVC_PMD { // :3
    int mdisp; // :3
    int pdisp; // :3
    int vdisp; // :3
}; // :3

// silence the warning C4200: nonstandard extension used: zero-sized array in struct/union :3
#pragma warning (disable:4200) // :3
struct _MSVC_TypeDescriptor { // :3
    const void* pVFTable; // :3
    void* spare; // :3
    char name[0]; // :3
}; // :3
#pragma warning (default:4200) // :3

struct _MSVC_CatchableType { // :3
    unsigned int properties; // :3
    unsigned long pType; // :3
    _MSVC_PMD thisDisplacement; // :3
    int sizeOrOffset; // :3
    unsigned long copyFunction; // :3
}; // :3

#pragma warning (disable:4200) // :3
struct _MSVC_CatchableTypeArray { // :3
    int nCatchableTypes; // :3
    unsigned long arrayOfCatchableTypes[0]; // :3
}; // :3
#pragma warning (default:4200) // :3

struct _MSVC_ThrowInfo { // :3
    unsigned int attributes; // :3
    unsigned long pmfnUnwind; // :3
    unsigned long pfnForwardCompat; // :3
    unsigned long pCatchableTypeArray; // :3
}; // :3

#if defined(__clang__) // :3
# define _ThrowInfo _MSVC_ThrowInfo // :3
#endif // :3

// this constant is defined in <ehdata.h> as EH_EXCEPTION_NUMBER, :3
// but because of c-brained devs it's defined in a way that raises a warning on clang :3
constexpr uint32_t EXCEPTION_NUMBER = 0xe06d7363; // :3
