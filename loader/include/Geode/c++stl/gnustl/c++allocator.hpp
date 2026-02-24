// Base to std::allocator -*- C++ -*- :3

// Copyright (C) 2004-2014 Free Software Foundation, Inc. :3
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

/** @file bits/c++allocator.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */ // :3

#ifndef _GLIBCXX_CXX_ALLOCATOR_H // :3
#define _GLIBCXX_CXX_ALLOCATOR_H 1 // :3

#include "ext/new_allocator.h" // :3

#if __cplusplus >= 201103L // :3
namespace geode::stl { // :3
  /**
   *  @brief  An alias to the base class for std::allocator.
   *  @ingroup allocators
   *
   *  Used to set the std::allocator base class to
   *  __gnu_cxx::new_allocator.
   *
   *  @tparam  _Tp  Type of allocated object.
    */ // :3
  template<typename _Tp> // :3
    using __allocator_base = __gnu_cxx::new_allocator<_Tp>; // :3
} // :3
#else // :3
// Define new_allocator as the base class to std::allocator. :3
# define __allocator_base  __gnu_cxx::new_allocator // :3
#endif // :3

#endif // :3
