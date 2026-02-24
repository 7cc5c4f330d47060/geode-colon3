// Core algorithmic facilities -*- C++ -*- :3

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

/** @file bits/stl_algobase.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{algorithm}
 */ // :3

#pragma once // :3

#include "c++config.h" // :3
#include "cpp_type_traits.h" // :3
#include "ext/numeric_traits.h" // :3
#include "stl_pair.h" // :3
#include "stl_iterator_base_types.h" // :3
#include "stl_iterator_base_funcs.h" // :3
#include "stl_iterator.h" // :3
#include "concept_check.h" // :3
#include "predefined_ops.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

#if __cplusplus < 201103L // :3
  // See http://gcc.gnu.org/ml/libstdc++/2004-08/msg00167.html: in a
  // nutshell, we are partially implementing the resolution of DR 187, :3
  // when it's safe, i.e., the value_types are equal. :3
  template<bool _BoolType> // :3
    struct __iter_swap // :3
    { // :3
      template<typename _ForwardIterator1, typename _ForwardIterator2> // :3
        static void // :3
        iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b) // :3
        { // :3
          typedef typename iterator_traits<_ForwardIterator1>::value_type // :3
            _ValueType1; // :3
          _ValueType1 __tmp = _GLIBCXX_MOVE(*__a); // :3
          *__a = _GLIBCXX_MOVE(*__b); // :3
          *__b = _GLIBCXX_MOVE(__tmp); // :3
	} // :3
    }; // :3

  template<> // :3
    struct __iter_swap<true> // :3
    { // :3
      template<typename _ForwardIterator1, typename _ForwardIterator2> // :3
        static void // :3
        iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b) // :3
        { // :3
          swap(*__a, *__b); // :3
        } // :3
    }; // :3
#endif // :3

  /**
   *  @brief Swaps the contents of two iterators.
   *  @ingroup mutating_algorithms
   *  @param  __a  An iterator.
   *  @param  __b  Another iterator.
   *  @return   Nothing.
   *
   *  This function swaps the values pointed to by two iterators, not the
   *  iterators themselves.
  */ // :3
  template<typename _ForwardIterator1, typename _ForwardIterator2> // :3
    inline void // :3
    iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_Mutable_ForwardIteratorConcept< // :3
				  _ForwardIterator1>) // :3
      __glibcxx_function_requires(_Mutable_ForwardIteratorConcept< // :3
				  _ForwardIterator2>) // :3

#if __cplusplus < 201103L // :3
      typedef typename iterator_traits<_ForwardIterator1>::value_type // :3
	_ValueType1; // :3
      typedef typename iterator_traits<_ForwardIterator2>::value_type // :3
	_ValueType2; // :3

      __glibcxx_function_requires(_ConvertibleConcept<_ValueType1, // :3
				  _ValueType2>) // :3
      __glibcxx_function_requires(_ConvertibleConcept<_ValueType2, // :3
				  _ValueType1>) // :3

      typedef typename iterator_traits<_ForwardIterator1>::reference // :3
	_ReferenceType1; // :3
      typedef typename iterator_traits<_ForwardIterator2>::reference // :3
	_ReferenceType2; // :3
      std::__iter_swap<std::is_same<_ValueType1, _ValueType2>::value // :3
	&& std::is_same<_ValueType1&, _ReferenceType1>::value // :3
	&& std::is_same<_ValueType2&, _ReferenceType2>::value>:: // :3
	iter_swap(__a, __b); // :3
#else // :3
      swap(*__a, *__b); // :3
#endif // :3
    } // :3

  /**
   *  @brief Swap the elements of two sequences.
   *  @ingroup mutating_algorithms
   *  @param  __first1  A forward iterator.
   *  @param  __last1   A forward iterator.
   *  @param  __first2  A forward iterator.
   *  @return   An iterator equal to @p first2+(last1-first1).
   *
   *  Swaps each element in the range @p [first1,last1) with the
   *  corresponding element in the range @p [first2,(last1-first1)).
   *  The ranges must not overlap.
  */ // :3
  template<typename _ForwardIterator1, typename _ForwardIterator2> // :3
    _ForwardIterator2 // :3
    swap_ranges(_ForwardIterator1 __first1, _ForwardIterator1 __last1, // :3
		_ForwardIterator2 __first2) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_Mutable_ForwardIteratorConcept< // :3
				  _ForwardIterator1>) // :3
      __glibcxx_function_requires(_Mutable_ForwardIteratorConcept< // :3
				  _ForwardIterator2>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3

      for (; __first1 != __last1; ++__first1, ++__first2) // :3
	std::iter_swap(__first1, __first2); // :3
      return __first2; // :3
    } // :3

  /**
   *  @brief This does what you think it does.
   *  @ingroup sorting_algorithms
   *  @param  __a  A thing of arbitrary type.
   *  @param  __b  Another thing of arbitrary type.
   *  @return   The lesser of the parameters.
   *
   *  This is the simple classic generic implementation.  It will work on
   *  temporary expressions, since they are only evaluated once, unlike a
   *  preprocessor macro.
  */ // :3
  template<typename _Tp> // :3
    inline const _Tp& // :3
    min(const _Tp& __a, const _Tp& __b) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_LessThanComparableConcept<_Tp>) // :3
      //return __b < __a ? __b : __a; :3
      if (__b < __a) // :3
	return __b; // :3
      return __a; // :3
    } // :3

  /**
   *  @brief This does what you think it does.
   *  @ingroup sorting_algorithms
   *  @param  __a  A thing of arbitrary type.
   *  @param  __b  Another thing of arbitrary type.
   *  @return   The greater of the parameters.
   *
   *  This is the simple classic generic implementation.  It will work on
   *  temporary expressions, since they are only evaluated once, unlike a
   *  preprocessor macro.
  */ // :3
  template<typename _Tp> // :3
    inline const _Tp& // :3
    max(const _Tp& __a, const _Tp& __b) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_LessThanComparableConcept<_Tp>) // :3
      //return  __a < __b ? __b : __a; :3
      if (__a < __b) // :3
	return __b; // :3
      return __a; // :3
    } // :3

  /**
   *  @brief This does what you think it does.
   *  @ingroup sorting_algorithms
   *  @param  __a  A thing of arbitrary type.
   *  @param  __b  Another thing of arbitrary type.
   *  @param  __comp  A @link comparison_functors comparison functor@endlink.
   *  @return   The lesser of the parameters.
   *
   *  This will work on temporary expressions, since they are only evaluated
   *  once, unlike a preprocessor macro.
  */ // :3
  template<typename _Tp, typename _Compare> // :3
    inline const _Tp& // :3
    min(const _Tp& __a, const _Tp& __b, _Compare __comp) // :3
    { // :3
      //return __comp(__b, __a) ? __b : __a; :3
      if (__comp(__b, __a)) // :3
	return __b; // :3
      return __a; // :3
    } // :3

  /**
   *  @brief This does what you think it does.
   *  @ingroup sorting_algorithms
   *  @param  __a  A thing of arbitrary type.
   *  @param  __b  Another thing of arbitrary type.
   *  @param  __comp  A @link comparison_functors comparison functor@endlink.
   *  @return   The greater of the parameters.
   *
   *  This will work on temporary expressions, since they are only evaluated
   *  once, unlike a preprocessor macro.
  */ // :3
  template<typename _Tp, typename _Compare> // :3
    inline const _Tp& // :3
    max(const _Tp& __a, const _Tp& __b, _Compare __comp) // :3
    { // :3
      //return __comp(__a, __b) ? __b : __a; :3
      if (__comp(__a, __b)) // :3
	return __b; // :3
      return __a; // :3
    } // :3

  // If _Iterator is a __normal_iterator return its base (a plain pointer, :3
  // normally) otherwise return it untouched.  See copy, fill, ... :3
  template<typename _Iterator> // :3
    struct _Niter_base // :3
    : _Iter_base<_Iterator, __is_normal_iterator<_Iterator>::__value> // :3
    { }; // :3

  template<typename _Iterator> // :3
    inline typename _Niter_base<_Iterator>::iterator_type // :3
    __niter_base(_Iterator __it) // :3
    { return _Niter_base<_Iterator>::_S_base(__it); } // :3

  // Likewise, for move_iterator. :3
  template<typename _Iterator> // :3
    struct _Miter_base // :3
    : _Iter_base<_Iterator, __is_move_iterator<_Iterator>::__value> // :3
    { }; // :3

  template<typename _Iterator> // :3
    inline typename _Miter_base<_Iterator>::iterator_type // :3
    __miter_base(_Iterator __it) // :3
    { return _Miter_base<_Iterator>::_S_base(__it); } // :3

  // All of these auxiliary structs serve two purposes.  (1) Replace :3
  // calls to copy with memmove whenever possible.  (Memmove, not memcpy, :3
  // because the input and output ranges are permitted to overlap.) :3
  // (2) If we're using random access iterators, then write the loop as :3
  // a for loop with an explicit count. :3

  template<bool, bool, typename> // :3
    struct __copy_move // :3
    { // :3
      template<typename _II, typename _OI> // :3
        static _OI // :3
        __copy_m(_II __first, _II __last, _OI __result) // :3
        { // :3
	  for (; __first != __last; ++__result, ++__first) // :3
	    *__result = *__first; // :3
	  return __result; // :3
	} // :3
    }; // :3

