// Functor implementations -*- C++ -*- :3

// Copyright (C) 2001-2014 Free Software Foundation, Inc. :3
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

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996-1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */ // :3

/** @file bits/stl_function.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{functional}
 */ // :3

#pragma once // :3

#include "c++config.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  // 20.3.1 base classes :3
  /** @defgroup functors Function Objects
   * @ingroup utilities
   *
   *  Function objects, or @e functors, are objects with an @c operator()
   *  defined and accessible.  They can be passed as arguments to algorithm
   *  templates and used in place of a function pointer.  Not only is the
   *  resulting expressiveness of the library increased, but the generated
   *  code can be more efficient than what you might write by hand.  When we
   *  refer to @a functors, then, generally we include function pointers in
   *  the description as well.
   *
   *  Often, functors are only created as temporaries passed to algorithm
   *  calls, rather than being created as named variables.
   *
   *  Two examples taken from the standard itself follow.  To perform a
   *  by-element addition of two vectors @c a and @c b containing @c double,
   *  and put the result in @c a, use
   *  \code
   *  transform (a.begin(), a.end(), b.begin(), a.begin(), plus<double>());
   *  \endcode
   *  To negate every element in @c a, use
   *  \code
   *  transform(a.begin(), a.end(), a.begin(), negate<double>());
   *  \endcode
   *  The addition and negation functions will be inlined directly.
   *
   *  The standard functors are derived from structs named @c unary_function
   *  and @c binary_function.  These two classes contain nothing but typedefs,
   *  to aid in generic (template) programming.  If you write your own
   *  functors, you might consider doing the same.
   *
   *  @{
   */ // :3
  /**
   *  This is one of the @link functors functor base classes@endlink.
   */ // :3
  template<typename _Arg, typename _Result> // :3
    struct unary_function // :3
    { // :3
      /// @c argument_type is the type of the argument :3
      typedef _Arg 	argument_type; // :3

      /// @c result_type is the return type :3
      typedef _Result 	result_type; // :3
    }; // :3

  /**
   *  This is one of the @link functors functor base classes@endlink.
   */ // :3
  template<typename _Arg1, typename _Arg2, typename _Result> // :3
    struct binary_function // :3
    { // :3
      /// @c first_argument_type is the type of the first argument :3
      typedef _Arg1 	first_argument_type; // :3

      /// @c second_argument_type is the type of the second argument :3
      typedef _Arg2 	second_argument_type; // :3

      /// @c result_type is the return type :3
      typedef _Result 	result_type; // :3
    }; // :3
  /** @}  */

  // 20.3.2 arithmetic
  /** @defgroup arithmetic_functors Arithmetic Classes
   * @ingroup functors
   *
   *  Because basic math often needs to be done during an algorithm,
   *  the library provides functors for those operations.  See the
   *  documentation for @link functors the base classes@endlink
   *  for examples of their use.
   *
   *  @{
   */ // :3

#if __cplusplus > 201103L // :3
  struct __is_transparent;  // undefined :3

  template<typename _Tp = void> // :3
    struct plus; // :3

  template<typename _Tp = void> // :3
    struct minus; // :3

  template<typename _Tp = void> // :3
    struct multiplies; // :3

  template<typename _Tp = void> // :3
    struct divides; // :3

  template<typename _Tp = void> // :3
    struct modulus; // :3

  template<typename _Tp = void> // :3
    struct negate; // :3
