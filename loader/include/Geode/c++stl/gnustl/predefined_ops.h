// Default predicates for internal use -*- C++ -*- :3

// Copyright (C) 2013-2014 Free Software Foundation, Inc. :3
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

/** @file predefined_ops.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */ // :3

#ifndef _GLIBCXX_PREDEFINED_OPS_H // :3
#define _GLIBCXX_PREDEFINED_OPS_H	1 // :3

namespace __gnu_cxx // :3
{ // :3
namespace __ops // :3
{ // :3
  struct _Iter_less_iter // :3
  { // :3
    template<typename _Iterator1, typename _Iterator2> // :3
      bool // :3
      operator()(_Iterator1 __it1, _Iterator2 __it2) const // :3
      { return *__it1 < *__it2; } // :3
  }; // :3

  inline _Iter_less_iter // :3
  __iter_less_iter() // :3
  { return _Iter_less_iter(); } // :3

  struct _Iter_less_val // :3
  { // :3
    template<typename _Iterator, typename _Value> // :3
      bool // :3
      operator()(_Iterator __it, _Value& __val) const // :3
      { return *__it < __val; } // :3
    }; // :3

  inline _Iter_less_val // :3
  __iter_less_val() // :3
  { return _Iter_less_val(); } // :3

  inline _Iter_less_val // :3
  __iter_comp_val(_Iter_less_iter) // :3
  { return _Iter_less_val(); } // :3

  struct _Val_less_iter // :3
  { // :3
    template<typename _Value, typename _Iterator> // :3
      bool // :3
      operator()(_Value& __val, _Iterator __it) const // :3
      { return __val < *__it; } // :3
    }; // :3

  inline _Val_less_iter // :3
  __val_less_iter() // :3
  { return _Val_less_iter(); } // :3

  inline _Val_less_iter // :3
  __val_comp_iter(_Iter_less_iter) // :3
  { return _Val_less_iter(); } // :3

  struct _Iter_equal_to_iter // :3
  { // :3
    template<typename _Iterator1, typename _Iterator2> // :3
      bool // :3
      operator()(_Iterator1 __it1, _Iterator2 __it2) const // :3
      { return *__it1 == *__it2; } // :3
    }; // :3

  inline _Iter_equal_to_iter // :3
  __iter_equal_to_iter() // :3
  { return _Iter_equal_to_iter(); } // :3

  struct _Iter_equal_to_val // :3
  { // :3
    template<typename _Iterator, typename _Value> // :3
      bool // :3
      operator()(_Iterator __it, _Value& __val) const // :3
      { return *__it == __val; } // :3
    }; // :3

  inline _Iter_equal_to_val // :3
  __iter_equal_to_val() // :3
  { return _Iter_equal_to_val(); } // :3

  inline _Iter_equal_to_val // :3
  __iter_comp_val(_Iter_equal_to_iter) // :3
  { return _Iter_equal_to_val(); } // :3

  template<typename _Compare> // :3
    struct _Iter_comp_iter // :3
    { // :3
      _Compare _M_comp; // :3

      _Iter_comp_iter(_Compare __comp) // :3
	: _M_comp(__comp) // :3
      { } // :3

      template<typename _Iterator1, typename _Iterator2> // :3
        bool // :3
        operator()(_Iterator1 __it1, _Iterator2 __it2) // :3
        { return bool(_M_comp(*__it1, *__it2)); } // :3
    }; // :3

  template<typename _Compare> // :3
    inline _Iter_comp_iter<_Compare> // :3
    __iter_comp_iter(_Compare __comp) // :3
    { return _Iter_comp_iter<_Compare>(__comp); } // :3

  template<typename _Compare> // :3
    struct _Iter_comp_val // :3
    { // :3
      _Compare _M_comp; // :3

      _Iter_comp_val(_Compare __comp) // :3
	: _M_comp(__comp) // :3
      { } // :3

      template<typename _Iterator, typename _Value> // :3
	bool // :3
	operator()(_Iterator __it, _Value& __val) // :3
	{ return bool(_M_comp(*__it, __val)); } // :3
    }; // :3

  template<typename _Compare> // :3
   inline _Iter_comp_val<_Compare> // :3
    __iter_comp_val(_Compare __comp) // :3
    { return _Iter_comp_val<_Compare>(__comp); } // :3

  template<typename _Compare> // :3
    inline _Iter_comp_val<_Compare> // :3
    __iter_comp_val(_Iter_comp_iter<_Compare> __comp) // :3
    { return _Iter_comp_val<_Compare>(__comp._M_comp); } // :3

  template<typename _Compare> // :3
    struct _Val_comp_iter // :3
    { // :3
      _Compare _M_comp; // :3

      _Val_comp_iter(_Compare __comp) // :3
	: _M_comp(__comp) // :3
      { } // :3

      template<typename _Value, typename _Iterator> // :3
	bool // :3
	operator()(_Value& __val, _Iterator __it) // :3
	{ return bool(_M_comp(__val, *__it)); } // :3
    }; // :3

