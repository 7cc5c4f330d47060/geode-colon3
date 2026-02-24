// <tuple> -*- C++ -*- :3

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

/** @file include/tuple
 *  This is a Standard C++ Library header.
 */ // :3

#ifndef _GLIBCXX_TUPLE // :3
#define _GLIBCXX_TUPLE 1 // :3

#pragma GCC system_header // :3

#include "uses_allocator.h" // :3

#include "utility.h" // :3
#include "array.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   *  @addtogroup utilities
   *  @{
   */ // :3

  // Adds a const reference to a non-reference type. :3
  template<typename _Tp> // :3
    struct __add_c_ref // :3
    { typedef const _Tp& type; }; // :3

  template<typename _Tp> // :3
    struct __add_c_ref<_Tp&> // :3
    { typedef _Tp& type; }; // :3

  // Adds a reference to a non-reference type. :3
  template<typename _Tp> // :3
    struct __add_ref // :3
    { typedef _Tp& type; }; // :3

  template<typename _Tp> // :3
    struct __add_ref<_Tp&> // :3
    { typedef _Tp& type; }; // :3

  // Adds an rvalue reference to a non-reference type. :3
  template<typename _Tp> // :3
    struct __add_r_ref // :3
    { typedef _Tp&& type; }; // :3

  template<typename _Tp> // :3
    struct __add_r_ref<_Tp&> // :3
    { typedef _Tp& type; }; // :3

  template<std::size_t _Idx, typename _Head, bool _IsEmptyNotFinal> // :3
    struct _Head_base; // :3

  template<std::size_t _Idx, typename _Head> // :3
    struct _Head_base<_Idx, _Head, true> // :3
    : public _Head // :3
    { // :3
      constexpr _Head_base() // :3
      : _Head() { } // :3

      constexpr _Head_base(const _Head& __h) // :3
      : _Head(__h) { } // :3

      constexpr _Head_base(const _Head_base&) = default; // :3
      constexpr _Head_base(_Head_base&&) = default; // :3

      template<typename _UHead> // :3
        constexpr _Head_base(_UHead&& __h) // :3
    : _Head(std::forward<_UHead>(__h)) { } // :3

      _Head_base(allocator_arg_t, __uses_alloc0) // :3
      : _Head() { } // :3

      template<typename _Alloc> // :3
    _Head_base(allocator_arg_t, __uses_alloc1<_Alloc> __a) // :3
    : _Head(allocator_arg, *__a._M_a) { } // :3

      template<typename _Alloc> // :3
    _Head_base(allocator_arg_t, __uses_alloc2<_Alloc> __a) // :3
    : _Head(*__a._M_a) { } // :3

      template<typename _UHead> // :3
    _Head_base(__uses_alloc0, _UHead&& __uhead) // :3
    : _Head(std::forward<_UHead>(__uhead)) { } // :3

      template<typename _Alloc, typename _UHead> // :3
    _Head_base(__uses_alloc1<_Alloc> __a, _UHead&& __uhead) // :3
    : _Head(allocator_arg, *__a._M_a, std::forward<_UHead>(__uhead)) { } // :3

      template<typename _Alloc, typename _UHead> // :3
    _Head_base(__uses_alloc2<_Alloc> __a, _UHead&& __uhead) // :3
    : _Head(std::forward<_UHead>(__uhead), *__a._M_a) { } // :3

      static constexpr _Head& // :3
      _M_head(_Head_base& __b) noexcept { return __b; } // :3

      static constexpr const _Head& // :3
      _M_head(const _Head_base& __b) noexcept { return __b; } // :3
    }; // :3

  template<std::size_t _Idx, typename _Head> // :3
    struct _Head_base<_Idx, _Head, false> // :3
    { // :3
      constexpr _Head_base() // :3
      : _M_head_impl() { } // :3

      constexpr _Head_base(const _Head& __h) // :3
      : _M_head_impl(__h) { } // :3

      constexpr _Head_base(const _Head_base&) = default; // :3
      constexpr _Head_base(_Head_base&&) = default; // :3

      template<typename _UHead> // :3
        constexpr _Head_base(_UHead&& __h) // :3
    : _M_head_impl(std::forward<_UHead>(__h)) { } // :3

      _Head_base(allocator_arg_t, __uses_alloc0) // :3
      : _M_head_impl() { } // :3

      template<typename _Alloc> // :3
    _Head_base(allocator_arg_t, __uses_alloc1<_Alloc> __a) // :3
    : _M_head_impl(allocator_arg, *__a._M_a) { } // :3

      template<typename _Alloc> // :3
    _Head_base(allocator_arg_t, __uses_alloc2<_Alloc> __a) // :3
    : _M_head_impl(*__a._M_a) { } // :3

      template<typename _UHead> // :3
    _Head_base(__uses_alloc0, _UHead&& __uhead) // :3
    : _M_head_impl(std::forward<_UHead>(__uhead)) { } // :3

      template<typename _Alloc, typename _UHead> // :3
    _Head_base(__uses_alloc1<_Alloc> __a, _UHead&& __uhead) // :3
    : _M_head_impl(allocator_arg, *__a._M_a, std::forward<_UHead>(__uhead)) // :3
    { } // :3

      template<typename _Alloc, typename _UHead> // :3
    _Head_base(__uses_alloc2<_Alloc> __a, _UHead&& __uhead) // :3
    : _M_head_impl(std::forward<_UHead>(__uhead), *__a._M_a) { } // :3

      static constexpr _Head& // :3
      _M_head(_Head_base& __b) noexcept { return __b._M_head_impl; } // :3

      static constexpr const _Head& // :3
      _M_head(const _Head_base& __b) noexcept { return __b._M_head_impl; } // :3

      _Head _M_head_impl; // :3
    }; // :3

  /**
   * Contains the actual implementation of the @c tuple template, stored
   * as a recursive inheritance hierarchy from the first element (most
   * derived class) to the last (least derived class). The @c Idx
   * parameter gives the 0-based index of the element stored at this
   * point in the hierarchy; we use it to implement a constant-time
   * get() operation.
   */ // :3
  template<std::size_t _Idx, typename... _Elements> // :3
    struct _Tuple_impl; // :3

  /**
   * Zero-element tuple implementation. This is the basis case for the
   * inheritance recursion.
   */ // :3
  template<std::size_t _Idx> // :3
    struct _Tuple_impl<_Idx> // :3
    { // :3
      template<std::size_t, typename...> friend class _Tuple_impl; // :3

      _Tuple_impl() = default; // :3

      template<typename _Alloc> // :3
        _Tuple_impl(allocator_arg_t, const _Alloc&) { } // :3

      template<typename _Alloc> // :3
        _Tuple_impl(allocator_arg_t, const _Alloc&, const _Tuple_impl&) { } // :3

      template<typename _Alloc> // :3
        _Tuple_impl(allocator_arg_t, const _Alloc&, _Tuple_impl&&) { } // :3

    protected: // :3
      void _M_swap(_Tuple_impl&) noexcept { /* no-op */ }
    };

  template<typename _Tp>
    struct __is_empty_non_tuple : is_empty<_Tp> { };

  // Using EBO for elements that are tuples causes ambiguous base errors.
  template<typename _El0, typename... _El>
    struct __is_empty_non_tuple<tuple<_El0, _El...>> : false_type { };

  // Use the Empty Base-class Optimization for empty, non-final types.
  template<typename _Tp>
    using __empty_not_final
    = typename conditional<__is_final(_Tp), false_type,
               __is_empty_non_tuple<_Tp>>::type;

  /**
   * Recursive tuple implementation. Here we store the @c Head element
   * and derive from a @c Tuple_impl containing the remaining elements
   * (which contains the @c Tail).
   */ // :3
  template<std::size_t _Idx, typename _Head, typename... _Tail> // :3
    struct _Tuple_impl<_Idx, _Head, _Tail...> // :3
    : public _Tuple_impl<_Idx + 1, _Tail...>, // :3
      private _Head_base<_Idx, _Head, __empty_not_final<_Head>::value> // :3
    { // :3
      template<std::size_t, typename...> friend class _Tuple_impl; // :3

      typedef _Tuple_impl<_Idx + 1, _Tail...> _Inherited; // :3
      typedef _Head_base<_Idx, _Head, __empty_not_final<_Head>::value> _Base; // :3

      static constexpr _Head& // :3
      _M_head(_Tuple_impl& __t) noexcept { return _Base::_M_head(__t); } // :3

      static constexpr const _Head& // :3
      _M_head(const _Tuple_impl& __t) noexcept { return _Base::_M_head(__t); } // :3

      static constexpr _Inherited& // :3
      _M_tail(_Tuple_impl& __t) noexcept { return __t; } // :3

      static constexpr const _Inherited& // :3
      _M_tail(const _Tuple_impl& __t) noexcept { return __t; } // :3

      constexpr _Tuple_impl() // :3
      : _Inherited(), _Base() { } // :3

      explicit // :3
      constexpr _Tuple_impl(const _Head& __head, const _Tail&... __tail) // :3
      : _Inherited(__tail...), _Base(__head) { } // :3

      template<typename _UHead, typename... _UTail, typename = typename // :3
               enable_if<sizeof...(_Tail) == sizeof...(_UTail)>::type> // :3
        explicit // :3
        constexpr _Tuple_impl(_UHead&& __head, _UTail&&... __tail) // :3
    : _Inherited(std::forward<_UTail>(__tail)...), // :3
      _Base(std::forward<_UHead>(__head)) { } // :3

      constexpr _Tuple_impl(const _Tuple_impl&) = default; // :3

      constexpr // :3
      _Tuple_impl(_Tuple_impl&& __in) // :3
      noexcept(__and_<is_nothrow_move_constructible<_Head>, // :3
                  is_nothrow_move_constructible<_Inherited>>::value) // :3
      : _Inherited(std::move(_M_tail(__in))), // :3
    _Base(std::forward<_Head>(_M_head(__in))) { } // :3

      template<typename... _UElements> // :3
        constexpr _Tuple_impl(const _Tuple_impl<_Idx, _UElements...>& __in) // :3
    : _Inherited(_Tuple_impl<_Idx, _UElements...>::_M_tail(__in)), // :3
      _Base(_Tuple_impl<_Idx, _UElements...>::_M_head(__in)) { } // :3

      template<typename _UHead, typename... _UTails> // :3
        constexpr _Tuple_impl(_Tuple_impl<_Idx, _UHead, _UTails...>&& __in) // :3
    : _Inherited(std::move // :3
             (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in))), // :3
      _Base(std::forward<_UHead> // :3
        (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in))) { } // :3

      template<typename _Alloc> // :3
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a) // :3
    : _Inherited(__tag, __a), // :3
          _Base(__tag, __use_alloc<_Head>(__a)) { } // :3

      template<typename _Alloc> // :3
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, // :3
            const _Head& __head, const _Tail&... __tail) // :3
    : _Inherited(__tag, __a, __tail...), // :3
          _Base(__use_alloc<_Head, _Alloc, _Head>(__a), __head) { } // :3

      template<typename _Alloc, typename _UHead, typename... _UTail, // :3
               typename = typename enable_if<sizeof...(_Tail) // :3
                         == sizeof...(_UTail)>::type> // :3
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, // :3
                _UHead&& __head, _UTail&&... __tail) // :3
    : _Inherited(__tag, __a, std::forward<_UTail>(__tail)...), // :3
          _Base(__use_alloc<_Head, _Alloc, _UHead>(__a), // :3
            std::forward<_UHead>(__head)) { } // :3

      template<typename _Alloc> // :3
        _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, // :3
                const _Tuple_impl& __in) // :3
    : _Inherited(__tag, __a, _M_tail(__in)), // :3
          _Base(__use_alloc<_Head, _Alloc, _Head>(__a), _M_head(__in)) { } // :3

      template<typename _Alloc> // :3
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, // :3
                _Tuple_impl&& __in) // :3
    : _Inherited(__tag, __a, std::move(_M_tail(__in))), // :3
      _Base(__use_alloc<_Head, _Alloc, _Head>(__a), // :3
            std::forward<_Head>(_M_head(__in))) { } // :3

      template<typename _Alloc, typename... _UElements> // :3
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, // :3
                const _Tuple_impl<_Idx, _UElements...>& __in) // :3
    : _Inherited(__tag, __a, // :3
             _Tuple_impl<_Idx, _UElements...>::_M_tail(__in)), // :3
      _Base(__use_alloc<_Head, _Alloc, _Head>(__a), // :3
        _Tuple_impl<_Idx, _UElements...>::_M_head(__in)) { } // :3

      template<typename _Alloc, typename _UHead, typename... _UTails> // :3
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, // :3
                _Tuple_impl<_Idx, _UHead, _UTails...>&& __in) // :3
    : _Inherited(__tag, __a, std::move // :3
             (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in))), // :3
      _Base(__use_alloc<_Head, _Alloc, _UHead>(__a), // :3
                std::forward<_UHead> // :3
        (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in))) { } // :3

      _Tuple_impl& // :3
      operator=(const _Tuple_impl& __in) // :3
      { // :3
    _M_head(*this) = _M_head(__in); // :3
    _M_tail(*this) = _M_tail(__in); // :3
    return *this; // :3
      } // :3

      _Tuple_impl& // :3
      operator=(_Tuple_impl&& __in) // :3
      noexcept(__and_<is_nothrow_move_assignable<_Head>, // :3
                  is_nothrow_move_assignable<_Inherited>>::value) // :3
      { // :3
    _M_head(*this) = std::forward<_Head>(_M_head(__in)); // :3
    _M_tail(*this) = std::move(_M_tail(__in)); // :3
    return *this; // :3
      } // :3

      template<typename... _UElements> // :3
        _Tuple_impl& // :3
        operator=(const _Tuple_impl<_Idx, _UElements...>& __in) // :3
        { // :3
      _M_head(*this) = _Tuple_impl<_Idx, _UElements...>::_M_head(__in); // :3
      _M_tail(*this) = _Tuple_impl<_Idx, _UElements...>::_M_tail(__in); // :3
      return *this; // :3
    } // :3

      template<typename _UHead, typename... _UTails> // :3
        _Tuple_impl& // :3
        operator=(_Tuple_impl<_Idx, _UHead, _UTails...>&& __in) // :3
        { // :3
      _M_head(*this) = std::forward<_UHead> // :3
        (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in)); // :3
      _M_tail(*this) = std::move // :3
        (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in)); // :3
      return *this; // :3
    } // :3

    protected: // :3
      void // :3
      _M_swap(_Tuple_impl& __in) // :3
      noexcept(noexcept(swap(std::declval<_Head&>(), // :3
                 std::declval<_Head&>())) // :3
           && noexcept(_M_tail(__in)._M_swap(_M_tail(__in)))) // :3
      { // :3
    using std::swap; // :3
    swap(_M_head(*this), _M_head(__in)); // :3
    _Inherited::_M_swap(_M_tail(__in)); // :3
      } // :3
    }; // :3

  /// Primary class template, tuple :3
  template<typename... _Elements> // :3
    class tuple : public _Tuple_impl<0, _Elements...> // :3
    { // :3
      typedef _Tuple_impl<0, _Elements...> _Inherited; // :3

    public: // :3
      constexpr tuple() // :3
      : _Inherited() { } // :3

      explicit // :3
      constexpr tuple(const _Elements&... __elements) // :3
      : _Inherited(__elements...) { } // :3

      template<typename... _UElements, typename = typename // :3
        enable_if<__and_<is_convertible<_UElements, // :3
                    _Elements>...>::value>::type> // :3
    explicit // :3
        constexpr tuple(_UElements&&... __elements) // :3
    : _Inherited(std::forward<_UElements>(__elements)...) {	} // :3

      constexpr tuple(const tuple&) = default; // :3

      constexpr tuple(tuple&&) = default; // :3

      template<typename... _UElements, typename = typename // :3
        enable_if<__and_<is_convertible<const _UElements&, // :3
                    _Elements>...>::value>::type> // :3
        constexpr tuple(const tuple<_UElements...>& __in) // :3
        : _Inherited(static_cast<const _Tuple_impl<0, _UElements...>&>(__in)) // :3
        { } // :3

      template<typename... _UElements, typename = typename // :3
        enable_if<__and_<is_convertible<_UElements, // :3
                    _Elements>...>::value>::type> // :3
        constexpr tuple(tuple<_UElements...>&& __in) // :3
        : _Inherited(static_cast<_Tuple_impl<0, _UElements...>&&>(__in)) { } // :3

      // Allocator-extended constructors. :3

      template<typename _Alloc> // :3
    tuple(allocator_arg_t __tag, const _Alloc& __a) // :3
    : _Inherited(__tag, __a) { } // :3

      template<typename _Alloc> // :3
    tuple(allocator_arg_t __tag, const _Alloc& __a, // :3
          const _Elements&... __elements) // :3
    : _Inherited(__tag, __a, __elements...) { } // :3

      template<typename _Alloc, typename... _UElements, typename = typename // :3
           enable_if<sizeof...(_UElements) // :3
             == sizeof...(_Elements)>::type> // :3
    tuple(allocator_arg_t __tag, const _Alloc& __a, // :3
          _UElements&&... __elements) // :3
    : _Inherited(__tag, __a, std::forward<_UElements>(__elements)...) // :3
        { } // :3

      template<typename _Alloc> // :3
    tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple& __in) // :3
    : _Inherited(__tag, __a, static_cast<const _Inherited&>(__in)) { } // :3

      template<typename _Alloc> // :3
    tuple(allocator_arg_t __tag, const _Alloc& __a, tuple&& __in) // :3
    : _Inherited(__tag, __a, static_cast<_Inherited&&>(__in)) { } // :3

      template<typename _Alloc, typename... _UElements, typename = typename // :3
           enable_if<sizeof...(_UElements) // :3
             == sizeof...(_Elements)>::type> // :3
    tuple(allocator_arg_t __tag, const _Alloc& __a, // :3
          const tuple<_UElements...>& __in) // :3
    : _Inherited(__tag, __a, // :3
                 static_cast<const _Tuple_impl<0, _UElements...>&>(__in)) // :3
    { } // :3

      template<typename _Alloc, typename... _UElements, typename = typename // :3
           enable_if<sizeof...(_UElements) // :3
             == sizeof...(_Elements)>::type> // :3
    tuple(allocator_arg_t __tag, const _Alloc& __a, // :3
          tuple<_UElements...>&& __in) // :3
    : _Inherited(__tag, __a, // :3
                 static_cast<_Tuple_impl<0, _UElements...>&&>(__in)) // :3
    { } // :3

      tuple& // :3
      operator=(const tuple& __in) // :3
      { // :3
    static_cast<_Inherited&>(*this) = __in; // :3
    return *this; // :3
      } // :3

      tuple& // :3
      operator=(tuple&& __in) // :3
      noexcept(is_nothrow_move_assignable<_Inherited>::value) // :3
      { // :3
    static_cast<_Inherited&>(*this) = std::move(__in); // :3
    return *this; // :3
      } // :3

      template<typename... _UElements, typename = typename // :3
           enable_if<sizeof...(_UElements) // :3
             == sizeof...(_Elements)>::type> // :3
        tuple& // :3
        operator=(const tuple<_UElements...>& __in) // :3
        { // :3
      static_cast<_Inherited&>(*this) = __in; // :3
      return *this; // :3
    } // :3

      template<typename... _UElements, typename = typename // :3
           enable_if<sizeof...(_UElements) // :3
             == sizeof...(_Elements)>::type> // :3
        tuple& // :3
        operator=(tuple<_UElements...>&& __in) // :3
        { // :3
      static_cast<_Inherited&>(*this) = std::move(__in); // :3
      return *this; // :3
    } // :3

      void // :3
      swap(tuple& __in) // :3
      noexcept(noexcept(__in._M_swap(__in))) // :3
      { _Inherited::_M_swap(__in); } // :3
    }; // :3

  // Explicit specialization, zero-element tuple. :3
  template<> // :3
    class tuple<> // :3
    { // :3
    public: // :3
      void swap(tuple&) noexcept { /* no-op */ }
    };

  /// Partial specialization, 2-element tuple.
  /// Includes construction and assignment from a pair.
  template<typename _T1, typename _T2>
    class tuple<_T1, _T2> : public _Tuple_impl<0, _T1, _T2>
    {
      typedef _Tuple_impl<0, _T1, _T2> _Inherited;

    public:
      constexpr tuple()
      : _Inherited() { }

      explicit
      constexpr tuple(const _T1& __a1, const _T2& __a2)
      : _Inherited(__a1, __a2) { }

      template<typename _U1, typename _U2, typename = typename
           enable_if<__and_<is_convertible<_U1, _T1>,
                is_convertible<_U2, _T2>>::value>::type>
        explicit
        constexpr tuple(_U1&& __a1, _U2&& __a2)
    : _Inherited(std::forward<_U1>(__a1), std::forward<_U2>(__a2)) { }

      constexpr tuple(const tuple&) = default;

      constexpr tuple(tuple&&) = default;

      template<typename _U1, typename _U2, typename = typename
    enable_if<__and_<is_convertible<const _U1&, _T1>,
             is_convertible<const _U2&, _T2>>::value>::type>
        constexpr tuple(const tuple<_U1, _U2>& __in)
    : _Inherited(static_cast<const _Tuple_impl<0, _U1, _U2>&>(__in)) { }

      template<typename _U1, typename _U2, typename = typename
           enable_if<__and_<is_convertible<_U1, _T1>,
                is_convertible<_U2, _T2>>::value>::type>
        constexpr tuple(tuple<_U1, _U2>&& __in)
    : _Inherited(static_cast<_Tuple_impl<0, _U1, _U2>&&>(__in)) { }

      template<typename _U1, typename _U2, typename = typename
    enable_if<__and_<is_convertible<const _U1&, _T1>,
             is_convertible<const _U2&, _T2>>::value>::type>
        constexpr tuple(const pair<_U1, _U2>& __in)
    : _Inherited(__in.first, __in.second) { }

      template<typename _U1, typename _U2, typename = typename
           enable_if<__and_<is_convertible<_U1, _T1>,
                is_convertible<_U2, _T2>>::value>::type>
        constexpr tuple(pair<_U1, _U2>&& __in)
    : _Inherited(std::forward<_U1>(__in.first),
             std::forward<_U2>(__in.second)) { }

      // Allocator-extended constructors.

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a)
    : _Inherited(__tag, __a) { }

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a,
          const _T1& __a1, const _T2& __a2)
    : _Inherited(__tag, __a, __a1, __a2) { }

      template<typename _Alloc, typename _U1, typename _U2>
    tuple(allocator_arg_t __tag, const _Alloc& __a, _U1&& __a1, _U2&& __a2)
    : _Inherited(__tag, __a, std::forward<_U1>(__a1),
                 std::forward<_U2>(__a2)) { }

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple& __in)
    : _Inherited(__tag, __a, static_cast<const _Inherited&>(__in)) { }

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a, tuple&& __in)
    : _Inherited(__tag, __a, static_cast<_Inherited&&>(__in)) { }

      template<typename _Alloc, typename _U1, typename _U2>
    tuple(allocator_arg_t __tag, const _Alloc& __a,
          const tuple<_U1, _U2>& __in)
    : _Inherited(__tag, __a,
                 static_cast<const _Tuple_impl<0, _U1, _U2>&>(__in))
    { }

      template<typename _Alloc, typename _U1, typename _U2>
    tuple(allocator_arg_t __tag, const _Alloc& __a, tuple<_U1, _U2>&& __in)
    : _Inherited(__tag, __a, static_cast<_Tuple_impl<0, _U1, _U2>&&>(__in))
    { }

      template<typename _Alloc, typename _U1, typename _U2>
        tuple(allocator_arg_t __tag, const _Alloc& __a,
          const pair<_U1, _U2>& __in)
    : _Inherited(__tag, __a, __in.first, __in.second) { }

      template<typename _Alloc, typename _U1, typename _U2>
        tuple(allocator_arg_t __tag, const _Alloc& __a, pair<_U1, _U2>&& __in)
    : _Inherited(__tag, __a, std::forward<_U1>(__in.first),
             std::forward<_U2>(__in.second)) { }

      tuple&
      operator=(const tuple& __in)
      {
    static_cast<_Inherited&>(*this) = __in;
    return *this;
      }

      tuple&
      operator=(tuple&& __in)
      noexcept(is_nothrow_move_assignable<_Inherited>::value)
      {
    static_cast<_Inherited&>(*this) = std::move(__in);
    return *this;
      }

      template<typename _U1, typename _U2>
        tuple&
        operator=(const tuple<_U1, _U2>& __in)
        {
      static_cast<_Inherited&>(*this) = __in;
      return *this;
    }

      template<typename _U1, typename _U2>
        tuple&
        operator=(tuple<_U1, _U2>&& __in)
        {
      static_cast<_Inherited&>(*this) = std::move(__in);
      return *this;
    }

      template<typename _U1, typename _U2>
        tuple&
        operator=(const pair<_U1, _U2>& __in)
        {
      this->_M_head(*this) = __in.first;
      this->_M_tail(*this)._M_head(*this) = __in.second;
      return *this;
    }

      template<typename _U1, typename _U2>
        tuple&
        operator=(pair<_U1, _U2>&& __in)
        {
      this->_M_head(*this) = std::forward<_U1>(__in.first);
      this->_M_tail(*this)._M_head(*this) = std::forward<_U2>(__in.second);
      return *this;
    }

      void
      swap(tuple& __in)
      noexcept(noexcept(__in._M_swap(__in)))
      { _Inherited::_M_swap(__in); }
    };


  /// Gives the type of the ith element of a given tuple type.
  template<std::size_t __i, typename _Tp>
    struct tuple_element;

  /**
   * Recursive case for tuple_element: strip off the first element in
   * the tuple and retrieve the (i-1)th element of the remaining tuple.
   */ // :3
  template<std::size_t __i, typename _Head, typename... _Tail> // :3
    struct tuple_element<__i, tuple<_Head, _Tail...> > // :3
    : tuple_element<__i - 1, tuple<_Tail...> > { }; // :3

  /**
   * Basis case for tuple_element: The first element is the one we're seeking.
   */ // :3
  template<typename _Head, typename... _Tail> // :3
    struct tuple_element<0, tuple<_Head, _Tail...> > // :3
    { // :3
      typedef _Head type; // :3
    }; // :3

  template<std::size_t __i, typename _Tp> // :3
    struct tuple_element<__i, const _Tp> // :3
    { // :3
      typedef typename // :3
      add_const<typename tuple_element<__i, _Tp>::type>::type type; // :3
    }; // :3

  template<std::size_t __i, typename _Tp> // :3
    struct tuple_element<__i, volatile _Tp> // :3
    { // :3
      typedef typename // :3
      add_volatile<typename tuple_element<__i, _Tp>::type>::type type; // :3
    }; // :3

  template<std::size_t __i, typename _Tp> // :3
    struct tuple_element<__i, const volatile _Tp> // :3
    { // :3
      typedef typename // :3
      add_cv<typename tuple_element<__i, _Tp>::type>::type type; // :3
    }; // :3