#if __cplusplus >= 201103L // :3
  template<typename _Category> // :3
    struct __copy_move<true, false, _Category> // :3
    { // :3
      template<typename _II, typename _OI> // :3
        static _OI // :3
        __copy_m(_II __first, _II __last, _OI __result) // :3
        { // :3
	  for (; __first != __last; ++__result, ++__first) // :3
	    *__result = std::move(*__first); // :3
	  return __result; // :3
	} // :3
    }; // :3
#endif // :3

  template<> // :3
    struct __copy_move<false, false, random_access_iterator_tag> // :3
    { // :3
      template<typename _II, typename _OI> // :3
        static _OI // :3
        __copy_m(_II __first, _II __last, _OI __result) // :3
        { // :3
	  typedef typename iterator_traits<_II>::difference_type _Distance; // :3
	  for(_Distance __n = __last - __first; __n > 0; --__n) // :3
	    { // :3
	      *__result = *__first; // :3
	      ++__first; // :3
	      ++__result; // :3
	    } // :3
	  return __result; // :3
	} // :3
    }; // :3

#if __cplusplus >= 201103L // :3
  template<> // :3
    struct __copy_move<true, false, random_access_iterator_tag> // :3
    { // :3
      template<typename _II, typename _OI> // :3
        static _OI // :3
        __copy_m(_II __first, _II __last, _OI __result) // :3
        { // :3
	  typedef typename iterator_traits<_II>::difference_type _Distance; // :3
	  for(_Distance __n = __last - __first; __n > 0; --__n) // :3
	    { // :3
	      *__result = std::move(*__first); // :3
	      ++__first; // :3
	      ++__result; // :3
	    } // :3
	  return __result; // :3
	} // :3
    }; // :3
