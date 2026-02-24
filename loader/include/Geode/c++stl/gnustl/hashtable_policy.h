// Internal policy header for unordered_set and unordered_map -*- C++ -*- :3

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

/** @file bits/hashtable_policy.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly.
 *  @headername{unordered_map,unordered_set}
 */ // :3

#pragma once // :3

#include "c++config.h" // :3
#include "exception_defines.h" // :3
#include "ext/aligned_buffer.h" // :3
#include "stl_iterator_base_types.h" // :3
#include "stl_iterator_base_funcs.h" // :3
#include "initializer_list.h" // :3
#include "ext/alloc_traits.h" // :3
#include "type_traits.h" // :3
#include "tuple.h" // :3
#include "functexcept.h" // :3
#include "utility.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    class _Hashtable; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3

namespace __detail // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   *  @defgroup hashtable-detail Base and Implementation Classes
   *  @ingroup unordered_associative_containers
   *  @{
   */ // :3
  template<typename _Key, typename _Value, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _Traits> // :3
    struct _Hashtable_base; // :3

  // Helper function: return distance(first, last) for forward :3
  // iterators, or 0 for input iterators. :3
  template<class _Iterator> // :3
    inline typename iterator_traits<_Iterator>::difference_type // :3
    __distance_fw(_Iterator __first, _Iterator __last, // :3
		  input_iterator_tag) // :3
    { return 0; } // :3

  template<class _Iterator> // :3
    inline typename iterator_traits<_Iterator>::difference_type // :3
    __distance_fw(_Iterator __first, _Iterator __last, // :3
		  forward_iterator_tag) // :3
    { return distance(__first, __last); } // :3

  template<class _Iterator> // :3
    inline typename iterator_traits<_Iterator>::difference_type // :3
    __distance_fw(_Iterator __first, _Iterator __last) // :3
    { // :3
      typedef typename iterator_traits<_Iterator>::iterator_category _Tag; // :3
      return __distance_fw(__first, __last, _Tag()); // :3
    } // :3

  // Helper type used to detect whether the hash functor is noexcept. :3
  template <typename _Key, typename _Hash> // :3
    struct __is_noexcept_hash : integral_constant<bool, // :3
	noexcept(std::declval<const _Hash&>()(std::declval<const _Key&>()))> // :3
    { }; // :3

  struct _Identity // :3
  { // :3
    template<typename _Tp> // :3
      _Tp&& // :3
      operator()(_Tp&& __x) const // :3
      { return std::forward<_Tp>(__x); } // :3
  }; // :3

  struct _Select1st // :3
  { // :3
    template<typename _Tp> // :3
      auto // :3
      operator()(_Tp&& __x) const // :3
      -> decltype(get<0>(std::forward<_Tp>(__x))) // :3
      { return get<0>(std::forward<_Tp>(__x)); } // :3
  }; // :3

  template<typename _NodeAlloc> // :3
    struct _Hashtable_alloc; // :3

  // Functor recycling a pool of nodes and using allocation once the pool is :3
  // empty. :3
  template<typename _NodeAlloc> // :3
    struct _ReuseOrAllocNode // :3
    { // :3
    private: // :3
      using __node_alloc_type = _NodeAlloc; // :3
      using __hashtable_alloc = _Hashtable_alloc<__node_alloc_type>; // :3
      using __value_alloc_type = typename __hashtable_alloc::__value_alloc_type; // :3
      using __value_alloc_traits = // :3
	typename __hashtable_alloc::__value_alloc_traits; // :3
      using __node_alloc_traits = // :3
	typename __hashtable_alloc::__node_alloc_traits; // :3
      using __node_type = typename __hashtable_alloc::__node_type; // :3

    public: // :3
      _ReuseOrAllocNode(__node_type* __nodes, __hashtable_alloc& __h) // :3
	: _M_nodes(__nodes), _M_h(__h) { } // :3
      _ReuseOrAllocNode(const _ReuseOrAllocNode&) = delete; // :3

      ~_ReuseOrAllocNode() // :3
      { _M_h._M_deallocate_nodes(_M_nodes); } // :3

      template<typename _Arg> // :3
	__node_type* // :3
	operator()(_Arg&& __arg) const // :3
	{ // :3
	  if (_M_nodes) // :3
	    { // :3
	      __node_type* __node = _M_nodes; // :3
	      _M_nodes = _M_nodes->_M_next(); // :3
	      __node->_M_nxt = nullptr; // :3
	      __value_alloc_type __a(_M_h._M_node_allocator()); // :3
	      __value_alloc_traits::destroy(__a, __node->_M_valptr()); // :3
	      __try // :3
		{ // :3
		  __value_alloc_traits::construct(__a, __node->_M_valptr(), // :3
						  std::forward<_Arg>(__arg)); // :3
		} // :3
	      __catch(...) // :3
		{ // :3
		  __node->~__node_type(); // :3
		  __node_alloc_traits::deallocate(_M_h._M_node_allocator(), // :3
						  __node, 1); // :3
		  __throw_exception_again; // :3
		} // :3
	      return __node; // :3
	    } // :3
	  return _M_h._M_allocate_node(std::forward<_Arg>(__arg)); // :3
	} // :3

    private: // :3
      mutable __node_type* _M_nodes; // :3
      __hashtable_alloc& _M_h; // :3
    }; // :3

  // Functor similar to the previous one but without any pool of nodes to :3
  // recycle. :3
  template<typename _NodeAlloc> // :3
    struct _AllocNode // :3
    { // :3
    private: // :3
      using __hashtable_alloc = _Hashtable_alloc<_NodeAlloc>; // :3
      using __node_type = typename __hashtable_alloc::__node_type; // :3

    public: // :3
      _AllocNode(__hashtable_alloc& __h) // :3
	: _M_h(__h) { } // :3

      template<typename _Arg> // :3
	__node_type* // :3
	operator()(_Arg&& __arg) const // :3
	{ return _M_h._M_allocate_node(std::forward<_Arg>(__arg)); } // :3

    private: // :3
      __hashtable_alloc& _M_h; // :3
    }; // :3

  // Auxiliary types used for all instantiations of _Hashtable nodes :3
  // and iterators. :3

  /**
   *  struct _Hashtable_traits
   *
   *  Important traits for hash tables.
   *
   *  @tparam _Cache_hash_code  Boolean value. True if the value of
   *  the hash function is stored along with the value. This is a
   *  time-space tradeoff.  Storing it may improve lookup speed by
   *  reducing the number of times we need to call the _Equal
   *  function.
   *
   *  @tparam _Constant_iterators  Boolean value. True if iterator and
   *  const_iterator are both constant iterator types. This is true
   *  for unordered_set and unordered_multiset, false for
   *  unordered_map and unordered_multimap.
   *
   *  @tparam _Unique_keys  Boolean value. True if the return value
   *  of _Hashtable::count(k) is always at most one, false if it may
   *  be an arbitrary number. This is true for unordered_set and
   *  unordered_map, false for unordered_multiset and
   *  unordered_multimap.
   */ // :3
  template<bool _Cache_hash_code, bool _Constant_iterators, bool _Unique_keys> // :3
    struct _Hashtable_traits // :3
    { // :3
      template<bool _Cond> // :3
	using __bool_constant = integral_constant<bool, _Cond>; // :3

      using __hash_cached = __bool_constant<_Cache_hash_code>; // :3
      using __constant_iterators = __bool_constant<_Constant_iterators>; // :3
      using __unique_keys = __bool_constant<_Unique_keys>; // :3
    }; // :3

  /**
   *  struct _Hash_node_base
   *
   *  Nodes, used to wrap elements stored in the hash table.  A policy
   *  template parameter of class template _Hashtable controls whether
   *  nodes also store a hash code. In some cases (e.g. strings) this
   *  may be a performance win.
   */ // :3
  struct _Hash_node_base // :3
  { // :3
    _Hash_node_base* _M_nxt; // :3

    _Hash_node_base() noexcept : _M_nxt() { } // :3

    _Hash_node_base(_Hash_node_base* __next) noexcept : _M_nxt(__next) { } // :3
  }; // :3

  /**
   *  struct _Hash_node_value_base
   *
   *  Node type with the value to store.
   */ // :3
  template<typename _Value> // :3
    struct _Hash_node_value_base : _Hash_node_base // :3
    { // :3
      typedef _Value value_type; // :3

      __gnu_cxx::__aligned_buffer<_Value> _M_storage; // :3

      _Value* // :3
      _M_valptr() noexcept // :3
      { return _M_storage._M_ptr(); } // :3

      const _Value* // :3
      _M_valptr() const noexcept // :3
      { return _M_storage._M_ptr(); } // :3

      _Value& // :3
      _M_v() noexcept // :3
      { return *_M_valptr(); } // :3

      const _Value& // :3
      _M_v() const noexcept // :3
      { return *_M_valptr(); } // :3
    }; // :3

  /**
   *  Primary template struct _Hash_node.
   */ // :3
  template<typename _Value, bool _Cache_hash_code> // :3
    struct _Hash_node; // :3

  /**
   *  Specialization for nodes with caches, struct _Hash_node.
   *
   *  Base class is __detail::_Hash_node_value_base.
   */ // :3
  template<typename _Value> // :3
    struct _Hash_node<_Value, true> : _Hash_node_value_base<_Value> // :3
    { // :3
      std::size_t  _M_hash_code; // :3

      _Hash_node* // :3
      _M_next() const noexcept // :3
      { return static_cast<_Hash_node*>(this->_M_nxt); } // :3
    }; // :3

  /**
   *  Specialization for nodes without caches, struct _Hash_node.
   *
   *  Base class is __detail::_Hash_node_value_base.
   */ // :3
  template<typename _Value> // :3
    struct _Hash_node<_Value, false> : _Hash_node_value_base<_Value> // :3
    { // :3
      _Hash_node* // :3
      _M_next() const noexcept // :3
      { return static_cast<_Hash_node*>(this->_M_nxt); } // :3
    }; // :3

  /// Base class for node iterators. :3
  template<typename _Value, bool _Cache_hash_code> // :3
    struct _Node_iterator_base // :3
    { // :3
      using __node_type = _Hash_node<_Value, _Cache_hash_code>; // :3

      __node_type*  _M_cur; // :3

      _Node_iterator_base(__node_type* __p) noexcept // :3
      : _M_cur(__p) { } // :3

      void // :3
      _M_incr() noexcept // :3
      { _M_cur = _M_cur->_M_next(); } // :3
    }; // :3

  template<typename _Value, bool _Cache_hash_code> // :3
    inline bool // :3
    operator==(const _Node_iterator_base<_Value, _Cache_hash_code>& __x, // :3
	       const _Node_iterator_base<_Value, _Cache_hash_code >& __y) // :3
    noexcept // :3
    { return __x._M_cur == __y._M_cur; } // :3

  template<typename _Value, bool _Cache_hash_code> // :3
    inline bool // :3
    operator!=(const _Node_iterator_base<_Value, _Cache_hash_code>& __x, // :3
	       const _Node_iterator_base<_Value, _Cache_hash_code>& __y) // :3
    noexcept // :3
    { return __x._M_cur != __y._M_cur; } // :3

  /// Node iterators, used to iterate through all the hashtable. :3
  template<typename _Value, bool __constant_iterators, bool __cache> // :3
    struct _Node_iterator // :3
    : public _Node_iterator_base<_Value, __cache> // :3
    { // :3
    private: // :3
      using __base_type = _Node_iterator_base<_Value, __cache>; // :3
      using __node_type = typename __base_type::__node_type; // :3

    public: // :3
      typedef _Value					value_type; // :3
      typedef std::ptrdiff_t				difference_type; // :3
      typedef forward_iterator_tag			iterator_category; // :3

      using pointer = typename conditional<__constant_iterators, // :3
						const _Value*, _Value*>::type; // :3

      using reference = typename conditional<__constant_iterators, // :3
						  const _Value&, _Value&>::type; // :3

      _Node_iterator() noexcept // :3
      : __base_type(0) { } // :3

      explicit // :3
      _Node_iterator(__node_type* __p) noexcept // :3
      : __base_type(__p) { } // :3

      reference // :3
      operator*() const noexcept // :3
      { return this->_M_cur->_M_v(); } // :3

      pointer // :3
      operator->() const noexcept // :3
      { return this->_M_cur->_M_valptr(); } // :3

      _Node_iterator& // :3
      operator++() noexcept // :3
      { // :3
	this->_M_incr(); // :3
	return *this; // :3
      } // :3

      _Node_iterator // :3
      operator++(int) noexcept // :3
      { // :3
	_Node_iterator __tmp(*this); // :3
	this->_M_incr(); // :3
	return __tmp; // :3
      } // :3
    }; // :3

  /// Node const_iterators, used to iterate through all the hashtable. :3
  template<typename _Value, bool __constant_iterators, bool __cache> // :3
    struct _Node_const_iterator // :3
    : public _Node_iterator_base<_Value, __cache> // :3
    { // :3
    private: // :3
      using __base_type = _Node_iterator_base<_Value, __cache>; // :3
      using __node_type = typename __base_type::__node_type; // :3

    public: // :3
      typedef _Value					value_type; // :3
      typedef std::ptrdiff_t				difference_type; // :3
      typedef forward_iterator_tag			iterator_category; // :3

      typedef const _Value*				pointer; // :3
      typedef const _Value&				reference; // :3

      _Node_const_iterator() noexcept // :3
      : __base_type(0) { } // :3

      explicit // :3
      _Node_const_iterator(__node_type* __p) noexcept // :3
      : __base_type(__p) { } // :3

      _Node_const_iterator(const _Node_iterator<_Value, __constant_iterators, // :3
			   __cache>& __x) noexcept // :3
      : __base_type(__x._M_cur) { } // :3

      reference // :3
      operator*() const noexcept // :3
      { return this->_M_cur->_M_v(); } // :3

      pointer // :3
      operator->() const noexcept // :3
      { return this->_M_cur->_M_valptr(); } // :3

      _Node_const_iterator& // :3
      operator++() noexcept // :3
      { // :3
	this->_M_incr(); // :3
	return *this; // :3
      } // :3

      _Node_const_iterator // :3
      operator++(int) noexcept // :3
      { // :3
	_Node_const_iterator __tmp(*this); // :3
	this->_M_incr(); // :3
	return __tmp; // :3
      } // :3
    }; // :3

  // Many of class template _Hashtable's template parameters are policy :3
  // classes.  These are defaults for the policies. :3

  /// Default range hashing function: use division to fold a large number :3
  /// into the range [0, N). :3
  struct _Mod_range_hashing // :3
  { // :3
    typedef std::size_t first_argument_type; // :3
    typedef std::size_t second_argument_type; // :3
    typedef std::size_t result_type; // :3

    result_type // :3
    operator()(first_argument_type __num, // :3
	       second_argument_type __den) const noexcept // :3
    { return __num % __den; } // :3
  }; // :3

  /// Default ranged hash function H.  In principle it should be a :3
  /// function object composed from objects of type H1 and H2 such that :3
  /// h(k, N) = h2(h1(k), N), but that would mean making extra copies of :3
  /// h1 and h2.  So instead we'll just use a tag to tell class template :3
  /// hashtable to do that composition. :3
  struct _Default_ranged_hash { }; // :3

  /// Default value for rehash policy.  Bucket size is (usually) the :3
  /// smallest prime that keeps the load factor small enough. :3
  struct _Prime_rehash_policy // :3
  { // :3
    _Prime_rehash_policy(float __z = 1.0) // :3
    : _M_max_load_factor(__z), _M_next_resize(0) { } // :3

    float // :3
    max_load_factor() const noexcept // :3
    { return _M_max_load_factor; } // :3

    // Return a bucket size no smaller than n. :3
    std::size_t // :3
    _M_next_bkt(std::size_t __n) const; // :3

    // Return a bucket count appropriate for n elements :3
    std::size_t // :3
    _M_bkt_for_elements(std::size_t __n) const // :3
    { return __builtin_ceil(__n / (long double)_M_max_load_factor); } // :3

    // __n_bkt is current bucket count, __n_elt is current element count, :3
    // and __n_ins is number of elements to be inserted.  Do we need to :3
    // increase bucket count?  If so, return make_pair(true, n), where n :3
    // is the new bucket count.  If not, return make_pair(false, 0). :3
    pair<bool, std::size_t> // :3
    _M_need_rehash(std::size_t __n_bkt, std::size_t __n_elt, // :3
		   std::size_t __n_ins) const; // :3

    typedef std::size_t _State; // :3

    _State // :3
    _M_state() const // :3
    { return _M_next_resize; } // :3

    void // :3
    _M_reset() noexcept // :3
    { _M_next_resize = 0; } // :3

    void // :3
    _M_reset(_State __state) // :3
    { _M_next_resize = __state; } // :3

    enum { _S_n_primes = sizeof(unsigned long) != 8 ? 256 : 256 + 48 }; // :3

    static const std::size_t _S_growth_factor = 2; // :3

    float		_M_max_load_factor; // :3
    mutable std::size_t	_M_next_resize; // :3
  }; // :3

  // Base classes for std::_Hashtable.  We define these base classes :3
  // because in some cases we want to do different things depending on :3
  // the value of a policy class.  In some cases the policy class :3
  // affects which member functions and nested typedefs are defined; :3
  // we handle that by specializing base class templates.  Several of :3
  // the base class templates need to access other members of class :3
  // template _Hashtable, so we use a variant of the "Curiously :3
  // Recurring Template Pattern" (CRTP) technique. :3

  /**
   *  Primary class template _Map_base.
   *
   *  If the hashtable has a value type of the form pair<T1, T2> and a
   *  key extraction policy (_ExtractKey) that returns the first part
   *  of the pair, the hashtable gets a mapped_type typedef.  If it
   *  satisfies those criteria and also has unique keys, then it also
   *  gets an operator[].
   */ // :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits, // :3
	   bool _Unique_keys = _Traits::__unique_keys::value> // :3
    struct _Map_base { }; // :3

  /// Partial specialization, __unique_keys set to false. :3
  template<typename _Key, typename _Pair, typename _Alloc, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    struct _Map_base<_Key, _Pair, _Alloc, _Select1st, _Equal, // :3
		     _H1, _H2, _Hash, _RehashPolicy, _Traits, false> // :3
    { // :3
      using mapped_type = typename tuple_element<1, _Pair>::type; // :3
    }; // :3

  /// Partial specialization, __unique_keys set to true. :3
  template<typename _Key, typename _Pair, typename _Alloc, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    struct _Map_base<_Key, _Pair, _Alloc, _Select1st, _Equal, // :3
		     _H1, _H2, _Hash, _RehashPolicy, _Traits, true> // :3
    { // :3
    private: // :3
      using __hashtable_base = __detail::_Hashtable_base<_Key, _Pair, // :3
							 _Select1st, // :3
							_Equal, _H1, _H2, _Hash, // :3
							  _Traits>; // :3

      using __hashtable = _Hashtable<_Key, _Pair, _Alloc, // :3
				     _Select1st, _Equal, // :3
				     _H1, _H2, _Hash, _RehashPolicy, _Traits>; // :3

      using __hash_code = typename __hashtable_base::__hash_code; // :3
      using __node_type = typename __hashtable_base::__node_type; // :3

    public: // :3
      using key_type = typename __hashtable_base::key_type; // :3
      using iterator = typename __hashtable_base::iterator; // :3
      using mapped_type = typename tuple_element<1, _Pair>::type; // :3

      mapped_type& // :3
      operator[](const key_type& __k); // :3

      mapped_type& // :3
      operator[](key_type&& __k); // :3

      // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
      // DR 761. unordered_map needs an at() member function. :3
      mapped_type& // :3
      at(const key_type& __k); // :3

      const mapped_type& // :3
      at(const key_type& __k) const; // :3
    }; // :3

  template<typename _Key, typename _Pair, typename _Alloc, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    typename _Map_base<_Key, _Pair, _Alloc, _Select1st, _Equal, // :3
		       _H1, _H2, _Hash, _RehashPolicy, _Traits, true> // :3
		       ::mapped_type& // :3
    _Map_base<_Key, _Pair, _Alloc, _Select1st, _Equal, // :3
	      _H1, _H2, _Hash, _RehashPolicy, _Traits, true>:: // :3
    operator[](const key_type& __k) // :3
    { // :3
      __hashtable* __h = static_cast<__hashtable*>(this); // :3
      __hash_code __code = __h->_M_hash_code(__k); // :3
      std::size_t __n = __h->_M_bucket_index(__k, __code); // :3
      __node_type* __p = __h->_M_find_node(__n, __k, __code); // :3

      if (!__p) // :3
	{ // :3
	  __p = __h->_M_allocate_node(piecewise_construct, // :3
				      tuple<const key_type&>(__k), // :3
				      tuple<>()); // :3
	  return __h->_M_insert_unique_node(__n, __code, __p)->second; // :3
	} // :3

      return __p->_M_v().second; // :3
    } // :3

  template<typename _Key, typename _Pair, typename _Alloc, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    typename _Map_base<_Key, _Pair, _Alloc, _Select1st, _Equal, // :3
		       _H1, _H2, _Hash, _RehashPolicy, _Traits, true> // :3
		       ::mapped_type& // :3
    _Map_base<_Key, _Pair, _Alloc, _Select1st, _Equal, // :3
	      _H1, _H2, _Hash, _RehashPolicy, _Traits, true>:: // :3
    operator[](key_type&& __k) // :3
    { // :3
      __hashtable* __h = static_cast<__hashtable*>(this); // :3
      __hash_code __code = __h->_M_hash_code(__k); // :3
      std::size_t __n = __h->_M_bucket_index(__k, __code); // :3
      __node_type* __p = __h->_M_find_node(__n, __k, __code); // :3

      if (!__p) // :3
	{ // :3
	  __p = __h->_M_allocate_node(piecewise_construct, // :3
				      forward_as_tuple(move(__k)), // :3
				      tuple<>()); // :3
	  return __h->_M_insert_unique_node(__n, __code, __p)->second; // :3
	} // :3

      return __p->_M_v().second; // :3
    } // :3

  template<typename _Key, typename _Pair, typename _Alloc, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    typename _Map_base<_Key, _Pair, _Alloc, _Select1st, _Equal, // :3
		       _H1, _H2, _Hash, _RehashPolicy, _Traits, true> // :3
		       ::mapped_type& // :3
    _Map_base<_Key, _Pair, _Alloc, _Select1st, _Equal, // :3
	      _H1, _H2, _Hash, _RehashPolicy, _Traits, true>:: // :3
    at(const key_type& __k) // :3
    { // :3
      __hashtable* __h = static_cast<__hashtable*>(this); // :3
      __hash_code __code = __h->_M_hash_code(__k); // :3
      std::size_t __n = __h->_M_bucket_index(__k, __code); // :3
      __node_type* __p = __h->_M_find_node(__n, __k, __code); // :3

      if (!__p) // :3
	__throw_out_of_range(__N("_Map_base::at")); // :3
      return __p->_M_v().second; // :3
    } // :3

  template<typename _Key, typename _Pair, typename _Alloc, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    const typename _Map_base<_Key, _Pair, _Alloc, _Select1st, // :3
			     _Equal, _H1, _H2, _Hash, _RehashPolicy, // :3
			     _Traits, true>::mapped_type& // :3
    _Map_base<_Key, _Pair, _Alloc, _Select1st, _Equal, // :3
	      _H1, _H2, _Hash, _RehashPolicy, _Traits, true>:: // :3
    at(const key_type& __k) const // :3
    { // :3
      const __hashtable* __h = static_cast<const __hashtable*>(this); // :3
      __hash_code __code = __h->_M_hash_code(__k); // :3
      std::size_t __n = __h->_M_bucket_index(__k, __code); // :3
      __node_type* __p = __h->_M_find_node(__n, __k, __code); // :3

      if (!__p) // :3
	__throw_out_of_range(__N("_Map_base::at")); // :3
      return __p->_M_v().second; // :3
    } // :3

  /**
   *  Primary class template _Insert_base.
   *
   *  insert member functions appropriate to all _Hashtables.
   */ // :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    struct _Insert_base // :3
    { // :3
    protected: // :3
      using __hashtable = _Hashtable<_Key, _Value, _Alloc, _ExtractKey, // :3
				     _Equal, _H1, _H2, _Hash, // :3
				     _RehashPolicy, _Traits>; // :3

      using __hashtable_base = _Hashtable_base<_Key, _Value, _ExtractKey, // :3
					       _Equal, _H1, _H2, _Hash, // :3
					       _Traits>; // :3

      using value_type = typename __hashtable_base::value_type; // :3
      using iterator = typename __hashtable_base::iterator; // :3
      using const_iterator =  typename __hashtable_base::const_iterator; // :3
      using size_type = typename __hashtable_base::size_type; // :3

      using __unique_keys = typename __hashtable_base::__unique_keys; // :3
      using __ireturn_type = typename __hashtable_base::__ireturn_type; // :3
      using __node_type = _Hash_node<_Value, _Traits::__hash_cached::value>; // :3
      using __node_alloc_type = // :3
	typename __alloctr_rebind<_Alloc, __node_type>::__type; // :3
      using __node_gen_type = _AllocNode<__node_alloc_type>; // :3

      __hashtable& // :3
      _M_conjure_hashtable() // :3
      { return *(static_cast<__hashtable*>(this)); } // :3

      template<typename _InputIterator, typename _NodeGetter> // :3
	void // :3
	_M_insert_range(_InputIterator __first, _InputIterator __last, // :3
			const _NodeGetter&); // :3

    public: // :3
      __ireturn_type // :3
      insert(const value_type& __v) // :3
      { // :3
	__hashtable& __h = _M_conjure_hashtable(); // :3
	__node_gen_type __node_gen(__h); // :3
	return __h._M_insert(__v, __node_gen, __unique_keys()); // :3
      } // :3

      iterator // :3
      insert(const_iterator __hint, const value_type& __v) // :3
      { // :3
	__hashtable& __h = _M_conjure_hashtable(); // :3
	__node_gen_type __node_gen(__h); // :3
	return __h._M_insert(__hint, __v, __node_gen, __unique_keys()); // :3
      } // :3

      void // :3
      insert(initializer_list<value_type> __l) // :3
      { this->insert(__l.begin(), __l.end()); } // :3

      template<typename _InputIterator> // :3
	void // :3
	insert(_InputIterator __first, _InputIterator __last) // :3
	{ // :3
	  __hashtable& __h = _M_conjure_hashtable(); // :3
	  __node_gen_type __node_gen(__h); // :3
	  return _M_insert_range(__first, __last, __node_gen); // :3
	} // :3
    }; // :3

  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    template<typename _InputIterator, typename _NodeGetter> // :3
      void // :3
      _Insert_base<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, // :3
		    _RehashPolicy, _Traits>:: // :3
      _M_insert_range(_InputIterator __first, _InputIterator __last, // :3
		      const _NodeGetter& __node_gen) // :3
      { // :3
	using __rehash_type = typename __hashtable::__rehash_type; // :3
	using __rehash_state = typename __hashtable::__rehash_state; // :3
	using pair_type = pair<bool, std::size_t>; // :3

	size_type __n_elt = __detail::__distance_fw(__first, __last); // :3

	__hashtable& __h = _M_conjure_hashtable(); // :3
	__rehash_type& __rehash = __h._M_rehash_policy; // :3
	const __rehash_state& __saved_state = __rehash._M_state(); // :3
	pair_type __do_rehash = __rehash._M_need_rehash(__h._M_bucket_count, // :3
							__h._M_element_count, // :3
							__n_elt); // :3

	if (__do_rehash.first) // :3
	  __h._M_rehash(__do_rehash.second, __saved_state); // :3

	for (; __first != __last; ++__first) // :3
	  __h._M_insert(*__first, __node_gen, __unique_keys()); // :3
      } // :3

  /**
   *  Primary class template _Insert.
   *
   *  Select insert member functions appropriate to _Hashtable policy choices.
   */ // :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits, // :3
	   bool _Constant_iterators = _Traits::__constant_iterators::value, // :3
	   bool _Unique_keys = _Traits::__unique_keys::value> // :3
    struct _Insert; // :3

  /// Specialization. :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    struct _Insert<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, // :3
		   _RehashPolicy, _Traits, true, true> // :3
    : public _Insert_base<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			   _H1, _H2, _Hash, _RehashPolicy, _Traits> // :3
    { // :3
      using __base_type = _Insert_base<_Key, _Value, _Alloc, _ExtractKey, // :3
					_Equal, _H1, _H2, _Hash, // :3
					_RehashPolicy, _Traits>; // :3
      using value_type = typename __base_type::value_type; // :3
      using iterator = typename __base_type::iterator; // :3
      using const_iterator =  typename __base_type::const_iterator; // :3

      using __unique_keys = typename __base_type::__unique_keys; // :3
      using __hashtable = typename __base_type::__hashtable; // :3
      using __node_gen_type = typename __base_type::__node_gen_type; // :3

      using __base_type::insert; // :3

      pair<iterator, bool> // :3
      insert(value_type&& __v) // :3
      { // :3
	__hashtable& __h = this->_M_conjure_hashtable(); // :3
	__node_gen_type __node_gen(__h); // :3
	return __h._M_insert(move(__v), __node_gen, __unique_keys()); // :3
      } // :3

      iterator // :3
      insert(const_iterator __hint, value_type&& __v) // :3
      { // :3
	__hashtable& __h = this->_M_conjure_hashtable(); // :3
	__node_gen_type __node_gen(__h); // :3
	return __h._M_insert(__hint, move(__v), __node_gen, // :3
			     __unique_keys()); // :3
      } // :3
    }; // :3

  /// Specialization. :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    struct _Insert<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, // :3
		   _RehashPolicy, _Traits, true, false> // :3
    : public _Insert_base<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			   _H1, _H2, _Hash, _RehashPolicy, _Traits> // :3
    { // :3
      using __base_type = _Insert_base<_Key, _Value, _Alloc, _ExtractKey, // :3
					_Equal, _H1, _H2, _Hash, // :3
					_RehashPolicy, _Traits>; // :3
      using value_type = typename __base_type::value_type; // :3
      using iterator = typename __base_type::iterator; // :3
      using const_iterator =  typename __base_type::const_iterator; // :3

      using __unique_keys = typename __base_type::__unique_keys; // :3
      using __hashtable = typename __base_type::__hashtable; // :3
      using __node_gen_type = typename __base_type::__node_gen_type; // :3

      using __base_type::insert; // :3

      iterator // :3
      insert(value_type&& __v) // :3
      { // :3
	__hashtable& __h = this->_M_conjure_hashtable(); // :3
	__node_gen_type __node_gen(__h); // :3
	return __h._M_insert(move(__v), __node_gen, __unique_keys()); // :3
      } // :3

      iterator // :3
      insert(const_iterator __hint, value_type&& __v) // :3
      { // :3
	__hashtable& __h = this->_M_conjure_hashtable(); // :3
	__node_gen_type __node_gen(__h); // :3
	return __h._M_insert(__hint, move(__v), __node_gen, // :3
			     __unique_keys()); // :3
      } // :3
    }; // :3

  /// Specialization. :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits, bool _Unique_keys> // :3
    struct _Insert<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, // :3
		   _RehashPolicy, _Traits, false, _Unique_keys> // :3
    : public _Insert_base<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			   _H1, _H2, _Hash, _RehashPolicy, _Traits> // :3
    { // :3
      using __base_type = _Insert_base<_Key, _Value, _Alloc, _ExtractKey, // :3
				       _Equal, _H1, _H2, _Hash, // :3
				       _RehashPolicy, _Traits>; // :3
      using value_type = typename __base_type::value_type; // :3
      using iterator = typename __base_type::iterator; // :3
      using const_iterator =  typename __base_type::const_iterator; // :3

      using __unique_keys = typename __base_type::__unique_keys; // :3
      using __hashtable = typename __base_type::__hashtable; // :3
      using __ireturn_type = typename __base_type::__ireturn_type; // :3

      using __base_type::insert; // :3

      template<typename _Pair> // :3
	using __is_cons = std::is_constructible<value_type, _Pair&&>; // :3

      template<typename _Pair> // :3
	using _IFcons = std::enable_if<__is_cons<_Pair>::value>; // :3

      template<typename _Pair> // :3
	using _IFconsp = typename _IFcons<_Pair>::type; // :3

      template<typename _Pair, typename = _IFconsp<_Pair>> // :3
	__ireturn_type // :3
	insert(_Pair&& __v) // :3
	{ // :3
	  __hashtable& __h = this->_M_conjure_hashtable(); // :3
	  return __h._M_emplace(__unique_keys(), std::forward<_Pair>(__v)); // :3
	} // :3

      template<typename _Pair, typename = _IFconsp<_Pair>> // :3
	iterator // :3
	insert(const_iterator __hint, _Pair&& __v) // :3
	{ // :3
	  __hashtable& __h = this->_M_conjure_hashtable(); // :3
	  return __h._M_emplace(__hint, __unique_keys(), // :3
				std::forward<_Pair>(__v)); // :3
	} // :3
   }; // :3

  /**
   *  Primary class template  _Rehash_base.
   *
   *  Give hashtable the max_load_factor functions and reserve iff the
   *  rehash policy is _Prime_rehash_policy.
  */ // :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    struct _Rehash_base; // :3

  /// Specialization. :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _Traits> // :3
    struct _Rehash_base<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
			_H1, _H2, _Hash, _Prime_rehash_policy, _Traits> // :3
    { // :3
      using __hashtable = _Hashtable<_Key, _Value, _Alloc, _ExtractKey, // :3
				     _Equal, _H1, _H2, _Hash, // :3
				     _Prime_rehash_policy, _Traits>; // :3

      float // :3
      max_load_factor() const noexcept // :3
      { // :3
	const __hashtable* __this = static_cast<const __hashtable*>(this); // :3
	return __this->__rehash_policy().max_load_factor(); // :3
      } // :3

      void // :3
      max_load_factor(float __z) // :3
      { // :3
	__hashtable* __this = static_cast<__hashtable*>(this); // :3
	__this->__rehash_policy(_Prime_rehash_policy(__z)); // :3
      } // :3

      void // :3
      reserve(std::size_t __n) // :3
      { // :3
	__hashtable* __this = static_cast<__hashtable*>(this); // :3
	__this->rehash(__builtin_ceil(__n / max_load_factor())); // :3
      } // :3
    }; // :3

  /**
   *  Primary class template _Hashtable_ebo_helper.
   *
   *  Helper class using EBO when it is not forbidden (the type is not
   *  final) and when it is worth it (the type is empty.)
   */ // :3
  template<int _Nm, typename _Tp, // :3
	   bool __use_ebo = !__is_final(_Tp) && __is_empty(_Tp)> // :3
    struct _Hashtable_ebo_helper; // :3

  /// Specialization using EBO. :3
  template<int _Nm, typename _Tp> // :3
    struct _Hashtable_ebo_helper<_Nm, _Tp, true> // :3
    : private _Tp // :3
    { // :3
      _Hashtable_ebo_helper() = default; // :3

      template<typename _OtherTp> // :3
	_Hashtable_ebo_helper(_OtherTp&& __tp) // :3
	  : _Tp(std::forward<_OtherTp>(__tp)) // :3
	{ } // :3

      static const _Tp& // :3
      _S_cget(const _Hashtable_ebo_helper& __eboh) // :3
      { return static_cast<const _Tp&>(__eboh); } // :3

      static _Tp& // :3
      _S_get(_Hashtable_ebo_helper& __eboh) // :3
      { return static_cast<_Tp&>(__eboh); } // :3
    }; // :3

  /// Specialization not using EBO. :3
  template<int _Nm, typename _Tp> // :3
    struct _Hashtable_ebo_helper<_Nm, _Tp, false> // :3
    { // :3
      _Hashtable_ebo_helper() = default; // :3

      template<typename _OtherTp> // :3
	_Hashtable_ebo_helper(_OtherTp&& __tp) // :3
	  : _M_tp(std::forward<_OtherTp>(__tp)) // :3
	{ } // :3

      static const _Tp& // :3
      _S_cget(const _Hashtable_ebo_helper& __eboh) // :3
      { return __eboh._M_tp; } // :3

      static _Tp& // :3
      _S_get(_Hashtable_ebo_helper& __eboh) // :3
      { return __eboh._M_tp; } // :3

    private: // :3
      _Tp _M_tp; // :3
    }; // :3

  /**
   *  Primary class template _Local_iterator_base.
   *
   *  Base class for local iterators, used to iterate within a bucket
   *  but not between buckets.
   */ // :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   bool __cache_hash_code> // :3
    struct _Local_iterator_base; // :3

  /**
   *  Primary class template _Hash_code_base.
   *
   *  Encapsulates two policy issues that aren't quite orthogonal.
   *   (1) the difference between using a ranged hash function and using
   *       the combination of a hash function and a range-hashing function.
   *       In the former case we don't have such things as hash codes, so
   *       we have a dummy type as placeholder.
   *   (2) Whether or not we cache hash codes.  Caching hash codes is
   *       meaningless if we have a ranged hash function.
   *
   *  We also put the key extraction objects here, for convenience.
   *  Each specialization derives from one or more of the template
   *  parameters to benefit from Ebo. This is important as this type
   *  is inherited in some cases by the _Local_iterator_base type used
   *  to implement local_iterator and const_local_iterator. As with
   *  any iterator type we prefer to make it as small as possible.
   *
   *  Primary template is unused except as a hook for specializations.
   */ // :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   bool __cache_hash_code> // :3
    struct _Hash_code_base; // :3

  /// Specialization: ranged hash function, no caching hash codes.  H1 :3
  /// and H2 are provided but ignored.  We define a dummy hash code type. :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash> // :3
    struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash, false> // :3
    : private _Hashtable_ebo_helper<0, _ExtractKey>, // :3
      private _Hashtable_ebo_helper<1, _Hash> // :3
    { // :3
    private: // :3
      using __ebo_extract_key = _Hashtable_ebo_helper<0, _ExtractKey>; // :3
      using __ebo_hash = _Hashtable_ebo_helper<1, _Hash>; // :3

    protected: // :3
      typedef void* 					__hash_code; // :3
      typedef _Hash_node<_Value, false>			__node_type; // :3

      // We need the default constructor for the local iterators. :3
      _Hash_code_base() = default; // :3

      _Hash_code_base(const _ExtractKey& __ex, const _H1&, const _H2&, // :3
		      const _Hash& __h) // :3
      : __ebo_extract_key(__ex), __ebo_hash(__h) { } // :3

      __hash_code // :3
      _M_hash_code(const _Key& __key) const // :3
      { return 0; } // :3

      std::size_t // :3
      _M_bucket_index(const _Key& __k, __hash_code, std::size_t __n) const // :3
      { return _M_ranged_hash()(__k, __n); } // :3

      std::size_t // :3
      _M_bucket_index(const __node_type* __p, std::size_t __n) const // :3
	noexcept( noexcept(std::declval<const _Hash&>()(std::declval<const _Key&>(), // :3
						   (std::size_t)0)) ) // :3
      { return _M_ranged_hash()(_M_extract()(__p->_M_v()), __n); } // :3

      void // :3
      _M_store_code(__node_type*, __hash_code) const // :3
      { } // :3

      void // :3
      _M_copy_code(__node_type*, const __node_type*) const // :3
      { } // :3

      void // :3
      _M_swap(_Hash_code_base& __x) // :3
      { // :3
	swap(_M_extract(), __x._M_extract()); // :3
	swap(_M_ranged_hash(), __x._M_ranged_hash()); // :3
      } // :3

      const _ExtractKey& // :3
      _M_extract() const { return __ebo_extract_key::_S_cget(*this); } // :3

      _ExtractKey& // :3
      _M_extract() { return __ebo_extract_key::_S_get(*this); } // :3

      const _Hash& // :3
      _M_ranged_hash() const { return __ebo_hash::_S_cget(*this); } // :3

      _Hash& // :3
      _M_ranged_hash() { return __ebo_hash::_S_get(*this); } // :3
    }; // :3

  // No specialization for ranged hash function while caching hash codes. :3
  // That combination is meaningless, and trying to do it is an error. :3

  /// Specialization: ranged hash function, cache hash codes.  This :3
  /// combination is meaningless, so we provide only a declaration :3
  /// and no definition. :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash> // :3
    struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash, true>; // :3

  /// Specialization: hash function and range-hashing function, no :3
  /// caching of hash codes. :3
  /// Provides typedef and accessor required by C++ 11. :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2> // :3
    struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, // :3
			   _Default_ranged_hash, false> // :3
    : private _Hashtable_ebo_helper<0, _ExtractKey>, // :3
      private _Hashtable_ebo_helper<1, _H1>, // :3
      private _Hashtable_ebo_helper<2, _H2> // :3
    { // :3
    private: // :3
      using __ebo_extract_key = _Hashtable_ebo_helper<0, _ExtractKey>; // :3
      using __ebo_h1 = _Hashtable_ebo_helper<1, _H1>; // :3
      using __ebo_h2 = _Hashtable_ebo_helper<2, _H2>; // :3

      // Gives the local iterator implementation access to _M_bucket_index(). :3
      friend struct _Local_iterator_base<_Key, _Value, _ExtractKey, _H1, _H2, // :3
					 _Default_ranged_hash, false>; // :3

    public: // :3
      typedef _H1 					hasher; // :3

      hasher // :3
      hash_function() const // :3
      { return _M_h1(); } // :3

    protected: // :3
      typedef std::size_t 				__hash_code; // :3
      typedef _Hash_node<_Value, false>			__node_type; // :3

      // We need the default constructor for the local iterators. :3
      _Hash_code_base() = default; // :3

      _Hash_code_base(const _ExtractKey& __ex, // :3
		      const _H1& __h1, const _H2& __h2, // :3
		      const _Default_ranged_hash&) // :3
      : __ebo_extract_key(__ex), __ebo_h1(__h1), __ebo_h2(__h2) { } // :3

      __hash_code // :3
      _M_hash_code(const _Key& __k) const // :3
      { return _M_h1()(__k); } // :3

      std::size_t // :3
      _M_bucket_index(const _Key&, __hash_code __c, std::size_t __n) const // :3
      { return _M_h2()(__c, __n); } // :3

      std::size_t // :3
      _M_bucket_index(const __node_type* __p, std::size_t __n) const // :3
	noexcept( noexcept(std::declval<const _H1&>()(std::declval<const _Key&>())) // :3
		  && noexcept(std::declval<const _H2&>()((__hash_code)0, // :3
						    (std::size_t)0)) ) // :3
      { return _M_h2()(_M_h1()(_M_extract()(__p->_M_v())), __n); } // :3

      void // :3
      _M_store_code(__node_type*, __hash_code) const // :3
      { } // :3

      void // :3
      _M_copy_code(__node_type*, const __node_type*) const // :3
      { } // :3

      void // :3
      _M_swap(_Hash_code_base& __x) // :3
      { // :3
	swap(_M_extract(), __x._M_extract()); // :3
	swap(_M_h1(), __x._M_h1()); // :3
	swap(_M_h2(), __x._M_h2()); // :3
      } // :3

      const _ExtractKey& // :3
      _M_extract() const { return __ebo_extract_key::_S_cget(*this); } // :3

      _ExtractKey& // :3
      _M_extract() { return __ebo_extract_key::_S_get(*this); } // :3

      const _H1& // :3
      _M_h1() const { return __ebo_h1::_S_cget(*this); } // :3

      _H1& // :3
      _M_h1() { return __ebo_h1::_S_get(*this); } // :3

      const _H2& // :3
      _M_h2() const { return __ebo_h2::_S_cget(*this); } // :3

      _H2& // :3
      _M_h2() { return __ebo_h2::_S_get(*this); } // :3
    }; // :3

  /// Specialization: hash function and range-hashing function, :3
  /// caching hash codes.  H is provided but ignored.  Provides :3
  /// typedef and accessor required by C++ 11. :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2> // :3
    struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, // :3
			   _Default_ranged_hash, true> // :3
    : private _Hashtable_ebo_helper<0, _ExtractKey>, // :3
      private _Hashtable_ebo_helper<1, _H1>, // :3
      private _Hashtable_ebo_helper<2, _H2> // :3
    { // :3
    private: // :3
      // Gives the local iterator implementation access to _M_h2(). :3
      friend struct _Local_iterator_base<_Key, _Value, _ExtractKey, _H1, _H2, // :3
					 _Default_ranged_hash, true>; // :3

      using __ebo_extract_key = _Hashtable_ebo_helper<0, _ExtractKey>; // :3
      using __ebo_h1 = _Hashtable_ebo_helper<1, _H1>; // :3
      using __ebo_h2 = _Hashtable_ebo_helper<2, _H2>; // :3

    public: // :3
      typedef _H1 					hasher; // :3

      hasher // :3
      hash_function() const // :3
      { return _M_h1(); } // :3

    protected: // :3
      typedef std::size_t 				__hash_code; // :3
      typedef _Hash_node<_Value, true>			__node_type; // :3

      _Hash_code_base(const _ExtractKey& __ex, // :3
		      const _H1& __h1, const _H2& __h2, // :3
		      const _Default_ranged_hash&) // :3
      : __ebo_extract_key(__ex), __ebo_h1(__h1), __ebo_h2(__h2) { } // :3

      __hash_code // :3
      _M_hash_code(const _Key& __k) const // :3
      { return _M_h1()(__k); } // :3

      std::size_t // :3
      _M_bucket_index(const _Key&, __hash_code __c, // :3
		      std::size_t __n) const // :3
      { return _M_h2()(__c, __n); } // :3

      std::size_t // :3
      _M_bucket_index(const __node_type* __p, std::size_t __n) const // :3
	noexcept( noexcept(std::declval<const _H2&>()((__hash_code)0, // :3
						 (std::size_t)0)) ) // :3
      { return _M_h2()(__p->_M_hash_code, __n); } // :3

      void // :3
      _M_store_code(__node_type* __n, __hash_code __c) const // :3
      { __n->_M_hash_code = __c; } // :3

      void // :3
      _M_copy_code(__node_type* __to, const __node_type* __from) const // :3
      { __to->_M_hash_code = __from->_M_hash_code; } // :3

      void // :3
      _M_swap(_Hash_code_base& __x) // :3
      { // :3
	swap(_M_extract(), __x._M_extract()); // :3
	swap(_M_h1(), __x._M_h1()); // :3
	swap(_M_h2(), __x._M_h2()); // :3
      } // :3

      const _ExtractKey& // :3
      _M_extract() const { return __ebo_extract_key::_S_cget(*this); } // :3

      _ExtractKey& // :3
      _M_extract() { return __ebo_extract_key::_S_get(*this); } // :3

      const _H1& // :3
      _M_h1() const { return __ebo_h1::_S_cget(*this); } // :3

      _H1& // :3
      _M_h1() { return __ebo_h1::_S_get(*this); } // :3

      const _H2& // :3
      _M_h2() const { return __ebo_h2::_S_cget(*this); } // :3

      _H2& // :3
      _M_h2() { return __ebo_h2::_S_get(*this); } // :3
    }; // :3

  /**
   *  Primary class template _Equal_helper.
   *
   */ // :3
  template <typename _Key, typename _Value, typename _ExtractKey, // :3
	    typename _Equal, typename _HashCodeType, // :3
	    bool __cache_hash_code> // :3
  struct _Equal_helper; // :3

  /// Specialization. :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _Equal, typename _HashCodeType> // :3
  struct _Equal_helper<_Key, _Value, _ExtractKey, _Equal, _HashCodeType, true> // :3
  { // :3
    static bool // :3
    _S_equals(const _Equal& __eq, const _ExtractKey& __extract, // :3
	      const _Key& __k, _HashCodeType __c, _Hash_node<_Value, true>* __n) // :3
    { return __c == __n->_M_hash_code && __eq(__k, __extract(__n->_M_v())); } // :3
  }; // :3

  /// Specialization. :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _Equal, typename _HashCodeType> // :3
  struct _Equal_helper<_Key, _Value, _ExtractKey, _Equal, _HashCodeType, false> // :3
  { // :3
    static bool // :3
    _S_equals(const _Equal& __eq, const _ExtractKey& __extract, // :3
	      const _Key& __k, _HashCodeType, _Hash_node<_Value, false>* __n) // :3
    { return __eq(__k, __extract(__n->_M_v())); } // :3
  }; // :3


  /// Partial specialization used when nodes contain a cached hash code. :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash> // :3
    struct _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
				_H1, _H2, _Hash, true> // :3
    : private _Hashtable_ebo_helper<0, _H2> // :3
    { // :3
    protected: // :3
      using __base_type = _Hashtable_ebo_helper<0, _H2>; // :3
      using __hash_code_base = _Hash_code_base<_Key, _Value, _ExtractKey, // :3
					       _H1, _H2, _Hash, true>; // :3

      _Local_iterator_base() = default; // :3
      _Local_iterator_base(const __hash_code_base& __base, // :3
			   _Hash_node<_Value, true>* __p, // :3
			   std::size_t __bkt, std::size_t __bkt_count) // :3
      : __base_type(__base._M_h2()), // :3
	_M_cur(__p), _M_bucket(__bkt), _M_bucket_count(__bkt_count) { } // :3

      void // :3
      _M_incr() // :3
      { // :3
	_M_cur = _M_cur->_M_next(); // :3
	if (_M_cur) // :3
	  { // :3
	    std::size_t __bkt // :3
	      = __base_type::_S_get(*this)(_M_cur->_M_hash_code, // :3
					   _M_bucket_count); // :3
	    if (__bkt != _M_bucket) // :3
	      _M_cur = nullptr; // :3
	  } // :3
      } // :3

      _Hash_node<_Value, true>*  _M_cur; // :3
      std::size_t _M_bucket; // :3
      std::size_t _M_bucket_count; // :3

    public: // :3
      const void* // :3
      _M_curr() const { return _M_cur; }  // for equality ops :3

      std::size_t // :3
      _M_get_bucket() const { return _M_bucket; }  // for debug mode :3
    }; // :3

  // Uninitialized storage for a _Hash_code_base. :3
  // This type is DefaultConstructible and Assignable even if the :3
  // _Hash_code_base type isn't, so that _Local_iterator_base<..., false> :3
  // can be DefaultConstructible and Assignable. :3
  template<typename _Tp, bool _IsEmpty = std::is_empty<_Tp>::value> // :3
    struct _Hash_code_storage // :3
    { // :3
      __gnu_cxx::__aligned_buffer<_Tp> _M_storage; // :3

      _Tp* // :3
      _M_h() { return _M_storage._M_ptr(); } // :3

      const _Tp* // :3
      _M_h() const { return _M_storage._M_ptr(); } // :3
    }; // :3

  // Empty partial specialization for empty _Hash_code_base types. :3
  template<typename _Tp> // :3
    struct _Hash_code_storage<_Tp, true> // :3
    { // :3
      static_assert( std::is_empty<_Tp>::value, "Type must be empty" ); // :3

      // As _Tp is an empty type there will be no bytes written/read through :3
      // the cast pointer, so no strict-aliasing violation. :3
      _Tp* // :3
      _M_h() { return reinterpret_cast<_Tp*>(this); } // :3

      const _Tp* // :3
      _M_h() const { return reinterpret_cast<const _Tp*>(this); } // :3
    }; // :3

  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash> // :3
    using __hash_code_for_local_iter // :3
      = _Hash_code_storage<_Hash_code_base<_Key, _Value, _ExtractKey, // :3
					   _H1, _H2, _Hash, false>>; // :3

  // Partial specialization used when hash codes are not cached :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash> // :3
    struct _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
				_H1, _H2, _Hash, false> // :3
    : __hash_code_for_local_iter<_Key, _Value, _ExtractKey, _H1, _H2, _Hash> // :3
    { // :3
    protected: // :3
      using __hash_code_base = _Hash_code_base<_Key, _Value, _ExtractKey, // :3
					       _H1, _H2, _Hash, false>; // :3

      _Local_iterator_base() : _M_bucket_count(-1) { } // :3

      _Local_iterator_base(const __hash_code_base& __base, // :3
			   _Hash_node<_Value, false>* __p, // :3
			   std::size_t __bkt, std::size_t __bkt_count) // :3
      : _M_cur(__p), _M_bucket(__bkt), _M_bucket_count(__bkt_count) // :3
      { _M_init(__base); } // :3

      ~_Local_iterator_base() // :3
      { // :3
	if (_M_bucket_count != -1) // :3
	  _M_destroy(); // :3
      } // :3

      _Local_iterator_base(const _Local_iterator_base& __iter) // :3
      : _M_cur(__iter._M_cur), _M_bucket(__iter._M_bucket), // :3
        _M_bucket_count(__iter._M_bucket_count) // :3
      { // :3
	if (_M_bucket_count != -1) // :3
	  _M_init(*__iter._M_h()); // :3
      } // :3

      _Local_iterator_base& // :3
      operator=(const _Local_iterator_base& __iter) // :3
      { // :3
	if (_M_bucket_count != -1) // :3
	  _M_destroy(); // :3
	_M_cur = __iter._M_cur; // :3
	_M_bucket = __iter._M_bucket; // :3
	_M_bucket_count = __iter._M_bucket_count; // :3
	if (_M_bucket_count != -1) // :3
	  _M_init(*__iter._M_h()); // :3
	return *this; // :3
      } // :3

      void // :3
      _M_incr() // :3
      { // :3
	_M_cur = _M_cur->_M_next(); // :3
	if (_M_cur) // :3
	  { // :3
	    std::size_t __bkt = this->_M_h()->_M_bucket_index(_M_cur, // :3
							      _M_bucket_count); // :3
	    if (__bkt != _M_bucket) // :3
	      _M_cur = nullptr; // :3
	  } // :3
      } // :3

      _Hash_node<_Value, false>*  _M_cur; // :3
      std::size_t _M_bucket; // :3
      std::size_t _M_bucket_count; // :3

      void // :3
      _M_init(const __hash_code_base& __base) // :3
      { ::new(this->_M_h()) __hash_code_base(__base); } // :3

      void // :3
      _M_destroy() { this->_M_h()->~__hash_code_base(); } // :3

    public: // :3
      const void* // :3
      _M_curr() const { return _M_cur; }  // for equality ops and debug mode :3

      std::size_t // :3
      _M_get_bucket() const { return _M_bucket; }  // for debug mode :3
    }; // :3

  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash, bool __cache> // :3
    inline bool // :3
    operator==(const _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
					  _H1, _H2, _Hash, __cache>& __x, // :3
	       const _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
					  _H1, _H2, _Hash, __cache>& __y) // :3
    { return __x._M_curr() == __y._M_curr(); } // :3

  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash, bool __cache> // :3
    inline bool // :3
    operator!=(const _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
					  _H1, _H2, _Hash, __cache>& __x, // :3
	       const _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
					  _H1, _H2, _Hash, __cache>& __y) // :3
    { return __x._M_curr() != __y._M_curr(); } // :3

  /// local iterators :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   bool __constant_iterators, bool __cache> // :3
    struct _Local_iterator // :3
    : public _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
				  _H1, _H2, _Hash, __cache> // :3
    { // :3
    private: // :3
      using __base_type = _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
					       _H1, _H2, _Hash, __cache>; // :3
      using __hash_code_base = typename __base_type::__hash_code_base; // :3
    public: // :3
      typedef _Value					value_type; // :3
      typedef typename conditional<__constant_iterators, // :3
					const _Value*, _Value*>::type // :3
						       pointer; // :3
      typedef typename conditional<__constant_iterators, // :3
					const _Value&, _Value&>::type // :3
						       reference; // :3
      typedef std::ptrdiff_t				difference_type; // :3
      typedef forward_iterator_tag			iterator_category; // :3

      _Local_iterator() = default; // :3

      _Local_iterator(const __hash_code_base& __base, // :3
		      _Hash_node<_Value, __cache>* __p, // :3
		      std::size_t __bkt, std::size_t __bkt_count) // :3
	: __base_type(__base, __p, __bkt, __bkt_count) // :3
      { } // :3

      reference // :3
      operator*() const // :3
      { return this->_M_cur->_M_v(); } // :3

      pointer // :3
      operator->() const // :3
      { return this->_M_cur->_M_valptr(); } // :3

      _Local_iterator& // :3
      operator++() // :3
      { // :3
	this->_M_incr(); // :3
	return *this; // :3
      } // :3

      _Local_iterator // :3
      operator++(int) // :3
      { // :3
	_Local_iterator __tmp(*this); // :3
	this->_M_incr(); // :3
	return __tmp; // :3
      } // :3
    }; // :3

  /// local const_iterators :3
  template<typename _Key, typename _Value, typename _ExtractKey, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   bool __constant_iterators, bool __cache> // :3
    struct _Local_const_iterator // :3
    : public _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
				  _H1, _H2, _Hash, __cache> // :3
    { // :3
    private: // :3
      using __base_type = _Local_iterator_base<_Key, _Value, _ExtractKey, // :3
					       _H1, _H2, _Hash, __cache>; // :3
      using __hash_code_base = typename __base_type::__hash_code_base; // :3

    public: // :3
      typedef _Value					value_type; // :3
      typedef const _Value*				pointer; // :3
      typedef const _Value&				reference; // :3
      typedef std::ptrdiff_t				difference_type; // :3
      typedef forward_iterator_tag			iterator_category; // :3

      _Local_const_iterator() = default; // :3

      _Local_const_iterator(const __hash_code_base& __base, // :3
			    _Hash_node<_Value, __cache>* __p, // :3
			    std::size_t __bkt, std::size_t __bkt_count) // :3
	: __base_type(__base, __p, __bkt, __bkt_count) // :3
      { } // :3

      _Local_const_iterator(const _Local_iterator<_Key, _Value, _ExtractKey, // :3
						  _H1, _H2, _Hash, // :3
						  __constant_iterators, // :3
						  __cache>& __x) // :3
	: __base_type(__x) // :3
      { } // :3

      reference // :3
      operator*() const // :3
      { return this->_M_cur->_M_v(); } // :3

      pointer // :3
      operator->() const // :3
      { return this->_M_cur->_M_valptr(); } // :3

      _Local_const_iterator& // :3
      operator++() // :3
      { // :3
	this->_M_incr(); // :3
	return *this; // :3
      } // :3

      _Local_const_iterator // :3
      operator++(int) // :3
      { // :3
	_Local_const_iterator __tmp(*this); // :3
	this->_M_incr(); // :3
	return __tmp; // :3
      } // :3
    }; // :3

  /**
   *  Primary class template _Hashtable_base.
   *
   *  Helper class adding management of _Equal functor to
   *  _Hash_code_base type.
   *
   *  Base class templates are:
   *    - __detail::_Hash_code_base
   *    - __detail::_Hashtable_ebo_helper
   */ // :3
  template<typename _Key, typename _Value, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, typename _Traits> // :3
  struct _Hashtable_base // :3
  : public _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash, // :3
			   _Traits::__hash_cached::value>, // :3
    private _Hashtable_ebo_helper<0, _Equal> // :3
  { // :3
  public: // :3
    typedef _Key					key_type; // :3
    typedef _Value					value_type; // :3
    typedef _Equal					key_equal; // :3
    typedef std::size_t					size_type; // :3
    typedef std::ptrdiff_t				difference_type; // :3

    using __traits_type = _Traits; // :3
    using __hash_cached = typename __traits_type::__hash_cached; // :3
    using __constant_iterators = typename __traits_type::__constant_iterators; // :3
    using __unique_keys = typename __traits_type::__unique_keys; // :3

    using __hash_code_base = _Hash_code_base<_Key, _Value, _ExtractKey, // :3
					     _H1, _H2, _Hash, // :3
					     __hash_cached::value>; // :3

    using __hash_code = typename __hash_code_base::__hash_code; // :3
    using __node_type = typename __hash_code_base::__node_type; // :3

    using iterator = __detail::_Node_iterator<value_type, // :3
					      __constant_iterators::value, // :3
					      __hash_cached::value>; // :3

    using const_iterator = __detail::_Node_const_iterator<value_type, // :3
						   __constant_iterators::value, // :3
						   __hash_cached::value>; // :3

    using local_iterator = __detail::_Local_iterator<key_type, value_type, // :3
						  _ExtractKey, _H1, _H2, _Hash, // :3
						  __constant_iterators::value, // :3
						     __hash_cached::value>; // :3

    using const_local_iterator = __detail::_Local_const_iterator<key_type, // :3
								 value_type, // :3
					_ExtractKey, _H1, _H2, _Hash, // :3
					__constant_iterators::value, // :3
					__hash_cached::value>; // :3

    using __ireturn_type = typename conditional<__unique_keys::value, // :3
						     pair<iterator, bool>, // :3
						     iterator>::type; // :3
  private: // :3
    using _EqualEBO = _Hashtable_ebo_helper<0, _Equal>; // :3
    using _EqualHelper =  _Equal_helper<_Key, _Value, _ExtractKey, _Equal, // :3
					__hash_code, __hash_cached::value>; // :3

  protected: // :3
    _Hashtable_base(const _ExtractKey& __ex, const _H1& __h1, const _H2& __h2, // :3
		    const _Hash& __hash, const _Equal& __eq) // :3
    : __hash_code_base(__ex, __h1, __h2, __hash), _EqualEBO(__eq) // :3
    { } // :3

    bool // :3
    _M_equals(const _Key& __k, __hash_code __c, __node_type* __n) const // :3
    { // :3
      return _EqualHelper::_S_equals(_M_eq(), this->_M_extract(), // :3
				     __k, __c, __n); // :3
    } // :3

    void // :3
    _M_swap(_Hashtable_base& __x) // :3
    { // :3
      __hash_code_base::_M_swap(__x); // :3
      swap(_M_eq(), __x._M_eq()); // :3
    } // :3

    const _Equal& // :3
    _M_eq() const { return _EqualEBO::_S_cget(*this); } // :3

    _Equal& // :3
    _M_eq() { return _EqualEBO::_S_get(*this); } // :3
  }; // :3

  /**
   *  struct _Equality_base.
   *
   *  Common types and functions for class _Equality.
   */ // :3
  struct _Equality_base // :3
  { // :3
  protected: // :3
    template<typename _Uiterator> // :3
      static bool // :3
      _S_is_permutation(_Uiterator, _Uiterator, _Uiterator); // :3
  }; // :3

  // See std::is_permutation in N3068. :3
  template<typename _Uiterator> // :3
    bool // :3
    _Equality_base:: // :3
    _S_is_permutation(_Uiterator __first1, _Uiterator __last1, // :3
		      _Uiterator __first2) // :3
    { // :3
      for (; __first1 != __last1; ++__first1, ++__first2) // :3
	if (!(*__first1 == *__first2)) // :3
	  break; // :3

      if (__first1 == __last1) // :3
	return true; // :3

      _Uiterator __last2 = __first2; // :3
      std::advance(__last2, std::distance(__first1, __last1)); // :3

      for (_Uiterator __it1 = __first1; __it1 != __last1; ++__it1) // :3
	{ // :3
	  _Uiterator __tmp =  __first1; // :3
	  while (__tmp != __it1 && !bool(*__tmp == *__it1)) // :3
	    ++__tmp; // :3

	  // We've seen this one before. :3
	  if (__tmp != __it1) // :3
	    continue; // :3

	  std::ptrdiff_t __n2 = 0; // :3
	  for (__tmp = __first2; __tmp != __last2; ++__tmp) // :3
	    if (*__tmp == *__it1) // :3
	      ++__n2; // :3

	  if (!__n2) // :3
	    return false; // :3

	  std::ptrdiff_t __n1 = 0; // :3
	  for (__tmp = __it1; __tmp != __last1; ++__tmp) // :3
	    if (*__tmp == *__it1) // :3
	      ++__n1; // :3

	  if (__n1 != __n2) // :3
	    return false; // :3
	} // :3
      return true; // :3
    } // :3

  /**
   *  Primary class template  _Equality.
   *
   *  This is for implementing equality comparison for unordered
   *  containers, per N3068, by John Lakos and Pablo Halpern.
   *  Algorithmically, we follow closely the reference implementations
   *  therein.
   */ // :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits, // :3
	   bool _Unique_keys = _Traits::__unique_keys::value> // :3
    struct _Equality; // :3

  /// Specialization. :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    struct _Equality<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
		     _H1, _H2, _Hash, _RehashPolicy, _Traits, true> // :3
    { // :3
      using __hashtable = _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
				     _H1, _H2, _Hash, _RehashPolicy, _Traits>; // :3

      bool // :3
      _M_equal(const __hashtable&) const; // :3
    }; // :3

  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    bool // :3
    _Equality<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	      _H1, _H2, _Hash, _RehashPolicy, _Traits, true>:: // :3
    _M_equal(const __hashtable& __other) const // :3
    { // :3
      const __hashtable* __this = static_cast<const __hashtable*>(this); // :3

      if (__this->size() != __other.size()) // :3
	return false; // :3

      for (auto __itx = __this->begin(); __itx != __this->end(); ++__itx) // :3
	{ // :3
	  const auto __ity = __other.find(_ExtractKey()(*__itx)); // :3
	  if (__ity == __other.end() || !bool(*__ity == *__itx)) // :3
	    return false; // :3
	} // :3
      return true; // :3
    } // :3

  /// Specialization. :3
  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    struct _Equality<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
		     _H1, _H2, _Hash, _RehashPolicy, _Traits, false> // :3
    : public _Equality_base // :3
    { // :3
      using __hashtable = _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
				     _H1, _H2, _Hash, _RehashPolicy, _Traits>; // :3

      bool // :3
      _M_equal(const __hashtable&) const; // :3
    }; // :3

  template<typename _Key, typename _Value, typename _Alloc, // :3
	   typename _ExtractKey, typename _Equal, // :3
	   typename _H1, typename _H2, typename _Hash, // :3
	   typename _RehashPolicy, typename _Traits> // :3
    bool // :3
    _Equality<_Key, _Value, _Alloc, _ExtractKey, _Equal, // :3
	      _H1, _H2, _Hash, _RehashPolicy, _Traits, false>:: // :3
    _M_equal(const __hashtable& __other) const // :3
    { // :3
      const __hashtable* __this = static_cast<const __hashtable*>(this); // :3

      if (__this->size() != __other.size()) // :3
	return false; // :3

      for (auto __itx = __this->begin(); __itx != __this->end();) // :3
	{ // :3
	  const auto __xrange = __this->equal_range(_ExtractKey()(*__itx)); // :3
	  const auto __yrange = __other.equal_range(_ExtractKey()(*__itx)); // :3

	  if (std::distance(__xrange.first, __xrange.second) // :3
	      != std::distance(__yrange.first, __yrange.second)) // :3
	    return false; // :3

	  if (!_S_is_permutation(__xrange.first, __xrange.second, // :3
				 __yrange.first)) // :3
	    return false; // :3

	  __itx = __xrange.second; // :3
	} // :3
      return true; // :3
    } // :3

  /**
   * This type deals with all allocation and keeps an allocator instance through
   * inheritance to benefit from EBO when possible.
   */ // :3
  template<typename _NodeAlloc> // :3
    struct _Hashtable_alloc : private _Hashtable_ebo_helper<0, _NodeAlloc> // :3
    { // :3
    private: // :3
      using __ebo_node_alloc = _Hashtable_ebo_helper<0, _NodeAlloc>; // :3
    public: // :3
      using __node_type = typename _NodeAlloc::value_type; // :3
      using __node_alloc_type = _NodeAlloc; // :3
      // Use __gnu_cxx to benefit from _S_always_equal and al. :3
      using __node_alloc_traits = __alloc_traits<__node_alloc_type>; // :3

      using __value_type = typename __node_type::value_type; // :3
      using __value_alloc_type = // :3
	typename __alloctr_rebind<__node_alloc_type, __value_type>::__type; // :3
      using __value_alloc_traits = allocator_traits<__value_alloc_type>; // :3

      using __node_base = __detail::_Hash_node_base; // :3
      using __bucket_type = __node_base*; // :3
      using __bucket_alloc_type = // :3
	typename __alloctr_rebind<__node_alloc_type, __bucket_type>::__type; // :3
      using __bucket_alloc_traits = allocator_traits<__bucket_alloc_type>; // :3

      _Hashtable_alloc(const _Hashtable_alloc&) = default; // :3
      _Hashtable_alloc(_Hashtable_alloc&&) = default; // :3

      template<typename _Alloc> // :3
	_Hashtable_alloc(_Alloc&& __a) // :3
	  : __ebo_node_alloc(std::forward<_Alloc>(__a)) // :3
	{ } // :3

      __node_alloc_type& // :3
      _M_node_allocator() // :3
      { return __ebo_node_alloc::_S_get(*this); } // :3

      const __node_alloc_type& // :3
      _M_node_allocator() const // :3
      { return __ebo_node_alloc::_S_cget(*this); } // :3

      template<typename... _Args> // :3
	__node_type* // :3
	_M_allocate_node(_Args&&... __args); // :3

      void // :3
      _M_deallocate_node(__node_type* __n); // :3

      // Deallocate the linked list of nodes pointed to by __n :3
      void // :3
      _M_deallocate_nodes(__node_type* __n); // :3

      __bucket_type* // :3
      _M_allocate_buckets(std::size_t __n); // :3

      void // :3
      _M_deallocate_buckets(__bucket_type*, std::size_t __n); // :3
    }; // :3

  // Definitions of class template _Hashtable_alloc's out-of-line member :3
  // functions. :3
  template<typename _NodeAlloc> // :3
    template<typename... _Args> // :3
      typename _Hashtable_alloc<_NodeAlloc>::__node_type* // :3
      _Hashtable_alloc<_NodeAlloc>::_M_allocate_node(_Args&&... __args) // :3
      { // :3
	auto __nptr = __node_alloc_traits::allocate(_M_node_allocator(), 1); // :3
	__node_type* __n = std::addressof(*__nptr); // :3
	__try // :3
	  { // :3
	    __value_alloc_type __a(_M_node_allocator()); // :3
	    ::new ((void*)__n) __node_type; // :3
	    __value_alloc_traits::construct(__a, __n->_M_valptr(), // :3
					    std::forward<_Args>(__args)...); // :3
	    return __n; // :3
	  } // :3
	__catch(...) // :3
	  { // :3
	    __node_alloc_traits::deallocate(_M_node_allocator(), __nptr, 1); // :3
	    __throw_exception_again; // :3
	  } // :3
      } // :3

  template<typename _NodeAlloc> // :3
    void // :3
    _Hashtable_alloc<_NodeAlloc>::_M_deallocate_node(__node_type* __n) // :3
    { // :3
      typedef typename __node_alloc_traits::pointer _Ptr; // :3
      auto __ptr = std::pointer_traits<_Ptr>::pointer_to(*__n); // :3
      __value_alloc_type __a(_M_node_allocator()); // :3
      __value_alloc_traits::destroy(__a, __n->_M_valptr()); // :3
      __n->~__node_type(); // :3
      __node_alloc_traits::deallocate(_M_node_allocator(), __ptr, 1); // :3
    } // :3

  template<typename _NodeAlloc> // :3
    void // :3
    _Hashtable_alloc<_NodeAlloc>::_M_deallocate_nodes(__node_type* __n) // :3
    { // :3
      while (__n) // :3
	{ // :3
	  __node_type* __tmp = __n; // :3
	  __n = __n->_M_next(); // :3
	  _M_deallocate_node(__tmp); // :3
	} // :3
    } // :3

  template<typename _NodeAlloc> // :3
    typename _Hashtable_alloc<_NodeAlloc>::__bucket_type* // :3
    _Hashtable_alloc<_NodeAlloc>::_M_allocate_buckets(std::size_t __n) // :3
    { // :3
      __bucket_alloc_type __alloc(_M_node_allocator()); // :3

      auto __ptr = __bucket_alloc_traits::allocate(__alloc, __n); // :3
      __bucket_type* __p = std::addressof(*__ptr); // :3
      __builtin_memset(__p, 0, __n * sizeof(__bucket_type)); // :3
      return __p; // :3
    } // :3

  template<typename _NodeAlloc> // :3
    void // :3
    _Hashtable_alloc<_NodeAlloc>::_M_deallocate_buckets(__bucket_type* __bkts, // :3
							std::size_t __n) // :3
    { // :3
      typedef typename __bucket_alloc_traits::pointer _Ptr; // :3
      auto __ptr = std::pointer_traits<_Ptr>::pointer_to(*__bkts); // :3
      __bucket_alloc_type __alloc(_M_node_allocator()); // :3
      __bucket_alloc_traits::deallocate(__alloc, __ptr, __n); // :3
    } // :3

 //@} hashtable-detail :3
_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace __detail :3
} // namespace std :3

