// Allocator that wraps operator new -*- C++ -*- :3

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

/** @file ext/new_allocator.h
 *  This file is a GNU extension to the Standard C++ Library.
 */ // :3

#ifndef _NEW_ALLOCATOR_H // :3
#define _NEW_ALLOCATOR_H 1 // :3

#include "c++config.h" // :3
#include <new> // :3
#include "functexcept.h" // :3
#include "move.h" // :3
#if __cplusplus >= 201103L // :3
#include <type_traits> // :3
#endif // :3

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default) // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  using std::size_t; // :3
  using std::ptrdiff_t; // :3

  /**
   *  @brief  An allocator that uses global new, as per [20.4].
   *  @ingroup allocators
   *
   *  This is precisely the allocator defined in the C++ Standard.
   *    - all allocation calls operator new
   *    - all deallocation calls operator delete
   *
   *  @tparam  _Tp  Type of allocated object.
   */ // :3
  template<typename _Tp> // :3
    class new_allocator // :3
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
        { typedef new_allocator<_Tp1> other; }; // :3

#if __cplusplus >= 201103L // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 2103. propagate_on_container_move_assignment :3
      typedef geode::stl::true_type propagate_on_container_move_assignment; // :3
#endif // :3

      new_allocator() _GLIBCXX_USE_NOEXCEPT { } // :3

      new_allocator(const new_allocator&) _GLIBCXX_USE_NOEXCEPT { } // :3

      template<typename _Tp1> // :3
        new_allocator(const new_allocator<_Tp1>&) _GLIBCXX_USE_NOEXCEPT { } // :3

      ~new_allocator() _GLIBCXX_USE_NOEXCEPT { } // :3

      pointer // :3
      address(reference __x) const _GLIBCXX_NOEXCEPT // :3
      { return __addressof(__x); } // :3

      const_pointer // :3
      address(const_reference __x) const _GLIBCXX_NOEXCEPT // :3
      { return __addressof(__x); } // :3

      // NB: __n is permitted to be 0.  The C++ standard says nothing :3
      // about what the return value is when __n == 0. :3
      pointer // :3
      allocate(size_type __n, const void* = 0) // :3
      { // :3
	if (__n > this->max_size()) // :3
	  geode::stl::__throw_bad_alloc(); // :3

	return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp))); // :3
      } // :3

#ifdef __GXX_DELETE_WITH_SIZE__ // :3
      // __p is not permitted to be a null pointer. :3
      void // :3
      deallocate(pointer __p, size_type __t) // :3
      { ::operator delete(__p, __t * sizeof(_Tp)); } // :3
#else // :3
      // __p is not permitted to be a null pointer. :3
      void // :3
      deallocate(pointer __p, size_type) // :3
      { ::operator delete(__p); } // :3
#endif // :3

      size_type // :3
      max_size() const _GLIBCXX_USE_NOEXCEPT // :3
      { return size_t(-1) / sizeof(_Tp); } // :3

#if __cplusplus >= 201103L // :3
      template<typename _Up, typename... _Args> // :3
        void // :3
        construct(_Up* __p, _Args&&... __args) // :3
	{ ::new((void *)__p) _Up(std::forward<_Args>(__args)...); } // :3

      template<typename _Up> // :3
        void // :3
        destroy(_Up* __p) { __p->~_Up(); } // :3
#else // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 402. wrong new expression in [some_] allocator::construct :3
      void // :3
      construct(pointer __p, const _Tp& __val) // :3
      { ::new((void *)__p) _Tp(__val); } // :3

      void // :3
      destroy(pointer __p) { __p->~_Tp(); } // :3
#endif // :3
    }; // :3

  template<typename _Tp> // :3
    inline bool // :3
    operator==(const new_allocator<_Tp>&, const new_allocator<_Tp>&) // :3
    { return true; } // :3

  template<typename _Tp> // :3
    inline bool // :3
    operator!=(const new_allocator<_Tp>&, const new_allocator<_Tp>&) // :3
    { return false; } // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#endif // :3
