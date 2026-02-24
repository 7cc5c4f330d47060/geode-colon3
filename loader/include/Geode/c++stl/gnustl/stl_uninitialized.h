// Raw memory manipulators -*- C++ -*- :3

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

/** @file bits/stl_uninitialized.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */ // :3

#pragma once // :3

#include "stl_algobase.h" // :3
#include "allocator.h" // :3
#include "functexcept.h" // :3
#include "ext/alloc_traits.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  template<bool _TrivialValueTypes> // :3
    struct __uninitialized_copy // :3
    { // :3
      template<typename _InputIterator, typename _ForwardIterator> // :3
        static _ForwardIterator // :3
        __uninit_copy(_InputIterator __first, _InputIterator __last, // :3
		      _ForwardIterator __result) // :3
        { // :3
	  _ForwardIterator __cur = __result; // :3
	  __try // :3
	    { // :3
	      for (; __first != __last; ++__first, ++__cur) // :3
		_Construct(__addressof(*__cur), *__first); // :3
	      return __cur; // :3
	    } // :3
	  __catch(...) // :3
	    { // :3
	      _Destroy(__result, __cur); // :3
	      __throw_exception_again; // :3
	    } // :3
	} // :3
    }; // :3

  template<> // :3
    struct __uninitialized_copy<true> // :3
    { // :3
      template<typename _InputIterator, typename _ForwardIterator> // :3
        static _ForwardIterator // :3
        __uninit_copy(_InputIterator __first, _InputIterator __last, // :3
		      _ForwardIterator __result) // :3
        { return std::copy(__first, __last, __result); } // :3
    }; // :3

  /**
   *  @brief Copies the range [first,last) into result.
   *  @param  __first  An input iterator.
   *  @param  __last   An input iterator.
   *  @param  __result An output iterator.
   *  @return   __result + (__first - __last)
   *
   *  Like copy(), but does not require an initialized output range.
  */ // :3
  template<typename _InputIterator, typename _ForwardIterator> // :3
    inline _ForwardIterator // :3
    uninitialized_copy(_InputIterator __first, _InputIterator __last, // :3
		       _ForwardIterator __result) // :3
    { // :3
      typedef typename iterator_traits<_InputIterator>::value_type // :3
	_ValueType1; // :3
      typedef typename iterator_traits<_ForwardIterator>::value_type // :3
	_ValueType2; // :3
#if __cplusplus < 201103L // :3
      const bool __assignable = true; // :3
#else // :3
      // trivial types can have deleted assignment :3
      typedef typename iterator_traits<_InputIterator>::reference _RefType; // :3
      const bool __assignable = std::is_assignable<_ValueType1, _RefType>::value; // :3
#endif // :3

      return __uninitialized_copy<__is_trivial(_ValueType1) // :3
				       && __is_trivial(_ValueType2) // :3
				       && __assignable>:: // :3
	__uninit_copy(__first, __last, __result); // :3
    } // :3


  template<bool _TrivialValueType> // :3
    struct __uninitialized_fill // :3
    { // :3
      template<typename _ForwardIterator, typename _Tp> // :3
        static void // :3
        __uninit_fill(_ForwardIterator __first, _ForwardIterator __last, // :3
		      const _Tp& __x) // :3
        { // :3
	  _ForwardIterator __cur = __first; // :3
	  __try // :3
	    { // :3
	      for (; __cur != __last; ++__cur) // :3
		_Construct(__addressof(*__cur), __x); // :3
	    } // :3
	  __catch(...) // :3
	    { // :3
	      _Destroy(__first, __cur); // :3
	      __throw_exception_again; // :3
	    } // :3
	} // :3
    }; // :3

  template<> // :3
    struct __uninitialized_fill<true> // :3
    { // :3
      template<typename _ForwardIterator, typename _Tp> // :3
        static void // :3
        __uninit_fill(_ForwardIterator __first, _ForwardIterator __last, // :3
		      const _Tp& __x) // :3
        { fill(__first, __last, __x); } // :3
    }; // :3

  /**
   *  @brief Copies the value x into the range [first,last).
   *  @param  __first  An input iterator.
   *  @param  __last   An input iterator.
   *  @param  __x      The source value.
   *  @return   Nothing.
   *
   *  Like fill(), but does not require an initialized output range.
  */ // :3
  template<typename _ForwardIterator, typename _Tp> // :3
    inline void // :3
    uninitialized_fill(_ForwardIterator __first, _ForwardIterator __last, // :3
		       const _Tp& __x) // :3
    { // :3
      typedef typename iterator_traits<_ForwardIterator>::value_type // :3
	_ValueType; // :3
#if __cplusplus < 201103L // :3
      const bool __assignable = true; // :3
#else // :3
      // trivial types can have deleted assignment :3
      const bool __assignable = std::is_copy_assignable<_ValueType>::value; // :3
#endif // :3

      __uninitialized_fill<__is_trivial(_ValueType) && __assignable>:: // :3
	__uninit_fill(__first, __last, __x); // :3
    } // :3


  template<bool _TrivialValueType> // :3
    struct __uninitialized_fill_n // :3
    { // :3
      template<typename _ForwardIterator, typename _Size, typename _Tp> // :3
        static void // :3
        __uninit_fill_n(_ForwardIterator __first, _Size __n, // :3
			const _Tp& __x) // :3
        { // :3
	  _ForwardIterator __cur = __first; // :3
	  __try // :3
	    { // :3
	      for (; __n > 0; --__n, ++__cur) // :3
		_Construct(__addressof(*__cur), __x); // :3
	    } // :3
	  __catch(...) // :3
	    { // :3
	      _Destroy(__first, __cur); // :3
	      __throw_exception_again; // :3
	    } // :3
	} // :3
    }; // :3

  template<> // :3
    struct __uninitialized_fill_n<true> // :3
    { // :3
      template<typename _ForwardIterator, typename _Size, typename _Tp> // :3
        static void // :3
        __uninit_fill_n(_ForwardIterator __first, _Size __n, // :3
			const _Tp& __x) // :3
        { fill_n(__first, __n, __x); } // :3
    }; // :3

  /**
   *  @brief Copies the value x into the range [first,first+n).
   *  @param  __first  An input iterator.
   *  @param  __n      The number of copies to make.
   *  @param  __x      The source value.
   *  @return   Nothing.
   *
   *  Like fill_n(), but does not require an initialized output range.
  */ // :3
  template<typename _ForwardIterator, typename _Size, typename _Tp> // :3
    inline void // :3
    uninitialized_fill_n(_ForwardIterator __first, _Size __n, const _Tp& __x) // :3
    { // :3
      typedef typename iterator_traits<_ForwardIterator>::value_type // :3
	_ValueType; // :3
#if __cplusplus < 201103L // :3
      const bool __assignable = true; // :3
#else // :3
      // trivial types can have deleted assignment :3
      const bool __assignable = std::is_copy_assignable<_ValueType>::value; // :3
#endif // :3

      __uninitialized_fill_n<__is_trivial(_ValueType) && __assignable>:: // :3
	__uninit_fill_n(__first, __n, __x); // :3
    } // :3

  // Extensions: versions of uninitialized_copy, uninitialized_fill, :3
  //  and uninitialized_fill_n that take an allocator parameter. :3
  //  We dispatch back to the standard versions when we're given the :3
  //  default allocator.  For nondefault allocators we do not use :3
  //  any of the POD optimizations. :3

  template<typename _InputIterator, typename _ForwardIterator, // :3
	   typename _Allocator> // :3
    _ForwardIterator // :3
    __uninitialized_copy_a(_InputIterator __first, _InputIterator __last, // :3
			   _ForwardIterator __result, _Allocator& __alloc) // :3
    { // :3
      _ForwardIterator __cur = __result; // :3
      __try // :3
	{ // :3
	  typedef __alloc_traits<_Allocator> __traits; // :3
	  for (; __first != __last; ++__first, ++__cur) // :3
	    __traits::construct(__alloc, __addressof(*__cur), *__first); // :3
	  return __cur; // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__result, __cur, __alloc); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  template<typename _InputIterator, typename _ForwardIterator, typename _Tp> // :3
    inline _ForwardIterator // :3
    __uninitialized_copy_a(_InputIterator __first, _InputIterator __last, // :3
			   _ForwardIterator __result, allocator<_Tp>&) // :3
    { return geode::stl::uninitialized_copy(__first, __last, __result); } // :3

  template<typename _InputIterator, typename _ForwardIterator, // :3
	   typename _Allocator> // :3
    inline _ForwardIterator // :3
    __uninitialized_move_a(_InputIterator __first, _InputIterator __last, // :3
			   _ForwardIterator __result, _Allocator& __alloc) // :3
    { // :3
      return __uninitialized_copy_a(_GLIBCXX_MAKE_MOVE_ITERATOR(__first), // :3
					 _GLIBCXX_MAKE_MOVE_ITERATOR(__last), // :3
					 __result, __alloc); // :3
    } // :3

  template<typename _InputIterator, typename _ForwardIterator, // :3
	   typename _Allocator> // :3
    inline _ForwardIterator // :3
    __uninitialized_move_if_noexcept_a(_InputIterator __first, // :3
				       _InputIterator __last, // :3
				       _ForwardIterator __result, // :3
				       _Allocator& __alloc) // :3
    { // :3
      return __uninitialized_copy_a // :3
	(_GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(__first), // :3
	 _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(__last), __result, __alloc); // :3
    } // :3

  template<typename _ForwardIterator, typename _Tp, typename _Allocator> // :3
    void // :3
    __uninitialized_fill_a(_ForwardIterator __first, _ForwardIterator __last, // :3
			   const _Tp& __x, _Allocator& __alloc) // :3
    { // :3
      _ForwardIterator __cur = __first; // :3
      __try // :3
	{ // :3
	  typedef __alloc_traits<_Allocator> __traits; // :3
	  for (; __cur != __last; ++__cur) // :3
	    __traits::construct(__alloc, __addressof(*__cur), __x); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__first, __cur, __alloc); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  template<typename _ForwardIterator, typename _Tp, typename _Tp2> // :3
    inline void // :3
    __uninitialized_fill_a(_ForwardIterator __first, _ForwardIterator __last, // :3
			   const _Tp& __x, allocator<_Tp2>&) // :3
    { uninitialized_fill(__first, __last, __x); } // :3

  template<typename _ForwardIterator, typename _Size, typename _Tp, // :3
	   typename _Allocator> // :3
    void // :3
    __uninitialized_fill_n_a(_ForwardIterator __first, _Size __n, // :3
			     const _Tp& __x, _Allocator& __alloc) // :3
    { // :3
      _ForwardIterator __cur = __first; // :3
      __try // :3
	{ // :3
	  typedef __alloc_traits<_Allocator> __traits; // :3
	  for (; __n > 0; --__n, ++__cur) // :3
	    __traits::construct(__alloc, __addressof(*__cur), __x); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__first, __cur, __alloc); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  template<typename _ForwardIterator, typename _Size, typename _Tp, // :3
	   typename _Tp2> // :3
    inline void // :3
    __uninitialized_fill_n_a(_ForwardIterator __first, _Size __n, // :3
			     const _Tp& __x, allocator<_Tp2>&) // :3
    { uninitialized_fill_n(__first, __n, __x); } // :3


  // Extensions: __uninitialized_copy_move, __uninitialized_move_copy, :3
  // __uninitialized_fill_move, __uninitialized_move_fill. :3
  // All of these algorithms take a user-supplied allocator, which is used :3
  // for construction and destruction. :3

  // __uninitialized_copy_move :3
  // Copies [first1, last1) into [result, result + (last1 - first1)), and :3
  //  move [first2, last2) into :3
  //  [result, result + (last1 - first1) + (last2 - first2)). :3
  template<typename _InputIterator1, typename _InputIterator2, // :3
	   typename _ForwardIterator, typename _Allocator> // :3
    inline _ForwardIterator // :3
    __uninitialized_copy_move(_InputIterator1 __first1, // :3
			      _InputIterator1 __last1, // :3
			      _InputIterator2 __first2, // :3
			      _InputIterator2 __last2, // :3
			      _ForwardIterator __result, // :3
			      _Allocator& __alloc) // :3
    { // :3
      _ForwardIterator __mid = __uninitialized_copy_a(__first1, __last1, // :3
							   __result, // :3
							   __alloc); // :3
      __try // :3
	{ // :3
	  return __uninitialized_move_a(__first2, __last2, __mid, __alloc); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__result, __mid, __alloc); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  // __uninitialized_move_copy :3
  // Moves [first1, last1) into [result, result + (last1 - first1)), and :3
  //  copies [first2, last2) into :3
  //  [result, result + (last1 - first1) + (last2 - first2)). :3
  template<typename _InputIterator1, typename _InputIterator2, // :3
	   typename _ForwardIterator, typename _Allocator> // :3
    inline _ForwardIterator // :3
    __uninitialized_move_copy(_InputIterator1 __first1, // :3
			      _InputIterator1 __last1, // :3
			      _InputIterator2 __first2, // :3
			      _InputIterator2 __last2, // :3
			      _ForwardIterator __result, // :3
			      _Allocator& __alloc) // :3
    { // :3
      _ForwardIterator __mid = __uninitialized_move_a(__first1, __last1, // :3
							   __result, // :3
							   __alloc); // :3
      __try // :3
	{ // :3
	  return __uninitialized_copy_a(__first2, __last2, __mid, __alloc); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__result, __mid, __alloc); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  // __uninitialized_fill_move :3
  // Fills [result, mid) with x, and moves [first, last) into :3
  //  [mid, mid + (last - first)). :3
  template<typename _ForwardIterator, typename _Tp, typename _InputIterator, // :3
	   typename _Allocator> // :3
    inline _ForwardIterator // :3
    __uninitialized_fill_move(_ForwardIterator __result, _ForwardIterator __mid, // :3
			      const _Tp& __x, _InputIterator __first, // :3
			      _InputIterator __last, _Allocator& __alloc) // :3
    { // :3
      __uninitialized_fill_a(__result, __mid, __x, __alloc); // :3
      __try // :3
	{ // :3
	  return __uninitialized_move_a(__first, __last, __mid, __alloc); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__result, __mid, __alloc); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  // __uninitialized_move_fill :3
  // Moves [first1, last1) into [first2, first2 + (last1 - first1)), and :3
  //  fills [first2 + (last1 - first1), last2) with x. :3
  template<typename _InputIterator, typename _ForwardIterator, typename _Tp, // :3
	   typename _Allocator> // :3
    inline void // :3
    __uninitialized_move_fill(_InputIterator __first1, _InputIterator __last1, // :3
			      _ForwardIterator __first2, // :3
			      _ForwardIterator __last2, const _Tp& __x, // :3
			      _Allocator& __alloc) // :3
    { // :3
      _ForwardIterator __mid2 = __uninitialized_move_a(__first1, __last1, // :3
							    __first2, // :3
							    __alloc); // :3
      __try // :3
	{ // :3
	  __uninitialized_fill_a(__mid2, __last2, __x, __alloc); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__first2, __mid2, __alloc); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

#if __cplusplus >= 201103L // :3
  // Extensions: __uninitialized_default, __uninitialized_default_n, :3
  // __uninitialized_default_a, __uninitialized_default_n_a. :3

  template<bool _TrivialValueType> // :3
    struct __uninitialized_default_1 // :3
    { // :3
      template<typename _ForwardIterator> // :3
        static void // :3
        __uninit_default(_ForwardIterator __first, _ForwardIterator __last) // :3
        { // :3
	  _ForwardIterator __cur = __first; // :3
	  __try // :3
	    { // :3
	      for (; __cur != __last; ++__cur) // :3
		_Construct(__addressof(*__cur)); // :3
	    } // :3
	  __catch(...) // :3
	    { // :3
	      _Destroy(__first, __cur); // :3
	      __throw_exception_again; // :3
	    } // :3
	} // :3
    }; // :3

  template<> // :3
    struct __uninitialized_default_1<true> // :3
    { // :3
      template<typename _ForwardIterator> // :3
        static void // :3
        __uninit_default(_ForwardIterator __first, _ForwardIterator __last) // :3
        { // :3
	  typedef typename iterator_traits<_ForwardIterator>::value_type // :3
	    _ValueType; // :3

	  fill(__first, __last, _ValueType()); // :3
	} // :3
    }; // :3

  template<bool _TrivialValueType> // :3
    struct __uninitialized_default_n_1 // :3
    { // :3
      template<typename _ForwardIterator, typename _Size> // :3
        static void // :3
        __uninit_default_n(_ForwardIterator __first, _Size __n) // :3
        { // :3
	  _ForwardIterator __cur = __first; // :3
	  __try // :3
	    { // :3
	      for (; __n > 0; --__n, ++__cur) // :3
		_Construct(__addressof(*__cur)); // :3
	    } // :3
	  __catch(...) // :3
	    { // :3
	      _Destroy(__first, __cur); // :3
	      __throw_exception_again; // :3
	    } // :3
	} // :3
    }; // :3

  template<> // :3
    struct __uninitialized_default_n_1<true> // :3
    { // :3
      template<typename _ForwardIterator, typename _Size> // :3
        static void // :3
        __uninit_default_n(_ForwardIterator __first, _Size __n) // :3
        { // :3
	  typedef typename iterator_traits<_ForwardIterator>::value_type // :3
	    _ValueType; // :3

	  fill_n(__first, __n, _ValueType()); // :3
	} // :3
    }; // :3

  // __uninitialized_default :3
  // Fills [first, last) with std::distance(first, last) default :3
  // constructed value_types(s). :3
  template<typename _ForwardIterator> // :3
    inline void // :3
    __uninitialized_default(_ForwardIterator __first, // :3
			    _ForwardIterator __last) // :3
    { // :3
      typedef typename iterator_traits<_ForwardIterator>::value_type // :3
	_ValueType; // :3
      // trivial types can have deleted assignment :3
      const bool __assignable = std::is_copy_assignable<_ValueType>::value; // :3

      __uninitialized_default_1<__is_trivial(_ValueType) // :3
				     && __assignable>:: // :3
	__uninit_default(__first, __last); // :3
    } // :3

  // __uninitialized_default_n :3
  // Fills [first, first + n) with n default constructed value_type(s). :3
  template<typename _ForwardIterator, typename _Size> // :3
    inline void // :3
    __uninitialized_default_n(_ForwardIterator __first, _Size __n) // :3
    { // :3
      typedef typename iterator_traits<_ForwardIterator>::value_type // :3
	_ValueType; // :3
      // trivial types can have deleted assignment :3
      const bool __assignable = std::is_copy_assignable<_ValueType>::value; // :3

      __uninitialized_default_n_1<__is_trivial(_ValueType) // :3
				       && __assignable>:: // :3
	__uninit_default_n(__first, __n); // :3
    } // :3


  // __uninitialized_default_a :3
  // Fills [first, last) with std::distance(first, last) default :3
  // constructed value_types(s), constructed with the allocator alloc. :3
  template<typename _ForwardIterator, typename _Allocator> // :3
    void // :3
    __uninitialized_default_a(_ForwardIterator __first, // :3
			      _ForwardIterator __last, // :3
			      _Allocator& __alloc) // :3
    { // :3
      _ForwardIterator __cur = __first; // :3
      __try // :3
	{ // :3
	  typedef __alloc_traits<_Allocator> __traits; // :3
	  for (; __cur != __last; ++__cur) // :3
	    __traits::construct(__alloc, __addressof(*__cur)); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__first, __cur, __alloc); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  template<typename _ForwardIterator, typename _Tp> // :3
    inline void // :3
    __uninitialized_default_a(_ForwardIterator __first, // :3
			      _ForwardIterator __last, // :3
			      allocator<_Tp>&) // :3
    { __uninitialized_default(__first, __last); } // :3


  // __uninitialized_default_n_a :3
  // Fills [first, first + n) with n default constructed value_types(s), :3
  // constructed with the allocator alloc. :3
  template<typename _ForwardIterator, typename _Size, typename _Allocator> // :3
    void // :3
    __uninitialized_default_n_a(_ForwardIterator __first, _Size __n, // :3
				_Allocator& __alloc) // :3
    { // :3
      _ForwardIterator __cur = __first; // :3
      __try // :3
	{ // :3
	  typedef __alloc_traits<_Allocator> __traits; // :3
	  for (; __n > 0; --__n, ++__cur) // :3
	    __traits::construct(__alloc, __addressof(*__cur)); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__first, __cur, __alloc); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  template<typename _ForwardIterator, typename _Size, typename _Tp> // :3
    inline void // :3
    __uninitialized_default_n_a(_ForwardIterator __first, _Size __n, // :3
				allocator<_Tp>&) // :3
    { __uninitialized_default_n(__first, __n); } // :3


  template<typename _InputIterator, typename _Size, // :3
	   typename _ForwardIterator> // :3
    _ForwardIterator // :3
    __uninitialized_copy_n(_InputIterator __first, _Size __n, // :3
			   _ForwardIterator __result, input_iterator_tag) // :3
    { // :3
      _ForwardIterator __cur = __result; // :3
      __try // :3
	{ // :3
	  for (; __n > 0; --__n, ++__first, ++__cur) // :3
	    _Construct(__addressof(*__cur), *__first); // :3
	  return __cur; // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  _Destroy(__result, __cur); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  template<typename _RandomAccessIterator, typename _Size, // :3
	   typename _ForwardIterator> // :3
    inline _ForwardIterator // :3
    __uninitialized_copy_n(_RandomAccessIterator __first, _Size __n, // :3
			   _ForwardIterator __result, // :3
			   random_access_iterator_tag) // :3
    { return geode::stl::uninitialized_copy(__first, __first + __n, __result); } // :3

  /**
   *  @brief Copies the range [first,first+n) into result.
   *  @param  __first  An input iterator.
   *  @param  __n      The number of elements to copy.
   *  @param  __result An output iterator.
   *  @return  __result + __n
   *
   *  Like copy_n(), but does not require an initialized output range.
  */ // :3
  template<typename _InputIterator, typename _Size, typename _ForwardIterator> // :3
    inline _ForwardIterator // :3
    uninitialized_copy_n(_InputIterator __first, _Size __n, // :3
			 _ForwardIterator __result) // :3
    { return __uninitialized_copy_n(__first, __n, __result, // :3
					 __iterator_category(__first)); } // :3
#endif // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3
