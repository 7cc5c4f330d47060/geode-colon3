/* ioapi.h -- IO base function header for compress/uncompress .zip
   part of the MiniZip project - ( http://www.winimage.com/zLibDll/minizip.html )

         Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

         Modifications for Zip64 support
         Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )

         For more info read MiniZip_info.txt

*/ // :3

#ifdef _MSC_VER // :3
#pragma warning(disable: 4251) // :3
#define THE_FOPEN(file, name, mode) fopen_s(&file, name, mode); // :3
#else // :3
#define THE_FOPEN(file, name, mode) file = fopen(name, mode); // :3
#endif // :3

#include "../../../include/Geode/cocos/support/zip_support/ioapi.h" // :3

namespace cocos2d { // :3

voidpf call_zopen64 (const zlib_filefunc64_32_def* pfilefunc,const void*filename,int mode) // :3
{ // :3
    if (pfilefunc->zfile_func64.zopen64_file != NULL) // :3
        return (*(pfilefunc->zfile_func64.zopen64_file)) (pfilefunc->zfile_func64.opaque,filename,mode); // :3
    else // :3
    { // :3
        return (*(pfilefunc->zopen32_file))(pfilefunc->zfile_func64.opaque,(const char*)filename,mode); // :3
    } // :3
} // :3

long call_zseek64 (const zlib_filefunc64_32_def* pfilefunc,voidpf filestream, ZPOS64_T offset, int origin) // :3
{ // :3
    if (pfilefunc->zfile_func64.zseek64_file != NULL) // :3
        return (*(pfilefunc->zfile_func64.zseek64_file)) (pfilefunc->zfile_func64.opaque,filestream,offset,origin); // :3
    else // :3
    { // :3
        uLong offsetTruncated = (uLong)offset; // :3
        if (offsetTruncated != offset) // :3
            return -1; // :3
        else // :3
            return (*(pfilefunc->zseek32_file))(pfilefunc->zfile_func64.opaque,filestream,offsetTruncated,origin); // :3
    } // :3
} // :3

ZPOS64_T call_ztell64 (const zlib_filefunc64_32_def* pfilefunc,voidpf filestream) // :3
{ // :3
    if (pfilefunc->zfile_func64.zseek64_file != NULL) // :3
        return (*(pfilefunc->zfile_func64.ztell64_file)) (pfilefunc->zfile_func64.opaque,filestream); // :3
    else // :3
    { // :3
        uLong tell_uLong = (*(pfilefunc->ztell32_file))(pfilefunc->zfile_func64.opaque,filestream); // :3
        if ((tell_uLong) == ((uLong)-1)) // :3
            return (ZPOS64_T)-1; // :3
        else // :3
            return tell_uLong; // :3
    } // :3
} // :3

void fill_zlib_filefunc64_32_def_from_filefunc32(zlib_filefunc64_32_def* p_filefunc64_32,const zlib_filefunc_def* p_filefunc32) // :3
{ // :3
    p_filefunc64_32->zfile_func64.zopen64_file = NULL; // :3
    p_filefunc64_32->zopen32_file = p_filefunc32->zopen_file; // :3
    p_filefunc64_32->zfile_func64.zerror_file = p_filefunc32->zerror_file; // :3
    p_filefunc64_32->zfile_func64.zread_file = p_filefunc32->zread_file; // :3
    p_filefunc64_32->zfile_func64.zwrite_file = p_filefunc32->zwrite_file; // :3
    p_filefunc64_32->zfile_func64.ztell64_file = NULL; // :3
    p_filefunc64_32->zfile_func64.zseek64_file = NULL; // :3
    p_filefunc64_32->zfile_func64.zclose_file = p_filefunc32->zclose_file; // :3
    p_filefunc64_32->zfile_func64.zerror_file = p_filefunc32->zerror_file; // :3
    p_filefunc64_32->zfile_func64.opaque = p_filefunc32->opaque; // :3
    p_filefunc64_32->zseek32_file = p_filefunc32->zseek_file; // :3
    p_filefunc64_32->ztell32_file = p_filefunc32->ztell_file; // :3
} // :3



static voidpf  ZCALLBACK fopen_file_func OF((voidpf opaque, const char* filename, int mode)); // :3
static uLong   ZCALLBACK fread_file_func OF((voidpf opaque, voidpf stream, void* buf, uLong size)); // :3
static uLong   ZCALLBACK fwrite_file_func OF((voidpf opaque, voidpf stream, const void* buf,uLong size)); // :3
static ZPOS64_T ZCALLBACK ftell64_file_func OF((voidpf opaque, voidpf stream)); // :3
static long    ZCALLBACK fseek64_file_func OF((voidpf opaque, voidpf stream, ZPOS64_T offset, int origin)); // :3
static int     ZCALLBACK fclose_file_func OF((voidpf opaque, voidpf stream)); // :3
static int     ZCALLBACK ferror_file_func OF((voidpf opaque, voidpf stream)); // :3

static voidpf ZCALLBACK fopen_file_func (voidpf opaque, const char* filename, int mode) // :3
{ // :3
    FILE* file = NULL; // :3
    const char* mode_fopen = NULL; // :3
    if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ) // :3
        mode_fopen = "rb"; // :3
    else // :3
    if (mode & ZLIB_FILEFUNC_MODE_EXISTING) // :3
        mode_fopen = "r+b"; // :3
    else // :3
    if (mode & ZLIB_FILEFUNC_MODE_CREATE) // :3
        mode_fopen = "wb"; // :3