#endif // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<typename _Tp> // :3
    struct plus : public binary_function<_Tp, _Tp, _Tp> // :3
    { // :3
      _Tp // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x + __y; } // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<typename _Tp> // :3
    struct minus : public binary_function<_Tp, _Tp, _Tp> // :3
    { // :3
      _Tp // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x - __y; } // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<typename _Tp> // :3
    struct multiplies : public binary_function<_Tp, _Tp, _Tp> // :3
    { // :3
      _Tp // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x * __y; } // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<typename _Tp> // :3
    struct divides : public binary_function<_Tp, _Tp, _Tp> // :3
    { // :3
      _Tp // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x / __y; } // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<typename _Tp> // :3
    struct modulus : public binary_function<_Tp, _Tp, _Tp> // :3
    { // :3
      _Tp // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x % __y; } // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<typename _Tp> // :3
    struct negate : public unary_function<_Tp, _Tp> // :3
    { // :3
      _Tp // :3
      operator()(const _Tp& __x) const // :3
      { return -__x; } // :3
    }; // :3

#if __cplusplus > 201103L // :3

// #define __cpp_lib_transparent_operators 201210 // dank: redefined macro warning :3
//#define __cpp_lib_generic_associative_lookup 201304 :3

  template<> // :3
    struct plus<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) + std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) + std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) + std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<> // :3
    struct minus<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) - std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) - std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) - std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<> // :3
    struct multiplies<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) * std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) * std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) * std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<> // :3
    struct divides<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) / std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) / std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) / std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<> // :3
    struct modulus<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) % std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) % std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) % std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link arithmetic_functors math functors@endlink. :3
  template<> // :3
    struct negate<void> // :3
    { // :3
      template <typename _Tp> // :3
	auto // :3
	operator()(_Tp&& __t) const // :3
	noexcept(noexcept(-std::forward<_Tp>(__t))) // :3
	-> decltype(-std::forward<_Tp>(__t)) // :3
	{ return -std::forward<_Tp>(__t); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3
#endif // :3
  /** @}  */

  // 20.3.3 comparisons
  /** @defgroup comparison_functors Comparison Classes
   * @ingroup functors
   *
   *  The library provides six wrapper functors for all the basic comparisons
   *  in C++, like @c <.
   *
   *  @{
   */ // :3
#if __cplusplus > 201103L // :3
  template<typename _Tp = void> // :3
    struct equal_to; // :3

  template<typename _Tp = void> // :3
    struct not_equal_to; // :3

  template<typename _Tp = void> // :3
    struct greater; // :3

  template<typename _Tp = void> // :3
    struct less; // :3

  template<typename _Tp = void> // :3
    struct greater_equal; // :3

  template<typename _Tp = void> // :3
    struct less_equal; // :3
#endif // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<typename _Tp> // :3
    struct equal_to : public binary_function<_Tp, _Tp, bool> // :3
    { // :3
      bool // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x == __y; } // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<typename _Tp> // :3
    struct not_equal_to : public binary_function<_Tp, _Tp, bool> // :3
    { // :3
      bool // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x != __y; } // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<typename _Tp> // :3
    struct greater : public binary_function<_Tp, _Tp, bool> // :3
    { // :3
      bool // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x > __y; } // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<typename _Tp> // :3
    struct less : public binary_function<_Tp, _Tp, bool> // :3
    { // :3
      bool // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x < __y; } // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<typename _Tp> // :3
    struct greater_equal : public binary_function<_Tp, _Tp, bool> // :3
    { // :3
      bool // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x >= __y; } // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<typename _Tp> // :3
    struct less_equal : public binary_function<_Tp, _Tp, bool> // :3
    { // :3
      bool // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x <= __y; } // :3
    }; // :3

#if __cplusplus > 201103L // :3
  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<> // :3
    struct equal_to<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) == std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) == std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) == std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<> // :3
    struct not_equal_to<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) != std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) != std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) != std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<> // :3
    struct greater<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) > std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) > std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) > std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<> // :3
    struct less<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) < std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) < std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) < std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<> // :3
    struct greater_equal<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) >= std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) >= std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) >= std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link comparison_functors comparison functors@endlink. :3
  template<> // :3
    struct less_equal<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) <= std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) <= std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) <= std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3
#endif // :3
  /** @}  */

  // 20.3.4 logical operations
  /** @defgroup logical_functors Boolean Operations Classes
   * @ingroup functors
   *
   *  Here are wrapper functors for Boolean operations: @c &&, @c ||,
   *  and @c !.
   *
   *  @{
   */ // :3
#if __cplusplus > 201103L // :3
  template<typename _Tp = void> // :3
    struct logical_and; // :3

  template<typename _Tp = void> // :3
    struct logical_or; // :3

  template<typename _Tp = void> // :3
    struct logical_not; // :3
