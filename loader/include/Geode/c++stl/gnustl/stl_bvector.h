// vector<bool> specialization -*- C++ -*- :3

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
 * Copyright (c) 1996-1999
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

/** @file bits/stl_bvector.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{vector}
 */ // :3

#pragma once // :3

#include <initializer_list> // :3
#include "stl_algobase.h" // :3
#include "allocator.h" // :3
#include "functexcept.h" // :3
#include "ext/alloc_traits.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER // :3

  typedef unsigned long _Bit_type; // :3
  enum { _S_word_bit = int(__CHAR_BIT__ * sizeof(_Bit_type)) }; // :3

  struct _Bit_reference // :3
  { // :3
    _Bit_type * _M_p; // :3
    _Bit_type _M_mask; // :3

    _Bit_reference(_Bit_type * __x, _Bit_type __y) // :3
    : _M_p(__x), _M_mask(__y) { } // :3

    _Bit_reference() _GLIBCXX_NOEXCEPT : _M_p(0), _M_mask(0) { } // :3

    operator bool() const _GLIBCXX_NOEXCEPT // :3
    { return !!(*_M_p & _M_mask); } // :3

    _Bit_reference& // :3
    operator=(bool __x) _GLIBCXX_NOEXCEPT // :3
    { // :3
      if (__x) // :3
	*_M_p |= _M_mask; // :3
      else // :3
	*_M_p &= ~_M_mask; // :3
      return *this; // :3
    } // :3

    _Bit_reference& // :3
    operator=(const _Bit_reference& __x) _GLIBCXX_NOEXCEPT // :3
    { return *this = bool(__x); } // :3

    bool // :3
    operator==(const _Bit_reference& __x) const // :3
    { return bool(*this) == bool(__x); } // :3

    bool // :3
    operator<(const _Bit_reference& __x) const // :3
    { return !bool(*this) && bool(__x); } // :3

    void // :3
    flip() _GLIBCXX_NOEXCEPT // :3
    { *_M_p ^= _M_mask; } // :3
  }; // :3

#if __cplusplus >= 201103L // :3
  inline void // :3
  swap(_Bit_reference __x, _Bit_reference __y) noexcept // :3
  { // :3
    bool __tmp = __x; // :3
    __x = __y; // :3
    __y = __tmp; // :3
  } // :3

  inline void // :3
  swap(_Bit_reference __x, bool& __y) noexcept // :3
  { // :3
    bool __tmp = __x; // :3
    __x = __y; // :3
    __y = __tmp; // :3
  } // :3

  inline void // :3
  swap(bool& __x, _Bit_reference __y) noexcept // :3
  { // :3
    bool __tmp = __x; // :3
    __x = __y; // :3
    __y = __tmp; // :3
  } // :3
