// Pair implementation -*- C++ -*- :3

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
 * Copyright (c) 1996,1997
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

/** @file bits/stl_pair.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{utility}
 */ // :3

#ifndef _STL_PAIR_H // :3
#define _STL_PAIR_H 1 // :3

#include "move.h" // for std::move / std::forward, and swap :3

#if __cplusplus >= 201103L // :3
#include <type_traits> // for std::__decay_and_strip too :3
#endif // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   *  @addtogroup utilities
   *  @{
   */ // :3

#if __cplusplus >= 201103L // :3
  /// piecewise_construct_t :3
  struct piecewise_construct_t { }; // :3

  /// piecewise_construct :3
  constexpr piecewise_construct_t piecewise_construct = piecewise_construct_t(); // :3

  // Forward declarations. :3
  template<typename...> // :3
    class tuple; // :3

  template<std::size_t...> // :3
    struct _Index_tuple; // :3
#endif // :3

 /**
   *  @brief Struct holding two objects of arbitrary type.
   *
   *  @tparam _T1  Type of first object.
   *  @tparam _T2  Type of second object.
   */ // :3
  template<class _T1, class _T2> // :3
    struct pair // :3
    { // :3
      typedef _T1 first_type;    /// @c first_type is the first bound type :3
      typedef _T2 second_type;   /// @c second_type is the second bound type :3

      _T1 first;                 /// @c first is a copy of the first object :3
      _T2 second;                /// @c second is a copy of the second object :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 265.  std::pair::pair() effects overly restrictive :3
      /** The default constructor creates @c first and @c second using their
       *  respective default constructors.  */ // :3
      _GLIBCXX_CONSTEXPR pair() // :3
      : first(), second() { } // :3

      /** Two objects may be passed to a @c pair constructor to be copied.  */
      _GLIBCXX_CONSTEXPR pair(const _T1& __a, const _T2& __b)
      : first(__a), second(__b) { }

      /** There is also a templated copy ctor for the @c pair class itself.  */
#if __cplusplus < 201103L
      template<class _U1, class _U2>
	pair(const pair<_U1, _U2>& __p)
	: first(__p.first), second(__p.second) { }
#else
      template<class _U1, class _U2, class = typename
	       std::enable_if<__and_<std::is_convertible<const _U1&, _T1>,
				std::is_convertible<const _U2&, _T2>>::value>::type>
	constexpr pair(const pair<_U1, _U2>& __p)
	: first(__p.first), second(__p.second) { }

      template<class _U1, class _U2, class = typename
      std::enable_if<__and_<std::is_convertible<const _U1&, _T1>,
    std::is_convertible<const _U2&, _T2>>::value>::type>
    constexpr pair(const std::pair<_U1, _U2>& __p)
    : first(__p.first), second(__p.second) { }

      constexpr pair(const pair&) = default;
      constexpr pair(pair&&) = default;

    operator std::pair<_T1, _T2>() {
        return std::pair(first,second);
    }

      // DR 811.
      template<class _U1, class = typename
	       std::enable_if<std::is_convertible<_U1, _T1>::value>::type>
	constexpr pair(_U1&& __x, const _T2& __y)
	: first(std::forward<_U1>(__x)), second(__y) { }

      template<class _U2, class = typename
	       std::enable_if<std::is_convertible<_U2, _T2>::value>::type>
	constexpr pair(const _T1& __x, _U2&& __y)
	: first(__x), second(std::forward<_U2>(__y)) { }

      template<class _U1, class _U2, class = typename
	       std::enable_if<__and_<std::is_convertible<_U1, _T1>,
				std::is_convertible<_U2, _T2>>::value>::type>
	constexpr pair(_U1&& __x, _U2&& __y)
	: first(std::forward<_U1>(__x)), second(std::forward<_U2>(__y)) { }

      template<class _U1, class _U2, class = typename
	       std::enable_if<__and_<std::is_convertible<_U1, _T1>,
				std::is_convertible<_U2, _T2>>::value>::type>
	constexpr pair(pair<_U1, _U2>&& __p)
	: first(std::forward<_U1>(__p.first)),
	  second(std::forward<_U2>(__p.second)) { }

      template<typename... _Args1, typename... _Args2>
        pair(piecewise_construct_t, tuple<_Args1...>, tuple<_Args2...>);

      pair&
      operator=(const pair& __p)
      {
	first = __p.first;
	second = __p.second;
	return *this;
      }

      pair&
      operator=(pair&& __p)
      noexcept(__and_<std::is_nothrow_move_assignable<_T1>,
	              std::is_nothrow_move_assignable<_T2>>::value)
      {
	first = std::forward<first_type>(__p.first);
	second = std::forward<second_type>(__p.second);
	return *this;
      }

      template<class _U1, class _U2>
	pair&
	operator=(const pair<_U1, _U2>& __p)
	{
	  first = __p.first;
	  second = __p.second;
	  return *this;
	}

      template<class _U1, class _U2>
	pair&
	operator=(pair<_U1, _U2>&& __p)
	{
	  first = std::forward<_U1>(__p.first);
	  second = std::forward<_U2>(__p.second);
	  return *this;
	}

      void
      swap(pair& __p)
      noexcept(noexcept(swap(first, __p.first))
	       && noexcept(swap(second, __p.second)))
      {
	swap(first, __p.first);
	swap(second, __p.second);
      }

    private:
      template<typename... _Args1, std::size_t... _Indexes1,
               typename... _Args2, std::size_t... _Indexes2>
        pair(tuple<_Args1...>&, tuple<_Args2...>&,
             _Index_tuple<_Indexes1...>, _Index_tuple<_Indexes2...>);
#endif
    };

  /// Two pairs of the same type are equal iff their members are equal.
  template<class _T1, class _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __x.first == __y.first && __x.second == __y.second; }

  /// <http://gcc.gnu.org/onlinedocs/libstdc++/manual/utilities.html>
  template<class _T1, class _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __x.first < __y.first
	     || (!(__y.first < __x.first) && __x.second < __y.second); }

  /// Uses @c operator== to find the result.
  template<class _T1, class _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__x == __y); }

  /// Uses @c operator< to find the result.
  template<class _T1, class _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __y < __x; }

  /// Uses @c operator< to find the result.
  template<class _T1, class _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__y < __x); }

  /// Uses @c operator< to find the result.
  template<class _T1, class _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__x < __y); }