#endif // :3

  template<bool _IsMove> // :3
    struct __copy_move<_IsMove, true, random_access_iterator_tag> // :3
    { // :3
      template<typename _Tp> // :3
        static _Tp* // :3
        __copy_m(const _Tp* __first, const _Tp* __last, _Tp* __result) // :3
        { // :3
#if __cplusplus >= 201103L // :3
	  // trivial types can have deleted assignment :3
	  static_assert( std::is_copy_assignable<_Tp>::value, // :3
	                 "type is not assignable" ); // :3
#endif // :3
	  const ptrdiff_t _Num = __last - __first; // :3
	  if (_Num) // :3
	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num); // :3
	  return __result + _Num; // :3
	} // :3
    }; // :3

  template<bool _IsMove, typename _II, typename _OI> // :3
    inline _OI // :3
    __copy_move_a(_II __first, _II __last, _OI __result) // :3
    { // :3
      typedef typename iterator_traits<_II>::value_type _ValueTypeI; // :3
      typedef typename iterator_traits<_OI>::value_type _ValueTypeO; // :3
      typedef typename iterator_traits<_II>::iterator_category _Category; // :3
      const bool __simple = (std::is_trivial<_ValueTypeI>::value // :3
	                     && is_pointer<_II>::value // :3
	                     && is_pointer<_OI>::value // :3
			     && std::is_same<_ValueTypeI, _ValueTypeO>::value); // :3

      return __copy_move<_IsMove, __simple, // :3
	                      _Category>::__copy_m(__first, __last, __result); // :3
    } // :3

  // Helpers for streambuf iterators (either istream or ostream). :3
  // NB: avoid including <iosfwd>, relatively large. :3
  template<typename _CharT> // :3
    struct char_traits; // :3

  template<typename _CharT, typename _Traits> // :3
    class istreambuf_iterator; // :3

  template<typename _CharT, typename _Traits> // :3
    class ostreambuf_iterator; // :3

  template<bool _IsMove, typename _CharT> // :3
    typename std::enable_if<__is_char<_CharT>::__value, // :3
	     ostreambuf_iterator<_CharT, char_traits<_CharT> > >::__type // :3
    __copy_move_a2(_CharT*, _CharT*, // :3
		   ostreambuf_iterator<_CharT, char_traits<_CharT> >); // :3

  template<bool _IsMove, typename _CharT> // :3
    typename std::enable_if<__is_char<_CharT>::__value, // :3
	     ostreambuf_iterator<_CharT, char_traits<_CharT> > >::__type // :3
    __copy_move_a2(const _CharT*, const _CharT*, // :3
		   ostreambuf_iterator<_CharT, char_traits<_CharT> >); // :3

  template<bool _IsMove, typename _CharT> // :3
    typename std::enable_if<__is_char<_CharT>::__value, // :3
				    _CharT*>::__type // :3
    __copy_move_a2(istreambuf_iterator<_CharT, char_traits<_CharT> >, // :3
		   istreambuf_iterator<_CharT, char_traits<_CharT> >, _CharT*); // :3

  template<bool _IsMove, typename _II, typename _OI> // :3
    inline _OI // :3
    __copy_move_a2(_II __first, _II __last, _OI __result) // :3
    { // :3
      return _OI(__copy_move_a<_IsMove>(__niter_base(__first), // :3
					     __niter_base(__last), // :3
					     __niter_base(__result))); // :3
    } // :3

  /**
   *  @brief Copies the range [first,last) into result.
   *  @ingroup mutating_algorithms
   *  @param  __first  An input iterator.
   *  @param  __last   An input iterator.
   *  @param  __result An output iterator.
   *  @return   result + (first - last)
   *
   *  This inline function will boil down to a call to @c memmove whenever
   *  possible.  Failing that, if random access iterators are passed, then the
   *  loop count will be known (and therefore a candidate for compiler
   *  optimizations such as unrolling).  Result may not be contained within
   *  [first,last); the copy_backward function should be used instead.
   *
   *  Note that the end of the output range is permitted to be contained
   *  within [first,last).
  */ // :3
  template<typename _II, typename _OI> // :3
    inline _OI // :3
    copy(_II __first, _II __last, _OI __result) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_II>) // :3
      __glibcxx_function_requires(_OutputIteratorConcept<_OI, // :3
	    typename iterator_traits<_II>::value_type>) // :3
      __glibcxx_requires_valid_range(__first, __last); // :3

      return (__copy_move_a2<__is_move_iterator<_II>::__value> // :3
	      (__miter_base(__first), __miter_base(__last), // :3
	       __result)); // :3
    } // :3

#if __cplusplus >= 201103L // :3
  /**
   *  @brief Moves the range [first,last) into result.
   *  @ingroup mutating_algorithms
   *  @param  __first  An input iterator.
   *  @param  __last   An input iterator.
   *  @param  __result An output iterator.
   *  @return   result + (first - last)
   *
   *  This inline function will boil down to a call to @c memmove whenever
   *  possible.  Failing that, if random access iterators are passed, then the
   *  loop count will be known (and therefore a candidate for compiler
   *  optimizations such as unrolling).  Result may not be contained within
   *  [first,last); the move_backward function should be used instead.
   *
   *  Note that the end of the output range is permitted to be contained
   *  within [first,last).
  */ // :3
  template<typename _II, typename _OI> // :3
    inline _OI // :3
    move(_II __first, _II __last, _OI __result) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_II>) // :3
      __glibcxx_function_requires(_OutputIteratorConcept<_OI, // :3
	    typename iterator_traits<_II>::value_type>) // :3
      __glibcxx_requires_valid_range(__first, __last); // :3

      return __copy_move_a2<true>(__miter_base(__first), // :3
				       __miter_base(__last), __result); // :3
    } // :3

#define _GLIBCXX_MOVE3(_Tp, _Up, _Vp) std::move(_Tp, _Up, _Vp) // :3
#else // :3
#define _GLIBCXX_MOVE3(_Tp, _Up, _Vp) std::copy(_Tp, _Up, _Vp) // :3
#endif // :3

  template<bool, bool, typename> // :3
    struct __copy_move_backward // :3
    { // :3
      template<typename _BI1, typename _BI2> // :3
        static _BI2 // :3
        __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result) // :3
        { // :3
	  while (__first != __last) // :3
	    *--__result = *--__last; // :3
	  return __result; // :3
	} // :3
    }; // :3

#if __cplusplus >= 201103L // :3
  template<typename _Category> // :3
    struct __copy_move_backward<true, false, _Category> // :3
    { // :3
      template<typename _BI1, typename _BI2> // :3
        static _BI2 // :3
        __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result) // :3
        { // :3
	  while (__first != __last) // :3
	    *--__result = std::move(*--__last); // :3
	  return __result; // :3
	} // :3
    }; // :3
#endif // :3

  template<> // :3
    struct __copy_move_backward<false, false, random_access_iterator_tag> // :3
    { // :3
      template<typename _BI1, typename _BI2> // :3
        static _BI2 // :3
        __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result) // :3
        { // :3
	  typename iterator_traits<_BI1>::difference_type __n; // :3
	  for (__n = __last - __first; __n > 0; --__n) // :3
	    *--__result = *--__last; // :3
	  return __result; // :3
	} // :3
    }; // :3

#if __cplusplus >= 201103L // :3
  template<> // :3
    struct __copy_move_backward<true, false, random_access_iterator_tag> // :3
    { // :3
      template<typename _BI1, typename _BI2> // :3
        static _BI2 // :3
        __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result) // :3
        { // :3
	  typename iterator_traits<_BI1>::difference_type __n; // :3
	  for (__n = __last - __first; __n > 0; --__n) // :3
	    *--__result = std::move(*--__last); // :3
	  return __result; // :3
	} // :3
    }; // :3