#endif // :3

  struct _Bit_iterator_base // :3
  : public iterator<random_access_iterator_tag, bool> // :3
  { // :3
    _Bit_type * _M_p; // :3
    unsigned int _M_offset; // :3

    _Bit_iterator_base(_Bit_type * __x, unsigned int __y) // :3
    : _M_p(__x), _M_offset(__y) { } // :3

    void // :3
    _M_bump_up() // :3
    { // :3
      if (_M_offset++ == int(_S_word_bit) - 1) // :3
	{ // :3
	  _M_offset = 0; // :3
	  ++_M_p; // :3
	} // :3
    } // :3

    void // :3
    _M_bump_down() // :3
    { // :3
      if (_M_offset-- == 0) // :3
	{ // :3
	  _M_offset = int(_S_word_bit) - 1; // :3
	  --_M_p; // :3
	} // :3
    } // :3

    void // :3
    _M_incr(ptrdiff_t __i) // :3
    { // :3
      difference_type __n = __i + _M_offset; // :3
      _M_p += __n / int(_S_word_bit); // :3
      __n = __n % int(_S_word_bit); // :3
      if (__n < 0) // :3
	{ // :3
	  __n += int(_S_word_bit); // :3
	  --_M_p; // :3
	} // :3
      _M_offset = static_cast<unsigned int>(__n); // :3
    } // :3

    bool // :3
    operator==(const _Bit_iterator_base& __i) const // :3
    { return _M_p == __i._M_p && _M_offset == __i._M_offset; } // :3

    bool // :3
    operator<(const _Bit_iterator_base& __i) const // :3
    { // :3
      return _M_p < __i._M_p // :3
	     || (_M_p == __i._M_p && _M_offset < __i._M_offset); // :3
    } // :3

    bool // :3
    operator!=(const _Bit_iterator_base& __i) const // :3
    { return !(*this == __i); } // :3

    bool // :3
    operator>(const _Bit_iterator_base& __i) const // :3
    { return __i < *this; } // :3

    bool // :3
    operator<=(const _Bit_iterator_base& __i) const // :3
    { return !(__i < *this); } // :3

    bool // :3
    operator>=(const _Bit_iterator_base& __i) const // :3
    { return !(*this < __i); } // :3
  }; // :3

  inline ptrdiff_t // :3
  operator-(const _Bit_iterator_base& __x, const _Bit_iterator_base& __y) // :3
  { // :3
    return (int(_S_word_bit) * (__x._M_p - __y._M_p) // :3
	    + __x._M_offset - __y._M_offset); // :3
  } // :3

  struct _Bit_iterator : public _Bit_iterator_base // :3
  { // :3
    typedef _Bit_reference  reference; // :3
    typedef _Bit_reference* pointer; // :3
    typedef _Bit_iterator   iterator; // :3

    _Bit_iterator() : _Bit_iterator_base(0, 0) { } // :3

    _Bit_iterator(_Bit_type * __x, unsigned int __y) // :3
    : _Bit_iterator_base(__x, __y) { } // :3

    iterator // :3
    _M_const_cast() const // :3
    { return *this; } // :3

    reference // :3
    operator*() const // :3
    { return reference(_M_p, 1UL << _M_offset); } // :3

    iterator& // :3
    operator++() // :3
    { // :3
      _M_bump_up(); // :3
      return *this; // :3
    } // :3

    iterator // :3
    operator++(int) // :3
    { // :3
      iterator __tmp = *this; // :3
      _M_bump_up(); // :3
      return __tmp; // :3
    } // :3

    iterator& // :3
    operator--() // :3
    { // :3
      _M_bump_down(); // :3
      return *this; // :3
    } // :3

    iterator // :3
    operator--(int) // :3
    { // :3
      iterator __tmp = *this; // :3
      _M_bump_down(); // :3
      return __tmp; // :3
    } // :3

    iterator& // :3
    operator+=(difference_type __i) // :3
    { // :3
      _M_incr(__i); // :3
      return *this; // :3
    } // :3

    iterator& // :3
    operator-=(difference_type __i) // :3
    { // :3
      *this += -__i; // :3
      return *this; // :3
    } // :3

    iterator // :3
    operator+(difference_type __i) const // :3
    { // :3
      iterator __tmp = *this; // :3
      return __tmp += __i; // :3
    } // :3

    iterator // :3
    operator-(difference_type __i) const // :3
    { // :3
      iterator __tmp = *this; // :3
      return __tmp -= __i; // :3
    } // :3

    reference // :3
    operator[](difference_type __i) const // :3
    { return *(*this + __i); } // :3
  }; // :3

  inline _Bit_iterator // :3
  operator+(ptrdiff_t __n, const _Bit_iterator& __x) // :3
  { return __x + __n; } // :3

  struct _Bit_const_iterator : public _Bit_iterator_base // :3
  { // :3
    typedef bool                 reference; // :3
    typedef bool                 const_reference; // :3
    typedef const bool*          pointer; // :3
    typedef _Bit_const_iterator  const_iterator; // :3

    _Bit_const_iterator() : _Bit_iterator_base(0, 0) { } // :3

    _Bit_const_iterator(_Bit_type * __x, unsigned int __y) // :3
    : _Bit_iterator_base(__x, __y) { } // :3

    _Bit_const_iterator(const _Bit_iterator& __x) // :3
    : _Bit_iterator_base(__x._M_p, __x._M_offset) { } // :3

    _Bit_iterator // :3
    _M_const_cast() const // :3
    { return _Bit_iterator(_M_p, _M_offset); } // :3

    const_reference // :3
    operator*() const // :3
    { return _Bit_reference(_M_p, 1UL << _M_offset); } // :3

    const_iterator& // :3
    operator++() // :3
    { // :3
      _M_bump_up(); // :3
      return *this; // :3
    } // :3

    const_iterator // :3
    operator++(int) // :3
    { // :3
      const_iterator __tmp = *this; // :3
      _M_bump_up(); // :3
      return __tmp; // :3
    } // :3

    const_iterator& // :3
    operator--() // :3
    { // :3
      _M_bump_down(); // :3
      return *this; // :3
    } // :3

    const_iterator // :3
    operator--(int) // :3
    { // :3
      const_iterator __tmp = *this; // :3
      _M_bump_down(); // :3
      return __tmp; // :3
    } // :3

    const_iterator& // :3
    operator+=(difference_type __i) // :3
    { // :3
      _M_incr(__i); // :3
      return *this; // :3
    } // :3

    const_iterator& // :3
    operator-=(difference_type __i) // :3
    { // :3
      *this += -__i; // :3
      return *this; // :3
    } // :3

    const_iterator // :3
    operator+(difference_type __i) const // :3
    { // :3
      const_iterator __tmp = *this; // :3
      return __tmp += __i; // :3
    } // :3

    const_iterator // :3
    operator-(difference_type __i) const // :3
    { // :3
      const_iterator __tmp = *this; // :3
      return __tmp -= __i; // :3
    } // :3

    const_reference // :3
    operator[](difference_type __i) const // :3
    { return *(*this + __i); } // :3
  }; // :3

  inline _Bit_const_iterator // :3
  operator+(ptrdiff_t __n, const _Bit_const_iterator& __x) // :3
  { return __x + __n; } // :3

  inline void // :3
  __fill_bvector(_Bit_iterator __first, _Bit_iterator __last, bool __x) // :3
  { // :3
    for (; __first != __last; ++__first) // :3
      *__first = __x; // :3
  } // :3

  inline void // :3
  fill(_Bit_iterator __first, _Bit_iterator __last, const bool& __x) // :3
  { // :3
    if (__first._M_p != __last._M_p) // :3
      { // :3
	fill(__first._M_p + 1, __last._M_p, __x ? ~0 : 0); // :3
	__fill_bvector(__first, _Bit_iterator(__first._M_p + 1, 0), __x); // :3
	__fill_bvector(_Bit_iterator(__last._M_p, 0), __last, __x); // :3
      } // :3
    else // :3
      __fill_bvector(__first, __last, __x); // :3
  } // :3

  template<typename _Alloc> // :3
    struct _Bvector_base // :3
    { // :3
      typedef typename _Alloc::template rebind<_Bit_type>::other // :3
        _Bit_alloc_type; // :3

      struct _Bvector_impl // :3
      : public _Bit_alloc_type // :3
      { // :3
	_Bit_iterator 	_M_start; // :3
	_Bit_iterator 	_M_finish; // :3
	_Bit_type* 	_M_end_of_storage; // :3

	_Bvector_impl() // :3
	: _Bit_alloc_type(), _M_start(), _M_finish(), _M_end_of_storage(0) // :3
	{ } // :3

	_Bvector_impl(const _Bit_alloc_type& __a) // :3
	: _Bit_alloc_type(__a), _M_start(), _M_finish(), _M_end_of_storage(0) // :3
	{ } // :3

#if __cplusplus >= 201103L // :3
	_Bvector_impl(_Bit_alloc_type&& __a) // :3
	: _Bit_alloc_type(std::move(__a)), _M_start(), _M_finish(), // :3
	  _M_end_of_storage(0) // :3
	{ } // :3
#endif // :3
      }; // :3

    public: // :3
      typedef _Alloc allocator_type; // :3

      _Bit_alloc_type& // :3
      _M_get_Bit_allocator() _GLIBCXX_NOEXCEPT // :3
      { return *static_cast<_Bit_alloc_type*>(&this->_M_impl); } // :3

      const _Bit_alloc_type& // :3
      _M_get_Bit_allocator() const _GLIBCXX_NOEXCEPT // :3
      { return *static_cast<const _Bit_alloc_type*>(&this->_M_impl); } // :3

      allocator_type // :3
      get_allocator() const _GLIBCXX_NOEXCEPT // :3
      { return allocator_type(_M_get_Bit_allocator()); } // :3

      _Bvector_base() // :3
      : _M_impl() { } // :3

      _Bvector_base(const allocator_type& __a) // :3
      : _M_impl(__a) { } // :3

#if __cplusplus >= 201103L // :3
      _Bvector_base(_Bvector_base&& __x) noexcept // :3
      : _M_impl(std::move(__x._M_get_Bit_allocator())) // :3
      { // :3
	this->_M_impl._M_start = __x._M_impl._M_start; // :3
	this->_M_impl._M_finish = __x._M_impl._M_finish; // :3
	this->_M_impl._M_end_of_storage = __x._M_impl._M_end_of_storage; // :3
	__x._M_impl._M_start = _Bit_iterator(); // :3
	__x._M_impl._M_finish = _Bit_iterator(); // :3
	__x._M_impl._M_end_of_storage = 0; // :3
      } // :3
#endif // :3

      ~_Bvector_base() // :3
      { this->_M_deallocate(); } // :3

    protected: // :3
      _Bvector_impl _M_impl; // :3

      _Bit_type* // :3
      _M_allocate(size_t __n) // :3
      { return _M_impl.allocate(_S_nword(__n)); } // :3

      void // :3
      _M_deallocate() // :3
      { // :3
	if (_M_impl._M_start._M_p) // :3
	  _M_impl.deallocate(_M_impl._M_start._M_p, // :3
			     _M_impl._M_end_of_storage - _M_impl._M_start._M_p); // :3
      } // :3

      static size_t // :3
      _S_nword(size_t __n) // :3
      { return (__n + int(_S_word_bit) - 1) / int(_S_word_bit); } // :3
    }; // :3

