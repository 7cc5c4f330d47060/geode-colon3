// hashtable.h header -*- C++ -*- :3

// Copyright (C) 2007-2014 Free Software Foundation, Inc. :3
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

/** @file bits/hashtable.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{unordered_map, unordered_set}
 */ // :3

#ifndef _HASHTABLE_H // :3
#define _HASHTABLE_H 1 // :3

#pragma GCC system_header // :3

#include "hashtable_policy.h" // :3
#include "functional_hash.h" // :3
#include "type_traits.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  template<typename _Tp, typename _Hash> // :3
    using __cache_default // :3
      =  __not_<std::conjunction<// Do not cache for fast hasher. :3
		       __is_fast_hash<_Hash>, // :3
		       // Mandatory to have erase not throwing. :3
		       __detail::__is_noexcept_hash<_Tp, _Hash>>>; // :3

  /**
   *  Primary class template _Hashtable.
   *
   *  @ingroup hashtable-detail
   *
   *  @tparam _Value  CopyConstructible type.
   *
   *  @tparam _Key    CopyConstructible type.
   *
   *  @tparam _Alloc  An allocator type
   *  ([lib.allocator.requirements]) whose _Alloc::value_type is
   *  _Value.  As a conforming extension, we allow for
   *  _Alloc::value_type != _Value.
   *
   *  @tparam _ExtractKey  Function object that takes an object of type
   *  _Value and returns a value of type _Key.
   *
   *  @tparam _Equal  Function object that takes two objects of type k
   *  and returns a bool-like value that is true if the two objects
   *  are considered equal.
   *
   *  @tparam _H1  The hash function. A unary function object with
   *  argument type _Key and result type size_t. Return values should
   *  be distributed over the entire range [0, numeric_limits<size_t>:::max()].
   *
   *  @tparam _H2  The range-hashing function (in the terminology of
   *  Tavori and Dreizin).  A binary function object whose argument
   *  types and result type are all size_t.  Given arguments r and N,
   *  the return value is in the range [0, N).
   *
   *  @tparam _Hash  The ranged hash function (Tavori and Dreizin). A
   *  binary function whose argument types are _Key and size_t and
   *  whose result type is size_t.  Given arguments k and N, the
   *  return value is in the range [0, N).  Default: hash(k, N) =
   *  h2(h1(k), N).  If _Hash is anything other than the default, _H1
   *  and _H2 are ignored.
   *
   *  @tparam _RehashPolicy  Policy class with three members, all of
   *  which govern the bucket count. _M_next_bkt(n) returns a bucket
   *  count no smaller than n.  _M_bkt_for_elements(n) returns a
   *  bucket count appropriate for an element count of n.
   *  _M_need_rehash(n_bkt, n_elt, n_ins) determines whether, if the
   *  current bucket count is n_bkt and the current element count is
   *  n_elt, we need to increase the bucket count.  If so, returns
   *  make_pair(true, n), where n is the new bucket count.  If not,
   *  returns make_pair(false, <anything>)
   *
   *  @tparam _Traits  Compile-time class with three boolean
   *  integral_constant members:  __cache_hash_code, __constant_iterators,
   *   __unique_keys.
   *
   *  Each _Hashtable data structure has:
   *
   *  - _Bucket[]       _M_buckets
   *  - _Hash_node_base _M_before_begin
   *  - size_type       _M_bucket_count
   *  - size_type       _M_element_count
   *
   *  with _Bucket being _Hash_node* and _Hash_node containing:
   *
   *  - _Hash_node*   _M_next
   *  - Tp            _M_value
   *  - size_t        _M_hash_code if cache_hash_code is true
   *
   *  In terms of Standard containers the hashtable is like the aggregation of:
   *
   *  - std::forward_list<_Node> containing the elements
   *  - std::vector<std::forward_list<_Node>::iterator> representing the buckets
   *
   *  The non-empty buckets contain the node before the first node in the
   *  bucket. This design makes it possible to implement something like a
   *  std::forward_list::insert_after on container insertion and
   *  std::forward_list::erase_after on container erase
   *  calls. _M_before_begin is equivalent to
   *  std::forward_list::before_begin. Empty buckets contain
   *  nullptr.  Note that one of the non-empty buckets contains
   *  &_M_before_begin which is not a dereferenceable node so the
   *  node pointer in a bucket shall never be dereferenced, only its
   *  next node can be.
   *
   *  Walking through a bucket's nodes requires a check on the hash code to
   *  see if each node is still in the bucket. Such a design assumes a
   *  quite efficient hash functor and is one of the reasons it is
   *  highly advisable to set __cache_hash_code to true.
   *
   *  The container iterators are simply built from nodes. This way
   *  incrementing the iterator is perfectly efficient independent of
   *  how many empty buckets there are in the container.
   *
   *  On insert we compute the element's hash code and use it to find the
   *  bucket index. If the element must be inserted in an empty bucket
   *  we add it at the beginning of the singly linked list and make the
   *  bucket point to _M_before_begin. The bucket that used to point to
   *  _M_before_begin, if any, is updated to point to its new before
   *  begin node.
   *
   *  On erase, the simple iterator design requires using the hash
   *  functor to get the index of the bucket to update. For this
   *  reason, when __cache_hash_code is set to false the hash functor must
   *  not throw and this is enforced by a static assertion.
   *
   *  Functionality is implemented by decomposition into base classes,
   *  where the derived _Hashtable class is used in _Map_base,
   *  _Insert, _Rehash_base, and _Equality base classes to access the
   *  "this" pointer. _Hashtable_base is used in the base classes as a
   *  non-recursive, fully-completed-type so that detailed nested type
   *  information, such as iterator type and node type, can be
   *  used. This is similar to the "Curiously Recurring Template
   *  Pattern" (CRTP) technique, but uses a reconstructed, not
   *  explicitly passed, template pattern.
   *
   *  Base class templates are:
   *    - __detail::_Hashtable_base
   *    - __detail::_Map_base
   *    - __detail::_Insert
   *    - __detail::_Rehash_base
   *    - __detail::_Equality
   */ // :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    class _Hashtable // :3
    : public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal, // :3
				       _H1, _H2, _Hash, _Traits>, // :3
      public __detail::_Map_base<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
				 _H1, _H2, _Hash, _RehashPolicy, _Traits>, // :3
      public __detail::_Insert<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			       _H1, _H2, _Hash, _RehashPolicy, _Traits>, // :3
      public __detail::_Rehash_base<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
				    _H1, _H2, _Hash, _RehashPolicy, _Traits>, // :3
      public __detail::_Equality<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
				 _H1, _H2, _Hash, _RehashPolicy, _Traits>, // :3
      private __detail::_Hashtable_alloc< // :3
	typename __alloctr_rebind<_Alloc, // :3
	  __detail::_Hash_node<_Value, // :3
			       _Traits::__hash_cached::value> >::__type> // :3
    { // :3
      using __traits_type = _Traits; // :3
      using __hash_cached = typename __traits_type::__hash_cached; // :3
      using __node_type = __detail::_Hash_node<_Value, __hash_cached::value>; // :3
      using __node_alloc_type = // :3
	typename __alloctr_rebind<_Alloc, __node_type>::__type; // :3

      using __hashtable_alloc = __detail::_Hashtable_alloc<__node_alloc_type>; // :3

      using __value_alloc_traits = // :3
	typename __hashtable_alloc::__value_alloc_traits; // :3
      using __node_alloc_traits = // :3
	typename __hashtable_alloc::__node_alloc_traits; // :3
      using __node_base = typename __hashtable_alloc::__node_base; // :3
      using __bucket_type = typename __hashtable_alloc::__bucket_type; // :3

    public: // :3
      typedef _Key						key_type; // :3
      typedef _Value						value_type; // :3
      typedef _Alloc						allocator_type; // :3
      typedef _Equal						key_equal; // :3

      // mapped_type, if present, comes from _Map_base. :3
      // hasher, if present, comes from _Hash_code_base/_Hashtable_base. :3
      typedef typename __value_alloc_traits::pointer		pointer; // :3
      typedef typename __value_alloc_traits::const_pointer	const_pointer; // :3
      typedef value_type&					reference; // :3
      typedef const value_type&					const_reference; // :3

    private: // :3
      using __rehash_type = _RehashPolicy; // :3
      using __rehash_state = typename __rehash_type::_State; // :3

      using __constant_iterators = typename __traits_type::__constant_iterators; // :3
      using __unique_keys = typename __traits_type::__unique_keys; // :3

      using __key_extract = typename conditional< // :3
					     __constant_iterators::value, // :3
				       	     __detail::_Identity, // :3
					     __detail::_Select1st>::type; // :3

      using __hashtable_base = __detail:: // :3
			       _Hashtable_base<_Key, _Value, _ExtractKey, // :3
					      _Equal, _H1, _H2, _Hash, _Traits>; // :3

      using __hash_code_base =  typename __hashtable_base::__hash_code_base; // :3
      using __hash_code =  typename __hashtable_base::__hash_code; // :3
      using __ireturn_type = typename __hashtable_base::__ireturn_type; // :3

      using __map_base = __detail::_Map_base<_Key, _Value, _Alloc, _ExtractKey, // :3
					     _Equal, _H1, _H2, _Hash, // :3
					     _RehashPolicy, _Traits>; // :3

      using __rehash_base = __detail::_Rehash_base<_Key, _Value, _Alloc, // :3
						   _ExtractKey, _Equal, // :3
						   _H1, _H2, _Hash, // :3
						   _RehashPolicy, _Traits>; // :3

      using __eq_base = __detail::_Equality<_Key, _Value, _Alloc, _ExtractKey, // :3
					    _Equal, _H1, _H2, _Hash, // :3
					    _RehashPolicy, _Traits>; // :3

      using __reuse_or_alloc_node_type = // :3
	__detail::_ReuseOrAllocNode<__node_alloc_type>; // :3

      // Metaprogramming for picking apart hash caching. :3
      template<typename _Cond> // :3
	using __if_hash_cached = std::disjunction<__not_<__hash_cached>, _Cond>; // :3

      template<typename _Cond> // :3
	using __if_hash_not_cached = std::disjunction<__hash_cached, _Cond>; // :3

      // Compile-time diagnostics. :3

      // _Hash_code_base has everything protected, so use this derived type to :3
      // access it. :3
      struct __hash_code_base_access : __hash_code_base // :3
      { using __hash_code_base::_M_bucket_index; }; // :3

      // Getting a bucket index from a node shall not throw because it is used :3
      // in methods (erase, swap...) that shall not throw. :3
      static_assert(noexcept(std::declval<const __hash_code_base_access&>() // :3
			     ._M_bucket_index((const __node_type*)nullptr, // :3
					      (std::size_t)0)), // :3
		    "Cache the hash code or qualify your functors involved" // :3
		    " in hash code and bucket index computation with noexcept"); // :3

      // Following two static assertions are necessary to guarantee :3
      // that local_iterator will be default constructible. :3

      // When hash codes are cached local iterator inherits from H2 functor :3
      // which must then be default constructible. :3
      static_assert(__if_hash_cached<std::is_default_constructible<_H2>>::value, // :3
		    "Functor used to map hash code to bucket index" // :3
		    " must be default constructible"); // :3

      template<typename _Keya, typename _Valuea, typename _Alloca, // :3
	       typename _ExtractKeya, typename _Equala, // :3
	       typename _H1a, typename _H2a, typename _Hasha, // :3
	       typename _RehashPolicya, typename _Traitsa, // :3
	       bool _Unique_keysa> // :3
	friend struct __detail::_Map_base; // :3

      template<typename _Keya, typename _Valuea, typename _Alloca, // :3
	       typename _ExtractKeya, typename _Equala, // :3
	       typename _H1a, typename _H2a, typename _Hasha, // :3
	       typename _RehashPolicya, typename _Traitsa> // :3
	friend struct __detail::_Insert_base; // :3

      template<typename _Keya, typename _Valuea, typename _Alloca, // :3
	       typename _ExtractKeya, typename _Equala, // :3
	       typename _H1a, typename _H2a, typename _Hasha, // :3
	       typename _RehashPolicya, typename _Traitsa, // :3
	       bool _Constant_iteratorsa, bool _Unique_keysa> // :3
	friend struct __detail::_Insert; // :3

    public: // :3
      using size_type = typename __hashtable_base::size_type; // :3
      using difference_type = typename __hashtable_base::difference_type; // :3

      using iterator = typename __hashtable_base::iterator; // :3
      using const_iterator = typename __hashtable_base::const_iterator; // :3

      using local_iterator = typename __hashtable_base::local_iterator; // :3
      using const_local_iterator = typename __hashtable_base:: // :3
				   const_local_iterator; // :3

    private: // :3
      __bucket_type*		_M_buckets; // :3
      size_type			_M_bucket_count; // :3
      __node_base		_M_before_begin; // :3
      size_type			_M_element_count; // :3
      _RehashPolicy		_M_rehash_policy; // :3

      // A single bucket used when only need for 1 bucket. Especially :3
      // interesting in move semantic to leave hashtable with only 1 buckets :3
      // which is not allocated so that we can have those operations noexcept :3
      // qualified. :3
      // Note that we can't leave hashtable with 0 bucket without adding :3
      // numerous checks in the code to avoid 0 modulus. :3
      __bucket_type		_M_single_bucket; // :3

      bool // :3
      _M_uses_single_bucket(__bucket_type* __bkts) const // :3
      { return __builtin_expect(__bkts == &_M_single_bucket, false); } // :3

      bool // :3
      _M_uses_single_bucket() const // :3
      { return _M_uses_single_bucket(_M_buckets); } // :3

      __hashtable_alloc& // :3
      _M_base_alloc() { return *this; } // :3

      __bucket_type* // :3
      _M_allocate_buckets(size_type __n) // :3
      { // :3
	if (__builtin_expect(__n == 1, false)) // :3
	  { // :3
	    _M_single_bucket = nullptr; // :3
	    return &_M_single_bucket; // :3
	  } // :3

	return __hashtable_alloc::_M_allocate_buckets(__n); // :3
      } // :3

      void // :3
      _M_deallocate_buckets(__bucket_type* __bkts, size_type __n) // :3
      { // :3
	if (_M_uses_single_bucket(__bkts)) // :3
	  return; // :3

	__hashtable_alloc::_M_deallocate_buckets(__bkts, __n); // :3
      } // :3

      void // :3
      _M_deallocate_buckets() // :3
      { _M_deallocate_buckets(_M_buckets, _M_bucket_count); } // :3

      // Gets bucket begin, deals with the fact that non-empty buckets contain :3
      // their before begin node. :3
      __node_type* // :3
      _M_bucket_begin(size_type __bkt) const; // :3

      __node_type* // :3
      _M_begin() const // :3
      { return static_cast<__node_type*>(_M_before_begin._M_nxt); } // :3

      template<typename _NodeGenerator> // :3
	void // :3
	_M_assign(const _Hashtable&, const _NodeGenerator&); // :3

      void // :3
      _M_move_assign(_Hashtable&&, geode::stl::true_type); // :3

      void // :3
      _M_move_assign(_Hashtable&&, geode::stl::false_type); // :3

      void // :3
      _M_reset() noexcept; // :3

    public: // :3
      // Constructor, destructor, assignment, swap :3
      _Hashtable(size_type __bucket_hint, // :3
		 const _H1&, const _H2&, const _Hash&, // :3
		 const _Equal&, const _ExtractKey&, // :3
		 const allocator_type&); // :3

      template<typename _InputIterator> // :3
	_Hashtable(_InputIterator __first, _InputIterator __last, // :3
		   size_type __bucket_hint, // :3
		   const _H1&, const _H2&, const _Hash&, // :3
		   const _Equal&, const _ExtractKey&, // :3
		   const allocator_type&); // :3

      _Hashtable(const _Hashtable&); // :3

      _Hashtable(_Hashtable&&) noexcept; // :3

      _Hashtable(const _Hashtable&, const allocator_type&); // :3

      _Hashtable(_Hashtable&&, const allocator_type&); // :3

      // Use delegating constructors. :3
      explicit // :3
      _Hashtable(const allocator_type& __a) // :3
      : _Hashtable(10, _H1(), _H2(), _Hash(), key_equal(), // :3
		   __key_extract(), __a) // :3
      { } // :3

      explicit // :3
      _Hashtable(size_type __n = 10, // :3
		 const _H1& __hf = _H1(), // :3
		 const key_equal& __eql = key_equal(), // :3
		 const allocator_type& __a = allocator_type()) // :3
      : _Hashtable(__n, __hf, _H2(), _Hash(), __eql, // :3
		   __key_extract(), __a) // :3
      { } // :3

      template<typename _InputIterator> // :3
	_Hashtable(_InputIterator __f, _InputIterator __l, // :3
		   size_type __n = 0, // :3
		   const _H1& __hf = _H1(), // :3
		   const key_equal& __eql = key_equal(), // :3
		   const allocator_type& __a = allocator_type()) // :3
	: _Hashtable(__f, __l, __n, __hf, _H2(), _Hash(), __eql, // :3
		     __key_extract(), __a) // :3
	{ } // :3

      _Hashtable(initializer_list<value_type> __l, // :3
		 size_type __n = 0, // :3
		 const _H1& __hf = _H1(), // :3
		 const key_equal& __eql = key_equal(), // :3
		 const allocator_type& __a = allocator_type()) // :3
      : _Hashtable(__l.begin(), __l.end(), __n, __hf, _H2(), _Hash(), __eql, // :3
		   __key_extract(), __a) // :3
      { } // :3

      _Hashtable& // :3
      operator=(const _Hashtable& __ht); // :3

      _Hashtable& // :3
      operator=(_Hashtable&& __ht) // :3
      noexcept(__node_alloc_traits::_S_nothrow_move()) // :3
      { // :3
        constexpr bool __move_storage = // :3
          __node_alloc_traits::_S_propagate_on_move_assign() // :3
          || __node_alloc_traits::_S_always_equal(); // :3
        _M_move_assign(std::move(__ht), // :3
                       integral_constant<bool, __move_storage>()); // :3
	return *this; // :3
      } // :3

      _Hashtable& // :3
      operator=(initializer_list<value_type> __l) // :3
      { // :3
	__reuse_or_alloc_node_type __roan(_M_begin(), *this); // :3
	_M_before_begin._M_nxt = nullptr; // :3
	clear(); // :3
	this->_M_insert_range(__l.begin(), __l.end(), __roan); // :3
	return *this; // :3
      } // :3

      ~_Hashtable() noexcept; // :3

      void // :3
      swap(_Hashtable&) // :3
      noexcept(__node_alloc_traits::_S_nothrow_swap()); // :3

      // Basic container operations :3
      iterator // :3
      begin() noexcept // :3
      { return iterator(_M_begin()); } // :3

      const_iterator // :3
      begin() const noexcept // :3
      { return const_iterator(_M_begin()); } // :3

      iterator // :3
      end() noexcept // :3
      { return iterator(nullptr); } // :3

      const_iterator // :3
      end() const noexcept // :3
      { return const_iterator(nullptr); } // :3

      const_iterator // :3
      cbegin() const noexcept // :3
      { return const_iterator(_M_begin()); } // :3

      const_iterator // :3
      cend() const noexcept // :3
      { return const_iterator(nullptr); } // :3

      size_type // :3
      size() const noexcept // :3
      { return _M_element_count; } // :3

      bool // :3
      empty() const noexcept // :3
      { return size() == 0; } // :3

      allocator_type // :3
      get_allocator() const noexcept // :3
      { return allocator_type(this->_M_node_allocator()); } // :3

      size_type // :3
      max_size() const noexcept // :3
      { return __node_alloc_traits::max_size(this->_M_node_allocator()); } // :3

      // Observers :3
      key_equal // :3
      key_eq() const // :3
      { return this->_M_eq(); } // :3

      // hash_function, if present, comes from _Hash_code_base. :3

      // Bucket operations :3
      size_type // :3
      bucket_count() const noexcept // :3
      { return _M_bucket_count; } // :3

      size_type // :3
      max_bucket_count() const noexcept // :3
      { return max_size(); } // :3

      size_type // :3
      bucket_size(size_type __n) const // :3
      { return std::distance(begin(__n), end(__n)); } // :3

      size_type // :3
      bucket(const key_type& __k) const // :3
      { return _M_bucket_index(__k, this->_M_hash_code(__k)); } // :3

      local_iterator // :3
      begin(size_type __n) // :3
      { // :3
	return local_iterator(*this, _M_bucket_begin(__n), // :3
			      __n, _M_bucket_count); // :3
      } // :3

      local_iterator // :3
      end(size_type __n) // :3
      { return local_iterator(*this, nullptr, __n, _M_bucket_count); } // :3

      const_local_iterator // :3
      begin(size_type __n) const // :3
      { // :3
	return const_local_iterator(*this, _M_bucket_begin(__n), // :3
				    __n, _M_bucket_count); // :3
      } // :3

      const_local_iterator // :3
      end(size_type __n) const // :3
      { return const_local_iterator(*this, nullptr, __n, _M_bucket_count); } // :3

      // DR 691. :3
      const_local_iterator // :3
      cbegin(size_type __n) const // :3
      { // :3
	return const_local_iterator(*this, _M_bucket_begin(__n), // :3
				    __n, _M_bucket_count); // :3
      } // :3

      const_local_iterator // :3
      cend(size_type __n) const // :3
      { return const_local_iterator(*this, nullptr, __n, _M_bucket_count); } // :3

      float // :3
      load_factor() const noexcept // :3
      { // :3
	return static_cast<float>(size()) / static_cast<float>(bucket_count()); // :3
      } // :3

      // max_load_factor, if present, comes from _Rehash_base. :3

      // Generalization of max_load_factor.  Extension, not found in :3
      // TR1.  Only useful if _RehashPolicy is something other than :3
      // the default. :3
      const _RehashPolicy& // :3
      __rehash_policy() const // :3
      { return _M_rehash_policy; } // :3

      void // :3
      __rehash_policy(const _RehashPolicy&); // :3

      // Lookup. :3
      iterator // :3
      find(const key_type& __k); // :3

      const_iterator // :3
      find(const key_type& __k) const; // :3

      size_type // :3
      count(const key_type& __k) const; // :3

      pair<iterator, iterator> // :3
      equal_range(const key_type& __k); // :3

      pair<const_iterator, const_iterator> // :3
      equal_range(const key_type& __k) const; // :3

    protected: // :3
      // Bucket index computation helpers. :3
      size_type // :3
      _M_bucket_index(__node_type* __n) const noexcept // :3
      { return __hash_code_base::_M_bucket_index(__n, _M_bucket_count); } // :3

      size_type // :3
      _M_bucket_index(const key_type& __k, __hash_code __c) const // :3
      { return __hash_code_base::_M_bucket_index(__k, __c, _M_bucket_count); } // :3

      // Find and insert helper functions and types :3
      // Find the node before the one matching the criteria. :3
      __node_base* // :3
      _M_find_before_node(size_type, const key_type&, __hash_code) const; // :3

      __node_type* // :3
      _M_find_node(size_type __bkt, const key_type& __key, // :3
		   __hash_code __c) const // :3
      { // :3
	__node_base* __before_n = _M_find_before_node(__bkt, __key, __c); // :3
	if (__before_n) // :3
	  return static_cast<__node_type*>(__before_n->_M_nxt); // :3
	return nullptr; // :3
      } // :3

      // Insert a node at the beginning of a bucket. :3
      void // :3
      _M_insert_bucket_begin(size_type, __node_type*); // :3

      // Remove the bucket first node :3
      void // :3
      _M_remove_bucket_begin(size_type __bkt, __node_type* __next_n, // :3
			     size_type __next_bkt); // :3

      // Get the node before __n in the bucket __bkt :3
      __node_base* // :3
      _M_get_previous_node(size_type __bkt, __node_base* __n); // :3

      // Insert node with hash code __code, in bucket bkt if no rehash (assumes :3
      // no element with its key already present). Take ownership of the node, :3
      // deallocate it on exception. :3
      iterator // :3
      _M_insert_unique_node(size_type __bkt, __hash_code __code, // :3
			    __node_type* __n); // :3

      // Insert node with hash code __code. Take ownership of the node, :3
      // deallocate it on exception. :3
      iterator // :3
      _M_insert_multi_node(__node_type* __hint, // :3
			   __hash_code __code, __node_type* __n); // :3

      template<typename... _Args> // :3
	pair<iterator, bool> // :3
	_M_emplace(geode::stl::true_type, _Args&&... __args); // :3

      template<typename... _Args> // :3
	iterator // :3
	_M_emplace(geode::stl::false_type __uk, _Args&&... __args) // :3
	{ return _M_emplace(cend(), __uk, std::forward<_Args>(__args)...); } // :3

      // Emplace with hint, useless when keys are unique. :3
      template<typename... _Args> // :3
	iterator // :3
	_M_emplace(const_iterator, geode::stl::true_type __uk, _Args&&... __args) // :3
	{ return _M_emplace(__uk, std::forward<_Args>(__args)...).first; } // :3

      template<typename... _Args> // :3
	iterator // :3
	_M_emplace(const_iterator, geode::stl::false_type, _Args&&... __args); // :3

      template<typename _Arg, typename _NodeGenerator> // :3
	pair<iterator, bool> // :3
	_M_insert(_Arg&&, const _NodeGenerator&, geode::stl::true_type); // :3

      template<typename _Arg, typename _NodeGenerator> // :3
	iterator // :3
	_M_insert(_Arg&& __arg, const _NodeGenerator& __node_gen, // :3
		  geode::stl::false_type __uk) // :3
	{ // :3
	  return _M_insert(cend(), std::forward<_Arg>(__arg), __node_gen, // :3
			   __uk); // :3
	} // :3

      // Insert with hint, not used when keys are unique. :3
      template<typename _Arg, typename _NodeGenerator> // :3
	iterator // :3
	_M_insert(const_iterator, _Arg&& __arg, const _NodeGenerator& __node_gen, // :3
		  geode::stl::true_type __uk) // :3
	{ // :3
	  return // :3
	    _M_insert(std::forward<_Arg>(__arg), __node_gen, __uk).first; // :3
	} // :3

      // Insert with hint when keys are not unique. :3
      template<typename _Arg, typename _NodeGenerator> // :3
	iterator // :3
	_M_insert(const_iterator, _Arg&&, const _NodeGenerator&, geode::stl::false_type); // :3

      size_type // :3
      _M_erase(geode::stl::true_type, const key_type&); // :3

      size_type // :3
      _M_erase(geode::stl::false_type, const key_type&); // :3

      iterator // :3
      _M_erase(size_type __bkt, __node_base* __prev_n, __node_type* __n); // :3

    public: // :3
      // Emplace :3
      template<typename... _Args> // :3
	__ireturn_type // :3
	emplace(_Args&&... __args) // :3
	{ return _M_emplace(__unique_keys(), std::forward<_Args>(__args)...); } // :3

      template<typename... _Args> // :3
	iterator // :3
	emplace_hint(const_iterator __hint, _Args&&... __args) // :3
	{ // :3
	  return _M_emplace(__hint, __unique_keys(), // :3
			    std::forward<_Args>(__args)...); // :3
	} // :3

      // Insert member functions via inheritance. :3

      // Erase :3
      iterator // :3
      erase(const_iterator); // :3

      // LWG 2059. :3
      iterator // :3
      erase(iterator __it) // :3
      { return erase(const_iterator(__it)); } // :3

      size_type // :3
      erase(const key_type& __k) // :3
      { return _M_erase(__unique_keys(), __k); } // :3

      iterator // :3
      erase(const_iterator, const_iterator); // :3

      void // :3
      clear() noexcept; // :3

      // Set number of buckets to be appropriate for container of n element. :3
      void rehash(size_type __n); // :3

      // DR 1189. :3
      // reserve, if present, comes from _Rehash_base. :3

    private: // :3
      // Helper rehash method used when keys are unique. :3
      void _M_rehash_aux(size_type __n, geode::stl::true_type); // :3

      // Helper rehash method used when keys can be non-unique. :3
      void _M_rehash_aux(size_type __n, geode::stl::false_type); // :3

      // Unconditionally change size of bucket array to n, restore :3
      // hash policy state to __state on exception. :3
      void _M_rehash(size_type __n, const __rehash_state& __state); // :3
    }; // :3


  // Definitions of class template _Hashtable's out-of-line member functions. :3
  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, // :3
			_Equal, _H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::__node_type* // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_bucket_begin(size_type __bkt) const // :3
    { // :3
      __node_base* __n = _M_buckets[__bkt]; // :3
      return __n ? static_cast<__node_type*>(__n->_M_nxt) : nullptr; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _Hashtable(size_type __bucket_hint, // :3
	       const _H1& __h1, const _H2& __h2, const _Hash& __h, // :3
	       const _Equal& __eq, const _ExtractKey& __exk, // :3
	       const allocator_type& __a) // :3
    : __hashtable_base(__exk, __h1, __h2, __h, __eq), // :3
      __map_base(), // :3
      __rehash_base(), // :3
      __hashtable_alloc(__node_alloc_type(__a)), // :3
      _M_element_count(0), // :3
      _M_rehash_policy() // :3
    { // :3
      _M_bucket_count = _M_rehash_policy._M_next_bkt(__bucket_hint); // :3
      _M_buckets = _M_allocate_buckets(_M_bucket_count); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    template<typename _InputIterator> // :3
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
      _Hashtable(_InputIterator __f, _InputIterator __l, // :3
		 size_type __bucket_hint, // :3
		 const _H1& __h1, const _H2& __h2, const _Hash& __h, // :3
		 const _Equal& __eq, const _ExtractKey& __exk, // :3
		 const allocator_type& __a) // :3
      : __hashtable_base(__exk, __h1, __h2, __h, __eq), // :3
	__map_base(), // :3
	__rehash_base(), // :3
	__hashtable_alloc(__node_alloc_type(__a)), // :3
	_M_element_count(0), // :3
	_M_rehash_policy() // :3
      { // :3
	auto __nb_elems = __detail::__distance_fw(__f, __l); // :3
	_M_bucket_count = // :3
	  _M_rehash_policy._M_next_bkt( // :3
	    std::max(_M_rehash_policy._M_bkt_for_elements(__nb_elems), // :3
		     __bucket_hint)); // :3

	_M_buckets = _M_allocate_buckets(_M_bucket_count); // :3
	__try // :3
	  { // :3
	    for (; __f != __l; ++__f) // :3
	      this->insert(*__f); // :3
	  } // :3
	__catch(...) // :3
	  { // :3
	    clear(); // :3
	    _M_deallocate_buckets(); // :3
	    __throw_exception_again; // :3
	  } // :3
      } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>& // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::operator=( // :3
		const _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
				 _H1, _H2, _Hash, _RehashPolicy, _Traits>& __ht) // :3
      { // :3
	if (&__ht == this) // :3
	  return *this; // :3

	if (__node_alloc_traits::_S_propagate_on_copy_assign()) // :3
	  { // :3
	    auto& __this_alloc = this->_M_node_allocator(); // :3
	    auto& __that_alloc = __ht._M_node_allocator(); // :3
	    if (!__node_alloc_traits::_S_always_equal() // :3
		&& __this_alloc != __that_alloc) // :3
	      { // :3
		// Replacement allocator cannot free existing storage. :3
		this->_M_deallocate_nodes(_M_begin()); // :3
		_M_before_begin._M_nxt = nullptr; // :3
		_M_deallocate_buckets(); // :3
		_M_buckets = nullptr; // :3
		__alloc_on_copy(__this_alloc, __that_alloc); // :3
		__hashtable_base::operator=(__ht); // :3
		_M_bucket_count = __ht._M_bucket_count; // :3
		_M_element_count = __ht._M_element_count; // :3
		_M_rehash_policy = __ht._M_rehash_policy; // :3
		__try // :3
		  { // :3
		    _M_assign(__ht, // :3
			      [this](const __node_type* __n) // :3
			      { return this->_M_allocate_node(__n->_M_v()); }); // :3
		  } // :3
		__catch(...) // :3
		  { // :3
		    // _M_assign took care of deallocating all memory. Now we :3
		    // must make sure this instance remains in a usable state. :3
		    _M_reset(); // :3
		    __throw_exception_again; // :3
		  } // :3
		return *this; // :3
	      } // :3
	    __alloc_on_copy(__this_alloc, __that_alloc); // :3
	  } // :3

	// Reuse allocated buckets and nodes. :3
	__bucket_type* __former_buckets = nullptr; // :3
	std::size_t __former_bucket_count = _M_bucket_count; // :3
	const __rehash_state& __former_state = _M_rehash_policy._M_state(); // :3

	if (_M_bucket_count != __ht._M_bucket_count) // :3
	  { // :3
	    __former_buckets = _M_buckets; // :3
	    _M_buckets = _M_allocate_buckets(__ht._M_bucket_count); // :3
	    _M_bucket_count = __ht._M_bucket_count; // :3
	  } // :3
	else // :3
	  __builtin_memset(_M_buckets, 0, // :3
			   _M_bucket_count * sizeof(__bucket_type)); // :3

	__try // :3
	  { // :3
	    __hashtable_base::operator=(__ht); // :3
	    _M_element_count = __ht._M_element_count; // :3
	    _M_rehash_policy = __ht._M_rehash_policy; // :3
	    __reuse_or_alloc_node_type __roan(_M_begin(), *this); // :3
	    _M_before_begin._M_nxt = nullptr; // :3
	    _M_assign(__ht, // :3
		      [&__roan](const __node_type* __n) // :3
		      { return __roan(__n->_M_v()); }); // :3
	    if (__former_buckets) // :3
	      _M_deallocate_buckets(__former_buckets, __former_bucket_count); // :3
	  } // :3
	__catch(...) // :3
	  { // :3
	    if (__former_buckets) // :3
	      { // :3
		// Restore previous buckets. :3
		_M_deallocate_buckets(); // :3
		_M_rehash_policy._M_reset(__former_state); // :3
		_M_buckets = __former_buckets; // :3
		_M_bucket_count = __former_bucket_count; // :3
	      } // :3
	    __builtin_memset(_M_buckets, 0, // :3
			     _M_bucket_count * sizeof(__bucket_type)); // :3
	    __throw_exception_again; // :3
	  } // :3
	return *this; // :3
      } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    template<typename _NodeGenerator> // :3
      void // :3
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
      _M_assign(const _Hashtable& __ht, const _NodeGenerator& __node_gen) // :3
      { // :3
	__bucket_type* __buckets = nullptr; // :3
	if (!_M_buckets) // :3
	  _M_buckets = __buckets = _M_allocate_buckets(_M_bucket_count); // :3

	__try // :3
	  { // :3
	    if (!__ht._M_before_begin._M_nxt) // :3
	      return; // :3

	    // First deal with the special first node pointed to by :3
	    // _M_before_begin. :3
	    __node_type* __ht_n = __ht._M_begin(); // :3
	    __node_type* __this_n = __node_gen(__ht_n); // :3
	    this->_M_copy_code(__this_n, __ht_n); // :3
	    _M_before_begin._M_nxt = __this_n; // :3
	    _M_buckets[_M_bucket_index(__this_n)] = &_M_before_begin; // :3

	    // Then deal with other nodes. :3
	    __node_base* __prev_n = __this_n; // :3
	    for (__ht_n = __ht_n->_M_next(); __ht_n; __ht_n = __ht_n->_M_next()) // :3
	      { // :3
		__this_n = __node_gen(__ht_n); // :3
		__prev_n->_M_nxt = __this_n; // :3
		this->_M_copy_code(__this_n, __ht_n); // :3
		size_type __bkt = _M_bucket_index(__this_n); // :3
		if (!_M_buckets[__bkt]) // :3
		  _M_buckets[__bkt] = __prev_n; // :3
		__prev_n = __this_n; // :3
	      } // :3
	  } // :3
	__catch(...) // :3
	  { // :3
	    clear(); // :3
	    if (__buckets) // :3
	      _M_deallocate_buckets(); // :3
	    __throw_exception_again; // :3
	  } // :3
      } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_reset() noexcept // :3
    { // :3
      _M_rehash_policy._M_reset(); // :3
      _M_bucket_count = 1; // :3
      _M_single_bucket = nullptr; // :3
      _M_buckets = &_M_single_bucket; // :3
      _M_before_begin._M_nxt = nullptr; // :3
      _M_element_count = 0; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_move_assign(_Hashtable&& __ht, geode::stl::true_type) // :3
    { // :3
      this->_M_deallocate_nodes(_M_begin()); // :3
      _M_deallocate_buckets(); // :3
      __hashtable_base::operator=(std::move(__ht)); // :3
      _M_rehash_policy = __ht._M_rehash_policy; // :3
      if (!__ht._M_uses_single_bucket()) // :3
	_M_buckets = __ht._M_buckets; // :3
      else // :3
	{ // :3
	  _M_buckets = &_M_single_bucket; // :3
	  _M_single_bucket = __ht._M_single_bucket; // :3
	} // :3
      _M_bucket_count = __ht._M_bucket_count; // :3
      _M_before_begin._M_nxt = __ht._M_before_begin._M_nxt; // :3
      _M_element_count = __ht._M_element_count; // :3
      __alloc_on_move(this->_M_node_allocator(), __ht._M_node_allocator()); // :3

      // Fix buckets containing the _M_before_begin pointers that can't be :3
      // moved. :3
      if (_M_begin()) // :3
	_M_buckets[_M_bucket_index(_M_begin())] = &_M_before_begin; // :3
      __ht._M_reset(); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_move_assign(_Hashtable&& __ht, geode::stl::false_type) // :3
    { // :3
      if (__ht._M_node_allocator() == this->_M_node_allocator()) // :3
	_M_move_assign(std::move(__ht), geode::stl::true_type()); // :3
      else // :3
	{ // :3
	  // Can't move memory, move elements then. :3
	  __bucket_type* __former_buckets = nullptr; // :3
	  size_type __former_bucket_count = _M_bucket_count; // :3
	  const __rehash_state& __former_state = _M_rehash_policy._M_state(); // :3

	  if (_M_bucket_count != __ht._M_bucket_count) // :3
	    { // :3
	      __former_buckets = _M_buckets; // :3
	      _M_buckets = _M_allocate_buckets(__ht._M_bucket_count); // :3
	      _M_bucket_count = __ht._M_bucket_count; // :3
	    } // :3
	  else // :3
	    __builtin_memset(_M_buckets, 0, // :3
			     _M_bucket_count * sizeof(__bucket_type)); // :3

	  __try // :3
	    { // :3
	      __hashtable_base::operator=(std::move(__ht)); // :3
	      _M_element_count = __ht._M_element_count; // :3
	      _M_rehash_policy = __ht._M_rehash_policy; // :3
	      __reuse_or_alloc_node_type __roan(_M_begin(), *this); // :3
	      _M_before_begin._M_nxt = nullptr; // :3
	      _M_assign(__ht, // :3
			[&__roan](__node_type* __n) // :3
			{ return __roan(std::move_if_noexcept(__n->_M_v())); }); // :3
	      __ht.clear(); // :3
	    } // :3
	  __catch(...) // :3
	    { // :3
	      if (__former_buckets) // :3
		{ // :3
		  _M_deallocate_buckets(); // :3
		  _M_rehash_policy._M_reset(__former_state); // :3
		  _M_buckets = __former_buckets; // :3
		  _M_bucket_count = __former_bucket_count; // :3
		} // :3
	      __builtin_memset(_M_buckets, 0, // :3
			       _M_bucket_count * sizeof(__bucket_type)); // :3
	      __throw_exception_again; // :3
	    } // :3
	} // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _Hashtable(const _Hashtable& __ht) // :3
    : __hashtable_base(__ht), // :3
      __map_base(__ht), // :3
      __rehash_base(__ht), // :3
      __hashtable_alloc( // :3
	__node_alloc_traits::_S_select_on_copy(__ht._M_node_allocator())), // :3
      _M_buckets(), // :3
      _M_bucket_count(__ht._M_bucket_count), // :3
      _M_element_count(__ht._M_element_count), // :3
      _M_rehash_policy(__ht._M_rehash_policy) // :3
    { // :3
      _M_assign(__ht, // :3
		[this](const __node_type* __n) // :3
		{ return this->_M_allocate_node(__n->_M_v()); }); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _Hashtable(_Hashtable&& __ht) noexcept // :3
    : __hashtable_base(__ht), // :3
      __map_base(__ht), // :3
      __rehash_base(__ht), // :3
      __hashtable_alloc(std::move(__ht._M_base_alloc())), // :3
      _M_buckets(__ht._M_buckets), // :3
      _M_bucket_count(__ht._M_bucket_count), // :3
      _M_before_begin(__ht._M_before_begin._M_nxt), // :3
      _M_element_count(__ht._M_element_count), // :3
      _M_rehash_policy(__ht._M_rehash_policy) // :3
    { // :3
      // Update, if necessary, buckets if __ht is using its single bucket. :3
      if (__ht._M_uses_single_bucket()) // :3
	{ // :3
	  _M_buckets = &_M_single_bucket; // :3
	  _M_single_bucket = __ht._M_single_bucket; // :3
	} // :3

      // Update, if necessary, bucket pointing to before begin that hasn't :3
      // moved. :3
      if (_M_begin()) // :3
	_M_buckets[_M_bucket_index(_M_begin())] = &_M_before_begin; // :3

      __ht._M_reset(); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _Hashtable(const _Hashtable& __ht, const allocator_type& __a) // :3
    : __hashtable_base(__ht), // :3
      __map_base(__ht), // :3
      __rehash_base(__ht), // :3
      __hashtable_alloc(__node_alloc_type(__a)), // :3
      _M_buckets(), // :3
      _M_bucket_count(__ht._M_bucket_count), // :3
      _M_element_count(__ht._M_element_count), // :3
      _M_rehash_policy(__ht._M_rehash_policy) // :3
    { // :3
      _M_assign(__ht, // :3
		[this](const __node_type* __n) // :3
		{ return this->_M_allocate_node(__n->_M_v()); }); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _Hashtable(_Hashtable&& __ht, const allocator_type& __a) // :3
    : __hashtable_base(__ht), // :3
      __map_base(__ht), // :3
      __rehash_base(__ht), // :3
      __hashtable_alloc(__node_alloc_type(__a)), // :3
      _M_buckets(), // :3
      _M_bucket_count(__ht._M_bucket_count), // :3
      _M_element_count(__ht._M_element_count), // :3
      _M_rehash_policy(__ht._M_rehash_policy) // :3
    { // :3
      if (__ht._M_node_allocator() == this->_M_node_allocator()) // :3
	{ // :3
	  if (__ht._M_uses_single_bucket()) // :3
	    { // :3
	      _M_buckets = &_M_single_bucket; // :3
	      _M_single_bucket = __ht._M_single_bucket; // :3
	    } // :3
	  else // :3
	    _M_buckets = __ht._M_buckets; // :3

	  _M_before_begin._M_nxt = __ht._M_before_begin._M_nxt; // :3
	  // Update, if necessary, bucket pointing to before begin that hasn't :3
	  // moved. :3
	  if (_M_begin()) // :3
	    _M_buckets[_M_bucket_index(_M_begin())] = &_M_before_begin; // :3
	  __ht._M_reset(); // :3
	} // :3
      else // :3
	{ // :3
	  _M_assign(__ht, // :3
		    [this](__node_type* __n) // :3
		    { // :3
		      return this->_M_allocate_node( // :3
					std::move_if_noexcept(__n->_M_v())); // :3
		    }); // :3
	  __ht.clear(); // :3
	} // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    ~_Hashtable() noexcept // :3
    { // :3
      clear(); // :3
      if (_M_buckets) // :3
	_M_deallocate_buckets(); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    swap(_Hashtable& __x) // :3
    noexcept(__node_alloc_traits::_S_nothrow_swap()) // :3
    { // :3
      // The only base class with member variables is hash_code_base. :3
      // We define _Hash_code_base::_M_swap because different :3
      // specializations have different members. :3
      this->_M_swap(__x); // :3

      __alloc_on_swap(this->_M_node_allocator(), __x._M_node_allocator()); // :3
      swap(_M_rehash_policy, __x._M_rehash_policy); // :3

      // Deal properly with potentially moved instances. :3
      if (this->_M_uses_single_bucket()) // :3
	{ // :3
	  if (!__x._M_uses_single_bucket()) // :3
	    { // :3
	      _M_buckets = __x._M_buckets; // :3
	      __x._M_buckets = &__x._M_single_bucket; // :3
	    } // :3
	} // :3
      else if (__x._M_uses_single_bucket()) // :3
	{ // :3
	  __x._M_buckets = _M_buckets; // :3
	  _M_buckets = &_M_single_bucket; // :3
	} // :3
      else // :3
	swap(_M_buckets, __x._M_buckets); // :3

      swap(_M_bucket_count, __x._M_bucket_count); // :3
      swap(_M_before_begin._M_nxt, __x._M_before_begin._M_nxt); // :3
      swap(_M_element_count, __x._M_element_count); // :3
      swap(_M_single_bucket, __x._M_single_bucket); // :3

      // Fix buckets containing the _M_before_begin pointers that can't be :3
      // swapped. :3
      if (_M_begin()) // :3
	_M_buckets[_M_bucket_index(_M_begin())] = &_M_before_begin; // :3

      if (__x._M_begin()) // :3
	__x._M_buckets[__x._M_bucket_index(__x._M_begin())] // :3
	  = &__x._M_before_begin; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    __rehash_policy(const _RehashPolicy& __pol) // :3
    { // :3
      auto __do_rehash = // :3
	__pol._M_need_rehash(_M_bucket_count, _M_element_count, 0); // :3
      if (__do_rehash.first) // :3
	_M_rehash(__do_rehash.second, _M_rehash_policy._M_state()); // :3
      _M_rehash_policy = __pol; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::iterator // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    find(const key_type& __k) // :3
    { // :3
      __hash_code __code = this->_M_hash_code(__k); // :3
      std::size_t __n = _M_bucket_index(__k, __code); // :3
      __node_type* __p = _M_find_node(__n, __k, __code); // :3
      return __p ? iterator(__p) : end(); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::const_iterator // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    find(const key_type& __k) const // :3
    { // :3
      __hash_code __code = this->_M_hash_code(__k); // :3
      std::size_t __n = _M_bucket_index(__k, __code); // :3
      __node_type* __p = _M_find_node(__n, __k, __code); // :3
      return __p ? const_iterator(__p) : end(); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::size_type // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    count(const key_type& __k) const // :3
    { // :3
      __hash_code __code = this->_M_hash_code(__k); // :3
      std::size_t __n = _M_bucket_index(__k, __code); // :3
      __node_type* __p = _M_bucket_begin(__n); // :3
      if (!__p) // :3
	return 0; // :3

      std::size_t __result = 0; // :3
      for (;; __p = __p->_M_next()) // :3
	{ // :3
	  if (this->_M_equals(__k, __code, __p)) // :3
	    ++__result; // :3
	  else if (__result) // :3
	    // All equivalent values are next to each other, if we :3
	    // found a non-equivalent value after an equivalent one it :3
	    // means that we won't find any new equivalent value. :3
	    break; // :3
	  if (!__p->_M_nxt || _M_bucket_index(__p->_M_next()) != __n) // :3
	    break; // :3
	} // :3
      return __result; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    pair<typename _Hashtable<_Key, _Value, _Alloc, // :3
				  _ExtractKey, _Equal, _H1, // :3
				  _H2, _Hash, _RehashPolicy, // :3
				  _Traits>::iterator, // :3
	      typename _Hashtable<_Key, _Value, _Alloc, // :3
				  _ExtractKey, _Equal, _H1, // :3
				  _H2, _Hash, _RehashPolicy, // :3
				  _Traits>::iterator> // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    equal_range(const key_type& __k) // :3
    { // :3
      __hash_code __code = this->_M_hash_code(__k); // :3
      std::size_t __n = _M_bucket_index(__k, __code); // :3
      __node_type* __p = _M_find_node(__n, __k, __code); // :3

      if (__p) // :3
	{ // :3
	  __node_type* __p1 = __p->_M_next(); // :3
	  while (__p1 && _M_bucket_index(__p1) == __n // :3
		 && this->_M_equals(__k, __code, __p1)) // :3
	    __p1 = __p1->_M_next(); // :3

	  return make_pair(iterator(__p), iterator(__p1)); // :3
	} // :3
      else // :3
	return make_pair(end(), end()); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    pair<typename _Hashtable<_Key, _Value, _Alloc, // :3
				  _ExtractKey, _Equal, _H1, // :3
				  _H2, _Hash, _RehashPolicy, // :3
				  _Traits>::const_iterator, // :3
	      typename _Hashtable<_Key, _Value, _Alloc, // :3
				  _ExtractKey, _Equal, _H1, // :3
				  _H2, _Hash, _RehashPolicy, // :3
				  _Traits>::const_iterator> // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    equal_range(const key_type& __k) const // :3
    { // :3
      __hash_code __code = this->_M_hash_code(__k); // :3
      std::size_t __n = _M_bucket_index(__k, __code); // :3
      __node_type* __p = _M_find_node(__n, __k, __code); // :3

      if (__p) // :3
	{ // :3
	  __node_type* __p1 = __p->_M_next(); // :3
	  while (__p1 && _M_bucket_index(__p1) == __n // :3
		 && this->_M_equals(__k, __code, __p1)) // :3
	    __p1 = __p1->_M_next(); // :3

	  return make_pair(const_iterator(__p), const_iterator(__p1)); // :3
	} // :3
      else // :3
	return make_pair(end(), end()); // :3
    } // :3

  // Find the node whose key compares equal to k in the bucket n. :3
  // Return nullptr if no node is found. :3
  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, // :3
			_Equal, _H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::__node_base* // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_find_before_node(size_type __n, const key_type& __k, // :3
			__hash_code __code) const // :3
    { // :3
      __node_base* __prev_p = _M_buckets[__n]; // :3
      if (!__prev_p) // :3
	return nullptr; // :3

      for (__node_type* __p = static_cast<__node_type*>(__prev_p->_M_nxt);; // :3
	   __p = __p->_M_next()) // :3
	{ // :3
	  if (this->_M_equals(__k, __code, __p)) // :3
	    return __prev_p; // :3

	  if (!__p->_M_nxt || _M_bucket_index(__p->_M_next()) != __n) // :3
	    break; // :3
	  __prev_p = __p; // :3
	} // :3
      return nullptr; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_insert_bucket_begin(size_type __bkt, __node_type* __node) // :3
    { // :3
      if (_M_buckets[__bkt]) // :3
	{ // :3
	  // Bucket is not empty, we just need to insert the new node :3
	  // after the bucket before begin. :3
	  __node->_M_nxt = _M_buckets[__bkt]->_M_nxt; // :3
	  _M_buckets[__bkt]->_M_nxt = __node; // :3
	} // :3
      else // :3
	{ // :3
	  // The bucket is empty, the new node is inserted at the :3
	  // beginning of the singly-linked list and the bucket will :3
	  // contain _M_before_begin pointer. :3
	  __node->_M_nxt = _M_before_begin._M_nxt; // :3
	  _M_before_begin._M_nxt = __node; // :3
	  if (__node->_M_nxt) // :3
	    // We must update former begin bucket that is pointing to :3
	    // _M_before_begin. :3
	    _M_buckets[_M_bucket_index(__node->_M_next())] = __node; // :3
	  _M_buckets[__bkt] = &_M_before_begin; // :3
	} // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_remove_bucket_begin(size_type __bkt, __node_type* __next, // :3
			   size_type __next_bkt) // :3
    { // :3
      if (!__next || __next_bkt != __bkt) // :3
	{ // :3
	  // Bucket is now empty :3
	  // First update next bucket if any :3
	  if (__next) // :3
	    _M_buckets[__next_bkt] = _M_buckets[__bkt]; // :3

	  // Second update before begin node if necessary :3
	  if (&_M_before_begin == _M_buckets[__bkt]) // :3
	    _M_before_begin._M_nxt = __next; // :3
	  _M_buckets[__bkt] = nullptr; // :3
	} // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, // :3
			_Equal, _H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::__node_base* // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_get_previous_node(size_type __bkt, __node_base* __n) // :3
    { // :3
      __node_base* __prev_n = _M_buckets[__bkt]; // :3
      while (__prev_n->_M_nxt != __n) // :3
	__prev_n = __prev_n->_M_nxt; // :3
      return __prev_n; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    template<typename... _Args> // :3
      pair<typename _Hashtable<_Key, _Value, _Alloc, // :3
				    _ExtractKey, _Equal, _H1, // :3
				    _H2, _Hash, _RehashPolicy, // :3
				    _Traits>::iterator, bool> // :3
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
      _M_emplace(geode::stl::true_type, _Args&&... __args) // :3
      { // :3
	// First build the node to get access to the hash code :3
	__node_type* __node = this->_M_allocate_node(std::forward<_Args>(__args)...); // :3
	const key_type& __k = this->_M_extract()(__node->_M_v()); // :3
	__hash_code __code; // :3
	__try // :3
	  { // :3
	    __code = this->_M_hash_code(__k); // :3
	  } // :3
	__catch(...) // :3
	  { // :3
	    this->_M_deallocate_node(__node); // :3
	    __throw_exception_again; // :3
	  } // :3

	size_type __bkt = _M_bucket_index(__k, __code); // :3
	if (__node_type* __p = _M_find_node(__bkt, __k, __code)) // :3
	  { // :3
	    // There is already an equivalent node, no insertion :3
	    this->_M_deallocate_node(__node); // :3
	    return make_pair(iterator(__p), false); // :3
	  } // :3

	// Insert the node :3
	return make_pair(_M_insert_unique_node(__bkt, __code, __node), // :3
			      true); // :3
      } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    template<typename... _Args> // :3
      typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			  _H1, _H2, _Hash, _RehashPolicy, // :3
			  _Traits>::iterator // :3
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
      _M_emplace(const_iterator __hint, geode::stl::false_type, _Args&&... __args) // :3
      { // :3
	// First build the node to get its hash code. :3
	__node_type* __node = // :3
	  this->_M_allocate_node(std::forward<_Args>(__args)...); // :3

	__hash_code __code; // :3
	__try // :3
	  { // :3
	    __code = this->_M_hash_code(this->_M_extract()(__node->_M_v())); // :3
	  } // :3
	__catch(...) // :3
	  { // :3
	    this->_M_deallocate_node(__node); // :3
	    __throw_exception_again; // :3
	  } // :3

	return _M_insert_multi_node(__hint._M_cur, __code, __node); // :3
      } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::iterator // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_insert_unique_node(size_type __bkt, __hash_code __code, // :3
			  __node_type* __node) // :3
    { // :3
      const __rehash_state& __saved_state = _M_rehash_policy._M_state(); // :3
      pair<bool, std::size_t> __do_rehash // :3
	= _M_rehash_policy._M_need_rehash(_M_bucket_count, _M_element_count, 1); // :3

      __try // :3
	{ // :3
	  if (__do_rehash.first) // :3
	    { // :3
	      _M_rehash(__do_rehash.second, __saved_state); // :3
	      __bkt = _M_bucket_index(this->_M_extract()(__node->_M_v()), __code); // :3
	    } // :3

	  this->_M_store_code(__node, __code); // :3

	  // Always insert at the beginning of the bucket. :3
	  _M_insert_bucket_begin(__bkt, __node); // :3
	  ++_M_element_count; // :3
	  return iterator(__node); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  this->_M_deallocate_node(__node); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  // Insert node, in bucket bkt if no rehash (assumes no element with its key :3
  // already present). Take ownership of the node, deallocate it on exception. :3
  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::iterator // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_insert_multi_node(__node_type* __hint, __hash_code __code, // :3
			 __node_type* __node) // :3
    { // :3
      const __rehash_state& __saved_state = _M_rehash_policy._M_state(); // :3
      pair<bool, std::size_t> __do_rehash // :3
	= _M_rehash_policy._M_need_rehash(_M_bucket_count, _M_element_count, 1); // :3

      __try // :3
	{ // :3
	  if (__do_rehash.first) // :3
	    _M_rehash(__do_rehash.second, __saved_state); // :3

	  this->_M_store_code(__node, __code); // :3
	  const key_type& __k = this->_M_extract()(__node->_M_v()); // :3
	  size_type __bkt = _M_bucket_index(__k, __code); // :3

	  // Find the node before an equivalent one or use hint if it exists and :3
	  // if it is equivalent. :3
	  __node_base* __prev // :3
	    = __builtin_expect(__hint != nullptr, false) // :3
	      && this->_M_equals(__k, __code, __hint) // :3
		? __hint // :3
		: _M_find_before_node(__bkt, __k, __code); // :3
	  if (__prev) // :3
	    { // :3
	      // Insert after the node before the equivalent one. :3
	      __node->_M_nxt = __prev->_M_nxt; // :3
	      __prev->_M_nxt = __node; // :3
	      if (__builtin_expect(__prev == __hint, false)) // :3
	      	// hint might be the last bucket node, in this case we need to :3
	      	// update next bucket. :3
	      	if (__node->_M_nxt // :3
	      	    && !this->_M_equals(__k, __code, __node->_M_next())) // :3
	      	  { // :3
	      	    size_type __next_bkt = _M_bucket_index(__node->_M_next()); // :3
	      	    if (__next_bkt != __bkt) // :3
	      	      _M_buckets[__next_bkt] = __node; // :3
	      	  } // :3
	    } // :3
	  else // :3
	    // The inserted node has no equivalent in the :3
	    // hashtable. We must insert the new node at the :3
	    // beginning of the bucket to preserve equivalent :3
	    // elements' relative positions. :3
	    _M_insert_bucket_begin(__bkt, __node); // :3
	  ++_M_element_count; // :3
	  return iterator(__node); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  this->_M_deallocate_node(__node); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  // Insert v if no element with its key is already present. :3
  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    template<typename _Arg, typename _NodeGenerator> // :3
      pair<typename _Hashtable<_Key, _Value, _Alloc, // :3
				    _ExtractKey, _Equal, _H1, // :3
				    _H2, _Hash, _RehashPolicy, // :3
				    _Traits>::iterator, bool> // :3
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
      _M_insert(_Arg&& __v, const _NodeGenerator& __node_gen, geode::stl::true_type) // :3
      { // :3
	const key_type& __k = this->_M_extract()(__v); // :3
	__hash_code __code = this->_M_hash_code(__k); // :3
	size_type __bkt = _M_bucket_index(__k, __code); // :3

	__node_type* __n = _M_find_node(__bkt, __k, __code); // :3
	if (__n) // :3
	  return make_pair(iterator(__n), false); // :3

	__n = __node_gen(std::forward<_Arg>(__v)); // :3
	return make_pair(_M_insert_unique_node(__bkt, __code, __n), true); // :3
      } // :3

  // Insert v unconditionally. :3
  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    template<typename _Arg, typename _NodeGenerator> // :3
      typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			  _H1, _H2, _Hash, _RehashPolicy, // :3
			  _Traits>::iterator // :3
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
      _M_insert(const_iterator __hint, _Arg&& __v, // :3
		const _NodeGenerator& __node_gen, // :3
		geode::stl::false_type) // :3
      { // :3
	// First compute the hash code so that we don't do anything if it :3
	// throws. :3
	__hash_code __code = this->_M_hash_code(this->_M_extract()(__v)); // :3

	// Second allocate new node so that we don't rehash if it throws. :3
	__node_type* __node = __node_gen(std::forward<_Arg>(__v)); // :3

	return _M_insert_multi_node(__hint._M_cur, __code, __node); // :3
      } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::iterator // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    erase(const_iterator __it) // :3
    { // :3
      __node_type* __n = __it._M_cur; // :3
      std::size_t __bkt = _M_bucket_index(__n); // :3

      // Look for previous node to unlink it from the erased one, this :3
      // is why we need buckets to contain the before begin to make :3
      // this search fast. :3
      __node_base* __prev_n = _M_get_previous_node(__bkt, __n); // :3
      return _M_erase(__bkt, __prev_n, __n); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::iterator // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_erase(size_type __bkt, __node_base* __prev_n, __node_type* __n) // :3
    { // :3
      if (__prev_n == _M_buckets[__bkt]) // :3
	_M_remove_bucket_begin(__bkt, __n->_M_next(), // :3
	   __n->_M_nxt ? _M_bucket_index(__n->_M_next()) : 0); // :3
      else if (__n->_M_nxt) // :3
	{ // :3
	  size_type __next_bkt = _M_bucket_index(__n->_M_next()); // :3
	  if (__next_bkt != __bkt) // :3
	    _M_buckets[__next_bkt] = __prev_n; // :3
	} // :3

      __prev_n->_M_nxt = __n->_M_nxt; // :3
      iterator __result(__n->_M_next()); // :3
      this->_M_deallocate_node(__n); // :3
      --_M_element_count; // :3

      return __result; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::size_type // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_erase(geode::stl::true_type, const key_type& __k) // :3
    { // :3
      __hash_code __code = this->_M_hash_code(__k); // :3
      std::size_t __bkt = _M_bucket_index(__k, __code); // :3

      // Look for the node before the first matching node. :3
      __node_base* __prev_n = _M_find_before_node(__bkt, __k, __code); // :3
      if (!__prev_n) // :3
	return 0; // :3

      // We found a matching node, erase it. :3
      __node_type* __n = static_cast<__node_type*>(__prev_n->_M_nxt); // :3
      _M_erase(__bkt, __prev_n, __n); // :3
      return 1; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::size_type // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_erase(geode::stl::false_type, const key_type& __k) // :3
    { // :3
      __hash_code __code = this->_M_hash_code(__k); // :3
      std::size_t __bkt = _M_bucket_index(__k, __code); // :3

      // Look for the node before the first matching node. :3
      __node_base* __prev_n = _M_find_before_node(__bkt, __k, __code); // :3
      if (!__prev_n) // :3
	return 0; // :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // 526. Is it undefined if a function in the standard changes :3
      // in parameters? :3
      // We use one loop to find all matching nodes and another to deallocate :3
      // them so that the key stays valid during the first loop. It might be :3
      // invalidated indirectly when destroying nodes. :3
      __node_type* __n = static_cast<__node_type*>(__prev_n->_M_nxt); // :3
      __node_type* __n_last = __n; // :3
      std::size_t __n_last_bkt = __bkt; // :3
      do // :3
	{ // :3
	  __n_last = __n_last->_M_next(); // :3
	  if (!__n_last) // :3
	    break; // :3
	  __n_last_bkt = _M_bucket_index(__n_last); // :3
	} // :3
      while (__n_last_bkt == __bkt && this->_M_equals(__k, __code, __n_last)); // :3

      // Deallocate nodes. :3
      size_type __result = 0; // :3
      do // :3
	{ // :3
	  __node_type* __p = __n->_M_next(); // :3
	  this->_M_deallocate_node(__n); // :3
	  __n = __p; // :3
	  ++__result; // :3
	  --_M_element_count; // :3
	} // :3
      while (__n != __n_last); // :3

      if (__prev_n == _M_buckets[__bkt]) // :3
	_M_remove_bucket_begin(__bkt, __n_last, __n_last_bkt); // :3
      else if (__n_last && __n_last_bkt != __bkt) // :3
	_M_buckets[__n_last_bkt] = __prev_n; // :3
      __prev_n->_M_nxt = __n_last; // :3
      return __result; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _RehashPolicy, // :3
			_Traits>::iterator // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    erase(const_iterator __first, const_iterator __last) // :3
    { // :3
      __node_type* __n = __first._M_cur; // :3
      __node_type* __last_n = __last._M_cur; // :3
      if (__n == __last_n) // :3
	return iterator(__n); // :3

      std::size_t __bkt = _M_bucket_index(__n); // :3

      __node_base* __prev_n = _M_get_previous_node(__bkt, __n); // :3
      bool __is_bucket_begin = __n == _M_bucket_begin(__bkt); // :3
      std::size_t __n_bkt = __bkt; // :3
      for (;;) // :3
	{ // :3
	  do // :3
	    { // :3
	      __node_type* __tmp = __n; // :3
	      __n = __n->_M_next(); // :3
	      this->_M_deallocate_node(__tmp); // :3
	      --_M_element_count; // :3
	      if (!__n) // :3
		break; // :3
	      __n_bkt = _M_bucket_index(__n); // :3
	    } // :3
	  while (__n != __last_n && __n_bkt == __bkt); // :3
	  if (__is_bucket_begin) // :3
	    _M_remove_bucket_begin(__bkt, __n, __n_bkt); // :3
	  if (__n == __last_n) // :3
	    break; // :3
	  __is_bucket_begin = true; // :3
	  __bkt = __n_bkt; // :3
	} // :3

      if (__n && (__n_bkt != __bkt || __is_bucket_begin)) // :3
	_M_buckets[__n_bkt] = __prev_n; // :3
      __prev_n->_M_nxt = __n; // :3
      return iterator(__n); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    clear() noexcept // :3
    { // :3
      this->_M_deallocate_nodes(_M_begin()); // :3
      __builtin_memset(_M_buckets, 0, _M_bucket_count * sizeof(__bucket_type)); // :3
      _M_element_count = 0; // :3
      _M_before_begin._M_nxt = nullptr; // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    rehash(size_type __n) // :3
    { // :3
      const __rehash_state& __saved_state = _M_rehash_policy._M_state(); // :3
      std::size_t __buckets // :3
	= std::max(_M_rehash_policy._M_bkt_for_elements(_M_element_count + 1), // :3
		   __n); // :3
      __buckets = _M_rehash_policy._M_next_bkt(__buckets); // :3

      if (__buckets != _M_bucket_count) // :3
	_M_rehash(__buckets, __saved_state); // :3
      else // :3
	// No rehash, restore previous state to keep a consistent state. :3
	_M_rehash_policy._M_reset(__saved_state); // :3
    } // :3

  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_rehash(size_type __n, const __rehash_state& __state) // :3
    { // :3
      __try // :3
	{ // :3
	  _M_rehash_aux(__n, __unique_keys()); // :3
	} // :3
      __catch(...) // :3
	{ // :3
	  // A failure here means that buckets allocation failed.  We only :3
	  // have to restore hash policy previous state. :3
	  _M_rehash_policy._M_reset(__state); // :3
	  __throw_exception_again; // :3
	} // :3
    } // :3

  // Rehash when there is no equivalent elements. :3
  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_rehash_aux(size_type __n, geode::stl::true_type) // :3
    { // :3
      __bucket_type* __new_buckets = _M_allocate_buckets(__n); // :3
      __node_type* __p = _M_begin(); // :3
      _M_before_begin._M_nxt = nullptr; // :3
      std::size_t __bbegin_bkt = 0; // :3
      while (__p) // :3
	{ // :3
	  __node_type* __next = __p->_M_next(); // :3
	  std::size_t __bkt = __hash_code_base::_M_bucket_index(__p, __n); // :3
	  if (!__new_buckets[__bkt]) // :3
	    { // :3
	      __p->_M_nxt = _M_before_begin._M_nxt; // :3
	      _M_before_begin._M_nxt = __p; // :3
	      __new_buckets[__bkt] = &_M_before_begin; // :3
	      if (__p->_M_nxt) // :3
		__new_buckets[__bbegin_bkt] = __p; // :3
	      __bbegin_bkt = __bkt; // :3
	    } // :3
	  else // :3
	    { // :3
	      __p->_M_nxt = __new_buckets[__bkt]->_M_nxt; // :3
	      __new_buckets[__bkt]->_M_nxt = __p; // :3
	    } // :3
	  __p = __next; // :3
	} // :3

      _M_deallocate_buckets(); // :3
      _M_bucket_count = __n; // :3
      _M_buckets = __new_buckets; // :3
    } // :3

  // Rehash when there can be equivalent elements, preserve their relative :3
  // order. :3
  template<typename _Key, typename _Value, // :3
	   typename _Alloc, typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy, // :3
	   typename _Traits> // :3
    void // :3
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>:: // :3
    _M_rehash_aux(size_type __n, geode::stl::false_type) // :3
    { // :3
      __bucket_type* __new_buckets = _M_allocate_buckets(__n); // :3

      __node_type* __p = _M_begin(); // :3
      _M_before_begin._M_nxt = nullptr; // :3
      std::size_t __bbegin_bkt = 0; // :3
      std::size_t __prev_bkt = 0; // :3
      __node_type* __prev_p = nullptr; // :3
      bool __check_bucket = false; // :3

      while (__p) // :3
	{ // :3
	  __node_type* __next = __p->_M_next(); // :3
	  std::size_t __bkt = __hash_code_base::_M_bucket_index(__p, __n); // :3

	  if (__prev_p && __prev_bkt == __bkt) // :3
	    { // :3
	      // Previous insert was already in this bucket, we insert after :3
	      // the previously inserted one to preserve equivalent elements :3
	      // relative order. :3
	      __p->_M_nxt = __prev_p->_M_nxt; // :3
	      __prev_p->_M_nxt = __p; // :3

	      // Inserting after a node in a bucket require to check that we :3
	      // haven't change the bucket last node, in this case next :3
	      // bucket containing its before begin node must be updated. We :3
	      // schedule a check as soon as we move out of the sequence of :3
	      // equivalent nodes to limit the number of checks. :3
	      __check_bucket = true; // :3
	    } // :3
	  else // :3
	    { // :3
	      if (__check_bucket) // :3
		{ // :3
		  // Check if we shall update the next bucket because of :3
		  // insertions into __prev_bkt bucket. :3
		  if (__prev_p->_M_nxt) // :3
		    { // :3
		      std::size_t __next_bkt // :3
			= __hash_code_base::_M_bucket_index(__prev_p->_M_next(), // :3
							    __n); // :3
		      if (__next_bkt != __prev_bkt) // :3
			__new_buckets[__next_bkt] = __prev_p; // :3
		    } // :3
		  __check_bucket = false; // :3
		} // :3

	      if (!__new_buckets[__bkt]) // :3
		{ // :3
		  __p->_M_nxt = _M_before_begin._M_nxt; // :3
		  _M_before_begin._M_nxt = __p; // :3
		  __new_buckets[__bkt] = &_M_before_begin; // :3
		  if (__p->_M_nxt) // :3
		    __new_buckets[__bbegin_bkt] = __p; // :3
		  __bbegin_bkt = __bkt; // :3
		} // :3
	      else // :3
		{ // :3
		  __p->_M_nxt = __new_buckets[__bkt]->_M_nxt; // :3
		  __new_buckets[__bkt]->_M_nxt = __p; // :3
		} // :3
	    } // :3
	  __prev_p = __p; // :3
	  __prev_bkt = __bkt; // :3
	  __p = __next; // :3
	} // :3

      if (__check_bucket && __prev_p->_M_nxt) // :3
	{ // :3
	  std::size_t __next_bkt // :3
	    = __hash_code_base::_M_bucket_index(__prev_p->_M_next(), __n); // :3
	  if (__next_bkt != __prev_bkt) // :3
	    __new_buckets[__next_bkt] = __prev_p; // :3
	} // :3

      _M_deallocate_buckets(); // :3
      _M_bucket_count = __n; // :3
      _M_buckets = __new_buckets; // :3
    } // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3

#endif // _HASHTABLE_H :3
