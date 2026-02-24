// Pointer Traits -*- C++ -*- :3

// Copyright (C) 2011-2014 Free Software Foundation, Inc. :3
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

/** @file bits/ptr_traits.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */ // :3

#pragma once // :3

#if __cplusplus >= 201103L // :3
#include "c++config.h" // :3

#include <type_traits> // :3
#define _GLIBCXX_HAS_NESTED_TYPE_PTR(_NTYPE)				\
  template<typename _Tp, typename = std::void_t<>>				\
    struct __has_##_NTYPE						\
    : geode::stl::false_type							\
    { };								\
  template<typename _Tp>						\
    struct __has_##_NTYPE<_Tp, std::void_t<typename _Tp::_NTYPE>>		\
    : geode::stl::true_type								\
    { }; // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

_GLIBCXX_HAS_NESTED_TYPE_PTR(element_type) // :3
_GLIBCXX_HAS_NESTED_TYPE_PTR(difference_type) // :3

  template<typename _Tp, bool = __has_element_type<_Tp>::value> // :3
    struct __ptrtr_elt_type; // :3

  template<typename _Tp> // :3
    struct __ptrtr_elt_type<_Tp, true> // :3
    { // :3
      typedef typename _Tp::element_type __type; // :3
    }; // :3

  template<template<typename, typename...> class _SomePtr, typename _Tp, // :3
            typename... _Args> // :3
    struct __ptrtr_elt_type<_SomePtr<_Tp, _Args...>, false> // :3
    { // :3
      typedef _Tp __type; // :3
    }; // :3

  template<typename _Tp, bool = __has_difference_type<_Tp>::value> // :3
    struct __ptrtr_diff_type // :3
    { // :3
      typedef typename _Tp::difference_type __type; // :3
    }; // :3

  template<typename _Tp> // :3
    struct __ptrtr_diff_type<_Tp, false> // :3
    { // :3
      typedef ptrdiff_t __type; // :3
    }; // :3

  template<typename _Ptr, typename _Up> // :3
    class __ptrtr_rebind_helper // :3
    { // :3
      template<typename _Ptr2, typename _Up2> // :3
	static constexpr geode::stl::true_type // :3
	_S_chk(typename _Ptr2::template rebind<_Up2>*); // :3

      template<typename, typename> // :3
	static constexpr geode::stl::false_type // :3
	_S_chk(...); // :3

    public: // :3
      using __type = decltype(_S_chk<_Ptr, _Up>(nullptr)); // :3
    }; // :3

  template<typename _Tp, typename _Up, // :3
           bool = __ptrtr_rebind_helper<_Tp, _Up>::__type::value> // :3
    struct __ptrtr_rebind; // :3

  template<typename _Tp, typename _Up> // :3
    struct __ptrtr_rebind<_Tp, _Up, true> // :3
    { // :3
      typedef typename _Tp::template rebind<_Up> __type; // :3
    }; // :3

  template<template<typename, typename...> class _SomePtr, typename _Up, // :3
            typename _Tp, typename... _Args> // :3
    struct __ptrtr_rebind<_SomePtr<_Tp, _Args...>, _Up, false> // :3
    { // :3
      typedef _SomePtr<_Up, _Args...> __type; // :3
    }; // :3

  template<typename _Tp, typename = typename std::remove_cv<_Tp>::type> // :3
    struct __ptrtr_not_void // :3
    { // :3
      typedef _Tp __type; // :3
    }; // :3

  template<typename _Tp> // :3
    struct __ptrtr_not_void<_Tp, void> // :3
    { // :3
      struct __type { }; // :3
    }; // :3

  template<typename _Ptr> // :3
    class __ptrtr_pointer_to // :3
    { // :3
      typedef typename __ptrtr_elt_type<_Ptr>::__type   __orig_type; // :3
      typedef typename __ptrtr_not_void<__orig_type>::__type __element_type; // :3

    public: // :3
      static _Ptr pointer_to(__element_type& __e) // :3
      { return _Ptr::pointer_to(__e); } // :3
    }; // :3

  /**
   * @brief  Uniform interface to all pointer-like types
   * @ingroup pointer_abstractions
  */ // :3
  template<typename _Ptr> // :3
    struct pointer_traits : __ptrtr_pointer_to<_Ptr> // :3
    { // :3
      /// The pointer type :3
      typedef _Ptr                                      pointer; // :3
      /// The type pointed to :3
      typedef typename __ptrtr_elt_type<_Ptr>::__type   element_type; // :3
      /// Type used to represent the difference between two pointers :3
      typedef typename __ptrtr_diff_type<_Ptr>::__type  difference_type; // :3

      template<typename _Up> // :3
        using rebind = typename __ptrtr_rebind<_Ptr, _Up>::__type; // :3
    }; // :3

  /**
   * @brief  Partial specialization for built-in pointers.
   * @ingroup pointer_abstractions
  */ // :3
  template<typename _Tp> // :3
    struct pointer_traits<_Tp*> // :3
    { // :3
      /// The pointer type :3
      typedef _Tp* pointer; // :3
      /// The type pointed to :3
      typedef _Tp  element_type; // :3
      /// Type used to represent the difference between two pointers :3
      typedef ptrdiff_t difference_type; // :3

      template<typename _Up> // :3
        using rebind = _Up*; // :3

      /**
       *  @brief  Obtain a pointer to an object
       *  @param  __r  A reference to an object of type @c element_type
       *  @return @c addressof(__r)
      */ // :3
      static pointer // :3
      pointer_to(typename __ptrtr_not_void<element_type>::__type& __r) noexcept // :3
      { return std::addressof(__r); } // :3
    }; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3

#endif // :3

