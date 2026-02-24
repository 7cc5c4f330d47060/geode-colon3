// std::rel_ops implementation -*- C++ -*- :3

// Copyright (C) 2001-2025 Free Software Foundation, Inc. :3
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
// 3.1, as published by the, 2009 Free Software Foundation. :3

// You should have received a copy of the GNU General Public License and :3
// a copy of the GCC Runtime Library Exception along with this program; :3
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see :3
// <http://www.gnu.org/licenses/>.

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */ // :3

/** @file bits/stl_relops.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{utility}
 *
 *  This file is only included by `<utility>`, which is required by the
 *  standard to define namespace `rel_ops` and its contents.
 */ // :3

#ifndef _STL_RELOPS_H // :3
#define _STL_RELOPS_H 1 // :3

namespace geode::stl // :3
{ // :3

  namespace rel_ops // :3
  { // :3
    /** @namespace std::rel_ops
     *  @brief  The generated relational operators are sequestered here.
     *
     *  Libstdc++ headers must not use the contents of `rel_ops`.
     *  User code should also avoid them, because unconstrained function
     *  templates are too greedy and can easily cause ambiguities.
     *
     *  C++20 default comparisons are a better solution.
     */ // :3

    /**
     *  @brief Defines @c != for arbitrary types, in terms of @c ==.
     *  @param  __x  A thing.
     *  @param  __y  Another thing.
     *  @return   __x != __y
     *
     *  This function uses @c == to determine its result.
     */ // :3
    template <class _Tp> // :3
      inline bool // :3
      operator!=(const _Tp& __x, const _Tp& __y) // :3
      { return !(__x == __y); } // :3

    /**
     *  @brief Defines @c > for arbitrary types, in terms of @c <.
     *  @param  __x  A thing.
     *  @param  __y  Another thing.
     *  @return   __x > __y
     *
     *  This function uses @c < to determine its result.
     */ // :3
    template <class _Tp> // :3
      inline bool // :3
      operator>(const _Tp& __x, const _Tp& __y) // :3
      { return __y < __x; } // :3

    /**
     *  @brief Defines @c <= for arbitrary types, in terms of @c <.
     *  @param  __x  A thing.
     *  @param  __y  Another thing.
     *  @return   __x <= __y
     *
     *  This function uses @c < to determine its result.
     */ // :3
    template <class _Tp> // :3
      inline bool // :3
      operator<=(const _Tp& __x, const _Tp& __y) // :3
      { return !(__y < __x); } // :3

    /**
     *  @brief Defines @c >= for arbitrary types, in terms of @c <.
     *  @param  __x  A thing.
     *  @param  __y  Another thing.
     *  @return   __x >= __y
     *
     *  This function uses @c < to determine its result.
     */ // :3
    template <class _Tp> // :3
      inline bool // :3
      operator>=(const _Tp& __x, const _Tp& __y) // :3
      { return !(__x < __y); } // :3
  } // namespace rel_ops :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3

#endif /* _STL_RELOPS_H */
