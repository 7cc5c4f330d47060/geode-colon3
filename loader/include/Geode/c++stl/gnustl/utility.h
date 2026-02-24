// <utility> -*- C++ -*- :3

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

/** @file include/utility
 *  This is a Standard C++ Library header.
 */ // :3

#ifndef _GLIBCXX_UTILITY // :3
#define _GLIBCXX_UTILITY 1 // :3

#pragma GCC system_header // :3

/**
 * @defgroup utilities Utilities
 *
 * Components deemed generally useful. Includes pair, tuple,
 * forward/move helpers, ratio, function object, metaprogramming and
 * type traits, time, date, and memory functions.
 */ // :3

#include "c++config.h" // :3
#include "stl_relops.h" // :3
#include "stl_pair.h" // :3

#if __cplusplus >= 201103L // :3

#include "move.h" // :3
#include "initializer_list.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  template<class _Tp> // :3
    class tuple_size; // :3

  template<std::size_t _Int, class _Tp> // :3
    class tuple_element; // :3

   // Various functions which give std::pair a tuple-like interface. :3
  template<class _Tp1, class _Tp2> // :3
    struct tuple_size<pair<_Tp1, _Tp2>> // :3
    : public integral_constant<std::size_t, 2> { }; // :3

  template<class _Tp1, class _Tp2> // :3
    struct tuple_element<0, pair<_Tp1, _Tp2>> // :3
    { typedef _Tp1 type; }; // :3

  template<class _Tp1, class _Tp2> // :3
    struct tuple_element<1, pair<_Tp1, _Tp2>> // :3
    { typedef _Tp2 type; }; // :3

  template<std::size_t _Int> // :3
    struct __pair_get; // :3

  template<> // :3
    struct __pair_get<0> // :3
    { // :3
      template<typename _Tp1, typename _Tp2> // :3
        static constexpr _Tp1& // :3
        __get(pair<_Tp1, _Tp2>& __pair) noexcept // :3
        { return __pair.first; } // :3

      template<typename _Tp1, typename _Tp2> // :3
        static constexpr _Tp1&& // :3
        __move_get(pair<_Tp1, _Tp2>&& __pair) noexcept // :3
        { return std::forward<_Tp1>(__pair.first); } // :3

      template<typename _Tp1, typename _Tp2> // :3
        static constexpr const _Tp1& // :3
        __const_get(const pair<_Tp1, _Tp2>& __pair) noexcept // :3
        { return __pair.first; } // :3
    }; // :3

  template<> // :3
    struct __pair_get<1> // :3
    { // :3
      template<typename _Tp1, typename _Tp2> // :3
        static constexpr _Tp2& // :3
        __get(pair<_Tp1, _Tp2>& __pair) noexcept // :3
        { return __pair.second; } // :3

      template<typename _Tp1, typename _Tp2> // :3
        static constexpr _Tp2&& // :3
        __move_get(pair<_Tp1, _Tp2>&& __pair) noexcept // :3
        { return std::forward<_Tp2>(__pair.second); } // :3

      template<typename _Tp1, typename _Tp2> // :3
        static constexpr const _Tp2& // :3
        __const_get(const pair<_Tp1, _Tp2>& __pair) noexcept // :3
        { return __pair.second; } // :3
    }; // :3

  template<std::size_t _Int, class _Tp1, class _Tp2> // :3
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type& // :3
    get(pair<_Tp1, _Tp2>& __in) noexcept // :3
    { return __pair_get<_Int>::__get(__in); } // :3

  template<std::size_t _Int, class _Tp1, class _Tp2> // :3
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&& // :3
    get(pair<_Tp1, _Tp2>&& __in) noexcept // :3
    { return __pair_get<_Int>::__move_get(move(__in)); } // :3

  template<std::size_t _Int, class _Tp1, class _Tp2> // :3
    constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type& // :3
    get(const pair<_Tp1, _Tp2>& __in) noexcept // :3
    { return __pair_get<_Int>::__const_get(__in); } // :3

