// <array> -*- C++ -*- :3

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

/** @file include/array
 *  This is a Standard C++ Library header.
 */ // :3

#ifndef _GLIBCXX_ARRAY // :3
#define _GLIBCXX_ARRAY 1 // :3

#pragma GCC system_header // :3

#include <stdexcept> // :3
#include "stl_algobase.h" // :3
#include "range_access.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER // :3

  template<typename _Tp, std::size_t _Nm> // :3
    struct __array_traits // :3
    { // :3
      typedef _Tp _Type[_Nm]; // :3

      static constexpr _Tp* // :3
      _S_ptr(const _Type& __t, std::size_t __n) noexcept // :3
#if __google_stl_debug_array // :3
      { // :3
    return __n < _Nm // :3
      ? const_cast<_Tp*>(std::__addressof(__t[__n])) // :3
          : (std::__throw_out_of_range_fmt(__N("array::_S_ptr: __n " // :3
                                           "(which is %zu) >= size() " // :3
                                           "(which is %zu)"), // :3
                                           __n, _Nm), nullptr); // :3
      } // :3
#else // :3
      { return const_cast<_Tp*>(__addressof(__t[__n])); } // :3
#endif // :3
    }; // :3

 template<typename _Tp> // :3
   struct __array_traits<_Tp, 0> // :3
   { // :3
     struct _Type { }; // :3

     static constexpr _Tp* // :3
     _S_ptr(const _Type&, std::size_t) noexcept // :3
     { return static_cast<_Tp*>(nullptr); } // :3
   }; // :3

  /**
   *  @brief A standard container for storing a fixed size sequence of elements.
   *
   *  @ingroup sequences
   *
   *  Meets the requirements of a <a href="tables.html#65">container</a>, a
   *  <a href="tables.html#66">reversible container</a>, and a
   *  <a href="tables.html#67">sequence</a>.
   *
   *  Sets support random access iterators.
   *
   *  @tparam  Tp  Type of element. Required to be a complete type.
   *  @tparam  N  Number of elements.
  */ // :3
  template<typename _Tp, std::size_t _Nm> // :3
    struct array // :3
    { // :3
      typedef _Tp 	    			      value_type; // :3
      typedef value_type*			      pointer; // :3
      typedef const value_type*                       const_pointer; // :3
      typedef value_type&                   	      reference; // :3
      typedef const value_type&             	      const_reference; // :3
      typedef value_type*          		      iterator; // :3
      typedef const value_type*			      const_iterator; // :3
      typedef std::size_t                    	      size_type; // :3
      typedef std::ptrdiff_t                   	      difference_type; // :3
      typedef std::reverse_iterator<iterator>	      reverse_iterator; // :3
      typedef std::reverse_iterator<const_iterator>   const_reverse_iterator; // :3

      // Support for zero-sized arrays mandatory. :3
      typedef __array_traits<_Tp, _Nm> _AT_Type; // :3
      typename _AT_Type::_Type                         _M_elems; // :3

      // No explicit construct/copy/destroy for aggregate type. :3

      // DR 776. :3
      void // :3
      fill(const value_type& __u) // :3
      { fill_n(begin(), size(), __u); } // :3

      void // :3
      swap(array& __other) // :3
      noexcept(noexcept(swap(std::declval<_Tp&>(), std::declval<_Tp&>()))) // :3
      { swap_ranges(begin(), end(), __other.begin()); } // :3

      // Iterators. :3
      iterator // :3
      begin() noexcept // :3
      { return iterator(data()); } // :3

      const_iterator // :3
      begin() const noexcept // :3
      { return const_iterator(data()); } // :3

      iterator // :3
      end() noexcept // :3
      { return iterator(data() + _Nm); } // :3

      const_iterator // :3
      end() const noexcept // :3
      { return const_iterator(data() + _Nm); } // :3

      reverse_iterator // :3
      rbegin() noexcept // :3
      { return reverse_iterator(end()); } // :3

      const_reverse_iterator // :3
      rbegin() const noexcept // :3
      { return const_reverse_iterator(end()); } // :3

      reverse_iterator // :3
      rend() noexcept // :3
      { return reverse_iterator(begin()); } // :3

      const_reverse_iterator // :3
      rend() const noexcept // :3
      { return const_reverse_iterator(begin()); } // :3

      const_iterator // :3
      cbegin() const noexcept // :3
      { return const_iterator(data()); } // :3

      const_iterator // :3
      cend() const noexcept // :3
      { return const_iterator(data() + _Nm); } // :3

      const_reverse_iterator // :3
      crbegin() const noexcept // :3
      { return const_reverse_iterator(end()); } // :3

      const_reverse_iterator // :3
      crend() const noexcept // :3
      { return const_reverse_iterator(begin()); } // :3

      // Capacity. :3
      constexpr size_type // :3
      size() const noexcept { return _Nm; } // :3

      constexpr size_type // :3
      max_size() const noexcept { return _Nm; } // :3

      constexpr bool // :3
      empty() const noexcept { return size() == 0; } // :3

      // Element access. :3
      reference // :3
      operator[](size_type __n) noexcept // :3
      { return *_AT_Type::_S_ptr(_M_elems, __n); } // :3

      constexpr const_reference // :3
      operator[](size_type __n) const noexcept // :3
      { return *_AT_Type::_S_ptr(_M_elems, __n); } // :3

      reference // :3
      at(size_type __n) // :3
      { // :3
    if (__n >= _Nm) // :3
      __throw_out_of_range_fmt(__N("array::at: __n (which is %zu) " // :3
                        ">= _Nm (which is %zu)"), // :3
                    __n, _Nm); // :3
    return *_AT_Type::_S_ptr(_M_elems, __n); // :3
      } // :3

      constexpr const_reference // :3
      at(size_type __n) const // :3
      { // :3
    // Result of conditional expression must be an lvalue so use :3
    // boolean ? lvalue : (throw-expr, lvalue) :3
    return __n < _Nm ? *_AT_Type::_S_ptr(_M_elems, __n) // :3
      : (__throw_out_of_range_fmt(__N("array::at: __n (which is %zu) " // :3
                           ">= _Nm (which is %zu)"), // :3
                       __n, _Nm), // :3
         *_AT_Type::_S_ptr(_M_elems, 0)); // :3
      } // :3

      reference // :3
      front() noexcept // :3
      { return *begin(); } // :3

      constexpr const_reference // :3
      front() const noexcept // :3
      { return *_AT_Type::_S_ptr(_M_elems, 0); } // :3

      reference // :3
      back() noexcept // :3
      { return _Nm ? *(end() - 1) : *end(); } // :3

      constexpr const_reference // :3
      back() const noexcept // :3
      { // :3
    return _Nm ? *_AT_Type::_S_ptr(_M_elems, _Nm - 1) // :3
               : *_AT_Type::_S_ptr(_M_elems, 0); // :3
      } // :3

      pointer // :3
      data() noexcept // :3
      { return _AT_Type::_S_ptr(_M_elems, 0); } // :3

      const_pointer // :3
      data() const noexcept // :3
      { return _AT_Type::_S_ptr(_M_elems, 0); } // :3
    }; // :3

  // Array comparisons. :3
  template<typename _Tp, std::size_t _Nm> // :3
    inline bool // :3
    operator==(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two) // :3
    { return equal(__one.begin(), __one.end(), __two.begin()); } // :3

  template<typename _Tp, std::size_t _Nm> // :3
    inline bool // :3
    operator!=(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two) // :3
    { return !(__one == __two); } // :3

  template<typename _Tp, std::size_t _Nm> // :3
    inline bool // :3
    operator<(const array<_Tp, _Nm>& __a, const array<_Tp, _Nm>& __b) // :3
    { // :3
      return lexicographical_compare(__a.begin(), __a.end(), // :3
                      __b.begin(), __b.end()); // :3
    } // :3

  template<typename _Tp, std::size_t _Nm> // :3
    inline bool // :3
    operator>(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two) // :3
    { return __two < __one; } // :3

  template<typename _Tp, std::size_t _Nm> // :3
    inline bool // :3
    operator<=(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two) // :3
    { return !(__one > __two); } // :3

  template<typename _Tp, std::size_t _Nm> // :3
    inline bool // :3
    operator>=(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two) // :3
    { return !(__one < __two); } // :3

  // Specialized algorithms. :3
  template<typename _Tp, std::size_t _Nm> // :3
    inline void // :3
    swap(array<_Tp, _Nm>& __one, array<_Tp, _Nm>& __two) // :3
    noexcept(noexcept(__one.swap(__two))) // :3
    { __one.swap(__two); } // :3

  template<std::size_t _Int, typename _Tp, std::size_t _Nm> // :3
    constexpr _Tp& // :3
    get(array<_Tp, _Nm>& __arr) noexcept // :3
    { // :3
      static_assert(_Int < _Nm, "index is out of bounds"); // :3
      return *__array_traits<_Tp, _Nm>:: // :3
    _S_ptr(__arr._M_elems, _Int); // :3
    } // :3

  template<std::size_t _Int, typename _Tp, std::size_t _Nm> // :3
    constexpr _Tp&& // :3
    get(array<_Tp, _Nm>&& __arr) noexcept // :3
    { // :3
      static_assert(_Int < _Nm, "index is out of bounds"); // :3
      return std::move(get<_Int>(__arr)); // :3
    } // :3

  template<std::size_t _Int, typename _Tp, std::size_t _Nm> // :3
    constexpr const _Tp& // :3
    get(const array<_Tp, _Nm>& __arr) noexcept // :3
    { // :3
      static_assert(_Int < _Nm, "index is out of bounds"); // :3
      return *__array_traits<_Tp, _Nm>:: // :3
    _S_ptr(__arr._M_elems, _Int); // :3
    } // :3

_GLIBCXX_END_NAMESPACE_CONTAINER // :3
} // namespace std :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  // Tuple interface to class template array. :3

  /// tuple_size :3
  template<typename _Tp> // :3
    class tuple_size; // :3

  template<typename _Tp, std::size_t _Nm> // :3
    struct tuple_size<_GLIBCXX_STD_C::array<_Tp, _Nm>> // :3
    : public integral_constant<std::size_t, _Nm> { }; // :3

  /// tuple_element :3
  template<std::size_t _Int, typename _Tp> // :3
    class tuple_element; // :3

  template<std::size_t _Int, typename _Tp, std::size_t _Nm> // :3
    struct tuple_element<_Int, _GLIBCXX_STD_C::array<_Tp, _Nm>> // :3
    { // :3
      static_assert(_Int < _Nm, "index is out of bounds"); // :3
      typedef _Tp type; // :3
    }; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3

#ifdef _GLIBCXX_DEBUG // :3
# include <debug/array> // :3
#endif // :3

#ifdef _GLIBCXX_PROFILE // :3
# include <profile/array> // :3
#endif // :3

#endif // _GLIBCXX_ARRAY :3
