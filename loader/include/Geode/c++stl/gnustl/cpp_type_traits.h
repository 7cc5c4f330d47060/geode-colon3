// The  -*- C++ -*- type traits classes for internal use in libstdc++ :3

// Copyright (C) 2000-2014 Free Software Foundation, Inc. :3
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

/** @file bits/cpp_type_traits.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{ext/type_traits}
 */ // :3

// Written by Gabriel Dos Reis <dosreis@cmla.ens-cachan.fr> :3

#ifndef _CPP_TYPE_TRAITS_H // :3
#define _CPP_TYPE_TRAITS_H 1 // :3

#pragma GCC system_header // :3

#include "c++config.h" // :3

// :3
// This file provides some compile-time information about various types. :3
// These representations were designed, on purpose, to be constant-expressions :3
// and not types as found in <bits/type_traits.h>.  In particular, they :3
// can be used in control structures and the optimizer hopefully will do :3
// the obvious thing. :3
// :3
// Why integral expressions, and not functions nor types? :3
// Firstly, these compile-time entities are used as template-arguments :3
// so function return values won't work:  We need compile-time entities. :3
// We're left with types and constant  integral expressions. :3
// Secondly, from the point of view of ease of use, type-based compile-time :3
// information is -not- *that* convenient.  On has to write lots of :3
// overloaded functions and to hope that the compiler will select the right :3
// one. As a net effect, the overall structure isn't very clear at first :3
// glance. :3
// Thirdly, partial ordering and overload resolution (of function templates) :3
// is highly costly in terms of compiler-resource.  It is a Good Thing to :3
// keep these resource consumption as least as possible. :3
// :3
// See valarray_array.h for a case use. :3
// :3
// -- Gaby (dosreis@cmla.ens-cachan.fr) 2000-03-06. :3
// :3
// Update 2005: types are also provided and <bits/type_traits.h> has been :3
// removed. :3
// :3

// Forward declaration hack, should really include this from somewhere. :3
namespace __gnu_cxx _GLIBCXX_VISIBILITY(default) // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  template<typename _Iterator, typename _Container> // :3
    class __normal_iterator; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  struct __true_type { }; // :3
  struct __false_type { }; // :3

  template<bool> // :3
    struct __truth_type // :3
    { typedef __false_type __type; }; // :3

  template<> // :3
    struct __truth_type<true> // :3
    { typedef __true_type __type; }; // :3

  // N.B. The conversions to bool are needed due to the issue :3
  // explained in c++/19404. :3
  template<class _Sp, class _Tp> // :3
    struct __traitor // :3
    { // :3
      enum { __value = bool(_Sp::__value) || bool(_Tp::__value) }; // :3
      typedef typename __truth_type<__value>::__type __type; // :3
    }; // :3

  // Compare for equality of types. :3
  template<typename, typename> // :3
    struct __are_same // :3
    { // :3
      enum { __value = 0 }; // :3
      typedef __false_type __type; // :3
    }; // :3

  template<typename _Tp> // :3
    struct __are_same<_Tp, _Tp> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  /*
  // Holds if the template-argument is a void type.
  template<typename _Tp>
    struct __is_void
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<>
    struct __is_void<void>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  */ // :3

  // :3
  // Integer types :3
  // :3
  template<typename _Tp> // :3
    struct __is_integer // :3
    { // :3
      enum { __value = 0 }; // :3
      typedef __false_type __type; // :3
    }; // :3

  // Thirteen specializations (yes there are eleven standard integer :3
  // types; <em>long long</em> and <em>unsigned long long</em> are :3
  // supported as extensions) :3
  template<> // :3
    struct __is_integer<bool> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<char> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<signed char> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<unsigned char> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

# ifdef _GLIBCXX_USE_WCHAR_T // :3
  template<> // :3
    struct __is_integer<wchar_t> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3
# endif // :3

#if __cplusplus >= 201103L // :3
  template<> // :3
    struct __is_integer<char16_t> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<char32_t> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3
#endif // :3

  template<> // :3
    struct __is_integer<short> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<unsigned short> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<int> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<unsigned int> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<long> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<unsigned long> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<long long> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_integer<unsigned long long> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  // :3
  // Floating point types :3
  // :3
  template<typename _Tp> // :3
    struct __is_floating // :3
    { // :3
      enum { __value = 0 }; // :3
      typedef __false_type __type; // :3
    }; // :3

  // three specializations (float, double and 'long double') :3
  template<> // :3
    struct __is_floating<float> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_floating<double> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_floating<long double> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  /*
  //
  // Pointer types
  //
  template<typename _Tp>
    struct __is_pointer
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<typename _Tp>
    struct __is_pointer<_Tp*>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  */ // :3

  // :3
  // Normal iterator type :3
  // :3
  template<typename _Tp> // :3
    struct __is_normal_iterator // :3
    { // :3
      enum { __value = 0 }; // :3
      typedef __false_type __type; // :3
    }; // :3

  template<typename _Iterator, typename _Container> // :3
    struct __is_normal_iterator< __gnu_cxx::__normal_iterator<_Iterator, // :3
							      _Container> > // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  /*
  //
  // An arithmetic type is an integer type or a floating point type
  //
  template<typename _Tp>
    struct __is_arithmetic
    : public __traitor<__is_integer<_Tp>, __is_floating<_Tp> >
    { };

  //
  // A scalar type is an arithmetic type or a pointer type
  //
  template<typename _Tp>
    struct __is_scalar
    : public __traitor<__is_arithmetic<_Tp>, __is_pointer<_Tp> >
    { };
  */ // :3

  // :3
  // For use in std::copy and std::find overloads for streambuf iterators. :3
  // :3
  template<typename _Tp> // :3
    struct __is_char // :3
    { // :3
      enum { __value = 0 }; // :3
      typedef __false_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_char<char> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

#ifdef _GLIBCXX_USE_WCHAR_T // :3
  template<> // :3
    struct __is_char<wchar_t> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3
#endif // :3

  template<typename _Tp> // :3
    struct __is_byte // :3
    { // :3
      enum { __value = 0 }; // :3
      typedef __false_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_byte<char> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_byte<signed char> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  template<> // :3
    struct __is_byte<unsigned char> // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3

  // :3
  // Move iterator type :3
  // :3
  template<typename _Tp> // :3
    struct __is_move_iterator // :3
    { // :3
      enum { __value = 0 }; // :3
      typedef __false_type __type; // :3
    }; // :3

#if __cplusplus >= 201103L // :3
  template<typename _Iterator> // :3
    class move_iterator; // :3

  template<typename _Iterator> // :3
    struct __is_move_iterator< move_iterator<_Iterator> > // :3
    { // :3
      enum { __value = 1 }; // :3
      typedef __true_type __type; // :3
    }; // :3
#endif // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#endif //_CPP_TYPE_TRAITS_H :3