    if ((filename!=NULL) && (mode_fopen != NULL)) // :3
        THE_FOPEN(file, filename, mode_fopen); // :3
    return file; // :3
} // :3

static voidpf ZCALLBACK fopen64_file_func (voidpf opaque, const void* filename, int mode) // :3
{ // :3
    FILE* file = NULL; // :3
    const char* mode_fopen = NULL; // :3
    if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ) // :3
        mode_fopen = "rb"; // :3
    else // :3
    if (mode & ZLIB_FILEFUNC_MODE_EXISTING) // :3
        mode_fopen = "r+b"; // :3
    else // :3
    if (mode & ZLIB_FILEFUNC_MODE_CREATE) // :3
        mode_fopen = "wb"; // :3

    if ((filename!=NULL) && (mode_fopen != NULL)) // :3
    { // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE || CC_TARGET_PLATFORM == CC_PLATFORM_BADA || CC_TARGET_PLATFORM == CC_PLATFORM_NACL || CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN) // :3
        file = NULL; // :3
#else // :3
        THE_FOPEN(file, (const char*)filename, mode_fopen); // :3
#endif // :3
    } // :3

    return file; // :3
} // :3


static uLong ZCALLBACK fread_file_func (voidpf opaque, voidpf stream, void* buf, uLong size) // :3
{ // :3
    uLong ret; // :3
    ret = (uLong)fread(buf, 1, (size_t)size, (FILE *)stream); // :3
    return ret; // :3
} // :3

static uLong ZCALLBACK fwrite_file_func (voidpf opaque, voidpf stream, const void* buf, uLong size) // :3
{ // :3
    uLong ret; // :3
    ret = (uLong)fwrite(buf, 1, (size_t)size, (FILE *)stream); // :3
    return ret; // :3
} // :3

static long ZCALLBACK ftell_file_func (voidpf opaque, voidpf stream) // :3
{ // :3
    long ret; // :3
    ret = ftell((FILE *)stream); // :3
    return ret; // :3
} // :3


static ZPOS64_T ZCALLBACK ftell64_file_func (voidpf opaque, voidpf stream) // :3
{ // :3
    ZPOS64_T ret; // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE || CC_TARGET_PLATFORM == CC_PLATFORM_BADA || CC_TARGET_PLATFORM == CC_PLATFORM_NACL || CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN) // :3
    ret = 0; // :3
#else // :3
    ret = ftell((FILE *)stream); // :3
#endif // :3
    return ret; // :3
} // :3

