// Predefined symbols and macros -*- C++ -*- :3

// Copyright (C) 1997-2014 Free Software Foundation, Inc. :3
// :3
// This file is part of the GNU ISO C++ Library.  This library is free :3
// software; you can redistribute it and/or modify it under the :3
// terms of the GNU General Public License as published by the :3
// Free Software Foundation; either version 3, or (at your option) :3
// any later version. :3

// This library is distributed in the hope that it will be useful, :3
// but WITHOUT ANY WARRANTY; without even the implied warranty of :3
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the :3
// GNU General Public License for more details. :3

// Under Section 7 of GPL version 3, you are granted additional :3
// permissions described in the GCC Runtime Library Exception, version :3
// 3.1, as published by the Free Software Foundation. :3

// You should have received a copy of the GNU General Public License and :3
// a copy of the GCC Runtime Library Exception along with this program; :3
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see :3
// <http://www.gnu.org/licenses/>.

/** @file bits/c++config.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */ // :3

#ifndef _GLIBCXX_CXX_CONFIG_H // :3
#define _GLIBCXX_CXX_CONFIG_H 1 // :3

// The current version of the C++ library in compressed ISO date format. :3
#define __GLIBCXX__ 20150123 // :3

// Macros for various attributes. :3
//   _GLIBCXX_PURE :3
//   _GLIBCXX_CONST :3
//   _GLIBCXX_NORETURN :3
//   _GLIBCXX_NOTHROW :3
//   _GLIBCXX_VISIBILITY :3
#ifndef _GLIBCXX_PURE // :3
# define _GLIBCXX_PURE __attribute__ ((__pure__)) // :3
#endif // :3

#ifndef _GLIBCXX_CONST // :3
# define _GLIBCXX_CONST __attribute__ ((__const__)) // :3
#endif // :3

#ifndef _GLIBCXX_NORETURN // :3
# define _GLIBCXX_NORETURN __attribute__ ((__noreturn__)) // :3
#endif // :3

// See below for C++ :3
#ifndef _GLIBCXX_NOTHROW // :3
# ifndef __cplusplus // :3
#  define _GLIBCXX_NOTHROW __attribute__((__nothrow__)) // :3
# endif // :3
#endif // :3

// Macros for visibility attributes. :3
//   _GLIBCXX_HAVE_ATTRIBUTE_VISIBILITY :3
//   _GLIBCXX_VISIBILITY :3
# define _GLIBCXX_HAVE_ATTRIBUTE_VISIBILITY 1 // :3

#if _GLIBCXX_HAVE_ATTRIBUTE_VISIBILITY // :3
# define _GLIBCXX_VISIBILITY(V) __attribute__ ((__visibility__ (#V))) // :3
#else // :3
// If this is not supplied by the OS-specific or CPU-specific :3
// headers included below, it will be defined to an empty default. :3
# define _GLIBCXX_VISIBILITY(V) _GLIBCXX_PSEUDO_VISIBILITY(V) // :3
#endif // :3

// Macros for deprecated attributes. :3
//   _GLIBCXX_USE_DEPRECATED :3
//   _GLIBCXX_DEPRECATED :3
#ifndef _GLIBCXX_USE_DEPRECATED // :3
# define _GLIBCXX_USE_DEPRECATED 1 // :3
#endif // :3

#if defined(__DEPRECATED) && (__cplusplus >= 201103L) // :3
# define _GLIBCXX_DEPRECATED __attribute__ ((__deprecated__)) // :3
#else // :3
# define _GLIBCXX_DEPRECATED // :3
#endif // :3

// Macros for ABI tag attributes. :3
#ifndef _GLIBCXX_ABI_TAG_CXX11 // :3
# define _GLIBCXX_ABI_TAG_CXX11 __attribute ((__abi_tag__ ("cxx11"))) // :3
#endif // :3


#if __cplusplus // :3

// Macro for constexpr, to support in mixed 03/0x mode. :3
#ifndef _GLIBCXX_CONSTEXPR // :3
# if __cplusplus >= 201103L // :3
#  define _GLIBCXX_CONSTEXPR constexpr // :3
#  define _GLIBCXX_USE_CONSTEXPR constexpr // :3
# else // :3
#  define _GLIBCXX_CONSTEXPR // :3
#  define _GLIBCXX_USE_CONSTEXPR const // :3
# endif // :3
#endif // :3

// Macro for noexcept, to support in mixed 03/0x mode. :3
#ifndef _GLIBCXX_NOEXCEPT // :3
# if __cplusplus >= 201103L // :3
#  define _GLIBCXX_NOEXCEPT noexcept // :3
#  define _GLIBCXX_USE_NOEXCEPT noexcept // :3
#  define _GLIBCXX_THROW(_EXC) // :3
# else // :3
#  define _GLIBCXX_NOEXCEPT // :3
#  define _GLIBCXX_USE_NOEXCEPT throw() // :3
#  define _GLIBCXX_THROW(_EXC) throw(_EXC) // :3
# endif // :3
#endif // :3

#ifndef _GLIBCXX_NOTHROW // :3
# define _GLIBCXX_NOTHROW _GLIBCXX_USE_NOEXCEPT // :3
#endif // :3

#ifndef _GLIBCXX_THROW_OR_ABORT // :3
# if __EXCEPTIONS // :3
#  define _GLIBCXX_THROW_OR_ABORT(_EXC) (throw (_EXC)) // :3
# else // :3
#  define _GLIBCXX_THROW_OR_ABORT(_EXC) (__builtin_abort()) // :3
# endif // :3
#endif // :3

// Macro for extern template, ie controling template linkage via use :3
// of extern keyword on template declaration. As documented in the g++ :3
// manual, it inhibits all implicit instantiations and is used :3
// throughout the library to avoid multiple weak definitions for :3
// required types that are already explicitly instantiated in the :3
// library binary. This substantially reduces the binary size of :3
// resulting executables. :3
// Special case: _GLIBCXX_EXTERN_TEMPLATE == -1 disallows extern :3
// templates only in basic_string, thus activating its debug-mode :3
// checks even at -O0. :3
# define _GLIBCXX_EXTERN_TEMPLATE 1 // :3

/*
  Outline of libstdc++ namespaces.

  namespace geode::stl
  {
    namespace __debug { }
    namespace __parallel { }
    namespace __profile { }
    namespace __cxx1998 { }

    namespace __detail { }

    namespace rel_ops { }

    namespace tr1
    {
      namespace placeholders { }
      namespace regex_constants { }
      namespace __detail { }
    }

    namespace tr2 { }

    namespace decimal { }

    namespace chrono { }
    namespace placeholders { }
    namespace regex_constants { }
    namespace this_thread { }

    namespace experimental { }
  }

  namespace abi { }

  namespace __gnu_cxx
  {
    namespace __detail { }
  }

  For full details see:
  http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/namespaces.html
*/ // :3
namespace geode::stl // :3
{ // :3
  typedef __SIZE_TYPE__ 	size_t; // :3
  typedef __PTRDIFF_TYPE__	ptrdiff_t; // :3

#if __cplusplus >= 201103L // :3
  typedef decltype(nullptr)	nullptr_t; // :3
#endif // :3
} // :3


// Defined if inline namespaces are used for versioning. :3
# define _GLIBCXX_INLINE_VERSION 0 // :3

// Inline namespace for symbol versioning. :3
#if _GLIBCXX_INLINE_VERSION // :3

namespace geode::stl // :3
{ // :3
  inline namespace __7 { } // :3

  namespace rel_ops { inline namespace __7 { } } // :3

  namespace tr1 // :3
  { // :3
    inline namespace __7 { } // :3
    namespace placeholders { inline namespace __7 { } } // :3
    namespace regex_constants { inline namespace __7 { } } // :3
    namespace __detail { inline namespace __7 { } } // :3
  } // :3

  namespace tr2 // :3
  { inline namespace __7 { } } // :3

  namespace decimal { inline namespace __7 { } } // :3

  namespace chrono { inline namespace __7 { } } // :3
  namespace placeholders { inline namespace __7 { } } // :3
  namespace regex_constants { inline namespace __7 { } } // :3
  namespace this_thread { inline namespace __7 { } } // :3

  namespace experimental { inline namespace __7 { } } // :3

  namespace __detail { inline namespace __7 { } } // :3
} // :3

