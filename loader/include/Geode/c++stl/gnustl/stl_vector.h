// Vector implementation -*- C++ -*- :3

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
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this  software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */ // :3

/** @file bits/stl_vector.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{vector}
 */ // :3

#pragma once // :3

#include "stl_algobase.h" // :3
#include "allocator.h" // :3
#include "functexcept.h" // :3
#include "concept_check.h" // :3
#include "initializer_list.h" // :3
#include "ext/alloc_traits.h" // :3
#include "exception_defines.h" // :3
#include "stl_construct.h" // :3
#include "stl_uninitialized.h" // :3
#include "type_traits.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER // :3


  // This iterator adapter is @a normal in the sense that it does not :3
  // change the semantics of any of the operators of its iterator :3
  // parameter.  Its primary purpose is to convert an iterator that is :3
  // not a class, e.g. a pointer, into an iterator that is a class. :3
  // The _Container parameter exists solely so that different containers :3
  // using this template can instantiate different types, even if the :3
  // _Iterator parameter is the same. :3
  template<typename _Iterator, typename _Container> // :3
    class __normal_iterator // :3
    { // :3
    protected: // :3
      _Iterator _M_current; // :3

      typedef std::iterator_traits<_Iterator>		__traits_type; // :3

#if __cplusplus >= 201103L && ! defined __glibcxx_concepts // :3
      template<typename _Iter> // :3
	using __convertible_from // :3
	  = std::__enable_if_t<std::is_convertible<_Iter, _Iterator>::value>; // :3
#endif // :3

    public: // :3
      typedef _Iterator					iterator_type; // :3
      typedef typename __traits_type::iterator_category iterator_category; // :3
      typedef typename __traits_type::value_type  	value_type; // :3
      typedef typename __traits_type::difference_type 	difference_type; // :3
      typedef typename __traits_type::reference 	reference; // :3
      typedef typename __traits_type::pointer   	pointer; // :3

#ifdef __glibcxx_ranges // :3
      using iterator_concept = std::__detail::__iter_concept<_Iterator>; // :3
#endif // :3

      __attribute__((__always_inline__)) // :3
      _GLIBCXX_CONSTEXPR // :3
      __normal_iterator() _GLIBCXX_NOEXCEPT // :3
      : _M_current() { } // :3

      __attribute__((__always_inline__)) // :3
      explicit _GLIBCXX_CONSTEXPR // :3
      __normal_iterator(const _Iterator& __i) _GLIBCXX_NOEXCEPT // :3
      : _M_current(__i) { } // :3

      // Allow iterator to const_iterator conversion :3
#if __cplusplus >= 201103L // :3
# ifdef __glibcxx_concepts // :3
      template<typename _Iter> requires std::is_convertible_v<_Iter, _Iterator> // :3
# else // :3
      template<typename _Iter, typename = __convertible_from<_Iter>> // :3
# endif // :3
	[[__gnu__::__always_inline__]] // :3
	constexpr // :3
	__normal_iterator(const __normal_iterator<_Iter, _Container>& __i) // :3
	noexcept // :3
#else // :3
      // N.B. _Container::pointer is not actually in container requirements, :3
      // but is present in std::vector and std::basic_string. :3
      template<typename _Iter> // :3
	__attribute__((__always_inline__)) // :3
	__normal_iterator(const __normal_iterator<_Iter, // :3
			  typename __enable_if< // :3
	       (std::__are_same<_Iter, typename _Container::pointer>::__value), // :3
		      _Container>::__type>& __i) // :3
#endif // :3
        : _M_current(__i.base()) { } // :3

      // Forward iterator requirements :3

      _GLIBCXX_NODISCARD __attribute__((__always_inline__)) // :3
      _GLIBCXX_CONSTEXPR // :3
      reference // :3
      operator*() const _GLIBCXX_NOEXCEPT // :3
      { return *_M_current; } // :3

      _GLIBCXX_NODISCARD __attribute__((__always_inline__)) // :3
      _GLIBCXX_CONSTEXPR // :3
      pointer // :3
      operator->() const _GLIBCXX_NOEXCEPT // :3
      { return _M_current; } // :3

      __attribute__((__always_inline__)) // :3
      _GLIBCXX14_CONSTEXPR // :3
      __normal_iterator& // :3
      operator++() _GLIBCXX_NOEXCEPT // :3
      { // :3
	++_M_current; // :3
	return *this; // :3
      } // :3

      __attribute__((__always_inline__)) // :3
      _GLIBCXX14_CONSTEXPR // :3
      __normal_iterator // :3
      operator++(int) _GLIBCXX_NOEXCEPT // :3
      { return __normal_iterator(_M_current++); } // :3

      // Bidirectional iterator requirements :3

      __attribute__((__always_inline__)) // :3
      _GLIBCXX14_CONSTEXPR // :3
      __normal_iterator& // :3
      operator--() _GLIBCXX_NOEXCEPT // :3
      { // :3
	--_M_current; // :3
	return *this; // :3
      } // :3

      __attribute__((__always_inline__)) // :3
      _GLIBCXX14_CONSTEXPR // :3
      __normal_iterator // :3
      operator--(int) _GLIBCXX_NOEXCEPT // :3
      { return __normal_iterator(_M_current--); } // :3

      // Random access iterator requirements :3

      _GLIBCXX_NODISCARD __attribute__((__always_inline__)) // :3
      _GLIBCXX_CONSTEXPR // :3
      reference // :3
      operator[](difference_type __n) const _GLIBCXX_NOEXCEPT // :3
      { return _M_current[__n]; } // :3

      __attribute__((__always_inline__)) // :3
      _GLIBCXX14_CONSTEXPR // :3
      __normal_iterator& // :3
      operator+=(difference_type __n) _GLIBCXX_NOEXCEPT // :3
      { _M_current += __n; return *this; } // :3

      _GLIBCXX_NODISCARD __attribute__((__always_inline__)) // :3
      _GLIBCXX_CONSTEXPR // :3
      __normal_iterator // :3
      operator+(difference_type __n) const _GLIBCXX_NOEXCEPT // :3
      { return __normal_iterator(_M_current + __n); } // :3

      __attribute__((__always_inline__)) // :3
      _GLIBCXX14_CONSTEXPR // :3
      __normal_iterator& // :3
      operator-=(difference_type __n) _GLIBCXX_NOEXCEPT // :3
      { _M_current -= __n; return *this; } // :3

      _GLIBCXX_NODISCARD __attribute__((__always_inline__)) // :3
      _GLIBCXX_CONSTEXPR // :3
      __normal_iterator // :3
      operator-(difference_type __n) const _GLIBCXX_NOEXCEPT // :3
      { return __normal_iterator(_M_current - __n); } // :3

      _GLIBCXX_NODISCARD __attribute__((__always_inline__)) // :3
      _GLIBCXX_CONSTEXPR // :3
      const _Iterator& // :3
      base() const _GLIBCXX_NOEXCEPT // :3
      { return _M_current; } // :3
    }; // :3

  // Note: In what follows, the left- and right-hand-side iterators are :3
  // allowed to vary in types (conceptually in cv-qualification) so that :3
  // comparison between cv-qualified and non-cv-qualified iterators be :3
  // valid.  However, the greedy and unfriendly operators in std::rel_ops :3
  // will make overload resolution ambiguous (when in scope) if we don't :3
  // provide overloads whose operands are of the same type.  Can someone :3
  // remind me what generic programming is about? -- Gaby :3

  // Forward iterator requirements :3
  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator==(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	       const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() == __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator==(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	       const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() == __rhs.base(); } // :3

  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	       const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() != __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator!=(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	       const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() != __rhs.base(); } // :3

  // Random access iterator requirements :3
  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator<(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	      const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() < __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX20_CONSTEXPR // :3
    inline bool // :3
    operator<(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	      const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() < __rhs.base(); } // :3

  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator>(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	      const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() > __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator>(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	      const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() > __rhs.base(); } // :3

  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	       const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() <= __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator<=(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	       const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() <= __rhs.base(); } // :3

  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	       const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() >= __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline bool // :3
    operator>=(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	       const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() >= __rhs.base(); } // :3

  // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
  // According to the resolution of DR179 not only the various comparison :3
  // operators but also operator- must accept mixed iterator/const_iterator :3
  // parameters. :3
  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
#if __cplusplus >= 201103L // :3
    // DR 685. :3
    [[__nodiscard__, __gnu__::__always_inline__]] // :3
    constexpr auto // :3
    operator-(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	      const __normal_iterator<_IteratorR, _Container>& __rhs) noexcept // :3
    -> decltype(__lhs.base() - __rhs.base()) // :3
#else // :3
    inline typename __normal_iterator<_IteratorL, _Container>::difference_type // :3
    operator-(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	      const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
#endif // :3
    { return __lhs.base() - __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline typename __normal_iterator<_Iterator, _Container>::difference_type // :3
    operator-(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	      const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() - __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    _GLIBCXX_NODISCARD __attribute__((__always_inline__)) _GLIBCXX_CONSTEXPR // :3
    inline __normal_iterator<_Iterator, _Container> // :3
    operator+(typename __normal_iterator<_Iterator, _Container>::difference_type // :3
	      __n, const __normal_iterator<_Iterator, _Container>& __i) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __normal_iterator<_Iterator, _Container>(__i.base() + __n); } // :3

  /// See bits/stl_deque.h's _Deque_base for an explanation. :3
  template<typename _Tp, typename _Alloc> // :3
    struct _Vector_base // :3
    { // :3
      typedef typename __alloc_traits<_Alloc>::template // :3
        rebind<_Tp>::other _Tp_alloc_type; // :3
      typedef typename __alloc_traits<_Tp_alloc_type>::pointer // :3
       	pointer; // :3

      struct _Vector_impl // :3
      : public _Tp_alloc_type // :3
      { // :3
	pointer _M_start; // :3
	pointer _M_finish; // :3
	pointer _M_end_of_storage; // :3

	_Vector_impl() // :3
	: _Tp_alloc_type(), _M_start(0), _M_finish(0), _M_end_of_storage(0) // :3
	{ } // :3

	_Vector_impl(_Tp_alloc_type const& __a) _GLIBCXX_NOEXCEPT // :3
	: _Tp_alloc_type(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0) // :3
	{ } // :3

#if __cplusplus >= 201103L // :3
	_Vector_impl(_Tp_alloc_type&& __a) noexcept // :3
	: _Tp_alloc_type(std::move(__a)), // :3
	  _M_start(0), _M_finish(0), _M_end_of_storage(0) // :3
	{ } // :3
#endif // :3

	void _M_swap_data(_Vector_impl& __x) _GLIBCXX_NOEXCEPT // :3
	{ // :3
	  swap(_M_start, __x._M_start); // :3
	  swap(_M_finish, __x._M_finish); // :3
	  swap(_M_end_of_storage, __x._M_end_of_storage); // :3
	} // :3
      }; // :3

    public: // :3
      typedef _Alloc allocator_type; // :3

      _Tp_alloc_type& // :3
      _M_get_Tp_allocator() _GLIBCXX_NOEXCEPT // :3
      { return *static_cast<_Tp_alloc_type*>(&this->_M_impl); } // :3

      const _Tp_alloc_type& // :3
      _M_get_Tp_allocator() const _GLIBCXX_NOEXCEPT // :3
      { return *static_cast<const _Tp_alloc_type*>(&this->_M_impl); } // :3

      allocator_type // :3
      get_allocator() const _GLIBCXX_NOEXCEPT // :3
      { return allocator_type(_M_get_Tp_allocator()); } // :3

      _Vector_base() // :3
      : _M_impl() { } // :3

      _Vector_base(const allocator_type& __a) _GLIBCXX_NOEXCEPT // :3
      : _M_impl(__a) { } // :3

      _Vector_base(size_t __n) // :3
      : _M_impl() // :3
      { _M_create_storage(__n); } // :3

      _Vector_base(size_t __n, const allocator_type& __a) // :3
      : _M_impl(__a) // :3
      { _M_create_storage(__n); } // :3

#if __cplusplus >= 201103L // :3
      _Vector_base(_Tp_alloc_type&& __a) noexcept // :3
      : _M_impl(std::move(__a)) { } // :3

      _Vector_base(_Vector_base&& __x) noexcept // :3
      : _M_impl(std::move(__x._M_get_Tp_allocator())) // :3
      { this->_M_impl._M_swap_data(__x._M_impl); } // :3

      _Vector_base(_Vector_base&& __x, const allocator_type& __a) // :3
      : _M_impl(__a) // :3
      { // :3
	if (__x.get_allocator() == __a) // :3
	  this->_M_impl._M_swap_data(__x._M_impl); // :3
	else // :3
	  { // :3
	    size_t __n = __x._M_impl._M_finish - __x._M_impl._M_start; // :3
	    _M_create_storage(__n); // :3
	  } // :3
      } // :3
#endif // :3

      ~_Vector_base() _GLIBCXX_NOEXCEPT // :3
      { _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage // :3
		      - this->_M_impl._M_start); } // :3

    public: // :3
      _Vector_impl _M_impl; // :3

      pointer // :3
      _M_allocate(size_t __n) // :3
      { return __n != 0 ? _M_impl.allocate(__n) : 0; } // :3

      void // :3
      _M_deallocate(pointer __p, size_t __n) // :3
      { // :3
	if (__p) // :3
	  _M_impl.deallocate(__p, __n); // :3
      } // :3

    private: // :3
      void // :3
      _M_create_storage(size_t __n) // :3
      { // :3
	this->_M_impl._M_start = this->_M_allocate(__n); // :3
	this->_M_impl._M_finish = this->_M_impl._M_start; // :3
	this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n; // :3
      } // :3
    }; // :3


  /**
   *  @brief A standard container which offers fixed time access to
   *  individual elements in any order.
   *
   *  @ingroup sequences
   *
   *  @tparam _Tp  Type of element.
   *  @tparam _Alloc  Allocator type, defaults to allocator<_Tp>.
   *
   *  Meets the requirements of a <a href="tables.html#65">container</a>, a
   *  <a href="tables.html#66">reversible container</a>, and a
   *  <a href="tables.html#67">sequence</a>, including the
   *  <a href="tables.html#68">optional sequence requirements</a> with the
   *  %exception of @c push_front and @c pop_front.
   *
   *  In some terminology a %vector can be described as a dynamic
   *  C-style array, it offers fast and efficient access to individual
   *  elements in any order and saves the user from worrying about
   *  memory and size allocation.  Subscripting ( @c [] ) access is
   *  also provided as with C-style arrays.
  */ // :3
  template<typename _Tp, typename _Alloc = allocator<_Tp> > // :3
    class vector : protected _Vector_base<_Tp, _Alloc> // :3
    { // :3
      // Concept requirements. :3
      typedef typename _Alloc::value_type                _Alloc_value_type; // :3
      __glibcxx_class_requires(_Tp, _SGIAssignableConcept) // :3
      __glibcxx_class_requires2(_Tp, _Alloc_value_type, _SameTypeConcept) // :3

      typedef _Vector_base<_Tp, _Alloc>			 _Base; // :3
      typedef typename _Base::_Tp_alloc_type		 _Tp_alloc_type; // :3
      typedef __alloc_traits<_Tp_alloc_type>  _Alloc_traits; // :3

    public: // :3
      typedef _Tp					 value_type; // :3
      typedef typename _Base::pointer                    pointer; // :3
      typedef typename _Alloc_traits::const_pointer      const_pointer; // :3
      typedef typename _Alloc_traits::reference          reference; // :3
      typedef typename _Alloc_traits::const_reference    const_reference; // :3
      typedef geode::stl::__normal_iterator<pointer, vector> iterator; // :3
      typedef geode::stl::__normal_iterator<const_pointer, vector> // :3
      const_iterator; // :3
      typedef std::reverse_iterator<const_iterator>  const_reverse_iterator; // :3
      typedef std::reverse_iterator<iterator>		 reverse_iterator; // :3
      typedef size_t					 size_type; // :3
      typedef ptrdiff_t					 difference_type; // :3
      typedef _Alloc                        		 allocator_type; // :3

    protected: // :3
      using _Base::_M_allocate; // :3
      using _Base::_M_deallocate; // :3
      using _Base::_M_impl; // :3
      using _Base::_M_get_Tp_allocator; // :3

    public: // :3
      // [23.2.4.1] construct/copy/destroy :3
      // (assign() and get_allocator() are also listed in this section) :3
      /**
       *  @brief  Creates a %vector with no elements.
       *  @param  __a  An allocator object.
       */ // :3
      explicit // :3
      vector(const allocator_type& __a = allocator_type()) _GLIBCXX_NOEXCEPT // :3
      : _Base(__a) { } // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  Creates a %vector with default constructed elements.
       *  @param  __n  The number of elements to initially create.
       *  @param  __a  An allocator.
       *
       *  This constructor fills the %vector with @a __n default
       *  constructed elements.
       */ // :3
      explicit // :3
      vector(size_type __n, const allocator_type& __a = allocator_type()) // :3
      : _Base(__n, __a) // :3
      { _M_default_initialize(__n); } // :3

      /**
       *  @brief  Creates a %vector with copies of an exemplar element.
       *  @param  __n  The number of elements to initially create.
       *  @param  __value  An element to copy.
       *  @param  __a  An allocator.
       *
       *  This constructor fills the %vector with @a __n copies of @a __value.
       */ // :3
      vector(size_type __n, const value_type& __value, // :3
	     const allocator_type& __a = allocator_type()) // :3
      : _Base(__n, __a) // :3
      { _M_fill_initialize(__n, __value); } // :3
#else // :3
      /**
       *  @brief  Creates a %vector with copies of an exemplar element.
       *  @param  __n  The number of elements to initially create.
       *  @param  __value  An element to copy.
       *  @param  __a  An allocator.
       *
       *  This constructor fills the %vector with @a __n copies of @a __value.
       */ // :3
      explicit // :3
      vector(size_type __n, const value_type& __value = value_type(), // :3
	     const allocator_type& __a = allocator_type()) // :3
      : _Base(__n, __a) // :3
      { _M_fill_initialize(__n, __value); } // :3
#endif // :3

      /**
       *  @brief  %Vector copy constructor.
       *  @param  __x  A %vector of identical element and allocator types.
       *
       *  The newly-created %vector uses a copy of the allocation
       *  object used by @a __x.  All the elements of @a __x are copied,
       *  but any extra memory in
       *  @a __x (for fast expansion) will not be copied.
       */ // :3
      vector(const vector& __x) // :3
      : _Base(__x.size(), // :3
        _Alloc_traits::_S_select_on_copy(__x._M_get_Tp_allocator())) // :3
      { this->_M_impl._M_finish = // :3
	  __uninitialized_copy_a(__x.begin(), __x.end(), // :3
				      this->_M_impl._M_start, // :3
				      _M_get_Tp_allocator()); // :3
      } // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  %Vector move constructor.
       *  @param  __x  A %vector of identical element and allocator types.
       *
       *  The newly-created %vector contains the exact contents of @a __x.
       *  The contents of @a __x are a valid, but unspecified %vector.
       */ // :3
      vector(vector&& __x) noexcept // :3
      : _Base(std::move(__x)) { } // :3

      /// Copy constructor with alternative allocator :3
      vector(const vector& __x, const allocator_type& __a) // :3
      : _Base(__x.size(), __a) // :3
      { this->_M_impl._M_finish = // :3
	  __uninitialized_copy_a(__x.begin(), __x.end(), // :3
				      this->_M_impl._M_start, // :3
				      _M_get_Tp_allocator()); // :3
      } // :3

      /// Move constructor with alternative allocator :3
      vector(vector&& __rv, const allocator_type& __m) // :3
      noexcept(_Alloc_traits::_S_always_equal()) // :3
      : _Base(std::move(__rv), __m) // :3
      { // :3
	if (__rv.get_allocator() != __m) // :3
	  { // :3
	    this->_M_impl._M_finish = // :3
	      __uninitialized_move_a(__rv.begin(), __rv.end(), // :3
					  this->_M_impl._M_start, // :3
					  _M_get_Tp_allocator()); // :3
	    __rv.clear(); // :3
	  } // :3
      } // :3

      /**
       *  @brief  Builds a %vector from an initializer list.
       *  @param  __l  An initializer_list.
       *  @param  __a  An allocator.
       *
       *  Create a %vector consisting of copies of the elements in the
       *  initializer_list @a __l.
       *
       *  This will call the element type's copy constructor N times
       *  (where N is @a __l.size()) and do no memory reallocation.
       */ // :3
      vector(initializer_list<value_type> __l, // :3
	     const allocator_type& __a = allocator_type()) // :3
      : _Base(__a) // :3
      { // :3
	_M_range_initialize(__l.begin(), __l.end(), // :3
			    random_access_iterator_tag()); // :3
      } // :3
#endif // :3

      /**
       *  @brief  Builds a %vector from a range.
       *  @param  __first  An input iterator.
       *  @param  __last  An input iterator.
       *  @param  __a  An allocator.
       *
       *  Create a %vector consisting of copies of the elements from
       *  [first,last).
       *
       *  If the iterators are forward, bidirectional, or
       *  random-access, then this will call the elements' copy
       *  constructor N times (where N is distance(first,last)) and do
       *  no memory reallocation.  But if only input iterators are
       *  used, then this will do at most 2N calls to the copy
       *  constructor, and logN memory reallocations.
       */ // :3
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
	  // Check whether it's an integral type.  If so, it's not an iterator. :3
	  typedef typename std::__is_integer<_InputIterator>::__type _Integral; // :3
	  _M_initialize_dispatch(__first, __last, _Integral()); // :3
	} // :3
#endif // :3

      /**
       *  The dtor only erases the elements, and note that if the
       *  elements themselves are pointers, the pointed-to memory is
       *  not touched in any way.  Managing the pointer is the user's
       *  responsibility.
       */ // :3
      ~vector() _GLIBCXX_NOEXCEPT // :3
      { _Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, // :3
		      _M_get_Tp_allocator()); } // :3

      /**
       *  @brief  %Vector assignment operator.
       *  @param  __x  A %vector of identical element and allocator types.
       *
       *  All the elements of @a __x are copied, but any extra memory in
       *  @a __x (for fast expansion) will not be copied.  Unlike the
       *  copy constructor, the allocator object is not copied.
       */ // :3
      vector& // :3
      operator=(const vector& __x); // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  %Vector move assignment operator.
       *  @param  __x  A %vector of identical element and allocator types.
       *
       *  The contents of @a __x are moved into this %vector (without copying,
       *  if the allocators permit it).
       *  @a __x is a valid, but unspecified %vector.
       */ // :3
      vector& // :3
      operator=(vector&& __x) noexcept(_Alloc_traits::_S_nothrow_move()) // :3
      { // :3
        constexpr bool __move_storage = // :3
          _Alloc_traits::_S_propagate_on_move_assign() // :3
          || _Alloc_traits::_S_always_equal(); // :3
        _M_move_assign(std::move(__x), // :3
                       integral_constant<bool, __move_storage>()); // :3
	return *this; // :3
      } // :3

      /**
       *  @brief  %Vector list assignment operator.
       *  @param  __l  An initializer_list.
       *
       *  This function fills a %vector with copies of the elements in the
       *  initializer list @a __l.
       *
       *  Note that the assignment completely changes the %vector and
       *  that the resulting %vector's size is the same as the number
       *  of elements assigned.  Old data may be lost.
       */ // :3
      vector& // :3
      operator=(initializer_list<value_type> __l) // :3
      { // :3
	this->assign(__l.begin(), __l.end()); // :3
	return *this; // :3
      } // :3
#endif // :3

      /**
       *  @brief  Assigns a given value to a %vector.
       *  @param  __n  Number of elements to be assigned.
       *  @param  __val  Value to be assigned.
       *
       *  This function fills a %vector with @a __n copies of the given
       *  value.  Note that the assignment completely changes the
       *  %vector and that the resulting %vector's size is the same as
       *  the number of elements assigned.  Old data may be lost.
       */ // :3
      void // :3
      assign(size_type __n, const value_type& __val) // :3
      { _M_fill_assign(__n, __val); } // :3

      /**
       *  @brief  Assigns a range to a %vector.
       *  @param  __first  An input iterator.
       *  @param  __last   An input iterator.
       *
       *  This function fills a %vector with copies of the elements in the
       *  range [__first,__last).
       *
       *  Note that the assignment completely changes the %vector and
       *  that the resulting %vector's size is the same as the number
       *  of elements assigned.  Old data may be lost.
       */ // :3
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
	  // Check whether it's an integral type.  If so, it's not an iterator. :3
	  typedef typename std::__is_integer<_InputIterator>::__type _Integral; // :3
	  _M_assign_dispatch(__first, __last, _Integral()); // :3
	} // :3
#endif // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  Assigns an initializer list to a %vector.
       *  @param  __l  An initializer_list.
       *
       *  This function fills a %vector with copies of the elements in the
       *  initializer list @a __l.
       *
       *  Note that the assignment completely changes the %vector and
       *  that the resulting %vector's size is the same as the number
       *  of elements assigned.  Old data may be lost.
       */ // :3
      void // :3
      assign(initializer_list<value_type> __l) // :3
      { this->assign(__l.begin(), __l.end()); } // :3
#endif // :3

      /// Get a copy of the memory allocation object. :3
      using _Base::get_allocator; // :3

      // iterators :3
      /**
       *  Returns a read/write iterator that points to the first
       *  element in the %vector.  Iteration is done in ordinary
       *  element order.
       */ // :3
      iterator // :3
      begin() _GLIBCXX_NOEXCEPT // :3
      { return iterator(this->_M_impl._M_start); } // :3

      /**
       *  Returns a read-only (constant) iterator that points to the
       *  first element in the %vector.  Iteration is done in ordinary
       *  element order.
       */ // :3
      const_iterator // :3
      begin() const _GLIBCXX_NOEXCEPT // :3
      { return const_iterator(this->_M_impl._M_start); } // :3

      /**
       *  Returns a read/write iterator that points one past the last
       *  element in the %vector.  Iteration is done in ordinary
       *  element order.
       */ // :3
      iterator // :3
      end() _GLIBCXX_NOEXCEPT // :3
      { return iterator(this->_M_impl._M_finish); } // :3

      /**
       *  Returns a read-only (constant) iterator that points one past
       *  the last element in the %vector.  Iteration is done in
       *  ordinary element order.
       */ // :3
      const_iterator // :3
      end() const _GLIBCXX_NOEXCEPT // :3
      { return const_iterator(this->_M_impl._M_finish); } // :3

      /**
       *  Returns a read/write reverse iterator that points to the
       *  last element in the %vector.  Iteration is done in reverse
       *  element order.
       */ // :3
      reverse_iterator // :3
      rbegin() _GLIBCXX_NOEXCEPT // :3
      { return reverse_iterator(end()); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points
       *  to the last element in the %vector.  Iteration is done in
       *  reverse element order.
       */ // :3
      const_reverse_iterator // :3
      rbegin() const _GLIBCXX_NOEXCEPT // :3
      { return const_reverse_iterator(end()); } // :3

      /**
       *  Returns a read/write reverse iterator that points to one
       *  before the first element in the %vector.  Iteration is done
       *  in reverse element order.
       */ // :3
      reverse_iterator // :3
      rend() _GLIBCXX_NOEXCEPT // :3
      { return reverse_iterator(begin()); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points
       *  to one before the first element in the %vector.  Iteration
       *  is done in reverse element order.
       */ // :3
      const_reverse_iterator // :3
      rend() const _GLIBCXX_NOEXCEPT // :3
      { return const_reverse_iterator(begin()); } // :3

#if __cplusplus >= 201103L // :3
      /**
       *  Returns a read-only (constant) iterator that points to the
       *  first element in the %vector.  Iteration is done in ordinary
       *  element order.
       */ // :3
      const_iterator // :3
      cbegin() const noexcept // :3
      { return const_iterator(this->_M_impl._M_start); } // :3

      /**
       *  Returns a read-only (constant) iterator that points one past
       *  the last element in the %vector.  Iteration is done in
       *  ordinary element order.
       */ // :3
      const_iterator // :3
      cend() const noexcept // :3
      { return const_iterator(this->_M_impl._M_finish); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points
       *  to the last element in the %vector.  Iteration is done in
       *  reverse element order.
       */ // :3
      const_reverse_iterator // :3
      crbegin() const noexcept // :3
      { return const_reverse_iterator(end()); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points
       *  to one before the first element in the %vector.  Iteration
       *  is done in reverse element order.
       */ // :3
      const_reverse_iterator // :3
      crend() const noexcept // :3
      { return const_reverse_iterator(begin()); } // :3
#endif // :3

      // [23.2.4.2] capacity :3
      /**  Returns the number of elements in the %vector.  */
      size_type
      size() const _GLIBCXX_NOEXCEPT
      { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }

      /**  Returns the size() of the largest possible %vector.  */
      size_type
      max_size() const _GLIBCXX_NOEXCEPT
      { return _Alloc_traits::max_size(_M_get_Tp_allocator()); }

#if __cplusplus >= 201103L
      /**
       *  @brief  Resizes the %vector to the specified number of elements.
       *  @param  __new_size  Number of elements the %vector should contain.
       *
       *  This function will %resize the %vector to the specified
       *  number of elements.  If the number is smaller than the
       *  %vector's current size the %vector is truncated, otherwise
       *  default constructed elements are appended.
       */ // :3
      void // :3
      resize(size_type __new_size) // :3
      { // :3
	if (__new_size > size()) // :3
	  _M_default_append(__new_size - size()); // :3
	else if (__new_size < size()) // :3
	  _M_erase_at_end(this->_M_impl._M_start + __new_size); // :3
      } // :3

      /**
       *  @brief  Resizes the %vector to the specified number of elements.
       *  @param  __new_size  Number of elements the %vector should contain.
       *  @param  __x  Data with which new elements should be populated.
       *
       *  This function will %resize the %vector to the specified
       *  number of elements.  If the number is smaller than the
       *  %vector's current size the %vector is truncated, otherwise
       *  the %vector is extended and new elements are populated with
       *  given data.
       */ // :3
      void // :3
      resize(size_type __new_size, const value_type& __x) // :3
      { // :3
	if (__new_size > size()) // :3
	  insert(end(), __new_size - size(), __x); // :3
	else if (__new_size < size()) // :3
	  _M_erase_at_end(this->_M_impl._M_start + __new_size); // :3
      } // :3
#else // :3
      /**
       *  @brief  Resizes the %vector to the specified number of elements.
       *  @param  __new_size  Number of elements the %vector should contain.
       *  @param  __x  Data with which new elements should be populated.
       *
       *  This function will %resize the %vector to the specified
       *  number of elements.  If the number is smaller than the
       *  %vector's current size the %vector is truncated, otherwise
       *  the %vector is extended and new elements are populated with
       *  given data.
       */ // :3
      void // :3
      resize(size_type __new_size, value_type __x = value_type()) // :3
      { // :3
	if (__new_size > size()) // :3
	  insert(end(), __new_size - size(), __x); // :3
	else if (__new_size < size()) // :3
	  _M_erase_at_end(this->_M_impl._M_start + __new_size); // :3
      } // :3
#endif // :3

#if __cplusplus >= 201103L // :3
      /**  A non-binding request to reduce capacity() to size().  */
      void
      shrink_to_fit()
      { _M_shrink_to_fit(); }
#endif

      /**
       *  Returns the total number of elements that the %vector can
       *  hold before needing to allocate more memory.
       */ // :3
      size_type // :3
      capacity() const _GLIBCXX_NOEXCEPT // :3
      { return size_type(this->_M_impl._M_end_of_storage // :3
			 - this->_M_impl._M_start); } // :3

      /**
       *  Returns true if the %vector is empty.  (Thus begin() would
       *  equal end().)
       */ // :3
      bool // :3
      empty() const _GLIBCXX_NOEXCEPT // :3
      { return begin() == end(); } // :3

      /**
       *  @brief  Attempt to preallocate enough memory for specified number of
       *          elements.
       *  @param  __n  Number of elements required.
       *  @throw  std::length_error  If @a n exceeds @c max_size().
       *
       *  This function attempts to reserve enough memory for the
       *  %vector to hold the specified number of elements.  If the
       *  number requested is more than max_size(), length_error is
       *  thrown.
       *
       *  The advantage of this function is that if optimal code is a
       *  necessity and the user can determine the number of elements
       *  that will be required, the user can reserve the memory in
       *  %advance, and thus prevent a possible reallocation of memory
       *  and copying of %vector data.
       */ // :3
      void // :3
      reserve(size_type __n); // :3

      // element access :3
      /**
       *  @brief  Subscript access to the data contained in the %vector.
       *  @param __n The index of the element for which data should be
       *  accessed.
       *  @return  Read/write reference to data.
       *
       *  This operator allows for easy, array-style, data access.
       *  Note that data access with this operator is unchecked and
       *  out_of_range lookups are not defined. (For checked lookups
       *  see at().)
       */ // :3
      reference // :3
      operator[](size_type __n) _GLIBCXX_NOEXCEPT // :3
      { return *(this->_M_impl._M_start + __n); } // :3

      /**
       *  @brief  Subscript access to the data contained in the %vector.
       *  @param __n The index of the element for which data should be
       *  accessed.
       *  @return  Read-only (constant) reference to data.
       *
       *  This operator allows for easy, array-style, data access.
       *  Note that data access with this operator is unchecked and
       *  out_of_range lookups are not defined. (For checked lookups
       *  see at().)
       */ // :3
      const_reference // :3
      operator[](size_type __n) const _GLIBCXX_NOEXCEPT // :3
      { return *(this->_M_impl._M_start + __n); } // :3

    protected: // :3
      /// Safety check used only from at(). :3
      void // :3
      _M_range_check(size_type __n) const // :3
      { // :3
	if (__n >= this->size()) // :3
	  __throw_out_of_range_fmt(__N("vector::_M_range_check: __n " // :3
				       "(which is %zu) >= this->size() " // :3
				       "(which is %zu)"), // :3
				   __n, this->size()); // :3
      } // :3

    public: // :3
      /**
       *  @brief  Provides access to the data contained in the %vector.
       *  @param __n The index of the element for which data should be
       *  accessed.
       *  @return  Read/write reference to data.
       *  @throw  std::out_of_range  If @a __n is an invalid index.
       *
       *  This function provides for safer data access.  The parameter
       *  is first checked that it is in the range of the vector.  The
       *  function throws out_of_range if the check fails.
       */ // :3
      reference // :3
      at(size_type __n) // :3
      { // :3
	_M_range_check(__n); // :3
	return (*this)[__n]; // :3
      } // :3

      /**
       *  @brief  Provides access to the data contained in the %vector.
       *  @param __n The index of the element for which data should be
       *  accessed.
       *  @return  Read-only (constant) reference to data.
       *  @throw  std::out_of_range  If @a __n is an invalid index.
       *
       *  This function provides for safer data access.  The parameter
       *  is first checked that it is in the range of the vector.  The
       *  function throws out_of_range if the check fails.
       */ // :3
      const_reference // :3
      at(size_type __n) const // :3
      { // :3
	_M_range_check(__n); // :3
	return (*this)[__n]; // :3
      } // :3

      /**
       *  Returns a read/write reference to the data at the first
       *  element of the %vector.
       */ // :3
      reference // :3
      front() _GLIBCXX_NOEXCEPT // :3
      { return *begin(); } // :3

      /**
       *  Returns a read-only (constant) reference to the data at the first
       *  element of the %vector.
       */ // :3
      const_reference // :3
      front() const _GLIBCXX_NOEXCEPT // :3
      { return *begin(); } // :3

      /**
       *  Returns a read/write reference to the data at the last
       *  element of the %vector.
       */ // :3
      reference // :3
      back() _GLIBCXX_NOEXCEPT // :3
      { return *(end() - 1); } // :3

      /**
       *  Returns a read-only (constant) reference to the data at the
       *  last element of the %vector.
       */ // :3
      const_reference // :3
      back() const _GLIBCXX_NOEXCEPT // :3
      { return *(end() - 1); } // :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // DR 464. Suggestion for new member functions in standard containers. :3
      // data access :3
      /**
       *   Returns a pointer such that [data(), data() + size()) is a valid
       *   range.  For a non-empty %vector, data() == &front().
       */ // :3
#if __cplusplus >= 201103L // :3
      _Tp* // :3
#else // :3
      pointer // :3
#endif // :3
      data() _GLIBCXX_NOEXCEPT // :3
      { return __addressof(front()); } // :3

#if __cplusplus >= 201103L // :3
      const _Tp* // :3
#else // :3
      const_pointer // :3
#endif // :3
      data() const _GLIBCXX_NOEXCEPT // :3
      { return __addressof(front()); } // :3

      // [23.2.4.3] modifiers :3
      /**
       *  @brief  Add data to the end of the %vector.
       *  @param  __x  Data to be added.
       *
       *  This is a typical stack operation.  The function creates an
       *  element at the end of the %vector and assigns the given data
       *  to it.  Due to the nature of a %vector this operation can be
       *  done in constant time if the %vector has preallocated space
       *  available.
       */ // :3
      void // :3
      push_back(const value_type& __x) // :3
      { // :3
	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage) // :3
	  { // :3
	    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, // :3
	                             __x); // :3
	    ++this->_M_impl._M_finish; // :3
	  } // :3
	else // :3
#if __cplusplus >= 201103L // :3
	  _M_emplace_back_aux(__x); // :3
#else // :3
	  _M_insert_aux(end(), __x); // :3
#endif // :3
      } // :3

#if __cplusplus >= 201103L // :3
      void // :3
      push_back(value_type&& __x) // :3
      { emplace_back(std::move(__x)); } // :3

      template<typename... _Args> // :3
        void // :3
        emplace_back(_Args&&... __args); // :3
#endif // :3

      /**
       *  @brief  Removes last element.
       *
       *  This is a typical stack operation. It shrinks the %vector by one.
       *
       *  Note that no data is returned, and if the last element's
       *  data is needed, it should be retrieved before pop_back() is
       *  called.
       */ // :3
      void // :3
      pop_back() _GLIBCXX_NOEXCEPT // :3
      { // :3
	--this->_M_impl._M_finish; // :3
	_Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish); // :3
      } // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  Inserts an object in %vector before specified iterator.
       *  @param  __position  A const_iterator into the %vector.
       *  @param  __args  Arguments.
       *  @return  An iterator that points to the inserted data.
       *
       *  This function will insert an object of type T constructed
       *  with T(std::forward<Args>(args)...) before the specified location.
       *  Note that this kind of operation could be expensive for a %vector
       *  and if it is frequently used the user should consider using
       *  std::list.
       */ // :3
      template<typename... _Args> // :3
        iterator // :3
        emplace(const_iterator __position, _Args&&... __args); // :3

      /**
       *  @brief  Inserts given value into %vector before specified iterator.
       *  @param  __position  A const_iterator into the %vector.
       *  @param  __x  Data to be inserted.
       *  @return  An iterator that points to the inserted data.
       *
       *  This function will insert a copy of the given value before
       *  the specified location.  Note that this kind of operation
       *  could be expensive for a %vector and if it is frequently
       *  used the user should consider using std::list.
       */ // :3
      iterator // :3
      insert(const_iterator __position, const value_type& __x); // :3
#else // :3
      /**
       *  @brief  Inserts given value into %vector before specified iterator.
       *  @param  __position  An iterator into the %vector.
       *  @param  __x  Data to be inserted.
       *  @return  An iterator that points to the inserted data.
       *
       *  This function will insert a copy of the given value before
       *  the specified location.  Note that this kind of operation
       *  could be expensive for a %vector and if it is frequently
       *  used the user should consider using std::list.
       */ // :3
      iterator // :3
      insert(iterator __position, const value_type& __x); // :3
#endif // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  Inserts given rvalue into %vector before specified iterator.
       *  @param  __position  A const_iterator into the %vector.
       *  @param  __x  Data to be inserted.
       *  @return  An iterator that points to the inserted data.
       *
       *  This function will insert a copy of the given rvalue before
       *  the specified location.  Note that this kind of operation
       *  could be expensive for a %vector and if it is frequently
       *  used the user should consider using std::list.
       */ // :3
      iterator // :3
      insert(const_iterator __position, value_type&& __x) // :3
      { return emplace(__position, std::move(__x)); } // :3

      /**
       *  @brief  Inserts an initializer_list into the %vector.
       *  @param  __position  An iterator into the %vector.
       *  @param  __l  An initializer_list.
       *
       *  This function will insert copies of the data in the
       *  initializer_list @a l into the %vector before the location
       *  specified by @a position.
       *
       *  Note that this kind of operation could be expensive for a
       *  %vector and if it is frequently used the user should
       *  consider using std::list.
       */ // :3
      iterator // :3
      insert(const_iterator __position, initializer_list<value_type> __l) // :3
      { return this->insert(__position, __l.begin(), __l.end()); } // :3
#endif // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  Inserts a number of copies of given data into the %vector.
       *  @param  __position  A const_iterator into the %vector.
       *  @param  __n  Number of elements to be inserted.
       *  @param  __x  Data to be inserted.
       *  @return  An iterator that points to the inserted data.
       *
       *  This function will insert a specified number of copies of
       *  the given data before the location specified by @a position.
       *
       *  Note that this kind of operation could be expensive for a
       *  %vector and if it is frequently used the user should
       *  consider using std::list.
       */ // :3
      iterator // :3
      insert(const_iterator __position, size_type __n, const value_type& __x) // :3
      { // :3
	difference_type __offset = __position - cbegin(); // :3
	_M_fill_insert(begin() + __offset, __n, __x); // :3
	return begin() + __offset; // :3
      } // :3
#else // :3
      /**
       *  @brief  Inserts a number of copies of given data into the %vector.
       *  @param  __position  An iterator into the %vector.
       *  @param  __n  Number of elements to be inserted.
       *  @param  __x  Data to be inserted.
       *
       *  This function will insert a specified number of copies of
       *  the given data before the location specified by @a position.
       *
       *  Note that this kind of operation could be expensive for a
       *  %vector and if it is frequently used the user should
       *  consider using std::list.
       */ // :3
      void // :3
      insert(iterator __position, size_type __n, const value_type& __x) // :3
      { _M_fill_insert(__position, __n, __x); } // :3
#endif // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  Inserts a range into the %vector.
       *  @param  __position  A const_iterator into the %vector.
       *  @param  __first  An input iterator.
       *  @param  __last   An input iterator.
       *  @return  An iterator that points to the inserted data.
       *
       *  This function will insert copies of the data in the range
       *  [__first,__last) into the %vector before the location specified
       *  by @a pos.
       *
       *  Note that this kind of operation could be expensive for a
       *  %vector and if it is frequently used the user should
       *  consider using std::list.
       */ // :3
      template<typename _InputIterator, // :3
	       typename = _RequireInputIter<_InputIterator>> // :3
        iterator // :3
        insert(const_iterator __position, _InputIterator __first, // :3
	       _InputIterator __last) // :3
        { // :3
	  difference_type __offset = __position - cbegin(); // :3
	  _M_insert_dispatch(begin() + __offset, // :3
			     __first, __last, __false_type()); // :3
	  return begin() + __offset; // :3
	} // :3
#else // :3
      /**
       *  @brief  Inserts a range into the %vector.
       *  @param  __position  An iterator into the %vector.
       *  @param  __first  An input iterator.
       *  @param  __last   An input iterator.
       *
       *  This function will insert copies of the data in the range
       *  [__first,__last) into the %vector before the location specified
       *  by @a pos.
       *
       *  Note that this kind of operation could be expensive for a
       *  %vector and if it is frequently used the user should
       *  consider using std::list.
       */ // :3
      template<typename _InputIterator> // :3
        void // :3
        insert(iterator __position, _InputIterator __first, // :3
	       _InputIterator __last) // :3
        { // :3
	  // Check whether it's an integral type.  If so, it's not an iterator. :3
	  typedef typename std::__is_integer<_InputIterator>::__type _Integral; // :3
	  _M_insert_dispatch(__position, __first, __last, _Integral()); // :3
	} // :3
#endif // :3

      /**
       *  @brief  Remove element at given position.
       *  @param  __position  Iterator pointing to element to be erased.
       *  @return  An iterator pointing to the next element (or end()).
       *
       *  This function will erase the element at the given position and thus
       *  shorten the %vector by one.
       *
       *  Note This operation could be expensive and if it is
       *  frequently used the user should consider using std::list.
       *  The user is also cautioned that this function only erases
       *  the element, and that if the element is itself a pointer,
       *  the pointed-to memory is not touched in any way.  Managing
       *  the pointer is the user's responsibility.
       */ // :3
      iterator // :3
#if __cplusplus >= 201103L // :3
      erase(const_iterator __position) // :3
      { return _M_erase(begin() + (__position - cbegin())); } // :3
#else // :3
      erase(iterator __position) // :3
      { return _M_erase(__position); } // :3
#endif // :3

      /**
       *  @brief  Remove a range of elements.
       *  @param  __first  Iterator pointing to the first element to be erased.
       *  @param  __last  Iterator pointing to one past the last element to be
       *                  erased.
       *  @return  An iterator pointing to the element pointed to by @a __last
       *           prior to erasing (or end()).
       *
       *  This function will erase the elements in the range
       *  [__first,__last) and shorten the %vector accordingly.
       *
       *  Note This operation could be expensive and if it is
       *  frequently used the user should consider using std::list.
       *  The user is also cautioned that this function only erases
       *  the elements, and that if the elements themselves are
       *  pointers, the pointed-to memory is not touched in any way.
       *  Managing the pointer is the user's responsibility.
       */ // :3
      iterator // :3
#if __cplusplus >= 201103L // :3
      erase(const_iterator __first, const_iterator __last) // :3
      { // :3
	const auto __beg = begin(); // :3
	const auto __cbeg = cbegin(); // :3
	return _M_erase(__beg + (__first - __cbeg), __beg + (__last - __cbeg)); // :3
      } // :3
#else // :3
      erase(iterator __first, iterator __last) // :3
      { return _M_erase(__first, __last); } // :3
#endif // :3

      /**
       *  @brief  Swaps data with another %vector.
       *  @param  __x  A %vector of the same element and allocator types.
       *
       *  This exchanges the elements between two vectors in constant time.
       *  (Three pointers, so it should be quite fast.)
       *  Note that the global swap() function is specialized such that
       *  swap(v1,v2) will feed to this function.
       */ // :3
      void // :3
      swap(vector& __x) // :3
#if __cplusplus >= 201103L // :3
      noexcept(_Alloc_traits::_S_nothrow_swap()) // :3
#endif // :3
      { // :3
	this->_M_impl._M_swap_data(__x._M_impl); // :3
	_Alloc_traits::_S_on_swap(_M_get_Tp_allocator(), // :3
	                          __x._M_get_Tp_allocator()); // :3
      } // :3

      /**
       *  Erases all the elements.  Note that this function only erases the
       *  elements, and that if the elements themselves are pointers, the
       *  pointed-to memory is not touched in any way.  Managing the pointer is
       *  the user's responsibility.
       */ // :3
      void // :3
      clear() _GLIBCXX_NOEXCEPT // :3
      { _M_erase_at_end(this->_M_impl._M_start); } // :3

    protected: // :3
      /**
       *  Memory expansion handler.  Uses the member allocation function to
       *  obtain @a n bytes of memory, and then copies [first,last) into it.
       */ // :3
      template<typename _ForwardIterator> // :3
        pointer // :3
        _M_allocate_and_copy(size_type __n, // :3
			     _ForwardIterator __first, _ForwardIterator __last) // :3
        { // :3
	  pointer __result = this->_M_allocate(__n); // :3
	  __try // :3
	    { // :3
	      __uninitialized_copy_a(__first, __last, __result, // :3
					  _M_get_Tp_allocator()); // :3
	      return __result; // :3
	    } // :3
	  __catch(...) // :3
	    { // :3
	      _M_deallocate(__result, __n); // :3
	      __throw_exception_again; // :3
	    } // :3
	} // :3


      // Internal constructor functions follow. :3

      // Called by the range constructor to implement [23.1.1]/9 :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 438. Ambiguity in the "do the right thing" clause :3
      template<typename _Integer> // :3
        void // :3
        _M_initialize_dispatch(_Integer __n, _Integer __value, __true_type) // :3
        { // :3
	  this->_M_impl._M_start = _M_allocate(static_cast<size_type>(__n)); // :3
	  this->_M_impl._M_end_of_storage = // :3
	    this->_M_impl._M_start + static_cast<size_type>(__n); // :3
	  _M_fill_initialize(static_cast<size_type>(__n), __value); // :3
	} // :3

      // Called by the range constructor to implement [23.1.1]/9 :3
      template<typename _InputIterator> // :3
        void // :3
        _M_initialize_dispatch(_InputIterator __first, _InputIterator __last, // :3
			       __false_type) // :3
        { // :3
	  typedef typename iterator_traits<_InputIterator>:: // :3
	    iterator_category _IterCategory; // :3
	  _M_range_initialize(__first, __last, _IterCategory()); // :3
	} // :3

      // Called by the second initialize_dispatch above :3
      template<typename _InputIterator> // :3
        void // :3
        _M_range_initialize(_InputIterator __first, // :3
			    _InputIterator __last, input_iterator_tag) // :3
        { // :3
	  for (; __first != __last; ++__first) // :3
#if __cplusplus >= 201103L // :3
	    emplace_back(*__first); // :3
#else // :3
	    push_back(*__first); // :3
#endif // :3
	} // :3

      // Called by the second initialize_dispatch above :3
      template<typename _ForwardIterator> // :3
        void // :3
        _M_range_initialize(_ForwardIterator __first, // :3
			    _ForwardIterator __last, forward_iterator_tag) // :3
        { // :3
	  const size_type __n = std::distance(__first, __last); // :3
	  this->_M_impl._M_start = this->_M_allocate(__n); // :3
	  this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n; // :3
	  this->_M_impl._M_finish = // :3
	    __uninitialized_copy_a(__first, __last, // :3
					this->_M_impl._M_start, // :3
					_M_get_Tp_allocator()); // :3
	} // :3

      // Called by the first initialize_dispatch above and by the :3
      // vector(n,value,a) constructor. :3
      void // :3
      _M_fill_initialize(size_type __n, const value_type& __value) // :3
      { // :3
	__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, // :3
				      _M_get_Tp_allocator()); // :3
	this->_M_impl._M_finish = this->_M_impl._M_end_of_storage; // :3
      } // :3

#if __cplusplus >= 201103L // :3
      // Called by the vector(n) constructor. :3
      void // :3
      _M_default_initialize(size_type __n) // :3
      { // :3
	__uninitialized_default_n_a(this->_M_impl._M_start, __n, // :3
					 _M_get_Tp_allocator()); // :3
	this->_M_impl._M_finish = this->_M_impl._M_end_of_storage; // :3
      } // :3
#endif // :3

      // Internal assign functions follow.  The *_aux functions do the actual :3
      // assignment work for the range versions. :3

      // Called by the range assign to implement [23.1.1]/9 :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 438. Ambiguity in the "do the right thing" clause :3
      template<typename _Integer> // :3
        void // :3
        _M_assign_dispatch(_Integer __n, _Integer __val, __true_type) // :3
        { _M_fill_assign(__n, __val); } // :3

      // Called by the range assign to implement [23.1.1]/9 :3
      template<typename _InputIterator> // :3
        void // :3
        _M_assign_dispatch(_InputIterator __first, _InputIterator __last, // :3
			   __false_type) // :3
        { // :3
	  typedef typename iterator_traits<_InputIterator>:: // :3
	    iterator_category _IterCategory; // :3
	  _M_assign_aux(__first, __last, _IterCategory()); // :3
	} // :3

      // Called by the second assign_dispatch above :3
      template<typename _InputIterator> // :3
        void // :3
        _M_assign_aux(_InputIterator __first, _InputIterator __last, // :3
		      input_iterator_tag); // :3

      // Called by the second assign_dispatch above :3
      template<typename _ForwardIterator> // :3
        void // :3
        _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last, // :3
		      forward_iterator_tag); // :3

      // Called by assign(n,t), and the range assign when it turns out :3
      // to be the same thing. :3
      void // :3
      _M_fill_assign(size_type __n, const value_type& __val); // :3


      // Internal insert functions follow. :3

      // Called by the range insert to implement [23.1.1]/9 :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 438. Ambiguity in the "do the right thing" clause :3
      template<typename _Integer> // :3
        void // :3
        _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val, // :3
			   __true_type) // :3
        { _M_fill_insert(__pos, __n, __val); } // :3

      // Called by the range insert to implement [23.1.1]/9 :3
      template<typename _InputIterator> // :3
        void // :3
        _M_insert_dispatch(iterator __pos, _InputIterator __first, // :3
			   _InputIterator __last, __false_type) // :3
        { // :3
	  typedef typename iterator_traits<_InputIterator>:: // :3
	    iterator_category _IterCategory; // :3
	  _M_range_insert(__pos, __first, __last, _IterCategory()); // :3
	} // :3

      // Called by the second insert_dispatch above :3
      template<typename _InputIterator> // :3
        void // :3
        _M_range_insert(iterator __pos, _InputIterator __first, // :3
			_InputIterator __last, input_iterator_tag); // :3

      // Called by the second insert_dispatch above :3
      template<typename _ForwardIterator> // :3
        void // :3
        _M_range_insert(iterator __pos, _ForwardIterator __first, // :3
			_ForwardIterator __last, forward_iterator_tag); // :3

      // Called by insert(p,n,x), and the range insert when it turns out to be :3
      // the same thing. :3
      void // :3
      _M_fill_insert(iterator __pos, size_type __n, const value_type& __x); // :3