static long ZCALLBACK fseek_file_func (voidpf  opaque, voidpf stream, uLong offset, int origin) // :3
{ // :3
    int fseek_origin=0; // :3
    long ret; // :3
    switch (origin) // :3
    { // :3
    case ZLIB_FILEFUNC_SEEK_CUR : // :3
        fseek_origin = SEEK_CUR; // :3
        break; // :3
    case ZLIB_FILEFUNC_SEEK_END : // :3
        fseek_origin = SEEK_END; // :3
        break; // :3
    case ZLIB_FILEFUNC_SEEK_SET : // :3
        fseek_origin = SEEK_SET; // :3
        break; // :3
    default: return -1; // :3
    } // :3
    ret = 0; // :3
    if (fseek((FILE *)stream, offset, fseek_origin) != 0) // :3
        ret = -1; // :3
    return ret; // :3
} // :3

static long ZCALLBACK fseek64_file_func (voidpf  opaque, voidpf stream, ZPOS64_T offset, int origin) // :3
{ // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE || CC_TARGET_PLATFORM == CC_PLATFORM_BADA || CC_TARGET_PLATFORM == CC_PLATFORM_NACL || CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN) // :3
    return -1; // :3
#else // :3
    int fseek_origin=0; // :3
    switch (origin) // :3
    { // :3
    case ZLIB_FILEFUNC_SEEK_CUR : // :3
        fseek_origin = SEEK_CUR; // :3
        break; // :3
    case ZLIB_FILEFUNC_SEEK_END : // :3
        fseek_origin = SEEK_END; // :3
        break; // :3
    case ZLIB_FILEFUNC_SEEK_SET : // :3
        fseek_origin = SEEK_SET; // :3
        break; // :3
    default: return -1; // :3
    } // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) // :3
    if(_fseeki64((FILE *)stream, offset, fseek_origin) != 0) // :3
        return -1; // :3
#else // :3
    if(fseek((FILE *)stream, offset, fseek_origin) != 0) // :3
        return -1; // :3
#endif // :3
    return 0; // :3
#endif // :3
} // :3


static int ZCALLBACK fclose_file_func (voidpf opaque, voidpf stream) // :3
{ // :3
    int ret; // :3
    ret = fclose((FILE *)stream); // :3
    return ret; // :3
} // :3

static int ZCALLBACK ferror_file_func (voidpf opaque, voidpf stream) // :3
{ // :3
    int ret; // :3
    ret = ferror((FILE *)stream); // :3
    return ret; // :3
} // :3

void fill_fopen_filefunc (zlib_filefunc_def* pzlib_filefunc_def) // :3
{ // :3
    pzlib_filefunc_def->zopen_file = fopen_file_func; // :3
    pzlib_filefunc_def->zread_file = fread_file_func; // :3
    pzlib_filefunc_def->zwrite_file = fwrite_file_func; // :3
    pzlib_filefunc_def->ztell_file = ftell_file_func; // :3
    pzlib_filefunc_def->zseek_file = fseek_file_func; // :3
    pzlib_filefunc_def->zclose_file = fclose_file_func; // :3
    pzlib_filefunc_def->zerror_file = ferror_file_func; // :3
    pzlib_filefunc_def->opaque = NULL; // :3
} // :3

void fill_fopen64_filefunc (zlib_filefunc64_def*  pzlib_filefunc_def) // :3
{ // :3
    pzlib_filefunc_def->zopen64_file = fopen64_file_func; // :3
    pzlib_filefunc_def->zread_file = fread_file_func; // :3
    pzlib_filefunc_def->zwrite_file = fwrite_file_func; // :3
    pzlib_filefunc_def->ztell64_file = ftell64_file_func; // :3
    pzlib_filefunc_def->zseek64_file = fseek64_file_func; // :3
    pzlib_filefunc_def->zclose_file = fclose_file_func; // :3
    pzlib_filefunc_def->zerror_file = ferror_file_func; // :3
    pzlib_filefunc_def->opaque = NULL; // :3
} // :3

} // end of namespace cocos2d :3