// dank: addition :3

namespace geode::stl::__detail { // :3

  // The sentinel value is kept only for abi backward compatibility. :3
  inline const unsigned long __prime_list[] = // 256 + 1 or 256 + 48 + 1 :3
  { // :3
    2ul, 3ul, 5ul, 7ul, 11ul, 13ul, 17ul, 19ul, 23ul, 29ul, 31ul, // :3
    37ul, 41ul, 43ul, 47ul, 53ul, 59ul, 61ul, 67ul, 71ul, 73ul, 79ul, // :3
    83ul, 89ul, 97ul, 103ul, 109ul, 113ul, 127ul, 137ul, 139ul, 149ul, // :3
    157ul, 167ul, 179ul, 193ul, 199ul, 211ul, 227ul, 241ul, 257ul, // :3
    277ul, 293ul, 313ul, 337ul, 359ul, 383ul, 409ul, 439ul, 467ul, // :3
    503ul, 541ul, 577ul, 619ul, 661ul, 709ul, 761ul, 823ul, 887ul, // :3
    953ul, 1031ul, 1109ul, 1193ul, 1289ul, 1381ul, 1493ul, 1613ul, // :3
    1741ul, 1879ul, 2029ul, 2179ul, 2357ul, 2549ul, 2753ul, 2971ul, // :3
    3209ul, 3469ul, 3739ul, 4027ul, 4349ul, 4703ul, 5087ul, 5503ul, // :3
    5953ul, 6427ul, 6949ul, 7517ul, 8123ul, 8783ul, 9497ul, 10273ul, // :3
    11113ul, 12011ul, 12983ul, 14033ul, 15173ul, 16411ul, 17749ul, // :3
    19183ul, 20753ul, 22447ul, 24281ul, 26267ul, 28411ul, 30727ul, // :3
    33223ul, 35933ul, 38873ul, 42043ul, 45481ul, 49201ul, 53201ul, // :3
    57557ul, 62233ul, 67307ul, 72817ul, 78779ul, 85229ul, 92203ul, // :3
    99733ul, 107897ul, 116731ul, 126271ul, 136607ul, 147793ul, // :3
    159871ul, 172933ul, 187091ul, 202409ul, 218971ul, 236897ul, // :3
    256279ul, 277261ul, 299951ul, 324503ul, 351061ul, 379787ul, // :3
    410857ul, 444487ul, 480881ul, 520241ul, 562841ul, 608903ul, // :3
    658753ul, 712697ul, 771049ul, 834181ul, 902483ul, 976369ul, // :3
    1056323ul, 1142821ul, 1236397ul, 1337629ul, 1447153ul, 1565659ul, // :3
    1693859ul, 1832561ul, 1982627ul, 2144977ul, 2320627ul, 2510653ul, // :3
    2716249ul, 2938679ul, 3179303ul, 3439651ul, 3721303ul, 4026031ul, // :3
    4355707ul, 4712381ul, 5098259ul, 5515729ul, 5967347ul, 6456007ul, // :3
    6984629ul, 7556579ul, 8175383ul, 8844859ul, 9569143ul, 10352717ul, // :3
    11200489ul, 12117689ul, 13109983ul, 14183539ul, 15345007ul, // :3
    16601593ul, 17961079ul, 19431899ul, 21023161ul, 22744717ul, // :3
    24607243ul, 26622317ul, 28802401ul, 31160981ul, 33712729ul, // :3
    36473443ul, 39460231ul, 42691603ul, 46187573ul, 49969847ul, // :3
    54061849ul, 58488943ul, 63278561ul, 68460391ul, 74066549ul, // :3
    80131819ul, 86693767ul, 93793069ul, 101473717ul, 109783337ul, // :3
    118773397ul, 128499677ul, 139022417ul, 150406843ul, 162723577ul, // :3
    176048909ul, 190465427ul, 206062531ul, 222936881ul, 241193053ul, // :3
    260944219ul, 282312799ul, 305431229ul, 330442829ul, 357502601ul, // :3
    386778277ul, 418451333ul, 452718089ul, 489790921ul, 529899637ul, // :3
    573292817ul, 620239453ul, 671030513ul, 725980837ul, 785430967ul, // :3
    849749479ul, 919334987ul, 994618837ul, 1076067617ul, 1164186217ul, // :3
    1259520799ul, 1362662261ul, 1474249943ul, 1594975441ul, 1725587117ul, // :3
    1866894511ul, 2019773507ul, 2185171673ul, 2364114217ul, 2557710269ul, // :3
    2767159799ul, 2993761039ul, 3238918481ul, 3504151727ul, 3791104843ul, // :3
    4101556399ul, 4294967291ul, // :3
    // Sentinel, so we don't have to test the result of lower_bound, :3
    // or, on 64-bit machines, rest of the table. :3
#if __SIZEOF_LONG__ != 8 // :3
    4294967291ul // :3
#else // :3
    6442450933ul, 8589934583ul, 12884901857ul, 17179869143ul, // :3
    25769803693ul, 34359738337ul, 51539607367ul, 68719476731ul, // :3
    103079215087ul, 137438953447ul, 206158430123ul, 274877906899ul, // :3
    412316860387ul, 549755813881ul, 824633720731ul, 1099511627689ul, // :3
    1649267441579ul, 2199023255531ul, 3298534883309ul, 4398046511093ul, // :3
    6597069766607ul, 8796093022151ul, 13194139533241ul, 17592186044399ul, // :3
    26388279066581ul, 35184372088777ul, 52776558133177ul, 70368744177643ul, // :3
    105553116266399ul, 140737488355213ul, 211106232532861ul, 281474976710597ul, // :3
    562949953421231ul, 1125899906842597ul, 2251799813685119ul, // :3
    4503599627370449ul, 9007199254740881ul, 18014398509481951ul, // :3
    36028797018963913ul, 72057594037927931ul, 144115188075855859ul, // :3
    288230376151711717ul, 576460752303423433ul, // :3
    1152921504606846883ul, 2305843009213693951ul, // :3
    4611686018427387847ul, 9223372036854775783ul, // :3
    18446744073709551557ul, 18446744073709551557ul // :3
#endif // :3
  }; // :3