#if __cplusplus >= 201103L // :3
      // Called by resize(n). :3
      void // :3
      _M_default_append(size_type __n); // :3

      bool // :3
      _M_shrink_to_fit(); // :3
#endif // :3

      // Called by insert(p,x) :3
#if __cplusplus < 201103L // :3
      void // :3
      _M_insert_aux(iterator __position, const value_type& __x); // :3
#else // :3
      template<typename... _Args> // :3
        void // :3
        _M_insert_aux(iterator __position, _Args&&... __args); // :3

      template<typename... _Args> // :3
        void // :3
        _M_emplace_back_aux(_Args&&... __args); // :3
#endif // :3

      // Called by the latter. :3
      size_type // :3
      _M_check_len(size_type __n, const char* __s) const // :3
      { // :3
	// if (max_size() - size() < __n) :3
	//   throw; :3

	const size_type __len = size() + std::max(size(), __n); // :3
	return (__len < size() || __len > max_size()) ? max_size() : __len; // :3
      } // :3

      // Internal erase functions follow. :3

      // Called by erase(q1,q2), clear(), resize(), _M_fill_assign, :3
      // _M_assign_aux. :3
      void // :3
      _M_erase_at_end(pointer __pos) _GLIBCXX_NOEXCEPT // :3
      { // :3
	_Destroy(__pos, this->_M_impl._M_finish, _M_get_Tp_allocator()); // :3
	this->_M_impl._M_finish = __pos; // :3
      } // :3

      iterator // :3
      _M_erase(iterator __position); // :3

      iterator // :3
      _M_erase(iterator __first, iterator __last); // :3