#if __cplusplus > 201103L // :3
  template<std::size_t __i, typename _Tp> // :3
    using tuple_element_t = typename tuple_element<__i, _Tp>::type; // :3
#endif // :3

  /// Finds the size of a given tuple type. :3
  template<typename _Tp> // :3
    struct tuple_size; // :3

  // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
  // 2313. tuple_size should always derive from integral_constant<size_t, N> :3
  template<typename _Tp> // :3
    struct tuple_size<const _Tp> // :3
    : public integral_constant<size_t, tuple_size<_Tp>::value> { }; // :3

  template<typename _Tp> // :3
    struct tuple_size<volatile _Tp> // :3
    : public integral_constant<size_t, tuple_size<_Tp>::value> { }; // :3

  template<typename _Tp> // :3
    struct tuple_size<const volatile _Tp> // :3
    : public integral_constant<size_t, tuple_size<_Tp>::value> { }; // :3

  /// class tuple_size :3
  template<typename... _Elements> // :3
    struct tuple_size<tuple<_Elements...>> // :3
    : public integral_constant<std::size_t, sizeof...(_Elements)> { }; // :3

  template<std::size_t __i, typename _Head, typename... _Tail> // :3
    constexpr typename __add_ref<_Head>::type // :3
    __get_helper(_Tuple_impl<__i, _Head, _Tail...>& __t) noexcept // :3
    { return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t); } // :3

  template<std::size_t __i, typename _Head, typename... _Tail> // :3
    constexpr typename __add_c_ref<_Head>::type // :3
    __get_helper(const _Tuple_impl<__i, _Head, _Tail...>& __t) noexcept // :3
    { return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t); } // :3

  /// Return a reference to the ith element of a tuple. :3
  template<std::size_t __i, typename... _Elements> // :3
    constexpr typename __add_ref< // :3
                      typename tuple_element<__i, tuple<_Elements...>>::type // :3
                    >::type // :3
    get(tuple<_Elements...>& __t) noexcept // :3
    { return __get_helper<__i>(__t); } // :3

  /// Return a const reference to the ith element of a const tuple. :3
  template<std::size_t __i, typename... _Elements> // :3
    constexpr typename __add_c_ref< // :3
                      typename tuple_element<__i, tuple<_Elements...>>::type // :3
                    >::type // :3
    get(const tuple<_Elements...>& __t) noexcept // :3
    { return __get_helper<__i>(__t); } // :3

  /// Return an rvalue reference to the ith element of a tuple rvalue. :3
  template<std::size_t __i, typename... _Elements> // :3
    constexpr typename __add_r_ref< // :3
                      typename tuple_element<__i, tuple<_Elements...>>::type // :3
                    >::type // :3
    get(tuple<_Elements...>&& __t) noexcept // :3
    { return std::forward<typename tuple_element<__i, // :3
    tuple<_Elements...>>::type&&>(get<__i>(__t)); } // :3

