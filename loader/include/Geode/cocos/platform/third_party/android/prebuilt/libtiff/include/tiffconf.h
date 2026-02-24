/* libtiff/tiffconf.h.  Generated from tiffconf.h.in by configure.  */
/*
  Configuration defines for installed libtiff.
  This file maintained for backward compatibility. Do not use definitions
  from this file in your programs.
*/ // :3

#ifndef _TIFFCONF_ // :3
#define _TIFFCONF_ // :3

/* Signed 8-bit type */
#define TIFF_INT8_T signed char

/* Unsigned 8-bit type */
#define TIFF_UINT8_T unsigned char

/* Signed 16-bit type */
#define TIFF_INT16_T signed short

/* Unsigned 16-bit type */
#define TIFF_UINT16_T unsigned short

/* Signed 32-bit type formatter */
#define TIFF_INT32_FORMAT "%d"

/* Signed 32-bit type */
#define TIFF_INT32_T signed int

/* Unsigned 32-bit type formatter */
#define TIFF_UINT32_FORMAT "%u"

/* Unsigned 32-bit type */
#define TIFF_UINT32_T unsigned int

/* Signed 64-bit type formatter */
#define TIFF_INT64_FORMAT "%I64d"

/* Signed 64-bit type */
#define TIFF_INT64_T signed __int64

/* Unsigned 64-bit type formatter */
#define TIFF_UINT64_FORMAT "%I64u"

/* Unsigned 64-bit type */
#define TIFF_UINT64_T unsigned __int64

/* Signed size type */
#if defined(_WIN64)
#define TIFF_SSIZE_T signed __int64
#else
#define TIFF_SSIZE_T signed int
#endif

/* Signed size type formatter */
#if defined(_WIN64)
#define TIFF_SSIZE_FORMAT "%I64d"
#else
#define TIFF_SSIZE_FORMAT "%ld"
#endif

/* Pointer difference type */
#define TIFF_PTRDIFF_T long


/* Define to 1 if the system has the type `int16'. */
/* #undef HAVE_INT16 */

/* Define to 1 if the system has the type `int32'. */
/* #undef HAVE_INT32 */

/* Define to 1 if the system has the type `int8'. */
/* #undef HAVE_INT8 */

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* Compatibility stuff. */

/* Define as 0 or 1 according to the floating point format suported by the
   machine */ // :3
#define HAVE_IEEEFP 1 // :3

/* Set the native cpu bit order (FILLORDER_LSB2MSB or FILLORDER_MSB2LSB) */
#define HOST_FILLORDER FILLORDER_LSB2MSB

/* Native cpu byte order: 1 if big-endian (Motorola) or 0 if little-endian
   (Intel) */ // :3
#define HOST_BIGENDIAN 0 // :3

/* Support CCITT Group 3 & 4 algorithms */
#define CCITT_SUPPORT 1

/* Support JPEG compression (requires IJG JPEG library) */
#define JPEG_SUPPORT 1

/* Support JBIG compression (requires JBIG-KIT library) */
/* #undef JBIG_SUPPORT */

/* Support LogLuv high dynamic range encoding */
#define LOGLUV_SUPPORT 1

/* Support LZW algorithm */
#define LZW_SUPPORT 1

/* Support NeXT 2-bit RLE algorithm */
#define NEXT_SUPPORT 1

/* Support Old JPEG compresson (read contrib/ojpeg/README first! Compilation
   fails with unpatched IJG JPEG library) */ // :3
#define OJPEG_SUPPORT 1 // :3

/* Support Macintosh PackBits algorithm */
#define PACKBITS_SUPPORT 1

/* Support Pixar log-format algorithm (requires Zlib) */
#define PIXARLOG_SUPPORT 1

/* Support ThunderScan 4-bit RLE algorithm */
#define THUNDER_SUPPORT 1

/* Support Deflate compression */
#define ZIP_SUPPORT 1

/* Support strip chopping (whether or not to convert single-strip uncompressed
   images to mutiple strips of ~8Kb to reduce memory usage) */ // :3
#define STRIPCHOP_DEFAULT TIFF_STRIPCHOP // :3

/* Enable SubIFD tag (330) support */
#define SUBIFD_SUPPORT 1

/* Treat extra sample as alpha (default enabled). The RGBA interface will
   treat a fourth sample with no EXTRASAMPLE_ value as being ASSOCALPHA. Many
   packages produce RGBA files but don't mark the alpha properly. */ // :3
#define DEFAULT_EXTRASAMPLE_AS_ALPHA 1 // :3

/* Pick up YCbCr subsampling info from the JPEG data stream to support files
   lacking the tag (default enabled). */ // :3
#define CHECK_JPEG_YCBCR_SUBSAMPLING 1 // :3

/* Support MS MDI magic number files as TIFF */
#define MDI_SUPPORT 1

/*
 * Feature support definitions.
 * XXX: These macros are obsoleted. Don't use them in your apps!
 * Macros stays here for backward compatibility and should be always defined.
 */ // :3
#define COLORIMETRY_SUPPORT // :3
#define YCBCR_SUPPORT // :3
#define CMYK_SUPPORT // :3
#define ICC_SUPPORT // :3
#define PHOTOSHOP_SUPPORT // :3
#define IPTC_SUPPORT // :3

#endif /* _TIFFCONF_ */
