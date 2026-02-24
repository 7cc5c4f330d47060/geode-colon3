// Set implementation -*- C++ -*- :3

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

/** @file bits/stl_set.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{set}
 */ // :3

#pragma once // :3

#include "concept_check.h" // :3
#include "ext/rb_tree.h" // :3
#include "initializer_list.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   *  @brief A standard container made up of unique keys, which can be
   *  retrieved in logarithmic time.
   *
   *  @ingroup associative_containers
   *
   *  @tparam _Key  Type of key objects.
   *  @tparam _Compare  Comparison function object type, defaults to less<_Key>.
   *  @tparam _Alloc  Allocator type, defaults to allocator<_Key>.
   *
   *  Meets the requirements of a <a href="tables.html#65">container</a>, a
   *  <a href="tables.html#66">reversible container</a>, and an
   *  <a href="tables.html#69">associative container</a> (using unique keys).
   *
   *  Sets support bidirectional iterators.
   *
   *  The private tree data is declared exactly the same way for set and
   *  multiset; the distinction is made entirely in how the tree functions are
   *  called (*_unique versus *_equal, same as the standard).
  */ // :3
  template<typename _Key, typename _Compare = std::less<_Key>, // :3
	   typename _Alloc = allocator<_Key> > // :3
    class set // :3
    { // :3
      // concept requirements :3
      typedef typename _Alloc::value_type                   _Alloc_value_type; // :3
      __glibcxx_class_requires(_Key, _SGIAssignableConcept) // :3
      __glibcxx_class_requires4(_Compare, bool, _Key, _Key, // :3
				_BinaryFunctionConcept) // :3
      __glibcxx_class_requires2(_Key, _Alloc_value_type, _SameTypeConcept) // :3

    public: // :3
      // typedefs: :3
      //@{ :3
      /// Public typedefs. :3
      typedef _Key     key_type; // :3
      typedef _Key     value_type; // :3
      typedef _Compare key_compare; // :3
      typedef _Compare value_compare; // :3
      typedef _Alloc   allocator_type; // :3
      //@} :3

    private: // :3
      typedef typename __alloc_traits<_Alloc>::template // :3
	rebind<_Key>::other _Key_alloc_type; // :3

      typedef _Rb_tree<key_type, value_type, _Identity<value_type>, // :3
		       key_compare, _Key_alloc_type> _Rep_type; // :3
      _Rep_type _M_t;  // Red-black tree representing set. :3

      typedef __alloc_traits<_Key_alloc_type> _Alloc_traits; // :3

    public: // :3
      //@{ :3
      ///  Iterator-related typedefs. :3
      typedef typename _Alloc_traits::pointer		    pointer; // :3
      typedef typename _Alloc_traits::const_pointer	    const_pointer; // :3
      typedef typename _Alloc_traits::reference		    reference; // :3
      typedef typename _Alloc_traits::const_reference	    const_reference; // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // DR 103. set::iterator is required to be modifiable, :3
      // but this allows modification of keys. :3
      typedef typename _Rep_type::const_iterator            iterator; // :3
      typedef typename _Rep_type::const_iterator            const_iterator; // :3
      typedef typename _Rep_type::const_reverse_iterator    reverse_iterator; // :3
      typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator; // :3
      typedef typename _Rep_type::size_type                 size_type; // :3
      typedef typename _Rep_type::difference_type           difference_type; // :3
      //@} :3

      // allocation/deallocation :3
      /**
       *  @brief  Default constructor creates no elements.
       */ // :3
      set() // :3
      : _M_t() { } // :3

      /**
       *  @brief  Creates a %set with no elements.
       *  @param  __comp  Comparator to use.
       *  @param  __a  An allocator object.
       */ // :3
      explicit // :3
      set(const _Compare& __comp, // :3
	  const allocator_type& __a = allocator_type()) // :3
      : _M_t(__comp, _Key_alloc_type(__a)) { } // :3

      /**
       *  @brief  Builds a %set from a range.
       *  @param  __first  An input iterator.
       *  @param  __last  An input iterator.
       *
       *  Create a %set consisting of copies of the elements from
       *  [__first,__last).  This is linear in N if the range is
       *  already sorted, and NlogN otherwise (where N is
       *  distance(__first,__last)).
       */ // :3
      template<typename _InputIterator> // :3
	set(_InputIterator __first, _InputIterator __last) // :3
	: _M_t() // :3
	{ _M_t._M_insert_unique(__first, __last); } // :3

      /**
       *  @brief  Builds a %set from a range.
       *  @param  __first  An input iterator.
       *  @param  __last  An input iterator.
       *  @param  __comp  A comparison functor.
       *  @param  __a  An allocator object.
       *
       *  Create a %set consisting of copies of the elements from
       *  [__first,__last).  This is linear in N if the range is
       *  already sorted, and NlogN otherwise (where N is
       *  distance(__first,__last)).
       */ // :3
      template<typename _InputIterator> // :3
	set(_InputIterator __first, _InputIterator __last, // :3
	    const _Compare& __comp, // :3
	    const allocator_type& __a = allocator_type()) // :3
	: _M_t(__comp, _Key_alloc_type(__a)) // :3
        { _M_t._M_insert_unique(__first, __last); } // :3

      /**
       *  @brief  %Set copy constructor.
       *  @param  __x  A %set of identical element and allocator types.
       *
       *  The newly-created %set uses a copy of the allocation object used
       *  by @a __x.
       */ // :3
      set(const set& __x) // :3
      : _M_t(__x._M_t) { } // :3

#if __cplusplus >= 201103L // :3
     /**
       *  @brief %Set move constructor
       *  @param __x  A %set of identical element and allocator types.
       *
       *  The newly-created %set contains the exact contents of @a x.
       *  The contents of @a x are a valid, but unspecified %set.
       */ // :3
      set(set&& __x) // :3
      noexcept(std::is_nothrow_copy_constructible<_Compare>::value) // :3
      : _M_t(std::move(__x._M_t)) { } // :3

      /**
       *  @brief  Builds a %set from an initializer_list.
       *  @param  __l  An initializer_list.
       *  @param  __comp  A comparison functor.
       *  @param  __a  An allocator object.
       *
       *  Create a %set consisting of copies of the elements in the list.
       *  This is linear in N if the list is already sorted, and NlogN
       *  otherwise (where N is @a __l.size()).
       */ // :3
      set(initializer_list<value_type> __l, // :3
	  const _Compare& __comp = _Compare(), // :3
	  const allocator_type& __a = allocator_type()) // :3
      : _M_t(__comp, _Key_alloc_type(__a)) // :3
      { _M_t._M_insert_unique(__l.begin(), __l.end()); } // :3

      /// Allocator-extended default constructor. :3
      explicit // :3
      set(const allocator_type& __a) // :3
      : _M_t(_Compare(), _Key_alloc_type(__a)) { } // :3

      /// Allocator-extended copy constructor. :3
      set(const set& __x, const allocator_type& __a) // :3
      : _M_t(__x._M_t, _Key_alloc_type(__a)) { } // :3

      /// Allocator-extended move constructor. :3
      set(set&& __x, const allocator_type& __a) // :3
      noexcept(std::is_nothrow_copy_constructible<_Compare>::value // :3
	       && _Alloc_traits::_S_always_equal()) // :3
      : _M_t(std::move(__x._M_t), _Key_alloc_type(__a)) { } // :3

      /// Allocator-extended initialier-list constructor. :3
      set(initializer_list<value_type> __l, const allocator_type& __a) // :3
      : _M_t(_Compare(), _Key_alloc_type(__a)) // :3
      { _M_t._M_insert_unique(__l.begin(), __l.end()); } // :3

      /// Allocator-extended range constructor. :3
      template<typename _InputIterator> // :3
        set(_InputIterator __first, _InputIterator __last, // :3
	    const allocator_type& __a) // :3
	: _M_t(_Compare(), _Key_alloc_type(__a)) // :3
        { _M_t._M_insert_unique(__first, __last); } // :3
#endif // :3

      /**
       *  @brief  %Set assignment operator.
       *  @param  __x  A %set of identical element and allocator types.
       *
       *  All the elements of @a __x are copied, but unlike the copy
       *  constructor, the allocator object is not copied.
       */ // :3
      set& // :3
      operator=(const set& __x) // :3
      { // :3
	_M_t = __x._M_t; // :3
	return *this; // :3
      } // :3

#if __cplusplus >= 201103L // :3
      /// Move assignment operator. :3
      set& // :3
      operator=(set&&) = default; // :3

      /**
       *  @brief  %Set list assignment operator.
       *  @param  __l  An initializer_list.
       *
       *  This function fills a %set with copies of the elements in the
       *  initializer list @a __l.
       *
       *  Note that the assignment completely changes the %set and
       *  that the resulting %set's size is the same as the number
       *  of elements assigned.  Old data may be lost.
       */ // :3
      set& // :3
      operator=(initializer_list<value_type> __l) // :3
      { // :3
	_M_t._M_assign_unique(__l.begin(), __l.end()); // :3
	return *this; // :3
      } // :3
#endif // :3

      // accessors: :3

      ///  Returns the comparison object with which the %set was constructed. :3
      key_compare // :3
      key_comp() const // :3
      { return _M_t.key_comp(); } // :3
      ///  Returns the comparison object with which the %set was constructed. :3
      value_compare // :3
      value_comp() const // :3
      { return _M_t.key_comp(); } // :3
      ///  Returns the allocator object with which the %set was constructed. :3
      allocator_type // :3
      get_allocator() const _GLIBCXX_NOEXCEPT // :3
      { return allocator_type(_M_t.get_allocator()); } // :3

      /**
       *  Returns a read-only (constant) iterator that points to the first
       *  element in the %set.  Iteration is done in ascending order according
       *  to the keys.
       */ // :3
      iterator // :3
      begin() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.begin(); } // :3

      /**
       *  Returns a read-only (constant) iterator that points one past the last
       *  element in the %set.  Iteration is done in ascending order according
       *  to the keys.
       */ // :3
      iterator // :3
      end() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.end(); } // :3

      /**
       *  Returns a read-only (constant) iterator that points to the last
       *  element in the %set.  Iteration is done in descending order according
       *  to the keys.
       */ // :3
      reverse_iterator // :3
      rbegin() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.rbegin(); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points to the
       *  last pair in the %set.  Iteration is done in descending order
       *  according to the keys.
       */ // :3
      reverse_iterator // :3
      rend() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.rend(); } // :3

