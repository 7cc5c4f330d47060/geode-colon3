// Iterators -*- C++ -*- :3

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

/** @file bits/stl_iterator.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 *
 *  This file implements reverse_iterator, back_insert_iterator,
 *  front_insert_iterator, insert_iterator, __normal_iterator, and their
 *  supporting functions and overloaded operators.
 */ // :3

#ifndef _STL_ITERATOR_H // :3
#define _STL_ITERATOR_H 1 // :3

//#include "cpp_type_traits.h" :3
//#include "ext/type_traits.h" :3
#include "move.h" // :3
#include "ptr_traits.h" // :3
#include "stl_iterator_base_types.h" // :3
#include "stl_iterator_base_funcs.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   * @addtogroup iterators
   * @{
   */ // :3

  // 24.4.1 Reverse iterators :3
  /**
   *  Bidirectional and random access iterators have corresponding reverse
   *  %iterator adaptors that iterate through the data structure in the
   *  opposite direction.  They have the same signatures as the corresponding
   *  iterators.  The fundamental relation between a reverse %iterator and its
   *  corresponding %iterator @c i is established by the identity:
   *  @code
   *      &*(reverse_iterator(i)) == &*(i - 1)
   *  @endcode
   *
   *  <em>This mapping is dictated by the fact that while there is always a
   *  pointer past the end of an array, there might not be a valid pointer
   *  before the beginning of an array.</em> [24.4.1]/1,2
   *
   *  Reverse iterators can be tricky and surprising at first.  Their
   *  semantics make sense, however, and the trickiness is a side effect of
   *  the requirement that the iterators must be safe.
  */ // :3
  template<typename _Iterator> // :3
    class reverse_iterator // :3
    : public iterator<typename iterator_traits<_Iterator>::iterator_category, // :3
		      typename iterator_traits<_Iterator>::value_type, // :3
		      typename iterator_traits<_Iterator>::difference_type, // :3
		      typename iterator_traits<_Iterator>::pointer, // :3
                      typename iterator_traits<_Iterator>::reference> // :3
    { // :3
    protected: // :3
      _Iterator current; // :3

      typedef iterator_traits<_Iterator>		__traits_type; // :3

    public: // :3
      typedef _Iterator					iterator_type; // :3
      typedef typename __traits_type::difference_type	difference_type; // :3
      typedef typename __traits_type::pointer		pointer; // :3
      typedef typename __traits_type::reference		reference; // :3

      /**
       *  The default constructor value-initializes member @p current.
       *  If it is a pointer, that means it is zero-initialized.
      */ // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 235 No specification of default ctor for reverse_iterator :3
      reverse_iterator() : current() { } // :3

      /**
       *  This %iterator will move in the opposite direction that @p x does.
      */ // :3
      explicit // :3
      reverse_iterator(iterator_type __x) : current(__x) { } // :3

      /**
       *  The copy constructor is normal.
      */ // :3
      reverse_iterator(const reverse_iterator& __x) // :3
      : current(__x.current) { } // :3

      /**
       *  A %reverse_iterator across other types can be copied if the
       *  underlying %iterator can be converted to the type of @c current.
      */ // :3
      template<typename _Iter> // :3
        reverse_iterator(const reverse_iterator<_Iter>& __x) // :3
	: current(__x.base()) { } // :3

      /**
       *  @return  @c current, the %iterator used for underlying work.
      */ // :3
      iterator_type // :3
      base() const // :3
      { return current; } // :3

      /**
       *  @return  A reference to the value at @c --current
       *
       *  This requires that @c --current is dereferenceable.
       *
       *  @warning This implementation requires that for an iterator of the
       *           underlying iterator type, @c x, a reference obtained by
       *           @c *x remains valid after @c x has been modified or
       *           destroyed. This is a bug: http://gcc.gnu.org/PR51823
      */ // :3
      reference // :3
      operator*() const // :3
      { // :3
	_Iterator __tmp = current; // :3
	return *--__tmp; // :3
      } // :3

      /**
       *  @return  A pointer to the value at @c --current
       *
       *  This requires that @c --current is dereferenceable.
      */ // :3
      pointer // :3
      operator->() const // :3
      { return &(operator*()); } // :3

      /**
       *  @return  @c *this
       *
       *  Decrements the underlying iterator.
      */ // :3
      reverse_iterator& // :3
      operator++() // :3
      { // :3
	--current; // :3
	return *this; // :3
      } // :3

      /**
       *  @return  The original value of @c *this
       *
       *  Decrements the underlying iterator.
      */ // :3
      reverse_iterator // :3
      operator++(int) // :3
      { // :3
	reverse_iterator __tmp = *this; // :3
	--current; // :3
	return __tmp; // :3
      } // :3

      /**
       *  @return  @c *this
       *
       *  Increments the underlying iterator.
      */ // :3
      reverse_iterator& // :3
      operator--() // :3
      { // :3
	++current; // :3
	return *this; // :3
      } // :3

      /**
       *  @return  A reverse_iterator with the previous value of @c *this
       *
       *  Increments the underlying iterator.
      */ // :3
      reverse_iterator // :3
      operator--(int) // :3
      { // :3
	reverse_iterator __tmp = *this; // :3
	++current; // :3
	return __tmp; // :3
      } // :3

      /**
       *  @return  A reverse_iterator that refers to @c current - @a __n
       *
       *  The underlying iterator must be a Random Access Iterator.
      */ // :3
      reverse_iterator // :3
      operator+(difference_type __n) const // :3
      { return reverse_iterator(current - __n); } // :3

      /**
       *  @return  *this
       *
       *  Moves the underlying iterator backwards @a __n steps.
       *  The underlying iterator must be a Random Access Iterator.
      */ // :3
      reverse_iterator& // :3
      operator+=(difference_type __n) // :3
      { // :3
	current -= __n; // :3
	return *this; // :3
      } // :3

      /**
       *  @return  A reverse_iterator that refers to @c current - @a __n
       *
       *  The underlying iterator must be a Random Access Iterator.
      */ // :3
      reverse_iterator // :3
      operator-(difference_type __n) const // :3
      { return reverse_iterator(current + __n); } // :3

      /**
       *  @return  *this
       *
       *  Moves the underlying iterator forwards @a __n steps.
       *  The underlying iterator must be a Random Access Iterator.
      */ // :3
      reverse_iterator& // :3
      operator-=(difference_type __n) // :3
      { // :3
	current += __n; // :3
	return *this; // :3
      } // :3

      /**
       *  @return  The value at @c current - @a __n - 1
       *
       *  The underlying iterator must be a Random Access Iterator.
      */ // :3
      reference // :3
      operator[](difference_type __n) const // :3
      { return *(*this + __n); } // :3
    }; // :3

  //@{ :3
  /**
   *  @param  __x  A %reverse_iterator.
   *  @param  __y  A %reverse_iterator.
   *  @return  A simple bool.
   *
   *  Reverse iterators forward many operations to their underlying base()
   *  iterators.  Others are implemented in terms of one another.
   *
  */ // :3
  template<typename _Iterator> // :3
    inline bool // :3
    operator==(const reverse_iterator<_Iterator>& __x, // :3
	       const reverse_iterator<_Iterator>& __y) // :3
    { return __x.base() == __y.base(); } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator<(const reverse_iterator<_Iterator>& __x, // :3
	      const reverse_iterator<_Iterator>& __y) // :3
    { return __y.base() < __x.base(); } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator!=(const reverse_iterator<_Iterator>& __x, // :3
	       const reverse_iterator<_Iterator>& __y) // :3
    { return !(__x == __y); } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator>(const reverse_iterator<_Iterator>& __x, // :3
	      const reverse_iterator<_Iterator>& __y) // :3
    { return __y < __x; } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator<=(const reverse_iterator<_Iterator>& __x, // :3
	       const reverse_iterator<_Iterator>& __y) // :3
    { return !(__y < __x); } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator>=(const reverse_iterator<_Iterator>& __x, // :3
	       const reverse_iterator<_Iterator>& __y) // :3
    { return !(__x < __y); } // :3

  template<typename _Iterator> // :3
    inline typename reverse_iterator<_Iterator>::difference_type // :3
    operator-(const reverse_iterator<_Iterator>& __x, // :3
	      const reverse_iterator<_Iterator>& __y) // :3
    { return __y.base() - __x.base(); } // :3

  template<typename _Iterator> // :3
    inline reverse_iterator<_Iterator> // :3
    operator+(typename reverse_iterator<_Iterator>::difference_type __n, // :3
	      const reverse_iterator<_Iterator>& __x) // :3
    { return reverse_iterator<_Iterator>(__x.base() - __n); } // :3

  // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
  // DR 280. Comparison of reverse_iterator to const reverse_iterator. :3
  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator==(const reverse_iterator<_IteratorL>& __x, // :3
	       const reverse_iterator<_IteratorR>& __y) // :3
    { return __x.base() == __y.base(); } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator<(const reverse_iterator<_IteratorL>& __x, // :3
	      const reverse_iterator<_IteratorR>& __y) // :3
    { return __y.base() < __x.base(); } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator!=(const reverse_iterator<_IteratorL>& __x, // :3
	       const reverse_iterator<_IteratorR>& __y) // :3
    { return !(__x == __y); } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator>(const reverse_iterator<_IteratorL>& __x, // :3
	      const reverse_iterator<_IteratorR>& __y) // :3
    { return __y < __x; } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator<=(const reverse_iterator<_IteratorL>& __x, // :3
	       const reverse_iterator<_IteratorR>& __y) // :3
    { return !(__y < __x); } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator>=(const reverse_iterator<_IteratorL>& __x, // :3
	       const reverse_iterator<_IteratorR>& __y) // :3
    { return !(__x < __y); } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
#if __cplusplus >= 201103L // :3
    // DR 685. :3
    inline auto // :3
    operator-(const reverse_iterator<_IteratorL>& __x, // :3
	      const reverse_iterator<_IteratorR>& __y) // :3
    -> decltype(__y.base() - __x.base()) // :3
#else // :3
    inline typename reverse_iterator<_IteratorL>::difference_type // :3
    operator-(const reverse_iterator<_IteratorL>& __x, // :3
	      const reverse_iterator<_IteratorR>& __y) // :3
#endif // :3
    { return __y.base() - __x.base(); } // :3
  //@} :3

  // 24.4.2.2.1 back_insert_iterator :3
  /**
   *  @brief  Turns assignment into insertion.
   *
   *  These are output iterators, constructed from a container-of-T.
   *  Assigning a T to the iterator appends it to the container using
   *  push_back.
   *
   *  Tip:  Using the back_inserter function to create these iterators can
   *  save typing.
  */ // :3
  template<typename _Container> // :3
    class back_insert_iterator // :3
    : public iterator<output_iterator_tag, void, void, void, void> // :3
    { // :3
    protected: // :3
      _Container* container; // :3

    public: // :3
      /// A nested typedef for the type of whatever container you used. :3
      typedef _Container          container_type; // :3

      /// The only way to create this %iterator is with a container. :3
      explicit // :3
      back_insert_iterator(_Container& __x) : container(&__x) { } // :3

      /**
       *  @param  __value  An instance of whatever type
       *                 container_type::const_reference is; presumably a
       *                 reference-to-const T for container<T>.
       *  @return  This %iterator, for chained operations.
       *
       *  This kind of %iterator doesn't really have a @a position in the
       *  container (you can think of the position as being permanently at
       *  the end, if you like).  Assigning a value to the %iterator will
       *  always append the value to the end of the container.
      */ // :3
#if __cplusplus < 201103L // :3
      back_insert_iterator& // :3
      operator=(typename _Container::const_reference __value) // :3
      { // :3
	container->push_back(__value); // :3
	return *this; // :3
      } // :3
#else // :3
      back_insert_iterator& // :3
      operator=(const typename _Container::value_type& __value) // :3
      { // :3
	container->push_back(__value); // :3
	return *this; // :3
      } // :3

      back_insert_iterator& // :3
      operator=(typename _Container::value_type&& __value) // :3
      { // :3
	container->push_back(std::move(__value)); // :3
	return *this; // :3
      } // :3
#endif // :3

      /// Simply returns *this. :3
      back_insert_iterator& // :3
      operator*() // :3
      { return *this; } // :3

      /// Simply returns *this.  (This %iterator does not @a move.) :3
      back_insert_iterator& // :3
      operator++() // :3
      { return *this; } // :3

      /// Simply returns *this.  (This %iterator does not @a move.) :3
      back_insert_iterator // :3
      operator++(int) // :3
      { return *this; } // :3
    }; // :3

  /**
   *  @param  __x  A container of arbitrary type.
   *  @return  An instance of back_insert_iterator working on @p __x.
   *
   *  This wrapper function helps in creating back_insert_iterator instances.
   *  Typing the name of the %iterator requires knowing the precise full
   *  type of the container, which can be tedious and impedes generic
   *  programming.  Using this function lets you take advantage of automatic
   *  template parameter deduction, making the compiler match the correct
   *  types for you.
  */ // :3
  template<typename _Container> // :3
    inline back_insert_iterator<_Container> // :3
    back_inserter(_Container& __x) // :3
    { return back_insert_iterator<_Container>(__x); } // :3

  /**
   *  @brief  Turns assignment into insertion.
   *
   *  These are output iterators, constructed from a container-of-T.
   *  Assigning a T to the iterator prepends it to the container using
   *  push_front.
   *
   *  Tip:  Using the front_inserter function to create these iterators can
   *  save typing.
  */ // :3
  template<typename _Container> // :3
    class front_insert_iterator // :3
    : public iterator<output_iterator_tag, void, void, void, void> // :3
    { // :3
    protected: // :3
      _Container* container; // :3

    public: // :3
      /// A nested typedef for the type of whatever container you used. :3
      typedef _Container          container_type; // :3

      /// The only way to create this %iterator is with a container. :3
      explicit front_insert_iterator(_Container& __x) : container(&__x) { } // :3

      /**
       *  @param  __value  An instance of whatever type
       *                 container_type::const_reference is; presumably a
       *                 reference-to-const T for container<T>.
       *  @return  This %iterator, for chained operations.
       *
       *  This kind of %iterator doesn't really have a @a position in the
       *  container (you can think of the position as being permanently at
       *  the front, if you like).  Assigning a value to the %iterator will
       *  always prepend the value to the front of the container.
      */ // :3
#if __cplusplus < 201103L // :3
      front_insert_iterator& // :3
      operator=(typename _Container::const_reference __value) // :3
      { // :3
	container->push_front(__value); // :3
	return *this; // :3
      } // :3
#else // :3
      front_insert_iterator& // :3
      operator=(const typename _Container::value_type& __value) // :3
      { // :3
	container->push_front(__value); // :3
	return *this; // :3
      } // :3

      front_insert_iterator& // :3
      operator=(typename _Container::value_type&& __value) // :3
      { // :3
	container->push_front(std::move(__value)); // :3
	return *this; // :3
      } // :3
#endif // :3

      /// Simply returns *this. :3
      front_insert_iterator& // :3
      operator*() // :3
      { return *this; } // :3

      /// Simply returns *this.  (This %iterator does not @a move.) :3
      front_insert_iterator& // :3
      operator++() // :3
      { return *this; } // :3

      /// Simply returns *this.  (This %iterator does not @a move.) :3
      front_insert_iterator // :3
      operator++(int) // :3
      { return *this; } // :3
    }; // :3

  /**
   *  @param  __x  A container of arbitrary type.
   *  @return  An instance of front_insert_iterator working on @p x.
   *
   *  This wrapper function helps in creating front_insert_iterator instances.
   *  Typing the name of the %iterator requires knowing the precise full
   *  type of the container, which can be tedious and impedes generic
   *  programming.  Using this function lets you take advantage of automatic
   *  template parameter deduction, making the compiler match the correct
   *  types for you.
  */ // :3
  template<typename _Container> // :3
    inline front_insert_iterator<_Container> // :3
    front_inserter(_Container& __x) // :3
    { return front_insert_iterator<_Container>(__x); } // :3

  /**
   *  @brief  Turns assignment into insertion.
   *
   *  These are output iterators, constructed from a container-of-T.
   *  Assigning a T to the iterator inserts it in the container at the
   *  %iterator's position, rather than overwriting the value at that
   *  position.
   *
   *  (Sequences will actually insert a @e copy of the value before the
   *  %iterator's position.)
   *
   *  Tip:  Using the inserter function to create these iterators can
   *  save typing.
  */ // :3
  template<typename _Container> // :3
    class insert_iterator // :3
    : public iterator<output_iterator_tag, void, void, void, void> // :3
    { // :3
    protected: // :3
      _Container* container; // :3
      typename _Container::iterator iter; // :3

    public: // :3
      /// A nested typedef for the type of whatever container you used. :3
      typedef _Container          container_type; // :3

      /**
       *  The only way to create this %iterator is with a container and an
       *  initial position (a normal %iterator into the container).
      */ // :3
      insert_iterator(_Container& __x, typename _Container::iterator __i) // :3
      : container(&__x), iter(__i) {} // :3

      /**
       *  @param  __value  An instance of whatever type
       *                 container_type::const_reference is; presumably a
       *                 reference-to-const T for container<T>.
       *  @return  This %iterator, for chained operations.
       *
       *  This kind of %iterator maintains its own position in the
       *  container.  Assigning a value to the %iterator will insert the
       *  value into the container at the place before the %iterator.
       *
       *  The position is maintained such that subsequent assignments will
       *  insert values immediately after one another.  For example,
       *  @code
       *     // vector v contains A and Z
       *
       *     insert_iterator i (v, ++v.begin());
       *     i = 1;
       *     i = 2;
       *     i = 3;
       *
       *     // vector v contains A, 1, 2, 3, and Z
       *  @endcode
      */ // :3
#if __cplusplus < 201103L // :3
      insert_iterator& // :3
      operator=(typename _Container::const_reference __value) // :3
      { // :3
	iter = container->insert(iter, __value); // :3
	++iter; // :3
	return *this; // :3
      } // :3
#else // :3
      insert_iterator& // :3
      operator=(const typename _Container::value_type& __value) // :3
      { // :3
	iter = container->insert(iter, __value); // :3
	++iter; // :3
	return *this; // :3
      } // :3

      insert_iterator& // :3
      operator=(typename _Container::value_type&& __value) // :3
      { // :3
	iter = container->insert(iter, std::move(__value)); // :3
	++iter; // :3
	return *this; // :3
      } // :3
#endif // :3

      /// Simply returns *this. :3
      insert_iterator& // :3
      operator*() // :3
      { return *this; } // :3

      /// Simply returns *this.  (This %iterator does not @a move.) :3
      insert_iterator& // :3
      operator++() // :3
      { return *this; } // :3

      /// Simply returns *this.  (This %iterator does not @a move.) :3
      insert_iterator& // :3
      operator++(int) // :3
      { return *this; } // :3
    }; // :3

  /**
   *  @param __x  A container of arbitrary type.
   *  @return  An instance of insert_iterator working on @p __x.
   *
   *  This wrapper function helps in creating insert_iterator instances.
   *  Typing the name of the %iterator requires knowing the precise full
   *  type of the container, which can be tedious and impedes generic
   *  programming.  Using this function lets you take advantage of automatic
   *  template parameter deduction, making the compiler match the correct
   *  types for you.
  */ // :3
  template<typename _Container, typename _Iterator> // :3
    inline insert_iterator<_Container> // :3
    inserter(_Container& __x, _Iterator __i) // :3
    { // :3
      return insert_iterator<_Container>(__x, // :3
					 typename _Container::iterator(__i)); // :3
    } // :3

  // @} group iterators :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default) // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  // This iterator adapter is @a normal in the sense that it does not :3
  // change the semantics of any of the operators of its iterator :3
  // parameter.  Its primary purpose is to convert an iterator that is :3
  // not a class, e.g. a pointer, into an iterator that is a class. :3
  // The _Container parameter exists solely so that different containers :3
  // using this template can instantiate different types, even if the :3
  // _Iterator parameter is the same. :3
  using geode::stl::iterator_traits; // :3
  using geode::stl::iterator; // :3
  template<typename _Iterator, typename _Container> // :3
    class __normal_iterator // :3
    { // :3
    protected: // :3
      _Iterator _M_current; // :3

      typedef iterator_traits<_Iterator>		__traits_type; // :3

    public: // :3
      typedef _Iterator					iterator_type; // :3
      typedef typename __traits_type::iterator_category iterator_category; // :3
      typedef typename __traits_type::value_type  	value_type; // :3
      typedef typename __traits_type::difference_type 	difference_type; // :3
      typedef typename __traits_type::reference 	reference; // :3
      typedef typename __traits_type::pointer   	pointer; // :3

      _GLIBCXX_CONSTEXPR __normal_iterator() _GLIBCXX_NOEXCEPT // :3
      : _M_current(_Iterator()) { } // :3

      explicit // :3
      __normal_iterator(const _Iterator& __i) _GLIBCXX_NOEXCEPT // :3
      : _M_current(__i) { } // :3

      // Allow iterator to const_iterator conversion :3
      template<typename _Iter> // :3
        __normal_iterator(const __normal_iterator<_Iter, // :3
			  typename std::enable_if_t< // :3
      	       (std::is_same_v<_Iter, typename _Container::pointer>), // :3
		      _Container>>& __i) _GLIBCXX_NOEXCEPT // :3
        : _M_current(__i.base()) { } // :3

      // Forward iterator requirements :3
      reference // :3
      operator*() const _GLIBCXX_NOEXCEPT // :3
      { return *_M_current; } // :3

      pointer // :3
      operator->() const _GLIBCXX_NOEXCEPT // :3
      { return _M_current; } // :3

      __normal_iterator& // :3
      operator++() _GLIBCXX_NOEXCEPT // :3
      { // :3
	++_M_current; // :3
	return *this; // :3
      } // :3

      __normal_iterator // :3
      operator++(int) _GLIBCXX_NOEXCEPT // :3
      { return __normal_iterator(_M_current++); } // :3

      // Bidirectional iterator requirements :3
      __normal_iterator& // :3
      operator--() _GLIBCXX_NOEXCEPT // :3
      { // :3
	--_M_current; // :3
	return *this; // :3
      } // :3

      __normal_iterator // :3
      operator--(int) _GLIBCXX_NOEXCEPT // :3
      { return __normal_iterator(_M_current--); } // :3

      // Random access iterator requirements :3
      reference // :3
      operator[](difference_type __n) const _GLIBCXX_NOEXCEPT // :3
      { return _M_current[__n]; } // :3

      __normal_iterator& // :3
      operator+=(difference_type __n) _GLIBCXX_NOEXCEPT // :3
      { _M_current += __n; return *this; } // :3

      __normal_iterator // :3
      operator+(difference_type __n) const _GLIBCXX_NOEXCEPT // :3
      { return __normal_iterator(_M_current + __n); } // :3

      __normal_iterator& // :3
      operator-=(difference_type __n) _GLIBCXX_NOEXCEPT // :3
      { _M_current -= __n; return *this; } // :3

      __normal_iterator // :3
      operator-(difference_type __n) const _GLIBCXX_NOEXCEPT // :3
      { return __normal_iterator(_M_current - __n); } // :3

      const _Iterator& // :3
      base() const _GLIBCXX_NOEXCEPT // :3
      { return _M_current; } // :3

      // geode addition :3
      operator ::std::__wrap_iter<_Iterator> () { // :3
        return *reinterpret_cast<::std::__wrap_iter<_Iterator>*>(this); // :3
      } // :3
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
    inline bool // :3
    operator==(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	       const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() == __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    inline bool // :3
    operator==(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	       const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() == __rhs.base(); } // :3

  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    inline bool // :3
    operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	       const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() != __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    inline bool // :3
    operator!=(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	       const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() != __rhs.base(); } // :3

  // Random access iterator requirements :3
  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    inline bool // :3
    operator<(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	      const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() < __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    inline bool // :3
    operator<(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	      const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() < __rhs.base(); } // :3

  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    inline bool // :3
    operator>(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	      const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() > __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    inline bool // :3
    operator>(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	      const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() > __rhs.base(); } // :3

  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    inline bool // :3
    operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	       const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() <= __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    inline bool // :3
    operator<=(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	       const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() <= __rhs.base(); } // :3

  template<typename _IteratorL, typename _IteratorR, typename _Container> // :3
    inline bool // :3
    operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs, // :3
	       const __normal_iterator<_IteratorR, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() >= __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
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
    inline auto // :3
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
    inline typename __normal_iterator<_Iterator, _Container>::difference_type // :3
    operator-(const __normal_iterator<_Iterator, _Container>& __lhs, // :3
	      const __normal_iterator<_Iterator, _Container>& __rhs) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __lhs.base() - __rhs.base(); } // :3

  template<typename _Iterator, typename _Container> // :3
    inline __normal_iterator<_Iterator, _Container> // :3
    operator+(typename __normal_iterator<_Iterator, _Container>::difference_type // :3
	      __n, const __normal_iterator<_Iterator, _Container>& __i) // :3
    _GLIBCXX_NOEXCEPT // :3
    { return __normal_iterator<_Iterator, _Container>(__i.base() + __n); } // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#if __cplusplus >= 201103L // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   * @addtogroup iterators
   * @{
   */ // :3

  // 24.4.3  Move iterators :3
  /**
   *  Class template move_iterator is an iterator adapter with the same
   *  behavior as the underlying iterator except that its dereference
   *  operator implicitly converts the value returned by the underlying
   *  iterator's dereference operator to an rvalue reference.  Some
   *  generic algorithms can be called with move iterators to replace
   *  copying with moving.
   */ // :3
  template<typename _Iterator> // :3
    class move_iterator // :3
    { // :3
    protected: // :3
      _Iterator _M_current; // :3

      typedef iterator_traits<_Iterator>		__traits_type; // :3
      typedef typename __traits_type::reference		__base_ref; // :3

    public: // :3
      typedef _Iterator					iterator_type; // :3
      typedef typename __traits_type::iterator_category iterator_category; // :3
      typedef typename __traits_type::value_type  	value_type; // :3
      typedef typename __traits_type::difference_type	difference_type; // :3
      // NB: DR 680. :3
      typedef _Iterator					pointer; // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 2106. move_iterator wrapping iterators returning prvalues :3
      typedef typename std::conditional_t<std::is_reference_v<__base_ref>, // :3
			 typename std::remove_reference_t<__base_ref>&&, // :3
			 __base_ref>		reference; // :3

      move_iterator() // :3
      : _M_current() { } // :3

      explicit // :3
      move_iterator(iterator_type __i) // :3
      : _M_current(__i) { } // :3

      template<typename _Iter> // :3
	move_iterator(const move_iterator<_Iter>& __i) // :3
	: _M_current(__i.base()) { } // :3

      iterator_type // :3
      base() const // :3
      { return _M_current; } // :3

      reference // :3
      operator*() const // :3
      { return static_cast<reference>(*_M_current); } // :3

      pointer // :3
      operator->() const // :3
      { return _M_current; } // :3

      move_iterator& // :3
      operator++() // :3
      { // :3
	++_M_current; // :3
	return *this; // :3
      } // :3

      move_iterator // :3
      operator++(int) // :3
      { // :3
	move_iterator __tmp = *this; // :3
	++_M_current; // :3
	return __tmp; // :3
      } // :3

      move_iterator& // :3
      operator--() // :3
      { // :3
	--_M_current; // :3
	return *this; // :3
      } // :3

      move_iterator // :3
      operator--(int) // :3
      { // :3
	move_iterator __tmp = *this; // :3
	--_M_current; // :3
	return __tmp; // :3
      } // :3

      move_iterator // :3
      operator+(difference_type __n) const // :3
      { return move_iterator(_M_current + __n); } // :3

      move_iterator& // :3
      operator+=(difference_type __n) // :3
      { // :3
	_M_current += __n; // :3
	return *this; // :3
      } // :3

      move_iterator // :3
      operator-(difference_type __n) const // :3
      { return move_iterator(_M_current - __n); } // :3

      move_iterator& // :3
      operator-=(difference_type __n) // :3
      { // :3
	_M_current -= __n; // :3
	return *this; // :3
      } // :3

      reference // :3
      operator[](difference_type __n) const // :3
      { return std::move(_M_current[__n]); } // :3
    }; // :3

  // Note: See __normal_iterator operators note from Gaby to understand :3
  // why there are always 2 versions for most of the move_iterator :3
  // operators. :3
  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator==(const move_iterator<_IteratorL>& __x, // :3
	       const move_iterator<_IteratorR>& __y) // :3
    { return __x.base() == __y.base(); } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator==(const move_iterator<_Iterator>& __x, // :3
	       const move_iterator<_Iterator>& __y) // :3
    { return __x.base() == __y.base(); } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator!=(const move_iterator<_IteratorL>& __x, // :3
	       const move_iterator<_IteratorR>& __y) // :3
    { return !(__x == __y); } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator!=(const move_iterator<_Iterator>& __x, // :3
	       const move_iterator<_Iterator>& __y) // :3
    { return !(__x == __y); } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator<(const move_iterator<_IteratorL>& __x, // :3
	      const move_iterator<_IteratorR>& __y) // :3
    { return __x.base() < __y.base(); } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator<(const move_iterator<_Iterator>& __x, // :3
	      const move_iterator<_Iterator>& __y) // :3
    { return __x.base() < __y.base(); } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator<=(const move_iterator<_IteratorL>& __x, // :3
	       const move_iterator<_IteratorR>& __y) // :3
    { return !(__y < __x); } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator<=(const move_iterator<_Iterator>& __x, // :3
	       const move_iterator<_Iterator>& __y) // :3
    { return !(__y < __x); } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator>(const move_iterator<_IteratorL>& __x, // :3
	      const move_iterator<_IteratorR>& __y) // :3
    { return __y < __x; } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator>(const move_iterator<_Iterator>& __x, // :3
	      const move_iterator<_Iterator>& __y) // :3
    { return __y < __x; } // :3

  template<typename _IteratorL, typename _IteratorR> // :3
    inline bool // :3
    operator>=(const move_iterator<_IteratorL>& __x, // :3
	       const move_iterator<_IteratorR>& __y) // :3
    { return !(__x < __y); } // :3

  template<typename _Iterator> // :3
    inline bool // :3
    operator>=(const move_iterator<_Iterator>& __x, // :3
	       const move_iterator<_Iterator>& __y) // :3
    { return !(__x < __y); } // :3

  // DR 685. :3
  template<typename _IteratorL, typename _IteratorR> // :3
    inline auto // :3
    operator-(const move_iterator<_IteratorL>& __x, // :3
	      const move_iterator<_IteratorR>& __y) // :3
    -> decltype(__x.base() - __y.base()) // :3
    { return __x.base() - __y.base(); } // :3

  template<typename _Iterator> // :3
    inline auto // :3
    operator-(const move_iterator<_Iterator>& __x, // :3
	      const move_iterator<_Iterator>& __y) // :3
    -> decltype(__x.base() - __y.base()) // :3
    { return __x.base() - __y.base(); } // :3

  template<typename _Iterator> // :3
    inline move_iterator<_Iterator> // :3
    operator+(typename move_iterator<_Iterator>::difference_type __n, // :3
	      const move_iterator<_Iterator>& __x) // :3
    { return __x + __n; } // :3

  template<typename _Iterator> // :3
    inline move_iterator<_Iterator> // :3
    make_move_iterator(_Iterator __i) // :3
    { return move_iterator<_Iterator>(__i); } // :3

  template<typename _Iterator, typename _ReturnType // :3
    = typename std::conditional_t<__move_if_noexcept_cond // :3
      <typename iterator_traits<_Iterator>::value_type>::value, // :3
                _Iterator, move_iterator<_Iterator>>> // :3
    inline _ReturnType // :3
    __make_move_if_noexcept_iterator(_Iterator __i) // :3
    { return _ReturnType(__i); } // :3

  // @} group iterators :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#define _GLIBCXX_MAKE_MOVE_ITERATOR(_Iter) std::make_move_iterator(_Iter) // :3
#define _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(_Iter) \
  __make_move_if_noexcept_iterator(_Iter) // :3
#else // :3
#define _GLIBCXX_MAKE_MOVE_ITERATOR(_Iter) (_Iter) // :3
#define _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(_Iter) (_Iter) // :3
#endif // C++11 :3

#endif // :3