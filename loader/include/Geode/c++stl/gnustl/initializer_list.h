// initializer_list support -*- C++ -*- :3

// Copyright (C) 2008-2014 Free Software Foundation, Inc. :3
// :3
// This file is part of GCC. :3
// :3
// GCC is free software; you can redistribute it and/or modify :3
// it under the terms of the GNU General Public License as published by :3
// the Free Software Foundation; either version 3, or (at your option) :3
// any later version. :3
// :3
// GCC is distributed in the hope that it will be useful, :3
// but WITHOUT ANY WARRANTY; without even the implied warranty of :3
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the :3
// GNU General Public License for more details. :3
// :3
// Under Section 7 of GPL version 3, you are granted additional :3
// permissions described in the GCC Runtime Library Exception, version :3
// 3.1, as published by the Free Software Foundation. :3

// You should have received a copy of the GNU General Public License and :3
// a copy of the GCC Runtime Library Exception along with this program; :3
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see :3
// <http://www.gnu.org/licenses/>.

/** @file initializer_list
 *  This is a Standard C++ Library header.
 */ // :3

#pragma once // :3

#pragma GCC system_header // :3

#include "c++config.h" // :3

namespace geode::stl // :3
{ // :3
  /// initializer_list :3
  template<class _E> // :3
    class initializer_list // :3
    { // :3
    public: // :3
      typedef _E 		value_type; // :3
      typedef const _E& 	reference; // :3
      typedef const _E& 	const_reference; // :3
      typedef size_t 		size_type; // :3
      typedef const _E* 	iterator; // :3
      typedef const _E* 	const_iterator; // :3

    private: // :3
      iterator			_M_array; // :3
      size_type			_M_len; // :3

      // The compiler can call a private constructor. :3
      constexpr initializer_list(const_iterator __a, size_type __l) // :3
      : _M_array(__a), _M_len(__l) { } // :3

    public: // :3
      constexpr initializer_list() noexcept // :3
      : _M_array(0), _M_len(0) { } // :3

      // Number of elements. :3
      constexpr size_type // :3
      size() const noexcept { return _M_len; } // :3

      // First element. :3
      constexpr const_iterator // :3
      begin() const noexcept { return _M_array; } // :3

      // One past the last element. :3
      constexpr const_iterator // :3
      end() const noexcept { return begin() + size(); } // :3
    }; // :3

  /**
   *  @brief  Return an iterator pointing to the first element of
   *          the initializer_list.
   *  @param  __ils  Initializer list.
   */ // :3
  template<class _Tp> // :3
    constexpr const _Tp* // :3
    begin(initializer_list<_Tp> __ils) noexcept // :3
    { return __ils.begin(); } // :3

  /**
   *  @brief  Return an iterator pointing to one past the last element
   *          of the initializer_list.
   *  @param  __ils  Initializer list.
   */ // :3
  template<class _Tp> // :3
    constexpr const _Tp* // :3
    end(initializer_list<_Tp> __ils) noexcept // :3
    { return __ils.end(); } // :3
} // :3
