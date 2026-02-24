// Map implementation -*- C++ -*- :3

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

/** @file bits/stl_map.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{map}
 */ // :3

#pragma once // :3

#include "functexcept.h" // :3
#include "concept_check.h" // :3
#include "ext/rb_tree.h" // :3
#include "initializer_list.h" // :3
#include "stl_function.h" // :3
#include "tuple.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER // :3

  /**
   *  @brief A standard container made up of (key,value) pairs, which can be
   *  retrieved based on a key, in logarithmic time.
   *
   *  @ingroup associative_containers
   *
   *  @tparam _Key  Type of key objects.
   *  @tparam  _Tp  Type of mapped objects.
   *  @tparam _Compare  Comparison function object type, defaults to less<_Key>.
   *  @tparam _Alloc  Allocator type, defaults to
   *                  allocator<pair<const _Key, _Tp>.
   *
   *  Meets the requirements of a <a href="tables.html#65">container</a>, a
   *  <a href="tables.html#66">reversible container</a>, and an
   *  <a href="tables.html#69">associative container</a> (using unique keys).
   *  For a @c map<Key,T> the key_type is Key, the mapped_type is T, and the
   *  value_type is std::pair<const Key,T>.
   *
   *  Maps support bidirectional iterators.
   *
   *  The private tree data is declared exactly the same way for map and
   *  multimap; the distinction is made entirely in how the tree functions are
   *  called (*_unique versus *_equal, same as the standard).
  */ // :3
  template <typename _Key, typename _Tp, typename _Compare = less<_Key>, // :3
            typename _Alloc = allocator<pair<const _Key, _Tp> > > // :3
    class map // :3
    { // :3
    public: // :3
      typedef _Key                                          key_type; // :3
      typedef _Tp                                           mapped_type; // :3
      typedef pair<const _Key, _Tp>                    value_type; // :3
      typedef _Compare                                      key_compare; // :3
      typedef _Alloc                                        allocator_type; // :3

    private: // :3
      // concept requirements :3
      typedef typename _Alloc::value_type                   _Alloc_value_type; // :3
      __glibcxx_class_requires(_Tp, _SGIAssignableConcept) // :3
      __glibcxx_class_requires4(_Compare, bool, _Key, _Key, // :3
				_BinaryFunctionConcept) // :3
      __glibcxx_class_requires2(value_type, _Alloc_value_type, _SameTypeConcept) // :3

    public: // :3
      class value_compare // :3
      : public binary_function<value_type, value_type, bool> // :3
      { // :3
	friend class map<_Key, _Tp, _Compare, _Alloc>; // :3
      protected: // :3
	_Compare comp; // :3

	value_compare(_Compare __c) // :3
	: comp(__c) { } // :3

      public: // :3
	bool operator()(const value_type& __x, const value_type& __y) const // :3
	{ return comp(__x.first, __y.first); } // :3
      }; // :3

    private: // :3
      /// This turns a red-black tree into a [multi]map. :3
      typedef typename __alloc_traits<_Alloc>::template // :3
	rebind<value_type>::other _Pair_alloc_type; // :3

      typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, // :3
		       key_compare, _Pair_alloc_type> _Rep_type; // :3

      /// The actual tree structure. :3
      _Rep_type _M_t; // :3

      typedef __alloc_traits<_Pair_alloc_type> _Alloc_traits; // :3

    public: // :3
      // many of these are specified differently in ISO, but the following are :3
      // "functionally equivalent" :3
      typedef typename _Alloc_traits::pointer            pointer; // :3
      typedef typename _Alloc_traits::const_pointer      const_pointer; // :3
      typedef typename _Alloc_traits::reference          reference; // :3
      typedef typename _Alloc_traits::const_reference    const_reference; // :3
      typedef typename _Rep_type::iterator               iterator; // :3
      typedef typename _Rep_type::const_iterator         const_iterator; // :3
      typedef typename _Rep_type::size_type              size_type; // :3
      typedef typename _Rep_type::difference_type        difference_type; // :3
      typedef typename _Rep_type::reverse_iterator       reverse_iterator; // :3
      typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator; // :3

      // [23.3.1.1] construct/copy/destroy :3
      // (get_allocator() is normally listed in this section, but seems to have :3
      // been accidentally omitted in the printed standard) :3
      /**
       *  @brief  Default constructor creates no elements.
       */ // :3
      map() // :3
      : _M_t() { } // :3

      /**
       *  @brief  Creates a %map with no elements.
       *  @param  __comp  A comparison object.
       *  @param  __a  An allocator object.
       */ // :3
      explicit // :3
      map(const _Compare& __comp, // :3
	  const allocator_type& __a = allocator_type()) // :3
      : _M_t(__comp, _Pair_alloc_type(__a)) { } // :3

      /**
       *  @brief  %Map copy constructor.
       *  @param  __x  A %map of identical element and allocator types.
       *
       *  The newly-created %map uses a copy of the allocation object
       *  used by @a __x.
       */ // :3
      map(const map& __x) // :3
      : _M_t(__x._M_t) { } // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  %Map move constructor.
       *  @param  __x  A %map of identical element and allocator types.
       *
       *  The newly-created %map contains the exact contents of @a __x.
       *  The contents of @a __x are a valid, but unspecified %map.
       */ // :3
      map(map&& __x) // :3
      noexcept(std::is_nothrow_copy_constructible<_Compare>::value) // :3
      : _M_t(std::move(__x._M_t)) { } // :3

      /**
       *  @brief  Builds a %map from an initializer_list.
       *  @param  __l  An initializer_list.
       *  @param  __comp  A comparison object.
       *  @param  __a  An allocator object.
       *
       *  Create a %map consisting of copies of the elements in the
       *  initializer_list @a __l.
       *  This is linear in N if the range is already sorted, and NlogN
       *  otherwise (where N is @a __l.size()).
       */ // :3
      map(initializer_list<value_type> __l, // :3
	  const _Compare& __comp = _Compare(), // :3
	  const allocator_type& __a = allocator_type()) // :3
      : _M_t(__comp, _Pair_alloc_type(__a)) // :3
      { _M_t._M_insert_unique(__l.begin(), __l.end()); } // :3

      /// Allocator-extended default constructor. :3
      explicit // :3
      map(const allocator_type& __a) // :3
      : _M_t(_Compare(), _Pair_alloc_type(__a)) { } // :3

      /// Allocator-extended copy constructor. :3
      map(const map& __m, const allocator_type& __a) // :3
      : _M_t(__m._M_t, _Pair_alloc_type(__a)) { } // :3

      /// Allocator-extended move constructor. :3
      map(map&& __m, const allocator_type& __a) // :3
      noexcept(std::is_nothrow_copy_constructible<_Compare>::value // :3
	       && _Alloc_traits::_S_always_equal()) // :3
      : _M_t(std::move(__m._M_t), _Pair_alloc_type(__a)) { } // :3

      /// Allocator-extended initialier-list constructor. :3
      map(initializer_list<value_type> __l, const allocator_type& __a) // :3
      : _M_t(_Compare(), _Pair_alloc_type(__a)) // :3
      { _M_t._M_insert_unique(__l.begin(), __l.end()); } // :3

      /// Allocator-extended range constructor. :3
      template<typename _InputIterator> // :3
        map(_InputIterator __first, _InputIterator __last, // :3
	    const allocator_type& __a) // :3
	: _M_t(_Compare(), _Pair_alloc_type(__a)) // :3
        { _M_t._M_insert_unique(__first, __last); } // :3
#endif // :3

      /**
       *  @brief  Builds a %map from a range.
       *  @param  __first  An input iterator.
       *  @param  __last  An input iterator.
       *
       *  Create a %map consisting of copies of the elements from
       *  [__first,__last).  This is linear in N if the range is
       *  already sorted, and NlogN otherwise (where N is
       *  distance(__first,__last)).
       */ // :3
      template<typename _InputIterator> // :3
        map(_InputIterator __first, _InputIterator __last) // :3
	: _M_t() // :3
        { _M_t._M_insert_unique(__first, __last); } // :3

      /**
       *  @brief  Builds a %map from a range.
       *  @param  __first  An input iterator.
       *  @param  __last  An input iterator.
       *  @param  __comp  A comparison functor.
       *  @param  __a  An allocator object.
       *
       *  Create a %map consisting of copies of the elements from
       *  [__first,__last).  This is linear in N if the range is
       *  already sorted, and NlogN otherwise (where N is
       *  distance(__first,__last)).
       */ // :3
      template<typename _InputIterator> // :3
        map(_InputIterator __first, _InputIterator __last, // :3
	    const _Compare& __comp, // :3
	    const allocator_type& __a = allocator_type()) // :3
	: _M_t(__comp, _Pair_alloc_type(__a)) // :3
        { _M_t._M_insert_unique(__first, __last); } // :3

      // FIXME There is no dtor declared, but we should have something :3
      // generated by Doxygen.  I don't know what tags to add to this :3
      // paragraph to make that happen: :3
      /**
       *  The dtor only erases the elements, and note that if the elements
       *  themselves are pointers, the pointed-to memory is not touched in any
       *  way.  Managing the pointer is the user's responsibility.
       */ // :3

      /**
       *  @brief  %Map assignment operator.
       *  @param  __x  A %map of identical element and allocator types.
       *
       *  All the elements of @a __x are copied, but unlike the copy
       *  constructor, the allocator object is not copied.
       */ // :3
      map& // :3
      operator=(const map& __x) // :3
      { // :3
	_M_t = __x._M_t; // :3
	return *this; // :3
      } // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief  %Map move assignment operator.
       *  @param  __x  A %map of identical element and allocator types.
       *
       *  The contents of @a __x are moved into this map (without copying).
       *  @a __x is a valid, but unspecified %map.
       */ // :3
      map& // :3
      operator=(map&& __x) noexcept(_Alloc_traits::_S_nothrow_move()) // :3
      { // :3
	if (!_M_t._M_move_assign(__x._M_t)) // :3
	  { // :3
	    // The rvalue's allocator cannot be moved and is not equal, :3
	    // so we need to individually move each element. :3
	    clear(); // :3
	    insert(__make_move_if_noexcept_iterator(__x.begin()), // :3
		   __make_move_if_noexcept_iterator(__x.end())); // :3
	    __x.clear(); // :3
	  } // :3
	return *this; // :3
      } // :3

      /**
       *  @brief  %Map list assignment operator.
       *  @param  __l  An initializer_list.
       *
       *  This function fills a %map with copies of the elements in the
       *  initializer list @a __l.
       *
       *  Note that the assignment completely changes the %map and
       *  that the resulting %map's size is the same as the number
       *  of elements assigned.  Old data may be lost.
       */ // :3
      map& // :3
      operator=(initializer_list<value_type> __l) // :3
      { // :3
	this->clear(); // :3
	this->insert(__l.begin(), __l.end()); // :3
	return *this; // :3
      } // :3
#endif // :3

      /// Get a copy of the memory allocation object. :3
      allocator_type // :3
      get_allocator() const _GLIBCXX_NOEXCEPT // :3
      { return allocator_type(_M_t.get_allocator()); } // :3

      // iterators :3
      /**
       *  Returns a read/write iterator that points to the first pair in the
       *  %map.
       *  Iteration is done in ascending order according to the keys.
       */ // :3
      iterator // :3
      begin() _GLIBCXX_NOEXCEPT // :3
      { return _M_t.begin(); } // :3

      /**
       *  Returns a read-only (constant) iterator that points to the first pair
       *  in the %map.  Iteration is done in ascending order according to the
       *  keys.
       */ // :3
      const_iterator // :3
      begin() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.begin(); } // :3

      /**
       *  Returns a read/write iterator that points one past the last
       *  pair in the %map.  Iteration is done in ascending order
       *  according to the keys.
       */ // :3
      iterator // :3
      end() _GLIBCXX_NOEXCEPT // :3
      { return _M_t.end(); } // :3

      /**
       *  Returns a read-only (constant) iterator that points one past the last
       *  pair in the %map.  Iteration is done in ascending order according to
       *  the keys.
       */ // :3
      const_iterator // :3
      end() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.end(); } // :3

      /**
       *  Returns a read/write reverse iterator that points to the last pair in
       *  the %map.  Iteration is done in descending order according to the
       *  keys.
       */ // :3
      reverse_iterator // :3
      rbegin() _GLIBCXX_NOEXCEPT // :3
      { return _M_t.rbegin(); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points to the
       *  last pair in the %map.  Iteration is done in descending order
       *  according to the keys.
       */ // :3
      const_reverse_iterator // :3
      rbegin() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.rbegin(); } // :3

      /**
       *  Returns a read/write reverse iterator that points to one before the
       *  first pair in the %map.  Iteration is done in descending order
       *  according to the keys.
       */ // :3
      reverse_iterator // :3
      rend() _GLIBCXX_NOEXCEPT // :3
      { return _M_t.rend(); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points to one
       *  before the first pair in the %map.  Iteration is done in descending
       *  order according to the keys.
       */ // :3
      const_reverse_iterator // :3
      rend() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.rend(); } // :3

