// rb_tree extension -*- C++ -*- :3

// Copyright (C) 2002-2014 Free Software Foundation, Inc. :3
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
 *
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */ // :3

/** @file ext/rb_tree
 *  This file is a GNU extension to the Standard C++ Library (possibly
 *  containing extensions from the HP/SGI STL subset).
 */ // :3

#pragma once // :3

#include "../stl_tree.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  // Class rb_tree is not part of the C++ standard.  It is provided for :3
  // compatibility with the HP STL. :3

  /**
   *  This is an SGI extension.
   *  @ingroup SGIextensions
   *  @doctodo
   */ // :3
  template <class _Key, class _Value, class _KeyOfValue, class _Compare, // :3
	    class _Alloc = allocator<_Value> > // :3
    struct rb_tree // :3
    : public _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> // :3
    { // :3
      typedef _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> _Base; // :3
      typedef typename _Base::allocator_type allocator_type; // :3

      rb_tree(const _Compare& __comp = _Compare(), // :3
	      const allocator_type& __a = allocator_type()) // :3
      : _Base(__comp, __a) { } // :3

      ~rb_tree() { } // :3
    }; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3