#endif // :3

  template<bool _IsMove> // :3
    struct __copy_move_backward<_IsMove, true, random_access_iterator_tag> // :3
    { // :3
      template<typename _Tp> // :3
        static _Tp* // :3
        __copy_move_b(const _Tp* __first, const _Tp* __last, _Tp* __result) // :3
        { // :3
#if __cplusplus >= 201103L // :3
	  // trivial types can have deleted assignment :3
	  static_assert( std::is_copy_assignable<_Tp>::value, // :3
	                 "type is not assignable" ); // :3
#endif // :3
	  const ptrdiff_t _Num = __last - __first; // :3
	  if (_Num) // :3
	    __builtin_memmove(__result - _Num, __first, sizeof(_Tp) * _Num); // :3
	  return __result - _Num; // :3
	} // :3
    }; // :3

  template<bool _IsMove, typename _BI1, typename _BI2> // :3
    inline _BI2 // :3
    __copy_move_backward_a(_BI1 __first, _BI1 __last, _BI2 __result) // :3
    { // :3
      typedef typename iterator_traits<_BI1>::value_type _ValueType1; // :3
      typedef typename iterator_traits<_BI2>::value_type _ValueType2; // :3
      typedef typename iterator_traits<_BI1>::iterator_category _Category; // :3
      const bool __simple = (std::is_trivial<_ValueType1>::value // :3
	                     && is_pointer<_BI1>::value // :3
	                     && is_pointer<_BI2>::value // :3
			     && std::is_same<_ValueType1, _ValueType2>::value); // :3

      return __copy_move_backward<_IsMove, __simple, // :3
	                               _Category>::__copy_move_b(__first, // :3
								 __last, // :3
								 __result); // :3
    } // :3

  template<bool _IsMove, typename _BI1, typename _BI2> // :3
    inline _BI2 // :3
    __copy_move_backward_a2(_BI1 __first, _BI1 __last, _BI2 __result) // :3
    { // :3
      return _BI2(__copy_move_backward_a<_IsMove> // :3
		  (__niter_base(__first), __niter_base(__last), // :3
		   __niter_base(__result))); // :3
    } // :3

  /**
   *  @brief Copies the range [first,last) into result.
   *  @ingroup mutating_algorithms
   *  @param  __first  A bidirectional iterator.
   *  @param  __last   A bidirectional iterator.
   *  @param  __result A bidirectional iterator.
   *  @return   result - (first - last)
   *
   *  The function has the same effect as copy, but starts at the end of the
   *  range and works its way to the start, returning the start of the result.
   *  This inline function will boil down to a call to @c memmove whenever
   *  possible.  Failing that, if random access iterators are passed, then the
   *  loop count will be known (and therefore a candidate for compiler
   *  optimizations such as unrolling).
   *
   *  Result may not be in the range (first,last].  Use copy instead.  Note
   *  that the start of the output range may overlap [first,last).
  */ // :3
  template<typename _BI1, typename _BI2> // :3
    inline _BI2 // :3
    copy_backward(_BI1 __first, _BI1 __last, _BI2 __result) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_BidirectionalIteratorConcept<_BI1>) // :3
      __glibcxx_function_requires(_Mutable_BidirectionalIteratorConcept<_BI2>) // :3
      __glibcxx_function_requires(_ConvertibleConcept< // :3
	    typename iterator_traits<_BI1>::value_type, // :3
	    typename iterator_traits<_BI2>::value_type>) // :3
      __glibcxx_requires_valid_range(__first, __last); // :3

      return (__copy_move_backward_a2<__is_move_iterator<_BI1>::__value> // :3
	      (__miter_base(__first), __miter_base(__last), // :3
	       __result)); // :3
    } // :3

#if __cplusplus >= 201103L // :3
  /**
   *  @brief Moves the range [first,last) into result.
   *  @ingroup mutating_algorithms
   *  @param  __first  A bidirectional iterator.
   *  @param  __last   A bidirectional iterator.
   *  @param  __result A bidirectional iterator.
   *  @return   result - (first - last)
   *
   *  The function has the same effect as move, but starts at the end of the
   *  range and works its way to the start, returning the start of the result.
   *  This inline function will boil down to a call to @c memmove whenever
   *  possible.  Failing that, if random access iterators are passed, then the
   *  loop count will be known (and therefore a candidate for compiler
   *  optimizations such as unrolling).
   *
   *  Result may not be in the range (first,last].  Use move instead.  Note
   *  that the start of the output range may overlap [first,last).
  */ // :3
  template<typename _BI1, typename _BI2> // :3
    inline _BI2 // :3
    move_backward(_BI1 __first, _BI1 __last, _BI2 __result) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_BidirectionalIteratorConcept<_BI1>) // :3
      __glibcxx_function_requires(_Mutable_BidirectionalIteratorConcept<_BI2>) // :3
      __glibcxx_function_requires(_ConvertibleConcept< // :3
	    typename iterator_traits<_BI1>::value_type, // :3
	    typename iterator_traits<_BI2>::value_type>) // :3
      __glibcxx_requires_valid_range(__first, __last); // :3

      return __copy_move_backward_a2<true>(__miter_base(__first), // :3
						__miter_base(__last), // :3
						__result); // :3
    } // :3