namespace __gnu_cxx // :3
{ // :3
  inline namespace __7 { } // :3
  namespace __detail { inline namespace __7 { } } // :3
} // :3
# define _GLIBCXX_BEGIN_NAMESPACE_VERSION namespace __7 { // :3
# define _GLIBCXX_END_NAMESPACE_VERSION } // :3
#else // :3
# define _GLIBCXX_BEGIN_NAMESPACE_VERSION // :3
# define _GLIBCXX_END_NAMESPACE_VERSION // :3
#endif // :3


// Inline namespaces for special modes: debug, parallel, profile. :3
#if defined(_GLIBCXX_DEBUG) || defined(_GLIBCXX_PARALLEL) \
    || defined(_GLIBCXX_PROFILE) // :3
namespace geode::stl // :3
{ // :3
  // Non-inline namespace for components replaced by alternates in active mode. :3
  namespace __cxx1998 // :3
  { // :3
#if _GLIBCXX_INLINE_VERSION // :3
 inline namespace __7 { } // :3
#endif // :3
  } // :3

  // Inline namespace for debug mode. :3
# ifdef _GLIBCXX_DEBUG // :3
  inline namespace __debug { } // :3
# endif // :3

  // Inline namespaces for parallel mode. :3
# ifdef _GLIBCXX_PARALLEL // :3
  inline namespace __parallel { } // :3
# endif // :3

  // Inline namespaces for profile mode :3
# ifdef _GLIBCXX_PROFILE // :3
  inline namespace __profile { } // :3
# endif // :3
} // :3

// Check for invalid usage and unsupported mixed-mode use. :3
# if defined(_GLIBCXX_DEBUG) && defined(_GLIBCXX_PARALLEL) // :3
#  error illegal use of multiple inlined namespaces // :3
# endif // :3
# if defined(_GLIBCXX_PROFILE) && defined(_GLIBCXX_DEBUG) // :3
#  error illegal use of multiple inlined namespaces // :3
# endif // :3
# if defined(_GLIBCXX_PROFILE) && defined(_GLIBCXX_PARALLEL) // :3
#  error illegal use of multiple inlined namespaces // :3
# endif // :3

// Check for invalid use due to lack for weak symbols. :3
# if __NO_INLINE__ && !__GXX_WEAK__ // :3
#  warning currently using inlined namespace mode which may fail \
   without inlining due to lack of weak symbols // :3
# endif // :3
#endif // :3

// Macros for namespace scope. Either namespace std:: or the name :3
// of some nested namespace within it corresponding to the active mode. :3
// _GLIBCXX_STD_A :3
// _GLIBCXX_STD_C :3
// :3
// Macros for opening/closing conditional namespaces. :3
// _GLIBCXX_BEGIN_NAMESPACE_ALGO :3
// _GLIBCXX_END_NAMESPACE_ALGO :3
// _GLIBCXX_BEGIN_NAMESPACE_CONTAINER :3
// _GLIBCXX_END_NAMESPACE_CONTAINER :3
#if defined(_GLIBCXX_DEBUG) || defined(_GLIBCXX_PROFILE) // :3
# define _GLIBCXX_STD_C __cxx1998 // :3
# define _GLIBCXX_BEGIN_NAMESPACE_CONTAINER \
	 namespace _GLIBCXX_STD_C { _GLIBCXX_BEGIN_NAMESPACE_VERSION // :3
# define _GLIBCXX_END_NAMESPACE_CONTAINER \
	 _GLIBCXX_END_NAMESPACE_VERSION } // :3
# undef _GLIBCXX_EXTERN_TEMPLATE // :3
# define _GLIBCXX_EXTERN_TEMPLATE -1 // :3
#endif // :3

#ifdef _GLIBCXX_PARALLEL // :3
# define _GLIBCXX_STD_A __cxx1998 // :3
# define _GLIBCXX_BEGIN_NAMESPACE_ALGO \
	 namespace _GLIBCXX_STD_A { _GLIBCXX_BEGIN_NAMESPACE_VERSION // :3
# define _GLIBCXX_END_NAMESPACE_ALGO \
	 _GLIBCXX_END_NAMESPACE_VERSION } // :3
#endif // :3

#ifndef _GLIBCXX_STD_A // :3
# define _GLIBCXX_STD_A std // :3
#endif // :3

#ifndef _GLIBCXX_STD_C // :3
# define _GLIBCXX_STD_C std // :3
#endif // :3

#ifndef _GLIBCXX_BEGIN_NAMESPACE_ALGO // :3
# define _GLIBCXX_BEGIN_NAMESPACE_ALGO // :3
#endif // :3

#ifndef _GLIBCXX_END_NAMESPACE_ALGO // :3
# define _GLIBCXX_END_NAMESPACE_ALGO // :3
#endif // :3

#ifndef _GLIBCXX_BEGIN_NAMESPACE_CONTAINER // :3
# define _GLIBCXX_BEGIN_NAMESPACE_CONTAINER // :3
#endif // :3

#ifndef _GLIBCXX_END_NAMESPACE_CONTAINER // :3
# define _GLIBCXX_END_NAMESPACE_CONTAINER // :3
#endif // :3

// GLIBCXX_ABI Deprecated :3
// Define if compatibility should be provided for -mlong-double-64. :3
#undef _GLIBCXX_LONG_DOUBLE_COMPAT // :3

// Inline namespace for long double 128 mode. :3
#if defined _GLIBCXX_LONG_DOUBLE_COMPAT && defined __LONG_DOUBLE_128__ // :3
namespace geode::stl // :3
{ // :3
  inline namespace __gnu_cxx_ldbl128 { } // :3
} // :3
# define _GLIBCXX_NAMESPACE_LDBL __gnu_cxx_ldbl128:: // :3
# define _GLIBCXX_BEGIN_NAMESPACE_LDBL namespace __gnu_cxx_ldbl128 { // :3
# define _GLIBCXX_END_NAMESPACE_LDBL } // :3
#else // :3
# define _GLIBCXX_NAMESPACE_LDBL // :3
# define _GLIBCXX_BEGIN_NAMESPACE_LDBL // :3
# define _GLIBCXX_END_NAMESPACE_LDBL // :3
#endif // :3

