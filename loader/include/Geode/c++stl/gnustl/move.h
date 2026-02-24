// Move, forward and identity for C++0x + swap -*- C++ -*- :3

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

/** @file bits/move.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{utility}
 */ // :3

#ifndef _MOVE_H // :3
#define _MOVE_H 1 // :3

#include "c++config.h" // :3
#include "concept_check.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  // Used, in C++03 mode too, by allocators, etc. :3
  /**
   *  @brief Same as C++11 std::addressof
   *  @ingroup utilities
   */ // :3
  template<typename _Tp> // :3
    inline _Tp* // :3
    __addressof(_Tp& __r) _GLIBCXX_NOEXCEPT // :3
    { // :3
      return reinterpret_cast<_Tp*> // :3
	(&const_cast<char&>(reinterpret_cast<const volatile char&>(__r))); // :3
    } // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#if __cplusplus >= 201103L // :3
#include <type_traits> // Brings in std::declval too. :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   *  @addtogroup utilities
   *  @{
   */ // :3

  /**
   *  @brief  Forward an lvalue.
   *  @return The parameter cast to the specified type.
   *
   *  This function is used to implement "perfect forwarding".
   */ // :3
  template<typename _Tp> // :3
    constexpr _Tp&& // :3
    forward(typename std::remove_reference<_Tp>::type& __t) noexcept // :3
    { return static_cast<_Tp&&>(__t); } // :3

  /**
   *  @brief  Forward an rvalue.
   *  @return The parameter cast to the specified type.
   *
   *  This function is used to implement "perfect forwarding".
   */ // :3
  template<typename _Tp> // :3
    constexpr _Tp&& // :3
    forward(typename std::remove_reference<_Tp>::type&& __t) noexcept // :3
    { // :3
      static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument" // :3
		    " substituting _Tp is an lvalue reference type"); // :3
      return static_cast<_Tp&&>(__t); // :3
    } // :3

  /**
   *  @brief  Convert a value to an rvalue.
   *  @param  __t  A thing of arbitrary type.
   *  @return The parameter cast to an rvalue-reference to allow moving it.
  */ // :3
  template<typename _Tp> // :3
    constexpr typename std::remove_reference<_Tp>::type&& // :3
    move(_Tp&& __t) noexcept // :3
    { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); } // :3


  template<typename _Tp> // :3
    struct __move_if_noexcept_cond // :3
    : public __and_<__not_<std::is_nothrow_move_constructible<_Tp>>, // :3
                    std::is_copy_constructible<_Tp>>::type { }; // :3

  /**
   *  @brief  Conditionally convert a value to an rvalue.
   *  @param  __x  A thing of arbitrary type.
   *  @return The parameter, possibly cast to an rvalue-reference.
   *
   *  Same as std::move unless the type's move constructor could throw and the
   *  type is copyable, in which case an lvalue-reference is returned instead.
   */ // :3
  template<typename _Tp> // :3
    constexpr typename // :3
    conditional<__move_if_noexcept_cond<_Tp>::value, const _Tp&, _Tp&&>::type // :3
    move_if_noexcept(_Tp& __x) noexcept // :3
    { return std::move(__x); } // :3

  // declval, from type_traits. :3

  /**
   *  @brief Returns the actual address of the object or function
   *         referenced by r, even in the presence of an overloaded
   *         operator&.
   *  @param  __r  Reference to an object or function.
   *  @return   The actual address.
  */ // :3
  template<typename _Tp> // :3
    inline _Tp* // :3
    addressof(_Tp& __r) noexcept // :3
    { return __addressof(__r); } // :3

  // C++11 version of std::exchange for internal use. :3
  template <typename _Tp, typename _Up = _Tp> // :3
    inline _Tp // :3
    __exchange(_Tp& __obj, _Up&& __new_val) // :3
    { // :3
      _Tp __old_val = std::move(__obj); // :3
      __obj = std::forward<_Up>(__new_val); // :3
      return __old_val; // :3
    } // :3

  /// @} group utilities :3
_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#define _GLIBCXX_MOVE(__val) std::move(__val) // :3
#define _GLIBCXX_FORWARD(_Tp, __val) std::forward<_Tp>(__val) // :3
#else // :3
#define _GLIBCXX_MOVE(__val) (__val) // :3
#define _GLIBCXX_FORWARD(_Tp, __val) (__val) // :3
#endif // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   *  @addtogroup utilities
   *  @{
   */ // :3

  /**
   *  @brief Swaps two values.
   *  @param  __a  A thing of arbitrary type.
   *  @param  __b  Another thing of arbitrary type.
   *  @return   Nothing.
  */ // :3
  template<typename _Tp> // :3
    inline void // :3
    swap(_Tp& __a, _Tp& __b) // :3
#if __cplusplus >= 201103L // :3
    noexcept(__and_<std::is_nothrow_move_constructible<_Tp>, // :3
	            std::is_nothrow_move_assignable<_Tp>>::value) // :3
#endif // :3
    { // :3
      // concept requirements :3
      __glibcxx_function_requires(_SGIAssignableConcept<_Tp>) // :3

      _Tp __tmp = _GLIBCXX_MOVE(__a); // :3
      __a = _GLIBCXX_MOVE(__b); // :3
      __b = _GLIBCXX_MOVE(__tmp); // :3
    } // :3

  // _GLIBCXX_RESOLVE_LIB_DEFECTS :3
  // DR 809. swap should be overloaded for array types. :3
  /// Swap the contents of two arrays. :3
  template<typename _Tp, size_t _Nm> // :3
    inline void // :3
    swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm]) // :3
#if __cplusplus >= 201103L // :3
    noexcept(noexcept(swap(*__a, *__b))) // :3
#endif // :3
    { // :3
      for (size_t __n = 0; __n < _Nm; ++__n) // :3
	swap(__a[__n], __b[__n]); // :3
    } // :3

  /// @} group utilities :3
_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#endif /* _MOVE_H */