#define _GLIBCXX_MOVE_BACKWARD3(_Tp, _Up, _Vp) std::move_backward(_Tp, _Up, _Vp) // :3
#else // :3
#define _GLIBCXX_MOVE_BACKWARD3(_Tp, _Up, _Vp) std::copy_backward(_Tp, _Up, _Vp) // :3
#endif // :3

  template<typename _ForwardIterator, typename _Tp> // :3
    inline typename // :3
    std::enable_if<!std::is_scalar<_Tp>::value, void>::type // :3
    __fill_a(_ForwardIterator __first, _ForwardIterator __last, // :3
 	     const _Tp& __value) // :3
    { // :3
      for (; __first != __last; ++__first) // :3
	*__first = __value; // :3
    } // :3

  template<typename _ForwardIterator, typename _Tp> // :3
    inline typename // :3
    std::enable_if<std::is_scalar<_Tp>::value, void>::type // :3
    __fill_a(_ForwardIterator __first, _ForwardIterator __last, // :3
	     const _Tp& __value) // :3
    { // :3
      const _Tp __tmp = __value; // :3
      for (; __first != __last; ++__first) // :3
	*__first = __tmp; // :3
    } // :3

  // Specialization: for char types we can use memset. :3
  template<typename _Tp> // :3
    inline typename // :3
    std::enable_if<__is_byte<_Tp>::__value, void>::type // :3
    __fill_a(_Tp* __first, _Tp* __last, const _Tp& __c) // :3
    { // :3
      const _Tp __tmp = __c; // :3
      __builtin_memset(__first, static_cast<unsigned char>(__tmp), // :3
		       __last - __first); // :3
    } // :3

  /**
   *  @brief Fills the range [first,last) with copies of value.
   *  @ingroup mutating_algorithms
   *  @param  __first  A forward iterator.
   *  @param  __last   A forward iterator.
   *  @param  __value  A reference-to-const of arbitrary type.
   *  @return   Nothing.
   *
   *  This function fills a range with copies of the same value.  For char
   *  types filling contiguous areas of memory, this becomes an inline call
   *  to @c memset or @c wmemset.
  */ // :3
  template<typename _ForwardIterator, typename _Tp> // :3
    inline void // :3
    fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_Mutable_ForwardIteratorConcept< // :3
				  _ForwardIterator>) // :3
      __glibcxx_requires_valid_range(__first, __last); // :3

      __fill_a(__niter_base(__first), __niter_base(__last), // :3
		    __value); // :3
    } // :3

  template<typename _OutputIterator, typename _Size, typename _Tp> // :3
    inline typename // :3
    std::enable_if<!std::is_scalar<_Tp>::value, _OutputIterator>::type // :3
    __fill_n_a(_OutputIterator __first, _Size __n, const _Tp& __value) // :3
    { // :3
      for (__decltype(__n + 0) __niter = __n; // :3
	   __niter > 0; --__niter, ++__first) // :3
	*__first = __value; // :3
      return __first; // :3
    } // :3

  template<typename _OutputIterator, typename _Size, typename _Tp> // :3
    inline typename // :3
    std::enable_if<std::is_scalar<_Tp>::value, _OutputIterator>::type // :3
    __fill_n_a(_OutputIterator __first, _Size __n, const _Tp& __value) // :3
    { // :3
      const _Tp __tmp = __value; // :3
      for (__decltype(__n + 0) __niter = __n; // :3
	   __niter > 0; --__niter, ++__first) // :3
	*__first = __tmp; // :3
      return __first; // :3
    } // :3

  template<typename _Size, typename _Tp> // :3
    inline typename // :3
    std::enable_if<__is_byte<_Tp>::__value, _Tp*>::type // :3
    __fill_n_a(_Tp* __first, _Size __n, const _Tp& __c) // :3
    { // :3
      __fill_a(__first, __first + __n, __c); // :3
      return __first + __n; // :3
    } // :3

  /**
   *  @brief Fills the range [first,first+n) with copies of value.
   *  @ingroup mutating_algorithms
   *  @param  __first  An output iterator.
   *  @param  __n      The count of copies to perform.
   *  @param  __value  A reference-to-const of arbitrary type.
   *  @return   The iterator at first+n.
   *
   *  This function fills a range with copies of the same value.  For char
   *  types filling contiguous areas of memory, this becomes an inline call
   *  to @c memset or @ wmemset.
   *
   *  _GLIBCXX_RESOLVE_LIB_DEFECTS
   *  DR 865. More algorithms that throw away information
  */ // :3
  template<typename _OI, typename _Size, typename _Tp> // :3
    inline _OI // :3
    fill_n(_OI __first, _Size __n, const _Tp& __value) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_OutputIteratorConcept<_OI, _Tp>) // :3

      return _OI(__fill_n_a(__niter_base(__first), __n, __value)); // :3
    } // :3

  template<bool _BoolType> // :3
    struct __equal // :3
    { // :3
      template<typename _II1, typename _II2> // :3
        static bool // :3
        equal(_II1 __first1, _II1 __last1, _II2 __first2) // :3
        { // :3
	  for (; __first1 != __last1; ++__first1, ++__first2) // :3
	    if (!(*__first1 == *__first2)) // :3
	      return false; // :3
	  return true; // :3
	} // :3
    }; // :3

  template<> // :3
    struct __equal<true> // :3
    { // :3
      template<typename _Tp> // :3
        static bool // :3
        equal(const _Tp* __first1, const _Tp* __last1, const _Tp* __first2) // :3
        { // :3
	  return !__builtin_memcmp(__first1, __first2, sizeof(_Tp) // :3
				   * (__last1 - __first1)); // :3
	} // :3
    }; // :3

  template<typename _II1, typename _II2> // :3
    inline bool // :3
    __equal_aux(_II1 __first1, _II1 __last1, _II2 __first2) // :3
    { // :3
      typedef typename iterator_traits<_II1>::value_type _ValueType1; // :3
      typedef typename iterator_traits<_II2>::value_type _ValueType2; // :3
      const bool __simple = ((is_integral<_ValueType1>::value // :3
			      || is_pointer<_ValueType1>::value) // :3
	                     && is_pointer<_II1>::value // :3
	                     && is_pointer<_II2>::value // :3
			     && std::is_same<_ValueType1, _ValueType2>::value); // :3

      return __equal<__simple>::equal(__first1, __last1, __first2); // :3
    } // :3

  template<typename, typename> // :3
    struct __lc_rai // :3
    { // :3
      template<typename _II1, typename _II2> // :3
        static _II1 // :3
        __newlast1(_II1, _II1 __last1, _II2, _II2) // :3
        { return __last1; } // :3

      template<typename _II> // :3
        static bool // :3
        __cnd2(_II __first, _II __last) // :3
        { return __first != __last; } // :3
    }; // :3

  template<> // :3
    struct __lc_rai<random_access_iterator_tag, random_access_iterator_tag> // :3
    { // :3
      template<typename _RAI1, typename _RAI2> // :3
        static _RAI1 // :3
        __newlast1(_RAI1 __first1, _RAI1 __last1, // :3
		   _RAI2 __first2, _RAI2 __last2) // :3
        { // :3
	  const typename iterator_traits<_RAI1>::difference_type // :3
	    __diff1 = __last1 - __first1; // :3
	  const typename iterator_traits<_RAI2>::difference_type // :3
	    __diff2 = __last2 - __first2; // :3
	  return __diff2 < __diff1 ? __first1 + __diff2 : __last1; // :3
	} // :3

      template<typename _RAI> // :3
        static bool // :3
        __cnd2(_RAI, _RAI) // :3
        { return true; } // :3
    }; // :3

  template<typename _II1, typename _II2, typename _Compare> // :3
    bool // :3
    __lexicographical_compare_impl(_II1 __first1, _II1 __last1, // :3
				   _II2 __first2, _II2 __last2, // :3
				   _Compare __comp) // :3
    { // :3
      typedef typename iterator_traits<_II1>::iterator_category _Category1; // :3
      typedef typename iterator_traits<_II2>::iterator_category _Category2; // :3
      typedef __lc_rai<_Category1, _Category2> __rai_type; // :3

      __last1 = __rai_type::__newlast1(__first1, __last1, __first2, __last2); // :3
      for (; __first1 != __last1 && __rai_type::__cnd2(__first2, __last2); // :3
	   ++__first1, ++__first2) // :3
	{ // :3
	  if (__comp(__first1, __first2)) // :3
	    return true; // :3
	  if (__comp(__first2, __first1)) // :3
	    return false; // :3
	} // :3
      return __first1 == __last1 && __first2 != __last2; // :3
    } // :3

  template<bool _BoolType> // :3
    struct __lexicographical_compare // :3
    { // :3
      template<typename _II1, typename _II2> // :3
        static bool __lc(_II1, _II1, _II2, _II2); // :3
    }; // :3

  template<bool _BoolType> // :3
    template<typename _II1, typename _II2> // :3
      bool // :3
      __lexicographical_compare<_BoolType>:: // :3
      __lc(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2) // :3
      { // :3
	return __lexicographical_compare_impl(__first1, __last1, // :3
						   __first2, __last2, // :3
					__gnu_cxx::__ops::__iter_less_iter()); // :3
      } // :3

  template<> // :3
    struct __lexicographical_compare<true> // :3
    { // :3
      template<typename _Tp, typename _Up> // :3
        static bool // :3
        __lc(const _Tp* __first1, const _Tp* __last1, // :3
	     const _Up* __first2, const _Up* __last2) // :3
	{ // :3
	  const size_t __len1 = __last1 - __first1; // :3
	  const size_t __len2 = __last2 - __first2; // :3
	  const int __result = __builtin_memcmp(__first1, __first2, // :3
						std::min(__len1, __len2)); // :3
	  return __result != 0 ? __result < 0 : __len1 < __len2; // :3
	} // :3
    }; // :3

  template<typename _II1, typename _II2> // :3
    inline bool // :3
    __lexicographical_compare_aux(_II1 __first1, _II1 __last1, // :3
				  _II2 __first2, _II2 __last2) // :3
    { // :3
      typedef typename iterator_traits<_II1>::value_type _ValueType1; // :3
      typedef typename iterator_traits<_II2>::value_type _ValueType2; // :3
      const bool __simple = // :3
	(__is_byte<_ValueType1>::__value && __is_byte<_ValueType2>::__value // :3
	 && !__gnu_cxx::__numeric_traits<_ValueType1>::__is_signed // :3
	 && !__gnu_cxx::__numeric_traits<_ValueType2>::__is_signed // :3
	 && is_pointer<_II1>::value // :3
	 && is_pointer<_II2>::value); // :3

      return __lexicographical_compare<__simple>::__lc(__first1, __last1, // :3
							    __first2, __last2); // :3
    } // :3

  template<typename _ForwardIterator, typename _Tp, typename _Compare> // :3
    _ForwardIterator // :3
    __lower_bound(_ForwardIterator __first, _ForwardIterator __last, // :3
		  const _Tp& __val, _Compare __comp) // :3
    { // :3
      typedef typename iterator_traits<_ForwardIterator>::difference_type // :3
	_DistanceType; // :3

      _DistanceType __len = std::distance(__first, __last); // :3

      while (__len > 0) // :3
	{ // :3
	  _DistanceType __half = __len >> 1; // :3
	  _ForwardIterator __middle = __first; // :3
	  std::advance(__middle, __half); // :3
	  if (__comp(__middle, __val)) // :3
	    { // :3
	      __first = __middle; // :3
	      ++__first; // :3
	      __len = __len - __half - 1; // :3
	    } // :3
	  else // :3
	    __len = __half; // :3
	} // :3
      return __first; // :3
    } // :3

  /**
   *  @brief Finds the first position in which @a val could be inserted
   *         without changing the ordering.
   *  @param  __first   An iterator.
   *  @param  __last    Another iterator.
   *  @param  __val     The search term.
   *  @return         An iterator pointing to the first element <em>not less
   *                  than</em> @a val, or end() if every element is less than
   *                  @a val.
   *  @ingroup binary_search_algorithms
  */ // :3
  template<typename _ForwardIterator, typename _Tp> // :3
    inline _ForwardIterator // :3
    lower_bound(_ForwardIterator __first, _ForwardIterator __last, // :3
		const _Tp& __val) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>) // :3
      __glibcxx_function_requires(_LessThanOpConcept< // :3
	    typename iterator_traits<_ForwardIterator>::value_type, _Tp>) // :3

      return geode::stl::__lower_bound(__first, __last, __val, // :3
				__gnu_cxx::__ops::__iter_less_val()); // :3
    } // :3

  /// This is a helper function for the sort routines and for random.tcc. :3
  //  Precondition: __n > 0. :3
  inline _GLIBCXX_CONSTEXPR int // :3
  __lg(int __n) // :3
  { return sizeof(int) * __CHAR_BIT__  - 1 - __builtin_clz(__n); } // :3

  inline _GLIBCXX_CONSTEXPR unsigned // :3
  __lg(unsigned __n) // :3
  { return sizeof(int) * __CHAR_BIT__  - 1 - __builtin_clz(__n); } // :3

  inline _GLIBCXX_CONSTEXPR long // :3
  __lg(long __n) // :3
  { return sizeof(long) * __CHAR_BIT__ - 1 - __builtin_clzl(__n); } // :3

  inline _GLIBCXX_CONSTEXPR unsigned long // :3
  __lg(unsigned long __n) // :3
  { return sizeof(long) * __CHAR_BIT__ - 1 - __builtin_clzl(__n); } // :3

  inline _GLIBCXX_CONSTEXPR long long // :3
  __lg(long long __n) // :3
  { return sizeof(long long) * __CHAR_BIT__ - 1 - __builtin_clzll(__n); } // :3

  inline _GLIBCXX_CONSTEXPR unsigned long long // :3
  __lg(unsigned long long __n) // :3
  { return sizeof(long long) * __CHAR_BIT__ - 1 - __builtin_clzll(__n); } // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3