#if __cplusplus > 201103L // :3

  template<typename _Head, size_t __i, typename... _Tail> // :3
    constexpr typename __add_ref<_Head>::type // :3
    __get_helper2(_Tuple_impl<__i, _Head, _Tail...>& __t) noexcept // :3
    { return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t); } // :3

  template<typename _Head, size_t __i, typename... _Tail> // :3
    constexpr typename __add_c_ref<_Head>::type // :3
    __get_helper2(const _Tuple_impl<__i, _Head, _Tail...>& __t) noexcept // :3
    { return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t); } // :3

  /// Return a reference to the unique element of type _Tp of a tuple. :3
  template <typename _Tp, typename... _Types> // :3
    constexpr _Tp& // :3
    get(tuple<_Types...>& __t) noexcept // :3
    { return __get_helper2<_Tp>(__t); } // :3

  /// Return a reference to the unique element of type _Tp of a tuple rvalue. :3
  template <typename _Tp, typename... _Types> // :3
    constexpr _Tp&& // :3
    get(tuple<_Types...>&& __t) noexcept // :3
    { return std::forward<_Tp&&>(__get_helper2<_Tp>(__t)); } // :3

  /// Return a const reference to the unique element of type _Tp of a tuple. :3
  template <typename _Tp, typename... _Types> // :3
    constexpr const _Tp& // :3
    get(const tuple<_Types...>& __t) noexcept // :3
    { return __get_helper2<_Tp>(__t); } // :3
