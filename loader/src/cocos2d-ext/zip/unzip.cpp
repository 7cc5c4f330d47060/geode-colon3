/* unzip.c -- IO for uncompress .zip files using zlib
   Version 1.1, February 14h, 2010
   part of the MiniZip project - ( http://www.winimage.com/zLibDll/minizip.html )

         Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

         Modifications of Unzip for Zip64
         Copyright (C) 2007-2008 Even Rouault

         Modifications for Zip64 support on both zip and unzip
         Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )

         For more info read MiniZip_info.txt


  ------------------------------------------------------------------------------------
  Decryption code comes from crypt.c by Info-ZIP but has been greatly reduced in terms of
  compatibility with older software. The following is from the original crypt.c.
  Code woven in by Terry Thorsen 1/2003.

  Copyright (c) 1990-2000 Info-ZIP.  All rights reserved.

  See the accompanying file LICENSE, version 2000-Apr-09 or later
  (the contents of which are also included in zip.h) for terms of use.
  If, for some reason, all these files are missing, the Info-ZIP license
  also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html

        crypt.c (full version) by Info-ZIP.      Last revised:  [see crypt.h]

  The encryption/decryption parts of this source code (as opposed to the
  non-echoing password parts) were originally written in Europe.  The
  whole source package can be freely distributed, including from the USA.
  (Prior to January 2000, re-export from the US was a violation of US law.)

        This encryption code is a direct transcription of the algorithm from
  Roger Schlafly, described by Phil Katz in the file appnote.txt.  This
  file (appnote.txt) is distributed with the PKZIP program (even in the
  version without encryption capabilities).

        ------------------------------------------------------------------------------------

        Changes in unzip.c

        2007-2008 - Even Rouault - Addition of cpl_unzGetCurrentFileZStreamPos
  2007-2008 - Even Rouault - Decoration of symbol names unz* -> cpl_unz*
  2007-2008 - Even Rouault - Remove old C style function prototypes
  2007-2008 - Even Rouault - Add unzip support for ZIP64

        Copyright (C) 2007-2008 Even Rouault


        Oct-2009 - Mathias Svensson - Removed cpl_* from symbol names (Even Rouault added them but since this is now moved to a new project (minizip64) I renamed them again).
  Oct-2009 - Mathias Svensson - Fixed problem if uncompressed size was > 4G and compressed size was <4G
                                should only read the compressed/uncompressed size from the Zip64 format if
                                the size from normal header was 0xFFFFFFFF
  Oct-2009 - Mathias Svensson - Applied some bug fixes from patches received from Gilles Vollant
        Oct-2009 - Mathias Svensson - Applied support to unzip files with compression method BZIP2 (bzip2 lib is required)
                                Patch created by Daniel Borca

  Jan-2010 - back to unzip and minizip 1.0 name scheme, with compatibility layer

  Copyright (C) 1998 - 2010 Gilles Vollant, Even Rouault, Mathias Svensson

*/ // :3

#ifdef _MSC_VER // :3
#pragma warning(disable: 4251) // :3
#endif // :3

#include <stdio.h> // :3
#include <stdlib.h> // :3
#include <string.h> // :3

#ifndef NOUNCRYPT // :3
        #define NOUNCRYPT // :3
#endif // :3

#include <../platform/IncludeZlib.h> // :3
#include <../support/zip_support/unzip.h> // :3

#ifdef STDC // :3
#  include <stddef.h> // :3
#  include <string.h> // :3
#  include <stdlib.h> // :3
#endif // :3
#ifdef NO_ERRNO_H // :3
    extern int errno; // :3
#else // :3
#   include <errno.h> // :3
#endif // :3