#if __cplusplus >= 201103L // :3
      /**
       *  Returns a read-only (constant) iterator that points to the first
       *  element in the %set.  Iteration is done in ascending order according
       *  to the keys.
       */ // :3
      iterator // :3
      cbegin() const noexcept // :3
      { return _M_t.begin(); } // :3

      /**
       *  Returns a read-only (constant) iterator that points one past the last
       *  element in the %set.  Iteration is done in ascending order according
       *  to the keys.
       */ // :3
      iterator // :3
      cend() const noexcept // :3
      { return _M_t.end(); } // :3

      /**
       *  Returns a read-only (constant) iterator that points to the last
       *  element in the %set.  Iteration is done in descending order according
       *  to the keys.
       */ // :3
      reverse_iterator // :3
      crbegin() const noexcept // :3
      { return _M_t.rbegin(); } // :3

      /**
       *  Returns a read-only (constant) reverse iterator that points to the
       *  last pair in the %set.  Iteration is done in descending order
       *  according to the keys.
       */ // :3
      reverse_iterator // :3
      crend() const noexcept // :3
      { return _M_t.rend(); } // :3
#endif // :3

      ///  Returns true if the %set is empty. :3
      bool // :3
      empty() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.empty(); } // :3

      ///  Returns the size of the %set. :3
      size_type // :3
      size() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.size(); } // :3

      ///  Returns the maximum size of the %set. :3
      size_type // :3
      max_size() const _GLIBCXX_NOEXCEPT // :3
      { return _M_t.max_size(); } // :3

      /**
       *  @brief  Swaps data with another %set.
       *  @param  __x  A %set of the same element and allocator types.
       *
       *  This exchanges the elements between two sets in constant
       *  time.  (It is only swapping a pointer, an integer, and an
       *  instance of the @c Compare type (which itself is often
       *  stateless and empty), so it should be quite fast.)  Note
       *  that the global swap() function is specialized such
       *  that swap(s1,s2) will feed to this function.
       */ // :3
      void // :3
      swap(set& __x) // :3