_GLIBCXX_END_NAMESPACE_CONTAINER // :3
} // namespace std :3

// Declare a partial specialization of vector<T, Alloc>. :3
#include "stl_vector.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER // :3

  /**
   *  @brief  A specialization of vector for booleans which offers fixed time
   *  access to individual elements in any order.
   *
   *  @ingroup sequences
   *
   *  @tparam _Alloc  Allocator type.
   *
   *  Note that vector<bool> does not actually meet the requirements for being
   *  a container.  This is because the reference and pointer types are not
   *  really references and pointers to bool.  See DR96 for details.  @see
   *  vector for function documentation.
   *
   *  In some terminology a %vector can be described as a dynamic
   *  C-style array, it offers fast and efficient access to individual
   *  elements in any order and saves the user from worrying about
   *  memory and size allocation.  Subscripting ( @c [] ) access is
   *  also provided as with C-style arrays.
  */ // :3
template<typename _Alloc> // :3
  class vector<bool, _Alloc> : protected _Bvector_base<_Alloc> // :3
  { // :3
    typedef _Bvector_base<_Alloc>			 _Base; // :3

#if __cplusplus >= 201103L // :3
    template<typename> friend struct hash; // :3
#endif // :3

  public: // :3
    typedef bool                                         value_type; // :3
    typedef size_t                                       size_type; // :3
    typedef ptrdiff_t                                    difference_type; // :3
    typedef _Bit_reference                               reference; // :3
    typedef bool                                         const_reference; // :3
    typedef _Bit_reference*                              pointer; // :3
    typedef const bool*                                  const_pointer; // :3
    typedef _Bit_iterator                                iterator; // :3
    typedef _Bit_const_iterator                          const_iterator; // :3
    typedef reverse_iterator<const_iterator>        const_reverse_iterator; // :3
    typedef reverse_iterator<iterator>              reverse_iterator; // :3
    typedef _Alloc                        		 allocator_type; // :3

    allocator_type get_allocator() const // :3
    { return _Base::get_allocator(); } // :3

  protected: // :3
    using _Base::_M_allocate; // :3
    using _Base::_M_deallocate; // :3
    using _Base::_S_nword; // :3
    using _Base::_M_get_Bit_allocator; // :3

  public: // :3
    vector() // :3
    : _Base() { } // :3

    explicit // :3
    vector(const allocator_type& __a) // :3
    : _Base(__a) { } // :3

#if __cplusplus >= 201103L // :3
    explicit // :3
    vector(size_type __n, const allocator_type& __a = allocator_type()) // :3
    : vector(__n, false, __a) // :3
    { } // :3

    vector(size_type __n, const bool& __value, // :3
	   const allocator_type& __a = allocator_type()) // :3
    : _Base(__a) // :3
    { // :3
      _M_initialize(__n); // :3
      fill(this->_M_impl._M_start._M_p, this->_M_impl._M_end_of_storage, // :3
		__value ? ~0 : 0); // :3
    } // :3
#else // :3
    explicit // :3
    vector(size_type __n, const bool& __value = bool(), // :3
	   const allocator_type& __a = allocator_type()) // :3
    : _Base(__a) // :3
    { // :3
      _M_initialize(__n); // :3
      fill(this->_M_impl._M_start._M_p, this->_M_impl._M_end_of_storage, // :3
		__value ? ~0 : 0); // :3
    } // :3
#endif // :3

    vector(const vector& __x) // :3
    : _Base(__x._M_get_Bit_allocator()) // :3
    { // :3
      _M_initialize(__x.size()); // :3
      _M_copy_aligned(__x.begin(), __x.end(), this->_M_impl._M_start); // :3
    } // :3

#if __cplusplus >= 201103L // :3
    vector(vector&& __x) noexcept // :3
    : _Base(std::move(__x)) { } // :3

    vector(initializer_list<bool> __l, // :3
	   const allocator_type& __a = allocator_type()) // :3
    : _Base(__a) // :3
    { // :3
      _M_initialize_range(__l.begin(), __l.end(), // :3
			  random_access_iterator_tag()); // :3
    } // :3
#endif // :3

#if __cplusplus >= 201103L // :3
    template<typename _InputIterator, // :3
	     typename = _RequireInputIter<_InputIterator>> // :3
      vector(_InputIterator __first, _InputIterator __last, // :3
	     const allocator_type& __a = allocator_type()) // :3
      : _Base(__a) // :3
      { _M_initialize_dispatch(__first, __last, __false_type()); } // :3
#else // :3
    template<typename _InputIterator> // :3
      vector(_InputIterator __first, _InputIterator __last, // :3
	     const allocator_type& __a = allocator_type()) // :3
      : _Base(__a) // :3
      { // :3
	typedef typename std::__is_integer<_InputIterator>::__type _Integral; // :3
	_M_initialize_dispatch(__first, __last, _Integral()); // :3
      } // :3
#endif // :3

    ~vector() _GLIBCXX_NOEXCEPT { } // :3

    vector& // :3
    operator=(const vector& __x) // :3
    { // :3
      if (&__x == this) // :3
	return *this; // :3
      if (__x.size() > capacity()) // :3
	{ // :3
	  this->_M_deallocate(); // :3
	  _M_initialize(__x.size()); // :3
	} // :3
      this->_M_impl._M_finish = _M_copy_aligned(__x.begin(), __x.end(), // :3
						begin()); // :3
      return *this; // :3
    } // :3

#if __cplusplus >= 201103L // :3
    vector& // :3
    operator=(vector&& __x) // :3
    { // :3
      // NB: DR 1204. :3
      // NB: DR 675. :3
      this->clear(); // :3
      this->swap(__x); // :3
      return *this; // :3
    } // :3

    vector& // :3
    operator=(initializer_list<bool> __l) // :3
    { // :3
      this->assign (__l.begin(), __l.end()); // :3
      return *this; // :3
    } // :3
#endif // :3

    // assign(), a generalized assignment member function.  Two :3
    // versions: one that takes a count, and one that takes a range. :3
    // The range version is a member template, so we dispatch on whether :3
    // or not the type is an integer. :3
    void // :3
    assign(size_type __n, const bool& __x) // :3
    { _M_fill_assign(__n, __x); } // :3

#if __cplusplus >= 201103L // :3
    template<typename _InputIterator, // :3
	     typename = _RequireInputIter<_InputIterator>> // :3
      void // :3
      assign(_InputIterator __first, _InputIterator __last) // :3
      { _M_assign_dispatch(__first, __last, __false_type()); } // :3
#else // :3
    template<typename _InputIterator> // :3
      void // :3
      assign(_InputIterator __first, _InputIterator __last) // :3
      { // :3
	typedef typename std::__is_integer<_InputIterator>::__type _Integral; // :3
	_M_assign_dispatch(__first, __last, _Integral()); // :3
      } // :3
#endif // :3

#if __cplusplus >= 201103L // :3
    void // :3
    assign(initializer_list<bool> __l) // :3
    { this->assign(__l.begin(), __l.end()); } // :3
#endif // :3

    iterator // :3
    begin() _GLIBCXX_NOEXCEPT // :3
    { return this->_M_impl._M_start; } // :3

    const_iterator // :3
    begin() const _GLIBCXX_NOEXCEPT // :3
    { return this->_M_impl._M_start; } // :3

    iterator // :3
    end() _GLIBCXX_NOEXCEPT // :3
    { return this->_M_impl._M_finish; } // :3

    const_iterator // :3
    end() const _GLIBCXX_NOEXCEPT // :3
    { return this->_M_impl._M_finish; } // :3

    reverse_iterator // :3
    rbegin() _GLIBCXX_NOEXCEPT // :3
    { return reverse_iterator(end()); } // :3

    const_reverse_iterator // :3
    rbegin() const _GLIBCXX_NOEXCEPT // :3
    { return const_reverse_iterator(end()); } // :3

    reverse_iterator // :3
    rend() _GLIBCXX_NOEXCEPT // :3
    { return reverse_iterator(begin()); } // :3

    const_reverse_iterator // :3
    rend() const _GLIBCXX_NOEXCEPT // :3
    { return const_reverse_iterator(begin()); } // :3

#if __cplusplus >= 201103L // :3
    const_iterator // :3
    cbegin() const noexcept // :3
    { return this->_M_impl._M_start; } // :3

    const_iterator // :3
    cend() const noexcept // :3
    { return this->_M_impl._M_finish; } // :3

    const_reverse_iterator // :3
    crbegin() const noexcept // :3
    { return const_reverse_iterator(end()); } // :3

    const_reverse_iterator // :3
    crend() const noexcept // :3
    { return const_reverse_iterator(begin()); } // :3
#endif // :3

    size_type // :3
    size() const _GLIBCXX_NOEXCEPT // :3
    { return size_type(end() - begin()); } // :3

    size_type // :3
    max_size() const _GLIBCXX_NOEXCEPT // :3
    { // :3
      const size_type __isize = // :3
	__gnu_cxx::__numeric_traits<difference_type>::__max // :3
	- int(_S_word_bit) + 1; // :3
      const size_type __asize = _M_get_Bit_allocator().max_size(); // :3
      return (__asize <= __isize / int(_S_word_bit) // :3
	      ? __asize * int(_S_word_bit) : __isize); // :3
    } // :3

    size_type // :3
    capacity() const _GLIBCXX_NOEXCEPT // :3
    { return size_type(const_iterator(this->_M_impl._M_end_of_storage, 0) // :3
		       - begin()); } // :3

    bool // :3
    empty() const _GLIBCXX_NOEXCEPT // :3
    { return begin() == end(); } // :3

    reference // :3
    operator[](size_type __n) // :3
    { // :3
      return *iterator(this->_M_impl._M_start._M_p // :3
		       + __n / int(_S_word_bit), __n % int(_S_word_bit)); // :3
    } // :3

    const_reference // :3
    operator[](size_type __n) const // :3
    { // :3
      return *const_iterator(this->_M_impl._M_start._M_p // :3
			     + __n / int(_S_word_bit), __n % int(_S_word_bit)); // :3
    } // :3

  protected: // :3
    void // :3
    _M_range_check(size_type __n) const // :3
    { // :3
      if (__n >= this->size()) // :3
	__throw_out_of_range_fmt(__N("vector<bool>::_M_range_check: __n " // :3
				     "(which is %zu) >= this->size() " // :3
				     "(which is %zu)"), // :3
				 __n, this->size()); // :3
    } // :3

  public: // :3
    reference // :3
    at(size_type __n) // :3
    { _M_range_check(__n); return (*this)[__n]; } // :3

    const_reference // :3
    at(size_type __n) const // :3
    { _M_range_check(__n); return (*this)[__n]; } // :3

    void // :3
    reserve(size_type __n) // :3
    { // :3
      if (__n > max_size()) // :3
	__throw_length_error(__N("vector::reserve")); // :3
      if (capacity() < __n) // :3
	_M_reallocate(__n); // :3
    } // :3

    reference // :3
    front() // :3
    { return *begin(); } // :3

    const_reference // :3
    front() const // :3
    { return *begin(); } // :3

    reference // :3
    back() // :3
    { return *(end() - 1); } // :3

    const_reference // :3
    back() const // :3
    { return *(end() - 1); } // :3

    // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
    // DR 464. Suggestion for new member functions in standard containers. :3
    // N.B. DR 464 says nothing about vector<bool> but we need something :3
    // here due to the way we are implementing DR 464 in the debug-mode :3
    // vector class. :3
    void // :3
    data() _GLIBCXX_NOEXCEPT { } // :3

    void // :3
    push_back(bool __x) // :3
    { // :3
      if (this->_M_impl._M_finish._M_p != this->_M_impl._M_end_of_storage) // :3
        *this->_M_impl._M_finish++ = __x; // :3
      else // :3
        _M_insert_aux(end(), __x); // :3
    } // :3

    void // :3
    swap(vector& __x) // :3
    { // :3
      swap(this->_M_impl._M_start, __x._M_impl._M_start); // :3
      swap(this->_M_impl._M_finish, __x._M_impl._M_finish); // :3
      swap(this->_M_impl._M_end_of_storage, // :3
		__x._M_impl._M_end_of_storage); // :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 431. Swapping containers with unequal allocators. :3
      __alloc_swap<typename _Base::_Bit_alloc_type>:: // :3
	_S_do_it(_M_get_Bit_allocator(), __x._M_get_Bit_allocator()); // :3
    } // :3

    // [23.2.5]/1, third-to-last entry in synopsis listing :3
    static void // :3
    swap(reference __x, reference __y) _GLIBCXX_NOEXCEPT // :3
    { // :3
      bool __tmp = __x; // :3
      __x = __y; // :3
      __y = __tmp; // :3
    } // :3

    iterator // :3
#if __cplusplus >= 201103L // :3
    insert(const_iterator __position, const bool& __x = bool()) // :3
#else // :3
    insert(iterator __position, const bool& __x = bool()) // :3
#endif // :3
    { // :3
      const difference_type __n = __position - begin(); // :3
      if (this->_M_impl._M_finish._M_p != this->_M_impl._M_end_of_storage // :3
	  && __position == end()) // :3
        *this->_M_impl._M_finish++ = __x; // :3
      else // :3
        _M_insert_aux(__position._M_const_cast(), __x); // :3
      return begin() + __n; // :3
    } // :3

#if __cplusplus >= 201103L // :3
    template<typename _InputIterator, // :3
	     typename = _RequireInputIter<_InputIterator>> // :3
      iterator // :3
      insert(const_iterator __position, // :3
	     _InputIterator __first, _InputIterator __last) // :3
      { // :3
	difference_type __offset = __position - cbegin(); // :3
	_M_insert_dispatch(__position._M_const_cast(), // :3
			   __first, __last, __false_type()); // :3
	return begin() + __offset; // :3
      } // :3
#else // :3
    template<typename _InputIterator> // :3
      void // :3
      insert(iterator __position, // :3
	     _InputIterator __first, _InputIterator __last) // :3
      { // :3
	typedef typename std::__is_integer<_InputIterator>::__type _Integral; // :3
	_M_insert_dispatch(__position, __first, __last, _Integral()); // :3
      } // :3
#endif // :3

#if __cplusplus >= 201103L // :3
    iterator // :3
    insert(const_iterator __position, size_type __n, const bool& __x) // :3
    { // :3
      difference_type __offset = __position - cbegin(); // :3
      _M_fill_insert(__position._M_const_cast(), __n, __x); // :3
      return begin() + __offset; // :3
    } // :3
#else // :3
    void // :3
    insert(iterator __position, size_type __n, const bool& __x) // :3
    { _M_fill_insert(__position, __n, __x); } // :3
#endif // :3

#if __cplusplus >= 201103L // :3
    iterator // :3
    insert(const_iterator __p, initializer_list<bool> __l) // :3
    { return this->insert(__p, __l.begin(), __l.end()); } // :3
#endif // :3

    void // :3
    pop_back() // :3
    { --this->_M_impl._M_finish; } // :3

    iterator // :3
#if __cplusplus >= 201103L // :3
    erase(const_iterator __position) // :3
#else // :3
    erase(iterator __position) // :3
#endif // :3
    { return _M_erase(__position._M_const_cast()); } // :3

    iterator // :3
#if __cplusplus >= 201103L // :3
    erase(const_iterator __first, const_iterator __last) // :3
#else // :3
    erase(iterator __first, iterator __last) // :3
#endif // :3
    { return _M_erase(__first._M_const_cast(), __last._M_const_cast()); } // :3

    void // :3
    resize(size_type __new_size, bool __x = bool()) // :3
    { // :3
      if (__new_size < size()) // :3
        _M_erase_at_end(begin() + difference_type(__new_size)); // :3
      else // :3
        insert(end(), __new_size - size(), __x); // :3
    } // :3

#if __cplusplus >= 201103L // :3
    void // :3
    shrink_to_fit() // :3
    { _M_shrink_to_fit(); } // :3
#endif // :3

    void // :3
    flip() _GLIBCXX_NOEXCEPT // :3
    { // :3
      for (_Bit_type * __p = this->_M_impl._M_start._M_p; // :3
	   __p != this->_M_impl._M_end_of_storage; ++__p) // :3
        *__p = ~*__p; // :3
    } // :3

    void // :3
    clear() _GLIBCXX_NOEXCEPT // :3
    { _M_erase_at_end(begin()); } // :3

#if __cplusplus >= 201103L // :3
    template<typename... _Args> // :3
      void // :3
      emplace_back(_Args&&... __args) // :3
      { push_back(bool(__args...)); } // :3

    template<typename... _Args> // :3
      iterator // :3
      emplace(const_iterator __pos, _Args&&... __args) // :3
      { return insert(__pos, bool(__args...)); } // :3
#endif // :3

  protected: // :3
    // Precondition: __first._M_offset == 0 && __result._M_offset == 0. :3
    iterator // :3
    _M_copy_aligned(const_iterator __first, const_iterator __last, // :3
		    iterator __result) // :3
    { // :3
      _Bit_type* __q = std::copy(__first._M_p, __last._M_p, __result._M_p); // :3
      return std::copy(const_iterator(__last._M_p, 0), __last, // :3
		       iterator(__q, 0)); // :3
    } // :3

    void // :3
    _M_initialize(size_type __n) // :3
    { // :3
      _Bit_type* __q = this->_M_allocate(__n); // :3
      this->_M_impl._M_end_of_storage = __q + _S_nword(__n); // :3
      this->_M_impl._M_start = iterator(__q, 0); // :3
      this->_M_impl._M_finish = this->_M_impl._M_start + difference_type(__n); // :3
    } // :3

    void // :3
    _M_reallocate(size_type __n); // :3

#if __cplusplus >= 201103L // :3
    bool // :3
    _M_shrink_to_fit(); // :3
#endif // :3

    // Check whether it's an integral type.  If so, it's not an iterator. :3

    // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
    // 438. Ambiguity in the "do the right thing" clause :3
    template<typename _Integer> // :3
      void // :3
      _M_initialize_dispatch(_Integer __n, _Integer __x, __true_type) // :3
      { // :3
	_M_initialize(static_cast<size_type>(__n)); // :3
	fill(this->_M_impl._M_start._M_p, // :3
		  this->_M_impl._M_end_of_storage, __x ? ~0 : 0); // :3
      } // :3

    template<typename _InputIterator> // :3
      void // :3
      _M_initialize_dispatch(_InputIterator __first, _InputIterator __last, // :3
			     __false_type) // :3
      { _M_initialize_range(__first, __last, // :3
			    __iterator_category(__first)); } // :3

    template<typename _InputIterator> // :3
      void // :3
      _M_initialize_range(_InputIterator __first, _InputIterator __last, // :3
			  input_iterator_tag) // :3
      { // :3
	for (; __first != __last; ++__first) // :3
	  push_back(*__first); // :3
      } // :3

    template<typename _ForwardIterator> // :3
      void // :3
      _M_initialize_range(_ForwardIterator __first, _ForwardIterator __last, // :3
			  forward_iterator_tag) // :3
      { // :3
	const size_type __n = std::distance(__first, __last); // :3
	_M_initialize(__n); // :3
	std::copy(__first, __last, this->_M_impl._M_start); // :3
      } // :3

    // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
    // 438. Ambiguity in the "do the right thing" clause :3
    template<typename _Integer> // :3
      void // :3
      _M_assign_dispatch(_Integer __n, _Integer __val, __true_type) // :3
      { _M_fill_assign(__n, __val); } // :3

    template<class _InputIterator> // :3
      void // :3
      _M_assign_dispatch(_InputIterator __first, _InputIterator __last, // :3
			 __false_type) // :3
      { _M_assign_aux(__first, __last, __iterator_category(__first)); } // :3

    void // :3
    _M_fill_assign(size_t __n, bool __x) // :3
    { // :3
      if (__n > size()) // :3
	{ // :3
	  fill(this->_M_impl._M_start._M_p, // :3
		    this->_M_impl._M_end_of_storage, __x ? ~0 : 0); // :3
	  insert(end(), __n - size(), __x); // :3
	} // :3
      else // :3
	{ // :3
	  _M_erase_at_end(begin() + __n); // :3
	  fill(this->_M_impl._M_start._M_p, // :3
		    this->_M_impl._M_end_of_storage, __x ? ~0 : 0); // :3
	} // :3
    } // :3

    template<typename _InputIterator> // :3
      void // :3
      _M_assign_aux(_InputIterator __first, _InputIterator __last, // :3
		    input_iterator_tag) // :3
      { // :3
	iterator __cur = begin(); // :3
	for (; __first != __last && __cur != end(); ++__cur, ++__first) // :3
	  *__cur = *__first; // :3
	if (__first == __last) // :3
	  _M_erase_at_end(__cur); // :3
	else // :3
	  insert(end(), __first, __last); // :3
      } // :3

    template<typename _ForwardIterator> // :3
      void // :3
      _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last, // :3
		    forward_iterator_tag) // :3
      { // :3
	const size_type __len = std::distance(__first, __last); // :3
	if (__len < size()) // :3
	  _M_erase_at_end(std::copy(__first, __last, begin())); // :3
	else // :3
	  { // :3
	    _ForwardIterator __mid = __first; // :3
	    std::advance(__mid, size()); // :3
	    std::copy(__first, __mid, begin()); // :3
	    insert(end(), __mid, __last); // :3
	  } // :3
      } // :3

    // Check whether it's an integral type.  If so, it's not an iterator. :3

    // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
    // 438. Ambiguity in the "do the right thing" clause :3
    template<typename _Integer> // :3
      void // :3
      _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x, // :3
			 __true_type) // :3
      { _M_fill_insert(__pos, __n, __x); } // :3

    template<typename _InputIterator> // :3
      void // :3
      _M_insert_dispatch(iterator __pos, // :3
			 _InputIterator __first, _InputIterator __last, // :3
			 __false_type) // :3
      { _M_insert_range(__pos, __first, __last, // :3
			__iterator_category(__first)); } // :3

    void // :3
    _M_fill_insert(iterator __position, size_type __n, bool __x); // :3

    template<typename _InputIterator> // :3
      void // :3
      _M_insert_range(iterator __pos, _InputIterator __first, // :3
		      _InputIterator __last, input_iterator_tag) // :3
      { // :3
	for (; __first != __last; ++__first) // :3
	  { // :3
	    __pos = insert(__pos, *__first); // :3
	    ++__pos; // :3
	  } // :3
      } // :3

    template<typename _ForwardIterator> // :3
      void // :3
      _M_insert_range(iterator __position, _ForwardIterator __first, // :3
		      _ForwardIterator __last, forward_iterator_tag); // :3

    void // :3
    _M_insert_aux(iterator __position, bool __x); // :3

    size_type // :3
    _M_check_len(size_type __n, const char* __s) const // :3
    { // :3
      if (max_size() - size() < __n) // :3
	__throw_length_error(__N(__s)); // :3

      const size_type __len = size() + std::max(size(), __n); // :3
      return (__len < size() || __len > max_size()) ? max_size() : __len; // :3
    } // :3

    void // :3
    _M_erase_at_end(iterator __pos) // :3
    { this->_M_impl._M_finish = __pos; } // :3

    iterator // :3
    _M_erase(iterator __pos); // :3

    iterator // :3
    _M_erase(iterator __first, iterator __last); // :3
  }; // :3

_GLIBCXX_END_NAMESPACE_CONTAINER // :3
} // namespace std :3

#include "functional_hash.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  // DR 1182. :3
  /// std::hash specialization for vector<bool>. :3
  template<typename _Alloc> // :3
    struct hash<vector<bool, _Alloc>> // :3
    : public __hash_base<size_t, vector<bool, _Alloc>> // :3
    { // :3
      size_t // :3
      operator()(const vector<bool, _Alloc>&) const noexcept; // :3
    }; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
}// namespace std :3
