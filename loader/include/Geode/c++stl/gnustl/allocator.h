// Allocators -*- C++ -*- :3

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
 * Copyright (c) 1996-1997
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

/** @file bits/allocator.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */ // :3

#ifndef _ALLOCATOR_H // :3
#define _ALLOCATOR_H 1 // :3

#include "c++allocator.hpp" // Define the base class to std::allocator. :3
#include "memoryfwd.h" // :3
#if __cplusplus >= 201103L // :3
#include <type_traits> // :3
#endif // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   *  @addtogroup allocators
   *  @{
   */ // :3

  /// allocator<void> specialization. :3
  template<> // :3
    class allocator<void> // :3
    { // :3
    public: // :3
      typedef size_t      size_type; // :3
      typedef ptrdiff_t   difference_type; // :3
      typedef void*       pointer; // :3
      typedef const void* const_pointer; // :3
      typedef void        value_type; // :3

      template<typename _Tp1> // :3
        struct rebind // :3
        { typedef allocator<_Tp1> other; }; // :3

#if __cplusplus >= 201103L // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 2103. std::allocator propagate_on_container_move_assignment :3
      typedef geode::stl::true_type propagate_on_container_move_assignment; // :3
#endif // :3
    }; // :3

  /**
   * @brief  The @a standard allocator, as per [20.4].
   *
   *  See http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt04ch11.html
   *  for further details.
   *
   *  @tparam  _Tp  Type of allocated object.
   */ // :3
  template<typename _Tp> // :3
    class allocator: public __allocator_base<_Tp> // :3
    { // :3
   public: // :3
      typedef size_t     size_type; // :3
      typedef ptrdiff_t  difference_type; // :3
      typedef _Tp*       pointer; // :3
      typedef const _Tp* const_pointer; // :3
      typedef _Tp&       reference; // :3
      typedef const _Tp& const_reference; // :3
      typedef _Tp        value_type; // :3

      template<typename _Tp1> // :3
        struct rebind // :3
        { typedef allocator<_Tp1> other; }; // :3

#if __cplusplus >= 201103L // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 2103. std::allocator propagate_on_container_move_assignment :3
      typedef geode::stl::true_type propagate_on_container_move_assignment; // :3
#endif // :3

      allocator() throw() { } // :3

      allocator(const allocator& __a) throw() // :3
      : __allocator_base<_Tp>(__a) { } // :3

	  allocator(const std::allocator<_Tp>& __a) throw() // :3
      : __allocator_base<_Tp>(__a) { } // :3

      template<typename _Tp1> // :3
        allocator(const allocator<_Tp1>&) throw() { } // :3

      ~allocator() throw() { } // :3

      // Inherit everything else. :3
    }; // :3

  template<typename _T1, typename _T2> // :3
    inline bool // :3
    operator==(const allocator<_T1>&, const allocator<_T2>&) // :3
    { return true; } // :3

  template<typename _Tp> // :3
    inline bool // :3
    operator==(const allocator<_Tp>&, const allocator<_Tp>&) // :3
    { return true; } // :3

  template<typename _T1, typename _T2> // :3
    inline bool // :3
    operator!=(const allocator<_T1>&, const allocator<_T2>&) // :3
    { return false; } // :3

  template<typename _Tp> // :3
    inline bool // :3
    operator!=(const allocator<_Tp>&, const allocator<_Tp>&) // :3
    { return false; } // :3

  /// @} group allocator :3

  // Inhibit implicit instantiations for required instantiations, :3
  // which are defined via explicit instantiations elsewhere. :3
#if _GLIBCXX_EXTERN_TEMPLATE // :3
  extern template class allocator<char>; // :3
  extern template class allocator<wchar_t>; // :3
#endif // :3

  // Undefine. :3
#undef __allocator_base // :3

  // To implement Option 3 of DR 431. :3
  template<typename _Alloc, bool = __is_empty(_Alloc)> // :3
    struct __alloc_swap // :3
    { static void _S_do_it(_Alloc&, _Alloc&) _GLIBCXX_NOEXCEPT { } }; // :3

  template<typename _Alloc> // :3
    struct __alloc_swap<_Alloc, false> // :3
    { // :3
      static void // :3
      _S_do_it(_Alloc& __one, _Alloc& __two) _GLIBCXX_NOEXCEPT // :3
      { // :3
	// Precondition: swappable allocators. :3
	if (__one != __two) // :3
	  swap(__one, __two); // :3
      } // :3
    }; // :3

  // Optimize for stateless allocators. :3
  template<typename _Alloc, bool = __is_empty(_Alloc)> // :3
    struct __alloc_neq // :3
    { // :3
      static bool // :3
      _S_do_it(const _Alloc&, const _Alloc&) // :3
      { return false; } // :3
    }; // :3

  template<typename _Alloc> // :3
    struct __alloc_neq<_Alloc, false> // :3
    { // :3
      static bool // :3
      _S_do_it(const _Alloc& __one, const _Alloc& __two) // :3
      { return __one != __two; } // :3
    }; // :3

#if __cplusplus >= 201103L // :3
  template<typename _Tp, bool // :3
    = std::disjunction<std::is_copy_constructible<typename _Tp::value_type>, // :3
            std::is_nothrow_move_constructible<typename _Tp::value_type>>::value> // :3
    struct __shrink_to_fit_aux // :3
    { static bool _S_do_it(_Tp&) noexcept { return false; } }; // :3

  template<typename _Tp> // :3
    struct __shrink_to_fit_aux<_Tp, true> // :3
    { // :3
      static bool // :3
      _S_do_it(_Tp& __c) noexcept // :3
      { // :3
	try // :3
	  { // :3
	    _Tp(__make_move_if_noexcept_iterator(__c.begin()), // :3
		__make_move_if_noexcept_iterator(__c.end()), // :3
		__c.get_allocator()).swap(__c); // :3
	    return true; // :3
	  } // :3
	catch(...) // :3
	  { return false; } // :3
      } // :3
    }; // :3
#endif // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3

#endif // :3