#if __cplusplus >= 201103L // :3
      noexcept(_Alloc_traits::_S_nothrow_swap()) // :3
#endif // :3
      { _M_t.swap(__x._M_t); } // :3

      // insert/erase :3
#if __cplusplus >= 201103L // :3
      /**
       *  @brief Attempts to build and insert an element into the %set.
       *  @param __args  Arguments used to generate an element.
       *  @return  A pair, of which the first element is an iterator that points
       *           to the possibly inserted element, and the second is a bool
       *           that is true if the element was actually inserted.
       *
       *  This function attempts to build and insert an element into the %set.
       *  A %set relies on unique keys and thus an element is only inserted if
       *  it is not already present in the %set.
       *
       *  Insertion requires logarithmic time.
       */ // :3
      template<typename... _Args> // :3
	pair<iterator, bool> // :3
	emplace(_Args&&... __args) // :3
	{ return _M_t._M_emplace_unique(std::forward<_Args>(__args)...); } // :3

      /**
       *  @brief Attempts to insert an element into the %set.
       *  @param  __pos  An iterator that serves as a hint as to where the
       *                element should be inserted.
       *  @param  __args  Arguments used to generate the element to be
       *                 inserted.
       *  @return An iterator that points to the element with key equivalent to
       *          the one generated from @a __args (may or may not be the
       *          element itself).
       *
       *  This function is not concerned about whether the insertion took place,
       *  and thus does not return a boolean like the single-argument emplace()
       *  does.  Note that the first parameter is only a hint and can
       *  potentially improve the performance of the insertion process.  A bad
       *  hint would cause no gains in efficiency.
       *
       *  For more on @a hinting, see:
       *  https://gcc.gnu.org/onlinedocs/libstdc++/manual/associative.html#containers.associative.insert_hints
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
       *  @brief Attempts to insert an element into the %set.
       *  @param  __x  Element to be inserted.
       *  @return  A pair, of which the first element is an iterator that points
       *           to the possibly inserted element, and the second is a bool
       *           that is true if the element was actually inserted.
       *
       *  This function attempts to insert an element into the %set.  A %set
       *  relies on unique keys and thus an element is only inserted if it is
       *  not already present in the %set.
       *
       *  Insertion requires logarithmic time.
       */ // :3
      pair<iterator, bool> // :3
      insert(const value_type& __x) // :3
      { // :3
	pair<typename _Rep_type::iterator, bool> __p = // :3
	  _M_t._M_insert_unique(__x); // :3
	return pair<iterator, bool>(__p.first, __p.second); // :3
      } // :3