#if __cplusplus >= 201103L // :3
    private: // :3
      // Constant-time move assignment when source object's memory can be :3
      // moved, either because the source's allocator will move too :3
      // or because the allocators are equal. :3
      void // :3
      _M_move_assign(vector&& __x, geode::stl::true_type) noexcept // :3
      { // :3
	const vector __tmp(std::move(*this)); // :3
	this->_M_impl._M_swap_data(__x._M_impl); // :3
	if (_Alloc_traits::_S_propagate_on_move_assign()) // :3
	  __alloc_on_move(_M_get_Tp_allocator(), // :3
			       __x._M_get_Tp_allocator()); // :3
      } // :3

      // Do move assignment when it might not be possible to move source :3
      // object's memory, resulting in a linear-time operation. :3
      void // :3
      _M_move_assign(vector&& __x, geode::stl::false_type) // :3
      { // :3
	if (__x._M_get_Tp_allocator() == this->_M_get_Tp_allocator()) // :3
	  _M_move_assign(std::move(__x), geode::stl::true_type()); // :3
	else // :3
	  { // :3
	    // The rvalue's allocator cannot be moved and is not equal, :3
	    // so we need to individually move each element. :3
	    this->assign(__make_move_if_noexcept_iterator(__x.begin()), // :3
			 __make_move_if_noexcept_iterator(__x.end())); // :3
	    __x.clear(); // :3
	  } // :3
      } // :3