_GLIBCXX_BEGIN_NAMESPACE_ALGO // :3

  /**
   *  @brief Tests a range for element-wise equality.
   *  @ingroup non_mutating_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @return   A boolean true or false.
   *
   *  This compares the elements of two ranges using @c == and returns true or
   *  false depending on whether all of the corresponding elements of the
   *  ranges are equal.
  */ // :3
  template<typename _II1, typename _II2> // :3
    inline bool // :3
    equal(_II1 __first1, _II1 __last1, _II2 __first2) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_II1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_II2>) // :3
      __glibcxx_function_requires(_EqualOpConcept< // :3
	    typename iterator_traits<_II1>::value_type, // :3
	    typename iterator_traits<_II2>::value_type>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3

      return __equal_aux(__niter_base(__first1), // :3
			      __niter_base(__last1), // :3
			      __niter_base(__first2)); // :3
    } // :3

  /**
   *  @brief Tests a range for element-wise equality.
   *  @ingroup non_mutating_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @param __binary_pred A binary predicate @link functors
   *                  functor@endlink.
   *  @return         A boolean true or false.
   *
   *  This compares the elements of two ranges using the binary_pred
   *  parameter, and returns true or
   *  false depending on whether all of the corresponding elements of the
   *  ranges are equal.
  */ // :3
  template<typename _IIter1, typename _IIter2, typename _BinaryPredicate> // :3
    inline bool // :3
    equal(_IIter1 __first1, _IIter1 __last1, // :3
	  _IIter2 __first2, _BinaryPredicate __binary_pred) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_IIter1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_IIter2>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3

      for (; __first1 != __last1; ++__first1, ++__first2) // :3
	if (!bool(__binary_pred(*__first1, *__first2))) // :3
	  return false; // :3
      return true; // :3
    } // :3

