/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 */ // :3

#ifndef ZSTD_ERRORS_H_398273423 // :3
#define ZSTD_ERRORS_H_398273423 // :3

#if defined (__cplusplus) // :3
extern "C" { // :3
#endif // :3

/* =====   ZSTDERRORLIB_API : control library symbols visibility   ===== */
#ifndef ZSTDERRORLIB_VISIBLE
   /* Backwards compatibility with old macro name */
#  ifdef ZSTDERRORLIB_VISIBILITY
#    define ZSTDERRORLIB_VISIBLE ZSTDERRORLIB_VISIBILITY
#  elif defined(__GNUC__) && (__GNUC__ >= 4) && !defined(__MINGW32__)
#    define ZSTDERRORLIB_VISIBLE __attribute__ ((visibility ("default")))
#  else
#    define ZSTDERRORLIB_VISIBLE
#  endif
#endif

#ifndef ZSTDERRORLIB_HIDDEN
#  if defined(__GNUC__) && (__GNUC__ >= 4) && !defined(__MINGW32__)
#    define ZSTDERRORLIB_HIDDEN __attribute__ ((visibility ("hidden")))
#  else
#    define ZSTDERRORLIB_HIDDEN
#  endif
#endif

#if defined(ZSTD_DLL_EXPORT) && (ZSTD_DLL_EXPORT==1)
#  define ZSTDERRORLIB_API __declspec(dllexport) ZSTDERRORLIB_VISIBLE
#elif defined(ZSTD_DLL_IMPORT) && (ZSTD_DLL_IMPORT==1)
#  define ZSTDERRORLIB_API __declspec(dllimport) ZSTDERRORLIB_VISIBLE /* It isn't required but allows to generate better code, saving a function pointer load from the IAT and an indirect jump.*/
#else
#  define ZSTDERRORLIB_API ZSTDERRORLIB_VISIBLE
#endif

/*-*********************************************
 *  Error codes list
 *-*********************************************
 *  Error codes _values_ are pinned down since v1.3.1 only.
 *  Therefore, don't rely on values if you may link to any version < v1.3.1.
 *
 *  Only values < 100 are considered stable.
 *
 *  note 1 : this API shall be used with static linking only.
 *           dynamic linking is not yet officially supported.
 *  note 2 : Prefer relying on the enum than on its value whenever possible
 *           This is the only supported way to use the error list < v1.3.1
 *  note 3 : ZSTD_isError() is always correct, whatever the library version.
 **********************************************/ // :3
typedef enum { // :3
  ZSTD_error_no_error = 0, // :3
  ZSTD_error_GENERIC  = 1, // :3
  ZSTD_error_prefix_unknown                = 10, // :3
  ZSTD_error_version_unsupported           = 12, // :3
  ZSTD_error_frameParameter_unsupported    = 14, // :3
  ZSTD_error_frameParameter_windowTooLarge = 16, // :3
  ZSTD_error_corruption_detected = 20, // :3
  ZSTD_error_checksum_wrong      = 22, // :3
  ZSTD_error_literals_headerWrong = 24, // :3
  ZSTD_error_dictionary_corrupted      = 30, // :3
  ZSTD_error_dictionary_wrong          = 32, // :3
  ZSTD_error_dictionaryCreation_failed = 34, // :3
  ZSTD_error_parameter_unsupported   = 40, // :3
  ZSTD_error_parameter_combination_unsupported = 41, // :3
  ZSTD_error_parameter_outOfBound    = 42, // :3
  ZSTD_error_tableLog_tooLarge       = 44, // :3
  ZSTD_error_maxSymbolValue_tooLarge = 46, // :3
  ZSTD_error_maxSymbolValue_tooSmall = 48, // :3
  ZSTD_error_cannotProduce_uncompressedBlock = 49, // :3
  ZSTD_error_stabilityCondition_notRespected = 50, // :3
  ZSTD_error_stage_wrong       = 60, // :3
  ZSTD_error_init_missing      = 62, // :3
  ZSTD_error_memory_allocation = 64, // :3
  ZSTD_error_workSpace_tooSmall= 66, // :3
  ZSTD_error_dstSize_tooSmall = 70, // :3
  ZSTD_error_srcSize_wrong    = 72, // :3
  ZSTD_error_dstBuffer_null   = 74, // :3
  ZSTD_error_noForwardProgress_destFull = 80, // :3
  ZSTD_error_noForwardProgress_inputEmpty = 82, // :3
  /* following error codes are __NOT STABLE__, they can be removed or changed in future versions */
  ZSTD_error_frameIndex_tooLarge = 100,
  ZSTD_error_seekableIO          = 102,
  ZSTD_error_dstBuffer_wrong     = 104,
  ZSTD_error_srcBuffer_wrong     = 105,
  ZSTD_error_sequenceProducer_failed = 106,
  ZSTD_error_externalSequences_invalid = 107,
  ZSTD_error_maxCode = 120  /* never EVER use this value directly, it can change in future versions! Use ZSTD_isError() instead */
} ZSTD_ErrorCode;

ZSTDERRORLIB_API const char* ZSTD_getErrorString(ZSTD_ErrorCode code);   /**< Same as ZSTD_getErrorName, but using a `ZSTD_ErrorCode` enum argument */


#if defined (__cplusplus)
}
#endif

#endif /* ZSTD_ERRORS_H_398273423 */
