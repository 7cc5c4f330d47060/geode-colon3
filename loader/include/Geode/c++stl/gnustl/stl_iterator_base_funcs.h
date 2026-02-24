// Functions used by iterators -*- C++ -*- :3

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

/** @file bits/stl_iterator_base_funcs.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 *
 *  This file contains all of the general iterator-related utility
 *  functions, such as distance() and advance().
 */ // :3

#ifndef _STL_ITERATOR_BASE_FUNCS_H // :3
#define _STL_ITERATOR_BASE_FUNCS_H 1 // :3

#pragma GCC system_header // :3

#include "concept_check.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  template<typename _InputIterator> // :3
    inline typename iterator_traits<_InputIterator>::difference_type // :3
    __distance(_InputIterator __first, _InputIterator __last, // :3
               input_iterator_tag) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>) // :3

      typename iterator_traits<_InputIterator>::difference_type __n = 0; // :3
      while (__first != __last) // :3
	{ // :3
	  ++__first; // :3
	  ++__n; // :3
	} // :3
      return __n; // :3
    } // :3

  template<typename _RandomAccessIterator> // :3
    inline typename iterator_traits<_RandomAccessIterator>::difference_type // :3
    __distance(_RandomAccessIterator __first, _RandomAccessIterator __last, // :3
               random_access_iterator_tag) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_RandomAccessIteratorConcept< // :3
				  _RandomAccessIterator>) // :3
      return __last - __first; // :3
    } // :3

  /**
   *  @brief A generalization of pointer arithmetic.
   *  @param  __first  An input iterator.
   *  @param  __last  An input iterator.
   *  @return  The distance between them.
   *
   *  Returns @c n such that __first + n == __last.  This requires
   *  that @p __last must be reachable from @p __first.  Note that @c
   *  n may be negative.
   *
   *  For random access iterators, this uses their @c + and @c - operations
   *  and are constant time.  For other %iterator classes they are linear time.
  */ // :3
  template<typename _InputIterator> // :3
    inline typename iterator_traits<_InputIterator>::difference_type // :3
    distance(_InputIterator __first, _InputIterator __last) // :3
    { // :3
      // concept requirements -- taken care of in __distance :3
      return __distance(__first, __last, // :3
			     __iterator_category(__first)); // :3
    } // :3

  template<typename _InputIterator, typename _Distance> // :3
    inline void // :3
    __advance(_InputIterator& __i, _Distance __n, input_iterator_tag) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>) // :3
      _GLIBCXX_DEBUG_ASSERT(__n >= 0); // :3
      while (__n--) // :3
	++__i; // :3
    } // :3

  template<typename _BidirectionalIterator, typename _Distance> // :3
    inline void // :3
    __advance(_BidirectionalIterator& __i, _Distance __n, // :3
	      bidirectional_iterator_tag) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_BidirectionalIteratorConcept< // :3
				  _BidirectionalIterator>) // :3
      if (__n > 0) // :3
        while (__n--) // :3
	  ++__i; // :3
      else // :3
        while (__n++) // :3
	  --__i; // :3
    } // :3

  template<typename _RandomAccessIterator, typename _Distance> // :3
    inline void // :3
    __advance(_RandomAccessIterator& __i, _Distance __n, // :3
              random_access_iterator_tag) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_RandomAccessIteratorConcept< // :3
				  _RandomAccessIterator>) // :3
      __i += __n; // :3
    } // :3

  /**
   *  @brief A generalization of pointer arithmetic.
   *  @param  __i  An input iterator.
   *  @param  __n  The @a delta by which to change @p __i.
   *  @return  Nothing.
   *
   *  This increments @p i by @p n.  For bidirectional and random access
   *  iterators, @p __n may be negative, in which case @p __i is decremented.
   *
   *  For random access iterators, this uses their @c + and @c - operations
   *  and are constant time.  For other %iterator classes they are linear time.
  */ // :3
  template<typename _InputIterator, typename _Distance> // :3
    inline void // :3
    advance(_InputIterator& __i, _Distance __n) // :3
    { // :3
      // concept requirements -- taken care of in __advance :3
      typename iterator_traits<_InputIterator>::difference_type __d = __n; // :3
      __advance(__i, __d, __iterator_category(__i)); // :3
    } // :3

#if __cplusplus >= 201103L // :3

  template<typename _ForwardIterator> // :3
    inline _ForwardIterator // :3
    next(_ForwardIterator __x, typename // :3
	 iterator_traits<_ForwardIterator>::difference_type __n = 1) // :3
    { // :3
      advance(__x, __n); // :3
      return __x; // :3
    } // :3

  template<typename _BidirectionalIterator> // :3
    inline _BidirectionalIterator // :3
    prev(_BidirectionalIterator __x, typename // :3
	 iterator_traits<_BidirectionalIterator>::difference_type __n = 1) // :3
    { // :3
      advance(__x, -__n); // :3
      return __x; // :3
    } // :3

#endif // C++11 :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#endif /* _STL_ITERATOR_BASE_FUNCS_H */