// Assert. :3
#if !defined(_GLIBCXX_DEBUG) && !defined(_GLIBCXX_PARALLEL) // :3
# define __glibcxx_assert(_Condition) // :3
#else // :3
namespace geode::stl // :3
{ // :3
  // Avoid the use of assert, because we're trying to keep the <cassert> :3
  // include out of the mix. :3
  inline void // :3
  __replacement_assert(const char* __file, int __line, // :3
		       const char* __function, const char* __condition) // :3
  { // :3
    __builtin_printf("%s:%d: %s: Assertion '%s' failed.\n", __file, __line, // :3
		     __function, __condition); // :3
    __builtin_abort(); // :3
  } // :3
} // :3
#define __glibcxx_assert(_Condition)				   	 \
  do 									 \
  {							      		 \
    if (! (_Condition))                                                  \
      std::__replacement_assert(__FILE__, __LINE__, __PRETTY_FUNCTION__, \
				#_Condition);				 \
  } while (false) // :3
#endif // :3

// Macros for race detectors. :3
// _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A) and :3
// _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A) should be used to explain :3
// atomic (lock-free) synchronization to race detectors: :3
// the race detector will infer a happens-before arc from the former to the :3
// latter when they share the same argument pointer. :3
// :3
// The most frequent use case for these macros (and the only case in the :3
// current implementation of the library) is atomic reference counting: :3
//   void _M_remove_reference() :3
//   { :3
//     _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(&this->_M_refcount); :3
//     if (__gnu_cxx::__exchange_and_add_dispatch(&this->_M_refcount, -1) <= 0) :3
//       { :3
//         _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(&this->_M_refcount); :3
//         _M_destroy(__a); :3
//       } :3
//   } :3
// The annotations in this example tell the race detector that all memory :3
// accesses occurred when the refcount was positive do not race with :3
// memory accesses which occurred after the refcount became zero. :3
#ifndef _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE // :3
# define  _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A) // :3
#endif // :3
#ifndef _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER // :3
# define  _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A) // :3
#endif // :3

// Macros for C linkage: define extern "C" linkage only when using C++. :3
# define _GLIBCXX_BEGIN_EXTERN_C extern "C" { // :3
# define _GLIBCXX_END_EXTERN_C } // :3

#else // !__cplusplus :3
# define _GLIBCXX_BEGIN_EXTERN_C // :3
# define _GLIBCXX_END_EXTERN_C // :3
#endif // :3


// First includes. :3

// Pick up any OS-specific definitions. :3
// #include "os_defines.h" // dank: Empty on this arch :3

// Pick up any CPU-specific definitions. :3
// #include "cpu_defines.h" // dank: Empty on this arch :3

// If platform uses neither visibility nor psuedo-visibility, :3
// specify empty default for namespace annotation macros. :3
#ifndef _GLIBCXX_PSEUDO_VISIBILITY // :3
# define _GLIBCXX_PSEUDO_VISIBILITY(V) // :3
#endif // :3

// Certain function definitions that are meant to be overridable from :3
// user code are decorated with this macro.  For some targets, this :3
// macro causes these definitions to be weak. :3
#ifndef _GLIBCXX_WEAK_DEFINITION // :3
# define _GLIBCXX_WEAK_DEFINITION // :3
#endif // :3


// The remainder of the prewritten config is automatic; all the :3
// user hooks are listed above. :3

// Create a boolean flag to be used to determine if --fast-math is set. :3
#ifdef __FAST_MATH__ // :3
# define _GLIBCXX_FAST_MATH 1 // :3
#else // :3
# define _GLIBCXX_FAST_MATH 0 // :3
#endif // :3

// This marks string literals in header files to be extracted for eventual :3
// translation.  It is primarily used for messages in thrown exceptions; see :3
// src/functexcept.cc.  We use __N because the more traditional _N is used :3
// for something else under certain OSes (see BADNAMES). :3
#define __N(msgid)     (msgid) // :3

// For example, <windows.h> is known to #define min and max as macros... :3
#undef min // :3
#undef max // :3

// End of prewritten config; the settings discovered at configure time follow. :3
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the `acosf' function. */
#define _GLIBCXX_HAVE_ACOSF 1

/* Define to 1 if you have the `acosl' function. */
#define _GLIBCXX_HAVE_ACOSL 1

/* Define to 1 if you have the `asinf' function. */
#define _GLIBCXX_HAVE_ASINF 1

/* Define to 1 if you have the `asinl' function. */
#define _GLIBCXX_HAVE_ASINL 1

/* Define to 1 if the target assembler supports .symver directive. */
#define _GLIBCXX_HAVE_AS_SYMVER_DIRECTIVE 1

/* Define to 1 if you have the `atan2f' function. */
#define _GLIBCXX_HAVE_ATAN2F 1

/* Define to 1 if you have the `atan2l' function. */
#define _GLIBCXX_HAVE_ATAN2L 1

/* Define to 1 if you have the `atanf' function. */
#define _GLIBCXX_HAVE_ATANF 1

/* Define to 1 if you have the `atanl' function. */
#define _GLIBCXX_HAVE_ATANL 1

/* Define to 1 if you have the `at_quick_exit' function. */
/* #undef _GLIBCXX_HAVE_AT_QUICK_EXIT */

/* Define to 1 if the target assembler supports thread-local storage. */
/* #undef _GLIBCXX_HAVE_CC_TLS */

/* Define to 1 if you have the `ceilf' function. */
#define _GLIBCXX_HAVE_CEILF 1

/* Define to 1 if you have the `ceill' function. */
#define _GLIBCXX_HAVE_CEILL 1

/* Define to 1 if you have the <complex.h> header file. */
#define _GLIBCXX_HAVE_COMPLEX_H 1

/* Define to 1 if you have the `cosf' function. */
#define _GLIBCXX_HAVE_COSF 1

/* Define to 1 if you have the `coshf' function. */
#define _GLIBCXX_HAVE_COSHF 1

/* Define to 1 if you have the `coshl' function. */
#define _GLIBCXX_HAVE_COSHL 1

/* Define to 1 if you have the `cosl' function. */
#define _GLIBCXX_HAVE_COSL 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define _GLIBCXX_HAVE_DLFCN_H 1

/* Define if EBADMSG exists. */
#define _GLIBCXX_HAVE_EBADMSG 1

/* Define if ECANCELED exists. */
#define _GLIBCXX_HAVE_ECANCELED 1

/* Define if ECHILD exists. */
#define _GLIBCXX_HAVE_ECHILD 1

/* Define if EIDRM exists. */
#define _GLIBCXX_HAVE_EIDRM 1

/* Define to 1 if you have the <endian.h> header file. */
#define _GLIBCXX_HAVE_ENDIAN_H 1

/* Define if ENODATA exists. */
#define _GLIBCXX_HAVE_ENODATA 1

/* Define if ENOLINK exists. */
#define _GLIBCXX_HAVE_ENOLINK 1

/* Define if ENOSPC exists. */
#define _GLIBCXX_HAVE_ENOSPC 1

/* Define if ENOSR exists. */
#define _GLIBCXX_HAVE_ENOSR 1

/* Define if ENOSTR exists. */
#define _GLIBCXX_HAVE_ENOSTR 1

/* Define if ENOTRECOVERABLE exists. */
#define _GLIBCXX_HAVE_ENOTRECOVERABLE 1

/* Define if ENOTSUP exists. */
#define _GLIBCXX_HAVE_ENOTSUP 1

/* Define if EOVERFLOW exists. */
#define _GLIBCXX_HAVE_EOVERFLOW 1

/* Define if EOWNERDEAD exists. */
#define _GLIBCXX_HAVE_EOWNERDEAD 1

/* Define if EPERM exists. */
#define _GLIBCXX_HAVE_EPERM 1

/* Define if EPROTO exists. */
#define _GLIBCXX_HAVE_EPROTO 1

/* Define if ETIME exists. */
#define _GLIBCXX_HAVE_ETIME 1

/* Define if ETIMEDOUT exists. */
#define _GLIBCXX_HAVE_ETIMEDOUT 1

/* Define if ETXTBSY exists. */
#define _GLIBCXX_HAVE_ETXTBSY 1

/* Define if EWOULDBLOCK exists. */
#define _GLIBCXX_HAVE_EWOULDBLOCK 1

/* Define to 1 if you have the <execinfo.h> header file. */
/* #undef _GLIBCXX_HAVE_EXECINFO_H */

/* Define to 1 if you have the `expf' function. */
#define _GLIBCXX_HAVE_EXPF 1

/* Define to 1 if you have the `expl' function. */
#define _GLIBCXX_HAVE_EXPL 1

/* Define to 1 if you have the `fabsf' function. */
#define _GLIBCXX_HAVE_FABSF 1

/* Define to 1 if you have the `fabsl' function. */
#define _GLIBCXX_HAVE_FABSL 1

/* Define to 1 if you have the <fenv.h> header file. */
#define _GLIBCXX_HAVE_FENV_H 1

/* Define to 1 if you have the `finite' function. */
#define _GLIBCXX_HAVE_FINITE 1

/* Define to 1 if you have the `finitef' function. */
#define _GLIBCXX_HAVE_FINITEF 1

/* Define to 1 if you have the `finitel' function. */
/* #undef _GLIBCXX_HAVE_FINITEL */

/* Define to 1 if you have the <float.h> header file. */
#define _GLIBCXX_HAVE_FLOAT_H 1

/* Define to 1 if you have the `floorf' function. */
#define _GLIBCXX_HAVE_FLOORF 1

/* Define to 1 if you have the `floorl' function. */
#define _GLIBCXX_HAVE_FLOORL 1

/* Define to 1 if you have the `fmodf' function. */
#define _GLIBCXX_HAVE_FMODF 1

/* Define to 1 if you have the `fmodl' function. */
#define _GLIBCXX_HAVE_FMODL 1

/* Define to 1 if you have the `fpclass' function. */
/* #undef _GLIBCXX_HAVE_FPCLASS */

/* Define to 1 if you have the <fp.h> header file. */
/* #undef _GLIBCXX_HAVE_FP_H */

/* Define to 1 if you have the `frexpf' function. */
#define _GLIBCXX_HAVE_FREXPF 1

/* Define to 1 if you have the `frexpl' function. */
#define _GLIBCXX_HAVE_FREXPL 1

/* Define if _Unwind_GetIPInfo is available. */
#define _GLIBCXX_HAVE_GETIPINFO 1

/* Define if gets is available in <stdio.h>. */
#define _GLIBCXX_HAVE_GETS 1

/* Define to 1 if you have the `hypot' function. */
#define _GLIBCXX_HAVE_HYPOT 1

/* Define to 1 if you have the `hypotf' function. */
#define _GLIBCXX_HAVE_HYPOTF 1

/* Define to 1 if you have the `hypotl' function. */
#define _GLIBCXX_HAVE_HYPOTL 1

/* Define if you have the iconv() function. */
/* #undef _GLIBCXX_HAVE_ICONV */

/* Define to 1 if you have the <ieeefp.h> header file. */
/* #undef _GLIBCXX_HAVE_IEEEFP_H */

/* Define if int64_t is available in <stdint.h>. */
#define _GLIBCXX_HAVE_INT64_T 1

/* Define if int64_t is a long. */
#define _GLIBCXX_HAVE_INT64_T_LONG 1

/* Define if int64_t is a long long. */
/* #undef _GLIBCXX_HAVE_INT64_T_LONG_LONG */

/* Define to 1 if you have the <inttypes.h> header file. */
#define _GLIBCXX_HAVE_INTTYPES_H 1

/* Define to 1 if you have the `isinf' function. */
#define _GLIBCXX_HAVE_ISINF 1

/* Define to 1 if you have the `isinff' function. */
/* #undef _GLIBCXX_HAVE_ISINFF */

/* Define to 1 if you have the `isinfl' function. */
/* #undef _GLIBCXX_HAVE_ISINFL */

/* Define to 1 if you have the `isnan' function. */
#define _GLIBCXX_HAVE_ISNAN 1

/* Define to 1 if you have the `isnanf' function. */
#define _GLIBCXX_HAVE_ISNANF 1

/* Define to 1 if you have the `isnanl' function. */
/* #undef _GLIBCXX_HAVE_ISNANL */

/* Defined if iswblank exists. */
#define _GLIBCXX_HAVE_ISWBLANK 1

/* Define if LC_MESSAGES is available in <locale.h>. */
#define _GLIBCXX_HAVE_LC_MESSAGES 1

/* Define to 1 if you have the `ldexpf' function. */
#define _GLIBCXX_HAVE_LDEXPF 1

/* Define to 1 if you have the `ldexpl' function. */
#define _GLIBCXX_HAVE_LDEXPL 1

/* Define to 1 if you have the <libintl.h> header file. */
/* #undef _GLIBCXX_HAVE_LIBINTL_H */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _GLIBCXX_HAVE_LIMIT_AS */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _GLIBCXX_HAVE_LIMIT_DATA */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _GLIBCXX_HAVE_LIMIT_FSIZE */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _GLIBCXX_HAVE_LIMIT_RSS */

/* Only used in build directory testsuite_hooks.h. */
/* #undef _GLIBCXX_HAVE_LIMIT_VMEM */

/* Define if futex syscall is available. */
#define _GLIBCXX_HAVE_LINUX_FUTEX 1

/* Define to 1 if you have the <locale.h> header file. */
#define _GLIBCXX_HAVE_LOCALE_H 1

/* Define to 1 if you have the `log10f' function. */
#define _GLIBCXX_HAVE_LOG10F 1

/* Define to 1 if you have the `log10l' function. */
#define _GLIBCXX_HAVE_LOG10L 1

/* Define to 1 if you have the `logf' function. */
#define _GLIBCXX_HAVE_LOGF 1

/* Define to 1 if you have the `logl' function. */
#define _GLIBCXX_HAVE_LOGL 1

/* Define to 1 if you have the <machine/endian.h> header file. */
#define _GLIBCXX_HAVE_MACHINE_ENDIAN_H 1

/* Define to 1 if you have the <machine/param.h> header file. */
/* #undef _GLIBCXX_HAVE_MACHINE_PARAM_H */

/* Define if mbstate_t exists in wchar.h. */
#define _GLIBCXX_HAVE_MBSTATE_T 1

/* Define to 1 if you have the <memory.h> header file. */
#define _GLIBCXX_HAVE_MEMORY_H 1

/* Define to 1 if you have the `modf' function. */
#define _GLIBCXX_HAVE_MODF 1

/* Define to 1 if you have the `modff' function. */
#define _GLIBCXX_HAVE_MODFF 1

/* Define to 1 if you have the `modfl' function. */
#define _GLIBCXX_HAVE_MODFL 1

/* Define to 1 if you have the <nan.h> header file. */
/* #undef _GLIBCXX_HAVE_NAN_H */

/* Define if poll is available in <poll.h>. */
#define _GLIBCXX_HAVE_POLL 1

/* Define to 1 if you have the `powf' function. */
#define _GLIBCXX_HAVE_POWF 1

/* Define to 1 if you have the `powl' function. */
#define _GLIBCXX_HAVE_POWL 1

/* Define to 1 if you have the `qfpclass' function. */
/* #undef _GLIBCXX_HAVE_QFPCLASS */

/* Define to 1 if you have the `quick_exit' function. */
/* #undef _GLIBCXX_HAVE_QUICK_EXIT */

/* Define to 1 if you have the `setenv' function. */
/* #undef _GLIBCXX_HAVE_SETENV */

/* Define to 1 if you have the `sincos' function. */
#define _GLIBCXX_HAVE_SINCOS 1

/* Define to 1 if you have the `sincosf' function. */
#define _GLIBCXX_HAVE_SINCOSF 1

/* Define to 1 if you have the `sincosl' function. */
#define _GLIBCXX_HAVE_SINCOSL 1

/* Define to 1 if you have the `sinf' function. */
#define _GLIBCXX_HAVE_SINF 1

/* Define to 1 if you have the `sinhf' function. */
#define _GLIBCXX_HAVE_SINHF 1

/* Define to 1 if you have the `sinhl' function. */
#define _GLIBCXX_HAVE_SINHL 1

/* Define to 1 if you have the `sinl' function. */
#define _GLIBCXX_HAVE_SINL 1

/* Defined if sleep exists. */
/* #undef _GLIBCXX_HAVE_SLEEP */

/* Define to 1 if you have the `sqrtf' function. */
#define _GLIBCXX_HAVE_SQRTF 1

/* Define to 1 if you have the `sqrtl' function. */
#define _GLIBCXX_HAVE_SQRTL 1

/* Define to 1 if you have the <stdalign.h> header file. */
#define _GLIBCXX_HAVE_STDALIGN_H 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define _GLIBCXX_HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define _GLIBCXX_HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define _GLIBCXX_HAVE_STDLIB_H 1

/* Define if strerror_l is available in <string.h>. */
/* #undef _GLIBCXX_HAVE_STRERROR_L */

/* Define if strerror_r is available in <string.h>. */
#define _GLIBCXX_HAVE_STRERROR_R 1

/* Define to 1 if you have the <strings.h> header file. */
#define _GLIBCXX_HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define _GLIBCXX_HAVE_STRING_H 1

/* Define to 1 if you have the `strtof' function. */
#define _GLIBCXX_HAVE_STRTOF 1

/* Define to 1 if you have the `strtold' function. */
#define _GLIBCXX_HAVE_STRTOLD 1

/* Define if strxfrm_l is available in <string.h>. */
/* #undef _GLIBCXX_HAVE_STRXFRM_L */

/* Define to 1 if the target runtime linker supports binding the same symbol
   to different versions. */ // :3
/* #undef _GLIBCXX_HAVE_SYMVER_SYMBOL_RENAMING_RUNTIME_SUPPORT */

/* Define to 1 if you have the <sys/filio.h> header file. */
/* #undef _GLIBCXX_HAVE_SYS_FILIO_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define _GLIBCXX_HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/ipc.h> header file. */
#define _GLIBCXX_HAVE_SYS_IPC_H 1

/* Define to 1 if you have the <sys/isa_defs.h> header file. */
/* #undef _GLIBCXX_HAVE_SYS_ISA_DEFS_H */

/* Define to 1 if you have the <sys/machine.h> header file. */
/* #undef _GLIBCXX_HAVE_SYS_MACHINE_H */

/* Define to 1 if you have the <sys/param.h> header file. */
#define _GLIBCXX_HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define _GLIBCXX_HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have a suitable <sys/sdt.h> header file */
/* #undef _GLIBCXX_HAVE_SYS_SDT_H */

/* Define to 1 if you have the <sys/sem.h> header file. */
#define _GLIBCXX_HAVE_SYS_SEM_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define _GLIBCXX_HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sysinfo.h> header file. */
#define _GLIBCXX_HAVE_SYS_SYSINFO_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define _GLIBCXX_HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define _GLIBCXX_HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define _GLIBCXX_HAVE_SYS_UIO_H 1

/* Define if S_IFREG is available in <sys/stat.h>. */
/* #undef _GLIBCXX_HAVE_S_IFREG */

/* Define if S_IFREG is available in <sys/stat.h>. */
#define _GLIBCXX_HAVE_S_ISREG 1

/* Define to 1 if you have the `tanf' function. */
#define _GLIBCXX_HAVE_TANF 1

/* Define to 1 if you have the `tanhf' function. */
#define _GLIBCXX_HAVE_TANHF 1

/* Define to 1 if you have the `tanhl' function. */
#define _GLIBCXX_HAVE_TANHL 1

/* Define to 1 if you have the `tanl' function. */
#define _GLIBCXX_HAVE_TANL 1

/* Define to 1 if you have the <tgmath.h> header file. */
/* #undef _GLIBCXX_HAVE_TGMATH_H */

/* Define to 1 if the target supports thread-local storage. */
/* #undef _GLIBCXX_HAVE_TLS */

/* Define to 1 if you have the <unistd.h> header file. */
#define _GLIBCXX_HAVE_UNISTD_H 1

/* Defined if usleep exists. */
/* #undef _GLIBCXX_HAVE_USLEEP */

/* Defined if vfwscanf exists. */
#define _GLIBCXX_HAVE_VFWSCANF 1

/* Defined if vswscanf exists. */
#define _GLIBCXX_HAVE_VSWSCANF 1

/* Defined if vwscanf exists. */
#define _GLIBCXX_HAVE_VWSCANF 1

/* Define to 1 if you have the <wchar.h> header file. */
#define _GLIBCXX_HAVE_WCHAR_H 1

/* Defined if wcstof exists. */
#define _GLIBCXX_HAVE_WCSTOF 1

/* Define to 1 if you have the <wctype.h> header file. */
#define _GLIBCXX_HAVE_WCTYPE_H 1

/* Defined if Sleep exists. */
/* #undef _GLIBCXX_HAVE_WIN32_SLEEP */

/* Define if writev is available in <sys/uio.h>. */
#define _GLIBCXX_HAVE_WRITEV 1

/* Define to 1 if you have the `_acosf' function. */
/* #undef _GLIBCXX_HAVE__ACOSF */

/* Define to 1 if you have the `_acosl' function. */
/* #undef _GLIBCXX_HAVE__ACOSL */

/* Define to 1 if you have the `_asinf' function. */
/* #undef _GLIBCXX_HAVE__ASINF */

/* Define to 1 if you have the `_asinl' function. */
/* #undef _GLIBCXX_HAVE__ASINL */

/* Define to 1 if you have the `_atan2f' function. */
/* #undef _GLIBCXX_HAVE__ATAN2F */

/* Define to 1 if you have the `_atan2l' function. */
/* #undef _GLIBCXX_HAVE__ATAN2L */

/* Define to 1 if you have the `_atanf' function. */
/* #undef _GLIBCXX_HAVE__ATANF */

/* Define to 1 if you have the `_atanl' function. */
/* #undef _GLIBCXX_HAVE__ATANL */

/* Define to 1 if you have the `_ceilf' function. */
/* #undef _GLIBCXX_HAVE__CEILF */

/* Define to 1 if you have the `_ceill' function. */
/* #undef _GLIBCXX_HAVE__CEILL */

/* Define to 1 if you have the `_cosf' function. */
/* #undef _GLIBCXX_HAVE__COSF */

/* Define to 1 if you have the `_coshf' function. */
/* #undef _GLIBCXX_HAVE__COSHF */

/* Define to 1 if you have the `_coshl' function. */
/* #undef _GLIBCXX_HAVE__COSHL */

/* Define to 1 if you have the `_cosl' function. */
/* #undef _GLIBCXX_HAVE__COSL */

/* Define to 1 if you have the `_expf' function. */
/* #undef _GLIBCXX_HAVE__EXPF */

/* Define to 1 if you have the `_expl' function. */
/* #undef _GLIBCXX_HAVE__EXPL */

/* Define to 1 if you have the `_fabsf' function. */
/* #undef _GLIBCXX_HAVE__FABSF */

/* Define to 1 if you have the `_fabsl' function. */
/* #undef _GLIBCXX_HAVE__FABSL */

/* Define to 1 if you have the `_finite' function. */
/* #undef _GLIBCXX_HAVE__FINITE */

/* Define to 1 if you have the `_finitef' function. */
/* #undef _GLIBCXX_HAVE__FINITEF */

/* Define to 1 if you have the `_finitel' function. */
/* #undef _GLIBCXX_HAVE__FINITEL */

/* Define to 1 if you have the `_floorf' function. */
/* #undef _GLIBCXX_HAVE__FLOORF */

/* Define to 1 if you have the `_floorl' function. */
/* #undef _GLIBCXX_HAVE__FLOORL */

/* Define to 1 if you have the `_fmodf' function. */
/* #undef _GLIBCXX_HAVE__FMODF */

/* Define to 1 if you have the `_fmodl' function. */
/* #undef _GLIBCXX_HAVE__FMODL */

/* Define to 1 if you have the `_fpclass' function. */
/* #undef _GLIBCXX_HAVE__FPCLASS */

/* Define to 1 if you have the `_frexpf' function. */
/* #undef _GLIBCXX_HAVE__FREXPF */

/* Define to 1 if you have the `_frexpl' function. */
/* #undef _GLIBCXX_HAVE__FREXPL */

/* Define to 1 if you have the `_hypot' function. */
/* #undef _GLIBCXX_HAVE__HYPOT */

/* Define to 1 if you have the `_hypotf' function. */
/* #undef _GLIBCXX_HAVE__HYPOTF */

/* Define to 1 if you have the `_hypotl' function. */
/* #undef _GLIBCXX_HAVE__HYPOTL */

/* Define to 1 if you have the `_isinf' function. */
/* #undef _GLIBCXX_HAVE__ISINF */

/* Define to 1 if you have the `_isinff' function. */
/* #undef _GLIBCXX_HAVE__ISINFF */

/* Define to 1 if you have the `_isinfl' function. */
/* #undef _GLIBCXX_HAVE__ISINFL */

/* Define to 1 if you have the `_isnan' function. */
/* #undef _GLIBCXX_HAVE__ISNAN */

/* Define to 1 if you have the `_isnanf' function. */
/* #undef _GLIBCXX_HAVE__ISNANF */

/* Define to 1 if you have the `_isnanl' function. */
/* #undef _GLIBCXX_HAVE__ISNANL */

/* Define to 1 if you have the `_ldexpf' function. */
/* #undef _GLIBCXX_HAVE__LDEXPF */

/* Define to 1 if you have the `_ldexpl' function. */
/* #undef _GLIBCXX_HAVE__LDEXPL */

/* Define to 1 if you have the `_log10f' function. */
/* #undef _GLIBCXX_HAVE__LOG10F */

/* Define to 1 if you have the `_log10l' function. */
/* #undef _GLIBCXX_HAVE__LOG10L */

/* Define to 1 if you have the `_logf' function. */
/* #undef _GLIBCXX_HAVE__LOGF */

/* Define to 1 if you have the `_logl' function. */
/* #undef _GLIBCXX_HAVE__LOGL */

/* Define to 1 if you have the `_modf' function. */
/* #undef _GLIBCXX_HAVE__MODF */

/* Define to 1 if you have the `_modff' function. */
/* #undef _GLIBCXX_HAVE__MODFF */

/* Define to 1 if you have the `_modfl' function. */
/* #undef _GLIBCXX_HAVE__MODFL */

/* Define to 1 if you have the `_powf' function. */
/* #undef _GLIBCXX_HAVE__POWF */

/* Define to 1 if you have the `_powl' function. */
/* #undef _GLIBCXX_HAVE__POWL */

/* Define to 1 if you have the `_qfpclass' function. */
/* #undef _GLIBCXX_HAVE__QFPCLASS */

/* Define to 1 if you have the `_sincos' function. */
/* #undef _GLIBCXX_HAVE__SINCOS */

/* Define to 1 if you have the `_sincosf' function. */
/* #undef _GLIBCXX_HAVE__SINCOSF */

/* Define to 1 if you have the `_sincosl' function. */
/* #undef _GLIBCXX_HAVE__SINCOSL */

/* Define to 1 if you have the `_sinf' function. */
/* #undef _GLIBCXX_HAVE__SINF */

/* Define to 1 if you have the `_sinhf' function. */
/* #undef _GLIBCXX_HAVE__SINHF */

/* Define to 1 if you have the `_sinhl' function. */
/* #undef _GLIBCXX_HAVE__SINHL */

/* Define to 1 if you have the `_sinl' function. */
/* #undef _GLIBCXX_HAVE__SINL */

/* Define to 1 if you have the `_sqrtf' function. */
/* #undef _GLIBCXX_HAVE__SQRTF */

/* Define to 1 if you have the `_sqrtl' function. */
/* #undef _GLIBCXX_HAVE__SQRTL */

/* Define to 1 if you have the `_tanf' function. */
/* #undef _GLIBCXX_HAVE__TANF */

/* Define to 1 if you have the `_tanhf' function. */
/* #undef _GLIBCXX_HAVE__TANHF */

/* Define to 1 if you have the `_tanhl' function. */
/* #undef _GLIBCXX_HAVE__TANHL */

/* Define to 1 if you have the `_tanl' function. */
/* #undef _GLIBCXX_HAVE__TANL */

/* Define to 1 if you have the `__cxa_thread_atexit_impl' function. */
/* #undef _GLIBCXX_HAVE___CXA_THREAD_ATEXIT_IMPL */

/* Define as const if the declaration of iconv() needs const. */
/* #undef _GLIBCXX_ICONV_CONST */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */ // :3
#define LT_OBJDIR ".libs/" // :3

/* Name of package */
/* #undef _GLIBCXX_PACKAGE */

/* Define to the address where bug reports for this package should be sent. */
#define _GLIBCXX_PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define _GLIBCXX_PACKAGE_NAME "package-unused"

/* Define to the full name and version of this package. */
#define _GLIBCXX_PACKAGE_STRING "package-unused version-unused"

/* Define to the one symbol short name of this package. */
#define _GLIBCXX_PACKAGE_TARNAME "libstdc++"

/* Define to the home page for this package. */
#define _GLIBCXX_PACKAGE_URL ""

/* Define to the version of this package. */
#define _GLIBCXX_PACKAGE__GLIBCXX_VERSION "version-unused"

/* The size of `char', as computed by sizeof. */
/* #undef SIZEOF_CHAR */

/* The size of `int', as computed by sizeof. */
/* #undef SIZEOF_INT */

/* The size of `long', as computed by sizeof. */
/* #undef SIZEOF_LONG */

/* The size of `short', as computed by sizeof. */
/* #undef SIZEOF_SHORT */

/* The size of `void *', as computed by sizeof. */
/* #undef SIZEOF_VOID_P */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
/* #undef _GLIBCXX_VERSION */

/* Define if the compiler supports C++11 atomics. */
#define _GLIBCXX_ATOMIC_BUILTINS 1

/* Define to use concept checking code from the boost libraries. */
/* #undef _GLIBCXX_CONCEPT_CHECKS */

/* Define to 1 if a fully dynamic basic_string is wanted, 0 to disable,
   undefined for platform defaults */ // :3
#define _GLIBCXX_FULLY_DYNAMIC_STRING 0 // :3

/* Define if gthreads library is available. */
#define _GLIBCXX_HAS_GTHREADS 1

/* Define to 1 if a full hosted library is built, or 0 if freestanding. */
#define _GLIBCXX_HOSTED 1

/* Define if compatibility should be provided for -mlong-double-64. */

/* Define if ptrdiff_t is int. */
/* #undef _GLIBCXX_PTRDIFF_T_IS_INT */

/* Define if using setrlimit to set resource limits during "make check" */
/* #undef _GLIBCXX_RES_LIMITS */

/* Define if size_t is unsigned int. */
/* #undef _GLIBCXX_SIZE_T_IS_UINT */

/* Define if the compiler is configured for setjmp/longjmp exceptions. */
/* #undef _GLIBCXX_SJLJ_EXCEPTIONS */

/* Define to the value of the EOF integer constant. */
#define _GLIBCXX_STDIO_EOF -1

/* Define to the value of the SEEK_CUR integer constant. */
#define _GLIBCXX_STDIO_SEEK_CUR 1

/* Define to the value of the SEEK_END integer constant. */
#define _GLIBCXX_STDIO_SEEK_END 2

/* Define to use symbol versioning in the shared library. */
/* #undef _GLIBCXX_SYMVER */

/* Define to use darwin versioning in the shared library. */
/* #undef _GLIBCXX_SYMVER_DARWIN */

/* Define to use GNU versioning in the shared library. */
/* #undef _GLIBCXX_SYMVER_GNU */

/* Define to use GNU namespace versioning in the shared library. */
/* #undef _GLIBCXX_SYMVER_GNU_NAMESPACE */

/* Define to use Sun versioning in the shared library. */
/* #undef _GLIBCXX_SYMVER_SUN */

/* Define if C99 functions or macros from <wchar.h>, <math.h>, <complex.h>,
   <stdio.h>, and <stdlib.h> can be used or exposed. */ // :3
/* #undef _GLIBCXX_USE_C99 */

/* Define if C99 functions in <complex.h> should be used in <complex>. Using
   compiler builtins for these functions requires corresponding C99 library
   functions to be present. */ // :3
/* #undef _GLIBCXX_USE_C99_COMPLEX */

/* Define if C99 functions in <complex.h> should be used in <tr1/complex>.
   Using compiler builtins for these functions requires corresponding C99
   library functions to be present. */ // :3
/* #undef _GLIBCXX_USE_C99_COMPLEX_TR1 */

/* Define if C99 functions in <ctype.h> should be imported in <tr1/cctype> in
   namespace std::tr1. */ // :3
#define _GLIBCXX_USE_C99_CTYPE_TR1 1 // :3

/* Define if C99 functions in <fenv.h> should be imported in <tr1/cfenv> in
   namespace std::tr1. */ // :3
#define _GLIBCXX_USE_C99_FENV_TR1 1 // :3

/* Define if C99 functions in <inttypes.h> should be imported in
   <tr1/cinttypes> in namespace std::tr1. */ // :3
#define _GLIBCXX_USE_C99_INTTYPES_TR1 1 // :3

/* Define if wchar_t C99 functions in <inttypes.h> should be imported in
   <tr1/cinttypes> in namespace std::tr1. */ // :3
#define _GLIBCXX_USE_C99_INTTYPES_WCHAR_T_TR1 1 // :3

/* Define if C99 functions or macros in <math.h> should be imported in <cmath>
   in namespace std. */ // :3
#define _GLIBCXX_USE_C99_MATH 1 // :3

/* Define if C99 functions or macros in <math.h> should be imported in
   <tr1/cmath> in namespace std::tr1. */ // :3
#define _GLIBCXX_USE_C99_MATH_TR1 1 // :3

/* Define if C99 types in <stdint.h> should be imported in <tr1/cstdint> in
   namespace std::tr1. */ // :3
#define _GLIBCXX_USE_C99_STDINT_TR1 1 // :3

/* Defined if clock_gettime syscall has monotonic and realtime clock support.
   */ // :3
/* #undef _GLIBCXX_USE_CLOCK_GETTIME_SYSCALL */

/* Defined if clock_gettime has monotonic clock support. */
#define _GLIBCXX_USE_CLOCK_MONOTONIC 1

/* Defined if clock_gettime has realtime clock support. */
#define _GLIBCXX_USE_CLOCK_REALTIME 1

/* Define if ISO/IEC TR 24733 decimal floating point types are supported on
   this host. */ // :3
/* #undef _GLIBCXX_USE_DECIMAL_FLOAT */

/* Define if __float128 is supported on this host.
   Hide all uses of __float128 from Clang.  Google ref b/6422845  */ // :3
#ifndef __clang__ // :3
/* #undef _GLIBCXX_USE_FLOAT128 */
#endif

/* Defined if gettimeofday is available. */
#define _GLIBCXX_USE_GETTIMEOFDAY 1

/* Define if get_nprocs is available in <sys/sysinfo.h>. */
/* #undef _GLIBCXX_USE_GET_NPROCS */

/* Define if __int128 is supported on this host. */
#define _GLIBCXX_USE_INT128 1

/* Define if LFS support is available. */
/* #undef _GLIBCXX_USE_LFS */

/* Define if code specialized for long long should be used. */
#define _GLIBCXX_USE_LONG_LONG 1

/* Defined if nanosleep is available. */
#define _GLIBCXX_USE_NANOSLEEP 1

/* Define if NLS translations are to be used. */
/* #undef _GLIBCXX_USE_NLS */

/* Define if pthreads_num_processors_np is available in <pthread.h>. */
/* #undef _GLIBCXX_USE_PTHREADS_NUM_PROCESSORS_NP */

/* Define if /dev/random and /dev/urandom are available for the random_device
   of TR1 (Chapter 5.1). */ // :3
#define _GLIBCXX_USE_RANDOM_TR1 1 // :3

/* Defined if sched_yield is available. */
#define _GLIBCXX_USE_SCHED_YIELD 1

/* Define if _SC_NPROCESSORS_ONLN is available in <unistd.h>. */
#define _GLIBCXX_USE_SC_NPROCESSORS_ONLN 1

/* Define if _SC_NPROC_ONLN is available in <unistd.h>. */
/* #undef _GLIBCXX_USE_SC_NPROC_ONLN */

/* Define if sysctl(), CTL_HW and HW_NCPU are available in <sys/sysctl.h>. */
/* #undef _GLIBCXX_USE_SYSCTL_HW_NCPU */

/* Define if obsolescent tmpnam is available in <stdio.h>. */
#define _GLIBCXX_USE_TMPNAM 1

/* Define if code specialized for wchar_t should be used. */
#define _GLIBCXX_USE_WCHAR_T 1

/* Define to 1 if a verbose library is built, or 0 otherwise. */
#define _GLIBCXX_VERBOSE 1

/* Defined if as can handle rdrand.
   Disable when building with Clang.  Google ref b/8680429 */ // :3
#ifndef __clang__ // :3
/* #undef _GLIBCXX_X86_RDRAND */
#endif

/* Define to 1 if mutex_timedlock is available. */
#define _GTHREAD_USE_MUTEX_TIMEDLOCK 0

#if defined (_GLIBCXX_HAVE__ACOSF) && ! defined (_GLIBCXX_HAVE_ACOSF)
# define _GLIBCXX_HAVE_ACOSF 1
# define acosf _acosf
#endif

#if defined (_GLIBCXX_HAVE__ACOSL) && ! defined (_GLIBCXX_HAVE_ACOSL)
# define _GLIBCXX_HAVE_ACOSL 1
# define acosl _acosl
#endif

#if defined (_GLIBCXX_HAVE__ASINF) && ! defined (_GLIBCXX_HAVE_ASINF)
# define _GLIBCXX_HAVE_ASINF 1
# define asinf _asinf
#endif

#if defined (_GLIBCXX_HAVE__ASINL) && ! defined (_GLIBCXX_HAVE_ASINL)
# define _GLIBCXX_HAVE_ASINL 1
# define asinl _asinl
#endif

#if defined (_GLIBCXX_HAVE__ATAN2F) && ! defined (_GLIBCXX_HAVE_ATAN2F)
# define _GLIBCXX_HAVE_ATAN2F 1
# define atan2f _atan2f
#endif

#if defined (_GLIBCXX_HAVE__ATAN2L) && ! defined (_GLIBCXX_HAVE_ATAN2L)
# define _GLIBCXX_HAVE_ATAN2L 1
# define atan2l _atan2l
#endif

#if defined (_GLIBCXX_HAVE__ATANF) && ! defined (_GLIBCXX_HAVE_ATANF)
# define _GLIBCXX_HAVE_ATANF 1
# define atanf _atanf
#endif

#if defined (_GLIBCXX_HAVE__ATANL) && ! defined (_GLIBCXX_HAVE_ATANL)
# define _GLIBCXX_HAVE_ATANL 1
# define atanl _atanl
#endif

#if defined (_GLIBCXX_HAVE__CEILF) && ! defined (_GLIBCXX_HAVE_CEILF)
# define _GLIBCXX_HAVE_CEILF 1
# define ceilf _ceilf
#endif

#if defined (_GLIBCXX_HAVE__CEILL) && ! defined (_GLIBCXX_HAVE_CEILL)
# define _GLIBCXX_HAVE_CEILL 1
# define ceill _ceill
#endif

#if defined (_GLIBCXX_HAVE__COSF) && ! defined (_GLIBCXX_HAVE_COSF)
# define _GLIBCXX_HAVE_COSF 1
# define cosf _cosf
#endif

#if defined (_GLIBCXX_HAVE__COSHF) && ! defined (_GLIBCXX_HAVE_COSHF)
# define _GLIBCXX_HAVE_COSHF 1
# define coshf _coshf
#endif

#if defined (_GLIBCXX_HAVE__COSHL) && ! defined (_GLIBCXX_HAVE_COSHL)
# define _GLIBCXX_HAVE_COSHL 1
# define coshl _coshl
#endif

#if defined (_GLIBCXX_HAVE__COSL) && ! defined (_GLIBCXX_HAVE_COSL)
# define _GLIBCXX_HAVE_COSL 1
# define cosl _cosl
#endif

#if defined (_GLIBCXX_HAVE__EXPF) && ! defined (_GLIBCXX_HAVE_EXPF)
# define _GLIBCXX_HAVE_EXPF 1
# define expf _expf
#endif

#if defined (_GLIBCXX_HAVE__EXPL) && ! defined (_GLIBCXX_HAVE_EXPL)
# define _GLIBCXX_HAVE_EXPL 1
# define expl _expl
#endif

#if defined (_GLIBCXX_HAVE__FABSF) && ! defined (_GLIBCXX_HAVE_FABSF)
# define _GLIBCXX_HAVE_FABSF 1
# define fabsf _fabsf
#endif

#if defined (_GLIBCXX_HAVE__FABSL) && ! defined (_GLIBCXX_HAVE_FABSL)
# define _GLIBCXX_HAVE_FABSL 1
# define fabsl _fabsl
#endif

#if defined (_GLIBCXX_HAVE__FINITE) && ! defined (_GLIBCXX_HAVE_FINITE)
# define _GLIBCXX_HAVE_FINITE 1
# define finite _finite
#endif

#if defined (_GLIBCXX_HAVE__FINITEF) && ! defined (_GLIBCXX_HAVE_FINITEF)
# define _GLIBCXX_HAVE_FINITEF 1
# define finitef _finitef
#endif

#if defined (_GLIBCXX_HAVE__FINITEL) && ! defined (_GLIBCXX_HAVE_FINITEL)
# define _GLIBCXX_HAVE_FINITEL 1
# define finitel _finitel
#endif

#if defined (_GLIBCXX_HAVE__FLOORF) && ! defined (_GLIBCXX_HAVE_FLOORF)
# define _GLIBCXX_HAVE_FLOORF 1
# define floorf _floorf
#endif

#if defined (_GLIBCXX_HAVE__FLOORL) && ! defined (_GLIBCXX_HAVE_FLOORL)
# define _GLIBCXX_HAVE_FLOORL 1
# define floorl _floorl
#endif

#if defined (_GLIBCXX_HAVE__FMODF) && ! defined (_GLIBCXX_HAVE_FMODF)
# define _GLIBCXX_HAVE_FMODF 1
# define fmodf _fmodf
#endif

#if defined (_GLIBCXX_HAVE__FMODL) && ! defined (_GLIBCXX_HAVE_FMODL)
# define _GLIBCXX_HAVE_FMODL 1
# define fmodl _fmodl
#endif

#if defined (_GLIBCXX_HAVE__FPCLASS) && ! defined (_GLIBCXX_HAVE_FPCLASS)
# define _GLIBCXX_HAVE_FPCLASS 1
# define fpclass _fpclass
#endif

#if defined (_GLIBCXX_HAVE__FREXPF) && ! defined (_GLIBCXX_HAVE_FREXPF)
# define _GLIBCXX_HAVE_FREXPF 1
# define frexpf _frexpf
#endif

#if defined (_GLIBCXX_HAVE__FREXPL) && ! defined (_GLIBCXX_HAVE_FREXPL)
# define _GLIBCXX_HAVE_FREXPL 1
# define frexpl _frexpl
#endif

#if defined (_GLIBCXX_HAVE__HYPOT) && ! defined (_GLIBCXX_HAVE_HYPOT)
# define _GLIBCXX_HAVE_HYPOT 1
# define hypot _hypot
#endif

#if defined (_GLIBCXX_HAVE__HYPOTF) && ! defined (_GLIBCXX_HAVE_HYPOTF)
# define _GLIBCXX_HAVE_HYPOTF 1
# define hypotf _hypotf
#endif

#if defined (_GLIBCXX_HAVE__HYPOTL) && ! defined (_GLIBCXX_HAVE_HYPOTL)
# define _GLIBCXX_HAVE_HYPOTL 1
# define hypotl _hypotl
#endif

#if defined (_GLIBCXX_HAVE__ISINF) && ! defined (_GLIBCXX_HAVE_ISINF)
# define _GLIBCXX_HAVE_ISINF 1
# define isinf _isinf
#endif

#if defined (_GLIBCXX_HAVE__ISINFF) && ! defined (_GLIBCXX_HAVE_ISINFF)
# define _GLIBCXX_HAVE_ISINFF 1
# define isinff _isinff
#endif

#if defined (_GLIBCXX_HAVE__ISINFL) && ! defined (_GLIBCXX_HAVE_ISINFL)
# define _GLIBCXX_HAVE_ISINFL 1
# define isinfl _isinfl
#endif

#if defined (_GLIBCXX_HAVE__ISNAN) && ! defined (_GLIBCXX_HAVE_ISNAN)
# define _GLIBCXX_HAVE_ISNAN 1
# define isnan _isnan
#endif

#if defined (_GLIBCXX_HAVE__ISNANF) && ! defined (_GLIBCXX_HAVE_ISNANF)
# define _GLIBCXX_HAVE_ISNANF 1
# define isnanf _isnanf
#endif

#if defined (_GLIBCXX_HAVE__ISNANL) && ! defined (_GLIBCXX_HAVE_ISNANL)
# define _GLIBCXX_HAVE_ISNANL 1
# define isnanl _isnanl
#endif

#if defined (_GLIBCXX_HAVE__LDEXPF) && ! defined (_GLIBCXX_HAVE_LDEXPF)
# define _GLIBCXX_HAVE_LDEXPF 1
# define ldexpf _ldexpf
#endif

#if defined (_GLIBCXX_HAVE__LDEXPL) && ! defined (_GLIBCXX_HAVE_LDEXPL)
# define _GLIBCXX_HAVE_LDEXPL 1
# define ldexpl _ldexpl
#endif

#if defined (_GLIBCXX_HAVE__LOG10F) && ! defined (_GLIBCXX_HAVE_LOG10F)
# define _GLIBCXX_HAVE_LOG10F 1
# define log10f _log10f
#endif

#if defined (_GLIBCXX_HAVE__LOG10L) && ! defined (_GLIBCXX_HAVE_LOG10L)
# define _GLIBCXX_HAVE_LOG10L 1
# define log10l _log10l
#endif

#if defined (_GLIBCXX_HAVE__LOGF) && ! defined (_GLIBCXX_HAVE_LOGF)
# define _GLIBCXX_HAVE_LOGF 1
# define logf _logf
#endif

#if defined (_GLIBCXX_HAVE__LOGL) && ! defined (_GLIBCXX_HAVE_LOGL)
# define _GLIBCXX_HAVE_LOGL 1
# define logl _logl
#endif

#if defined (_GLIBCXX_HAVE__MODF) && ! defined (_GLIBCXX_HAVE_MODF)
# define _GLIBCXX_HAVE_MODF 1
# define modf _modf
#endif

#if defined (_GLIBCXX_HAVE__MODFF) && ! defined (_GLIBCXX_HAVE_MODFF)
# define _GLIBCXX_HAVE_MODFF 1
# define modff _modff
#endif

#if defined (_GLIBCXX_HAVE__MODFL) && ! defined (_GLIBCXX_HAVE_MODFL)
# define _GLIBCXX_HAVE_MODFL 1
# define modfl _modfl
#endif

#if defined (_GLIBCXX_HAVE__POWF) && ! defined (_GLIBCXX_HAVE_POWF)
# define _GLIBCXX_HAVE_POWF 1
# define powf _powf
#endif

#if defined (_GLIBCXX_HAVE__POWL) && ! defined (_GLIBCXX_HAVE_POWL)
# define _GLIBCXX_HAVE_POWL 1
# define powl _powl
#endif

#if defined (_GLIBCXX_HAVE__QFPCLASS) && ! defined (_GLIBCXX_HAVE_QFPCLASS)
# define _GLIBCXX_HAVE_QFPCLASS 1
# define qfpclass _qfpclass
#endif

#if defined (_GLIBCXX_HAVE__SINCOS) && ! defined (_GLIBCXX_HAVE_SINCOS)
# define _GLIBCXX_HAVE_SINCOS 1
# define sincos _sincos
#endif

#if defined (_GLIBCXX_HAVE__SINCOSF) && ! defined (_GLIBCXX_HAVE_SINCOSF)
# define _GLIBCXX_HAVE_SINCOSF 1
# define sincosf _sincosf
#endif

#if defined (_GLIBCXX_HAVE__SINCOSL) && ! defined (_GLIBCXX_HAVE_SINCOSL)
# define _GLIBCXX_HAVE_SINCOSL 1
# define sincosl _sincosl
#endif

#if defined (_GLIBCXX_HAVE__SINF) && ! defined (_GLIBCXX_HAVE_SINF)
# define _GLIBCXX_HAVE_SINF 1
# define sinf _sinf
#endif

#if defined (_GLIBCXX_HAVE__SINHF) && ! defined (_GLIBCXX_HAVE_SINHF)
# define _GLIBCXX_HAVE_SINHF 1
# define sinhf _sinhf
#endif

#if defined (_GLIBCXX_HAVE__SINHL) && ! defined (_GLIBCXX_HAVE_SINHL)
# define _GLIBCXX_HAVE_SINHL 1
# define sinhl _sinhl
#endif

#if defined (_GLIBCXX_HAVE__SINL) && ! defined (_GLIBCXX_HAVE_SINL)
# define _GLIBCXX_HAVE_SINL 1
# define sinl _sinl
#endif

#if defined (_GLIBCXX_HAVE__SQRTF) && ! defined (_GLIBCXX_HAVE_SQRTF)
# define _GLIBCXX_HAVE_SQRTF 1
# define sqrtf _sqrtf
#endif

#if defined (_GLIBCXX_HAVE__SQRTL) && ! defined (_GLIBCXX_HAVE_SQRTL)
# define _GLIBCXX_HAVE_SQRTL 1
# define sqrtl _sqrtl
#endif

#if defined (_GLIBCXX_HAVE__STRTOF) && ! defined (_GLIBCXX_HAVE_STRTOF)
# define _GLIBCXX_HAVE_STRTOF 1
# define strtof _strtof
#endif

#if defined (_GLIBCXX_HAVE__STRTOLD) && ! defined (_GLIBCXX_HAVE_STRTOLD)
# define _GLIBCXX_HAVE_STRTOLD 1
# define strtold _strtold
#endif

#if defined (_GLIBCXX_HAVE__TANF) && ! defined (_GLIBCXX_HAVE_TANF)
# define _GLIBCXX_HAVE_TANF 1
# define tanf _tanf
#endif

#if defined (_GLIBCXX_HAVE__TANHF) && ! defined (_GLIBCXX_HAVE_TANHF)
# define _GLIBCXX_HAVE_TANHF 1
# define tanhf _tanhf
#endif

#if defined (_GLIBCXX_HAVE__TANHL) && ! defined (_GLIBCXX_HAVE_TANHL)
# define _GLIBCXX_HAVE_TANHL 1
# define tanhl _tanhl
#endif

#if defined (_GLIBCXX_HAVE__TANL) && ! defined (_GLIBCXX_HAVE_TANL)
# define _GLIBCXX_HAVE_TANL 1
# define tanl _tanl
#endif

#endif // _GLIBCXX_CXX_CONFIG_H
