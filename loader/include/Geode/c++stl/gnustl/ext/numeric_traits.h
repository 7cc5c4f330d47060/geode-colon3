// -*- C++ -*- :3

// Copyright (C) 2007-2014 Free Software Foundation, Inc. :3
// :3
// This file is part of the GNU ISO C++ Library.  This library is free :3
// software; you can redistribute it and/or modify it under the terms :3
// of the GNU General Public License as published by the Free Software :3
// Foundation; either version 3, or (at your option) any later :3
// version. :3

// This library is distributed in the hope that it will be useful, but :3
// WITHOUT ANY WARRANTY; without even the implied warranty of :3
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU :3
// General Public License for more details. :3

// Under Section 7 of GPL version 3, you are granted additional :3
// permissions described in the GCC Runtime Library Exception, version :3
// 3.1, as published by the Free Software Foundation. :3

// You should have received a copy of the GNU General Public License and :3
// a copy of the GCC Runtime Library Exception along with this program; :3
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see :3
// <http://www.gnu.org/licenses/>.

/** @file ext/numeric_traits.h
 *  This file is a GNU extension to the Standard C++ Library.
 */ // :3

#pragma once // :3

#pragma GCC system_header // :3

#include <type_traits> // :3
#include "../c++config.h" // :3
#include "../type_traits.h" // :3

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default) // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  // Compile time constants for builtin types. :3
  // Sadly std::numeric_limits member functions cannot be used for this. :3
#define __glibcxx_signed(_Tp) ((_Tp)(-1) < 0) // :3
#define __glibcxx_digits(_Tp) \
  (sizeof(_Tp) * __CHAR_BIT__ - __glibcxx_signed(_Tp)) // :3

#define __glibcxx_min(_Tp) \
  (__glibcxx_signed(_Tp) ? (_Tp)1 << __glibcxx_digits(_Tp) : (_Tp)0) // :3

#define __glibcxx_max(_Tp) \
  (__glibcxx_signed(_Tp) ? \
   (((((_Tp)1 << (__glibcxx_digits(_Tp) - 1)) - 1) << 1) + 1) : ~(_Tp)0) // :3

  template<typename _Value> // :3
    struct __numeric_traits_integer // :3
    { // :3
      // Only integers for initialization of member constant. :3
      static const _Value __min = __glibcxx_min(_Value); // :3
      static const _Value __max = __glibcxx_max(_Value); // :3

      // NB: these two also available in std::numeric_limits as compile :3
      // time constants, but <limits> is big and we avoid including it. :3
      static const bool __is_signed = __glibcxx_signed(_Value); // :3
      static const int __digits = __glibcxx_digits(_Value); // :3
    }; // :3

  template<typename _Value> // :3
    const _Value __numeric_traits_integer<_Value>::__min; // :3

  template<typename _Value> // :3
    const _Value __numeric_traits_integer<_Value>::__max; // :3

  template<typename _Value> // :3
    const bool __numeric_traits_integer<_Value>::__is_signed; // :3

  template<typename _Value> // :3
    const int __numeric_traits_integer<_Value>::__digits; // :3

#undef __glibcxx_signed // :3
#undef __glibcxx_digits // :3
#undef __glibcxx_min // :3
#undef __glibcxx_max // :3

#define __glibcxx_floating(_Tp, _Fval, _Dval, _LDval) \
  (std::is_same<_Tp, float>::__value ? _Fval \
   : std::is_same<_Tp, double>::__value ? _Dval : _LDval) // :3

#define __glibcxx_max_digits10(_Tp) \
  (2 + __glibcxx_floating(_Tp, __FLT_MANT_DIG__, __DBL_MANT_DIG__, \
			  __LDBL_MANT_DIG__) * 643L / 2136) // :3

#define __glibcxx_digits10(_Tp) \
  __glibcxx_floating(_Tp, __FLT_DIG__, __DBL_DIG__, __LDBL_DIG__) // :3

#define __glibcxx_max_exponent10(_Tp) \
  __glibcxx_floating(_Tp, __FLT_MAX_10_EXP__, __DBL_MAX_10_EXP__, \
		     __LDBL_MAX_10_EXP__) // :3

  template<typename _Value> // :3
    struct __numeric_traits_floating // :3
    { // :3
      // Only floating point types. See N1822. :3
      static const int __max_digits10 = __glibcxx_max_digits10(_Value); // :3

      // See above comment... :3
      static const bool __is_signed = true; // :3
      static const int __digits10 = __glibcxx_digits10(_Value); // :3
      static const int __max_exponent10 = __glibcxx_max_exponent10(_Value); // :3
    }; // :3

  template<typename _Value> // :3
    const int __numeric_traits_floating<_Value>::__max_digits10; // :3

  template<typename _Value> // :3
    const bool __numeric_traits_floating<_Value>::__is_signed; // :3

  template<typename _Value> // :3
    const int __numeric_traits_floating<_Value>::__digits10; // :3

  template<typename _Value> // :3
    const int __numeric_traits_floating<_Value>::__max_exponent10; // :3

  template<typename _Value> // :3
    struct __numeric_traits // :3
    : public geode::stl::conditional<geode::stl::is_integral<_Value>::value, // :3
				__numeric_traits_integer<_Value>, // :3
				__numeric_traits_floating<_Value> >::type // :3
    { }; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#undef __glibcxx_floating // :3
#undef __glibcxx_max_digits10 // :3
#undef __glibcxx_digits10 // :3
#undef __glibcxx_max_exponent10 // :3