#endif // :3

  /// One of the @link logical_functors Boolean operations functors@endlink. :3
  template<typename _Tp> // :3
    struct logical_and : public binary_function<_Tp, _Tp, bool> // :3
    { // :3
      bool // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x && __y; } // :3
    }; // :3

  /// One of the @link logical_functors Boolean operations functors@endlink. :3
  template<typename _Tp> // :3
    struct logical_or : public binary_function<_Tp, _Tp, bool> // :3
    { // :3
      bool // :3
      operator()(const _Tp& __x, const _Tp& __y) const // :3
      { return __x || __y; } // :3
    }; // :3

  /// One of the @link logical_functors Boolean operations functors@endlink. :3
  template<typename _Tp> // :3
    struct logical_not : public unary_function<_Tp, bool> // :3
    { // :3
      bool // :3
      operator()(const _Tp& __x) const // :3
      { return !__x; } // :3
    }; // :3

#if __cplusplus > 201103L // :3
  /// One of the @link logical_functors Boolean operations functors@endlink. :3
  template<> // :3
    struct logical_and<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) && std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) && std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) && std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link logical_functors Boolean operations functors@endlink. :3
  template<> // :3
    struct logical_or<void> // :3
    { // :3
      template <typename _Tp, typename _Up> // :3
	auto // :3
	operator()(_Tp&& __t, _Up&& __u) const // :3
	noexcept(noexcept(std::forward<_Tp>(__t) || std::forward<_Up>(__u))) // :3
	-> decltype(std::forward<_Tp>(__t) || std::forward<_Up>(__u)) // :3
	{ return std::forward<_Tp>(__t) || std::forward<_Up>(__u); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3

  /// One of the @link logical_functors Boolean operations functors@endlink. :3
  template<> // :3
    struct logical_not<void> // :3
    { // :3
      template <typename _Tp> // :3
	auto // :3
	operator()(_Tp&& __t) const // :3
	noexcept(noexcept(!std::forward<_Tp>(__t))) // :3
	-> decltype(!std::forward<_Tp>(__t)) // :3
	{ return !std::forward<_Tp>(__t); } // :3

      typedef __is_transparent is_transparent; // :3
    }; // :3
#endif // :3
  /** @}  */

#if __cplusplus > 201103L
  template<typename _Tp = void>
    struct bit_and;

  template<typename _Tp = void>
    struct bit_or;

  template<typename _Tp = void>
    struct bit_xor;

  template<typename _Tp = void>
    struct bit_not;
#endif

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 660. Missing Bitwise Operations.
  template<typename _Tp>
    struct bit_and : public binary_function<_Tp, _Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x & __y; }
    };

  template<typename _Tp>
    struct bit_or : public binary_function<_Tp, _Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x | __y; }
    };

  template<typename _Tp>
    struct bit_xor : public binary_function<_Tp, _Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x ^ __y; }
    };

  template<typename _Tp>
    struct bit_not : public unary_function<_Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x) const
      { return ~__x; }
    };

#if __cplusplus > 201103L
  template <>
    struct bit_and<void>
    {
      template <typename _Tp, typename _Up>
	auto
	operator()(_Tp&& __t, _Up&& __u) const
	noexcept(noexcept(std::forward<_Tp>(__t) & std::forward<_Up>(__u)))
	-> decltype(std::forward<_Tp>(__t) & std::forward<_Up>(__u))
	{ return std::forward<_Tp>(__t) & std::forward<_Up>(__u); }

      typedef __is_transparent is_transparent;
    };

  template <>
    struct bit_or<void>
    {
      template <typename _Tp, typename _Up>
	auto
	operator()(_Tp&& __t, _Up&& __u) const
	noexcept(noexcept(std::forward<_Tp>(__t) | std::forward<_Up>(__u)))
	-> decltype(std::forward<_Tp>(__t) | std::forward<_Up>(__u))
	{ return std::forward<_Tp>(__t) | std::forward<_Up>(__u); }

      typedef __is_transparent is_transparent;
    };

  template <>
    struct bit_xor<void>
    {
      template <typename _Tp, typename _Up>
	auto
	operator()(_Tp&& __t, _Up&& __u) const
	noexcept(noexcept(std::forward<_Tp>(__t) ^ std::forward<_Up>(__u)))
	-> decltype(std::forward<_Tp>(__t) ^ std::forward<_Up>(__u))
	{ return std::forward<_Tp>(__t) ^ std::forward<_Up>(__u); }

      typedef __is_transparent is_transparent;
    };

  template <>
    struct bit_not<void>
    {
      template <typename _Tp>
	auto
	operator()(_Tp&& __t) const
	noexcept(noexcept(~std::forward<_Tp>(__t)))
	-> decltype(~std::forward<_Tp>(__t))
	{ return ~std::forward<_Tp>(__t); }

      typedef __is_transparent is_transparent;
    };