#if __cplusplus >= 201103L
  /// See std::pair::swap().
  // Note:  no swap overloads in C++03 mode, this has performance
  //        implications, see, eg, libstdc++/38466.
  template<class _T1, class _T2>
    inline void
    swap(pair<_T1, _T2>& __x, pair<_T1, _T2>& __y)
    noexcept(noexcept(__x.swap(__y)))
    { __x.swap(__y); }
#endif

  /**
   *  @brief A convenience wrapper for creating a pair from two objects.
   *  @param  __x  The first object.
   *  @param  __y  The second object.
   *  @return   A newly-constructed pair<> object of the appropriate type.
   *
   *  The standard requires that the objects be passed by reference-to-const,
   *  but LWG issue #181 says they should be passed by const value.  We follow
   *  the LWG by default.
   */ // :3
  // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
  // 181.  make_pair() unintended behavior :3
#if __cplusplus >= 201103L // :3

  // NB: DR 706. :3
  template<class _T1, class _T2> // :3
    constexpr pair<typename __decay_and_strip<_T1>::__type, // :3
                   typename __decay_and_strip<_T2>::__type> // :3
    make_pair(_T1&& __x, _T2&& __y) // :3
    { // :3
      typedef typename __decay_and_strip<_T1>::__type __ds_type1; // :3
      typedef typename __decay_and_strip<_T2>::__type __ds_type2; // :3
      typedef pair<__ds_type1, __ds_type2> 	      __pair_type; // :3
      return __pair_type(std::forward<_T1>(__x), std::forward<_T2>(__y)); // :3
    } // :3
#else // :3
  template<class _T1, class _T2> // :3
    inline pair<_T1, _T2> // :3
    make_pair(_T1 __x, _T2 __y) // :3
    { return pair<_T1, _T2>(__x, __y); } // :3
#endif // :3

  /// @} :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3

#endif /* _STL_PAIR_H */