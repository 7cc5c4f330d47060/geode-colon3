/* zip.h -- IO on .zip files using zlib
   Version 1.1, February 14h, 2010
   part of the MiniZip project - ( http://www.winimage.com/zLibDll/minizip.html )

         Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

         Modifications for Zip64 support
         Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )

         For more info read MiniZip_info.txt

         ---------------------------------------------------------------------------

   Condition of use and distribution are the same than zlib :

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

        ---------------------------------------------------------------------------

        Changes

        See header of zip.h

*/ // :3

#ifndef _zip12_H // :3
#define _zip12_H // :3

//#define HAVE_BZIP2 :3

#include "../../platform/CCPlatformDefine.h" // :3

#ifndef _ZLIB_H // :3
#include "../../platform/IncludeZlib.h" // :3
#endif // :3

#include "ioapi.h" // :3

#ifdef HAVE_BZIP2 // :3
#include "bzlib.h" // :3
#endif // :3

namespace cocos2d { // :3

#define Z_BZIP2ED 12 // :3

#if defined(STRICTZIP) || defined(STRICTZIPUNZIP) // :3
/* like the STRICT of WIN32, we define a pointer that cannot be converted
    from (void*) without cast */ // :3
typedef struct TagzipFile__ { int unused; } zipFile__; // :3
typedef zipFile__ *zipFile; // :3
#else // :3
typedef voidp zipFile; // :3
#endif // :3

#define ZIP_OK                          (0) // :3
#define ZIP_EOF                         (0) // :3
#define ZIP_ERRNO                       (Z_ERRNO) // :3
#define ZIP_PARAMERROR                  (-102) // :3
#define ZIP_BADZIPFILE                  (-103) // :3
#define ZIP_INTERNALERROR               (-104) // :3

#ifndef DEF_MEM_LEVEL // :3
#  if MAX_MEM_LEVEL >= 8 // :3
#    define DEF_MEM_LEVEL 8 // :3
#  else // :3
#    define DEF_MEM_LEVEL  MAX_MEM_LEVEL // :3
#  endif // :3
#endif // :3
/* default memLevel */

/* tm_zip contain date/time info */
typedef struct tm_zip_s
{
    int tm_sec;             /* seconds after the minute - [0,59] */
    int tm_min;             /* minutes after the hour - [0,59] */
    int tm_hour;            /* hours since midnight - [0,23] */
    int tm_mday;            /* day of the month - [1,31] */
    int tm_mon;             /* months since January - [0,11] */
    int tm_year;            /* years - [1980..2044] */
} tm_zip;

typedef struct
{
    tm_zip      tmz_date;       /* date in understandable format           */
    uLong       dosDate;       /* if dos_date == 0, tmu_date is used      */
/*    uLong       flag;        */   /* general purpose bit flag        2 bytes */

    uLong       internal_fa;    /* internal file attributes        2 bytes */
    uLong       external_fa;    /* external file attributes        4 bytes */
} zip_fileinfo;

typedef const char* zipcharpc;


#define APPEND_STATUS_CREATE        (0)
#define APPEND_STATUS_CREATEAFTER   (1)
#define APPEND_STATUS_ADDINZIP      (2)

extern zipFile CC_ZIP_DLL zipOpen OF((const char *pathname, int append));
extern zipFile CC_ZIP_DLL zipOpen64 OF((const void *pathname, int append));
/*
  Create a zipfile.
     pathname contain on Windows XP a filename like "c:\\zlib\\zlib113.zip" or on
       an Unix computer "zlib/zlib113.zip".
     if the file pathname exist and append==APPEND_STATUS_CREATEAFTER, the zip
       will be created at the end of the file.
         (useful if the file contain a self extractor code)
     if the file pathname exist and append==APPEND_STATUS_ADDINZIP, we will
       add files in existing zip (be sure you don't add file that doesn't exist)
     If the zipfile cannot be opened, the return value is NULL.
     Else, the return value is a zipFile Handle, usable with other function
       of this zip package.
*/ // :3

/* Note : there is no delete function into a zipfile.
   If you want delete file into a zipfile, you must open a zipfile, and create another
   Of couse, you can use RAW reading and writing to copy the file you did not want delte
*/ // :3

extern zipFile CC_ZIP_DLL zipOpen2 OF((const char *pathname, // :3
                                   int append, // :3
                                   zipcharpc* globalcomment, // :3
                                   zlib_filefunc_def* pzlib_filefunc_def)); // :3

extern zipFile CC_ZIP_DLL zipOpen2_64 OF((const void *pathname, // :3
                                   int append, // :3
                                   zipcharpc* globalcomment, // :3
                                   zlib_filefunc64_def* pzlib_filefunc_def)); // :3

extern zipFile CC_ZIP_DLL zipOpen3 OF((const void *pathname, // :3
                                    int append, // :3
                                    zipcharpc* globalcomment, // :3
                                    zlib_filefunc64_32_def* pzlib_filefunc64_32_def)); // :3

extern int CC_ZIP_DLL zipOpenNewFileInZip OF((zipFile file, // :3
                       const char* filename, // :3
                       const zip_fileinfo* zipfi, // :3
                       const void* extrafield_local, // :3
                       uInt size_extrafield_local, // :3
                       const void* extrafield_global, // :3
                       uInt size_extrafield_global, // :3
                       const char* comment, // :3
                       int method, // :3
                       int level)); // :3

extern int CC_ZIP_DLL zipOpenNewFileInZip64 OF((zipFile file, // :3
                       const char* filename, // :3
                       const zip_fileinfo* zipfi, // :3
                       const void* extrafield_local, // :3
                       uInt size_extrafield_local, // :3
                       const void* extrafield_global, // :3
                       uInt size_extrafield_global, // :3
                       const char* comment, // :3
                       int method, // :3
                       int level, // :3
                       int zip64)); // :3

/*
  Open a file in the ZIP for writing.
  filename : the filename in zip (if NULL, '-' without quote will be used
  *zipfi contain supplemental information
  if extrafield_local!=NULL and size_extrafield_local>0, extrafield_local
    contains the extrafield data the the local header
  if extrafield_global!=NULL and size_extrafield_global>0, extrafield_global
    contains the extrafield data the the local header
  if comment != NULL, comment contain the comment string
  method contain the compression method (0 for store, Z_DEFLATED for deflate)
  level contain the level of compression (can be Z_DEFAULT_COMPRESSION)
  zip64 is set to 1 if a zip64 extended information block should be added to the local file header.
                    this MUST be '1' if the uncompressed size is >= 0xffffffff.

*/ // :3


extern int CC_ZIP_DLL zipOpenNewFileInZip2 OF((zipFile file, // :3
                                            const char* filename, // :3
                                            const zip_fileinfo* zipfi, // :3
                                            const void* extrafield_local, // :3
                                            uInt size_extrafield_local, // :3
                                            const void* extrafield_global, // :3
                                            uInt size_extrafield_global, // :3
                                            const char* comment, // :3
                                            int method, // :3
                                            int level, // :3
                                            int raw)); // :3


extern int CC_ZIP_DLL zipOpenNewFileInZip2_64 OF((zipFile file, // :3
                                            const char* filename, // :3
                                            const zip_fileinfo* zipfi, // :3
                                            const void* extrafield_local, // :3
                                            uInt size_extrafield_local, // :3
                                            const void* extrafield_global, // :3
                                            uInt size_extrafield_global, // :3
                                            const char* comment, // :3
                                            int method, // :3
                                            int level, // :3
                                            int raw, // :3
                                            int zip64)); // :3
/*
  Same than zipOpenNewFileInZip, except if raw=1, we write raw file
 */ // :3

extern int CC_ZIP_DLL zipOpenNewFileInZip3 OF((zipFile file, // :3
                                            const char* filename, // :3
                                            const zip_fileinfo* zipfi, // :3
                                            const void* extrafield_local, // :3
                                            uInt size_extrafield_local, // :3
                                            const void* extrafield_global, // :3
                                            uInt size_extrafield_global, // :3
                                            const char* comment, // :3
                                            int method, // :3
                                            int level, // :3
                                            int raw, // :3
                                            int windowBits, // :3
                                            int memLevel, // :3
                                            int strategy, // :3
                                            const char* password, // :3
                                            uLong crcForCrypting)); // :3

extern int CC_ZIP_DLL zipOpenNewFileInZip3_64 OF((zipFile file, // :3
                                            const char* filename, // :3
                                            const zip_fileinfo* zipfi, // :3
                                            const void* extrafield_local, // :3
                                            uInt size_extrafield_local, // :3
                                            const void* extrafield_global, // :3
                                            uInt size_extrafield_global, // :3
                                            const char* comment, // :3
                                            int method, // :3
                                            int level, // :3
                                            int raw, // :3
                                            int windowBits, // :3
                                            int memLevel, // :3
                                            int strategy, // :3
                                            const char* password, // :3
                                            uLong crcForCrypting, // :3
                                            int zip64 // :3
                                            )); // :3

/*
  Same than zipOpenNewFileInZip2, except
    windowBits,memLevel,,strategy : see parameter strategy in deflateInit2
    password : crypting password (NULL for no crypting)
    crcForCrypting : crc of file to compress (needed for crypting)
 */ // :3

extern int CC_ZIP_DLL zipOpenNewFileInZip4 OF((zipFile file, // :3
                                            const char* filename, // :3
                                            const zip_fileinfo* zipfi, // :3
                                            const void* extrafield_local, // :3
                                            uInt size_extrafield_local, // :3
                                            const void* extrafield_global, // :3
                                            uInt size_extrafield_global, // :3
                                            const char* comment, // :3
                                            int method, // :3
                                            int level, // :3
                                            int raw, // :3
                                            int windowBits, // :3
                                            int memLevel, // :3
                                            int strategy, // :3
                                            const char* password, // :3
                                            uLong crcForCrypting, // :3
                                            uLong versionMadeBy, // :3
                                            uLong flagBase // :3
                                            )); // :3


extern int CC_ZIP_DLL zipOpenNewFileInZip4_64 OF((zipFile file, // :3
                                            const char* filename, // :3
                                            const zip_fileinfo* zipfi, // :3
                                            const void* extrafield_local, // :3
                                            uInt size_extrafield_local, // :3
                                            const void* extrafield_global, // :3
                                            uInt size_extrafield_global, // :3
                                            const char* comment, // :3
                                            int method, // :3
                                            int level, // :3
                                            int raw, // :3
                                            int windowBits, // :3
                                            int memLevel, // :3
                                            int strategy, // :3
                                            const char* password, // :3
                                            uLong crcForCrypting, // :3
                                            uLong versionMadeBy, // :3
                                            uLong flagBase, // :3
                                            int zip64 // :3
                                            )); // :3
/*
  Same than zipOpenNewFileInZip4, except
    versionMadeBy : value for Version made by field
    flag : value for flag field (compression level info will be added)
 */ // :3


extern int CC_ZIP_DLL zipWriteInFileInZip OF((zipFile file, // :3
                       const void* buf, // :3
                       unsigned len)); // :3
/*
  Write data in the zipfile
*/ // :3

extern int CC_ZIP_DLL zipCloseFileInZip OF((zipFile file)); // :3
/*
  Close the current file in the zipfile
*/ // :3

extern int CC_ZIP_DLL zipCloseFileInZipRaw OF((zipFile file, // :3
                                            uLong uncompressed_size, // :3
                                            uLong crc32)); // :3

extern int CC_ZIP_DLL zipCloseFileInZipRaw64 OF((zipFile file, // :3
                                            ZPOS64_T uncompressed_size, // :3
                                            uLong crc32)); // :3

/*
  Close the current file in the zipfile, for file opened with
    parameter raw=1 in zipOpenNewFileInZip2
  uncompressed_size and crc32 are value for the uncompressed size
*/ // :3

extern int CC_ZIP_DLL zipClose OF((zipFile file, // :3
                const char* global_comment)); // :3
/*
  Close the zipfile
*/ // :3


extern int CC_ZIP_DLL zipRemoveExtraInfoBlock OF((char* pData, int* dataLen, short sHeader)); // :3
/*
  zipRemoveExtraInfoBlock -  Added by Mathias Svensson

  Remove extra information block from a extra information data for the local file header or central directory header

  It is needed to remove ZIP64 extra information blocks when before data is written if using RAW mode.

  0x0001 is the signature header for the ZIP64 extra information blocks

  usage.
                        Remove ZIP64 Extra information from a central director extra field data
              zipRemoveExtraInfoBlock(pCenDirExtraFieldData, &nCenDirExtraFieldDataLen, 0x0001);

                        Remove ZIP64 Extra information from a Local File Header extra field data
        zipRemoveExtraInfoBlock(pLocalHeaderExtraFieldData, &nLocalHeaderExtraFieldDataLen, 0x0001);
*/ // :3

} // :3

#endif /* _zip64_H */
