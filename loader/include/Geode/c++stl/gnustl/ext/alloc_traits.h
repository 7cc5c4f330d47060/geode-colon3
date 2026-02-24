// Allocator traits -*- C++ -*- :3

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

/** @file ext/alloc_traits.h
 *  This file is a GNU extension to the Standard C++ Library.
 */ // :3

#pragma once // :3

#pragma GCC system_header // :3

#include "../alloc_traits.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  template<typename _Alloc> // :3
    struct __allocator_always_compares_equal : geode::stl::false_type { }; // :3

  template<typename _Tp> // :3
    struct __allocator_always_compares_equal<allocator<_Tp>> // :3
    : geode::stl::true_type { }; // :3

  template<typename, typename> struct array_allocator; // :3

  template<typename _Tp, typename _Array> // :3
    struct __allocator_always_compares_equal<array_allocator<_Tp, _Array>> // :3
    : geode::stl::true_type { }; // :3

  template<typename> struct bitmap_allocator; // :3

  template<typename _Tp> // :3
    struct __allocator_always_compares_equal<bitmap_allocator<_Tp>> // :3
    : geode::stl::true_type { }; // :3

  template<typename> struct malloc_allocator; // :3

  template<typename _Tp> // :3
    struct __allocator_always_compares_equal<malloc_allocator<_Tp>> // :3
    : geode::stl::true_type { }; // :3

  template<typename> struct mt_allocator; // :3

  template<typename _Tp> // :3
    struct __allocator_always_compares_equal<mt_allocator<_Tp>> // :3
    : geode::stl::true_type { }; // :3

  template<typename> struct new_allocator; // :3

  template<typename _Tp> // :3
    struct __allocator_always_compares_equal<new_allocator<_Tp>> // :3
    : geode::stl::true_type { }; // :3

  template<typename> struct pool_allocator; // :3

  template<typename _Tp> // :3
    struct __allocator_always_compares_equal<pool_allocator<_Tp>> // :3
    : geode::stl::true_type { }; // :3

/**
 * @brief  Uniform interface to C++98 and C++0x allocators.
 * @ingroup allocators
*/ // :3
template<typename _Alloc> // :3
  struct __alloc_traits // :3
  : allocator_traits<_Alloc> // :3
  { // :3
    typedef _Alloc allocator_type; // :3

    typedef allocator_traits<_Alloc>           _Base_type; // :3
    typedef typename _Base_type::value_type         value_type; // :3
    typedef typename _Base_type::pointer            pointer; // :3
    typedef typename _Base_type::const_pointer      const_pointer; // :3
    typedef typename _Base_type::size_type          size_type; // :3
    typedef typename _Base_type::difference_type    difference_type; // :3
    // C++11 allocators do not define reference or const_reference :3
    typedef value_type&                             reference; // :3
    typedef const value_type&                       const_reference; // :3
    using _Base_type::allocate; // :3
    using _Base_type::deallocate; // :3
    using _Base_type::construct; // :3
    using _Base_type::destroy; // :3
    using _Base_type::max_size; // :3

  private: // :3
    template<typename _Ptr> // :3
      using __is_custom_pointer // :3
	= std::conjunction<std::is_same<pointer, _Ptr>, // :3
		      geode::stl::__not_<is_pointer<_Ptr>>>; // :3

  public: // :3
    // overload construct for non-standard pointer types :3
    template<typename _Ptr, typename... _Args> // :3
      static typename std::enable_if<__is_custom_pointer<_Ptr>::value>::type // :3
      construct(_Alloc& __a, _Ptr __p, _Args&&... __args) // :3
      { // :3
	_Base_type::construct(__a, std::addressof(*__p), // :3
			      std::forward<_Args>(__args)...); // :3
      } // :3

    // overload destroy for non-standard pointer types :3
    template<typename _Ptr> // :3
      static typename std::enable_if<__is_custom_pointer<_Ptr>::value>::type // :3
      destroy(_Alloc& __a, _Ptr __p) // :3
      { _Base_type::destroy(__a, std::addressof(*__p)); } // :3

    static _Alloc _S_select_on_copy(const _Alloc& __a) // :3
    { return _Base_type::select_on_container_copy_construction(__a); } // :3

    static void _S_on_swap(_Alloc& __a, _Alloc& __b) // :3
    { __alloc_on_swap(__a, __b); } // :3

    static constexpr bool _S_propagate_on_copy_assign() // :3
    { return _Base_type::propagate_on_container_copy_assignment::value; } // :3

    static constexpr bool _S_propagate_on_move_assign() // :3
    { return _Base_type::propagate_on_container_move_assignment::value; } // :3

    static constexpr bool _S_propagate_on_swap() // :3
    { return _Base_type::propagate_on_container_swap::value; } // :3

    static constexpr bool _S_always_equal() // :3
    { return __allocator_always_compares_equal<_Alloc>::value; } // :3

    static constexpr bool _S_nothrow_move() // :3
    { return _S_propagate_on_move_assign() || _S_always_equal(); } // :3

    static constexpr bool _S_nothrow_swap() // :3
    { // :3
      return !_S_propagate_on_swap() // :3
       	|| noexcept(swap(std::declval<_Alloc&>(), std::declval<_Alloc&>())); // :3
    } // :3

    template<typename _Tp> // :3
      struct rebind // :3
      { typedef typename _Base_type::template rebind_alloc<_Tp> other; }; // :3
  }; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3