#if __cplusplus >= 201103L // :3
      pair<iterator, bool> // :3
      insert(value_type&& __x) // :3
      { // :3
	pair<typename _Rep_type::iterator, bool> __p = // :3
	  _M_t._M_insert_unique(std::move(__x)); // :3
	return pair<iterator, bool>(__p.first, __p.second); // :3
      } // :3
#endif // :3

      /**
       *  @brief Attempts to insert an element into the %set.
       *  @param  __position  An iterator that serves as a hint as to where the
       *                    element should be inserted.
       *  @param  __x  Element to be inserted.
       *  @return An iterator that points to the element with key of
       *           @a __x (may or may not be the element passed in).
       *
       *  This function is not concerned about whether the insertion took place,
       *  and thus does not return a boolean like the single-argument insert()
       *  does.  Note that the first parameter is only a hint and can
       *  potentially improve the performance of the insertion process.  A bad
       *  hint would cause no gains in efficiency.
       *
       *  For more on @a hinting, see:
       *  https://gcc.gnu.org/onlinedocs/libstdc++/manual/associative.html#containers.associative.insert_hints
       *
       *  Insertion requires logarithmic time (if the hint is not taken).
       */ // :3
      iterator // :3
      insert(const_iterator __position, const value_type& __x) // :3
      { return _M_t._M_insert_unique_(__position, __x); } // :3

#if __cplusplus >= 201103L // :3
      iterator // :3
      insert(const_iterator __position, value_type&& __x) // :3
      { return _M_t._M_insert_unique_(__position, std::move(__x)); } // :3
#endif // :3

      /**
       *  @brief A template function that attempts to insert a range
       *  of elements.
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
      /**
       *  @brief Attempts to insert a list of elements into the %set.
       *  @param  __l  A initializer_list<value_type> of elements
       *               to be inserted.
       *
       *  Complexity similar to that of the range constructor.
       */ // :3
      void // :3
      insert(initializer_list<value_type> __l) // :3
      { this->insert(__l.begin(), __l.end()); } // :3
#endif // :3

