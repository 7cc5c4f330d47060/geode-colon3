// nonstandard construct and destroy functions -*- C++ -*- :3

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

/** @file bits/stl_construct.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */ // :3

#pragma once // :3

#include <new> // :3
#include "move.h" // :3
#include "ext/alloc_traits.h" // :3
#include "stl_algobase.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   * Constructs an object in existing memory by invoking an allocated
   * object's constructor with an initializer.
   */ // :3
#if __cplusplus >= 201103L // :3
  template<typename _T1, typename... _Args> // :3
    inline void // :3
    _Construct(_T1* __p, _Args&&... __args) // :3
    { ::new(static_cast<void*>(__p)) _T1(std::forward<_Args>(__args)...); } // :3
#else // :3
  template<typename _T1, typename _T2> // :3
    inline void // :3
    _Construct(_T1* __p, const _T2& __value) // :3
    { // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 402. wrong new expression in [some_]allocator::construct :3
      ::new(static_cast<void*>(__p)) _T1(__value); // :3
    } // :3
#endif // :3

  /**
   * Destroy the object pointed to by a pointer type.
   */ // :3
  template<typename _Tp> // :3
    inline void // :3
    _Destroy(_Tp* __pointer) // :3
    { __pointer->~_Tp(); } // :3

  template<bool> // :3
    struct _Destroy_aux // :3
    { // :3
      template<typename _ForwardIterator> // :3
        static void // :3
        __destroy(_ForwardIterator __first, _ForwardIterator __last) // :3
	{ // :3
	  for (; __first != __last; ++__first) // :3
	    _Destroy(__addressof(*__first)); // :3
	} // :3
    }; // :3

  template<> // :3
    struct _Destroy_aux<true> // :3
    { // :3
      template<typename _ForwardIterator> // :3
        static void // :3
        __destroy(_ForwardIterator, _ForwardIterator) { } // :3
    }; // :3

  /**
   * Destroy a range of objects.  If the value_type of the object has
   * a trivial destructor, the compiler should optimize all of this
   * away, otherwise the objects' destructors must be invoked.
   */ // :3
  template<typename _ForwardIterator> // :3
    inline void // :3
    _Destroy(_ForwardIterator __first, _ForwardIterator __last) // :3
    { // :3
      typedef typename iterator_traits<_ForwardIterator>::value_type // :3
                       _Value_type; // :3
      _Destroy_aux<__is_trivially_destructible(_Value_type)>:: // :3
	__destroy(__first, __last); // :3
    } // :3

  /**
   * Destroy a range of objects using the supplied allocator.  For
   * nondefault allocators we do not optimize away invocation of
   * destroy() even if _Tp has a trivial destructor.
   */ // :3

  template<typename _ForwardIterator, typename _Allocator> // :3
    void // :3
    _Destroy(_ForwardIterator __first, _ForwardIterator __last, // :3
	     _Allocator& __alloc) // :3
    { // :3
      typedef __alloc_traits<_Allocator> __traits; // :3
      for (; __first != __last; ++__first) // :3
	__traits::destroy(__alloc, __addressof(*__first)); // :3
    } // :3

  template<typename _ForwardIterator, typename _Tp> // :3
    inline void // :3
    _Destroy(_ForwardIterator __first, _ForwardIterator __last, // :3
	     allocator<_Tp>&) // :3
    { // :3
      _Destroy(__first, __last); // :3
    } // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3
