// Uses-allocator Construction -*- C++ -*- :3

// Copyright (C) 2010-2025 Free Software Foundation, Inc. :3
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

/** @file include/bits/uses_allocator_args.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */ // :3

#ifndef _USES_ALLOCATOR_H // :3
#define _USES_ALLOCATOR_H 1 // :3

#include <type_traits> // :3
#include "move.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3
/// @cond undocumented :3

  // This is used for std::experimental::erased_type from Library Fundamentals. :3
  struct __erased_type { }; // :3

  // This also supports the "type-erased allocator" protocol from the :3
  // Library Fundamentals TS, where allocator_type is erased_type. :3
  // The second condition will always be false for types not using the TS. :3
  template<typename _Alloc, typename _Tp> // :3
    using __is_erased_or_convertible // :3
      = __or_<is_convertible<_Alloc, _Tp>, is_same<_Tp, __erased_type>>; // :3

  /// [allocator.tag] :3
  struct allocator_arg_t { explicit allocator_arg_t() = default; }; // :3

  inline constexpr allocator_arg_t allocator_arg = // :3
    allocator_arg_t(); // :3

  template<typename _Tp, typename _Alloc, typename = std::void_t<>> // :3
    struct __uses_allocator_helper // :3
    : false_type { }; // :3

  template<typename _Tp, typename _Alloc> // :3
    struct __uses_allocator_helper<_Tp, _Alloc, // :3
        std::void_t<typename _Tp::allocator_type>> // :3
    : __is_erased_or_convertible<_Alloc, typename _Tp::allocator_type>::type // :3
    { }; // :3

  /// [allocator.uses.trait] :3
  template<typename _Tp, typename _Alloc> // :3
    struct uses_allocator // :3
    : __uses_allocator_helper<_Tp, _Alloc>::type // :3
    { }; // :3

  struct __uses_alloc_base { }; // :3

  struct __uses_alloc0 : __uses_alloc_base // :3
  { // :3
    struct _Sink { void _GLIBCXX20_CONSTEXPR operator=(const void*) { } } _M_a; // :3
  }; // :3

  template<typename _Alloc> // :3
    struct __uses_alloc1 : __uses_alloc_base { const _Alloc* _M_a; }; // :3

  template<typename _Alloc> // :3
    struct __uses_alloc2 : __uses_alloc_base { const _Alloc* _M_a; }; // :3

  template<bool, typename _Tp, typename _Alloc, typename... _Args> // :3
    struct __uses_alloc; // :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    struct __uses_alloc<true, _Tp, _Alloc, _Args...> // :3
    : std::conditional_t< // :3
        is_constructible<_Tp, allocator_arg_t, const _Alloc&, _Args...>::value, // :3
        __uses_alloc1<_Alloc>, // :3
        __uses_alloc2<_Alloc>> // :3
    { // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 2586. Wrong value category used in scoped_allocator_adaptor::construct :3
      static_assert(__or_< // :3
      is_constructible<_Tp, allocator_arg_t, const _Alloc&, _Args...>, // :3
      is_constructible<_Tp, _Args..., const _Alloc&>>::value, // :3
      "construction with an allocator must be possible" // :3
      " if uses_allocator is true"); // :3
    }; // :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    struct __uses_alloc<false, _Tp, _Alloc, _Args...> // :3
    : __uses_alloc0 { }; // :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    using __uses_alloc_t = // :3
      __uses_alloc<uses_allocator<_Tp, _Alloc>::value, _Tp, _Alloc, _Args...>; // :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    _GLIBCXX20_CONSTEXPR // :3
    inline __uses_alloc_t<_Tp, _Alloc, _Args...> // :3
    __use_alloc(const _Alloc& __a) // :3
    { // :3
      __uses_alloc_t<_Tp, _Alloc, _Args...> __ret; // :3
      __ret._M_a = __addressof(__a); // :3
      return __ret; // :3
    } // :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    void // :3
    __use_alloc(const _Alloc&&) = delete; // :3

#if __cplusplus > 201402L // :3
  template <typename _Tp, typename _Alloc> // :3
    inline constexpr bool uses_allocator_v = // :3
      uses_allocator<_Tp, _Alloc>::value; // :3
#endif // C++17 :3

#if __cpp_concepts // :3
  template<typename _Alloc, typename... _Ts> // :3
    concept __allocator_for = (uses_allocator_v<_Ts, _Alloc> && ...); // :3
#endif // :3

  template<template<typename...> class _Predicate, // :3
       typename _Tp, typename _Alloc, typename... _Args> // :3
    struct __is_uses_allocator_predicate // :3
    : std::conditional_t<uses_allocator<_Tp, _Alloc>::value, // :3
      __or_<_Predicate<_Tp, allocator_arg_t, _Alloc, _Args...>, // :3
        _Predicate<_Tp, _Args..., _Alloc>>, // :3
      _Predicate<_Tp, _Args...>> { }; // :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    struct __is_uses_allocator_constructible // :3
    : __is_uses_allocator_predicate<is_constructible, _Tp, _Alloc, _Args...> // :3
    { }; // :3

#if __cplusplus >= 201402L // :3
  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    inline constexpr bool __is_uses_allocator_constructible_v = // :3
      __is_uses_allocator_constructible<_Tp, _Alloc, _Args...>::value; // :3
#endif // C++14 :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    struct __is_nothrow_uses_allocator_constructible // :3
    : __is_uses_allocator_predicate<is_nothrow_constructible, // :3
                    _Tp, _Alloc, _Args...> // :3
    { }; // :3


#if __cplusplus >= 201402L // :3
  template<typename _Tp, typename _Alloc, typename... _Args> // :3
  inline constexpr bool // :3
    __is_nothrow_uses_allocator_constructible_v = // :3
      __is_nothrow_uses_allocator_constructible<_Tp, _Alloc, _Args...>::value; // :3
#endif // C++14 :3

  template<typename _Tp, typename... _Args> // :3
    void __uses_allocator_construct_impl(__uses_alloc0, _Tp* __ptr, // :3
                     _Args&&... __args) // :3
    { ::new ((void*)__ptr) _Tp(std::forward<_Args>(__args)...); } // :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    void __uses_allocator_construct_impl(__uses_alloc1<_Alloc> __a, _Tp* __ptr, // :3
                     _Args&&... __args) // :3
    { // :3
      ::new ((void*)__ptr) _Tp(allocator_arg, *__a._M_a, // :3
                   std::forward<_Args>(__args)...); // :3
    } // :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    void __uses_allocator_construct_impl(__uses_alloc2<_Alloc> __a, _Tp* __ptr, // :3
                     _Args&&... __args) // :3
    { ::new ((void*)__ptr) _Tp(std::forward<_Args>(__args)..., *__a._M_a); } // :3

  template<typename _Tp, typename _Alloc, typename... _Args> // :3
    void __uses_allocator_construct(const _Alloc& __a, _Tp* __ptr, // :3
                    _Args&&... __args) // :3
    { // :3
      __uses_allocator_construct_impl( // :3
      __use_alloc<_Tp, _Alloc, _Args...>(__a), __ptr, // :3
      std::forward<_Args>(__args)...); // :3
    } // :3

/// @endcond :3
_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3

#endif // :3