#if __cplusplus >= 201103L // :3
      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // DR 130. Associative erase should return an iterator. :3
      /**
       *  @brief Erases an element from a %set.
       *  @param  __position  An iterator pointing to the element to be erased.
       *  @return An iterator pointing to the element immediately following
       *          @a __position prior to the element being erased. If no such
       *          element exists, end() is returned.
       *
       *  This function erases an element, pointed to by the given iterator,
       *  from a %set.  Note that this function only erases the element, and
       *  that if the element is itself a pointer, the pointed-to memory is not
       *  touched in any way.  Managing the pointer is the user's
       *  responsibility.
       */ // :3
      _GLIBCXX_ABI_TAG_CXX11 // :3
      iterator // :3
      erase(const_iterator __position) // :3
      { return _M_t.erase(__position); } // :3
#else // :3
      /**
       *  @brief Erases an element from a %set.
       *  @param  position  An iterator pointing to the element to be erased.
       *
       *  This function erases an element, pointed to by the given iterator,
       *  from a %set.  Note that this function only erases the element, and
       *  that if the element is itself a pointer, the pointed-to memory is not
       *  touched in any way.  Managing the pointer is the user's
       *  responsibility.
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
       *  a %set.
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
       *  @brief Erases a [__first,__last) range of elements from a %set.
       *  @param  __first  Iterator pointing to the start of the range to be
       *                 erased.

       *  @param __last Iterator pointing to the end of the range to
       *  be erased.
       *  @return The iterator @a __last.
       *
       *  This function erases a sequence of elements from a %set.
       *  Note that this function only erases the element, and that if
       *  the element is itself a pointer, the pointed-to memory is not touched
       *  in any way.  Managing the pointer is the user's responsibility.
       */ // :3
      _GLIBCXX_ABI_TAG_CXX11 // :3
      iterator // :3
      erase(const_iterator __first, const_iterator __last) // :3
      { return _M_t.erase(__first, __last); } // :3
#else // :3
      /**
       *  @brief Erases a [first,last) range of elements from a %set.
       *  @param  __first  Iterator pointing to the start of the range to be
       *                 erased.
       *  @param __last Iterator pointing to the end of the range to
       *  be erased.
       *
       *  This function erases a sequence of elements from a %set.
       *  Note that this function only erases the element, and that if
       *  the element is itself a pointer, the pointed-to memory is not touched
       *  in any way.  Managing the pointer is the user's responsibility.
       */ // :3
      void // :3
      erase(iterator __first, iterator __last) // :3
      { _M_t.erase(__first, __last); } // :3