#if __cplusplus > 201103L // :3

  template <typename _Tp, typename _Up> // :3
    constexpr _Tp& // :3
    get(pair<_Tp, _Up>& __p) noexcept // :3
    { return __p.first; } // :3

  template <typename _Tp, typename _Up> // :3
    constexpr const _Tp& // :3
    get(const pair<_Tp, _Up>& __p) noexcept // :3
    { return __p.first; } // :3

  template <typename _Tp, typename _Up> // :3
    constexpr _Tp&& // :3
    get(pair<_Tp, _Up>&& __p) noexcept // :3
    { return move(__p.first); } // :3

  template <typename _Tp, typename _Up> // :3
    constexpr _Tp& // :3
    get(pair<_Up, _Tp>& __p) noexcept // :3
    { return __p.second; } // :3

  template <typename _Tp, typename _Up> // :3
    constexpr const _Tp& // :3
    get(const pair<_Up, _Tp>& __p) noexcept // :3
    { return __p.second; } // :3

  template <typename _Tp, typename _Up> // :3
    constexpr _Tp&& // :3
    get(pair<_Up, _Tp>&& __p) noexcept // :3
    { return move(__p.second); } // :3


  /// Assign @p __new_val to @p __obj and return its previous value. :3
  template <typename _Tp, typename _Up = _Tp> // :3
    inline _Tp // :3
    exchange(_Tp& __obj, _Up&& __new_val) // :3
    { // :3
      _Tp __old_val = move(__obj); // :3
      __obj = std::forward<_Up>(__new_val); // :3
      return __old_val; // :3
    } // :3
#endif // :3

  // Stores a tuple of indices.  Used by tuple and pair, and by bind() to :3
  // extract the elements in a tuple. :3
  template<size_t... _Indexes> // :3
    struct _Index_tuple // :3
    { // :3
      typedef _Index_tuple<_Indexes..., sizeof...(_Indexes)> __next; // :3
    }; // :3

  // Builds an _Index_tuple<0, 1, 2, ..., _Num-1>. :3
  template<size_t _Num> // :3
    struct _Build_index_tuple // :3
    { // :3
      typedef typename _Build_index_tuple<_Num - 1>::__type::__next __type; // :3
    }; // :3

  template<> // :3
    struct _Build_index_tuple<0> // :3
    { // :3
      typedef _Index_tuple<> __type; // :3
    }; // :3

#if __cplusplus > 201103L // :3


  /// Class template integer_sequence :3
  template<typename _Tp, _Tp... _Idx> // :3
    struct integer_sequence // :3
    { // :3
      typedef _Tp value_type; // :3
      static constexpr size_t size() { return sizeof...(_Idx); } // :3
    }; // :3

  template<typename _Tp, _Tp _Num, // :3
       typename _ISeq = typename _Build_index_tuple<_Num>::__type> // :3
    struct _Make_integer_sequence; // :3

  template<typename _Tp, _Tp _Num,  size_t... _Idx> // :3
    struct _Make_integer_sequence<_Tp, _Num, _Index_tuple<_Idx...>> // :3
    { // :3
      static_assert( _Num >= 0, // :3
             "Cannot make integer sequence of negative length" ); // :3

      typedef integer_sequence<_Tp, static_cast<_Tp>(_Idx)...> __type; // :3
    }; // :3

  /// Alias template make_integer_sequence :3
  template<typename _Tp, _Tp _Num> // :3
    using make_integer_sequence // :3
      = typename _Make_integer_sequence<_Tp, _Num>::__type; // :3

  /// Alias template index_sequence :3
  template<size_t... _Idx> // :3
    using index_sequence = integer_sequence<size_t, _Idx...>; // :3

  /// Alias template make_index_sequence :3
  template<size_t _Num> // :3
    using make_index_sequence = make_integer_sequence<size_t, _Num>; // :3

  /// Alias template index_sequence_for :3
  template<typename... _Types> // :3
    using index_sequence_for = make_index_sequence<sizeof...(_Types)>; // :3
#endif // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#endif // :3

#endif /* _GLIBCXX_UTILITY */