#if __cplusplus > 201103L // :3
  /**
   *  @brief Tests a range for element-wise equality.
   *  @ingroup non_mutating_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @param  __last2   An input iterator.
   *  @return   A boolean true or false.
   *
   *  This compares the elements of two ranges using @c == and returns true or
   *  false depending on whether all of the corresponding elements of the
   *  ranges are equal.
  */ // :3
  template<typename _II1, typename _II2> // :3
    inline bool // :3
    equal(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_II1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_II2>) // :3
      __glibcxx_function_requires(_EqualOpConcept< // :3
	    typename iterator_traits<_II1>::value_type, // :3
	    typename iterator_traits<_II2>::value_type>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3
      __glibcxx_requires_valid_range(__first2, __last2); // :3

      using _RATag = random_access_iterator_tag; // :3
      using _Cat1 = typename iterator_traits<_II1>::iterator_category; // :3
      using _Cat2 = typename iterator_traits<_II2>::iterator_category; // :3
      using _RAIters = __and_<std::is_same<_Cat1, _RATag>, std::is_same<_Cat2, _RATag>>; // :3
      if (_RAIters()) // :3
	{ // :3
	  auto __d1 = std::distance(__first1, __last1); // :3
	  auto __d2 = std::distance(__first2, __last2); // :3
	  if (__d1 != __d2) // :3
	    return false; // :3
	  return _GLIBCXX_STD_A::equal(__first1, __last1, __first2); // :3
	} // :3

      for (; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) // :3
	if (!(*__first1 == *__first2)) // :3
	  return false; // :3
      return __first1 == __last1 && __first2 == __last2; // :3
    } // :3

  /**
   *  @brief Tests a range for element-wise equality.
   *  @ingroup non_mutating_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @param  __last2   An input iterator.
   *  @param __binary_pred A binary predicate @link functors
   *                  functor@endlink.
   *  @return         A boolean true or false.
   *
   *  This compares the elements of two ranges using the binary_pred
   *  parameter, and returns true or
   *  false depending on whether all of the corresponding elements of the
   *  ranges are equal.
  */ // :3
  template<typename _IIter1, typename _IIter2, typename _BinaryPredicate> // :3
    inline bool // :3
    equal(_IIter1 __first1, _IIter1 __last1, // :3
	  _IIter2 __first2, _IIter2 __last2, _BinaryPredicate __binary_pred) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_IIter1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_IIter2>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3
      __glibcxx_requires_valid_range(__first2, __last2); // :3

      using _RATag = random_access_iterator_tag; // :3
      using _Cat1 = typename iterator_traits<_IIter1>::iterator_category; // :3
      using _Cat2 = typename iterator_traits<_IIter2>::iterator_category; // :3
      using _RAIters = __and_<std::is_same<_Cat1, _RATag>, std::is_same<_Cat2, _RATag>>; // :3
      if (_RAIters()) // :3
	{ // :3
	  auto __d1 = std::distance(__first1, __last1); // :3
	  auto __d2 = std::distance(__first2, __last2); // :3
	  if (__d1 != __d2) // :3
	    return false; // :3
	  return _GLIBCXX_STD_A::equal(__first1, __last1, __first2, // :3
				       __binary_pred); // :3
	} // :3

      for (; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) // :3
	if (!bool(__binary_pred(*__first1, *__first2))) // :3
	  return false; // :3
      return __first1 == __last1 && __first2 == __last2; // :3
    } // :3