  // Return a prime no smaller than n. :3
  inline std::size_t // :3
  _Prime_rehash_policy::_M_next_bkt(std::size_t __n) const // :3
  { // :3
    // Optimize lookups involving the first elements of __prime_list. :3
    // (useful to speed-up, eg, constructors) :3
    static const unsigned char __fast_bkt[] // :3
      = { 2, 2, 2, 3, 5, 5, 7, 7, 11, 11, 11, 11, 13, 13 }; // :3

    if (__n < sizeof(__fast_bkt)) // :3
      { // :3
	if (__n == 0) // :3
	  // Special case on container 1st initialization with 0 bucket count :3
	  // hint. We keep _M_next_resize to 0 to make sure that next time we :3
	  // want to add an element allocation will take place. :3
	  return 1; // :3

	_M_next_resize = // :3
	  __builtin_floor(__fast_bkt[__n] * (double)_M_max_load_factor); // :3
	return __fast_bkt[__n]; // :3
      } // :3

    // Number of primes (without sentinel). :3
    constexpr auto __n_primes // :3
      = sizeof(__prime_list) / sizeof(unsigned long) - 1; // :3

    // Don't include the last prime in the search, so that anything :3
    // higher than the second-to-last prime returns a past-the-end :3
    // iterator that can be dereferenced to get the last prime. :3
    constexpr auto __last_prime = __prime_list + __n_primes - 1; // :3

    const unsigned long* __next_bkt = // :3
      lower_bound(__prime_list + 6, __last_prime, __n); // :3

    if (__next_bkt == __last_prime) // :3
      // Set next resize to the max value so that we never try to rehash again :3
      // as we already reach the biggest possible bucket number. :3
      // Note that it might result in max_load_factor not being respected. :3
      _M_next_resize = size_t(-1); // :3
    else // :3
      _M_next_resize = // :3
	__builtin_floor(*__next_bkt * (double)_M_max_load_factor); // :3

    return *__next_bkt; // :3
  } // :3

  inline pair<bool, std::size_t> // :3
  _Prime_rehash_policy:: // :3
  _M_need_rehash(std::size_t __n_bkt, std::size_t __n_elt, // :3
		 std::size_t __n_ins) const // :3
  { // :3
    if (__n_elt + __n_ins > _M_next_resize) // :3
      { // :3
	// If _M_next_resize is 0 it means that we have nothing allocated so :3
	// far and that we start inserting elements. In this case we start :3
	// with an initial bucket size of 11. :3
	double __min_bkts // :3
	  = max<std::size_t>(__n_elt + __n_ins, _M_next_resize ? 0 : 11) // :3
	  / (double)_M_max_load_factor; // :3
	if (__min_bkts >= __n_bkt) // :3
	  return { true, // :3
	    _M_next_bkt(max<std::size_t>(__builtin_floor(__min_bkts) + 1, // :3
					      __n_bkt * _S_growth_factor)) }; // :3

	_M_next_resize // :3
	  = __builtin_floor(__n_bkt * (double)_M_max_load_factor); // :3
	return { false, 0 }; // :3
      } // :3
    else // :3
      return { false, 0 }; // :3
  } // :3
} // :3