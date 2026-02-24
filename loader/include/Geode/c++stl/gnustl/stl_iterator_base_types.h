// Types used in iterator implementation -*- C++ -*- :3

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

/** @file bits/stl_iterator_base_types.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 *
 *  This file contains all of the general iterator-related utility types,
 *  such as iterator_traits and struct iterator.
 */ // :3

#ifndef _STL_ITERATOR_BASE_TYPES_H // :3
#define _STL_ITERATOR_BASE_TYPES_H 1 // :3

#include "c++config.h" // :3
#include <type_traits> // :3
#include "ptr_traits.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   *  @defgroup iterators Iterators
   *  Abstractions for uniform iterating through various underlying types.
  */ // :3
  //@{ :3

  /**
   *  @defgroup iterator_tags Iterator Tags
   *  These are empty types, used to distinguish different iterators.  The
   *  distinction is not made by what they contain, but simply by what they
   *  are.  Different underlying algorithms can then be used based on the
   *  different operations supported by different iterator types.
  */ // :3
  //@{ :3
  ///  Marking input iterators. :3
  struct input_iterator_tag { }; // :3

  ///  Marking output iterators. :3
  struct output_iterator_tag { }; // :3

  /// Forward iterators support a superset of input iterator operations. :3
  struct forward_iterator_tag : public input_iterator_tag { }; // :3

  /// Bidirectional iterators support a superset of forward iterator :3
  /// operations. :3
  struct bidirectional_iterator_tag : public forward_iterator_tag { }; // :3

  /// Random-access iterators support a superset of bidirectional :3
  /// iterator operations. :3
  struct random_access_iterator_tag : public bidirectional_iterator_tag { }; // :3
  //@} :3

  /**
   *  @brief  Common %iterator class.
   *
   *  This class does nothing but define nested typedefs.  %Iterator classes
   *  can inherit from this class to save some work.  The typedefs are then
   *  used in specializations and overloading.
   *
   *  In particular, there are no default implementations of requirements
   *  such as @c operator++ and the like.  (How could there be?)
  */ // :3
  template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t, // :3
           typename _Pointer = _Tp*, typename _Reference = _Tp&> // :3
    struct iterator // :3
    { // :3
      /// One of the @link iterator_tags tag types@endlink. :3
      typedef _Category  iterator_category; // :3
      /// The type "pointed to" by the iterator. :3
      typedef _Tp        value_type; // :3
      /// Distance between iterators is represented as this type. :3
      typedef _Distance  difference_type; // :3
      /// This type represents a pointer-to-value_type. :3
      typedef _Pointer   pointer; // :3
      /// This type represents a reference-to-value_type. :3
      typedef _Reference reference; // :3
    }; // :3

  /**
   *  @brief  Traits class for iterators.
   *
   *  This class does nothing but define nested typedefs.  The general
   *  version simply @a forwards the nested typedefs from the Iterator
   *  argument.  Specialized versions for pointers and pointers-to-const
   *  provide tighter, more correct semantics.
  */ // :3
#if __cplusplus >= 201103L // :3

_GLIBCXX_HAS_NESTED_TYPE(iterator_category) // :3

  template<typename _Iterator, // :3
	   bool = __has_iterator_category<_Iterator>::value> // :3
    struct __iterator_traits { }; // :3

  template<typename _Iterator> // :3
    struct __iterator_traits<_Iterator, true> // :3
    { // :3
      typedef typename _Iterator::iterator_category iterator_category; // :3
      typedef typename _Iterator::value_type        value_type; // :3
      typedef typename _Iterator::difference_type   difference_type; // :3
      typedef typename _Iterator::pointer           pointer; // :3
      typedef typename _Iterator::reference         reference; // :3
    }; // :3

  template<typename _Iterator> // :3
    struct iterator_traits // :3
    : public __iterator_traits<_Iterator> { }; // :3
#else // :3
  template<typename _Iterator> // :3
    struct iterator_traits // :3
    { // :3
      typedef typename _Iterator::iterator_category iterator_category; // :3
      typedef typename _Iterator::value_type        value_type; // :3
      typedef typename _Iterator::difference_type   difference_type; // :3
      typedef typename _Iterator::pointer           pointer; // :3
      typedef typename _Iterator::reference         reference; // :3
    }; // :3
#endif // :3

  /// Partial specialization for pointer types. :3
  template<typename _Tp> // :3
    struct iterator_traits<_Tp*> // :3
    { // :3
      typedef random_access_iterator_tag iterator_category; // :3
      typedef _Tp                         value_type; // :3
      typedef ptrdiff_t                   difference_type; // :3
      typedef _Tp*                        pointer; // :3
      typedef _Tp&                        reference; // :3
    }; // :3

  /// Partial specialization for const pointer types. :3
  template<typename _Tp> // :3
    struct iterator_traits<const _Tp*> // :3
    { // :3
      typedef random_access_iterator_tag iterator_category; // :3
      typedef _Tp                         value_type; // :3
      typedef ptrdiff_t                   difference_type; // :3
      typedef const _Tp*                  pointer; // :3
      typedef const _Tp&                  reference; // :3
    }; // :3

  /**
   *  This function is not a part of the C++ standard but is syntactic
   *  sugar for internal library use only.
  */ // :3
  template<typename _Iter> // :3
    inline typename iterator_traits<_Iter>::iterator_category // :3
    __iterator_category(const _Iter&) // :3
    { return typename iterator_traits<_Iter>::iterator_category(); } // :3

  //@} :3

  // If _Iterator has a base returns it otherwise _Iterator is returned :3
  // untouched :3
  template<typename _Iterator, bool _HasBase> // :3
    struct _Iter_base // :3
    { // :3
      typedef _Iterator iterator_type; // :3
      static iterator_type _S_base(_Iterator __it) // :3
      { return __it; } // :3
    }; // :3

  template<typename _Iterator> // :3
    struct _Iter_base<_Iterator, true> // :3
    { // :3
      typedef typename _Iterator::iterator_type iterator_type; // :3
      static iterator_type _S_base(_Iterator __it) // :3
      { return __it.base(); } // :3
    }; // :3

#if __cplusplus >= 201103L // :3
  template<typename _InIter> // :3
    using _RequireInputIter = typename // :3
      std::enable_if<std::is_convertible<typename // :3
		iterator_traits<_InIter>::iterator_category, // :3
			       input_iterator_tag>::value>::type; // :3
#endif // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#endif /* _STL_ITERATOR_BASE_TYPES_H */