namespace cocos2d { // :3

#ifndef local // :3
#  define local static // :3
#endif // :3
/* compile with -Dlocal if your debugger can't find static symbols */


#ifndef CASESENSITIVITYDEFAULT_NO
#  if !defined(unix) && !defined(CASESENSITIVITYDEFAULT_YES)
#    define CASESENSITIVITYDEFAULT_NO
#  endif
#endif


#ifndef UNZ_BUFSIZE
#define UNZ_BUFSIZE (16384)
#endif

#ifndef UNZ_MAXFILENAMEINZIP
#define UNZ_MAXFILENAMEINZIP (256)
#endif

#ifndef ALLOC
# define ALLOC(size) (malloc(size))
#endif
#ifndef TRYFREE
# define TRYFREE(p) {if (p) free(p);}
#endif

#define SIZECENTRALDIRITEM (0x2e)
#define SIZEZIPLOCALHEADER (0x1e)


const char unz_copyright[] =
   " unzip 1.01 Copyright 1998-2004 Gilles Vollant - http://www.winimage.com/zLibDll";

/* unz_file_info_interntal contain internal info about a file in zipfile*/
typedef struct unz_file_info64_internal_s
{
    ZPOS64_T offset_curfile;/* relative offset of local header 8 bytes */
} unz_file_info64_internal;


/* file_in_zip_read_info_s contain internal information about a file in zipfile,
    when reading and decompress it */ // :3
typedef struct // :3
{ // :3
    char  *read_buffer;         /* internal buffer for compressed data */
    z_stream stream;            /* zLib stream structure for inflate */

#ifdef HAVE_BZIP2
    bz_stream bstream;          /* bzLib stream structure for bziped */
#endif

    ZPOS64_T pos_in_zipfile;       /* position in byte on the zipfile, for fseek*/
    uLong stream_initialised;   /* flag set if stream structure is initialized*/

    ZPOS64_T offset_local_extrafield;/* offset of the local extra field */
    uInt  size_local_extrafield;/* size of the local extra field */
    ZPOS64_T pos_local_extrafield;   /* position in the local extra field in read*/
    ZPOS64_T total_out_64;

    uLong crc32;                /* crc32 of all data uncompressed */
    uLong crc32_wait;           /* crc32 we must obtain after decompress all */
    ZPOS64_T rest_read_compressed; /* number of byte to be decompressed */
    ZPOS64_T rest_read_uncompressed;/*number of byte to be obtained after decomp*/
    zlib_filefunc64_32_def z_filefunc;
    voidpf filestream;        /* io structure of the zipfile */
    uLong compression_method;   /* compression method (0==store) */
    ZPOS64_T byte_before_the_zipfile;/* byte before the zipfile, (>0 for sfx)*/
    int   raw;
} file_in_zip64_read_info_s;


/* unz64_s contain internal information about the zipfile
*/ // :3
typedef struct // :3
{ // :3
    zlib_filefunc64_32_def z_filefunc; // :3
    int is64bitOpenFunction; // :3
    voidpf filestream;        /* io structure of the zipfile */
    unz_global_info64 gi;       /* public global information */
    ZPOS64_T byte_before_the_zipfile;/* byte before the zipfile, (>0 for sfx)*/
    ZPOS64_T num_file;             /* number of the current file in the zipfile*/
    ZPOS64_T pos_in_central_dir;   /* pos of the current file in the central dir*/
    ZPOS64_T current_file_ok;      /* flag about the usability of the current file*/
    ZPOS64_T central_pos;          /* position of the beginning of the central dir*/

    ZPOS64_T size_central_dir;     /* size of the central directory  */
    ZPOS64_T offset_central_dir;   /* offset of start of central directory with
                                   respect to the starting disk number */ // :3

    unz_file_info64 cur_file_info; /* public info about the current file in zip*/
    unz_file_info64_internal cur_file_info_internal; /* private info about it*/
    file_in_zip64_read_info_s* pfile_in_zip_read; /* structure about the current
                                        file if we are decompressing it */ // :3
    int encrypted; // :3

    int isZip64; // :3

#    ifndef NOUNCRYPT // :3
    unsigned long keys[3];     /* keys defining the pseudo-random sequence */
    const unsigned long* pcrc_32_tab;
#    endif
} unz64_s;


#ifndef NOUNCRYPT
#include "crypt.h"
#endif

/* ===========================================================================
     Read a byte from a gz_stream; update next_in and avail_in. Return EOF
   for end of file.
   IN assertion: the stream s has been successfully opened for reading.
*/ // :3


local int unz64local_getByte OF(( // :3
    const zlib_filefunc64_32_def* pzlib_filefunc_def, // :3
    voidpf filestream, // :3
    int *pi)); // :3

local int unz64local_getByte(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream, int *pi) // :3
{ // :3
    unsigned char c; // :3
    int err = (int)ZREAD64(*pzlib_filefunc_def,filestream,&c,1); // :3
    if (err==1) // :3
    { // :3
        *pi = (int)c; // :3
        return UNZ_OK; // :3
    } // :3
    else // :3
    { // :3
        if (ZERROR64(*pzlib_filefunc_def,filestream)) // :3
            return UNZ_ERRNO; // :3
        else // :3
            return UNZ_EOF; // :3
    } // :3
} // :3


/* ===========================================================================
   Reads a long in LSB order from the given gz_stream. Sets
*/ // :3
local int unz64local_getShort OF(( // :3
    const zlib_filefunc64_32_def* pzlib_filefunc_def, // :3
    voidpf filestream, // :3
    uLong *pX)); // :3

local int unz64local_getShort (const zlib_filefunc64_32_def* pzlib_filefunc_def, // :3
                             voidpf filestream, // :3
                             uLong *pX) // :3
{ // :3
    uLong x ; // :3
    int i = 0; // :3
    int err; // :3

    err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x = (uLong)i; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((uLong)i)<<8; // :3

    if (err==UNZ_OK) // :3
        *pX = x; // :3
    else // :3
        *pX = 0; // :3
    return err; // :3
} // :3

local int unz64local_getLong OF(( // :3
    const zlib_filefunc64_32_def* pzlib_filefunc_def, // :3
    voidpf filestream, // :3
    uLong *pX)); // :3

local int unz64local_getLong (const zlib_filefunc64_32_def* pzlib_filefunc_def, // :3
                            voidpf filestream, // :3
                            uLong *pX) // :3
{ // :3
    uLong x ; // :3
    int i = 0; // :3
    int err; // :3

    err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x = (uLong)i; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((uLong)i)<<8; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((uLong)i)<<16; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x += ((uLong)i)<<24; // :3

    if (err==UNZ_OK) // :3
        *pX = x; // :3
    else // :3
        *pX = 0; // :3
    return err; // :3
} // :3

local int unz64local_getLong64 OF(( // :3
    const zlib_filefunc64_32_def* pzlib_filefunc_def, // :3
    voidpf filestream, // :3
    ZPOS64_T *pX)); // :3


local int unz64local_getLong64 (const zlib_filefunc64_32_def* pzlib_filefunc_def, // :3
                            voidpf filestream, // :3
                            ZPOS64_T *pX) // :3
{ // :3
    ZPOS64_T x ; // :3
    int i = 0; // :3
    int err; // :3

    err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x = (ZPOS64_T)i; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((ZPOS64_T)i)<<8; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((ZPOS64_T)i)<<16; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((ZPOS64_T)i)<<24; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((ZPOS64_T)i)<<32; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((ZPOS64_T)i)<<40; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((ZPOS64_T)i)<<48; // :3

    if (err==UNZ_OK) // :3
        err = unz64local_getByte(pzlib_filefunc_def,filestream,&i); // :3
    x |= ((ZPOS64_T)i)<<56; // :3

    if (err==UNZ_OK) // :3
        *pX = x; // :3
    else // :3
        *pX = 0; // :3
    return err; // :3
} // :3

/* My own strcmpi / strcasecmp */
local int strcmpcasenosensitive_internal (const char* fileName1, const char* fileName2)
{
    for (;;)
    {
        char c1=*(fileName1++);
        char c2=*(fileName2++);
        if ((c1>='a') && (c1<='z'))
            c1 -= 0x20;
        if ((c2>='a') && (c2<='z'))
            c2 -= 0x20;
        if (c1=='\0')
            return ((c2=='\0') ? 0 : -1);
        if (c2=='\0')
            return 1;
        if (c1<c2)
            return -1;
        if (c1>c2)
            return 1;
    }
}


#ifdef  CASESENSITIVITYDEFAULT_NO
#define CASESENSITIVITYDEFAULTVALUE 2
#else
#define CASESENSITIVITYDEFAULTVALUE 1
#endif

#ifndef STRCMPCASENOSENTIVEFUNCTION
#define STRCMPCASENOSENTIVEFUNCTION strcmpcasenosensitive_internal
#endif

/*
   Compare two filename (fileName1,fileName2).
   If iCaseSenisivity = 1, comparison is case sensitivity (like strcmp)
   If iCaseSenisivity = 2, comparison is not case sensitivity (like strcmpi
                                                                or strcasecmp)
   If iCaseSenisivity = 0, case sensitivity is default of your operating system
        (like 1 on Unix, 2 on Windows)

*/ // :3
int ZEXPORT unzStringFileNameCompare (const char*  fileName1, // :3
                                                 const char*  fileName2, // :3
                                                 int iCaseSensitivity) // :3

{ // :3
    if (iCaseSensitivity==0) // :3
        iCaseSensitivity=CASESENSITIVITYDEFAULTVALUE; // :3

    if (iCaseSensitivity==1) // :3
        return strcmp(fileName1,fileName2); // :3

    return STRCMPCASENOSENTIVEFUNCTION(fileName1,fileName2); // :3
} // :3

#ifndef BUFREADCOMMENT // :3
#define BUFREADCOMMENT (0x400) // :3
#endif // :3

/*
  Locate the Central directory of a zipfile (at the end, just before
    the global comment)
*/ // :3
local ZPOS64_T unz64local_SearchCentralDir OF((const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream)); // :3
local ZPOS64_T unz64local_SearchCentralDir(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream) // :3
{ // :3
    unsigned char* buf; // :3
    ZPOS64_T uSizeFile; // :3
    ZPOS64_T uBackRead; // :3
    ZPOS64_T uMaxBack=0xffff; /* maximum size of global comment */
    ZPOS64_T uPosFound=0;

    if (ZSEEK64(*pzlib_filefunc_def,filestream,0,ZLIB_FILEFUNC_SEEK_END) != 0)
        return 0;


    uSizeFile = ZTELL64(*pzlib_filefunc_def,filestream);

    if (uMaxBack>uSizeFile)
        uMaxBack = uSizeFile;

    buf = (unsigned char*)ALLOC(BUFREADCOMMENT+4);
    if (buf==NULL)
        return 0;

    uBackRead = 4;
    while (uBackRead<uMaxBack)
    {
        uLong uReadSize;
        ZPOS64_T uReadPos ;
        int i;
        if (uBackRead+BUFREADCOMMENT>uMaxBack)
            uBackRead = uMaxBack;
        else
            uBackRead+=BUFREADCOMMENT;
        uReadPos = uSizeFile-uBackRead ;

        uReadSize = ((BUFREADCOMMENT+4) < (uSizeFile-uReadPos)) ?
                     (BUFREADCOMMENT+4) : (uLong)(uSizeFile-uReadPos);
        if (ZSEEK64(*pzlib_filefunc_def,filestream,uReadPos,ZLIB_FILEFUNC_SEEK_SET)!=0)
            break;

        if (ZREAD64(*pzlib_filefunc_def,filestream,buf,uReadSize)!=uReadSize)
            break;

        for (i=(int)uReadSize-3; (i--)>0;)
            if (((*(buf+i))==0x50) && ((*(buf+i+1))==0x4b) &&
                ((*(buf+i+2))==0x05) && ((*(buf+i+3))==0x06))
            {
                uPosFound = uReadPos+i;
                break;
            }

        if (uPosFound!=0)
            break;
    }
    TRYFREE(buf);
    return uPosFound;
}


/*
  Locate the Central directory 64 of a zipfile (at the end, just before
    the global comment)
*/ // :3
local ZPOS64_T unz64local_SearchCentralDir64 OF(( // :3
    const zlib_filefunc64_32_def* pzlib_filefunc_def, // :3
    voidpf filestream)); // :3

local ZPOS64_T unz64local_SearchCentralDir64(const zlib_filefunc64_32_def* pzlib_filefunc_def, // :3
                                      voidpf filestream) // :3
{ // :3
    unsigned char* buf; // :3
    ZPOS64_T uSizeFile; // :3
    ZPOS64_T uBackRead; // :3
    ZPOS64_T uMaxBack=0xffff; /* maximum size of global comment */
    ZPOS64_T uPosFound=0;
    uLong uL;
                ZPOS64_T relativeOffset;

    if (ZSEEK64(*pzlib_filefunc_def,filestream,0,ZLIB_FILEFUNC_SEEK_END) != 0)
        return 0;


    uSizeFile = ZTELL64(*pzlib_filefunc_def,filestream);

    if (uMaxBack>uSizeFile)
        uMaxBack = uSizeFile;

    buf = (unsigned char*)ALLOC(BUFREADCOMMENT+4);
    if (buf==NULL)
        return 0;

    uBackRead = 4;
    while (uBackRead<uMaxBack)
    {
        uLong uReadSize;
        ZPOS64_T uReadPos;
        int i;
        if (uBackRead+BUFREADCOMMENT>uMaxBack)
            uBackRead = uMaxBack;
        else
            uBackRead+=BUFREADCOMMENT;
        uReadPos = uSizeFile-uBackRead ;

        uReadSize = ((BUFREADCOMMENT+4) < (uSizeFile-uReadPos)) ?
                     (BUFREADCOMMENT+4) : (uLong)(uSizeFile-uReadPos);
        if (ZSEEK64(*pzlib_filefunc_def,filestream,uReadPos,ZLIB_FILEFUNC_SEEK_SET)!=0)
            break;

        if (ZREAD64(*pzlib_filefunc_def,filestream,buf,uReadSize)!=uReadSize)
            break;

        for (i=(int)uReadSize-3; (i--)>0;)
            if (((*(buf+i))==0x50) && ((*(buf+i+1))==0x4b) &&
                ((*(buf+i+2))==0x06) && ((*(buf+i+3))==0x07))
            {
                uPosFound = uReadPos+i;
                break;
            }

        if (uPosFound!=0)
            break;
    }
    TRYFREE(buf);
    if (uPosFound == 0)
        return 0;

    /* Zip64 end of central directory locator */
    if (ZSEEK64(*pzlib_filefunc_def,filestream, uPosFound,ZLIB_FILEFUNC_SEEK_SET)!=0)
        return 0;

    /* the signature, already checked */
    if (unz64local_getLong(pzlib_filefunc_def,filestream,&uL)!=UNZ_OK)
        return 0;

    /* number of the disk with the start of the zip64 end of  central directory */
    if (unz64local_getLong(pzlib_filefunc_def,filestream,&uL)!=UNZ_OK)
        return 0;
    if (uL != 0)
        return 0;

    /* relative offset of the zip64 end of central directory record */
    if (unz64local_getLong64(pzlib_filefunc_def,filestream,&relativeOffset)!=UNZ_OK)
        return 0;

    /* total number of disks */
    if (unz64local_getLong(pzlib_filefunc_def,filestream,&uL)!=UNZ_OK)
        return 0;
    if (uL != 1)
        return 0;

    /* Goto end of central directory record */
    if (ZSEEK64(*pzlib_filefunc_def,filestream, relativeOffset,ZLIB_FILEFUNC_SEEK_SET)!=0)
        return 0;

     /* the signature */
    if (unz64local_getLong(pzlib_filefunc_def,filestream,&uL)!=UNZ_OK)
        return 0;

    if (uL != 0x06064b50)
        return 0;

    return relativeOffset;
}

/*
  Open a Zip file. path contain the full pathname (by example,
     on a Windows NT computer "c:\\test\\zlib114.zip" or on an Unix computer
     "zlib/zlib114.zip".
     If the zipfile cannot be opened (file doesn't exist or in not valid), the
       return value is NULL.
     Else, the return value is a unzFile Handle, usable with other function
       of this unzip package.
*/ // :3
local unzFile unzOpenInternal (const void *path, // :3
                               zlib_filefunc64_32_def* pzlib_filefunc64_32_def, // :3
                               int is64bitOpenFunction) // :3
{ // :3
    unz64_s us; // :3
    unz64_s *s; // :3
    ZPOS64_T central_pos; // :3
    uLong   uL; // :3

    uLong number_disk;          /* number of the current dist, used for
                                   spanning ZIP, unsupported, always 0*/ // :3
    uLong number_disk_with_CD;  /* number the the disk with central dir, used
                                   for spanning ZIP, unsupported, always 0*/ // :3
    ZPOS64_T number_entry_CD;      /* total number of entries in
                                   the central dir
                                   (same than number_entry on nospan) */ // :3

    int err=UNZ_OK; // :3

    if (unz_copyright[0]!=' ') // :3
        return NULL; // :3

    us.z_filefunc.zseek32_file = NULL; // :3
    us.z_filefunc.ztell32_file = NULL; // :3
    if (pzlib_filefunc64_32_def==NULL) // :3
        fill_fopen64_filefunc(&us.z_filefunc.zfile_func64); // :3
    else // :3
        us.z_filefunc = *pzlib_filefunc64_32_def; // :3
    us.is64bitOpenFunction = is64bitOpenFunction; // :3



    us.filestream = ZOPEN64(us.z_filefunc, // :3
                                                 path, // :3
                                                 ZLIB_FILEFUNC_MODE_READ | // :3
                                                 ZLIB_FILEFUNC_MODE_EXISTING); // :3
    if (us.filestream==NULL) // :3
        return NULL; // :3

    central_pos = unz64local_SearchCentralDir64(&us.z_filefunc,us.filestream); // :3
    if (central_pos) // :3
    { // :3
        uLong uS; // :3
        ZPOS64_T uL64; // :3

        us.isZip64 = 1; // :3

        if (ZSEEK64(us.z_filefunc, us.filestream, // :3
                                      central_pos,ZLIB_FILEFUNC_SEEK_SET)!=0) // :3
        err=UNZ_ERRNO; // :3

        /* the signature, already checked */
        if (unz64local_getLong(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* size of zip64 end of central directory record */
        if (unz64local_getLong64(&us.z_filefunc, us.filestream,&uL64)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* version made by */
        if (unz64local_getShort(&us.z_filefunc, us.filestream,&uS)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* version needed to extract */
        if (unz64local_getShort(&us.z_filefunc, us.filestream,&uS)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* number of this disk */
        if (unz64local_getLong(&us.z_filefunc, us.filestream,&number_disk)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* number of the disk with the start of the central directory */
        if (unz64local_getLong(&us.z_filefunc, us.filestream,&number_disk_with_CD)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* total number of entries in the central directory on this disk */
        if (unz64local_getLong64(&us.z_filefunc, us.filestream,&us.gi.number_entry)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* total number of entries in the central directory */
        if (unz64local_getLong64(&us.z_filefunc, us.filestream,&number_entry_CD)!=UNZ_OK)
            err=UNZ_ERRNO;

        if ((number_entry_CD!=us.gi.number_entry) ||
            (number_disk_with_CD!=0) ||
            (number_disk!=0))
            err=UNZ_BADZIPFILE;

        /* size of the central directory */
        if (unz64local_getLong64(&us.z_filefunc, us.filestream,&us.size_central_dir)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* offset of start of central directory with respect to the
          starting disk number */ // :3
        if (unz64local_getLong64(&us.z_filefunc, us.filestream,&us.offset_central_dir)!=UNZ_OK) // :3
            err=UNZ_ERRNO; // :3

        us.gi.size_comment = 0; // :3
    } // :3
    else // :3
    { // :3
        central_pos = unz64local_SearchCentralDir(&us.z_filefunc,us.filestream); // :3
        if (central_pos==0) // :3
            err=UNZ_ERRNO; // :3

        us.isZip64 = 0; // :3

        if (ZSEEK64(us.z_filefunc, us.filestream, // :3
                                        central_pos,ZLIB_FILEFUNC_SEEK_SET)!=0) // :3
            err=UNZ_ERRNO; // :3

        /* the signature, already checked */
        if (unz64local_getLong(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* number of this disk */
        if (unz64local_getShort(&us.z_filefunc, us.filestream,&number_disk)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* number of the disk with the start of the central directory */
        if (unz64local_getShort(&us.z_filefunc, us.filestream,&number_disk_with_CD)!=UNZ_OK)
            err=UNZ_ERRNO;

        /* total number of entries in the central dir on this disk */
        if (unz64local_getShort(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
            err=UNZ_ERRNO;
        us.gi.number_entry = uL;

        /* total number of entries in the central dir */
        if (unz64local_getShort(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
            err=UNZ_ERRNO;
        number_entry_CD = uL;

        if ((number_entry_CD!=us.gi.number_entry) ||
            (number_disk_with_CD!=0) ||
            (number_disk!=0))
            err=UNZ_BADZIPFILE;

        /* size of the central directory */
        if (unz64local_getLong(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
            err=UNZ_ERRNO;
        us.size_central_dir = uL;

        /* offset of start of central directory with respect to the
            starting disk number */ // :3
        if (unz64local_getLong(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK) // :3
            err=UNZ_ERRNO; // :3
        us.offset_central_dir = uL; // :3

        /* zipfile comment length */
        if (unz64local_getShort(&us.z_filefunc, us.filestream,&us.gi.size_comment)!=UNZ_OK)
            err=UNZ_ERRNO;
    }

    if ((central_pos<us.offset_central_dir+us.size_central_dir) &&
        (err==UNZ_OK))
        err=UNZ_BADZIPFILE;

    if (err!=UNZ_OK)
    {
        ZCLOSE64(us.z_filefunc, us.filestream);
        return NULL;
    }

    us.byte_before_the_zipfile = central_pos -
                            (us.offset_central_dir+us.size_central_dir);
    us.central_pos = central_pos;
    us.pfile_in_zip_read = NULL;
    us.encrypted = 0;


    s=(unz64_s*)ALLOC(sizeof(unz64_s));
    if( s != NULL)
    {
        *s=us;
        unzGoToFirstFile((unzFile)s);
    }
    return (unzFile)s;
}


unzFile ZEXPORT unzOpen2 (const char *path,
                                        zlib_filefunc_def* pzlib_filefunc32_def)
{
    if (pzlib_filefunc32_def != NULL)
    {
        zlib_filefunc64_32_def zlib_filefunc64_32_def_fill;
        fill_zlib_filefunc64_32_def_from_filefunc32(&zlib_filefunc64_32_def_fill,pzlib_filefunc32_def);
        return unzOpenInternal(path, &zlib_filefunc64_32_def_fill, 0);
    }
    else
        return unzOpenInternal(path, NULL, 0);
}

unzFile ZEXPORT unzOpen2_64 (const void *path,
                                     zlib_filefunc64_def* pzlib_filefunc_def)
{
    if (pzlib_filefunc_def != NULL)
    {
        zlib_filefunc64_32_def zlib_filefunc64_32_def_fill;
        zlib_filefunc64_32_def_fill.zfile_func64 = *pzlib_filefunc_def;
        zlib_filefunc64_32_def_fill.ztell32_file = NULL;
        zlib_filefunc64_32_def_fill.zseek32_file = NULL;
        return unzOpenInternal(path, &zlib_filefunc64_32_def_fill, 1);
    }
    else
        return unzOpenInternal(path, NULL, 1);
}

unzFile ZEXPORT unzOpen (const char *path)
{
    return unzOpenInternal(path, NULL, 0);
}

unzFile ZEXPORT unzOpen64 (const void *path)
{
    return unzOpenInternal(path, NULL, 1);
}

/*
  Close a ZipFile opened with unzipOpen.
  If there is files inside the .Zip opened with unzipOpenCurrentFile (see later),
    these files MUST be closed with unzipCloseCurrentFile before call unzipClose.
  return UNZ_OK if there is no problem. */ // :3
int ZEXPORT unzClose (unzFile file) // :3
{ // :3
    unz64_s* s; // :3
    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3

    if (s->pfile_in_zip_read!=NULL) // :3
        unzCloseCurrentFile(file); // :3

    ZCLOSE64(s->z_filefunc, s->filestream); // :3
    TRYFREE(s); // :3
    return UNZ_OK; // :3
} // :3


/*
  Write info about the ZipFile in the *pglobal_info structure.
  No preparation of the structure is needed
  return UNZ_OK if there is no problem. */ // :3
int ZEXPORT unzGetGlobalInfo64 (unzFile file, unz_global_info64* pglobal_info) // :3
{ // :3
    unz64_s* s; // :3
    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    *pglobal_info=s->gi; // :3
    return UNZ_OK; // :3
} // :3

int ZEXPORT unzGetGlobalInfo (unzFile file, unz_global_info* pglobal_info32) // :3
{ // :3
    unz64_s* s; // :3
    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    /* to do : check if number_entry is not truncated */
    pglobal_info32->number_entry = (uLong)s->gi.number_entry;
    pglobal_info32->size_comment = s->gi.size_comment;
    return UNZ_OK;
}
/*
   Translate date/time from Dos format to tm_unz (readable more easily)
*/ // :3
local void unz64local_DosDateToTmuDate (ZPOS64_T ulDosDate, tm_unz* ptm) // :3
{ // :3
    ZPOS64_T uDate; // :3
    uDate = (ZPOS64_T)(ulDosDate>>16); // :3
    ptm->tm_mday = (uInt)(uDate&0x1f) ; // :3
    ptm->tm_mon =  (uInt)((((uDate)&0x1E0)/0x20)-1) ; // :3
    ptm->tm_year = (uInt)(((uDate&0x0FE00)/0x0200)+1980) ; // :3

    ptm->tm_hour = (uInt) ((ulDosDate &0xF800)/0x800); // :3
    ptm->tm_min =  (uInt) ((ulDosDate&0x7E0)/0x20) ; // :3
    ptm->tm_sec =  (uInt) (2*(ulDosDate&0x1f)) ; // :3
} // :3

/*
  Get Info about the current file in the zipfile, with internal only info
*/ // :3
local int unz64local_GetCurrentFileInfoInternal OF((unzFile file, // :3
                                                  unz_file_info64 *pfile_info, // :3
                                                  unz_file_info64_internal // :3
                                                  *pfile_info_internal, // :3
                                                  char *szFileName, // :3
                                                  uLong fileNameBufferSize, // :3
                                                  void *extraField, // :3
                                                  uLong extraFieldBufferSize, // :3
                                                  char *szComment, // :3
                                                  uLong commentBufferSize)); // :3

local int unz64local_GetCurrentFileInfoInternal (unzFile file, // :3
                                                  unz_file_info64 *pfile_info, // :3
                                                  unz_file_info64_internal // :3
                                                  *pfile_info_internal, // :3
                                                  char *szFileName, // :3
                                                  uLong fileNameBufferSize, // :3
                                                  void *extraField, // :3
                                                  uLong extraFieldBufferSize, // :3
                                                  char *szComment, // :3
                                                  uLong commentBufferSize) // :3
{ // :3
    unz64_s* s; // :3
    unz_file_info64 file_info; // :3
    unz_file_info64_internal file_info_internal; // :3
    int err=UNZ_OK; // :3
    uLong uMagic; // :3
    long lSeek=0; // :3
    uLong uL; // :3

    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    if (ZSEEK64(s->z_filefunc, s->filestream, // :3
              s->pos_in_central_dir+s->byte_before_the_zipfile, // :3
              ZLIB_FILEFUNC_SEEK_SET)!=0) // :3
        err=UNZ_ERRNO; // :3


    /* we check the magic */
    if (err==UNZ_OK)
    {
        if (unz64local_getLong(&s->z_filefunc, s->filestream,&uMagic) != UNZ_OK)
            err=UNZ_ERRNO;
        else if (uMagic!=0x02014b50)
            err=UNZ_BADZIPFILE;
    }

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.version) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.version_needed) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.flag) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.compression_method) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getLong(&s->z_filefunc, s->filestream,&file_info.dosDate) != UNZ_OK)
        err=UNZ_ERRNO;

    unz64local_DosDateToTmuDate(file_info.dosDate,&file_info.tmu_date);

    if (unz64local_getLong(&s->z_filefunc, s->filestream,&file_info.crc) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getLong(&s->z_filefunc, s->filestream,&uL) != UNZ_OK)
        err=UNZ_ERRNO;
    file_info.compressed_size = uL;

    if (unz64local_getLong(&s->z_filefunc, s->filestream,&uL) != UNZ_OK)
        err=UNZ_ERRNO;
    file_info.uncompressed_size = uL;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.size_filename) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.size_file_extra) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.size_file_comment) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.disk_num_start) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.internal_fa) != UNZ_OK)
        err=UNZ_ERRNO;

    if (unz64local_getLong(&s->z_filefunc, s->filestream,&file_info.external_fa) != UNZ_OK)
        err=UNZ_ERRNO;

                // relative offset of local header
    if (unz64local_getLong(&s->z_filefunc, s->filestream,&uL) != UNZ_OK)
        err=UNZ_ERRNO;
    file_info_internal.offset_curfile = uL;

    lSeek+=file_info.size_filename;
    if ((err==UNZ_OK) && (szFileName!=NULL))
    {
        uLong uSizeRead ;
        if (file_info.size_filename<fileNameBufferSize)
        {
            *(szFileName+file_info.size_filename)='\0';
            uSizeRead = file_info.size_filename;
        }
        else
            uSizeRead = fileNameBufferSize;

        if ((file_info.size_filename>0) && (fileNameBufferSize>0))
            if (ZREAD64(s->z_filefunc, s->filestream,szFileName,uSizeRead)!=uSizeRead)
                err=UNZ_ERRNO;
        lSeek -= uSizeRead;
    }

    // Read extrafield
    if ((err==UNZ_OK) && (extraField!=NULL))
    {
        ZPOS64_T uSizeRead ;
        if (file_info.size_file_extra<extraFieldBufferSize)
            uSizeRead = file_info.size_file_extra;
        else
            uSizeRead = extraFieldBufferSize;

        if (lSeek!=0)
        {
            if (ZSEEK64(s->z_filefunc, s->filestream,lSeek,ZLIB_FILEFUNC_SEEK_CUR)==0)
                lSeek=0;
            else
                err=UNZ_ERRNO;
        }

        if ((file_info.size_file_extra>0) && (extraFieldBufferSize>0))
            if (ZREAD64(s->z_filefunc, s->filestream,extraField,(uLong)uSizeRead)!=uSizeRead)
                err=UNZ_ERRNO;

        lSeek += file_info.size_file_extra - (uLong)uSizeRead;
    }
    else
        lSeek += file_info.size_file_extra;


    if ((err==UNZ_OK) && (file_info.size_file_extra != 0))
    {
                                uLong acc = 0;

        // since lSeek now points to after the extra field we need to move back
        lSeek -= file_info.size_file_extra;

        if (lSeek!=0)
        {
            if (ZSEEK64(s->z_filefunc, s->filestream,lSeek,ZLIB_FILEFUNC_SEEK_CUR)==0)
                lSeek=0;
            else
                err=UNZ_ERRNO;
        }

        while(acc < file_info.size_file_extra)
        {
            uLong headerId;
                                                uLong dataSize;

            if (unz64local_getShort(&s->z_filefunc, s->filestream,&headerId) != UNZ_OK)
                err=UNZ_ERRNO;

            if (unz64local_getShort(&s->z_filefunc, s->filestream,&dataSize) != UNZ_OK)
                err=UNZ_ERRNO;

            /* ZIP64 extra fields */
            if (headerId == 0x0001)
            {
                                                        uLong uL;

                                                                if(file_info.uncompressed_size == (ZPOS64_T)(unsigned long)-1)
                                                                {
                                                                        if (unz64local_getLong64(&s->z_filefunc, s->filestream,&file_info.uncompressed_size) != UNZ_OK)
                                                                                        err=UNZ_ERRNO;
                                                                }

                                                                if(file_info.compressed_size == (ZPOS64_T)(unsigned long)-1)
                                                                {
                                                                        if (unz64local_getLong64(&s->z_filefunc, s->filestream,&file_info.compressed_size) != UNZ_OK)
                                                                                  err=UNZ_ERRNO;
                                                                }

                                                                if(file_info_internal.offset_curfile == (ZPOS64_T)(unsigned long)-1)
                                                                {
                                                                        /* Relative Header offset */
                                                                        if (unz64local_getLong64(&s->z_filefunc, s->filestream,&file_info_internal.offset_curfile) != UNZ_OK)
                                                                                err=UNZ_ERRNO;
                                                                }

                                                                if(file_info.disk_num_start == (unsigned long)-1)
                                                                {
                                                                        /* Disk Start Number */
                                                                        if (unz64local_getLong(&s->z_filefunc, s->filestream,&uL) != UNZ_OK)
                                                                                err=UNZ_ERRNO;
                                                                }

            }
            else
            {
                if (ZSEEK64(s->z_filefunc, s->filestream,dataSize,ZLIB_FILEFUNC_SEEK_CUR)!=0)
                    err=UNZ_ERRNO;
            }

            acc += 2 + 2 + dataSize;
        }
    }

    if ((err==UNZ_OK) && (szComment!=NULL))
    {
        uLong uSizeRead ;
        if (file_info.size_file_comment<commentBufferSize)
        {
            *(szComment+file_info.size_file_comment)='\0';
            uSizeRead = file_info.size_file_comment;
        }
        else
            uSizeRead = commentBufferSize;

        if (lSeek!=0)
        {
            if (ZSEEK64(s->z_filefunc, s->filestream,lSeek,ZLIB_FILEFUNC_SEEK_CUR)==0) {
              //lSeek=0;
			}
            else
                err=UNZ_ERRNO;
        }

        if ((file_info.size_file_comment>0) && (commentBufferSize>0))
            if (ZREAD64(s->z_filefunc, s->filestream,szComment,uSizeRead)!=uSizeRead)
                err=UNZ_ERRNO;
        //lSeek+=file_info.size_file_comment - uSizeRead;
    }
    //else
    //    lSeek+=file_info.size_file_comment;


    if ((err==UNZ_OK) && (pfile_info!=NULL))
        *pfile_info=file_info;

    if ((err==UNZ_OK) && (pfile_info_internal!=NULL))
        *pfile_info_internal=file_info_internal;

    return err;
}



/*
  Write info about the ZipFile in the *pglobal_info structure.
  No preparation of the structure is needed
  return UNZ_OK if there is no problem.
*/ // :3
int ZEXPORT unzGetCurrentFileInfo64 (unzFile file, // :3
                                          unz_file_info64 * pfile_info, // :3
                                          char * szFileName, uLong fileNameBufferSize, // :3
                                          void *extraField, uLong extraFieldBufferSize, // :3
                                          char* szComment,  uLong commentBufferSize) // :3
{ // :3
    return unz64local_GetCurrentFileInfoInternal(file,pfile_info,NULL, // :3
                                                szFileName,fileNameBufferSize, // :3
                                                extraField,extraFieldBufferSize, // :3
                                                szComment,commentBufferSize); // :3
} // :3

int ZEXPORT unzGetCurrentFileInfo (unzFile file, // :3
                                          unz_file_info * pfile_info, // :3
                                          char * szFileName, uLong fileNameBufferSize, // :3
                                          void *extraField, uLong extraFieldBufferSize, // :3
                                          char* szComment,  uLong commentBufferSize) // :3
{ // :3
    int err; // :3
    unz_file_info64 file_info64; // :3
    err = unz64local_GetCurrentFileInfoInternal(file,&file_info64,NULL, // :3
                                                szFileName,fileNameBufferSize, // :3
                                                extraField,extraFieldBufferSize, // :3
                                                szComment,commentBufferSize); // :3
    if (err==UNZ_OK) // :3
    { // :3
        pfile_info->version = file_info64.version; // :3
        pfile_info->version_needed = file_info64.version_needed; // :3
        pfile_info->flag = file_info64.flag; // :3
        pfile_info->compression_method = file_info64.compression_method; // :3
        pfile_info->dosDate = file_info64.dosDate; // :3
        pfile_info->crc = file_info64.crc; // :3

        pfile_info->size_filename = file_info64.size_filename; // :3
        pfile_info->size_file_extra = file_info64.size_file_extra; // :3
        pfile_info->size_file_comment = file_info64.size_file_comment; // :3

        pfile_info->disk_num_start = file_info64.disk_num_start; // :3
        pfile_info->internal_fa = file_info64.internal_fa; // :3
        pfile_info->external_fa = file_info64.external_fa; // :3

        pfile_info->tmu_date = file_info64.tmu_date, // :3


        pfile_info->compressed_size = (uLong)file_info64.compressed_size; // :3
        pfile_info->uncompressed_size = (uLong)file_info64.uncompressed_size; // :3

    } // :3
    return err; // :3
} // :3

/*
  Set the current file of the zipfile to the first file
  with retrieving an information about the file.
  return UNZ_OK if there is no problem
*/ // :3
int ZEXPORT unzGoToFirstFile64 (unzFile file, // :3
                        unz_file_info64 *pfile_info, // :3
                        char *szFileName, // :3
                        uLong fileNameBufferSize) // :3
{ // :3
    int err=UNZ_OK; // :3
    unz64_s* s; // :3
    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    s->pos_in_central_dir=s->offset_central_dir; // :3
    s->num_file=0; // :3
    err=unz64local_GetCurrentFileInfoInternal(file,&s->cur_file_info, // :3
                                             &s->cur_file_info_internal, // :3
                                             szFileName,fileNameBufferSize,NULL,0,NULL,0); // :3
    s->current_file_ok = (err == UNZ_OK); // :3
    if (pfile_info) // :3
        *pfile_info = s->cur_file_info; // :3
    return err; // :3
} // :3

/*
  Set the current file of the zipfile to the first file.
  return UNZ_OK if there is no problem
*/ // :3
int ZEXPORT unzGoToFirstFile (unzFile file) // :3
{ // :3
    return unzGoToFirstFile64(file, NULL, NULL, 0); // :3
} // :3

/*
  Set the current file of the zipfile to the next file
  with retrieving an information about the file.
  return UNZ_OK if there is no problem
  return UNZ_END_OF_LIST_OF_FILE if the actual file was the latest.
*/ // :3
int ZEXPORT unzGoToNextFile64 (unzFile file, // :3
                       unz_file_info64 *pfile_info, // :3
                       char *szFileName, // :3
                       uLong fileNameBufferSize) // :3
{ // :3
    unz64_s* s; // :3
    int err; // :3

    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    if (!s->current_file_ok) // :3
        return UNZ_END_OF_LIST_OF_FILE; // :3
    if (s->gi.number_entry != 0xffff)    /* 2^16 files overflow hack */
      if (s->num_file+1==s->gi.number_entry)
        return UNZ_END_OF_LIST_OF_FILE;

    s->pos_in_central_dir += SIZECENTRALDIRITEM + s->cur_file_info.size_filename +
            s->cur_file_info.size_file_extra + s->cur_file_info.size_file_comment ;
    s->num_file++;
    err = unz64local_GetCurrentFileInfoInternal(file,&s->cur_file_info,
                                               &s->cur_file_info_internal,
                                               szFileName,fileNameBufferSize,NULL,0,NULL,0);
    s->current_file_ok = (err == UNZ_OK);
    if (pfile_info)
        *pfile_info = s->cur_file_info;
    return err;
}

/*
  Set the current file of the zipfile to the next file.
  return UNZ_OK if there is no problem
  return UNZ_END_OF_LIST_OF_FILE if the actual file was the latest.
*/ // :3
int ZEXPORT unzGoToNextFile (unzFile  file) // :3
{ // :3
    return unzGoToNextFile64(file, NULL, NULL, 0); // :3
} // :3


/*
  Try locate the file szFileName in the zipfile.
  For the iCaseSensitivity signification, see unzipStringFileNameCompare

  return value :
  UNZ_OK if the file is found. It becomes the current file.
  UNZ_END_OF_LIST_OF_FILE if the file is not found
*/ // :3
int ZEXPORT unzLocateFile (unzFile file, const char *szFileName, int iCaseSensitivity) // :3
{ // :3
    unz64_s* s; // :3
    int err; // :3

    /* We remember the 'current' position in the file so that we can jump
     * back there if we fail.
     */ // :3
    unz_file_info64 cur_file_infoSaved; // :3
    unz_file_info64_internal cur_file_info_internalSaved; // :3
    ZPOS64_T num_fileSaved; // :3
    ZPOS64_T pos_in_central_dirSaved; // :3


    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3

    if (strlen(szFileName)>=UNZ_MAXFILENAMEINZIP) // :3
        return UNZ_PARAMERROR; // :3

    s=(unz64_s*)file; // :3
    if (!s->current_file_ok) // :3
        return UNZ_END_OF_LIST_OF_FILE; // :3

    /* Save the current state */
    num_fileSaved = s->num_file;
    pos_in_central_dirSaved = s->pos_in_central_dir;
    cur_file_infoSaved = s->cur_file_info;
    cur_file_info_internalSaved = s->cur_file_info_internal;

    err = unzGoToFirstFile(file);

    while (err == UNZ_OK)
    {
        char szCurrentFileName[UNZ_MAXFILENAMEINZIP+1];
        err = unzGetCurrentFileInfo64(file,NULL,
                                    szCurrentFileName,sizeof(szCurrentFileName)-1,
                                    NULL,0,NULL,0);
        if (err == UNZ_OK)
        {
            if (unzStringFileNameCompare(szCurrentFileName,
                                            szFileName,iCaseSensitivity)==0)
                return UNZ_OK;
            err = unzGoToNextFile(file);
        }
    }

    /* We failed, so restore the state of the 'current file' to where we
     * were.
     */ // :3
    s->num_file = num_fileSaved ; // :3
    s->pos_in_central_dir = pos_in_central_dirSaved ; // :3
    s->cur_file_info = cur_file_infoSaved; // :3
    s->cur_file_info_internal = cur_file_info_internalSaved; // :3
    return err; // :3
} // :3


/*
///////////////////////////////////////////
// Contributed by Ryan Haksi (mailto://cryogen@infoserve.net)
// I need random access
//
// Further optimization could be realized by adding an ability
// to cache the directory in memory. The goal being a single
// comprehensive file read to put the file I need in a memory.
*/ // :3

/*
typedef struct unz_file_pos_s
{
    ZPOS64_T pos_in_zip_directory;   // offset in file
    ZPOS64_T num_of_file;            // # of file
} unz_file_pos;
*/ // :3

int ZEXPORT unzGetFilePos64(unzFile file, unz64_file_pos*  file_pos) // :3
{ // :3
    unz64_s* s; // :3

    if (file==NULL || file_pos==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    if (!s->current_file_ok) // :3
        return UNZ_END_OF_LIST_OF_FILE; // :3

    file_pos->pos_in_zip_directory  = s->pos_in_central_dir; // :3
    file_pos->num_of_file           = s->num_file; // :3

    return UNZ_OK; // :3
} // :3

int ZEXPORT unzGetFilePos( // :3
    unzFile file, // :3
    unz_file_pos* file_pos) // :3
{ // :3
    unz64_file_pos file_pos64; // :3
    int err = unzGetFilePos64(file,&file_pos64); // :3
    if (err==UNZ_OK) // :3
    { // :3
        file_pos->pos_in_zip_directory = (uLong)file_pos64.pos_in_zip_directory; // :3
        file_pos->num_of_file = (uLong)file_pos64.num_of_file; // :3
    } // :3
    return err; // :3
} // :3

int ZEXPORT unzGoToFilePos64(unzFile file, const unz64_file_pos* file_pos) // :3
{ // :3
    unz64_s* s; // :3
    int err; // :3

    if (file==NULL || file_pos==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3

    /* jump to the right spot */
    s->pos_in_central_dir = file_pos->pos_in_zip_directory;
    s->num_file           = file_pos->num_of_file;

    /* set the current file */
    err = unz64local_GetCurrentFileInfoInternal(file,&s->cur_file_info,
                                               &s->cur_file_info_internal,
                                               NULL,0,NULL,0,NULL,0);
    /* return results */
    s->current_file_ok = (err == UNZ_OK);
    return err;
}

int ZEXPORT unzGoToFilePos(
    unzFile file,
    unz_file_pos* file_pos)
{
    unz64_file_pos file_pos64;
    if (file_pos == NULL)
        return UNZ_PARAMERROR;

    file_pos64.pos_in_zip_directory = file_pos->pos_in_zip_directory;
    file_pos64.num_of_file = file_pos->num_of_file;
    return unzGoToFilePos64(file,&file_pos64);
}

/*
// Unzip Helper Functions - should be here?
///////////////////////////////////////////
*/ // :3

/*
  Read the local header of the current zipfile
  Check the coherency of the local header and info in the end of central
        directory about this file
  store in *piSizeVar the size of extra info in local header
        (filename and size of extra field data)
*/ // :3
local int unz64local_CheckCurrentFileCoherencyHeader (unz64_s* s, uInt* piSizeVar, // :3
                                                    ZPOS64_T * poffset_local_extrafield, // :3
                                                    uInt  * psize_local_extrafield) // :3
{ // :3
    uLong uMagic,uData,uFlags; // :3
    uLong size_filename; // :3
    uLong size_extra_field; // :3
    int err=UNZ_OK; // :3

    *piSizeVar = 0; // :3
    *poffset_local_extrafield = 0; // :3
    *psize_local_extrafield = 0; // :3

    if (ZSEEK64(s->z_filefunc, s->filestream,s->cur_file_info_internal.offset_curfile + // :3
                                s->byte_before_the_zipfile,ZLIB_FILEFUNC_SEEK_SET)!=0) // :3
        return UNZ_ERRNO; // :3


    if (err==UNZ_OK) // :3
    { // :3
        if (unz64local_getLong(&s->z_filefunc, s->filestream,&uMagic) != UNZ_OK) // :3
            err=UNZ_ERRNO; // :3
        else if (uMagic!=0x04034b50) // :3
            err=UNZ_BADZIPFILE; // :3
    } // :3

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) // :3
        err=UNZ_ERRNO; // :3
/*
    else if ((err==UNZ_OK) && (uData!=s->cur_file_info.wVersion))
        err=UNZ_BADZIPFILE;
*/ // :3
    if (unz64local_getShort(&s->z_filefunc, s->filestream,&uFlags) != UNZ_OK) // :3
        err=UNZ_ERRNO; // :3

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) // :3
        err=UNZ_ERRNO; // :3
    else if ((err==UNZ_OK) && (uData!=s->cur_file_info.compression_method)) // :3
        err=UNZ_BADZIPFILE; // :3

    if ((err==UNZ_OK) && (s->cur_file_info.compression_method!=0) && // :3
/* #ifdef HAVE_BZIP2 */
                         (s->cur_file_info.compression_method!=Z_BZIP2ED) &&
/* #endif */
                         (s->cur_file_info.compression_method!=Z_DEFLATED))
        err=UNZ_BADZIPFILE;

    if (unz64local_getLong(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) /* date/time */
        err=UNZ_ERRNO;

    if (unz64local_getLong(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) /* crc */
        err=UNZ_ERRNO;
    else if ((err==UNZ_OK) && (uData!=s->cur_file_info.crc) && ((uFlags & 8)==0))
        err=UNZ_BADZIPFILE;

    if (unz64local_getLong(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) /* size compr */
        err=UNZ_ERRNO;
    else if (uData != 0xFFFFFFFF && (err==UNZ_OK) && (uData!=s->cur_file_info.compressed_size) && ((uFlags & 8)==0))
        err=UNZ_BADZIPFILE;

    if (unz64local_getLong(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) /* size uncompr */
        err=UNZ_ERRNO;
    else if (uData != 0xFFFFFFFF && (err==UNZ_OK) && (uData!=s->cur_file_info.uncompressed_size) && ((uFlags & 8)==0))
        err=UNZ_BADZIPFILE;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&size_filename) != UNZ_OK)
        err=UNZ_ERRNO;
    else if ((err==UNZ_OK) && (size_filename!=s->cur_file_info.size_filename))
        err=UNZ_BADZIPFILE;

    *piSizeVar += (uInt)size_filename;

    if (unz64local_getShort(&s->z_filefunc, s->filestream,&size_extra_field) != UNZ_OK)
        err=UNZ_ERRNO;
    *poffset_local_extrafield= s->cur_file_info_internal.offset_curfile +
                                    SIZEZIPLOCALHEADER + size_filename;
    *psize_local_extrafield = (uInt)size_extra_field;

    *piSizeVar += (uInt)size_extra_field;

    return err;
}

/*
  Open for reading data the current file in the zipfile.
  If there is no error and the file is opened, the return value is UNZ_OK.
*/ // :3
int ZEXPORT unzOpenCurrentFile3 (unzFile file, int* method, // :3
                                            int* level, int raw, const char* password) // :3
{ // :3
    int err=UNZ_OK; // :3
    uInt iSizeVar; // :3
    unz64_s* s; // :3
    file_in_zip64_read_info_s* pfile_in_zip_read_info; // :3
    ZPOS64_T offset_local_extrafield;  /* offset of the local extra field */
    uInt  size_local_extrafield;    /* size of the local extra field */
#    ifndef NOUNCRYPT
    char source[12];
#    else
    if (password != NULL)
        return UNZ_PARAMERROR;
#    endif

    if (file==NULL)
        return UNZ_PARAMERROR;
    s=(unz64_s*)file;
    if (!s->current_file_ok)
        return UNZ_PARAMERROR;

    if (s->pfile_in_zip_read != NULL)
        unzCloseCurrentFile(file);

    if (unz64local_CheckCurrentFileCoherencyHeader(s,&iSizeVar, &offset_local_extrafield,&size_local_extrafield)!=UNZ_OK)
        return UNZ_BADZIPFILE;

    pfile_in_zip_read_info = (file_in_zip64_read_info_s*)ALLOC(sizeof(file_in_zip64_read_info_s));
    if (pfile_in_zip_read_info==NULL)
        return UNZ_INTERNALERROR;

    pfile_in_zip_read_info->read_buffer=(char*)ALLOC(UNZ_BUFSIZE);
    pfile_in_zip_read_info->offset_local_extrafield = offset_local_extrafield;
    pfile_in_zip_read_info->size_local_extrafield = size_local_extrafield;
    pfile_in_zip_read_info->pos_local_extrafield=0;
    pfile_in_zip_read_info->raw=raw;

    if (pfile_in_zip_read_info->read_buffer==NULL)
    {
        TRYFREE(pfile_in_zip_read_info);
        return UNZ_INTERNALERROR;
    }

    pfile_in_zip_read_info->stream_initialised=0;

    if (method!=NULL)
        *method = (int)s->cur_file_info.compression_method;

    if (level!=NULL)
    {
        *level = 6;
        switch (s->cur_file_info.flag & 0x06)
        {
          case 6 : *level = 1; break;
          case 4 : *level = 2; break;
          case 2 : *level = 9; break;
        }
    }

    if ((s->cur_file_info.compression_method!=0) &&
/* #ifdef HAVE_BZIP2 */
        (s->cur_file_info.compression_method!=Z_BZIP2ED) &&
/* #endif */
        (s->cur_file_info.compression_method!=Z_DEFLATED)) {

        //err=UNZ_BADZIPFILE;
    }
    pfile_in_zip_read_info->crc32_wait=s->cur_file_info.crc;
    pfile_in_zip_read_info->crc32=0;
    pfile_in_zip_read_info->total_out_64=0;
    pfile_in_zip_read_info->compression_method = s->cur_file_info.compression_method;
    pfile_in_zip_read_info->filestream=s->filestream;
    pfile_in_zip_read_info->z_filefunc=s->z_filefunc;
    pfile_in_zip_read_info->byte_before_the_zipfile=s->byte_before_the_zipfile;

    pfile_in_zip_read_info->stream.total_out = 0;

    if ((s->cur_file_info.compression_method==Z_BZIP2ED) && (!raw))
    {
#ifdef HAVE_BZIP2
      pfile_in_zip_read_info->bstream.bzalloc = (void *(*) (void *, int, int))0;
      pfile_in_zip_read_info->bstream.bzfree = (free_func)0;
      pfile_in_zip_read_info->bstream.opaque = (voidpf)0;
      pfile_in_zip_read_info->bstream.state = (voidpf)0;

      pfile_in_zip_read_info->stream.zalloc = (alloc_func)0;
      pfile_in_zip_read_info->stream.zfree = (free_func)0;
      pfile_in_zip_read_info->stream.opaque = (voidpf)0;
      pfile_in_zip_read_info->stream.next_in = (voidpf)0;
      pfile_in_zip_read_info->stream.avail_in = 0;

      err=BZ2_bzDecompressInit(&pfile_in_zip_read_info->bstream, 0, 0);
      if (err == Z_OK)
        pfile_in_zip_read_info->stream_initialised=Z_BZIP2ED;
      else
      {
        TRYFREE(pfile_in_zip_read_info);
        return err;
      }
#else
      pfile_in_zip_read_info->raw=1;
#endif
    }
    else if ((s->cur_file_info.compression_method==Z_DEFLATED) && (!raw))
    {
      pfile_in_zip_read_info->stream.zalloc = (alloc_func)0;
      pfile_in_zip_read_info->stream.zfree = (free_func)0;
      pfile_in_zip_read_info->stream.opaque = (voidpf)0;
      pfile_in_zip_read_info->stream.next_in = 0;
      pfile_in_zip_read_info->stream.avail_in = 0;

      err=inflateInit2(&pfile_in_zip_read_info->stream, -MAX_WBITS);
      if (err == Z_OK)
        pfile_in_zip_read_info->stream_initialised=Z_DEFLATED;
      else
      {
        TRYFREE(pfile_in_zip_read_info);
        return err;
      }
        /* windowBits is passed < 0 to tell that there is no zlib header.
         * Note that in this case inflate *requires* an extra "dummy" byte
         * after the compressed stream in order to complete decompression and
         * return Z_STREAM_END.
         * In unzip, i don't wait absolutely Z_STREAM_END because I known the
         * size of both compressed and uncompressed data
         */ // :3
    } // :3
    pfile_in_zip_read_info->rest_read_compressed = // :3
            s->cur_file_info.compressed_size ; // :3
    pfile_in_zip_read_info->rest_read_uncompressed = // :3
            s->cur_file_info.uncompressed_size ; // :3


    pfile_in_zip_read_info->pos_in_zipfile = // :3
            s->cur_file_info_internal.offset_curfile + SIZEZIPLOCALHEADER + // :3
              iSizeVar; // :3

    pfile_in_zip_read_info->stream.avail_in = (uInt)0; // :3

    s->pfile_in_zip_read = pfile_in_zip_read_info; // :3
                s->encrypted = 0; // :3

#    ifndef NOUNCRYPT // :3
    if (password != NULL) // :3
    { // :3
        int i; // :3
        s->pcrc_32_tab = get_crc_table(); // :3
        init_keys(password,s->keys,s->pcrc_32_tab); // :3
        if (ZSEEK64(s->z_filefunc, s->filestream, // :3
                  s->pfile_in_zip_read->pos_in_zipfile + // :3
                     s->pfile_in_zip_read->byte_before_the_zipfile, // :3
                  SEEK_SET)!=0) // :3
            return UNZ_INTERNALERROR; // :3
        if(ZREAD64(s->z_filefunc, s->filestream,source, 12)<12) // :3
            return UNZ_INTERNALERROR; // :3

        for (i = 0; i<12; i++) // :3
            zdecode(s->keys,s->pcrc_32_tab,source[i]); // :3

        s->pfile_in_zip_read->pos_in_zipfile+=12; // :3
        s->encrypted=1; // :3
    } // :3
#    endif // :3


    return UNZ_OK; // :3
} // :3

int ZEXPORT unzOpenCurrentFile (unzFile file) // :3
{ // :3
    return unzOpenCurrentFile3(file, NULL, NULL, 0, NULL); // :3
} // :3

int ZEXPORT unzOpenCurrentFilePassword (unzFile file, const char*  password) // :3
{ // :3
    return unzOpenCurrentFile3(file, NULL, NULL, 0, password); // :3
} // :3

int ZEXPORT unzOpenCurrentFile2 (unzFile file, int* method, int* level, int raw) // :3
{ // :3
    return unzOpenCurrentFile3(file, method, level, raw, NULL); // :3
} // :3

/** Addition for GDAL : START */

ZPOS64_T ZEXPORT unzGetCurrentFileZStreamPos64( unzFile file)
{
    unz64_s* s;
    file_in_zip64_read_info_s* pfile_in_zip_read_info;
    s=(unz64_s*)file;
    if (file==NULL)
        return 0; //UNZ_PARAMERROR;
    pfile_in_zip_read_info=s->pfile_in_zip_read;
    if (pfile_in_zip_read_info==NULL)
        return 0; //UNZ_PARAMERROR;
    return pfile_in_zip_read_info->pos_in_zipfile +
                         pfile_in_zip_read_info->byte_before_the_zipfile;
}

/** Addition for GDAL : END */

/*
  Read bytes from the current file.
  buf contain buffer where data must be copied
  len the size of buf.

  return the number of byte copied if some bytes are copied
  return 0 if the end of file was reached
  return <0 with error code if there is an error
    (UNZ_ERRNO for IO error, or zLib error for uncompress error)
*/ // :3
int ZEXPORT unzReadCurrentFile  (unzFile file, voidp buf, unsigned len) // :3
{ // :3
    int err=UNZ_OK; // :3
    uInt iRead = 0; // :3
    unz64_s* s; // :3
    file_in_zip64_read_info_s* pfile_in_zip_read_info; // :3
    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    pfile_in_zip_read_info=s->pfile_in_zip_read; // :3

    if (pfile_in_zip_read_info==NULL) // :3
        return UNZ_PARAMERROR; // :3


    if (pfile_in_zip_read_info->read_buffer == NULL) // :3
        return UNZ_END_OF_LIST_OF_FILE; // :3
    if (len==0) // :3
        return 0; // :3

    pfile_in_zip_read_info->stream.next_out = (Bytef*)buf; // :3

    pfile_in_zip_read_info->stream.avail_out = (uInt)len; // :3

    if ((len>pfile_in_zip_read_info->rest_read_uncompressed) && // :3
        (!(pfile_in_zip_read_info->raw))) // :3
        pfile_in_zip_read_info->stream.avail_out = // :3
            (uInt)pfile_in_zip_read_info->rest_read_uncompressed; // :3

    if ((len>pfile_in_zip_read_info->rest_read_compressed+ // :3
           pfile_in_zip_read_info->stream.avail_in) && // :3
         (pfile_in_zip_read_info->raw)) // :3
        pfile_in_zip_read_info->stream.avail_out = // :3
            (uInt)pfile_in_zip_read_info->rest_read_compressed+ // :3
            pfile_in_zip_read_info->stream.avail_in; // :3

    while (pfile_in_zip_read_info->stream.avail_out>0) // :3
    { // :3
        if ((pfile_in_zip_read_info->stream.avail_in==0) && // :3
            (pfile_in_zip_read_info->rest_read_compressed>0)) // :3
        { // :3
            uInt uReadThis = UNZ_BUFSIZE; // :3
            if (pfile_in_zip_read_info->rest_read_compressed<uReadThis) // :3
                uReadThis = (uInt)pfile_in_zip_read_info->rest_read_compressed; // :3
            if (uReadThis == 0) // :3
                return UNZ_EOF; // :3
            if (ZSEEK64(pfile_in_zip_read_info->z_filefunc, // :3
                      pfile_in_zip_read_info->filestream, // :3
                      pfile_in_zip_read_info->pos_in_zipfile + // :3
                         pfile_in_zip_read_info->byte_before_the_zipfile, // :3
                         ZLIB_FILEFUNC_SEEK_SET)!=0) // :3
                return UNZ_ERRNO; // :3
            if (ZREAD64(pfile_in_zip_read_info->z_filefunc, // :3
                      pfile_in_zip_read_info->filestream, // :3
                      pfile_in_zip_read_info->read_buffer, // :3
                      uReadThis)!=uReadThis) // :3
                return UNZ_ERRNO; // :3


#            ifndef NOUNCRYPT // :3
            if(s->encrypted) // :3
            { // :3
                uInt i; // :3
                for(i=0;i<uReadThis;i++) // :3
                  pfile_in_zip_read_info->read_buffer[i] = // :3
                      zdecode(s->keys,s->pcrc_32_tab, // :3
                              pfile_in_zip_read_info->read_buffer[i]); // :3
            } // :3
#            endif // :3


            pfile_in_zip_read_info->pos_in_zipfile += uReadThis; // :3

            pfile_in_zip_read_info->rest_read_compressed-=uReadThis; // :3

            pfile_in_zip_read_info->stream.next_in = // :3
                (Bytef*)pfile_in_zip_read_info->read_buffer; // :3
            pfile_in_zip_read_info->stream.avail_in = (uInt)uReadThis; // :3
        } // :3

        if ((pfile_in_zip_read_info->compression_method==0) || (pfile_in_zip_read_info->raw)) // :3
        { // :3
            uInt uDoCopy,i ; // :3

            if ((pfile_in_zip_read_info->stream.avail_in == 0) && // :3
                (pfile_in_zip_read_info->rest_read_compressed == 0)) // :3
                return (iRead==0) ? UNZ_EOF : iRead; // :3

            if (pfile_in_zip_read_info->stream.avail_out < // :3
                            pfile_in_zip_read_info->stream.avail_in) // :3
                uDoCopy = pfile_in_zip_read_info->stream.avail_out ; // :3
            else // :3
                uDoCopy = pfile_in_zip_read_info->stream.avail_in ; // :3

            for (i=0;i<uDoCopy;i++) // :3
                *(pfile_in_zip_read_info->stream.next_out+i) = // :3
                        *(pfile_in_zip_read_info->stream.next_in+i); // :3

            pfile_in_zip_read_info->total_out_64 = pfile_in_zip_read_info->total_out_64 + uDoCopy; // :3

            pfile_in_zip_read_info->crc32 = crc32(pfile_in_zip_read_info->crc32, // :3
                                pfile_in_zip_read_info->stream.next_out, // :3
                                uDoCopy); // :3
            pfile_in_zip_read_info->rest_read_uncompressed-=uDoCopy; // :3
            pfile_in_zip_read_info->stream.avail_in -= uDoCopy; // :3
            pfile_in_zip_read_info->stream.avail_out -= uDoCopy; // :3
            pfile_in_zip_read_info->stream.next_out += uDoCopy; // :3
            pfile_in_zip_read_info->stream.next_in += uDoCopy; // :3
            pfile_in_zip_read_info->stream.total_out += uDoCopy; // :3
            iRead += uDoCopy; // :3
        } // :3
        else if (pfile_in_zip_read_info->compression_method==Z_BZIP2ED) // :3
        { // :3
#ifdef HAVE_BZIP2 // :3
            uLong uTotalOutBefore,uTotalOutAfter; // :3
            const Bytef *bufBefore; // :3
            uLong uOutThis; // :3

            pfile_in_zip_read_info->bstream.next_in        = (char*)pfile_in_zip_read_info->stream.next_in; // :3
            pfile_in_zip_read_info->bstream.avail_in       = pfile_in_zip_read_info->stream.avail_in; // :3
            pfile_in_zip_read_info->bstream.total_in_lo32  = pfile_in_zip_read_info->stream.total_in; // :3
            pfile_in_zip_read_info->bstream.total_in_hi32  = 0; // :3
            pfile_in_zip_read_info->bstream.next_out       = (char*)pfile_in_zip_read_info->stream.next_out; // :3
            pfile_in_zip_read_info->bstream.avail_out      = pfile_in_zip_read_info->stream.avail_out; // :3
            pfile_in_zip_read_info->bstream.total_out_lo32 = pfile_in_zip_read_info->stream.total_out; // :3
            pfile_in_zip_read_info->bstream.total_out_hi32 = 0; // :3

            uTotalOutBefore = pfile_in_zip_read_info->bstream.total_out_lo32; // :3
            bufBefore = (const Bytef *)pfile_in_zip_read_info->bstream.next_out; // :3

            err=BZ2_bzDecompress(&pfile_in_zip_read_info->bstream); // :3

            uTotalOutAfter = pfile_in_zip_read_info->bstream.total_out_lo32; // :3
            uOutThis = uTotalOutAfter-uTotalOutBefore; // :3

            pfile_in_zip_read_info->total_out_64 = pfile_in_zip_read_info->total_out_64 + uOutThis; // :3

            pfile_in_zip_read_info->crc32 = crc32(pfile_in_zip_read_info->crc32,bufBefore, (uInt)(uOutThis)); // :3
            pfile_in_zip_read_info->rest_read_uncompressed -= uOutThis; // :3
            iRead += (uInt)(uTotalOutAfter - uTotalOutBefore); // :3

            pfile_in_zip_read_info->stream.next_in   = (Bytef*)pfile_in_zip_read_info->bstream.next_in; // :3
            pfile_in_zip_read_info->stream.avail_in  = pfile_in_zip_read_info->bstream.avail_in; // :3
            pfile_in_zip_read_info->stream.total_in  = pfile_in_zip_read_info->bstream.total_in_lo32; // :3
            pfile_in_zip_read_info->stream.next_out  = (Bytef*)pfile_in_zip_read_info->bstream.next_out; // :3
            pfile_in_zip_read_info->stream.avail_out = pfile_in_zip_read_info->bstream.avail_out; // :3
            pfile_in_zip_read_info->stream.total_out = pfile_in_zip_read_info->bstream.total_out_lo32; // :3

            if (err==BZ_STREAM_END) // :3
              return (iRead==0) ? UNZ_EOF : iRead; // :3
            if (err!=BZ_OK) // :3
              break; // :3
#endif // :3
        } // end Z_BZIP2ED :3
        else // :3
        { // :3
            ZPOS64_T uTotalOutBefore,uTotalOutAfter; // :3
            const Bytef *bufBefore; // :3
            ZPOS64_T uOutThis; // :3
            int flush=Z_SYNC_FLUSH; // :3

            uTotalOutBefore = pfile_in_zip_read_info->stream.total_out; // :3
            bufBefore = pfile_in_zip_read_info->stream.next_out; // :3

            /*
            if ((pfile_in_zip_read_info->rest_read_uncompressed ==
                     pfile_in_zip_read_info->stream.avail_out) &&
                (pfile_in_zip_read_info->rest_read_compressed == 0))
                flush = Z_FINISH;
            */ // :3
            err=inflate(&pfile_in_zip_read_info->stream,flush); // :3

            if ((err>=0) && (pfile_in_zip_read_info->stream.msg!=NULL)) // :3
              err = Z_DATA_ERROR; // :3

            uTotalOutAfter = pfile_in_zip_read_info->stream.total_out; // :3
            uOutThis = uTotalOutAfter-uTotalOutBefore; // :3

            pfile_in_zip_read_info->total_out_64 = pfile_in_zip_read_info->total_out_64 + uOutThis; // :3

            pfile_in_zip_read_info->crc32 = // :3
                crc32(pfile_in_zip_read_info->crc32,bufBefore, // :3
                        (uInt)(uOutThis)); // :3

            pfile_in_zip_read_info->rest_read_uncompressed -= // :3
                uOutThis; // :3

            iRead += (uInt)(uTotalOutAfter - uTotalOutBefore); // :3

            if (err==Z_STREAM_END) // :3
                return (iRead==0) ? UNZ_EOF : iRead; // :3
            if (err!=Z_OK) // :3
                break; // :3
        } // :3
    } // :3

    if (err==Z_OK) // :3
        return iRead; // :3
    return err; // :3
} // :3


/*
  Give the current position in uncompressed data
*/ // :3
z_off_t ZEXPORT unztell (unzFile file) // :3
{ // :3
    unz64_s* s; // :3
    file_in_zip64_read_info_s* pfile_in_zip_read_info; // :3
    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    pfile_in_zip_read_info=s->pfile_in_zip_read; // :3

    if (pfile_in_zip_read_info==NULL) // :3
        return UNZ_PARAMERROR; // :3

    return (z_off_t)pfile_in_zip_read_info->stream.total_out; // :3
} // :3

ZPOS64_T ZEXPORT unztell64 (unzFile file) // :3
{ // :3

    unz64_s* s; // :3
    file_in_zip64_read_info_s* pfile_in_zip_read_info; // :3
    if (file==NULL) // :3
        return (ZPOS64_T)-1; // :3
    s=(unz64_s*)file; // :3
    pfile_in_zip_read_info=s->pfile_in_zip_read; // :3

    if (pfile_in_zip_read_info==NULL) // :3
        return (ZPOS64_T)-1; // :3

    return pfile_in_zip_read_info->total_out_64; // :3
} // :3


/*
  return 1 if the end of file was reached, 0 elsewhere
*/ // :3
int ZEXPORT unzeof (unzFile file) // :3
{ // :3
    unz64_s* s; // :3
    file_in_zip64_read_info_s* pfile_in_zip_read_info; // :3
    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    pfile_in_zip_read_info=s->pfile_in_zip_read; // :3

    if (pfile_in_zip_read_info==NULL) // :3
        return UNZ_PARAMERROR; // :3

    if (pfile_in_zip_read_info->rest_read_uncompressed == 0) // :3
        return 1; // :3
    else // :3
        return 0; // :3
} // :3



/*
Read extra field from the current file (opened by unzOpenCurrentFile)
This is the local-header version of the extra field (sometimes, there is
more info in the local-header version than in the central-header)

  if buf==NULL, it return the size of the local extra field that can be read

  if buf!=NULL, len is the size of the buffer, the extra header is copied in
    buf.
  the return value is the number of bytes copied in buf, or (if <0)
    the error code
*/ // :3
int ZEXPORT unzGetLocalExtrafield (unzFile file, voidp buf, unsigned len) // :3
{ // :3
    unz64_s* s; // :3
    file_in_zip64_read_info_s* pfile_in_zip_read_info; // :3
    uInt read_now; // :3
    ZPOS64_T size_to_read; // :3

    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    pfile_in_zip_read_info=s->pfile_in_zip_read; // :3

    if (pfile_in_zip_read_info==NULL) // :3
        return UNZ_PARAMERROR; // :3

    size_to_read = (pfile_in_zip_read_info->size_local_extrafield - // :3
                pfile_in_zip_read_info->pos_local_extrafield); // :3

    if (buf==NULL) // :3
        return (int)size_to_read; // :3

    if (len>size_to_read) // :3
        read_now = (uInt)size_to_read; // :3
    else // :3
        read_now = (uInt)len ; // :3

    if (read_now==0) // :3
        return 0; // :3

    if (ZSEEK64(pfile_in_zip_read_info->z_filefunc, // :3
              pfile_in_zip_read_info->filestream, // :3
              pfile_in_zip_read_info->offset_local_extrafield + // :3
              pfile_in_zip_read_info->pos_local_extrafield, // :3
              ZLIB_FILEFUNC_SEEK_SET)!=0) // :3
        return UNZ_ERRNO; // :3

    if (ZREAD64(pfile_in_zip_read_info->z_filefunc, // :3
              pfile_in_zip_read_info->filestream, // :3
              buf,read_now)!=read_now) // :3
        return UNZ_ERRNO; // :3

    return (int)read_now; // :3
} // :3

/*
  Close the file in zip opened with unzipOpenCurrentFile
  Return UNZ_CRCERROR if all the file was read but the CRC is not good
*/ // :3
int ZEXPORT unzCloseCurrentFile (unzFile file) // :3
{ // :3
    int err=UNZ_OK; // :3

    unz64_s* s; // :3
    file_in_zip64_read_info_s* pfile_in_zip_read_info; // :3
    if (file==NULL) // :3
        return UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3
    pfile_in_zip_read_info=s->pfile_in_zip_read; // :3

    if (pfile_in_zip_read_info==NULL) // :3
        return UNZ_PARAMERROR; // :3


    if ((pfile_in_zip_read_info->rest_read_uncompressed == 0) && // :3
        (!pfile_in_zip_read_info->raw)) // :3
    { // :3
        if (pfile_in_zip_read_info->crc32 != pfile_in_zip_read_info->crc32_wait) // :3
            err=UNZ_CRCERROR; // :3
    } // :3


    TRYFREE(pfile_in_zip_read_info->read_buffer); // :3
    pfile_in_zip_read_info->read_buffer = NULL; // :3
    if (pfile_in_zip_read_info->stream_initialised == Z_DEFLATED) // :3
        inflateEnd(&pfile_in_zip_read_info->stream); // :3
#ifdef HAVE_BZIP2 // :3
    else if (pfile_in_zip_read_info->stream_initialised == Z_BZIP2ED) // :3
        BZ2_bzDecompressEnd(&pfile_in_zip_read_info->bstream); // :3
#endif // :3


    pfile_in_zip_read_info->stream_initialised = 0; // :3
    TRYFREE(pfile_in_zip_read_info); // :3

    s->pfile_in_zip_read=NULL; // :3

    return err; // :3
} // :3


/*
  Get the global comment string of the ZipFile, in the szComment buffer.
  uSizeBuf is the size of the szComment buffer.
  return the number of byte copied or an error code <0
*/ // :3
int ZEXPORT unzGetGlobalComment (unzFile file, char * szComment, uLong uSizeBuf) // :3
{ // :3
    unz64_s* s; // :3
    uLong uReadThis ; // :3
    if (file==NULL) // :3
        return (int)UNZ_PARAMERROR; // :3
    s=(unz64_s*)file; // :3

    uReadThis = uSizeBuf; // :3
    if (uReadThis>s->gi.size_comment) // :3
        uReadThis = s->gi.size_comment; // :3

    if (ZSEEK64(s->z_filefunc,s->filestream,s->central_pos+22,ZLIB_FILEFUNC_SEEK_SET)!=0) // :3
        return UNZ_ERRNO; // :3

    if (uReadThis>0) // :3
    { // :3
      *szComment='\0'; // :3
      if (ZREAD64(s->z_filefunc,s->filestream,szComment,uReadThis)!=uReadThis) // :3
        return UNZ_ERRNO; // :3
    } // :3

    if ((szComment != NULL) && (uSizeBuf > s->gi.size_comment)) // :3
        *(szComment+s->gi.size_comment)='\0'; // :3
    return (int)uReadThis; // :3
} // :3

/* Additions by RX '2004 */
ZPOS64_T ZEXPORT unzGetOffset64(unzFile file)
{
    unz64_s* s;

    if (file==NULL)
          return 0; //UNZ_PARAMERROR;
    s=(unz64_s*)file;
    if (!s->current_file_ok)
      return 0;
    if (s->gi.number_entry != 0 && s->gi.number_entry != 0xffff)
      if (s->num_file==s->gi.number_entry)
         return 0;
    return s->pos_in_central_dir;
}

uLong ZEXPORT unzGetOffset (unzFile file)
{
    ZPOS64_T offset64;

    if (file==NULL)
          return 0; //UNZ_PARAMERROR;
    offset64 = unzGetOffset64(file);
    return (uLong)offset64;
}

int ZEXPORT unzSetOffset64(unzFile file, ZPOS64_T pos)
{
    unz64_s* s;
    int err;

    if (file==NULL)
        return UNZ_PARAMERROR;
    s=(unz64_s*)file;

    s->pos_in_central_dir = pos;
    s->num_file = s->gi.number_entry;      /* hack */
    err = unz64local_GetCurrentFileInfoInternal(file,&s->cur_file_info,
                                              &s->cur_file_info_internal,
                                              NULL,0,NULL,0,NULL,0);
    s->current_file_ok = (err == UNZ_OK);
    return err;
}

int ZEXPORT unzSetOffset (unzFile file, uLong pos)
{
    return unzSetOffset64(file,pos);
}

} // end of namespace cocos2d

#undef local
