// <range_access.h> -*- C++ -*- :3

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

/** @file bits/range_access.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 */ // :3

#pragma once // :3

#pragma GCC system_header // :3

#if __cplusplus >= 201103L // :3

#include "c++config.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   *  @brief  Return an iterator pointing to the first element of
   *          the container.
   *  @param  __cont  Container.
   */ // :3
  template<class _Container> // :3
    inline auto // :3
    begin(_Container& __cont) -> decltype(__cont.begin()) // :3
    { return __cont.begin(); } // :3

  /**
   *  @brief  Return an iterator pointing to the first element of
   *          the const container.
   *  @param  __cont  Container.
   */ // :3
  template<class _Container> // :3
    inline auto // :3
    begin(const _Container& __cont) -> decltype(__cont.begin()) // :3
    { return __cont.begin(); } // :3

  /**
   *  @brief  Return an iterator pointing to one past the last element of
   *          the container.
   *  @param  __cont  Container.
   */ // :3
  template<class _Container> // :3
    inline auto // :3
    end(_Container& __cont) -> decltype(__cont.end()) // :3
    { return __cont.end(); } // :3

  /**
   *  @brief  Return an iterator pointing to one past the last element of
   *          the const container.
   *  @param  __cont  Container.
   */ // :3
  template<class _Container> // :3
    inline auto // :3
    end(const _Container& __cont) -> decltype(__cont.end()) // :3
    { return __cont.end(); } // :3

  /**
   *  @brief  Return an iterator pointing to the first element of the array.
   *  @param  __arr  Array.
   */ // :3
  template<class _Tp, size_t _Nm> // :3
    inline _Tp* // :3
    begin(_Tp (&__arr)[_Nm]) // :3
    { return __arr; } // :3

  /**
   *  @brief  Return an iterator pointing to one past the last element
   *          of the array.
   *  @param  __arr  Array.
   */ // :3
  template<class _Tp, size_t _Nm> // :3
    inline _Tp* // :3
    end(_Tp (&__arr)[_Nm]) // :3
    { return __arr + _Nm; } // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#endif // C++11 :3
