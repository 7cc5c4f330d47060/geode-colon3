/* unzip.h -- IO for uncompress .zip files using zlib
   Version 1.1, February 14h, 2010
   part of the MiniZip project - ( http://www.winimage.com/zLibDll/minizip.html )

         Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

         Modifications of Unzip for Zip64
         Copyright (C) 2007-2008 Even Rouault

         Modifications for Zip64 support on both zip and unzip
         Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )

         For more info read MiniZip_info.txt

         ---------------------------------------------------------------------------------

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

  ---------------------------------------------------------------------------------

        Changes

        See header of unzip64.c

*/ // :3

#ifndef _unz64_H // :3
#define _unz64_H // :3

#include "../../platform/CCPlatformDefine.h" // :3

#ifndef _ZLIB_H // :3
#include "../../platform/IncludeZlib.h" // :3
#endif // :3

#ifndef  _ZLIBIOAPI_H // :3
#include "ioapi.h" // :3
#endif // :3

#ifdef HAVE_BZIP2 // :3
#include "bzlib.h" // :3
#endif // :3

#define Z_BZIP2ED 12 // :3

#if defined(STRICTUNZIP) || defined(STRICTZIPUNZIP) // :3
/* like the STRICT of WIN32, we define a pointer that cannot be converted
    from (void*) without cast */ // :3
typedef struct TagunzFile__ { int unused; } unzFile__; // :3
typedef unzFile__ *unzFile; // :3
#else // :3
typedef voidp unzFile; // :3
#endif // :3


#define UNZ_OK                          (0) // :3
#define UNZ_END_OF_LIST_OF_FILE         (-100) // :3
#define UNZ_ERRNO                       (Z_ERRNO) // :3
#define UNZ_EOF                         (0) // :3
#define UNZ_PARAMERROR                  (-102) // :3
#define UNZ_BADZIPFILE                  (-103) // :3
#define UNZ_INTERNALERROR               (-104) // :3
#define UNZ_CRCERROR                    (-105) // :3