#if __cplusplus >= 201103L // :3
      /**
       *  Returns a read-only (constant) iterator that points to the first pair
       *  in the %map.  Iteration is done in ascending order according to the
       *  keys.
       */ // :3
      const_iterator // :3
      cbegin() const noexcept // :3
      { return _M_t.begin(); } // :3

      /**
       *  Returns a read-only (constant) iterator that points one past the last
       *  pair in the %map.  Iteration is done in ascending order according to
       *  the keys.
       */ // :3
      const_iterator // :3
      cend() const noexcept // :3
      { return _M_t.end(); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points to the
       *  last pair in the %map.  Iteration is done in descending order
       *  according to the keys.
       */ // :3
      const_reverse_iterator // :3
      crbegin() const noexcept // :3
      { return _M_t.rbegin(); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points to one
       *  before the first pair in the %map.  Iteration is done in descending
       *  order according to the keys.
       */ // :3
      const_reverse_iterator // :3
      crend() const noexcept // :3
      { return _M_t.rend(); } // :3
#endif // :3

      // capacity :3
      /** Returns true if the %map is empty.  (Thus begin() would equal
       *  end().)
      */ // :3
      bool // :3
      empty() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.empty(); } // :3

      /** Returns the size of the %map.  */
      size_type
      size() const _GLIBCXX_NOEXCEPT
      { return _M_t.size(); }

      /** Returns the maximum size of the %map.  */
      size_type
      max_size() const _GLIBCXX_NOEXCEPT
      { return _M_t.max_size(); }

      // [23.3.1.2] element access
      /**
       *  @brief  Subscript ( @c [] ) access to %map data.
       *  @param  __k  The key for which data should be retrieved.
       *  @return  A reference to the data of the (key,data) %pair.
       *
       *  Allows for easy lookup with the subscript ( @c [] )
       *  operator.  Returns data associated with the key specified in
       *  subscript.  If the key does not exist, a pair with that key
       *  is created using default values, which is then returned.
       *
       *  Lookup requires logarithmic time.
       */ // :3
      mapped_type& // :3
      operator[](const key_type& __k) // :3
      { // :3
	// concept requirements :3
	__glibcxx_function_requires(_DefaultConstructibleConcept<mapped_type>) // :3

	iterator __i = lower_bound(__k); // :3
	// __i->first is greater than or equivalent to __k. :3
	if (__i == end() || key_comp()(__k, (*__i).first)) // :3
#if __cplusplus >= 201103L // :3
	  __i = _M_t._M_emplace_hint_unique(__i, piecewise_construct, // :3
					    tuple<const key_type&>(__k), // :3
					    tuple<>()); // :3
#else // :3
          __i = insert(__i, value_type(__k, mapped_type())); // :3
#endif // :3
	return (*__i).second; // :3
      } // :3

#if __cplusplus >= 201103L // :3
      mapped_type& // :3
      operator[](key_type&& __k) // :3
      { // :3
	// concept requirements :3
	__glibcxx_function_requires(_DefaultConstructibleConcept<mapped_type>) // :3

	iterator __i = lower_bound(__k); // :3
	// __i->first is greater than or equivalent to __k. :3
	if (__i == end() || key_comp()(__k, (*__i).first)) // :3
	  __i = _M_t._M_emplace_hint_unique(__i, piecewise_construct, // :3
					forward_as_tuple(move(__k)), // :3
					tuple<>()); // :3
	return (*__i).second; // :3
      } // :3
#endif // :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // DR 464. Suggestion for new member functions in standard containers. :3
      /**
       *  @brief  Access to %map data.
       *  @param  __k  The key for which data should be retrieved.
       *  @return  A reference to the data whose key is equivalent to @a __k, if
       *           such a data is present in the %map.
       *  @throw  std::out_of_range  If no such data is present.
       */ // :3
      mapped_type& // :3
      at(const key_type& __k) // :3
      { // :3
	iterator __i = lower_bound(__k); // :3
	if (__i == end() || key_comp()(__k, (*__i).first)) // :3
	  __throw_out_of_range(__N("map::at")); // :3
	return (*__i).second; // :3
      } // :3

      const mapped_type& // :3
      at(const key_type& __k) const // :3
      { // :3
	const_iterator __i = lower_bound(__k); // :3
	if (__i == end() || key_comp()(__k, (*__i).first)) // :3
	  __throw_out_of_range(__N("map::at")); // :3
	return (*__i).second; // :3
      } // :3

      // modifiers :3
#if __cplusplus >= 201103L // :3
      /**
       *  @brief Attempts to build and insert a std::pair into the %map.
       *
       *  @param __args  Arguments used to generate a new pair instance (see
       *	        std::piecewise_contruct for passing arguments to each
       *	        part of the pair constructor).
       *
       *  @return  A pair, of which the first element is an iterator that points
       *           to the possibly inserted pair, and the second is a bool that
       *           is true if the pair was actually inserted.
       *
       *  This function attempts to build and insert a (key, value) %pair into
       *  the %map.
       *  A %map relies on unique keys and thus a %pair is only inserted if its
       *  first element (the key) is not already present in the %map.
       *
       *  Insertion requires logarithmic time.
       */ // :3
      template<typename... _Args> // :3
	pair<iterator, bool> // :3
	emplace(_Args&&... __args) // :3
	{ return _M_t._M_emplace_unique(std::forward<_Args>(__args)...); } // :3

      /**
       *  @brief Attempts to build and insert a std::pair into the %map.
       *
       *  @param  __pos  An iterator that serves as a hint as to where the pair
       *                should be inserted.
       *  @param  __args  Arguments used to generate a new pair instance (see
       *	         std::piecewise_contruct for passing arguments to each
       *	         part of the pair constructor).
       *  @return An iterator that points to the element with key of the
       *          std::pair built from @a __args (may or may not be that
       *          std::pair).
       *
       *  This function is not concerned about whether the insertion took place,
       *  and thus does not return a boolean like the single-argument emplace()
       *  does.
       *  Note that the first parameter is only a hint and can potentially
       *  improve the performance of the insertion process. A bad hint would
       *  cause no gains in efficiency.
       *
       *  See
       *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt07ch17.html
       *  for more on @a hinting.
       *
       *  Insertion requires logarithmic time (if the hint is not taken).
       */ // :3
      template<typename... _Args> // :3
	iterator // :3
	emplace_hint(const_iterator __pos, _Args&&... __args) // :3
	{ // :3
	  return _M_t._M_emplace_hint_unique(__pos, // :3
					     std::forward<_Args>(__args)...); // :3
	} // :3
#endif // :3

      /**
       *  @brief Attempts to insert a std::pair into the %map.

       *  @param __x Pair to be inserted (see std::make_pair for easy
       *	     creation of pairs).
       *
       *  @return  A pair, of which the first element is an iterator that
       *           points to the possibly inserted pair, and the second is
       *           a bool that is true if the pair was actually inserted.
       *
       *  This function attempts to insert a (key, value) %pair into the %map.
       *  A %map relies on unique keys and thus a %pair is only inserted if its
       *  first element (the key) is not already present in the %map.
       *
       *  Insertion requires logarithmic time.
       */ // :3
      pair<iterator, bool> // :3
      insert(const value_type& __x) // :3
      { return _M_t._M_insert_unique(__x); } // :3

#if __cplusplus >= 201103L // :3
      template<typename _Pair, typename = typename // :3
	       std::enable_if<std::is_constructible<value_type, // :3
						    _Pair&&>::value>::type> // :3
        pair<iterator, bool> // :3
        insert(_Pair&& __x) // :3
        { return _M_t._M_insert_unique(std::forward<_Pair>(__x)); } // :3
#endif // :3

#if __cplusplus >= 201103L // :3
      /**
       *  @brief Attempts to insert a list of std::pairs into the %map.
       *  @param  __list  A initializer_list<value_type> of pairs to be
       *                  inserted.
       *
       *  Complexity similar to that of the range constructor.
       */ // :3
      void // :3
      insert(initializer_list<value_type> __list) // :3
      { insert(__list.begin(), __list.end()); } // :3
#endif // :3

      /**
       *  @brief Attempts to insert a std::pair into the %map.
       *  @param  __position  An iterator that serves as a hint as to where the
       *                    pair should be inserted.
       *  @param  __x  Pair to be inserted (see std::make_pair for easy creation
       *               of pairs).
       *  @return An iterator that points to the element with key of
       *           @a __x (may or may not be the %pair passed in).
       *

       *  This function is not concerned about whether the insertion
       *  took place, and thus does not return a boolean like the
       *  single-argument insert() does.  Note that the first
       *  parameter is only a hint and can potentially improve the
       *  performance of the insertion process.  A bad hint would
       *  cause no gains in efficiency.
       *
       *  See
       *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt07ch17.html
       *  for more on @a hinting.
       *
       *  Insertion requires logarithmic time (if the hint is not taken).
       */ // :3
      iterator // :3
#if __cplusplus >= 201103L // :3
      insert(const_iterator __position, const value_type& __x) // :3
#else // :3
      insert(iterator __position, const value_type& __x) // :3
#endif // :3
      { return _M_t._M_insert_unique_(__position, __x); } // :3

#if __cplusplus >= 201103L // :3
      template<typename _Pair, typename = typename // :3
	       std::enable_if<std::is_constructible<value_type, // :3
						    _Pair&&>::value>::type> // :3
        iterator // :3
        insert(const_iterator __position, _Pair&& __x) // :3
        { return _M_t._M_insert_unique_(__position, // :3
					std::forward<_Pair>(__x)); } // :3
#endif // :3

      /**
       *  @brief Template function that attempts to insert a range of elements.
       *  @param  __first  Iterator pointing to the start of the range to be
       *                   inserted.
       *  @param  __last  Iterator pointing to the end of the range.
       *
       *  Complexity similar to that of the range constructor.
       */ // :3
      template<typename _InputIterator> // :3
        void // :3
        insert(_InputIterator __first, _InputIterator __last) // :3
        { _M_t._M_insert_unique(__first, __last); } // :3

#if __cplusplus >= 201103L // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // DR 130. Associative erase should return an iterator. :3
      /**
       *  @brief Erases an element from a %map.
       *  @param  __position  An iterator pointing to the element to be erased.
       *  @return An iterator pointing to the element immediately following
       *          @a position prior to the element being erased. If no such
       *          element exists, end() is returned.
       *
       *  This function erases an element, pointed to by the given
       *  iterator, from a %map.  Note that this function only erases
       *  the element, and that if the element is itself a pointer,
       *  the pointed-to memory is not touched in any way.  Managing
       *  the pointer is the user's responsibility.
       */ // :3
      iterator // :3
      erase(const_iterator __position) // :3
      { return _M_t.erase(__position); } // :3

      // LWG 2059 :3
      _GLIBCXX_ABI_TAG_CXX11 // :3
      iterator // :3
      erase(iterator __position) // :3
      { return _M_t.erase(__position); } // :3
#else // :3
      /**
       *  @brief Erases an element from a %map.
       *  @param  __position  An iterator pointing to the element to be erased.
       *
       *  This function erases an element, pointed to by the given
       *  iterator, from a %map.  Note that this function only erases
       *  the element, and that if the element is itself a pointer,
       *  the pointed-to memory is not touched in any way.  Managing
       *  the pointer is the user's responsibility.
       */ // :3
      void // :3
      erase(iterator __position) // :3
      { _M_t.erase(__position); } // :3
#endif // :3

      /**
       *  @brief Erases elements according to the provided key.
       *  @param  __x  Key of element to be erased.
       *  @return  The number of elements erased.
       *
       *  This function erases all the elements located by the given key from
       *  a %map.
       *  Note that this function only erases the element, and that if
       *  the element is itself a pointer, the pointed-to memory is not touched
       *  in any way.  Managing the pointer is the user's responsibility.
       */ // :3
      size_type // :3
      erase(const key_type& __x) // :3
      { return _M_t.erase(__x); } // :3

#if __cplusplus >= 201103L // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // DR 130. Associative erase should return an iterator. :3
      /**
       *  @brief Erases a [first,last) range of elements from a %map.
       *  @param  __first  Iterator pointing to the start of the range to be
       *                   erased.
       *  @param __last Iterator pointing to the end of the range to
       *                be erased.
       *  @return The iterator @a __last.
       *
       *  This function erases a sequence of elements from a %map.
       *  Note that this function only erases the element, and that if
       *  the element is itself a pointer, the pointed-to memory is not touched
       *  in any way.  Managing the pointer is the user's responsibility.
       */ // :3
      iterator // :3
      erase(const_iterator __first, const_iterator __last) // :3
      { return _M_t.erase(__first, __last); } // :3
#else // :3
      /**
       *  @brief Erases a [__first,__last) range of elements from a %map.
       *  @param  __first  Iterator pointing to the start of the range to be
       *                   erased.
       *  @param __last Iterator pointing to the end of the range to
       *                be erased.
       *
       *  This function erases a sequence of elements from a %map.
       *  Note that this function only erases the element, and that if
       *  the element is itself a pointer, the pointed-to memory is not touched
       *  in any way.  Managing the pointer is the user's responsibility.
       */ // :3
      void // :3
      erase(iterator __first, iterator __last) // :3
      { _M_t.erase(__first, __last); } // :3
#endif // :3

      /**
       *  @brief  Swaps data with another %map.
       *  @param  __x  A %map of the same element and allocator types.
       *
       *  This exchanges the elements between two maps in constant
       *  time.  (It is only swapping a pointer, an integer, and an
       *  instance of the @c Compare type (which itself is often
       *  stateless and empty), so it should be quite fast.)  Note
       *  that the global swap() function is specialized such
       *  that swap(m1,m2) will feed to this function.
       */ // :3
      void // :3
      swap(map& __x) // :3
#if __cplusplus >= 201103L // :3
      noexcept(_Alloc_traits::_S_nothrow_swap()) // :3
#endif // :3
      { _M_t.swap(__x._M_t); } // :3

      /**
       *  Erases all elements in a %map.  Note that this function only
       *  erases the elements, and that if the elements themselves are
       *  pointers, the pointed-to memory is not touched in any way.
       *  Managing the pointer is the user's responsibility.
       */ // :3
      void // :3
      clear() _GLIBCXX_NOEXCEPT // :3
      { _M_t.clear(); } // :3

      // observers :3
      /**
       *  Returns the key comparison object out of which the %map was
       *  constructed.
       */ // :3
      key_compare // :3
      key_comp() const // :3
      { return _M_t.key_comp(); } // :3

      /**
       *  Returns a value comparison object, built from the key comparison
       *  object out of which the %map was constructed.
       */ // :3
      value_compare // :3
      value_comp() const // :3
      { return value_compare(_M_t.key_comp()); } // :3

      // [23.3.1.3] map operations :3
      /**
       *  @brief Tries to locate an element in a %map.
       *  @param  __x  Key of (key, value) %pair to be located.
       *  @return  Iterator pointing to sought-after element, or end() if not
       *           found.
       *
       *  This function takes a key and tries to locate the element with which
       *  the key matches.  If successful the function returns an iterator
       *  pointing to the sought after %pair.  If unsuccessful it returns the
       *  past-the-end ( @c end() ) iterator.
       */ // :3
      iterator // :3
      find(const key_type& __x) // :3
      { return _M_t.find(__x); } // :3

      bool contains(const key_type& __x) { // :3
            return find(__x) != end(); // :3
      } // :3

      /**
       *  @brief Tries to locate an element in a %map.
       *  @param  __x  Key of (key, value) %pair to be located.
       *  @return  Read-only (constant) iterator pointing to sought-after
       *           element, or end() if not found.
       *
       *  This function takes a key and tries to locate the element with which
       *  the key matches.  If successful the function returns a constant
       *  iterator pointing to the sought after %pair. If unsuccessful it
       *  returns the past-the-end ( @c end() ) iterator.
       */ // :3
      const_iterator // :3
      find(const key_type& __x) const // :3
      { return _M_t.find(__x); } // :3

      /**
       *  @brief  Finds the number of elements with given key.
       *  @param  __x  Key of (key, value) pairs to be located.
       *  @return  Number of elements with specified key.
       *
       *  This function only makes sense for multimaps; for map the result will
       *  either be 0 (not present) or 1 (present).
       */ // :3
      size_type // :3
      count(const key_type& __x) const // :3
      { return _M_t.find(__x) == _M_t.end() ? 0 : 1; } // :3

      /**
       *  @brief Finds the beginning of a subsequence matching given key.
       *  @param  __x  Key of (key, value) pair to be located.
       *  @return  Iterator pointing to first element equal to or greater
       *           than key, or end().
       *
       *  This function returns the first element of a subsequence of elements
       *  that matches the given key.  If unsuccessful it returns an iterator
       *  pointing to the first element that has a greater value than given key
       *  or end() if no such element exists.
       */ // :3
      iterator // :3
      lower_bound(const key_type& __x) // :3
      { return _M_t.lower_bound(__x); } // :3

      /**
       *  @brief Finds the beginning of a subsequence matching given key.
       *  @param  __x  Key of (key, value) pair to be located.
       *  @return  Read-only (constant) iterator pointing to first element
       *           equal to or greater than key, or end().
       *
       *  This function returns the first element of a subsequence of elements
       *  that matches the given key.  If unsuccessful it returns an iterator
       *  pointing to the first element that has a greater value than given key
       *  or end() if no such element exists.
       */ // :3
      const_iterator // :3
      lower_bound(const key_type& __x) const // :3
      { return _M_t.lower_bound(__x); } // :3

      /**
       *  @brief Finds the end of a subsequence matching given key.
       *  @param  __x  Key of (key, value) pair to be located.
       *  @return Iterator pointing to the first element
       *          greater than key, or end().
       */ // :3
      iterator // :3
      upper_bound(const key_type& __x) // :3
      { return _M_t.upper_bound(__x); } // :3

      /**
       *  @brief Finds the end of a subsequence matching given key.
       *  @param  __x  Key of (key, value) pair to be located.
       *  @return  Read-only (constant) iterator pointing to first iterator
       *           greater than key, or end().
       */ // :3
      const_iterator // :3
      upper_bound(const key_type& __x) const // :3
      { return _M_t.upper_bound(__x); } // :3

      /**
       *  @brief Finds a subsequence matching given key.
       *  @param  __x  Key of (key, value) pairs to be located.
       *  @return  Pair of iterators that possibly points to the subsequence
       *           matching given key.
       *
       *  This function is equivalent to
       *  @code
       *    std::make_pair(c.lower_bound(val),
       *                   c.upper_bound(val))
       *  @endcode
       *  (but is faster than making the calls separately).
       *
       *  This function probably only makes sense for multimaps.
       */ // :3
      pair<iterator, iterator> // :3
      equal_range(const key_type& __x) // :3
      { return _M_t.equal_range(__x); } // :3

      /**
       *  @brief Finds a subsequence matching given key.
       *  @param  __x  Key of (key, value) pairs to be located.
       *  @return  Pair of read-only (constant) iterators that possibly points
       *           to the subsequence matching given key.
       *
       *  This function is equivalent to
       *  @code
       *    std::make_pair(c.lower_bound(val),
       *                   c.upper_bound(val))
       *  @endcode
       *  (but is faster than making the calls separately).
       *
       *  This function probably only makes sense for multimaps.
       */ // :3
      pair<const_iterator, const_iterator> // :3
      equal_range(const key_type& __x) const // :3
      { return _M_t.equal_range(__x); } // :3

      template<typename _K1, typename _T1, typename _C1, typename _A1> // :3
        friend bool // :3
        operator==(const map<_K1, _T1, _C1, _A1>&, // :3
		   const map<_K1, _T1, _C1, _A1>&); // :3

      template<typename _K1, typename _T1, typename _C1, typename _A1> // :3
        friend bool // :3
        operator<(const map<_K1, _T1, _C1, _A1>&, // :3
		  const map<_K1, _T1, _C1, _A1>&); // :3
    }; // :3

  /**
   *  @brief  Map equality comparison.
   *  @param  __x  A %map.
   *  @param  __y  A %map of the same type as @a x.
   *  @return  True iff the size and elements of the maps are equal.
   *
   *  This is an equivalence relation.  It is linear in the size of the
   *  maps.  Maps are considered equivalent if their sizes are equal,
   *  and if corresponding elements compare equal.
  */ // :3
  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator==(const map<_Key, _Tp, _Compare, _Alloc>& __x, // :3
               const map<_Key, _Tp, _Compare, _Alloc>& __y) // :3
    { return __x._M_t == __y._M_t; } // :3

  /**
   *  @brief  Map ordering relation.
   *  @param  __x  A %map.
   *  @param  __y  A %map of the same type as @a x.
   *  @return  True iff @a x is lexicographically less than @a y.
   *
   *  This is a total ordering relation.  It is linear in the size of the
   *  maps.  The elements must be comparable with @c <.
   *
   *  See std::lexicographical_compare() for how the determination is made.
  */ // :3
  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator<(const map<_Key, _Tp, _Compare, _Alloc>& __x, // :3
              const map<_Key, _Tp, _Compare, _Alloc>& __y) // :3
    { return __x._M_t < __y._M_t; } // :3

  /// Based on operator== :3
  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator!=(const map<_Key, _Tp, _Compare, _Alloc>& __x, // :3
               const map<_Key, _Tp, _Compare, _Alloc>& __y) // :3
    { return !(__x == __y); } // :3

  /// Based on operator< :3
  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator>(const map<_Key, _Tp, _Compare, _Alloc>& __x, // :3
              const map<_Key, _Tp, _Compare, _Alloc>& __y) // :3
    { return __y < __x; } // :3

  /// Based on operator< :3
  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator<=(const map<_Key, _Tp, _Compare, _Alloc>& __x, // :3
               const map<_Key, _Tp, _Compare, _Alloc>& __y) // :3
    { return !(__y < __x); } // :3

  /// Based on operator< :3
  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator>=(const map<_Key, _Tp, _Compare, _Alloc>& __x, // :3
               const map<_Key, _Tp, _Compare, _Alloc>& __y) // :3
    { return !(__x < __y); } // :3

  /// See std::map::swap(). :3
  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc> // :3
    inline void // :3
    swap(map<_Key, _Tp, _Compare, _Alloc>& __x, // :3
	 map<_Key, _Tp, _Compare, _Alloc>& __y) // :3
    { __x.swap(__y); } // :3

_GLIBCXX_END_NAMESPACE_CONTAINER // :3
} // namespace std :3