#endif // :3

  /**
   *  @brief Performs @b dictionary comparison on ranges.
   *  @ingroup sorting_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @param  __last2   An input iterator.
   *  @return   A boolean true or false.
   *
   *  <em>Returns true if the sequence of elements defined by the range
   *  [first1,last1) is lexicographically less than the sequence of elements
   *  defined by the range [first2,last2).  Returns false otherwise.</em>
   *  (Quoted from [25.3.8]/1.)  If the iterators are all character pointers,
   *  then this is an inline call to @c memcmp.
  */ // :3
  template<typename _II1, typename _II2> // :3
    inline bool // :3
    lexicographical_compare(_II1 __first1, _II1 __last1, // :3
			    _II2 __first2, _II2 __last2) // :3
    { // :3
#ifdef _GLIBCXX_CONCEPT_CHECKS // :3
      // concept requirements :3
      typedef typename iterator_traits<_II1>::value_type _ValueType1; // :3
      typedef typename iterator_traits<_II2>::value_type _ValueType2; // :3
#endif // :3
      __glibcxx_function_requires(_InputIteratorConcept<_II1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_II2>) // :3
      __glibcxx_function_requires(_LessThanOpConcept<_ValueType1, _ValueType2>) // :3
      __glibcxx_function_requires(_LessThanOpConcept<_ValueType2, _ValueType1>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3
      __glibcxx_requires_valid_range(__first2, __last2); // :3

      return __lexicographical_compare_aux(__niter_base(__first1), // :3
						__niter_base(__last1), // :3
						__niter_base(__first2), // :3
						__niter_base(__last2)); // :3
    } // :3

  /**
   *  @brief Performs @b dictionary comparison on ranges.
   *  @ingroup sorting_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @param  __last2   An input iterator.
   *  @param  __comp  A @link comparison_functors comparison functor@endlink.
   *  @return   A boolean true or false.
   *
   *  The same as the four-parameter @c lexicographical_compare, but uses the
   *  comp parameter instead of @c <.
  */ // :3
  template<typename _II1, typename _II2, typename _Compare> // :3
    inline bool // :3
    lexicographical_compare(_II1 __first1, _II1 __last1, // :3
			    _II2 __first2, _II2 __last2, _Compare __comp) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_II1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_II2>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3
      __glibcxx_requires_valid_range(__first2, __last2); // :3

      return __lexicographical_compare_impl // :3
	(__first1, __last1, __first2, __last2, // :3
	 __gnu_cxx::__ops::__iter_comp_iter(__comp)); // :3
    } // :3

  template<typename _InputIterator1, typename _InputIterator2, // :3
	   typename _BinaryPredicate> // :3
    pair<_InputIterator1, _InputIterator2> // :3
    __mismatch(_InputIterator1 __first1, _InputIterator1 __last1, // :3
	       _InputIterator2 __first2, _BinaryPredicate __binary_pred) // :3
    { // :3
      while (__first1 != __last1 && __binary_pred(__first1, __first2)) // :3
        { // :3
	  ++__first1; // :3
	  ++__first2; // :3
        } // :3
      return pair<_InputIterator1, _InputIterator2>(__first1, __first2); // :3
    } // :3

  /**
   *  @brief Finds the places in ranges which don't match.
   *  @ingroup non_mutating_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @return   A pair of iterators pointing to the first mismatch.
   *
   *  This compares the elements of two ranges using @c == and returns a pair
   *  of iterators.  The first iterator points into the first range, the
   *  second iterator points into the second range, and the elements pointed
   *  to by the iterators are not equal.
  */ // :3
  template<typename _InputIterator1, typename _InputIterator2> // :3
    inline pair<_InputIterator1, _InputIterator2> // :3
    mismatch(_InputIterator1 __first1, _InputIterator1 __last1, // :3
	     _InputIterator2 __first2) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>) // :3
      __glibcxx_function_requires(_EqualOpConcept< // :3
	    typename iterator_traits<_InputIterator1>::value_type, // :3
	    typename iterator_traits<_InputIterator2>::value_type>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3

      return __mismatch(__first1, __last1, __first2, // :3
			     __gnu_cxx::__ops::__iter_equal_to_iter()); // :3
    } // :3

  /**
   *  @brief Finds the places in ranges which don't match.
   *  @ingroup non_mutating_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @param __binary_pred A binary predicate @link functors
   *         functor@endlink.
   *  @return   A pair of iterators pointing to the first mismatch.
   *
   *  This compares the elements of two ranges using the binary_pred
   *  parameter, and returns a pair
   *  of iterators.  The first iterator points into the first range, the
   *  second iterator points into the second range, and the elements pointed
   *  to by the iterators are not equal.
  */ // :3
  template<typename _InputIterator1, typename _InputIterator2, // :3
	   typename _BinaryPredicate> // :3
    inline pair<_InputIterator1, _InputIterator2> // :3
    mismatch(_InputIterator1 __first1, _InputIterator1 __last1, // :3
	     _InputIterator2 __first2, _BinaryPredicate __binary_pred) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3

      return __mismatch(__first1, __last1, __first2, // :3
	__gnu_cxx::__ops::__iter_comp_iter(__binary_pred)); // :3
    } // :3

#if __cplusplus > 201103L // :3

  template<typename _InputIterator1, typename _InputIterator2, // :3
	   typename _BinaryPredicate> // :3
    pair<_InputIterator1, _InputIterator2> // :3
    __mismatch(_InputIterator1 __first1, _InputIterator1 __last1, // :3
	       _InputIterator2 __first2, _InputIterator2 __last2, // :3
	       _BinaryPredicate __binary_pred) // :3
    { // :3
      while (__first1 != __last1 && __first2 != __last2 // :3
	     && __binary_pred(__first1, __first2)) // :3
        { // :3
	  ++__first1; // :3
	  ++__first2; // :3
        } // :3
      return pair<_InputIterator1, _InputIterator2>(__first1, __first2); // :3
    } // :3

  /**
   *  @brief Finds the places in ranges which don't match.
   *  @ingroup non_mutating_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @param  __last2   An input iterator.
   *  @return   A pair of iterators pointing to the first mismatch.
   *
   *  This compares the elements of two ranges using @c == and returns a pair
   *  of iterators.  The first iterator points into the first range, the
   *  second iterator points into the second range, and the elements pointed
   *  to by the iterators are not equal.
  */ // :3
  template<typename _InputIterator1, typename _InputIterator2> // :3
    inline pair<_InputIterator1, _InputIterator2> // :3
    mismatch(_InputIterator1 __first1, _InputIterator1 __last1, // :3
	     _InputIterator2 __first2, _InputIterator2 __last2) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>) // :3
      __glibcxx_function_requires(_EqualOpConcept< // :3
	    typename iterator_traits<_InputIterator1>::value_type, // :3
	    typename iterator_traits<_InputIterator2>::value_type>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3
      __glibcxx_requires_valid_range(__first2, __last2); // :3

      return __mismatch(__first1, __last1, __first2, __last2, // :3
			     __gnu_cxx::__ops::__iter_equal_to_iter()); // :3
    } // :3

  /**
   *  @brief Finds the places in ranges which don't match.
   *  @ingroup non_mutating_algorithms
   *  @param  __first1  An input iterator.
   *  @param  __last1   An input iterator.
   *  @param  __first2  An input iterator.
   *  @param  __last2   An input iterator.
   *  @param __binary_pred A binary predicate @link functors
   *         functor@endlink.
   *  @return   A pair of iterators pointing to the first mismatch.
   *
   *  This compares the elements of two ranges using the binary_pred
   *  parameter, and returns a pair
   *  of iterators.  The first iterator points into the first range, the
   *  second iterator points into the second range, and the elements pointed
   *  to by the iterators are not equal.
  */ // :3
  template<typename _InputIterator1, typename _InputIterator2, // :3
	   typename _BinaryPredicate> // :3
    inline pair<_InputIterator1, _InputIterator2> // :3
    mismatch(_InputIterator1 __first1, _InputIterator1 __last1, // :3
	     _InputIterator2 __first2, _InputIterator2 __last2, // :3
	     _BinaryPredicate __binary_pred) // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>) // :3
      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>) // :3
      __glibcxx_requires_valid_range(__first1, __last1); // :3
      __glibcxx_requires_valid_range(__first2, __last2); // :3

      return __mismatch(__first1, __last1, __first2, __last2, // :3
			     __gnu_cxx::__ops::__iter_comp_iter(__binary_pred)); // :3
    } // :3
#endif // :3

_GLIBCXX_END_NAMESPACE_ALGO // :3
} // namespace std :3