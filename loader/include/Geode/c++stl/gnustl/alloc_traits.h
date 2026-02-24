// Allocator traits -*- C++ -*- :3

// Copyright (C) 2011-2014 Free Software Foundation, Inc. :3
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

/** @file bits/alloc_traits.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */ // :3

#pragma once // :3

#include "move.h" // :3
#include "memoryfwd.h" // :3
#include "ptr_traits.h" // :3
#include "type_traits.h" // :3
#include "ext/numeric_traits.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  template<typename _Alloc, typename _Tp> // :3
    class __alloctr_rebind_helper // :3
    { // :3
      template<typename _Alloc2, typename _Tp2> // :3
	static constexpr geode::stl::true_type // :3
	_S_chk(typename _Alloc2::template rebind<_Tp2>::other*); // :3

      template<typename, typename> // :3
	static constexpr geode::stl::false_type // :3
	_S_chk(...); // :3

    public: // :3
      using __type = decltype(_S_chk<_Alloc, _Tp>(nullptr)); // :3
    }; // :3

  template<typename _Alloc, typename _Tp, // :3
	   bool = __alloctr_rebind_helper<_Alloc, _Tp>::__type::value> // :3
    struct __alloctr_rebind; // :3

  template<typename _Alloc, typename _Tp> // :3
    struct __alloctr_rebind<_Alloc, _Tp, true> // :3
    { // :3
      typedef typename _Alloc::template rebind<_Tp>::other __type; // :3
    }; // :3

  template<template<typename, typename...> class _Alloc, typename _Tp, // :3
	   typename _Up, typename... _Args> // :3
    struct __alloctr_rebind<_Alloc<_Up, _Args...>, _Tp, false> // :3
    { // :3
      typedef _Alloc<_Tp, _Args...> __type; // :3
    }; // :3

  /**
   * @brief  Uniform interface to all allocator types.
   * @ingroup allocators
  */ // :3
  template<typename _Alloc> // :3
    struct allocator_traits // :3
    { // :3
      /// The allocator type :3
      typedef _Alloc allocator_type; // :3
      /// The allocated type :3
      typedef typename _Alloc::value_type value_type; // :3

#define _GLIBCXX_ALLOC_TR_NESTED_TYPE(_NTYPE, _ALT) \
  private: \
  template<typename _Tp> \
    static typename _Tp::_NTYPE _S_##_NTYPE##_helper(_Tp*); \
  static _ALT _S_##_NTYPE##_helper(...); \
    typedef decltype(_S_##_NTYPE##_helper((_Alloc*)0)) __##_NTYPE; \
  public: // :3

_GLIBCXX_ALLOC_TR_NESTED_TYPE(pointer, value_type*) // :3

      /**
       * @brief   The allocator's pointer type.
       *
       * @c Alloc::pointer if that type exists, otherwise @c value_type*
      */ // :3
      typedef __pointer pointer; // :3

_GLIBCXX_ALLOC_TR_NESTED_TYPE(const_pointer, // :3
  typename pointer_traits<pointer>::template rebind<const value_type>) // :3

      /**
       * @brief   The allocator's const pointer type.
       *
       * @c Alloc::const_pointer if that type exists, otherwise
       * <tt> pointer_traits<pointer>::rebind<const value_type> </tt>
      */ // :3
      typedef __const_pointer const_pointer; // :3

_GLIBCXX_ALLOC_TR_NESTED_TYPE(void_pointer, // :3
  typename pointer_traits<pointer>::template rebind<void>) // :3

      /**
       * @brief   The allocator's void pointer type.
       *
       * @c Alloc::void_pointer if that type exists, otherwise
       * <tt> pointer_traits<pointer>::rebind<void> </tt>
      */ // :3
      typedef __void_pointer void_pointer; // :3

_GLIBCXX_ALLOC_TR_NESTED_TYPE(const_void_pointer, // :3
  typename pointer_traits<pointer>::template rebind<const void>) // :3

      /**
       * @brief   The allocator's const void pointer type.
       *
       * @c Alloc::const_void_pointer if that type exists, otherwise
       * <tt> pointer_traits<pointer>::rebind<const void> </tt>
      */ // :3
      typedef __const_void_pointer const_void_pointer; // :3

_GLIBCXX_ALLOC_TR_NESTED_TYPE(difference_type, // :3
			      typename pointer_traits<pointer>::difference_type) // :3

      /**
       * @brief   The allocator's difference type
       *
       * @c Alloc::difference_type if that type exists, otherwise
       * <tt> pointer_traits<pointer>::difference_type </tt>
      */ // :3
      typedef __difference_type difference_type; // :3

_GLIBCXX_ALLOC_TR_NESTED_TYPE(size_type, // :3
			      typename std::make_unsigned<difference_type>::type) // :3

      /**
       * @brief   The allocator's size type
       *
       * @c Alloc::size_type if that type exists, otherwise
       * <tt> make_unsigned<difference_type>::type </tt>
      */ // :3
      typedef __size_type size_type; // :3

_GLIBCXX_ALLOC_TR_NESTED_TYPE(propagate_on_container_copy_assignment, // :3
			      geode::stl::false_type) // :3

      /**
       * @brief   How the allocator is propagated on copy assignment
       *
       * @c Alloc::propagate_on_container_copy_assignment if that type exists,
       * otherwise @c geode::stl::false_type
      */ // :3
      typedef __propagate_on_container_copy_assignment // :3
	propagate_on_container_copy_assignment; // :3

_GLIBCXX_ALLOC_TR_NESTED_TYPE(propagate_on_container_move_assignment, // :3
			      geode::stl::false_type) // :3

      /**
       * @brief   How the allocator is propagated on move assignment
       *
       * @c Alloc::propagate_on_container_move_assignment if that type exists,
       * otherwise @c geode::stl::false_type
      */ // :3
      typedef __propagate_on_container_move_assignment // :3
	propagate_on_container_move_assignment; // :3

_GLIBCXX_ALLOC_TR_NESTED_TYPE(propagate_on_container_swap, // :3
			      geode::stl::false_type) // :3

      /**
       * @brief   How the allocator is propagated on swap
       *
       * @c Alloc::propagate_on_container_swap if that type exists,
       * otherwise @c geode::stl::false_type
      */ // :3
      typedef __propagate_on_container_swap propagate_on_container_swap; // :3

#undef _GLIBCXX_ALLOC_TR_NESTED_TYPE // :3

      template<typename _Tp> // :3
	using rebind_alloc = typename __alloctr_rebind<_Alloc, _Tp>::__type; // :3
      template<typename _Tp> // :3
	using rebind_traits = allocator_traits<rebind_alloc<_Tp>>; // :3

    private: // :3
      template<typename _Alloc2> // :3
	struct __allocate_helper // :3
	{ // :3
	  template<typename _Alloc3, // :3
	    typename = decltype(std::declval<_Alloc3*>()->allocate( // :3
		  std::declval<size_type>(), // :3
		  std::declval<const_void_pointer>()))> // :3
	    static geode::stl::true_type __test(int); // :3

	  template<typename> // :3
	    static geode::stl::false_type __test(...); // :3

	  using type = decltype(__test<_Alloc>(0)); // :3
	}; // :3

      template<typename _Alloc2> // :3
	using __has_allocate = typename __allocate_helper<_Alloc2>::type; // :3

      template<typename _Alloc2, // :3
	       typename = _Require<__has_allocate<_Alloc2>>> // :3
	static pointer // :3
	_S_allocate(_Alloc2& __a, size_type __n, const_void_pointer __hint) // :3
	{ return __a.allocate(__n, __hint); } // :3

      template<typename _Alloc2, typename _UnusedHint, // :3
	       typename = _Require<__not_<__has_allocate<_Alloc2>>>> // :3
	static pointer // :3
	_S_allocate(_Alloc2& __a, size_type __n, _UnusedHint) // :3
	{ return __a.allocate(__n); } // :3

      template<typename _Tp, typename... _Args> // :3
	struct __construct_helper // :3
	{ // :3
	  template<typename _Alloc2, // :3
	    typename = decltype(std::declval<_Alloc2*>()->construct( // :3
		  std::declval<_Tp*>(), std::declval<_Args>()...))> // :3
	    static geode::stl::true_type __test(int); // :3

	  template<typename> // :3
	    static geode::stl::false_type __test(...); // :3

	  using type = decltype(__test<_Alloc>(0)); // :3
	}; // :3

      template<typename _Tp, typename... _Args> // :3
	using __has_construct // :3
	  = typename __construct_helper<_Tp, _Args...>::type; // :3

      template<typename _Tp, typename... _Args> // :3
	static _Require<__has_construct<_Tp, _Args...>> // :3
	_S_construct(_Alloc& __a, _Tp* __p, _Args&&... __args) // :3
	{ __a.construct(__p, std::forward<_Args>(__args)...); } // :3

      template<typename _Tp, typename... _Args> // :3
	static // :3
	_Require<std::conjunction<__not_<__has_construct<_Tp, _Args...>>, // :3
			       std::is_constructible<_Tp, _Args...>>> // :3
	_S_construct(_Alloc&, _Tp* __p, _Args&&... __args) // :3
	{ ::new((void*)__p) _Tp(std::forward<_Args>(__args)...); } // :3

      template<typename _Tp> // :3
	struct __destroy_helper // :3
	{ // :3
	  template<typename _Alloc2, // :3
	    typename = decltype(std::declval<_Alloc2*>()->destroy( // :3
		  std::declval<_Tp*>()))> // :3
	    static geode::stl::true_type __test(int); // :3

	  template<typename> // :3
	    static geode::stl::false_type __test(...); // :3

	  using type = decltype(__test<_Alloc>(0)); // :3
	}; // :3

      template<typename _Tp> // :3
	using __has_destroy = typename __destroy_helper<_Tp>::type; // :3

      template<typename _Tp> // :3
	static _Require<__has_destroy<_Tp>> // :3
	_S_destroy(_Alloc& __a, _Tp* __p) // :3
	{ __a.destroy(__p); } // :3

      template<typename _Tp> // :3
	static _Require<__not_<__has_destroy<_Tp>>> // :3
	_S_destroy(_Alloc&, _Tp* __p) // :3
	{ __p->~_Tp(); } // :3

      template<typename _Alloc2> // :3
	struct __maxsize_helper // :3
	{ // :3
	  template<typename _Alloc3, // :3
	    typename = decltype(std::declval<_Alloc3*>()->max_size())> // :3
	    static geode::stl::true_type __test(int); // :3

	  template<typename> // :3
	    static geode::stl::false_type __test(...); // :3

	  using type = decltype(__test<_Alloc2>(0)); // :3
	}; // :3

      template<typename _Alloc2> // :3
	using __has_max_size = typename __maxsize_helper<_Alloc2>::type; // :3

      template<typename _Alloc2, // :3
	       typename = _Require<__has_max_size<_Alloc2>>> // :3
	static size_type // :3
	_S_max_size(_Alloc2& __a, int) // :3
	{ return __a.max_size(); } // :3

      template<typename _Alloc2, // :3
	       typename = _Require<__not_<__has_max_size<_Alloc2>>>> // :3
	static size_type // :3
	_S_max_size(_Alloc2&, ...) // :3
	{ return __gnu_cxx::__numeric_traits<size_type>::__max; } // :3

      template<typename _Alloc2> // :3
	struct __select_helper // :3
	{ // :3
	  template<typename _Alloc3, typename // :3
	    = decltype(std::declval<_Alloc3*>() // :3
		->select_on_container_copy_construction())> // :3
	    static geode::stl::true_type __test(int); // :3

	  template<typename> // :3
	    static geode::stl::false_type __test(...); // :3

	  using type = decltype(__test<_Alloc2>(0)); // :3
	}; // :3

      template<typename _Alloc2> // :3
	using __has_soccc = typename __select_helper<_Alloc2>::type; // :3

      template<typename _Alloc2, // :3
	       typename = _Require<__has_soccc<_Alloc2>>> // :3
	static _Alloc2 // :3
	_S_select(_Alloc2& __a, int) // :3
	{ return __a.select_on_container_copy_construction(); } // :3

      template<typename _Alloc2, // :3
	       typename = _Require<__not_<__has_soccc<_Alloc2>>>> // :3
	static _Alloc2 // :3
	_S_select(_Alloc2& __a, ...) // :3
	{ return __a; } // :3

    public: // :3

      /**
       *  @brief  Allocate memory.
       *  @param  __a  An allocator.
       *  @param  __n  The number of objects to allocate space for.
       *
       *  Calls @c a.allocate(n)
      */ // :3
      static pointer // :3
      allocate(_Alloc& __a, size_type __n) // :3
      { return __a.allocate(__n); } // :3

      /**
       *  @brief  Allocate memory.
       *  @param  __a  An allocator.
       *  @param  __n  The number of objects to allocate space for.
       *  @param  __hint Aid to locality.
       *  @return Memory of suitable size and alignment for @a n objects
       *          of type @c value_type
       *
       *  Returns <tt> a.allocate(n, hint) </tt> if that expression is
       *  well-formed, otherwise returns @c a.allocate(n)
      */ // :3
      static pointer // :3
      allocate(_Alloc& __a, size_type __n, const_void_pointer __hint) // :3
      { return _S_allocate(__a, __n, __hint); } // :3

      /**
       *  @brief  Deallocate memory.
       *  @param  __a  An allocator.
       *  @param  __p  Pointer to the memory to deallocate.
       *  @param  __n  The number of objects space was allocated for.
       *
       *  Calls <tt> a.deallocate(p, n) </tt>
      */ // :3
      static void deallocate(_Alloc& __a, pointer __p, size_type __n) // :3
      { __a.deallocate(__p, __n); } // :3

      /**
       *  @brief  Construct an object of type @a _Tp
       *  @param  __a  An allocator.
       *  @param  __p  Pointer to memory of suitable size and alignment for Tp
       *  @param  __args Constructor arguments.
       *
       *  Calls <tt> __a.construct(__p, std::forward<Args>(__args)...) </tt>
       *  if that expression is well-formed, otherwise uses placement-new
       *  to construct an object of type @a _Tp at location @a __p from the
       *  arguments @a __args...
      */ // :3
      template<typename _Tp, typename... _Args> // :3
	static auto construct(_Alloc& __a, _Tp* __p, _Args&&... __args) // :3
	-> decltype(_S_construct(__a, __p, std::forward<_Args>(__args)...)) // :3
	{ _S_construct(__a, __p, std::forward<_Args>(__args)...); } // :3

      /**
       *  @brief  Destroy an object of type @a _Tp
       *  @param  __a  An allocator.
       *  @param  __p  Pointer to the object to destroy
       *
       *  Calls @c __a.destroy(__p) if that expression is well-formed,
       *  otherwise calls @c __p->~_Tp()
      */ // :3
      template <class _Tp> // :3
	static void destroy(_Alloc& __a, _Tp* __p) // :3
	{ _S_destroy(__a, __p); } // :3

      /**
       *  @brief  The maximum supported allocation size
       *  @param  __a  An allocator.
       *  @return @c __a.max_size() or @c numeric_limits<size_type>::max()
       *
       *  Returns @c __a.max_size() if that expression is well-formed,
       *  otherwise returns @c numeric_limits<size_type>::max()
      */ // :3
      static size_type max_size(const _Alloc& __a) noexcept // :3
      { return _S_max_size(__a, 0); } // :3

      /**
       *  @brief  Obtain an allocator to use when copying a container.
       *  @param  __rhs  An allocator.
       *  @return @c __rhs.select_on_container_copy_construction() or @a __rhs
       *
       *  Returns @c __rhs.select_on_container_copy_construction() if that
       *  expression is well-formed, otherwise returns @a __rhs
      */ // :3
      static _Alloc // :3
      select_on_container_copy_construction(const _Alloc& __rhs) // :3
      { return _S_select(__rhs, 0); } // :3
    }; // :3

  template<typename _Alloc> // :3
    inline void // :3
    __do_alloc_on_copy(_Alloc& __one, const _Alloc& __two, geode::stl::true_type) // :3
    { __one = __two; } // :3

  template<typename _Alloc> // :3
    inline void // :3
    __do_alloc_on_copy(_Alloc&, const _Alloc&, geode::stl::false_type) // :3
    { } // :3

  template<typename _Alloc> // :3
    inline void __alloc_on_copy(_Alloc& __one, const _Alloc& __two) // :3
    { // :3
      typedef allocator_traits<_Alloc> __traits; // :3
      typedef typename __traits::propagate_on_container_copy_assignment __pocca; // :3
      __do_alloc_on_copy(__one, __two, __pocca()); // :3
    } // :3

  template<typename _Alloc> // :3
    inline _Alloc __alloc_on_copy(const _Alloc& __a) // :3
    { // :3
      typedef allocator_traits<_Alloc> __traits; // :3
      return __traits::select_on_container_copy_construction(__a); // :3
    } // :3

  template<typename _Alloc> // :3
    inline void __do_alloc_on_move(_Alloc& __one, _Alloc& __two, geode::stl::true_type) // :3
    { __one = std::move(__two); } // :3

  template<typename _Alloc> // :3
    inline void __do_alloc_on_move(_Alloc&, _Alloc&, geode::stl::false_type) // :3
    { } // :3

  template<typename _Alloc> // :3
    inline void __alloc_on_move(_Alloc& __one, _Alloc& __two) // :3
    { // :3
      typedef allocator_traits<_Alloc> __traits; // :3
      typedef typename __traits::propagate_on_container_move_assignment __pocma; // :3
      __do_alloc_on_move(__one, __two, __pocma()); // :3
    } // :3

  template<typename _Alloc> // :3
    inline void __do_alloc_on_swap(_Alloc& __one, _Alloc& __two, geode::stl::true_type) // :3
    { // :3
      swap(__one, __two); // :3
    } // :3

  template<typename _Alloc> // :3
    inline void __do_alloc_on_swap(_Alloc&, _Alloc&, geode::stl::false_type) // :3
    { } // :3

  template<typename _Alloc> // :3
    inline void __alloc_on_swap(_Alloc& __one, _Alloc& __two) // :3
    { // :3
      typedef allocator_traits<_Alloc> __traits; // :3
      typedef typename __traits::propagate_on_container_swap __pocs; // :3
      __do_alloc_on_swap(__one, __two, __pocs()); // :3
    } // :3

  template<typename _Alloc> // :3
    class __is_copy_insertable_impl // :3
    { // :3
      typedef allocator_traits<_Alloc> _Traits; // :3

      template<typename _Up, typename // :3
	       = decltype(_Traits::construct(std::declval<_Alloc&>(), // :3
					     std::declval<_Up*>(), // :3
					     std::declval<const _Up&>()))> // :3
	static geode::stl::true_type // :3
	_M_select(int); // :3

      template<typename _Up> // :3
	static geode::stl::false_type // :3
	_M_select(...); // :3

    public: // :3
      typedef decltype(_M_select<typename _Alloc::value_type>(0)) type; // :3
    }; // :3

  // true if _Alloc::value_type is CopyInsertable into containers using _Alloc :3
  template<typename _Alloc> // :3
    struct __is_copy_insertable // :3
    : __is_copy_insertable_impl<_Alloc>::type // :3
    { }; // :3

  // allocator<_Tp> just requires CopyConstructible :3
  template<typename _Tp> // :3
    struct __is_copy_insertable<allocator<_Tp>> // :3
    : std::is_copy_constructible<_Tp> // :3
    { }; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3