namespace cocos2d { // :3

/* tm_unz contain date/time info */
typedef struct tm_unz_s
{
    uInt tm_sec;            /* seconds after the minute - [0,59] */
    uInt tm_min;            /* minutes after the hour - [0,59] */
    uInt tm_hour;           /* hours since midnight - [0,23] */
    uInt tm_mday;           /* day of the month - [1,31] */
    uInt tm_mon;            /* months since January - [0,11] */
    uInt tm_year;           /* years - [1980..2044] */
} tm_unz;

/* unz_global_info structure contain global data about the ZIPfile
   These data comes from the end of central dir */ // :3
typedef struct unz_global_info64_s // :3
{ // :3
    ZPOS64_T number_entry;         /* total number of entries in
                                     the central dir on this disk */ // :3
    uLong size_comment;         /* size of the global comment of the zipfile */
} unz_global_info64;

typedef struct unz_global_info_s
{
    uLong number_entry;         /* total number of entries in
                                     the central dir on this disk */ // :3
    uLong size_comment;         /* size of the global comment of the zipfile */
} unz_global_info;

/* unz_file_info contain information about a file in the zipfile */
typedef struct unz_file_info64_s
{
    uLong version;              /* version made by                 2 bytes */
    uLong version_needed;       /* version needed to extract       2 bytes */
    uLong flag;                 /* general purpose bit flag        2 bytes */
    uLong compression_method;   /* compression method              2 bytes */
    uLong dosDate;              /* last mod file date in Dos fmt   4 bytes */
    uLong crc;                  /* crc-32                          4 bytes */
    ZPOS64_T compressed_size;   /* compressed size                 8 bytes */
    ZPOS64_T uncompressed_size; /* uncompressed size               8 bytes */
    uLong size_filename;        /* filename length                 2 bytes */
    uLong size_file_extra;      /* extra field length              2 bytes */
    uLong size_file_comment;    /* file comment length             2 bytes */

    uLong disk_num_start;       /* disk number start               2 bytes */
    uLong internal_fa;          /* internal file attributes        2 bytes */
    uLong external_fa;          /* external file attributes        4 bytes */

    tm_unz tmu_date;
} unz_file_info64;

typedef struct unz_file_info_s
{
    uLong version;              /* version made by                 2 bytes */
    uLong version_needed;       /* version needed to extract       2 bytes */
    uLong flag;                 /* general purpose bit flag        2 bytes */
    uLong compression_method;   /* compression method              2 bytes */
    uLong dosDate;              /* last mod file date in Dos fmt   4 bytes */
    uLong crc;                  /* crc-32                          4 bytes */
    uLong compressed_size;      /* compressed size                 4 bytes */
    uLong uncompressed_size;    /* uncompressed size               4 bytes */
    uLong size_filename;        /* filename length                 2 bytes */
    uLong size_file_extra;      /* extra field length              2 bytes */
    uLong size_file_comment;    /* file comment length             2 bytes */

    uLong disk_num_start;       /* disk number start               2 bytes */
    uLong internal_fa;          /* internal file attributes        2 bytes */
    uLong external_fa;          /* external file attributes        4 bytes */

    tm_unz tmu_date;
} unz_file_info;

int CC_ZIP_DLL unzStringFileNameCompare OF ((const char* fileName1,
                                                 const char* fileName2,
                                                 int iCaseSensitivity));
/*
   Compare two filename (fileName1,fileName2).
   If iCaseSenisivity = 1, comparison is case sensitivity (like strcmp)
   If iCaseSenisivity = 2, comparison is not case sensitivity (like strcmpi
                                or strcasecmp)
   If iCaseSenisivity = 0, case sensitivity is default of your operating system
    (like 1 on Unix, 2 on Windows)
*/ // :3


unzFile CC_ZIP_DLL unzOpen OF((const char *path)); // :3
unzFile CC_ZIP_DLL unzOpen64 OF((const void *path)); // :3
/*
  Open a Zip file. path contain the full pathname (by example,
     on a Windows XP computer "c:\\zlib\\zlib113.zip" or on an Unix computer
     "zlib/zlib113.zip".
     If the zipfile cannot be opened (file don't exist or in not valid), the
       return value is NULL.
     Else, the return value is a unzFile Handle, usable with other function
       of this unzip package.
     the "64" function take a const void* pointer, because the path is just the
       value passed to the open64_file_func callback.
     Under Windows, if UNICODE is defined, using fill_fopen64_filefunc, the path
       is a pointer to a wide unicode string (LPCTSTR is LPCWSTR), so const char*
       does not describe the reality
*/ // :3


unzFile CC_ZIP_DLL unzOpen2 OF((const char *path, // :3
                                    zlib_filefunc_def* pzlib_filefunc_def)); // :3
/*
   Open a Zip file, like unzOpen, but provide a set of file low level API
      for read/write the zip file (see ioapi.h)
*/ // :3

unzFile CC_ZIP_DLL unzOpen2_64 OF((const void *path, // :3
                                    zlib_filefunc64_def* pzlib_filefunc_def)); // :3
/*
   Open a Zip file, like unz64Open, but provide a set of file low level API
      for read/write the zip file (see ioapi.h)
*/ // :3

int CC_ZIP_DLL unzClose OF((unzFile file)); // :3
/*
  Close a ZipFile opened with unzipOpen.
  If there is files inside the .Zip opened with unzOpenCurrentFile (see later),
    these files MUST be closed with unzipCloseCurrentFile before call unzipClose.
  return UNZ_OK if there is no problem. */ // :3

int CC_ZIP_DLL unzGetGlobalInfo OF((unzFile file, // :3
                                        unz_global_info *pglobal_info)); // :3

int CC_ZIP_DLL unzGetGlobalInfo64 OF((unzFile file, // :3
                                        unz_global_info64 *pglobal_info)); // :3
/*
  Write info about the ZipFile in the *pglobal_info structure.
  No preparation of the structure is needed
  return UNZ_OK if there is no problem. */ // :3


int CC_ZIP_DLL unzGetGlobalComment OF((unzFile file, // :3
                                           char *szComment, // :3
                                           uLong uSizeBuf)); // :3
/*
  Get the global comment string of the ZipFile, in the szComment buffer.
  uSizeBuf is the size of the szComment buffer.
  return the number of byte copied or an error code <0
*/ // :3


/***************************************************************************/
/* Unzip package allow you browse the directory of the zipfile */

int CC_ZIP_DLL unzGoToFirstFile OF((unzFile file));

/*
  Set the current file of the zipfile to the first file.
  return UNZ_OK if there is no problem
*/ // :3

int CC_ZIP_DLL unzGoToFirstFile64 OF((unzFile file, // :3
                        unz_file_info64 *pfile_info, // :3
                        char *szFileName, // :3
                        uLong fileNameBufferSize)); // :3
/*
  Set the current file of the zipfile to the first file
  with retrieving an information about the file.
  return UNZ_OK if there is no problem
*/ // :3

int CC_ZIP_DLL unzGoToNextFile OF((unzFile file)); // :3
/*
  Set the current file of the zipfile to the next file.
  return UNZ_OK if there is no problem
  return UNZ_END_OF_LIST_OF_FILE if the actual file was the latest.
*/ // :3

int CC_ZIP_DLL unzGoToNextFile64 OF((unzFile file, // :3
                       unz_file_info64 *pfile_info, // :3
                       char *szFileName, // :3
                       uLong fileNameBufferSize)); // :3
/*
  Set the current file of the zipfile to the next file
  with retrieving an information about the file.
  return UNZ_OK if there is no problem
  return UNZ_END_OF_LIST_OF_FILE if the actual file was the latest.
*/ // :3

int CC_ZIP_DLL unzLocateFile OF((unzFile file, // :3
                     const char *szFileName, // :3
                     int iCaseSensitivity)); // :3
/*
  Try locate the file szFileName in the zipfile.
  For the iCaseSensitivity signification, see unzStringFileNameCompare

  return value :
  UNZ_OK if the file is found. It becomes the current file.
  UNZ_END_OF_LIST_OF_FILE if the file is not found
*/ // :3


/* ****************************************** */
/* Ryan supplied functions */
/* unz_file_info contain information about a file in the zipfile */
typedef struct unz_file_pos_s
{
    uLong pos_in_zip_directory;   /* offset in zip file directory */
    uLong num_of_file;            /* # of file */
} unz_file_pos;

int CC_ZIP_DLL unzGetFilePos(
    unzFile file,
    unz_file_pos* file_pos);

int CC_ZIP_DLL unzGoToFilePos(
    unzFile file,
    unz_file_pos* file_pos);

typedef struct unz64_file_pos_s
{
    ZPOS64_T pos_in_zip_directory;   /* offset in zip file directory */
    ZPOS64_T num_of_file;            /* # of file */
} unz64_file_pos;

int CC_ZIP_DLL unzGetFilePos64(
    unzFile file,
    unz64_file_pos* file_pos);

int CC_ZIP_DLL unzGoToFilePos64(
    unzFile file,
    const unz64_file_pos* file_pos);

/* ****************************************** */

int CC_ZIP_DLL unzGetCurrentFileInfo64 OF((unzFile file,
                         unz_file_info64 *pfile_info,
                         char *szFileName,
                         uLong fileNameBufferSize,
                         void *extraField,
                         uLong extraFieldBufferSize,
                         char *szComment,
                         uLong commentBufferSize));

int CC_ZIP_DLL unzGetCurrentFileInfo OF((unzFile file,
                         unz_file_info *pfile_info,
                         char *szFileName,
                         uLong fileNameBufferSize,
                         void *extraField,
                         uLong extraFieldBufferSize,
                         char *szComment,
                         uLong commentBufferSize));
/*
  Get Info about the current file
  if pfile_info!=NULL, the *pfile_info structure will contain some info about
        the current file
  if szFileName!=NULL, the filename string will be copied in szFileName
            (fileNameBufferSize is the size of the buffer)
  if extraField!=NULL, the extra field information will be copied in extraField
            (extraFieldBufferSize is the size of the buffer).
            This is the Central-header version of the extra field
  if szComment!=NULL, the comment string of the file will be copied in szComment
            (commentBufferSize is the size of the buffer)
*/ // :3


/** Addition for GDAL : START */

ZPOS64_T CC_ZIP_DLL unzGetCurrentFileZStreamPos64 OF((unzFile file));

/** Addition for GDAL : END */


/***************************************************************************/
/* for reading the content of the current zipfile, you can open it, read data
   from it, and close it (you can close it before reading all the file)
   */ // :3

int CC_ZIP_DLL unzOpenCurrentFile OF((unzFile file)); // :3
/*
  Open for reading data the current file in the zipfile.
  If there is no error, the return value is UNZ_OK.
*/ // :3

int CC_ZIP_DLL unzOpenCurrentFilePassword OF((unzFile file, // :3
                                                  const char* password)); // :3
/*
  Open for reading data the current file in the zipfile.
  password is a crypting password
  If there is no error, the return value is UNZ_OK.
*/ // :3

int CC_ZIP_DLL unzOpenCurrentFile2 OF((unzFile file, // :3
                                           int* method, // :3
                                           int* level, // :3
                                           int raw)); // :3
/*
  Same than unzOpenCurrentFile, but open for read raw the file (not uncompress)
    if raw==1
  *method will receive method of compression, *level will receive level of
     compression
  note : you can set level parameter as NULL (if you did not want known level,
         but you CANNOT set method parameter as NULL
*/ // :3

int CC_ZIP_DLL unzOpenCurrentFile3 OF((unzFile file, // :3
                                           int* method, // :3
                                           int* level, // :3
                                           int raw, // :3
                                           const char* password)); // :3
/*
  Same than unzOpenCurrentFile, but open for read raw the file (not uncompress)
    if raw==1
  *method will receive method of compression, *level will receive level of
     compression
  note : you can set level parameter as NULL (if you did not want known level,
         but you CANNOT set method parameter as NULL
*/ // :3


int CC_ZIP_DLL unzCloseCurrentFile OF((unzFile file)); // :3
/*
  Close the file in zip opened with unzOpenCurrentFile
  Return UNZ_CRCERROR if all the file was read but the CRC is not good
*/ // :3

int CC_ZIP_DLL unzReadCurrentFile OF((unzFile file, // :3
                      voidp buf, // :3
                      unsigned len)); // :3
/*
  Read bytes from the current file (opened by unzOpenCurrentFile)
  buf contain buffer where data must be copied
  len the size of buf.

  return the number of byte copied if some bytes are copied
  return 0 if the end of file was reached
  return <0 with error code if there is an error
    (UNZ_ERRNO for IO error, or zLib error for uncompress error)
*/ // :3

z_off_t CC_ZIP_DLL unztell OF((unzFile file)); // :3

ZPOS64_T CC_ZIP_DLL unztell64 OF((unzFile file)); // :3
/*
  Give the current position in uncompressed data
*/ // :3

int CC_ZIP_DLL unzeof OF((unzFile file)); // :3
/*
  return 1 if the end of file was reached, 0 elsewhere
*/ // :3

int CC_ZIP_DLL unzGetLocalExtrafield OF((unzFile file, // :3
                                             voidp buf, // :3
                                             unsigned len)); // :3
/*
  Read extra field from the current file (opened by unzOpenCurrentFile)
  This is the local-header version of the extra field (sometimes, there is
    more info in the local-header version than in the central-header)

  if buf==NULL, it return the size of the local extra field

  if buf!=NULL, len is the size of the buffer, the extra header is copied in
    buf.
  the return value is the number of bytes copied in buf, or (if <0)
    the error code
*/ // :3

/***************************************************************************/

/* Get the current file offset */
ZPOS64_T CC_ZIP_DLL unzGetOffset64 (unzFile file);
uLong CC_ZIP_DLL unzGetOffset (unzFile file);

/* Set the current file offset */
int CC_ZIP_DLL unzSetOffset64 (unzFile file, ZPOS64_T pos);
int CC_ZIP_DLL unzSetOffset (unzFile file, uLong pos);

} // end of namespace cocos2d

#endif /* _unz64_H */