#endif // :3

	  	// geode additions :3

		public: // :3

		operator std::vector<_Tp>() const { // :3
				return std::vector<_Tp>(this->_M_impl._M_start, this->_M_impl._M_start); // :3
		} // :3

		vector(const std::vector<_Tp>& __x) // :3
		: _Base(__x.size(), // :3
			_Alloc_traits::_S_select_on_copy(__x.get_allocator())) // :3
		{ this->_M_impl._M_finish = // :3
		__uninitialized_copy_a(__x.begin(), __x.end(), // :3
						this->_M_impl._M_start, // :3
						_M_get_Tp_allocator()); // :3
		} // :3
    }; // :3


  /**
   *  @brief  Vector equality comparison.
   *  @param  __x  A %vector.
   *  @param  __y  A %vector of the same type as @a __x.
   *  @return  True iff the size and elements of the vectors are equal.
   *
   *  This is an equivalence relation.  It is linear in the size of the
   *  vectors.  Vectors are considered equivalent if their sizes are equal,
   *  and if corresponding elements compare equal.
  */ // :3
  template<typename _Tp, typename _Alloc> // :3
    inline bool // :3
    operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) // :3
    { return (__x.size() == __y.size() // :3
	      && std::equal(__x.begin(), __x.end(), __y.begin())); } // :3

  /**
   *  @brief  Vector ordering relation.
   *  @param  __x  A %vector.
   *  @param  __y  A %vector of the same type as @a __x.
   *  @return  True iff @a __x is lexicographically less than @a __y.
   *
   *  This is a total ordering relation.  It is linear in the size of the
   *  vectors.  The elements must be comparable with @c <.
   *
   *  See std::lexicographical_compare() for how the determination is made.
  */ // :3
  template<typename _Tp, typename _Alloc> // :3
    inline bool // :3
    operator<(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) // :3
    { return std::lexicographical_compare(__x.begin(), __x.end(), // :3
					  __y.begin(), __y.end()); } // :3

  /// Based on operator== :3
  template<typename _Tp, typename _Alloc> // :3
    inline bool // :3
    operator!=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) // :3
    { return !(__x == __y); } // :3

  /// Based on operator< :3
  template<typename _Tp, typename _Alloc> // :3
    inline bool // :3
    operator>(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) // :3
    { return __y < __x; } // :3

  /// Based on operator< :3
  template<typename _Tp, typename _Alloc> // :3
    inline bool // :3
    operator<=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) // :3
    { return !(__y < __x); } // :3

  /// Based on operator< :3
  template<typename _Tp, typename _Alloc> // :3
    inline bool // :3
    operator>=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) // :3
    { return !(__x < __y); } // :3

  /// See std::vector::swap(). :3
  template<typename _Tp, typename _Alloc> // :3
    inline void // :3
    swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y) // :3
    { __x.swap(__y); } // :3

_GLIBCXX_END_NAMESPACE_CONTAINER // :3
} // namespace std :3