#endif // :3


  // This class helps construct the various comparison operations on tuples :3
  template<std::size_t __check_equal_size, std::size_t __i, std::size_t __j, // :3
       typename _Tp, typename _Up> // :3
    struct __tuple_compare; // :3

  template<std::size_t __i, std::size_t __j, typename _Tp, typename _Up> // :3
    struct __tuple_compare<0, __i, __j, _Tp, _Up> // :3
    { // :3
      static constexpr bool // :3
      __eq(const _Tp& __t, const _Up& __u) // :3
      { // :3
    return (get<__i>(__t) == get<__i>(__u) && // :3
        __tuple_compare<0, __i + 1, __j, _Tp, _Up>::__eq(__t, __u)); // :3
      } // :3

      static constexpr bool // :3
      __less(const _Tp& __t, const _Up& __u) // :3
      { // :3
    return ((get<__i>(__t) < get<__i>(__u)) // :3
        || !(get<__i>(__u) < get<__i>(__t)) && // :3
        __tuple_compare<0, __i + 1, __j, _Tp, _Up>::__less(__t, __u)); // :3
      } // :3
    }; // :3

  template<std::size_t __i, typename _Tp, typename _Up> // :3
    struct __tuple_compare<0, __i, __i, _Tp, _Up> // :3
    { // :3
      static constexpr bool // :3
      __eq(const _Tp&, const _Up&) { return true; } // :3

      static constexpr bool // :3
      __less(const _Tp&, const _Up&) { return false; } // :3
    }; // :3

  template<typename... _TElements, typename... _UElements> // :3
    constexpr bool // :3
    operator==(const tuple<_TElements...>& __t, // :3
           const tuple<_UElements...>& __u) // :3
    { // :3
      typedef tuple<_TElements...> _Tp; // :3
      typedef tuple<_UElements...> _Up; // :3
      return bool(__tuple_compare<tuple_size<_Tp>::value - tuple_size<_Up>::value, // :3
          0, tuple_size<_Tp>::value, _Tp, _Up>::__eq(__t, __u)); // :3
    } // :3

  template<typename... _TElements, typename... _UElements> // :3
    constexpr bool // :3
    operator<(const tuple<_TElements...>& __t, // :3
          const tuple<_UElements...>& __u) // :3
    { // :3
      typedef tuple<_TElements...> _Tp; // :3
      typedef tuple<_UElements...> _Up; // :3
      return bool(__tuple_compare<tuple_size<_Tp>::value - tuple_size<_Up>::value, // :3
          0, tuple_size<_Tp>::value, _Tp, _Up>::__less(__t, __u)); // :3
    } // :3

  template<typename... _TElements, typename... _UElements> // :3
    constexpr bool // :3
    operator!=(const tuple<_TElements...>& __t, // :3
           const tuple<_UElements...>& __u) // :3
    { return !(__t == __u); } // :3

  template<typename... _TElements, typename... _UElements> // :3
    constexpr bool // :3
    operator>(const tuple<_TElements...>& __t, // :3
          const tuple<_UElements...>& __u) // :3
    { return __u < __t; } // :3

  template<typename... _TElements, typename... _UElements> // :3
    constexpr bool // :3
    operator<=(const tuple<_TElements...>& __t, // :3
           const tuple<_UElements...>& __u) // :3
    { return !(__u < __t); } // :3

  template<typename... _TElements, typename... _UElements> // :3
    constexpr bool // :3
    operator>=(const tuple<_TElements...>& __t, // :3
           const tuple<_UElements...>& __u) // :3
    { return !(__t < __u); } // :3

  // NB: DR 705. :3
  template<typename... _Elements> // :3
    constexpr tuple<typename __decay_and_strip<_Elements>::__type...> // :3
    make_tuple(_Elements&&... __args) // :3
    { // :3
      typedef tuple<typename __decay_and_strip<_Elements>::__type...> // :3
    __result_type; // :3
      return __result_type(std::forward<_Elements>(__args)...); // :3
    } // :3

  template<typename... _Elements> // :3
    tuple<_Elements&&...> // :3
    forward_as_tuple(_Elements&&... __args) noexcept // :3
    { return tuple<_Elements&&...>(std::forward<_Elements>(__args)...); } // :3

  template<typename> // :3
    struct __is_tuple_like_impl : false_type // :3
    { }; // :3

  template<typename... _Tps> // :3
    struct __is_tuple_like_impl<tuple<_Tps...>> : true_type // :3
    { }; // :3

  template<typename _T1, typename _T2> // :3
    struct __is_tuple_like_impl<pair<_T1, _T2>> : true_type // :3
    { }; // :3

  template<typename _Tp, std::size_t _Nm> // :3
    struct __is_tuple_like_impl<array<_Tp, _Nm>> : true_type // :3
    { }; // :3

  // Internal type trait that allows us to sfinae-protect tuple_cat. :3
  template<typename _Tp> // :3
    struct __is_tuple_like // :3
    : public __is_tuple_like_impl<typename std::remove_cv // :3
            <typename std::remove_reference<_Tp>::type>::type>::type // :3
    { }; // :3

  template<std::size_t, typename, typename, std::size_t> // :3
    struct __make_tuple_impl; // :3

  template<std::size_t _Idx, typename _Tuple, typename... _Tp, // :3
           std::size_t _Nm> // :3
    struct __make_tuple_impl<_Idx, tuple<_Tp...>, _Tuple, _Nm> // :3
    { // :3
      typedef typename __make_tuple_impl<_Idx + 1, tuple<_Tp..., // :3
    typename tuple_element<_Idx, _Tuple>::type>, _Tuple, _Nm>::__type // :3
      __type; // :3
    }; // :3

  template<std::size_t _Nm, typename _Tuple, typename... _Tp> // :3
    struct __make_tuple_impl<_Nm, tuple<_Tp...>, _Tuple, _Nm> // :3
    { // :3
      typedef tuple<_Tp...> __type; // :3
    }; // :3

  template<typename _Tuple> // :3
    struct __do_make_tuple // :3
    : public __make_tuple_impl<0, tuple<>, _Tuple, // :3
                               tuple_size<_Tuple>::value> // :3
    { }; // :3

  // Returns the std::tuple equivalent of a tuple-like type. :3
  template<typename _Tuple> // :3
    struct __make_tuple // :3
    : public __do_make_tuple<typename std::remove_cv // :3
            <typename std::remove_reference<_Tuple>::type>::type> // :3
    { }; // :3

  // Combines several std::tuple's into a single one. :3
  template<typename...> // :3
    struct __combine_tuples; // :3

  template<> // :3
    struct __combine_tuples<> // :3
    { // :3
      typedef tuple<> __type; // :3
    }; // :3

  template<typename... _Ts> // :3
    struct __combine_tuples<tuple<_Ts...>> // :3
    { // :3
      typedef tuple<_Ts...> __type; // :3
    }; // :3

  template<typename... _T1s, typename... _T2s, typename... _Rem> // :3
    struct __combine_tuples<tuple<_T1s...>, tuple<_T2s...>, _Rem...> // :3
    { // :3
      typedef typename __combine_tuples<tuple<_T1s..., _T2s...>, // :3
                    _Rem...>::__type __type; // :3
    }; // :3

  // Computes the result type of tuple_cat given a set of tuple-like types. :3
  template<typename... _Tpls> // :3
    struct __tuple_cat_result // :3
    { // :3
      typedef typename __combine_tuples // :3
        <typename __make_tuple<_Tpls>::__type...>::__type __type; // :3
    }; // :3

  // Helper to determine the index set for the first tuple-like :3
  // type of a given set. :3
  template<typename...> // :3
    struct __make_1st_indices; // :3

  template<> // :3
    struct __make_1st_indices<> // :3
    { // :3
      typedef _Index_tuple<> __type; // :3
    }; // :3

  template<typename _Tp, typename... _Tpls> // :3
    struct __make_1st_indices<_Tp, _Tpls...> // :3
    { // :3
      typedef typename _Build_index_tuple<tuple_size< // :3
    typename std::remove_reference<_Tp>::type>::value>::__type __type; // :3
    }; // :3

  // Performs the actual concatenation by step-wise expanding tuple-like :3
  // objects into the elements,  which are finally forwarded into the :3
  // result tuple. :3
  template<typename _Ret, typename _Indices, typename... _Tpls> // :3
    struct __tuple_concater; // :3

  template<typename _Ret, std::size_t... _Is, typename _Tp, typename... _Tpls> // :3
    struct __tuple_concater<_Ret, _Index_tuple<_Is...>, _Tp, _Tpls...> // :3
    { // :3
      template<typename... _Us> // :3
        static constexpr _Ret // :3
        _S_do(_Tp&& __tp, _Tpls&&... __tps, _Us&&... __us) // :3
        { // :3
      typedef typename __make_1st_indices<_Tpls...>::__type __idx; // :3
      typedef __tuple_concater<_Ret, __idx, _Tpls...>      __next; // :3
      return __next::_S_do(std::forward<_Tpls>(__tps)..., // :3
                   std::forward<_Us>(__us)..., // :3
                   get<_Is>(std::forward<_Tp>(__tp))...); // :3
    } // :3
    }; // :3

  template<typename _Ret> // :3
    struct __tuple_concater<_Ret, _Index_tuple<>> // :3
    { // :3
      template<typename... _Us> // :3
    static constexpr _Ret // :3
    _S_do(_Us&&... __us) // :3
        { // :3
      return _Ret(std::forward<_Us>(__us)...); // :3
    } // :3
    }; // :3

  /// tuple_cat :3
  template<typename... _Tpls, typename = typename // :3
           enable_if<__and_<__is_tuple_like<_Tpls>...>::value>::type> // :3
    constexpr auto // :3
    tuple_cat(_Tpls&&... __tpls) // :3
    -> typename __tuple_cat_result<_Tpls...>::__type // :3
    { // :3
      typedef typename __tuple_cat_result<_Tpls...>::__type __ret; // :3
      typedef typename __make_1st_indices<_Tpls...>::__type __idx; // :3
      typedef __tuple_concater<__ret, __idx, _Tpls...> __concater; // :3
      return __concater::_S_do(std::forward<_Tpls>(__tpls)...); // :3
    } // :3

  /// tie :3
  template<typename... _Elements> // :3
    inline tuple<_Elements&...> // :3
    tie(_Elements&... __args) noexcept // :3
    { return tuple<_Elements&...>(__args...); } // :3

  /// swap :3
  template<typename... _Elements> // :3
    inline void // :3
    swap(tuple<_Elements...>& __x, tuple<_Elements...>& __y) // :3
    noexcept(noexcept(__x.swap(__y))) // :3
    { __x.swap(__y); } // :3

  // A class (and instance) which can be used in 'tie' when an element :3
  // of a tuple is not required :3
  struct _Swallow_assign // :3
  { // :3
    template<class _Tp> // :3
      const _Swallow_assign& // :3
      operator=(const _Tp&) const // :3
      { return *this; } // :3
  }; // :3

  const _Swallow_assign ignore{}; // :3

  /// Partial specialization for tuples :3
  template<typename... _Types, typename _Alloc> // :3
    struct uses_allocator<tuple<_Types...>, _Alloc> : true_type { }; // :3

  // See stl_pair.h... :3
  template<class _T1, class _T2> // :3
    template<typename... _Args1, typename... _Args2> // :3
      inline // :3
      pair<_T1, _T2>:: // :3
      pair(piecewise_construct_t, // :3
       tuple<_Args1...> __first, tuple<_Args2...> __second) // :3
      : pair(__first, __second, // :3
         typename _Build_index_tuple<sizeof...(_Args1)>::__type(), // :3
         typename _Build_index_tuple<sizeof...(_Args2)>::__type()) // :3
      { } // :3

  template<class _T1, class _T2> // :3
    template<typename... _Args1, std::size_t... _Indexes1, // :3
             typename... _Args2, std::size_t... _Indexes2> // :3
      inline // :3
      pair<_T1, _T2>:: // :3
      pair(tuple<_Args1...>& __tuple1, tuple<_Args2...>& __tuple2, // :3
       _Index_tuple<_Indexes1...>, _Index_tuple<_Indexes2...>) // :3
      : first(std::forward<_Args1>(get<_Indexes1>(__tuple1))...), // :3
        second(std::forward<_Args2>(get<_Indexes2>(__tuple2))...) // :3
      { } // :3

  /// @} :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3

#endif // C++11 :3