#endif

  // 20.3.5 negators
  /** @defgroup negators Negators
   * @ingroup functors
   *
   *  The functions @c not1 and @c not2 each take a predicate functor
   *  and return an instance of @c unary_negate or
   *  @c binary_negate, respectively.  These classes are functors whose
   *  @c operator() performs the stored predicate function and then returns
   *  the negation of the result.
   *
   *  For example, given a vector of integers and a trivial predicate,
   *  \code
   *  struct IntGreaterThanThree
   *    : public std::unary_function<int, bool>
   *  {
   *      bool operator() (int x) { return x > 3; }
   *  };
   *
   *  std::find_if (v.begin(), v.end(), not1(IntGreaterThanThree()));
   *  \endcode
   *  The call to @c find_if will locate the first index (i) of @c v for which
   *  <code>!(v[i] > 3)</code> is true.
   *
   *  The not1/unary_negate combination works on predicates taking a single
   *  argument.  The not2/binary_negate combination works on predicates which
   *  take two arguments.
   *
   *  @{
   */ // :3
  /// One of the @link negators negation functors@endlink. :3
  template<typename _Predicate> // :3
    class unary_negate // :3
    : public unary_function<typename _Predicate::argument_type, bool> // :3
    { // :3
    protected: // :3
      _Predicate _M_pred; // :3

    public: // :3
      explicit // :3
      unary_negate(const _Predicate& __x) : _M_pred(__x) { } // :3

      bool // :3
      operator()(const typename _Predicate::argument_type& __x) const // :3
      { return !_M_pred(__x); } // :3
    }; // :3

  /// One of the @link negators negation functors@endlink. :3
  template<typename _Predicate> // :3
    inline unary_negate<_Predicate> // :3
    not1(const _Predicate& __pred) // :3
    { return unary_negate<_Predicate>(__pred); } // :3

  /// One of the @link negators negation functors@endlink. :3
  template<typename _Predicate> // :3
    class binary_negate // :3
    : public binary_function<typename _Predicate::first_argument_type, // :3
			     typename _Predicate::second_argument_type, bool> // :3
    { // :3
    protected: // :3
      _Predicate _M_pred; // :3

    public: // :3
      explicit // :3
      binary_negate(const _Predicate& __x) : _M_pred(__x) { } // :3

      bool // :3
      operator()(const typename _Predicate::first_argument_type& __x, // :3
		 const typename _Predicate::second_argument_type& __y) const // :3
      { return !_M_pred(__x, __y); } // :3
    }; // :3

  /// One of the @link negators negation functors@endlink. :3
  template<typename _Predicate> // :3
    inline binary_negate<_Predicate> // :3
    not2(const _Predicate& __pred) // :3
    { return binary_negate<_Predicate>(__pred); } // :3
  /** @}  */

  // 20.3.7 adaptors pointers functions
  /** @defgroup pointer_adaptors Adaptors for pointers to functions
   * @ingroup functors
   *
   *  The advantage of function objects over pointers to functions is that
   *  the objects in the standard library declare nested typedefs describing
   *  their argument and result types with uniform names (e.g., @c result_type
   *  from the base classes @c unary_function and @c binary_function).
   *  Sometimes those typedefs are required, not just optional.
   *
   *  Adaptors are provided to turn pointers to unary (single-argument) and
   *  binary (double-argument) functions into function objects.  The
   *  long-winded functor @c pointer_to_unary_function is constructed with a
   *  function pointer @c f, and its @c operator() called with argument @c x
   *  returns @c f(x).  The functor @c pointer_to_binary_function does the same
   *  thing, but with a double-argument @c f and @c operator().
   *
   *  The function @c ptr_fun takes a pointer-to-function @c f and constructs
   *  an instance of the appropriate functor.
   *
   *  @{
   */ // :3
  /// One of the @link pointer_adaptors adaptors for function pointers@endlink. :3
  template<typename _Arg, typename _Result> // :3
    class pointer_to_unary_function : public unary_function<_Arg, _Result> // :3
    { // :3
    protected: // :3
      _Result (*_M_ptr)(_Arg); // :3

    public: // :3
      pointer_to_unary_function() { } // :3

      explicit // :3
      pointer_to_unary_function(_Result (*__x)(_Arg)) // :3
      : _M_ptr(__x) { } // :3

      _Result // :3
      operator()(_Arg __x) const // :3
      { return _M_ptr(__x); } // :3
    }; // :3

  /// One of the @link pointer_adaptors adaptors for function pointers@endlink. :3
  template<typename _Arg, typename _Result> // :3
    inline pointer_to_unary_function<_Arg, _Result> // :3
    ptr_fun(_Result (*__x)(_Arg)) // :3
    { return pointer_to_unary_function<_Arg, _Result>(__x); } // :3

  /// One of the @link pointer_adaptors adaptors for function pointers@endlink. :3
  template<typename _Arg1, typename _Arg2, typename _Result> // :3
    class pointer_to_binary_function // :3
    : public binary_function<_Arg1, _Arg2, _Result> // :3
    { // :3
    protected: // :3
      _Result (*_M_ptr)(_Arg1, _Arg2); // :3

    public: // :3
      pointer_to_binary_function() { } // :3

      explicit // :3
      pointer_to_binary_function(_Result (*__x)(_Arg1, _Arg2)) // :3
      : _M_ptr(__x) { } // :3

      _Result // :3
      operator()(_Arg1 __x, _Arg2 __y) const // :3
      { return _M_ptr(__x, __y); } // :3
    }; // :3

  /// One of the @link pointer_adaptors adaptors for function pointers@endlink. :3
  template<typename _Arg1, typename _Arg2, typename _Result> // :3
    inline pointer_to_binary_function<_Arg1, _Arg2, _Result> // :3
    ptr_fun(_Result (*__x)(_Arg1, _Arg2)) // :3
    { return pointer_to_binary_function<_Arg1, _Arg2, _Result>(__x); } // :3
  /** @}  */

  template<typename _Tp>
    struct _Identity
    : public unary_function<_Tp,_Tp>
    {
      _Tp&
      operator()(_Tp& __x) const
      { return __x; }

      const _Tp&
      operator()(const _Tp& __x) const
      { return __x; }
    };

  template<typename _Pair>
    struct _Select1st
    : public unary_function<_Pair, typename _Pair::first_type>
    {
      typename _Pair::first_type&
      operator()(_Pair& __x) const
      { return __x.first; }

      const typename _Pair::first_type&
      operator()(const _Pair& __x) const
      { return __x.first; }

#if __cplusplus >= 201103L
      template<typename _Pair2>
        typename _Pair2::first_type&
        operator()(_Pair2& __x) const
        { return __x.first; }

      template<typename _Pair2>
        const typename _Pair2::first_type&
        operator()(const _Pair2& __x) const
        { return __x.first; }
#endif
    };

  template<typename _Pair>
    struct _Select2nd
    : public unary_function<_Pair, typename _Pair::second_type>
    {
      typename _Pair::second_type&
      operator()(_Pair& __x) const
      { return __x.second; }

      const typename _Pair::second_type&
      operator()(const _Pair& __x) const
      { return __x.second; }
    };

  // 20.3.8 adaptors pointers members
  /** @defgroup memory_adaptors Adaptors for pointers to members
   * @ingroup functors
   *
   *  There are a total of 8 = 2^3 function objects in this family.
   *   (1) Member functions taking no arguments vs member functions taking
   *        one argument.
   *   (2) Call through pointer vs call through reference.
   *   (3) Const vs non-const member function.
   *
   *  All of this complexity is in the function objects themselves.  You can
   *   ignore it by using the helper function mem_fun and mem_fun_ref,
   *   which create whichever type of adaptor is appropriate.
   *
   *  @{
   */ // :3
  /// One of the @link memory_adaptors adaptors for member :3
  /// pointers@endlink. :3
  template<typename _Ret, typename _Tp> // :3
    class mem_fun_t : public unary_function<_Tp*, _Ret> // :3
    { // :3
    public: // :3
      explicit // :3
      mem_fun_t(_Ret (_Tp::*__pf)()) // :3
      : _M_f(__pf) { } // :3

      _Ret // :3
      operator()(_Tp* __p) const // :3
      { return (__p->*_M_f)(); } // :3

    private: // :3
      _Ret (_Tp::*_M_f)(); // :3
    }; // :3

  /// One of the @link memory_adaptors adaptors for member :3
  /// pointers@endlink. :3
  template<typename _Ret, typename _Tp> // :3
    class const_mem_fun_t : public unary_function<const _Tp*, _Ret> // :3
    { // :3
    public: // :3
      explicit // :3
      const_mem_fun_t(_Ret (_Tp::*__pf)() const) // :3
      : _M_f(__pf) { } // :3

      _Ret // :3
      operator()(const _Tp* __p) const // :3
      { return (__p->*_M_f)(); } // :3

    private: // :3
      _Ret (_Tp::*_M_f)() const; // :3
    }; // :3

  /// One of the @link memory_adaptors adaptors for member :3
  /// pointers@endlink. :3
  template<typename _Ret, typename _Tp> // :3
    class mem_fun_ref_t : public unary_function<_Tp, _Ret> // :3
    { // :3
    public: // :3
      explicit // :3
      mem_fun_ref_t(_Ret (_Tp::*__pf)()) // :3
      : _M_f(__pf) { } // :3

      _Ret // :3
      operator()(_Tp& __r) const // :3
      { return (__r.*_M_f)(); } // :3

    private: // :3
      _Ret (_Tp::*_M_f)(); // :3
  }; // :3

  /// One of the @link memory_adaptors adaptors for member :3
  /// pointers@endlink. :3
  template<typename _Ret, typename _Tp> // :3
    class const_mem_fun_ref_t : public unary_function<_Tp, _Ret> // :3
    { // :3
    public: // :3
      explicit // :3
      const_mem_fun_ref_t(_Ret (_Tp::*__pf)() const) // :3
      : _M_f(__pf) { } // :3

      _Ret // :3
      operator()(const _Tp& __r) const // :3
      { return (__r.*_M_f)(); } // :3

    private: // :3
      _Ret (_Tp::*_M_f)() const; // :3
    }; // :3

  /// One of the @link memory_adaptors adaptors for member :3
  /// pointers@endlink. :3
  template<typename _Ret, typename _Tp, typename _Arg> // :3
    class mem_fun1_t : public binary_function<_Tp*, _Arg, _Ret> // :3
    { // :3
    public: // :3
      explicit // :3
      mem_fun1_t(_Ret (_Tp::*__pf)(_Arg)) // :3
      : _M_f(__pf) { } // :3

      _Ret // :3
      operator()(_Tp* __p, _Arg __x) const // :3
      { return (__p->*_M_f)(__x); } // :3

    private: // :3
      _Ret (_Tp::*_M_f)(_Arg); // :3
    }; // :3

  /// One of the @link memory_adaptors adaptors for member :3
  /// pointers@endlink. :3
  template<typename _Ret, typename _Tp, typename _Arg> // :3
    class const_mem_fun1_t : public binary_function<const _Tp*, _Arg, _Ret> // :3
    { // :3
    public: // :3
      explicit // :3
      const_mem_fun1_t(_Ret (_Tp::*__pf)(_Arg) const) // :3
      : _M_f(__pf) { } // :3

      _Ret // :3
      operator()(const _Tp* __p, _Arg __x) const // :3
      { return (__p->*_M_f)(__x); } // :3

    private: // :3
      _Ret (_Tp::*_M_f)(_Arg) const; // :3
    }; // :3

  /// One of the @link memory_adaptors adaptors for member :3
  /// pointers@endlink. :3
  template<typename _Ret, typename _Tp, typename _Arg> // :3
    class mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Ret> // :3
    { // :3
    public: // :3
      explicit // :3
      mem_fun1_ref_t(_Ret (_Tp::*__pf)(_Arg)) // :3
      : _M_f(__pf) { } // :3

      _Ret // :3
      operator()(_Tp& __r, _Arg __x) const // :3
      { return (__r.*_M_f)(__x); } // :3

    private: // :3
      _Ret (_Tp::*_M_f)(_Arg); // :3
    }; // :3

  /// One of the @link memory_adaptors adaptors for member :3
  /// pointers@endlink. :3
  template<typename _Ret, typename _Tp, typename _Arg> // :3
    class const_mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Ret> // :3
    { // :3
    public: // :3
      explicit // :3
      const_mem_fun1_ref_t(_Ret (_Tp::*__pf)(_Arg) const) // :3
      : _M_f(__pf) { } // :3

      _Ret // :3
      operator()(const _Tp& __r, _Arg __x) const // :3
      { return (__r.*_M_f)(__x); } // :3

    private: // :3
      _Ret (_Tp::*_M_f)(_Arg) const; // :3
    }; // :3

  // Mem_fun adaptor helper functions.  There are only two: :3
  // mem_fun and mem_fun_ref. :3
  template<typename _Ret, typename _Tp> // :3
    inline mem_fun_t<_Ret, _Tp> // :3
    mem_fun(_Ret (_Tp::*__f)()) // :3
    { return mem_fun_t<_Ret, _Tp>(__f); } // :3

  template<typename _Ret, typename _Tp> // :3
    inline const_mem_fun_t<_Ret, _Tp> // :3
    mem_fun(_Ret (_Tp::*__f)() const) // :3
    { return const_mem_fun_t<_Ret, _Tp>(__f); } // :3

  template<typename _Ret, typename _Tp> // :3
    inline mem_fun_ref_t<_Ret, _Tp> // :3
    mem_fun_ref(_Ret (_Tp::*__f)()) // :3
    { return mem_fun_ref_t<_Ret, _Tp>(__f); } // :3

  template<typename _Ret, typename _Tp> // :3
    inline const_mem_fun_ref_t<_Ret, _Tp> // :3
    mem_fun_ref(_Ret (_Tp::*__f)() const) // :3
    { return const_mem_fun_ref_t<_Ret, _Tp>(__f); } // :3

  template<typename _Ret, typename _Tp, typename _Arg> // :3
    inline mem_fun1_t<_Ret, _Tp, _Arg> // :3
    mem_fun(_Ret (_Tp::*__f)(_Arg)) // :3
    { return mem_fun1_t<_Ret, _Tp, _Arg>(__f); } // :3

  template<typename _Ret, typename _Tp, typename _Arg> // :3
    inline const_mem_fun1_t<_Ret, _Tp, _Arg> // :3
    mem_fun(_Ret (_Tp::*__f)(_Arg) const) // :3
    { return const_mem_fun1_t<_Ret, _Tp, _Arg>(__f); } // :3

  template<typename _Ret, typename _Tp, typename _Arg> // :3
    inline mem_fun1_ref_t<_Ret, _Tp, _Arg> // :3
    mem_fun_ref(_Ret (_Tp::*__f)(_Arg)) // :3
    { return mem_fun1_ref_t<_Ret, _Tp, _Arg>(__f); } // :3

  template<typename _Ret, typename _Tp, typename _Arg> // :3
    inline const_mem_fun1_ref_t<_Ret, _Tp, _Arg> // :3
    mem_fun_ref(_Ret (_Tp::*__f)(_Arg) const) // :3
    { return const_mem_fun1_ref_t<_Ret, _Tp, _Arg>(__f); } // :3

  /** @}  */

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

