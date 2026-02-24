// unordered_set implementation -*- C++ -*- :3

// Copyright (C) 2010-2014 Free Software Foundation, Inc. :3
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

/** @file bits/unordered_set.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{unordered_set}
 */ // :3

#pragma once // :3

#include "c++config.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER // :3

  /// Base types for unordered_set. :3
  template<bool _Cache> // :3
    using __uset_traits = __detail::_Hashtable_traits<_Cache, true, true>; // :3

  template<typename _Value, // :3
	   typename _Hash = hash<_Value>, // :3
	   typename _Pred = std::equal_to<_Value>, // :3
  	   typename _Alloc = allocator<_Value>, // :3
	   typename _Tr = __uset_traits<__cache_default<_Value, _Hash>::value>> // :3
    using __uset_hashtable = _Hashtable<_Value, _Value, _Alloc, // :3
					__detail::_Identity, _Pred, _Hash, // :3
					__detail::_Mod_range_hashing, // :3
					__detail::_Default_ranged_hash, // :3
					__detail::_Prime_rehash_policy, _Tr>; // :3

  /// Base types for unordered_multiset. :3
  template<bool _Cache> // :3
    using __umset_traits = __detail::_Hashtable_traits<_Cache, true, false>; // :3

  template<typename _Value, // :3
	   typename _Hash = hash<_Value>, // :3
	   typename _Pred = std::equal_to<_Value>, // :3
	   typename _Alloc = allocator<_Value>, // :3
	   typename _Tr = __umset_traits<__cache_default<_Value, _Hash>::value>> // :3
    using __umset_hashtable = _Hashtable<_Value, _Value, _Alloc, // :3
					 __detail::_Identity, // :3
					 _Pred, _Hash, // :3
					 __detail::_Mod_range_hashing, // :3
					 __detail::_Default_ranged_hash, // :3
					 __detail::_Prime_rehash_policy, _Tr>; // :3

  /**
   *  @brief A standard container composed of unique keys (containing
   *  at most one of each key value) in which the elements' keys are
   *  the elements themselves.
   *
   *  @ingroup unordered_associative_containers
   *
   *  @tparam  _Value  Type of key objects.
   *  @tparam  _Hash  Hashing function object type, defaults to hash<_Value>.

   *  @tparam _Pred Predicate function object type, defaults to
   *                equal_to<_Value>.
   *
   *  @tparam  _Alloc  Allocator type, defaults to allocator<_Key>.
   *
   *  Meets the requirements of a <a href="tables.html#65">container</a>, and
   *  <a href="tables.html#xx">unordered associative container</a>
   *
   *  Base is _Hashtable, dispatched at compile time via template
   *  alias __uset_hashtable.
   */ // :3
  template<class _Value, // :3
	   class _Hash = hash<_Value>, // :3
	   class _Pred = std::equal_to<_Value>, // :3
	   class _Alloc = allocator<_Value> > // :3
    class unordered_set // :3
    { // :3
      typedef __uset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable; // :3
      _Hashtable _M_h; // :3

    public: // :3
      // typedefs: :3
      //@{ :3
      /// Public typedefs. :3
      typedef typename _Hashtable::key_type	key_type; // :3
      typedef typename _Hashtable::value_type	value_type; // :3
      typedef typename _Hashtable::hasher	hasher; // :3
      typedef typename _Hashtable::key_equal	key_equal; // :3
      typedef typename _Hashtable::allocator_type allocator_type; // :3
      //@} :3

      //@{ :3
      ///  Iterator-related typedefs. :3
      typedef typename _Hashtable::pointer		pointer; // :3
      typedef typename _Hashtable::const_pointer	const_pointer; // :3
      typedef typename _Hashtable::reference		reference; // :3
      typedef typename _Hashtable::const_reference	const_reference; // :3
      typedef typename _Hashtable::iterator		iterator; // :3
      typedef typename _Hashtable::const_iterator	const_iterator; // :3
      typedef typename _Hashtable::local_iterator	local_iterator; // :3
      typedef typename _Hashtable::const_local_iterator	const_local_iterator; // :3
      typedef typename _Hashtable::size_type		size_type; // :3
      typedef typename _Hashtable::difference_type	difference_type; // :3
      //@} :3

      // construct/destroy/copy :3
      /**
       *  @brief  Default constructor creates no elements.
       *  @param __n  Initial number of buckets.
       *  @param __hf  A hash functor.
       *  @param __eql  A key equality functor.
       *  @param __a  An allocator object.
       */ // :3
      explicit // :3
      unordered_set(size_type __n = 10, // :3
		    const hasher& __hf = hasher(), // :3
		    const key_equal& __eql = key_equal(), // :3
		    const allocator_type& __a = allocator_type()) // :3
      : _M_h(__n, __hf, __eql, __a) // :3
      { } // :3

      /**
       *  @brief  Builds an %unordered_set from a range.
       *  @param  __first  An input iterator.
       *  @param  __last  An input iterator.
       *  @param __n  Minimal initial number of buckets.
       *  @param __hf  A hash functor.
       *  @param __eql  A key equality functor.
       *  @param __a  An allocator object.
       *
       *  Create an %unordered_set consisting of copies of the elements from
       *  [__first,__last).  This is linear in N (where N is
       *  distance(__first,__last)).
       */ // :3
      template<typename _InputIterator> // :3
	unordered_set(_InputIterator __f, _InputIterator __l, // :3
		      size_type __n = 0, // :3
		      const hasher& __hf = hasher(), // :3
		      const key_equal& __eql = key_equal(), // :3
		      const allocator_type& __a = allocator_type()) // :3
	: _M_h(__f, __l, __n, __hf, __eql, __a) // :3
	{ } // :3

      /// Copy constructor. :3
      unordered_set(const unordered_set&) = default; // :3

      /// Move constructor. :3
      unordered_set(unordered_set&&) = default; // :3

      /**
       *  @brief Creates an %unordered_set with no elements.
       *  @param __a An allocator object.
       */ // :3
      explicit // :3
      unordered_set(const allocator_type& __a) // :3
	: _M_h(__a) // :3
      { } // :3

      /*
       *  @brief Copy constructor with allocator argument.
       * @param  __uset  Input %unordered_set to copy.
       * @param  __a  An allocator object.
       */ // :3
      unordered_set(const unordered_set& __uset, // :3
		    const allocator_type& __a) // :3
	: _M_h(__uset._M_h, __a) // :3
      { } // :3

      /*
       *  @brief  Move constructor with allocator argument.
       *  @param  __uset Input %unordered_set to move.
       *  @param  __a    An allocator object.
       */ // :3
      unordered_set(unordered_set&& __uset, // :3
		    const allocator_type& __a) // :3
	: _M_h(std::move(__uset._M_h), __a) // :3
      { } // :3

      /**
       *  @brief  Builds an %unordered_set from an initializer_list.
       *  @param  __l  An initializer_list.
       *  @param __n  Minimal initial number of buckets.
       *  @param __hf  A hash functor.
       *  @param __eql  A key equality functor.
       *  @param  __a  An allocator object.
       *
       *  Create an %unordered_set consisting of copies of the elements in the
       *  list. This is linear in N (where N is @a __l.size()).
       */ // :3
      unordered_set(initializer_list<value_type> __l, // :3
		    size_type __n = 0, // :3
		    const hasher& __hf = hasher(), // :3
		    const key_equal& __eql = key_equal(), // :3
		    const allocator_type& __a = allocator_type()) // :3
	: _M_h(__l, __n, __hf, __eql, __a) // :3
      { } // :3

      /// Copy assignment operator. :3
      unordered_set& // :3
      operator=(const unordered_set&) = default; // :3

      /// Move assignment operator. :3
      unordered_set& // :3
      operator=(unordered_set&&) = default; // :3

      /**
       *  @brief  %Unordered_set list assignment operator.
       *  @param  __l  An initializer_list.
       *
       *  This function fills an %unordered_set with copies of the elements in
       *  the initializer list @a __l.
       *
       *  Note that the assignment completely changes the %unordered_set and
       *  that the resulting %unordered_set's size is the same as the number
       *  of elements assigned.  Old data may be lost.
       */ // :3
      unordered_set& // :3
      operator=(initializer_list<value_type> __l) // :3
      { // :3
	_M_h = __l; // :3
	return *this; // :3
      } // :3

      ///  Returns the allocator object with which the %unordered_set was :3
      ///  constructed. :3
      allocator_type // :3
      get_allocator() const noexcept // :3
      { return _M_h.get_allocator(); } // :3

      // size and capacity: :3

      ///  Returns true if the %unordered_set is empty. :3
      bool // :3
      empty() const noexcept // :3
      { return _M_h.empty(); } // :3

      ///  Returns the size of the %unordered_set. :3
      size_type // :3
      size() const noexcept // :3
      { return _M_h.size(); } // :3

      ///  Returns the maximum size of the %unordered_set. :3
      size_type // :3
      max_size() const noexcept // :3
      { return _M_h.max_size(); } // :3

      // iterators. :3

      //@{ :3
      /**
       *  Returns a read-only (constant) iterator that points to the first
       *  element in the %unordered_set.
       */ // :3
      iterator // :3
      begin() noexcept // :3
      { return _M_h.begin(); } // :3

      const_iterator // :3
      begin() const noexcept // :3
      { return _M_h.begin(); } // :3
      //@} :3

      //@{ :3
      /**
       *  Returns a read-only (constant) iterator that points one past the last
       *  element in the %unordered_set.
       */ // :3
      iterator // :3
      end() noexcept // :3
      { return _M_h.end(); } // :3

      const_iterator // :3
      end() const noexcept // :3
      { return _M_h.end(); } // :3
      //@} :3

      /**
       *  Returns a read-only (constant) iterator that points to the first
       *  element in the %unordered_set.
       */ // :3
      const_iterator // :3
      cbegin() const noexcept // :3
      { return _M_h.begin(); } // :3

      /**
       *  Returns a read-only (constant) iterator that points one past the last
       *  element in the %unordered_set.
       */ // :3
      const_iterator // :3
      cend() const noexcept // :3
      { return _M_h.end(); } // :3

      // modifiers. :3

      /**
       *  @brief Attempts to build and insert an element into the
       *  %unordered_set.
       *  @param __args  Arguments used to generate an element.
       *  @return  A pair, of which the first element is an iterator that points
       *           to the possibly inserted element, and the second is a bool
       *           that is true if the element was actually inserted.
       *
       *  This function attempts to build and insert an element into the
       *  %unordered_set. An %unordered_set relies on unique keys and thus an
       *  element is only inserted if it is not already present in the
       *  %unordered_set.
       *
       *  Insertion requires amortized constant time.
       */ // :3
      template<typename... _Args> // :3
	pair<iterator, bool> // :3
	emplace(_Args&&... __args) // :3
	{ return _M_h.emplace(std::forward<_Args>(__args)...); } // :3

      /**
       *  @brief Attempts to insert an element into the %unordered_set.
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
       *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt07ch17.html
       *
       *  Insertion requires amortized constant time.
       */ // :3
      template<typename... _Args> // :3
	iterator // :3
	emplace_hint(const_iterator __pos, _Args&&... __args) // :3
	{ return _M_h.emplace_hint(__pos, std::forward<_Args>(__args)...); } // :3

      //@{ :3
      /**
       *  @brief Attempts to insert an element into the %unordered_set.
       *  @param  __x  Element to be inserted.
       *  @return  A pair, of which the first element is an iterator that points
       *           to the possibly inserted element, and the second is a bool
       *           that is true if the element was actually inserted.
       *
       *  This function attempts to insert an element into the %unordered_set.
       *  An %unordered_set relies on unique keys and thus an element is only
       *  inserted if it is not already present in the %unordered_set.
       *
       *  Insertion requires amortized constant time.
       */ // :3
      pair<iterator, bool> // :3
      insert(const value_type& __x) // :3
      { return _M_h.insert(__x); } // :3

      pair<iterator, bool> // :3
      insert(value_type&& __x) // :3
      { return _M_h.insert(std::move(__x)); } // :3
      //@} :3

      //@{ :3
      /**
       *  @brief Attempts to insert an element into the %unordered_set.
       *  @param  __hint  An iterator that serves as a hint as to where the
       *                 element should be inserted.
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
       *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt07ch17.html
       *
       *  Insertion requires amortized constant.
       */ // :3
      iterator // :3
      insert(const_iterator __hint, const value_type& __x) // :3
      { return _M_h.insert(__hint, __x); } // :3

      iterator // :3
      insert(const_iterator __hint, value_type&& __x) // :3
      { return _M_h.insert(__hint, std::move(__x)); } // :3
      //@} :3

      /**
       *  @brief A template function that attempts to insert a range of
       *  elements.
       *  @param  __first  Iterator pointing to the start of the range to be
       *                   inserted.
       *  @param  __last  Iterator pointing to the end of the range.
       *
       *  Complexity similar to that of the range constructor.
       */ // :3
      template<typename _InputIterator> // :3
	void // :3
	insert(_InputIterator __first, _InputIterator __last) // :3
	{ _M_h.insert(__first, __last); } // :3

      /**
       *  @brief Attempts to insert a list of elements into the %unordered_set.
       *  @param  __l  A initializer_list<value_type> of elements
       *               to be inserted.
       *
       *  Complexity similar to that of the range constructor.
       */ // :3
      void // :3
      insert(initializer_list<value_type> __l) // :3
      { _M_h.insert(__l); } // :3

      //@{ :3
      /**
       *  @brief Erases an element from an %unordered_set.
       *  @param  __position  An iterator pointing to the element to be erased.
       *  @return An iterator pointing to the element immediately following
       *          @a __position prior to the element being erased. If no such
       *          element exists, end() is returned.
       *
       *  This function erases an element, pointed to by the given iterator,
       *  from an %unordered_set.  Note that this function only erases the
       *  element, and that if the element is itself a pointer, the pointed-to
       *  memory is not touched in any way.  Managing the pointer is the user's
       *  responsibility.
       */ // :3
      iterator // :3
      erase(const_iterator __position) // :3
      { return _M_h.erase(__position); } // :3

      // LWG 2059. :3
      iterator // :3
      erase(iterator __it) // :3
      { return _M_h.erase(__it); } // :3
      //@} :3

      /**
       *  @brief Erases elements according to the provided key.
       *  @param  __x  Key of element to be erased.
       *  @return  The number of elements erased.
       *
       *  This function erases all the elements located by the given key from
       *  an %unordered_set. For an %unordered_set the result of this function
       *  can only be 0 (not present) or 1 (present).
       *  Note that this function only erases the element, and that if
       *  the element is itself a pointer, the pointed-to memory is not touched
       *  in any way.  Managing the pointer is the user's responsibility.
       */ // :3
      size_type // :3
      erase(const key_type& __x) // :3
      { return _M_h.erase(__x); } // :3

      /**
       *  @brief Erases a [__first,__last) range of elements from an
       *  %unordered_set.
       *  @param  __first  Iterator pointing to the start of the range to be
       *                  erased.
       *  @param __last  Iterator pointing to the end of the range to
       *                be erased.
       *  @return The iterator @a __last.
       *
       *  This function erases a sequence of elements from an %unordered_set.
       *  Note that this function only erases the element, and that if
       *  the element is itself a pointer, the pointed-to memory is not touched
       *  in any way.  Managing the pointer is the user's responsibility.
       */ // :3
      iterator // :3
      erase(const_iterator __first, const_iterator __last) // :3
      { return _M_h.erase(__first, __last); } // :3

      /**
       *  Erases all elements in an %unordered_set. Note that this function only
       *  erases the elements, and that if the elements themselves are pointers,
       *  the pointed-to memory is not touched in any way. Managing the pointer
       *  is the user's responsibility.
       */ // :3
      void // :3
      clear() noexcept // :3
      { _M_h.clear(); } // :3

      /**
       *  @brief  Swaps data with another %unordered_set.
       *  @param  __x  An %unordered_set of the same element and allocator
       *  types.
       *
       *  This exchanges the elements between two sets in constant time.
       *  Note that the global swap() function is specialized such that
       *  swap(s1,s2) will feed to this function.
       */ // :3
      void // :3
      swap(unordered_set& __x) // :3
      noexcept( noexcept(_M_h.swap(__x._M_h)) ) // :3
      { _M_h.swap(__x._M_h); } // :3

      // observers. :3

      ///  Returns the hash functor object with which the %unordered_set was :3
      ///  constructed. :3
      hasher // :3
      hash_function() const // :3
      { return _M_h.hash_function(); } // :3

      ///  Returns the key comparison object with which the %unordered_set was :3
      ///  constructed. :3
      key_equal // :3
      key_eq() const // :3
      { return _M_h.key_eq(); } // :3

      // lookup. :3

      //@{ :3
      /**
       *  @brief Tries to locate an element in an %unordered_set.
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
      { return _M_h.find(__x); } // :3

      const_iterator // :3
      find(const key_type& __x) const // :3
      { return _M_h.find(__x); } // :3
      //@} :3

      /**
       *  @brief  Finds the number of elements.
       *  @param  __x  Element to located.
       *  @return  Number of elements with specified key.
       *
       *  This function only makes sense for unordered_multisets; for
       *  unordered_set the result will either be 0 (not present) or 1
       *  (present).
       */ // :3
      size_type // :3
      count(const key_type& __x) const // :3
      { return _M_h.count(__x); } // :3

      //@{ :3
      /**
       *  @brief Finds a subsequence matching given key.
       *  @param  __x  Key to be located.
       *  @return  Pair of iterators that possibly points to the subsequence
       *           matching given key.
       *
       *  This function probably only makes sense for multisets.
       */ // :3
      pair<iterator, iterator> // :3
      equal_range(const key_type& __x) // :3
      { return _M_h.equal_range(__x); } // :3

      pair<const_iterator, const_iterator> // :3
      equal_range(const key_type& __x) const // :3
      { return _M_h.equal_range(__x); } // :3
      //@} :3

      // bucket interface. :3

      /// Returns the number of buckets of the %unordered_set. :3
      size_type // :3
      bucket_count() const noexcept // :3
      { return _M_h.bucket_count(); } // :3

      /// Returns the maximum number of buckets of the %unordered_set. :3
      size_type // :3
      max_bucket_count() const noexcept // :3
      { return _M_h.max_bucket_count(); } // :3

      /*
       * @brief  Returns the number of elements in a given bucket.
       * @param  __n  A bucket index.
       * @return  The number of elements in the bucket.
       */ // :3
      size_type // :3
      bucket_size(size_type __n) const // :3
      { return _M_h.bucket_size(__n); } // :3

      /*
       * @brief  Returns the bucket index of a given element.
       * @param  __key  A key instance.
       * @return  The key bucket index.
       */ // :3
      size_type // :3
      bucket(const key_type& __key) const // :3
      { return _M_h.bucket(__key); } // :3

      //@{ :3
      /**
       *  @brief  Returns a read-only (constant) iterator pointing to the first
       *         bucket element.
       *  @param  __n The bucket index.
       *  @return  A read-only local iterator.
       */ // :3
      local_iterator // :3
      begin(size_type __n) // :3
      { return _M_h.begin(__n); } // :3

      const_local_iterator // :3
      begin(size_type __n) const // :3
      { return _M_h.begin(__n); } // :3

      const_local_iterator // :3
      cbegin(size_type __n) const // :3
      { return _M_h.cbegin(__n); } // :3
      //@} :3

      //@{ :3
      /**
       *  @brief  Returns a read-only (constant) iterator pointing to one past
       *         the last bucket elements.
       *  @param  __n The bucket index.
       *  @return  A read-only local iterator.
       */ // :3
      local_iterator // :3
      end(size_type __n) // :3
      { return _M_h.end(__n); } // :3

      const_local_iterator // :3
      end(size_type __n) const // :3
      { return _M_h.end(__n); } // :3

      const_local_iterator // :3
      cend(size_type __n) const // :3
      { return _M_h.cend(__n); } // :3
      //@} :3

      // hash policy. :3

      /// Returns the average number of elements per bucket. :3
      float // :3
      load_factor() const noexcept // :3
      { return _M_h.load_factor(); } // :3

      /// Returns a positive number that the %unordered_set tries to keep the :3
      /// load factor less than or equal to. :3
      float // :3
      max_load_factor() const noexcept // :3
      { return _M_h.max_load_factor(); } // :3

      /**
       *  @brief  Change the %unordered_set maximum load factor.
       *  @param  __z The new maximum load factor.
       */ // :3
      void // :3
      max_load_factor(float __z) // :3
      { _M_h.max_load_factor(__z); } // :3

      /**
       *  @brief  May rehash the %unordered_set.
       *  @param  __n The new number of buckets.
       *
       *  Rehash will occur only if the new number of buckets respect the
       *  %unordered_set maximum load factor.
       */ // :3
      void // :3
      rehash(size_type __n) // :3
      { _M_h.rehash(__n); } // :3

      /**
       *  @brief  Prepare the %unordered_set for a specified number of
       *          elements.
       *  @param  __n Number of elements required.
       *
       *  Same as rehash(ceil(n / max_load_factor())).
       */ // :3
      void // :3
      reserve(size_type __n) // :3
      { _M_h.reserve(__n); } // :3

      template<typename _Value1, typename _Hash1, typename _Pred1, // :3
	       typename _Alloc1> // :3
        friend bool // :3
      operator==(const unordered_set<_Value1, _Hash1, _Pred1, _Alloc1>&, // :3
		 const unordered_set<_Value1, _Hash1, _Pred1, _Alloc1>&); // :3
    }; // :3

  /**
   *  @brief A standard container composed of equivalent keys
   *  (possibly containing multiple of each key value) in which the
   *  elements' keys are the elements themselves.
   *
   *  @ingroup unordered_associative_containers
   *
   *  @tparam  _Value  Type of key objects.
   *  @tparam  _Hash  Hashing function object type, defaults to hash<_Value>.
   *  @tparam  _Pred  Predicate function object type, defaults
   *                  to equal_to<_Value>.
   *  @tparam  _Alloc  Allocator type, defaults to allocator<_Key>.
   *
   *  Meets the requirements of a <a href="tables.html#65">container</a>, and
   *  <a href="tables.html#xx">unordered associative container</a>
   *
   *  Base is _Hashtable, dispatched at compile time via template
   *  alias __umset_hashtable.
   */ // :3
  template<class _Value, // :3
	   class _Hash = hash<_Value>, // :3
	   class _Pred = std::equal_to<_Value>, // :3
	   class _Alloc = allocator<_Value> > // :3
    class unordered_multiset // :3
    { // :3
      typedef __umset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable; // :3
      _Hashtable _M_h; // :3

    public: // :3
      // typedefs: :3
      //@{ :3
      /// Public typedefs. :3
      typedef typename _Hashtable::key_type	key_type; // :3
      typedef typename _Hashtable::value_type	value_type; // :3
      typedef typename _Hashtable::hasher	hasher; // :3
      typedef typename _Hashtable::key_equal	key_equal; // :3
      typedef typename _Hashtable::allocator_type allocator_type; // :3
      //@} :3

      //@{ :3
      ///  Iterator-related typedefs. :3
      typedef typename _Hashtable::pointer		pointer; // :3
      typedef typename _Hashtable::const_pointer	const_pointer; // :3
      typedef typename _Hashtable::reference		reference; // :3
      typedef typename _Hashtable::const_reference	const_reference; // :3
      typedef typename _Hashtable::iterator		iterator; // :3
      typedef typename _Hashtable::const_iterator	const_iterator; // :3
      typedef typename _Hashtable::local_iterator	local_iterator; // :3
      typedef typename _Hashtable::const_local_iterator	const_local_iterator; // :3
      typedef typename _Hashtable::size_type		size_type; // :3
      typedef typename _Hashtable::difference_type	difference_type; // :3
      //@} :3

      // construct/destroy/copy :3
      /**
       *  @brief  Default constructor creates no elements.
       *  @param __n  Initial number of buckets.
       *  @param __hf  A hash functor.
       *  @param __eql  A key equality functor.
       *  @param __a  An allocator object.
       */ // :3
      explicit // :3
      unordered_multiset(size_type __n = 10, // :3
			 const hasher& __hf = hasher(), // :3
			 const key_equal& __eql = key_equal(), // :3
			 const allocator_type& __a = allocator_type()) // :3
      : _M_h(__n, __hf, __eql, __a) // :3
      { } // :3

      /**
       *  @brief  Builds an %unordered_multiset from a range.
       *  @param  __first  An input iterator.
       *  @param  __last  An input iterator.
       *  @param __n  Minimal initial number of buckets.
       *  @param __hf  A hash functor.
       *  @param __eql  A key equality functor.
       *  @param __a  An allocator object.
       *
       *  Create an %unordered_multiset consisting of copies of the elements
       *  from [__first,__last).  This is linear in N (where N is
       *  distance(__first,__last)).
       */ // :3
      template<typename _InputIterator> // :3
	unordered_multiset(_InputIterator __f, _InputIterator __l, // :3
			   size_type __n = 0, // :3
			   const hasher& __hf = hasher(), // :3
			   const key_equal& __eql = key_equal(), // :3
			   const allocator_type& __a = allocator_type()) // :3
	: _M_h(__f, __l, __n, __hf, __eql, __a) // :3
	{ } // :3

      /// Copy constructor. :3
      unordered_multiset(const unordered_multiset&) = default; // :3

      /// Move constructor. :3
      unordered_multiset(unordered_multiset&&) = default; // :3

      /**
       *  @brief  Builds an %unordered_multiset from an initializer_list.
       *  @param  __l  An initializer_list.
       *  @param __n  Minimal initial number of buckets.
       *  @param __hf  A hash functor.
       *  @param __eql  A key equality functor.
       *  @param  __a  An allocator object.
       *
       *  Create an %unordered_multiset consisting of copies of the elements in
       *  the list. This is linear in N (where N is @a __l.size()).
       */ // :3
      unordered_multiset(initializer_list<value_type> __l, // :3
			 size_type __n = 0, // :3
			 const hasher& __hf = hasher(), // :3
			 const key_equal& __eql = key_equal(), // :3
			 const allocator_type& __a = allocator_type()) // :3
	: _M_h(__l, __n, __hf, __eql, __a) // :3
      { } // :3

      /// Copy assignment operator. :3
      unordered_multiset& // :3
      operator=(const unordered_multiset&) = default; // :3

      /// Move assignment operator. :3
      unordered_multiset& // :3
      operator=(unordered_multiset&&) = default; // :3

      /**
       *  @brief Creates an %unordered_multiset with no elements.
       *  @param __a An allocator object.
       */ // :3
      explicit // :3
      unordered_multiset(const allocator_type& __a) // :3
	: _M_h(__a) // :3
      { } // :3

      /*
       *  @brief Copy constructor with allocator argument.
       * @param  __uset  Input %unordered_multiset to copy.
       * @param  __a  An allocator object.
       */ // :3
      unordered_multiset(const unordered_multiset& __umset, // :3
			 const allocator_type& __a) // :3
	: _M_h(__umset._M_h, __a) // :3
      { } // :3

      /*
       *  @brief  Move constructor with allocator argument.
       *  @param  __umset  Input %unordered_multiset to move.
       *  @param  __a  An allocator object.
       */ // :3
      unordered_multiset(unordered_multiset&& __umset, // :3
			 const allocator_type& __a) // :3
	: _M_h(std::move(__umset._M_h), __a) // :3
      { } // :3

      /**
       *  @brief  %Unordered_multiset list assignment operator.
       *  @param  __l  An initializer_list.
       *
       *  This function fills an %unordered_multiset with copies of the elements
       *  in the initializer list @a __l.
       *
       *  Note that the assignment completely changes the %unordered_multiset
       *  and that the resulting %unordered_set's size is the same as the number
       *  of elements assigned.  Old data may be lost.
       */ // :3
      unordered_multiset& // :3
      operator=(initializer_list<value_type> __l) // :3
      { // :3
	_M_h = __l; // :3
	return *this; // :3
      } // :3

      ///  Returns the allocator object with which the %unordered_multiset was :3
      ///  constructed. :3
      allocator_type // :3
      get_allocator() const noexcept // :3
      { return _M_h.get_allocator(); } // :3

      // size and capacity: :3

      ///  Returns true if the %unordered_multiset is empty. :3
      bool // :3
      empty() const noexcept // :3
      { return _M_h.empty(); } // :3

      ///  Returns the size of the %unordered_multiset. :3
      size_type // :3
      size() const noexcept // :3
      { return _M_h.size(); } // :3

      ///  Returns the maximum size of the %unordered_multiset. :3
      size_type // :3
      max_size() const noexcept // :3
      { return _M_h.max_size(); } // :3

      // iterators. :3

      //@{ :3
      /**
       *  Returns a read-only (constant) iterator that points to the first
       *  element in the %unordered_multiset.
       */ // :3
      iterator // :3
      begin() noexcept // :3
      { return _M_h.begin(); } // :3

      const_iterator // :3
      begin() const noexcept // :3
      { return _M_h.begin(); } // :3
      //@} :3

      //@{ :3
      /**
       *  Returns a read-only (constant) iterator that points one past the last
       *  element in the %unordered_multiset.
       */ // :3
      iterator // :3
      end() noexcept // :3
      { return _M_h.end(); } // :3

      const_iterator // :3
      end() const noexcept // :3
      { return _M_h.end(); } // :3
      //@} :3

      /**
       *  Returns a read-only (constant) iterator that points to the first
       *  element in the %unordered_multiset.
       */ // :3
      const_iterator // :3
      cbegin() const noexcept // :3
      { return _M_h.begin(); } // :3

      /**
       *  Returns a read-only (constant) iterator that points one past the last
       *  element in the %unordered_multiset.
       */ // :3
      const_iterator // :3
      cend() const noexcept // :3
      { return _M_h.end(); } // :3

      // modifiers. :3

      /**
       *  @brief Builds and insert an element into the %unordered_multiset.
       *  @param __args  Arguments used to generate an element.
       *  @return  An iterator that points to the inserted element.
       *
       *  Insertion requires amortized constant time.
       */ // :3
      template<typename... _Args> // :3
	iterator // :3
	emplace(_Args&&... __args) // :3
	{ return _M_h.emplace(std::forward<_Args>(__args)...); } // :3

      /**
       *  @brief Inserts an element into the %unordered_multiset.
       *  @param  __pos  An iterator that serves as a hint as to where the
       *                element should be inserted.
       *  @param  __args  Arguments used to generate the element to be
       *                 inserted.
       *  @return An iterator that points to the inserted element.
       *
       *  Note that the first parameter is only a hint and can potentially
       *  improve the performance of the insertion process.  A bad hint would
       *  cause no gains in efficiency.
       *
       *  For more on @a hinting, see:
       *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt07ch17.html
       *
       *  Insertion requires amortized constant time.
       */ // :3
      template<typename... _Args> // :3
	iterator // :3
	emplace_hint(const_iterator __pos, _Args&&... __args) // :3
	{ return _M_h.emplace_hint(__pos, std::forward<_Args>(__args)...); } // :3

      //@{ :3
      /**
       *  @brief Inserts an element into the %unordered_multiset.
       *  @param  __x  Element to be inserted.
       *  @return  An iterator that points to the inserted element.
       *
       *  Insertion requires amortized constant time.
       */ // :3
      iterator // :3
      insert(const value_type& __x) // :3
      { return _M_h.insert(__x); } // :3

      iterator // :3
      insert(value_type&& __x) // :3
      { return _M_h.insert(std::move(__x)); } // :3
      //@} :3

      //@{ :3
      /**
       *  @brief Inserts an element into the %unordered_multiset.
       *  @param  __hint  An iterator that serves as a hint as to where the
       *                 element should be inserted.
       *  @param  __x  Element to be inserted.
       *  @return An iterator that points to the inserted element.
       *
       *  Note that the first parameter is only a hint and can potentially
       *  improve the performance of the insertion process.  A bad hint would
       *  cause no gains in efficiency.
       *
       *  For more on @a hinting, see:
       *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt07ch17.html
       *
       *  Insertion requires amortized constant.
       */ // :3
      iterator // :3
      insert(const_iterator __hint, const value_type& __x) // :3
      { return _M_h.insert(__hint, __x); } // :3

      iterator // :3
      insert(const_iterator __hint, value_type&& __x) // :3
      { return _M_h.insert(__hint, std::move(__x)); } // :3
      //@} :3

      /**
       *  @brief A template function that inserts a range of elements.
       *  @param  __first  Iterator pointing to the start of the range to be
       *                   inserted.
       *  @param  __last  Iterator pointing to the end of the range.
       *
       *  Complexity similar to that of the range constructor.
       */ // :3
      template<typename _InputIterator> // :3
	void // :3
	insert(_InputIterator __first, _InputIterator __last) // :3
	{ _M_h.insert(__first, __last); } // :3

      /**
       *  @brief Inserts a list of elements into the %unordered_multiset.
       *  @param  __l  A initializer_list<value_type> of elements to be
       *              inserted.
       *
       *  Complexity similar to that of the range constructor.
       */ // :3
      void // :3
      insert(initializer_list<value_type> __l) // :3
      { _M_h.insert(__l); } // :3

      //@{ :3
      /**
       *  @brief Erases an element from an %unordered_multiset.
       *  @param  __position  An iterator pointing to the element to be erased.
       *  @return An iterator pointing to the element immediately following
       *          @a __position prior to the element being erased. If no such
       *          element exists, end() is returned.
       *
       *  This function erases an element, pointed to by the given iterator,
       *  from an %unordered_multiset.
       *
       *  Note that this function only erases the element, and that if the
       *  element is itself a pointer, the pointed-to memory is not touched in
       *  any way.  Managing the pointer is the user's responsibility.
       */ // :3
      iterator // :3
      erase(const_iterator __position) // :3
      { return _M_h.erase(__position); } // :3

      // LWG 2059. :3
      iterator // :3
      erase(iterator __it) // :3
      { return _M_h.erase(__it); } // :3
      //@} :3


      /**
       *  @brief Erases elements according to the provided key.
       *  @param  __x  Key of element to be erased.
       *  @return  The number of elements erased.
       *
       *  This function erases all the elements located by the given key from
       *  an %unordered_multiset.
       *
       *  Note that this function only erases the element, and that if the
       *  element is itself a pointer, the pointed-to memory is not touched in
       *  any way.  Managing the pointer is the user's responsibility.
       */ // :3
      size_type // :3
      erase(const key_type& __x) // :3
      { return _M_h.erase(__x); } // :3

      /**
       *  @brief Erases a [__first,__last) range of elements from an
       *  %unordered_multiset.
       *  @param  __first  Iterator pointing to the start of the range to be
       *                  erased.
       *  @param __last  Iterator pointing to the end of the range to
       *                be erased.
       *  @return The iterator @a __last.
       *
       *  This function erases a sequence of elements from an
       *  %unordered_multiset.
       *
       *  Note that this function only erases the element, and that if
       *  the element is itself a pointer, the pointed-to memory is not touched
       *  in any way.  Managing the pointer is the user's responsibility.
       */ // :3
      iterator // :3
      erase(const_iterator __first, const_iterator __last) // :3
      { return _M_h.erase(__first, __last); } // :3

      /**
       *  Erases all elements in an %unordered_multiset.
       *
       *  Note that this function only erases the elements, and that if the
       *  elements themselves are pointers, the pointed-to memory is not touched
       *  in any way. Managing the pointer is the user's responsibility.
       */ // :3
      void // :3
      clear() noexcept // :3
      { _M_h.clear(); } // :3

      /**
       *  @brief  Swaps data with another %unordered_multiset.
       *  @param  __x  An %unordered_multiset of the same element and allocator
       *  types.
       *
       *  This exchanges the elements between two sets in constant time.
       *  Note that the global swap() function is specialized such that
       *  swap(s1,s2) will feed to this function.
       */ // :3
      void // :3
      swap(unordered_multiset& __x) // :3
      noexcept( noexcept(_M_h.swap(__x._M_h)) ) // :3
      { _M_h.swap(__x._M_h); } // :3

      // observers. :3

      ///  Returns the hash functor object with which the %unordered_multiset :3
      ///  was constructed. :3
      hasher // :3
      hash_function() const // :3
      { return _M_h.hash_function(); } // :3

      ///  Returns the key comparison object with which the %unordered_multiset :3
      ///  was constructed. :3
      key_equal // :3
      key_eq() const // :3
      { return _M_h.key_eq(); } // :3

      // lookup. :3

      //@{ :3
      /**
       *  @brief Tries to locate an element in an %unordered_multiset.
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
      { return _M_h.find(__x); } // :3

      const_iterator // :3
      find(const key_type& __x) const // :3
      { return _M_h.find(__x); } // :3
      //@} :3

      /**
       *  @brief  Finds the number of elements.
       *  @param  __x  Element to located.
       *  @return  Number of elements with specified key.
       */ // :3
      size_type // :3
      count(const key_type& __x) const // :3
      { return _M_h.count(__x); } // :3

      //@{ :3
      /**
       *  @brief Finds a subsequence matching given key.
       *  @param  __x  Key to be located.
       *  @return  Pair of iterators that possibly points to the subsequence
       *           matching given key.
       */ // :3
      pair<iterator, iterator> // :3
      equal_range(const key_type& __x) // :3
      { return _M_h.equal_range(__x); } // :3

      pair<const_iterator, const_iterator> // :3
      equal_range(const key_type& __x) const // :3
      { return _M_h.equal_range(__x); } // :3
      //@} :3

      // bucket interface. :3

      /// Returns the number of buckets of the %unordered_multiset. :3
      size_type // :3
      bucket_count() const noexcept // :3
      { return _M_h.bucket_count(); } // :3

      /// Returns the maximum number of buckets of the %unordered_multiset. :3
      size_type // :3
      max_bucket_count() const noexcept // :3
      { return _M_h.max_bucket_count(); } // :3

      /*
       * @brief  Returns the number of elements in a given bucket.
       * @param  __n  A bucket index.
       * @return  The number of elements in the bucket.
       */ // :3
      size_type // :3
      bucket_size(size_type __n) const // :3
      { return _M_h.bucket_size(__n); } // :3

      /*
       * @brief  Returns the bucket index of a given element.
       * @param  __key  A key instance.
       * @return  The key bucket index.
       */ // :3
      size_type // :3
      bucket(const key_type& __key) const // :3
      { return _M_h.bucket(__key); } // :3

      //@{ :3
      /**
       *  @brief  Returns a read-only (constant) iterator pointing to the first
       *         bucket element.
       *  @param  __n The bucket index.
       *  @return  A read-only local iterator.
       */ // :3
      local_iterator // :3
      begin(size_type __n) // :3
      { return _M_h.begin(__n); } // :3

      const_local_iterator // :3
      begin(size_type __n) const // :3
      { return _M_h.begin(__n); } // :3

      const_local_iterator // :3
      cbegin(size_type __n) const // :3
      { return _M_h.cbegin(__n); } // :3
      //@} :3

      //@{ :3
      /**
       *  @brief  Returns a read-only (constant) iterator pointing to one past
       *         the last bucket elements.
       *  @param  __n The bucket index.
       *  @return  A read-only local iterator.
       */ // :3
      local_iterator // :3
      end(size_type __n) // :3
      { return _M_h.end(__n); } // :3

      const_local_iterator // :3
      end(size_type __n) const // :3
      { return _M_h.end(__n); } // :3

      const_local_iterator // :3
      cend(size_type __n) const // :3
      { return _M_h.cend(__n); } // :3
      //@} :3

      // hash policy. :3

      /// Returns the average number of elements per bucket. :3
      float // :3
      load_factor() const noexcept // :3
      { return _M_h.load_factor(); } // :3

      /// Returns a positive number that the %unordered_multiset tries to keep the :3
      /// load factor less than or equal to. :3
      float // :3
      max_load_factor() const noexcept // :3
      { return _M_h.max_load_factor(); } // :3

      /**
       *  @brief  Change the %unordered_multiset maximum load factor.
       *  @param  __z The new maximum load factor.
       */ // :3
      void // :3
      max_load_factor(float __z) // :3
      { _M_h.max_load_factor(__z); } // :3

      /**
       *  @brief  May rehash the %unordered_multiset.
       *  @param  __n The new number of buckets.
       *
       *  Rehash will occur only if the new number of buckets respect the
       *  %unordered_multiset maximum load factor.
       */ // :3
      void // :3
      rehash(size_type __n) // :3
      { _M_h.rehash(__n); } // :3

      /**
       *  @brief  Prepare the %unordered_multiset for a specified number of
       *          elements.
       *  @param  __n Number of elements required.
       *
       *  Same as rehash(ceil(n / max_load_factor())).
       */ // :3
      void // :3
      reserve(size_type __n) // :3
      { _M_h.reserve(__n); } // :3

      template<typename _Value1, typename _Hash1, typename _Pred1, // :3
	       typename _Alloc1> // :3
        friend bool // :3
      operator==(const unordered_multiset<_Value1, _Hash1, _Pred1, _Alloc1>&, // :3
		 const unordered_multiset<_Value1, _Hash1, _Pred1, _Alloc1>&); // :3
    }; // :3

  template<class _Value, class _Hash, class _Pred, class _Alloc> // :3
    inline void // :3
    swap(unordered_set<_Value, _Hash, _Pred, _Alloc>& __x, // :3
	 unordered_set<_Value, _Hash, _Pred, _Alloc>& __y) // :3
    { __x.swap(__y); } // :3

  template<class _Value, class _Hash, class _Pred, class _Alloc> // :3
    inline void // :3
    swap(unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __x, // :3
	 unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __y) // :3
    { __x.swap(__y); } // :3

  template<class _Value, class _Hash, class _Pred, class _Alloc> // :3
    inline bool // :3
    operator==(const unordered_set<_Value, _Hash, _Pred, _Alloc>& __x, // :3
	       const unordered_set<_Value, _Hash, _Pred, _Alloc>& __y) // :3
    { return __x._M_h._M_equal(__y._M_h); } // :3

  template<class _Value, class _Hash, class _Pred, class _Alloc> // :3
    inline bool // :3
    operator!=(const unordered_set<_Value, _Hash, _Pred, _Alloc>& __x, // :3
	       const unordered_set<_Value, _Hash, _Pred, _Alloc>& __y) // :3
    { return !(__x == __y); } // :3

  template<class _Value, class _Hash, class _Pred, class _Alloc> // :3
    inline bool // :3
    operator==(const unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __x, // :3
	       const unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __y) // :3
    { return __x._M_h._M_equal(__y._M_h); } // :3

  template<class _Value, class _Hash, class _Pred, class _Alloc> // :3
    inline bool // :3
    operator!=(const unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __x, // :3
	       const unordered_multiset<_Value, _Hash, _Pred, _Alloc>& __y) // :3
    { return !(__x == __y); } // :3

_GLIBCXX_END_NAMESPACE_CONTAINER // :3
} // namespace std :3