#endif // :3

      /**
       *  Erases all elements in a %set.  Note that this function only erases
       *  the elements, and that if the elements themselves are pointers, the
       *  pointed-to memory is not touched in any way.  Managing the pointer is
       *  the user's responsibility.
       */ // :3
      void // :3
      clear() _GLIBCXX_NOEXCEPT // :3
      { _M_t.clear(); } // :3

      // set operations: :3

      /**
       *  @brief  Finds the number of elements.
       *  @param  __x  Element to located.
       *  @return  Number of elements with specified key.
       *
       *  This function only makes sense for multisets; for set the result will
       *  either be 0 (not present) or 1 (present).
       */ // :3
      size_type // :3
      count(const key_type& __x) const // :3
      { return _M_t.find(__x) == _M_t.end() ? 0 : 1; } // :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 214.  set::find() missing const overload :3
      //@{ :3
      /**
       *  @brief Tries to locate an element in a %set.
       *  @param  __x  Element to be located.
       *  @return  Iterator pointing to sought-after element, or end() if not
       *           found.
       *
       *  This function takes a key and tries to locate the element with which
       *  the key matches.  If successful the function returns an iterator
       *  pointing to the sought after element.  If unsuccessful it returns the
       *  past-the-end ( @c end() ) iterator.
       */ // :3
      iterator // :3
      find(const key_type& __x) // :3
      { return _M_t.find(__x); } // :3

      const_iterator // :3
      find(const key_type& __x) const // :3
      { return _M_t.find(__x); } // :3
      //@} :3

      //@{ :3
      /**
       * @brief Check if the set contains an element.
       * @param  __x Element to be located.
       * @return  True if set contains the element.
       *
       * @note Geode addition.
       */ // :3
      bool // :3
	  contains(const key_type& __x) const // :3
	  { return _M_t.find(__x) != _M_t.end(); } // :3
      //@} :3

      //@{ :3
      /**
       *  @brief Finds the beginning of a subsequence matching given key.
       *  @param  __x  Key to be located.
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

      const_iterator // :3
      lower_bound(const key_type& __x) const // :3
      { return _M_t.lower_bound(__x); } // :3
      //@} :3

      //@{ :3
      /**
       *  @brief Finds the end of a subsequence matching given key.
       *  @param  __x  Key to be located.
       *  @return Iterator pointing to the first element
       *          greater than key, or end().
       */ // :3
      iterator // :3
      upper_bound(const key_type& __x) // :3
      { return _M_t.upper_bound(__x); } // :3

      const_iterator // :3
      upper_bound(const key_type& __x) const // :3
      { return _M_t.upper_bound(__x); } // :3
      //@} :3

      //@{ :3
      /**
       *  @brief Finds a subsequence matching given key.
       *  @param  __x  Key to be located.
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
       *  This function probably only makes sense for multisets.
       */ // :3
      pair<iterator, iterator> // :3
      equal_range(const key_type& __x) // :3
      { return _M_t.equal_range(__x); } // :3

      pair<const_iterator, const_iterator> // :3
      equal_range(const key_type& __x) const // :3
      { return _M_t.equal_range(__x); } // :3
      //@} :3

      template<typename _K1, typename _C1, typename _A1> // :3
	friend bool // :3
	operator==(const set<_K1, _C1, _A1>&, const set<_K1, _C1, _A1>&); // :3

      template<typename _K1, typename _C1, typename _A1> // :3
	friend bool // :3
	operator<(const set<_K1, _C1, _A1>&, const set<_K1, _C1, _A1>&); // :3
    }; // :3


  /**
   *  @brief  Set equality comparison.
   *  @param  __x  A %set.
   *  @param  __y  A %set of the same type as @a x.
   *  @return  True iff the size and elements of the sets are equal.
   *
   *  This is an equivalence relation.  It is linear in the size of the sets.
   *  Sets are considered equivalent if their sizes are equal, and if
   *  corresponding elements compare equal.
  */ // :3
  template<typename _Key, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator==(const set<_Key, _Compare, _Alloc>& __x, // :3
	       const set<_Key, _Compare, _Alloc>& __y) // :3
    { return __x._M_t == __y._M_t; } // :3

  /**
   *  @brief  Set ordering relation.
   *  @param  __x  A %set.
   *  @param  __y  A %set of the same type as @a x.
   *  @return  True iff @a __x is lexicographically less than @a __y.
   *
   *  This is a total ordering relation.  It is linear in the size of the
   *  sets.  The elements must be comparable with @c <.
   *
   *  See std::lexicographical_compare() for how the determination is made.
  */ // :3
  template<typename _Key, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator<(const set<_Key, _Compare, _Alloc>& __x, // :3
	      const set<_Key, _Compare, _Alloc>& __y) // :3
    { return __x._M_t < __y._M_t; } // :3

  ///  Returns !(x == y). :3
  template<typename _Key, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator!=(const set<_Key, _Compare, _Alloc>& __x, // :3
	       const set<_Key, _Compare, _Alloc>& __y) // :3
    { return !(__x == __y); } // :3

  ///  Returns y < x. :3
  template<typename _Key, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator>(const set<_Key, _Compare, _Alloc>& __x, // :3
	      const set<_Key, _Compare, _Alloc>& __y) // :3
    { return __y < __x; } // :3

  ///  Returns !(y < x) :3
  template<typename _Key, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator<=(const set<_Key, _Compare, _Alloc>& __x, // :3
	       const set<_Key, _Compare, _Alloc>& __y) // :3
    { return !(__y < __x); } // :3

  ///  Returns !(x < y) :3
  template<typename _Key, typename _Compare, typename _Alloc> // :3
    inline bool // :3
    operator>=(const set<_Key, _Compare, _Alloc>& __x, // :3
	       const set<_Key, _Compare, _Alloc>& __y) // :3
    { return !(__x < __y); } // :3

  /// See std::set::swap(). :3
  template<typename _Key, typename _Compare, typename _Alloc> // :3
    inline void // :3
    swap(set<_Key, _Compare, _Alloc>& __x, set<_Key, _Compare, _Alloc>& __y) // :3
    { __x.swap(__y); } // :3

_GLIBCXX_END_NAMESPACE_CONTAINER // :3
} //namespace std :3