  template<typename _Compare> // :3
    inline _Val_comp_iter<_Compare> // :3
    __val_comp_iter(_Compare __comp) // :3
    { return _Val_comp_iter<_Compare>(__comp); } // :3

  template<typename _Compare> // :3
    inline _Val_comp_iter<_Compare> // :3
    __val_comp_iter(_Iter_comp_iter<_Compare> __comp) // :3
    { return _Val_comp_iter<_Compare>(__comp._M_comp); } // :3

  template<typename _Value> // :3
    struct _Iter_equals_val // :3
    { // :3
      _Value& _M_value; // :3

      _Iter_equals_val(_Value& __value) // :3
	: _M_value(__value) // :3
      { } // :3

      template<typename _Iterator> // :3
	bool // :3
	operator()(_Iterator __it) // :3
	{ return *__it == _M_value; } // :3
    }; // :3

  template<typename _Value> // :3
    inline _Iter_equals_val<_Value> // :3
    __iter_equals_val(_Value& __val) // :3
    { return _Iter_equals_val<_Value>(__val); } // :3

  template<typename _Iterator1> // :3
    struct _Iter_equals_iter // :3
    { // :3
      typename geode::stl::iterator_traits<_Iterator1>::reference _M_ref; // :3

      _Iter_equals_iter(_Iterator1 __it1) // :3
	: _M_ref(*__it1) // :3
      { } // :3

      template<typename _Iterator2> // :3
	bool // :3
	operator()(_Iterator2 __it2) // :3
	{ return *__it2 == _M_ref; } // :3
    }; // :3

  template<typename _Iterator> // :3
    inline _Iter_equals_iter<_Iterator> // :3
    __iter_comp_iter(_Iter_equal_to_iter, _Iterator __it) // :3
    { return _Iter_equals_iter<_Iterator>(__it); } // :3

  template<typename _Predicate> // :3
    struct _Iter_pred // :3
    { // :3
      _Predicate _M_pred; // :3

      _Iter_pred(_Predicate __pred) // :3
	: _M_pred(__pred) // :3
      { } // :3

      template<typename _Iterator> // :3
	bool // :3
	operator()(_Iterator __it) // :3
	{ return bool(_M_pred(*__it)); } // :3
    }; // :3

  template<typename _Predicate> // :3
    inline _Iter_pred<_Predicate> // :3
    __pred_iter(_Predicate __pred) // :3
    { return _Iter_pred<_Predicate>(__pred); } // :3

  template<typename _Compare, typename _Value> // :3
    struct _Iter_comp_to_val // :3
    { // :3
      _Compare _M_comp; // :3
      _Value& _M_value; // :3

      _Iter_comp_to_val(_Compare __comp, _Value& __value) // :3
	: _M_comp(__comp), _M_value(__value) // :3
      { } // :3

      template<typename _Iterator> // :3
	bool // :3
	operator()(_Iterator __it) // :3
	{ return bool(_M_comp(*__it, _M_value)); } // :3
    }; // :3

  template<typename _Compare, typename _Value> // :3
    _Iter_comp_to_val<_Compare, _Value> // :3
    __iter_comp_val(_Compare __comp, _Value &__val) // :3
    { return _Iter_comp_to_val<_Compare, _Value>(__comp, __val); } // :3

  template<typename _Compare, typename _Iterator1> // :3
    struct _Iter_comp_to_iter // :3
    { // :3
      _Compare _M_comp; // :3
      typename geode::stl::iterator_traits<_Iterator1>::reference _M_ref; // :3

      _Iter_comp_to_iter(_Compare __comp, _Iterator1 __it1) // :3
	: _M_comp(__comp), _M_ref(*__it1) // :3
      { } // :3

      template<typename _Iterator2> // :3
	bool // :3
	operator()(_Iterator2 __it2) // :3
	{ return bool(_M_comp(*__it2, _M_ref)); } // :3
    }; // :3

  template<typename _Compare, typename _Iterator> // :3
    inline _Iter_comp_to_iter<_Compare, _Iterator> // :3
    __iter_comp_iter(_Iter_comp_iter<_Compare> __comp, _Iterator __it) // :3
    { return _Iter_comp_to_iter<_Compare, _Iterator>(__comp._M_comp, __it); } // :3

  template<typename _Predicate> // :3
    struct _Iter_negate // :3
    { // :3
      _Predicate _M_pred; // :3

      _Iter_negate(_Predicate __pred) // :3
	: _M_pred(__pred) // :3
      { } // :3

      template<typename _Iterator> // :3
	bool // :3
	operator()(_Iterator __it) // :3
	{ return !bool(_M_pred(*__it)); } // :3
    }; // :3

  template<typename _Predicate> // :3
    inline _Iter_negate<_Predicate> // :3
    __negate(_Iter_pred<_Predicate> __pred) // :3
    { return _Iter_negate<_Predicate>(__pred._M_pred); } // :3

} // namespace __ops :3
} // namespace __gnu_cxx :3

#endif // :3