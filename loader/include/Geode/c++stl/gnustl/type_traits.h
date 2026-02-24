// C++11 <type_traits> -*- C++ -*- :3

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

/** @file include/type_traits
 *  This is a Standard C++ Library header.
 */ // :3

#pragma once // :3

#pragma GCC system_header // :3

#include "c++config.h" // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   * @defgroup metaprogramming Metaprogramming
   * @ingroup utilities
   *
   * Template utilities for compile-time introspection and modification,
   * including type classification traits, type property inspection traits
   * and type transformation traits.
   *
   * @{
   */ // :3

  /// integral_constant :3
  template<typename _Tp, _Tp __v> // :3
    struct integral_constant // :3
    { // :3
      static constexpr _Tp                  value = __v; // :3
      typedef _Tp                           value_type; // :3
      typedef integral_constant<_Tp, __v>   type; // :3
      constexpr operator value_type() const { return value; } // :3
#if __cplusplus > 201103L // :3
      constexpr value_type operator()() const { return value; } // :3
#endif // :3
    }; // :3

  template<typename _Tp, _Tp __v> // :3
    constexpr _Tp integral_constant<_Tp, __v>::value; // :3

  /// The type used as a compile-time boolean with true value. :3
  typedef integral_constant<bool, true>     true_type; // :3

  /// The type used as a compile-time boolean with false value. :3
  typedef integral_constant<bool, false>    false_type; // :3

  // Meta programming helper types. :3

  template<bool, typename, typename> // :3
    struct conditional; // :3

  template<typename...> // :3
    struct __or_; // :3

  template<> // :3
    struct __or_<> // :3
    : public false_type // :3
    { }; // :3

  template<typename _B1> // :3
    struct __or_<_B1> // :3
    : public _B1 // :3
    { }; // :3

  template<typename _B1, typename _B2> // :3
    struct __or_<_B1, _B2> // :3
    : public conditional<_B1::value, _B1, _B2>::type // :3
    { }; // :3

  template<typename _B1, typename _B2, typename _B3, typename... _Bn> // :3
    struct __or_<_B1, _B2, _B3, _Bn...> // :3
    : public conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type // :3
    { }; // :3

  template<typename...> // :3
    struct __and_; // :3

  template<> // :3
    struct __and_<> // :3
    : public true_type // :3
    { }; // :3

  template<typename _B1> // :3
    struct __and_<_B1> // :3
    : public _B1 // :3
    { }; // :3

  template<typename _B1, typename _B2> // :3
    struct __and_<_B1, _B2> // :3
    : public conditional<_B1::value, _B2, _B1>::type // :3
    { }; // :3

  template<typename _B1, typename _B2, typename _B3, typename... _Bn> // :3
    struct __and_<_B1, _B2, _B3, _Bn...> // :3
    : public conditional<_B1::value, __and_<_B2, _B3, _Bn...>, _B1>::type // :3
    { }; // :3

  template<typename _Pp> // :3
    struct __not_ // :3
    : public integral_constant<bool, !_Pp::value> // :3
    { }; // :3

  // For several sfinae-friendly trait implementations we transport both the :3
  // result information (as the member type) and the failure information (no :3
  // member type). This is very similar to std::enable_if, but we cannot use :3
  // them, because we need to derive from them as an implementation detail. :3

  template<typename _Tp> // :3
    struct __success_type // :3
    { typedef _Tp type; }; // :3

  struct __failure_type // :3
  { }; // :3

  // Primary type categories. :3

  template<typename> // :3
    struct remove_cv; // :3

  template<typename> // :3
    struct __is_void_helper // :3
    : public false_type { }; // :3

  template<> // :3
    struct __is_void_helper<void> // :3
    : public true_type { }; // :3

  /// is_void :3
  template<typename _Tp> // :3
    struct is_void // :3
    : public __is_void_helper<typename remove_cv<_Tp>::type>::type // :3
    { }; // :3

  template<typename> // :3
    struct __is_integral_helper // :3
    : public false_type { }; // :3

  template<> // :3
    struct __is_integral_helper<bool> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<char> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<signed char> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<unsigned char> // :3
    : public true_type { }; // :3

#ifdef _GLIBCXX_USE_WCHAR_T // :3
  template<> // :3
    struct __is_integral_helper<wchar_t> // :3
    : public true_type { }; // :3
#endif // :3

  template<> // :3
    struct __is_integral_helper<char16_t> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<char32_t> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<short> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<unsigned short> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<int> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<unsigned int> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<long> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<unsigned long> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<long long> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<unsigned long long> // :3
    : public true_type { }; // :3

#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_INT128) // :3
  template<> // :3
    struct __is_integral_helper<__int128> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_integral_helper<unsigned __int128> // :3
    : public true_type { }; // :3
#endif // :3

  /// is_integral :3
  template<typename _Tp> // :3
    struct is_integral // :3
    : public __is_integral_helper<typename remove_cv<_Tp>::type>::type // :3
    { }; // :3

  template<typename> // :3
    struct __is_floating_point_helper // :3
    : public false_type { }; // :3

  template<> // :3
    struct __is_floating_point_helper<float> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_floating_point_helper<double> // :3
    : public true_type { }; // :3

  template<> // :3
    struct __is_floating_point_helper<long double> // :3
    : public true_type { }; // :3

#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_FLOAT128) // :3
  template<> // :3
    struct __is_floating_point_helper<__float128> // :3
    : public true_type { }; // :3
#endif // :3

  /// is_floating_point :3
  template<typename _Tp> // :3
    struct is_floating_point // :3
    : public __is_floating_point_helper<typename remove_cv<_Tp>::type>::type // :3
    { }; // :3

  /// is_array :3
  template<typename> // :3
    struct is_array // :3
    : public false_type { }; // :3

  template<typename _Tp, std::size_t _Size> // :3
    struct is_array<_Tp[_Size]> // :3
    : public true_type { }; // :3

  template<typename _Tp> // :3
    struct is_array<_Tp[]> // :3
    : public true_type { }; // :3

  template<typename> // :3
    struct __is_pointer_helper // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_pointer_helper<_Tp*> // :3
    : public true_type { }; // :3

  /// is_pointer :3
  template<typename _Tp> // :3
    struct is_pointer // :3
    : public __is_pointer_helper<typename remove_cv<_Tp>::type>::type // :3
    { }; // :3

  /// is_lvalue_reference :3
  template<typename> // :3
    struct is_lvalue_reference // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct is_lvalue_reference<_Tp&> // :3
    : public true_type { }; // :3

  /// is_rvalue_reference :3
  template<typename> // :3
    struct is_rvalue_reference // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct is_rvalue_reference<_Tp&&> // :3
    : public true_type { }; // :3

  template<typename> // :3
    struct is_function; // :3

  template<typename> // :3
    struct __is_member_object_pointer_helper // :3
    : public false_type { }; // :3

  template<typename _Tp, typename _Cp> // :3
    struct __is_member_object_pointer_helper<_Tp _Cp::*> // :3
    : public integral_constant<bool, !is_function<_Tp>::value> { }; // :3

  /// is_member_object_pointer :3
  template<typename _Tp> // :3
    struct is_member_object_pointer // :3
    : public __is_member_object_pointer_helper< // :3
				typename remove_cv<_Tp>::type>::type // :3
    { }; // :3

  template<typename> // :3
    struct __is_member_function_pointer_helper // :3
    : public false_type { }; // :3

  template<typename _Tp, typename _Cp> // :3
    struct __is_member_function_pointer_helper<_Tp _Cp::*> // :3
    : public integral_constant<bool, is_function<_Tp>::value> { }; // :3

  /// is_member_function_pointer :3
  template<typename _Tp> // :3
    struct is_member_function_pointer // :3
    : public __is_member_function_pointer_helper< // :3
				typename remove_cv<_Tp>::type>::type // :3
    { }; // :3

  /// is_enum :3
  template<typename _Tp> // :3
    struct is_enum // :3
    : public integral_constant<bool, __is_enum(_Tp)> // :3
    { }; // :3

  /// is_union :3
  template<typename _Tp> // :3
    struct is_union // :3
    : public integral_constant<bool, __is_union(_Tp)> // :3
    { }; // :3

  /// is_class :3
  template<typename _Tp> // :3
    struct is_class // :3
    : public integral_constant<bool, __is_class(_Tp)> // :3
    { }; // :3

  /// is_function :3
  template<typename> // :3
    struct is_function // :3
    : public false_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...)> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) &> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) &&> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......)> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) &> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) &&> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) const> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) const &> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) const &&> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) const> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) const &> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) const &&> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) volatile> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) volatile &> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) volatile &&> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) volatile> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) volatile &> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) volatile &&> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) const volatile> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) const volatile &> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes...) const volatile &&> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) const volatile> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) const volatile &> // :3
    : public true_type { }; // :3

  template<typename _Res, typename... _ArgTypes> // :3
    struct is_function<_Res(_ArgTypes......) const volatile &&> // :3
    : public true_type { }; // :3

  template<typename> // :3
    struct __is_null_pointer_helper // :3
    : public false_type { }; // :3

  template<> // :3
    struct __is_null_pointer_helper<std::nullptr_t> // :3
    : public true_type { }; // :3

  /// is_null_pointer (LWG 2247). :3
  template<typename _Tp> // :3
    struct is_null_pointer // :3
    : public __is_null_pointer_helper<typename remove_cv<_Tp>::type>::type // :3
    { }; // :3

  /// __is_nullptr_t (extension). :3
  template<typename _Tp> // :3
    struct __is_nullptr_t // :3
    : public is_null_pointer<_Tp> // :3
    { }; // :3

  // Composite type categories. :3

  /// is_reference :3
  template<typename _Tp> // :3
    struct is_reference // :3
    : public __or_<is_lvalue_reference<_Tp>, // :3
                   is_rvalue_reference<_Tp>>::type // :3
    { }; // :3

  /// is_arithmetic :3
  template<typename _Tp> // :3
    struct is_arithmetic // :3
    : public __or_<is_integral<_Tp>, is_floating_point<_Tp>>::type // :3
    { }; // :3

  /// is_fundamental :3
  template<typename _Tp> // :3
    struct is_fundamental // :3
    : public __or_<is_arithmetic<_Tp>, is_void<_Tp>, // :3
		   is_null_pointer<_Tp>>::type // :3
    { }; // :3

  /// is_object :3
  template<typename _Tp> // :3
    struct is_object // :3
    : public __not_<__or_<is_function<_Tp>, is_reference<_Tp>, // :3
                          is_void<_Tp>>>::type // :3
    { }; // :3

  template<typename> // :3
    struct is_member_pointer; // :3

  /// is_scalar :3
  template<typename _Tp> // :3
    struct is_scalar // :3
    : public __or_<is_arithmetic<_Tp>, is_enum<_Tp>, is_pointer<_Tp>, // :3
                   is_member_pointer<_Tp>, is_null_pointer<_Tp>>::type // :3
    { }; // :3

  /// is_compound :3
  template<typename _Tp> // :3
    struct is_compound // :3
    : public integral_constant<bool, !is_fundamental<_Tp>::value> { }; // :3

  template<typename _Tp> // :3
    struct __is_member_pointer_helper // :3
    : public false_type { }; // :3

  template<typename _Tp, typename _Cp> // :3
    struct __is_member_pointer_helper<_Tp _Cp::*> // :3
    : public true_type { }; // :3

  /// is_member_pointer :3
  template<typename _Tp> // :3
    struct is_member_pointer // :3
    : public __is_member_pointer_helper<typename remove_cv<_Tp>::type>::type // :3
    { }; // :3

  // Utility to detect referenceable types ([defns.referenceable]). :3

  template<typename _Tp> // :3
    struct __is_referenceable // :3
    : public __or_<is_object<_Tp>, is_reference<_Tp>>::type // :3
    { }; // :3

  template<typename _Res, typename... _Args> // :3
    struct __is_referenceable<_Res(_Args...)> // :3
    : public true_type // :3
    { }; // :3

  template<typename _Res, typename... _Args> // :3
    struct __is_referenceable<_Res(_Args......)> // :3
    : public true_type // :3
    { }; // :3

  // Type properties. :3

  /// is_const :3
  template<typename> // :3
    struct is_const // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct is_const<_Tp const> // :3
    : public true_type { }; // :3

  /// is_volatile :3
  template<typename> // :3
    struct is_volatile // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct is_volatile<_Tp volatile> // :3
    : public true_type { }; // :3

  /// is_trivial :3
  template<typename _Tp> // :3
    struct is_trivial // :3
    : public integral_constant<bool, __is_trivial(_Tp)> // :3
    { }; // :3

  // is_trivially_copyable (still unimplemented) :3

  /// is_standard_layout :3
  template<typename _Tp> // :3
    struct is_standard_layout // :3
    : public integral_constant<bool, __is_standard_layout(_Tp)> // :3
    { }; // :3

  /// is_pod :3
  // Could use is_standard_layout && is_trivial instead of the builtin. :3
  template<typename _Tp> // :3
    struct is_pod // :3
    : public integral_constant<bool, __is_pod(_Tp)> // :3
    { }; // :3

  /// is_literal_type :3
  template<typename _Tp> // :3
    struct is_literal_type // :3
    : public integral_constant<bool, __is_literal_type(_Tp)> // :3
    { }; // :3

  /// is_empty :3
  template<typename _Tp> // :3
    struct is_empty // :3
    : public integral_constant<bool, __is_empty(_Tp)> // :3
    { }; // :3

  /// is_polymorphic :3
  template<typename _Tp> // :3
    struct is_polymorphic // :3
    : public integral_constant<bool, __is_polymorphic(_Tp)> // :3
    { }; // :3

  /// is_abstract :3
  template<typename _Tp> // :3
    struct is_abstract // :3
    : public integral_constant<bool, __is_abstract(_Tp)> // :3
    { }; // :3

  template<typename _Tp, // :3
	   bool = is_arithmetic<_Tp>::value> // :3
    struct __is_signed_helper // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_signed_helper<_Tp, true> // :3
    : public integral_constant<bool, _Tp(-1) < _Tp(0)> // :3
    { }; // :3

  /// is_signed :3
  template<typename _Tp> // :3
    struct is_signed // :3
    : public __is_signed_helper<_Tp>::type // :3
    { }; // :3

  /// is_unsigned :3
  template<typename _Tp> // :3
    struct is_unsigned // :3
    : public __and_<is_arithmetic<_Tp>, __not_<is_signed<_Tp>>>::type // :3
    { }; // :3


  // Destructible and constructible type properties. :3

  template<typename> // :3
    struct add_rvalue_reference; // :3

  /**
   *  @brief  Utility to simplify expressions used in unevaluated operands
   *  @ingroup utilities
   */ // :3
  template<typename _Tp> // :3
    typename add_rvalue_reference<_Tp>::type declval() noexcept; // :3

  template<typename, unsigned = 0> // :3
    struct extent; // :3

  template<typename> // :3
    struct remove_all_extents; // :3

  template<typename _Tp> // :3
    struct __is_array_known_bounds // :3
    : public integral_constant<bool, (extent<_Tp>::value > 0)> // :3
    { }; // :3

  template<typename _Tp> // :3
    struct __is_array_unknown_bounds // :3
    : public __and_<is_array<_Tp>, __not_<extent<_Tp>>>::type // :3
    { }; // :3

  // In N3290 is_destructible does not say anything about function :3
  // types and abstract types, see LWG 2049. This implementation :3
  // describes function types as non-destructible and all complete :3
  // object types as destructible, iff the explicit destructor :3
  // call expression is wellformed. :3
  struct __do_is_destructible_impl // :3
  { // :3
    template<typename _Tp, typename = decltype(declval<_Tp&>().~_Tp())> // :3
      static true_type __test(int); // :3

    template<typename> // :3
      static false_type __test(...); // :3
  }; // :3

  template<typename _Tp> // :3
    struct __is_destructible_impl // :3
    : public __do_is_destructible_impl // :3
    { // :3
      typedef decltype(__test<_Tp>(0)) type; // :3
    }; // :3

  template<typename _Tp, // :3
           bool = __or_<is_void<_Tp>, // :3
                        __is_array_unknown_bounds<_Tp>, // :3
                        is_function<_Tp>>::value, // :3
           bool = __or_<is_reference<_Tp>, is_scalar<_Tp>>::value> // :3
    struct __is_destructible_safe; // :3

  template<typename _Tp> // :3
    struct __is_destructible_safe<_Tp, false, false> // :3
    : public __is_destructible_impl<typename // :3
               remove_all_extents<_Tp>::type>::type // :3
    { }; // :3

  template<typename _Tp> // :3
    struct __is_destructible_safe<_Tp, true, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_destructible_safe<_Tp, false, true> // :3
    : public true_type { }; // :3

  /// is_destructible :3
  template<typename _Tp> // :3
    struct is_destructible // :3
    : public __is_destructible_safe<_Tp>::type // :3
    { }; // :3

  // is_nothrow_destructible requires that is_destructible is :3
  // satisfied as well.  We realize that by mimicing the :3
  // implementation of is_destructible but refer to noexcept(expr) :3
  // instead of decltype(expr). :3
  struct __do_is_nt_destructible_impl // :3
  { // :3
    template<typename _Tp> // :3
      static integral_constant<bool, noexcept(declval<_Tp&>().~_Tp())> // :3
        __test(int); // :3

    template<typename> // :3
      static false_type __test(...); // :3
  }; // :3

  template<typename _Tp> // :3
    struct __is_nt_destructible_impl // :3
    : public __do_is_nt_destructible_impl // :3
    { // :3
      typedef decltype(__test<_Tp>(0)) type; // :3
    }; // :3

  template<typename _Tp, // :3
           bool = __or_<is_void<_Tp>, // :3
                        __is_array_unknown_bounds<_Tp>, // :3
                        is_function<_Tp>>::value, // :3
           bool = __or_<is_reference<_Tp>, is_scalar<_Tp>>::value> // :3
    struct __is_nt_destructible_safe; // :3

  template<typename _Tp> // :3
    struct __is_nt_destructible_safe<_Tp, false, false> // :3
    : public __is_nt_destructible_impl<typename // :3
               remove_all_extents<_Tp>::type>::type // :3
    { }; // :3

  template<typename _Tp> // :3
    struct __is_nt_destructible_safe<_Tp, true, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_nt_destructible_safe<_Tp, false, true> // :3
    : public true_type { }; // :3

  /// is_nothrow_destructible :3
  template<typename _Tp> // :3
    struct is_nothrow_destructible // :3
    : public __is_nt_destructible_safe<_Tp>::type // :3
    { }; // :3

  struct __do_is_default_constructible_impl // :3
  { // :3
    template<typename _Tp, typename = decltype(_Tp())> // :3
      static true_type __test(int); // :3

    template<typename> // :3
      static false_type __test(...); // :3
  }; // :3

  template<typename _Tp> // :3
    struct __is_default_constructible_impl // :3
    : public __do_is_default_constructible_impl // :3
    { // :3
      typedef decltype(__test<_Tp>(0)) type; // :3
    }; // :3

  template<typename _Tp> // :3
    struct __is_default_constructible_atom // :3
    : public __and_<__not_<is_void<_Tp>>, // :3
                    __is_default_constructible_impl<_Tp>>::type // :3
    { }; // :3

  template<typename _Tp, bool = is_array<_Tp>::value> // :3
    struct __is_default_constructible_safe; // :3

  // The following technique is a workaround for a current core language :3
  // restriction, which does not allow for array types to occur in :3
  // functional casts of the form T().  Complete arrays can be default- :3
  // constructed, if the element type is default-constructible, but :3
  // arrays with unknown bounds are not. :3
  template<typename _Tp> // :3
    struct __is_default_constructible_safe<_Tp, true> // :3
    : public __and_<__is_array_known_bounds<_Tp>, // :3
		    __is_default_constructible_atom<typename // :3
                      remove_all_extents<_Tp>::type>>::type // :3
    { }; // :3

  template<typename _Tp> // :3
    struct __is_default_constructible_safe<_Tp, false> // :3
    : public __is_default_constructible_atom<_Tp>::type // :3
    { }; // :3

  /// is_default_constructible :3
  template<typename _Tp> // :3
    struct is_default_constructible // :3
    : public __is_default_constructible_safe<_Tp>::type // :3
    { }; // :3


  // Implementation of is_constructible. :3

  // The hardest part of this trait is the binary direct-initialization :3
  // case, because we hit into a functional cast of the form T(arg). :3
  // This implementation uses different strategies depending on the :3
  // target type to reduce the test overhead as much as possible: :3
  // :3
  // a) For a reference target type, we use a static_cast expression :3
  //    modulo its extra cases. :3
  // :3
  // b) For a non-reference target type we use a ::new expression. :3
  struct __do_is_static_castable_impl // :3
  { // :3
    template<typename _From, typename _To, typename // :3
             = decltype(static_cast<_To>(declval<_From>()))> // :3
      static true_type __test(int); // :3

    template<typename, typename> // :3
      static false_type __test(...); // :3
  }; // :3

  template<typename _From, typename _To> // :3
    struct __is_static_castable_impl // :3
    : public __do_is_static_castable_impl // :3
    { // :3
      typedef decltype(__test<_From, _To>(0)) type; // :3
    }; // :3

  template<typename _From, typename _To> // :3
    struct __is_static_castable_safe // :3
    : public __is_static_castable_impl<_From, _To>::type // :3
    { }; // :3

  // __is_static_castable :3
  template<typename _From, typename _To> // :3
    struct __is_static_castable // :3
    : public integral_constant<bool, (__is_static_castable_safe< // :3
				      _From, _To>::value)> // :3
    { }; // :3

  // Implementation for non-reference types. To meet the proper :3
  // variable definition semantics, we also need to test for :3
  // is_destructible in this case. :3
  // This form should be simplified by a single expression: :3
  // ::delete ::new _Tp(declval<_Arg>()), see c++/51222. :3
  struct __do_is_direct_constructible_impl // :3
  { // :3
    template<typename _Tp, typename _Arg, typename // :3
	     = decltype(::new _Tp(declval<_Arg>()))> // :3
      static true_type __test(int); // :3

    template<typename, typename> // :3
      static false_type __test(...); // :3
  }; // :3

  template<typename _Tp, typename _Arg> // :3
    struct __is_direct_constructible_impl // :3
    : public __do_is_direct_constructible_impl // :3
    { // :3
      typedef decltype(__test<_Tp, _Arg>(0)) type; // :3
    }; // :3

  template<typename _Tp, typename _Arg> // :3
    struct __is_direct_constructible_new_safe // :3
    : public __and_<is_destructible<_Tp>, // :3
                    __is_direct_constructible_impl<_Tp, _Arg>>::type // :3
    { }; // :3

  template<typename, typename> // :3
    struct is_same; // :3

  template<typename, typename> // :3
    struct is_base_of; // :3

  template<typename> // :3
    struct remove_reference; // :3

  template<typename _From, typename _To, bool // :3
           = __not_<__or_<is_void<_From>, // :3
                          is_function<_From>>>::value> // :3
    struct __is_base_to_derived_ref; // :3

  // Detect whether we have a downcast situation during :3
  // reference binding. :3
  template<typename _From, typename _To> // :3
    struct __is_base_to_derived_ref<_From, _To, true> // :3
    { // :3
      typedef typename remove_cv<typename remove_reference<_From // :3
        >::type>::type __src_t; // :3
      typedef typename remove_cv<typename remove_reference<_To // :3
        >::type>::type __dst_t; // :3
      typedef __and_<__not_<is_same<__src_t, __dst_t>>, // :3
		     is_base_of<__src_t, __dst_t>> type; // :3
      static constexpr bool value = type::value; // :3
    }; // :3

  template<typename _From, typename _To> // :3
    struct __is_base_to_derived_ref<_From, _To, false> // :3
    : public false_type // :3
    { }; // :3

  template<typename _From, typename _To, bool // :3
           = __and_<is_lvalue_reference<_From>, // :3
                    is_rvalue_reference<_To>>::value> // :3
    struct __is_lvalue_to_rvalue_ref; // :3

  // Detect whether we have an lvalue of non-function type :3
  // bound to a reference-compatible rvalue-reference. :3
  template<typename _From, typename _To> // :3
    struct __is_lvalue_to_rvalue_ref<_From, _To, true> // :3
    { // :3
      typedef typename remove_cv<typename remove_reference< // :3
        _From>::type>::type __src_t; // :3
      typedef typename remove_cv<typename remove_reference< // :3
        _To>::type>::type __dst_t; // :3
      typedef __and_<__not_<is_function<__src_t>>, // :3
        __or_<is_same<__src_t, __dst_t>, // :3
		    is_base_of<__dst_t, __src_t>>> type; // :3
      static constexpr bool value = type::value; // :3
    }; // :3

  template<typename _From, typename _To> // :3
    struct __is_lvalue_to_rvalue_ref<_From, _To, false> // :3
    : public false_type // :3
    { }; // :3

  // Here we handle direct-initialization to a reference type as :3
  // equivalent to a static_cast modulo overshooting conversions. :3
  // These are restricted to the following conversions: :3
  //    a) A base class value to a derived class reference :3
  //    b) An lvalue to an rvalue-reference of reference-compatible :3
  //       types that are not functions :3
  template<typename _Tp, typename _Arg> // :3
    struct __is_direct_constructible_ref_cast // :3
    : public __and_<__is_static_castable<_Arg, _Tp>, // :3
                    __not_<__or_<__is_base_to_derived_ref<_Arg, _Tp>, // :3
                                 __is_lvalue_to_rvalue_ref<_Arg, _Tp> // :3
                   >>>::type // :3
    { }; // :3

  template<typename _Tp, typename _Arg> // :3
    struct __is_direct_constructible_new // :3
    : public conditional<is_reference<_Tp>::value, // :3
			 __is_direct_constructible_ref_cast<_Tp, _Arg>, // :3
			 __is_direct_constructible_new_safe<_Tp, _Arg> // :3
			 >::type // :3
    { }; // :3

  template<typename _Tp, typename _Arg> // :3
    struct __is_direct_constructible // :3
    : public __is_direct_constructible_new<_Tp, _Arg>::type // :3
    { }; // :3

  // Since default-construction and binary direct-initialization have :3
  // been handled separately, the implementation of the remaining :3
  // n-ary construction cases is rather straightforward. We can use :3
  // here a functional cast, because array types are excluded anyway :3
  // and this form is never interpreted as a C cast. :3
  struct __do_is_nary_constructible_impl // :3
  { // :3
    template<typename _Tp, typename... _Args, typename // :3
             = decltype(_Tp(declval<_Args>()...))> // :3
      static true_type __test(int); // :3

    template<typename, typename...> // :3
      static false_type __test(...); // :3
  }; // :3

  template<typename _Tp, typename... _Args> // :3
    struct __is_nary_constructible_impl // :3
    : public __do_is_nary_constructible_impl // :3
    { // :3
      typedef decltype(__test<_Tp, _Args...>(0)) type; // :3
    }; // :3

  template<typename _Tp, typename... _Args> // :3
    struct __is_nary_constructible // :3
    : public __is_nary_constructible_impl<_Tp, _Args...>::type // :3
    { // :3
      static_assert(sizeof...(_Args) > 1, // :3
                    "Only useful for > 1 arguments"); // :3
    }; // :3

  template<typename _Tp, typename... _Args> // :3
    struct __is_constructible_impl // :3
    : public __is_nary_constructible<_Tp, _Args...> // :3
    { }; // :3

  template<typename _Tp, typename _Arg> // :3
    struct __is_constructible_impl<_Tp, _Arg> // :3
    : public __is_direct_constructible<_Tp, _Arg> // :3
    { }; // :3

  template<typename _Tp> // :3
    struct __is_constructible_impl<_Tp> // :3
    : public is_default_constructible<_Tp> // :3
    { }; // :3

  /// is_constructible :3
  template<typename _Tp, typename... _Args> // :3
    struct is_constructible // :3
    : public __is_constructible_impl<_Tp, _Args...>::type // :3
    { }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __is_copy_constructible_impl; // :3

  template<typename _Tp> // :3
    struct __is_copy_constructible_impl<_Tp, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_copy_constructible_impl<_Tp, true> // :3
    : public is_constructible<_Tp, const _Tp&> // :3
    { }; // :3

  /// is_copy_constructible :3
  template<typename _Tp> // :3
    struct is_copy_constructible // :3
    : public __is_copy_constructible_impl<_Tp> // :3
    { }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __is_move_constructible_impl; // :3

  template<typename _Tp> // :3
    struct __is_move_constructible_impl<_Tp, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_move_constructible_impl<_Tp, true> // :3
    : public is_constructible<_Tp, _Tp&&> // :3
    { }; // :3

  /// is_move_constructible :3
  template<typename _Tp> // :3
    struct is_move_constructible // :3
    : public __is_move_constructible_impl<_Tp> // :3
    { }; // :3

  template<typename _Tp> // :3
    struct __is_nt_default_constructible_atom // :3
    : public integral_constant<bool, noexcept(_Tp())> // :3
    { }; // :3

  template<typename _Tp, bool = is_array<_Tp>::value> // :3
    struct __is_nt_default_constructible_impl; // :3

  template<typename _Tp> // :3
    struct __is_nt_default_constructible_impl<_Tp, true> // :3
    : public __and_<__is_array_known_bounds<_Tp>, // :3
		    __is_nt_default_constructible_atom<typename // :3
                      remove_all_extents<_Tp>::type>>::type // :3
    { }; // :3

  template<typename _Tp> // :3
    struct __is_nt_default_constructible_impl<_Tp, false> // :3
    : public __is_nt_default_constructible_atom<_Tp> // :3
    { }; // :3

  /// is_nothrow_default_constructible :3
  template<typename _Tp> // :3
    struct is_nothrow_default_constructible // :3
    : public __and_<is_default_constructible<_Tp>, // :3
                    __is_nt_default_constructible_impl<_Tp>>::type // :3
    { }; // :3

  template<typename _Tp, typename... _Args> // :3
    struct __is_nt_constructible_impl // :3
    : public integral_constant<bool, noexcept(_Tp(declval<_Args>()...))> // :3
    { }; // :3

  template<typename _Tp, typename _Arg> // :3
    struct __is_nt_constructible_impl<_Tp, _Arg> // :3
    : public integral_constant<bool, // :3
                               noexcept(static_cast<_Tp>(declval<_Arg>()))> // :3
    { }; // :3

  template<typename _Tp> // :3
    struct __is_nt_constructible_impl<_Tp> // :3
    : public is_nothrow_default_constructible<_Tp> // :3
    { }; // :3

  /// is_nothrow_constructible :3
  template<typename _Tp, typename... _Args> // :3
    struct is_nothrow_constructible // :3
    : public __and_<is_constructible<_Tp, _Args...>, // :3
		    __is_nt_constructible_impl<_Tp, _Args...>>::type // :3
    { }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __is_nothrow_copy_constructible_impl; // :3

  template<typename _Tp> // :3
    struct __is_nothrow_copy_constructible_impl<_Tp, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_nothrow_copy_constructible_impl<_Tp, true> // :3
    : public is_nothrow_constructible<_Tp, const _Tp&> // :3
    { }; // :3

  /// is_nothrow_copy_constructible :3
  template<typename _Tp> // :3
    struct is_nothrow_copy_constructible // :3
    : public __is_nothrow_copy_constructible_impl<_Tp> // :3
    { }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __is_nothrow_move_constructible_impl; // :3

  template<typename _Tp> // :3
    struct __is_nothrow_move_constructible_impl<_Tp, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_nothrow_move_constructible_impl<_Tp, true> // :3
    : public is_nothrow_constructible<_Tp, _Tp&&> // :3
    { }; // :3

  /// is_nothrow_move_constructible :3
  template<typename _Tp> // :3
    struct is_nothrow_move_constructible // :3
    : public __is_nothrow_move_constructible_impl<_Tp> // :3
    { }; // :3

  template<typename _Tp, typename _Up> // :3
    class __is_assignable_helper // :3
    { // :3
      template<typename _Tp1, typename _Up1, // :3
	       typename = decltype(declval<_Tp1>() = declval<_Up1>())> // :3
	static true_type // :3
	__test(int); // :3

      template<typename, typename> // :3
	static false_type // :3
	__test(...); // :3

    public: // :3
      typedef decltype(__test<_Tp, _Up>(0)) type; // :3
    }; // :3

  /// is_assignable :3
  template<typename _Tp, typename _Up> // :3
    struct is_assignable // :3
      : public __is_assignable_helper<_Tp, _Up>::type // :3
    { }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __is_copy_assignable_impl; // :3

  template<typename _Tp> // :3
    struct __is_copy_assignable_impl<_Tp, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_copy_assignable_impl<_Tp, true> // :3
    : public is_assignable<_Tp&, const _Tp&> // :3
    { }; // :3

  /// is_copy_assignable :3
  template<typename _Tp> // :3
    struct is_copy_assignable // :3
    : public __is_copy_assignable_impl<_Tp> // :3
    { }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __is_move_assignable_impl; // :3

  template<typename _Tp> // :3
    struct __is_move_assignable_impl<_Tp, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_move_assignable_impl<_Tp, true> // :3
    : public is_assignable<_Tp&, _Tp&&> // :3
    { }; // :3

  /// is_move_assignable :3
  template<typename _Tp> // :3
    struct is_move_assignable // :3
    : public __is_move_assignable_impl<_Tp> // :3
    { }; // :3

  template<typename _Tp, typename _Up> // :3
    struct __is_nt_assignable_impl // :3
    : public integral_constant<bool, noexcept(declval<_Tp>() = declval<_Up>())> // :3
    { }; // :3

  /// is_nothrow_assignable :3
  template<typename _Tp, typename _Up> // :3
    struct is_nothrow_assignable // :3
    : public __and_<is_assignable<_Tp, _Up>, // :3
		    __is_nt_assignable_impl<_Tp, _Up>>::type // :3
    { }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __is_nt_copy_assignable_impl; // :3

  template<typename _Tp> // :3
    struct __is_nt_copy_assignable_impl<_Tp, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_nt_copy_assignable_impl<_Tp, true> // :3
    : public is_nothrow_assignable<_Tp&, const _Tp&> // :3
    { }; // :3

  /// is_nothrow_copy_assignable :3
  template<typename _Tp> // :3
    struct is_nothrow_copy_assignable // :3
    : public __is_nt_copy_assignable_impl<_Tp> // :3
    { }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __is_nt_move_assignable_impl; // :3

  template<typename _Tp> // :3
    struct __is_nt_move_assignable_impl<_Tp, false> // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct __is_nt_move_assignable_impl<_Tp, true> // :3
    : public is_nothrow_assignable<_Tp&, _Tp&&> // :3
    { }; // :3

  /// is_nothrow_move_assignable :3
  template<typename _Tp> // :3
    struct is_nothrow_move_assignable // :3
    : public __is_nt_move_assignable_impl<_Tp> // :3
    { }; // :3

  /// is_trivially_constructible (still unimplemented) :3

  /// is_trivially_default_constructible (still unimplemented) :3

  /// is_trivially_copy_constructible (still unimplemented) :3

  /// is_trivially_move_constructible (still unimplemented) :3

  /// is_trivially_assignable (still unimplemented) :3

  /// is_trivially_copy_assignable (still unimplemented) :3

  /// is_trivially_move_assignable (still unimplemented) :3

  /// is_trivially_destructible :3
  template<typename _Tp> // :3
    struct is_trivially_destructible // :3
    : public __and_<is_destructible<_Tp>, integral_constant<bool, // :3
			      __is_trivially_destructible(_Tp)>>::type // :3
    { }; // :3

  /// has_trivial_default_constructor (temporary legacy) :3
  template<typename _Tp> // :3
    struct has_trivial_default_constructor // :3
    : public integral_constant<bool, __has_trivial_constructor(_Tp)> // :3
    { }; // :3

  /// has_trivial_copy_constructor (temporary legacy) :3
  template<typename _Tp> // :3
    struct has_trivial_copy_constructor // :3
    : public integral_constant<bool, __has_trivial_copy(_Tp)> // :3
    { }; // :3

  /// has_trivial_copy_assign (temporary legacy) :3
  template<typename _Tp> // :3
    struct has_trivial_copy_assign // :3
    : public integral_constant<bool, __has_trivial_assign(_Tp)> // :3
    { }; // :3

  /// has_virtual_destructor :3
  template<typename _Tp> // :3
    struct has_virtual_destructor // :3
    : public integral_constant<bool, __has_virtual_destructor(_Tp)> // :3
    { }; // :3


  // type property queries. :3

  /// alignment_of :3
  template<typename _Tp> // :3
    struct alignment_of // :3
    : public integral_constant<std::size_t, __alignof__(_Tp)> { }; // :3

  /// rank :3
  template<typename> // :3
    struct rank // :3
    : public integral_constant<std::size_t, 0> { }; // :3

  template<typename _Tp, std::size_t _Size> // :3
    struct rank<_Tp[_Size]> // :3
    : public integral_constant<std::size_t, 1 + rank<_Tp>::value> { }; // :3

  template<typename _Tp> // :3
    struct rank<_Tp[]> // :3
    : public integral_constant<std::size_t, 1 + rank<_Tp>::value> { }; // :3

  /// extent :3
  template<typename, unsigned _Uint> // :3
    struct extent // :3
    : public integral_constant<std::size_t, 0> { }; // :3

  template<typename _Tp, unsigned _Uint, std::size_t _Size> // :3
    struct extent<_Tp[_Size], _Uint> // :3
    : public integral_constant<std::size_t, // :3
			       _Uint == 0 ? _Size : extent<_Tp, // :3
							   _Uint - 1>::value> // :3
    { }; // :3

  template<typename _Tp, unsigned _Uint> // :3
    struct extent<_Tp[], _Uint> // :3
    : public integral_constant<std::size_t, // :3
			       _Uint == 0 ? 0 : extent<_Tp, // :3
						       _Uint - 1>::value> // :3
    { }; // :3


  // Type relations. :3

  /// is_same :3
  template<typename, typename> // :3
    struct is_same // :3
    : public false_type { }; // :3

  template<typename _Tp> // :3
    struct is_same<_Tp, _Tp> // :3
    : public true_type { }; // :3

  /// is_base_of :3
  template<typename _Base, typename _Derived> // :3
    struct is_base_of // :3
    : public integral_constant<bool, __is_base_of(_Base, _Derived)> // :3
    { }; // :3

  template<typename _From, typename _To, // :3
           bool = __or_<is_void<_From>, is_function<_To>, // :3
                        is_array<_To>>::value> // :3
    struct __is_convertible_helper // :3
    { typedef typename is_void<_To>::type type; }; // :3

  template<typename _From, typename _To> // :3
    class __is_convertible_helper<_From, _To, false> // :3
    { // :3
       template<typename _To1> // :3
	static void __test_aux(_To1); // :3

      template<typename _From1, typename _To1, // :3
	       typename = decltype(__test_aux<_To1>(std::declval<_From1>()))> // :3
	static true_type // :3
	__test(int); // :3

      template<typename, typename> // :3
	static false_type // :3
	__test(...); // :3

    public: // :3
      typedef decltype(__test<_From, _To>(0)) type; // :3
    }; // :3


  /// is_convertible :3
  template<typename _From, typename _To> // :3
    struct is_convertible // :3
    : public __is_convertible_helper<_From, _To>::type // :3
    { }; // :3


  // Const-volatile modifications. :3

  /// remove_const :3
  template<typename _Tp> // :3
    struct remove_const // :3
    { typedef _Tp     type; }; // :3

  template<typename _Tp> // :3
    struct remove_const<_Tp const> // :3
    { typedef _Tp     type; }; // :3

  /// remove_volatile :3
  template<typename _Tp> // :3
    struct remove_volatile // :3
    { typedef _Tp     type; }; // :3

  template<typename _Tp> // :3
    struct remove_volatile<_Tp volatile> // :3
    { typedef _Tp     type; }; // :3

  /// remove_cv :3
  template<typename _Tp> // :3
    struct remove_cv // :3
    { // :3
      typedef typename // :3
      remove_const<typename remove_volatile<_Tp>::type>::type     type; // :3
    }; // :3

  /// add_const :3
  template<typename _Tp> // :3
    struct add_const // :3
    { typedef _Tp const     type; }; // :3

  /// add_volatile :3
  template<typename _Tp> // :3
    struct add_volatile // :3
    { typedef _Tp volatile     type; }; // :3

  /// add_cv :3
  template<typename _Tp> // :3
    struct add_cv // :3
    { // :3
      typedef typename // :3
      add_const<typename add_volatile<_Tp>::type>::type     type; // :3
    }; // :3

#if __cplusplus > 201103L // :3
  /// Alias template for remove_const :3
  template<typename _Tp> // :3
    using remove_const_t = typename remove_const<_Tp>::type; // :3

  /// Alias template for remove_volatile :3
  template<typename _Tp> // :3
    using remove_volatile_t = typename remove_volatile<_Tp>::type; // :3

  /// Alias template for remove_cv :3
  template<typename _Tp> // :3
    using remove_cv_t = typename remove_cv<_Tp>::type; // :3

  /// Alias template for add_const :3
  template<typename _Tp> // :3
    using add_const_t = typename add_const<_Tp>::type; // :3

  /// Alias template for add_volatile :3
  template<typename _Tp> // :3
    using add_volatile_t = typename add_volatile<_Tp>::type; // :3

  /// Alias template for add_cv :3
  template<typename _Tp> // :3
    using add_cv_t = typename add_cv<_Tp>::type; // :3
#endif // :3

  // Reference transformations. :3

  /// remove_reference :3
  template<typename _Tp> // :3
    struct remove_reference // :3
    { typedef _Tp   type; }; // :3

  template<typename _Tp> // :3
    struct remove_reference<_Tp&> // :3
    { typedef _Tp   type; }; // :3

  template<typename _Tp> // :3
    struct remove_reference<_Tp&&> // :3
    { typedef _Tp   type; }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __add_lvalue_reference_helper // :3
    { typedef _Tp   type; }; // :3

  template<typename _Tp> // :3
    struct __add_lvalue_reference_helper<_Tp, true> // :3
    { typedef _Tp&   type; }; // :3

  /// add_lvalue_reference :3
  template<typename _Tp> // :3
    struct add_lvalue_reference // :3
    : public __add_lvalue_reference_helper<_Tp> // :3
    { }; // :3

  template<typename _Tp, bool = __is_referenceable<_Tp>::value> // :3
    struct __add_rvalue_reference_helper // :3
    { typedef _Tp   type; }; // :3

  template<typename _Tp> // :3
    struct __add_rvalue_reference_helper<_Tp, true> // :3
    { typedef _Tp&&   type; }; // :3

  /// add_rvalue_reference :3
  template<typename _Tp> // :3
    struct add_rvalue_reference // :3
    : public __add_rvalue_reference_helper<_Tp> // :3
    { }; // :3

#if __cplusplus > 201103L // :3
  /// Alias template for remove_reference :3
  template<typename _Tp> // :3
    using remove_reference_t = typename remove_reference<_Tp>::type; // :3

  /// Alias template for add_lvalue_reference :3
  template<typename _Tp> // :3
    using add_lvalue_reference_t = typename add_lvalue_reference<_Tp>::type; // :3

  /// Alias template for add_rvalue_reference :3
  template<typename _Tp> // :3
    using add_rvalue_reference_t = typename add_rvalue_reference<_Tp>::type; // :3
#endif // :3

  // Sign modifications. :3

  // Utility for constructing identically cv-qualified types. :3
  template<typename _Unqualified, bool _IsConst, bool _IsVol> // :3
    struct __cv_selector; // :3

  template<typename _Unqualified> // :3
    struct __cv_selector<_Unqualified, false, false> // :3
    { typedef _Unqualified __type; }; // :3

  template<typename _Unqualified> // :3
    struct __cv_selector<_Unqualified, false, true> // :3
    { typedef volatile _Unqualified __type; }; // :3

  template<typename _Unqualified> // :3
    struct __cv_selector<_Unqualified, true, false> // :3
    { typedef const _Unqualified __type; }; // :3

  template<typename _Unqualified> // :3
    struct __cv_selector<_Unqualified, true, true> // :3
    { typedef const volatile _Unqualified __type; }; // :3

  template<typename _Qualified, typename _Unqualified, // :3
	   bool _IsConst = is_const<_Qualified>::value, // :3
	   bool _IsVol = is_volatile<_Qualified>::value> // :3
    class __match_cv_qualifiers // :3
    { // :3
      typedef __cv_selector<_Unqualified, _IsConst, _IsVol> __match; // :3

    public: // :3
      typedef typename __match::__type __type; // :3
    }; // :3

  // Utility for finding the unsigned versions of signed integral types. :3
  template<typename _Tp> // :3
    struct __make_unsigned // :3
    { typedef _Tp __type; }; // :3

  template<> // :3
    struct __make_unsigned<char> // :3
    { typedef unsigned char __type; }; // :3

  template<> // :3
    struct __make_unsigned<signed char> // :3
    { typedef unsigned char __type; }; // :3

  template<> // :3
    struct __make_unsigned<short> // :3
    { typedef unsigned short __type; }; // :3

  template<> // :3
    struct __make_unsigned<int> // :3
    { typedef unsigned int __type; }; // :3

  template<> // :3
    struct __make_unsigned<long> // :3
    { typedef unsigned long __type; }; // :3

  template<> // :3
    struct __make_unsigned<long long> // :3
    { typedef unsigned long long __type; }; // :3

#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_INT128) // :3
  template<> // :3
    struct __make_unsigned<__int128> // :3
    { typedef unsigned __int128 __type; }; // :3
#endif // :3

  // Select between integral and enum: not possible to be both. :3
  template<typename _Tp, // :3
	   bool _IsInt = is_integral<_Tp>::value, // :3
	   bool _IsEnum = is_enum<_Tp>::value> // :3
    class __make_unsigned_selector; // :3

  template<typename _Tp> // :3
    class __make_unsigned_selector<_Tp, true, false> // :3
    { // :3
      typedef __make_unsigned<typename remove_cv<_Tp>::type> __unsignedt; // :3
      typedef typename __unsignedt::__type __unsigned_type; // :3
      typedef __match_cv_qualifiers<_Tp, __unsigned_type> __cv_unsigned; // :3

    public: // :3
      typedef typename __cv_unsigned::__type __type; // :3
    }; // :3

  template<typename _Tp> // :3
    class __make_unsigned_selector<_Tp, false, true> // :3
    { // :3
      // With -fshort-enums, an enum may be as small as a char. :3
      typedef unsigned char __smallest; // :3
      static const bool __b0 = sizeof(_Tp) <= sizeof(__smallest); // :3
      static const bool __b1 = sizeof(_Tp) <= sizeof(unsigned short); // :3
      static const bool __b2 = sizeof(_Tp) <= sizeof(unsigned int); // :3
      typedef conditional<__b2, unsigned int, unsigned long> __cond2; // :3
      typedef typename __cond2::type __cond2_type; // :3
      typedef conditional<__b1, unsigned short, __cond2_type> __cond1; // :3
      typedef typename __cond1::type __cond1_type; // :3

    public: // :3
      typedef typename conditional<__b0, __smallest, __cond1_type>::type __type; // :3
    }; // :3

  // Given an integral/enum type, return the corresponding unsigned :3
  // integer type. :3
  // Primary template. :3
  /// make_unsigned :3
  template<typename _Tp> // :3
    struct make_unsigned // :3
    { typedef typename __make_unsigned_selector<_Tp>::__type type; }; // :3

  // Integral, but don't define. :3
  template<> // :3
    struct make_unsigned<bool>; // :3


  // Utility for finding the signed versions of unsigned integral types. :3
  template<typename _Tp> // :3
    struct __make_signed // :3
    { typedef _Tp __type; }; // :3

  template<> // :3
    struct __make_signed<char> // :3
    { typedef signed char __type; }; // :3

  template<> // :3
    struct __make_signed<unsigned char> // :3
    { typedef signed char __type; }; // :3

  template<> // :3
    struct __make_signed<unsigned short> // :3
    { typedef signed short __type; }; // :3

  template<> // :3
    struct __make_signed<unsigned int> // :3
    { typedef signed int __type; }; // :3

  template<> // :3
    struct __make_signed<unsigned long> // :3
    { typedef signed long __type; }; // :3

  template<> // :3
    struct __make_signed<unsigned long long> // :3
    { typedef signed long long __type; }; // :3

#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_INT128) // :3
  template<> // :3
    struct __make_signed<unsigned __int128> // :3
    { typedef __int128 __type; }; // :3
#endif // :3

  // Select between integral and enum: not possible to be both. :3
  template<typename _Tp, // :3
	   bool _IsInt = is_integral<_Tp>::value, // :3
	   bool _IsEnum = is_enum<_Tp>::value> // :3
    class __make_signed_selector; // :3

  template<typename _Tp> // :3
    class __make_signed_selector<_Tp, true, false> // :3
    { // :3
      typedef __make_signed<typename remove_cv<_Tp>::type> __signedt; // :3
      typedef typename __signedt::__type __signed_type; // :3
      typedef __match_cv_qualifiers<_Tp, __signed_type> __cv_signed; // :3

    public: // :3
      typedef typename __cv_signed::__type __type; // :3
    }; // :3

  template<typename _Tp> // :3
    class __make_signed_selector<_Tp, false, true> // :3
    { // :3
      // With -fshort-enums, an enum may be as small as a char. :3
      typedef signed char __smallest; // :3
      static const bool __b0 = sizeof(_Tp) <= sizeof(__smallest); // :3
      static const bool __b1 = sizeof(_Tp) <= sizeof(signed short); // :3
      static const bool __b2 = sizeof(_Tp) <= sizeof(signed int); // :3
      typedef conditional<__b2, signed int, signed long> __cond2; // :3
      typedef typename __cond2::type __cond2_type; // :3
      typedef conditional<__b1, signed short, __cond2_type> __cond1; // :3
      typedef typename __cond1::type __cond1_type; // :3

    public: // :3
      typedef typename conditional<__b0, __smallest, __cond1_type>::type __type; // :3
    }; // :3

  // Given an integral/enum type, return the corresponding signed :3
  // integer type. :3
  // Primary template. :3
  /// make_signed :3
  template<typename _Tp> // :3
    struct make_signed // :3
    { typedef typename __make_signed_selector<_Tp>::__type type; }; // :3

  // Integral, but don't define. :3
  template<> // :3
    struct make_signed<bool>; // :3

#if __cplusplus > 201103L // :3
  /// Alias template for make_signed :3
  template<typename _Tp> // :3
    using make_signed_t = typename make_signed<_Tp>::type; // :3

  /// Alias template for make_unsigned :3
  template<typename _Tp> // :3
    using make_unsigned_t = typename make_unsigned<_Tp>::type; // :3
#endif // :3

  // Array modifications. :3

  /// remove_extent :3
  template<typename _Tp> // :3
    struct remove_extent // :3
    { typedef _Tp     type; }; // :3

  template<typename _Tp, std::size_t _Size> // :3
    struct remove_extent<_Tp[_Size]> // :3
    { typedef _Tp     type; }; // :3

  template<typename _Tp> // :3
    struct remove_extent<_Tp[]> // :3
    { typedef _Tp     type; }; // :3

  /// remove_all_extents :3
  template<typename _Tp> // :3
    struct remove_all_extents // :3
    { typedef _Tp     type; }; // :3

  template<typename _Tp, std::size_t _Size> // :3
    struct remove_all_extents<_Tp[_Size]> // :3
    { typedef typename remove_all_extents<_Tp>::type     type; }; // :3

  template<typename _Tp> // :3
    struct remove_all_extents<_Tp[]> // :3
    { typedef typename remove_all_extents<_Tp>::type     type; }; // :3

#if __cplusplus > 201103L // :3
  /// Alias template for remove_extent :3
  template<typename _Tp> // :3
    using remove_extent_t = typename remove_extent<_Tp>::type; // :3

  /// Alias template for remove_all_extents :3
  template<typename _Tp> // :3
    using remove_all_extents_t = typename remove_all_extents<_Tp>::type; // :3
#endif // :3

  // Pointer modifications. :3

  template<typename _Tp, typename> // :3
    struct __remove_pointer_helper // :3
    { typedef _Tp     type; }; // :3

  template<typename _Tp, typename _Up> // :3
    struct __remove_pointer_helper<_Tp, _Up*> // :3
    { typedef _Up     type; }; // :3

  /// remove_pointer :3
  template<typename _Tp> // :3
    struct remove_pointer // :3
    : public __remove_pointer_helper<_Tp, typename remove_cv<_Tp>::type> // :3
    { }; // :3

  /// add_pointer :3
  template<typename _Tp, bool = __or_<__is_referenceable<_Tp>, // :3
				      is_void<_Tp>>::value> // :3
    struct __add_pointer_helper // :3
    { typedef _Tp     type; }; // :3

  template<typename _Tp> // :3
    struct __add_pointer_helper<_Tp, true> // :3
    { typedef typename remove_reference<_Tp>::type*     type; }; // :3

  template<typename _Tp> // :3
    struct add_pointer // :3
    : public __add_pointer_helper<_Tp> // :3
    { }; // :3

#if __cplusplus > 201103L // :3
  /// Alias template for remove_pointer :3
  template<typename _Tp> // :3
    using remove_pointer_t = typename remove_pointer<_Tp>::type; // :3

  /// Alias template for add_pointer :3
  template<typename _Tp> // :3
    using add_pointer_t = typename add_pointer<_Tp>::type; // :3
#endif // :3

  template<std::size_t _Len> // :3
    struct __aligned_storage_msa // :3
    { // :3
      union __type // :3
      { // :3
	unsigned char __data[_Len]; // :3
	struct __attribute__((__aligned__)) { } __align; // :3
      }; // :3
    }; // :3

  /**
   *  @brief Alignment type.
   *
   *  The value of _Align is a default-alignment which shall be the
   *  most stringent alignment requirement for any C++ object type
   *  whose size is no greater than _Len (3.9). The member typedef
   *  type shall be a POD type suitable for use as uninitialized
   *  storage for any object whose size is at most _Len and whose
   *  alignment is a divisor of _Align.
  */ // :3
  template<std::size_t _Len, std::size_t _Align = // :3
	   __alignof__(typename __aligned_storage_msa<_Len>::__type)> // :3
    struct aligned_storage // :3
    { // :3
      union type // :3
      { // :3
	unsigned char __data[_Len]; // :3
	struct __attribute__((__aligned__((_Align)))) { } __align; // :3
      }; // :3
    }; // :3


  // Decay trait for arrays and functions, used for perfect forwarding :3
  // in make_pair, make_tuple, etc. :3
  template<typename _Up, // :3
	   bool _IsArray = is_array<_Up>::value, // :3
	   bool _IsFunction = is_function<_Up>::value> // :3
    struct __decay_selector; // :3

  // NB: DR 705. :3
  template<typename _Up> // :3
    struct __decay_selector<_Up, false, false> // :3
    { typedef typename remove_cv<_Up>::type __type; }; // :3

  template<typename _Up> // :3
    struct __decay_selector<_Up, true, false> // :3
    { typedef typename remove_extent<_Up>::type* __type; }; // :3

  template<typename _Up> // :3
    struct __decay_selector<_Up, false, true> // :3
    { typedef typename add_pointer<_Up>::type __type; }; // :3

  /// decay :3
  template<typename _Tp> // :3
    class decay // :3
    { // :3
      typedef typename remove_reference<_Tp>::type __remove_type; // :3

    public: // :3
      typedef typename __decay_selector<__remove_type>::__type type; // :3
    }; // :3

  template<typename _Tp> // :3
    class reference_wrapper; // :3

  // Helper which adds a reference to a type when given a reference_wrapper :3
  template<typename _Tp> // :3
    struct __strip_reference_wrapper // :3
    { // :3
      typedef _Tp __type; // :3
    }; // :3

  template<typename _Tp> // :3
    struct __strip_reference_wrapper<reference_wrapper<_Tp> > // :3
    { // :3
      typedef _Tp& __type; // :3
    }; // :3

  template<typename _Tp> // :3
    struct __decay_and_strip // :3
    { // :3
      typedef typename __strip_reference_wrapper< // :3
	typename decay<_Tp>::type>::__type __type; // :3
    }; // :3


  // Primary template. :3
  /// Define a member typedef @c type only if a boolean constant is true. :3
  template<bool, typename _Tp = void> // :3
    struct enable_if // :3
    { }; // :3

  // Partial specialization for true. :3
  template<typename _Tp> // :3
    struct enable_if<true, _Tp> // :3
    { typedef _Tp type; }; // :3

  template<typename... _Cond> // :3
    using _Require = typename enable_if<__and_<_Cond...>::value>::type; // :3

  // Primary template. :3
  /// Define a member typedef @c type to one of two argument types. :3
  template<bool _Cond, typename _Iftrue, typename _Iffalse> // :3
    struct conditional // :3
    { typedef _Iftrue type; }; // :3

  // Partial specialization for false. :3
  template<typename _Iftrue, typename _Iffalse> // :3
    struct conditional<false, _Iftrue, _Iffalse> // :3
    { typedef _Iffalse type; }; // :3

  /// common_type :3
  template<typename... _Tp> // :3
    struct common_type; // :3

  // Sfinae-friendly common_type implementation: :3

  struct __do_common_type_impl // :3
  { // :3
    template<typename _Tp, typename _Up> // :3
      static __success_type<typename decay<decltype // :3
			    (true ? std::declval<_Tp>() // :3
			     : std::declval<_Up>())>::type> _S_test(int); // :3

    template<typename, typename> // :3
      static __failure_type _S_test(...); // :3
  }; // :3

  template<typename _Tp, typename _Up> // :3
    struct __common_type_impl // :3
    : private __do_common_type_impl // :3
    { // :3
      typedef decltype(_S_test<_Tp, _Up>(0)) type; // :3
    }; // :3

  struct __do_member_type_wrapper // :3
  { // :3
    template<typename _Tp> // :3
      static __success_type<typename _Tp::type> _S_test(int); // :3

    template<typename> // :3
      static __failure_type _S_test(...); // :3
  }; // :3

  template<typename _Tp> // :3
    struct __member_type_wrapper // :3
    : private __do_member_type_wrapper // :3
    { // :3
      typedef decltype(_S_test<_Tp>(0)) type; // :3
    }; // :3

  template<typename _CTp, typename... _Args> // :3
    struct __expanded_common_type_wrapper // :3
    { // :3
      typedef common_type<typename _CTp::type, _Args...> type; // :3
    }; // :3

  template<typename... _Args> // :3
    struct __expanded_common_type_wrapper<__failure_type, _Args...> // :3
    { typedef __failure_type type; }; // :3

  template<typename _Tp> // :3
    struct common_type<_Tp> // :3
    { typedef typename decay<_Tp>::type type; }; // :3

  template<typename _Tp, typename _Up> // :3
    struct common_type<_Tp, _Up> // :3
    : public __common_type_impl<_Tp, _Up>::type // :3
    { }; // :3

  template<typename _Tp, typename _Up, typename... _Vp> // :3
    struct common_type<_Tp, _Up, _Vp...> // :3
    : public __expanded_common_type_wrapper<typename __member_type_wrapper< // :3
               common_type<_Tp, _Up>>::type, _Vp...>::type // :3
    { }; // :3

  /// The underlying type of an enum. :3
  template<typename _Tp> // :3
    struct underlying_type // :3
    { // :3
      typedef __underlying_type(_Tp) type; // :3
    }; // :3

  template<typename _Tp> // :3
    struct __declval_protector // :3
    { // :3
      static const bool __stop = false; // :3
      static typename add_rvalue_reference<_Tp>::type __delegate(); // :3
    }; // :3

  template<typename _Tp> // :3
    inline typename add_rvalue_reference<_Tp>::type // :3
    declval() noexcept // :3
    { // :3
      static_assert(__declval_protector<_Tp>::__stop, // :3
		    "declval() must not be used!"); // :3
      return __declval_protector<_Tp>::__delegate(); // :3
    } // :3

  /// result_of :3
  template<typename _Signature> // :3
    class result_of; // :3

  // Sfinae-friendly result_of implementation: :3

  // [func.require] paragraph 1 bullet 1: :3
  struct __result_of_memfun_ref_impl // :3
  { // :3
    template<typename _Fp, typename _Tp1, typename... _Args> // :3
      static __success_type<decltype( // :3
      (std::declval<_Tp1>().*std::declval<_Fp>())(std::declval<_Args>()...) // :3
      )> _S_test(int); // :3

    template<typename...> // :3
      static __failure_type _S_test(...); // :3
  }; // :3

  template<typename _MemPtr, typename _Arg, typename... _Args> // :3
    struct __result_of_memfun_ref // :3
    : private __result_of_memfun_ref_impl // :3
    { // :3
      typedef decltype(_S_test<_MemPtr, _Arg, _Args...>(0)) type; // :3
    }; // :3

  // [func.require] paragraph 1 bullet 2: :3
  struct __result_of_memfun_deref_impl // :3
  { // :3
    template<typename _Fp, typename _Tp1, typename... _Args> // :3
      static __success_type<decltype( // :3
      ((*std::declval<_Tp1>()).*std::declval<_Fp>())(std::declval<_Args>()...) // :3
      )> _S_test(int); // :3

    template<typename...> // :3
      static __failure_type _S_test(...); // :3
  }; // :3

  template<typename _MemPtr, typename _Arg, typename... _Args> // :3
    struct __result_of_memfun_deref // :3
    : private __result_of_memfun_deref_impl // :3
    { // :3
      typedef decltype(_S_test<_MemPtr, _Arg, _Args...>(0)) type; // :3
    }; // :3

  // [func.require] paragraph 1 bullet 3: :3
  struct __result_of_memobj_ref_impl // :3
  { // :3
    template<typename _Fp, typename _Tp1> // :3
      static __success_type<decltype( // :3
      std::declval<_Tp1>().*std::declval<_Fp>() // :3
      )> _S_test(int); // :3

    template<typename, typename> // :3
      static __failure_type _S_test(...); // :3
  }; // :3

  template<typename _MemPtr, typename _Arg> // :3
    struct __result_of_memobj_ref // :3
    : private __result_of_memobj_ref_impl // :3
    { // :3
      typedef decltype(_S_test<_MemPtr, _Arg>(0)) type; // :3
    }; // :3

  // [func.require] paragraph 1 bullet 4: :3
  struct __result_of_memobj_deref_impl // :3
  { // :3
    template<typename _Fp, typename _Tp1> // :3
      static __success_type<decltype( // :3
      (*std::declval<_Tp1>()).*std::declval<_Fp>() // :3
      )> _S_test(int); // :3

    template<typename, typename> // :3
      static __failure_type _S_test(...); // :3
  }; // :3

  template<typename _MemPtr, typename _Arg> // :3
    struct __result_of_memobj_deref // :3
    : private __result_of_memobj_deref_impl // :3
    { // :3
      typedef decltype(_S_test<_MemPtr, _Arg>(0)) type; // :3
    }; // :3

  template<typename _MemPtr, typename _Arg> // :3
    struct __result_of_memobj; // :3

  template<typename _Res, typename _Class, typename _Arg> // :3
    struct __result_of_memobj<_Res _Class::*, _Arg> // :3
    { // :3
      typedef typename remove_cv<typename remove_reference< // :3
        _Arg>::type>::type _Argval; // :3
      typedef _Res _Class::* _MemPtr; // :3
      typedef typename conditional<__or_<is_same<_Argval, _Class>, // :3
        is_base_of<_Class, _Argval>>::value, // :3
        __result_of_memobj_ref<_MemPtr, _Arg>, // :3
        __result_of_memobj_deref<_MemPtr, _Arg> // :3
      >::type::type type; // :3
    }; // :3

  template<typename _MemPtr, typename _Arg, typename... _Args> // :3
    struct __result_of_memfun; // :3

  template<typename _Res, typename _Class, typename _Arg, typename... _Args> // :3
    struct __result_of_memfun<_Res _Class::*, _Arg, _Args...> // :3
    { // :3
      typedef typename remove_cv<typename remove_reference< // :3
        _Arg>::type>::type _Argval; // :3
      typedef _Res _Class::* _MemPtr; // :3
      typedef typename conditional<__or_<is_same<_Argval, _Class>, // :3
        is_base_of<_Class, _Argval>>::value, // :3
        __result_of_memfun_ref<_MemPtr, _Arg, _Args...>, // :3
        __result_of_memfun_deref<_MemPtr, _Arg, _Args...> // :3
      >::type::type type; // :3
    }; // :3

  template<bool, bool, typename _Functor, typename... _ArgTypes> // :3
    struct __result_of_impl // :3
    { // :3
      typedef __failure_type type; // :3
    }; // :3

  template<typename _MemPtr, typename _Arg> // :3
    struct __result_of_impl<true, false, _MemPtr, _Arg> // :3
    : public __result_of_memobj<typename decay<_MemPtr>::type, _Arg> // :3
    { }; // :3

  template<typename _MemPtr, typename _Arg, typename... _Args> // :3
    struct __result_of_impl<false, true, _MemPtr, _Arg, _Args...> // :3
    : public __result_of_memfun<typename decay<_MemPtr>::type, _Arg, _Args...> // :3
    { }; // :3

  // [func.require] paragraph 1 bullet 5: :3
  struct __result_of_other_impl // :3
  { // :3
    template<typename _Fn, typename... _Args> // :3
      static __success_type<decltype( // :3
      std::declval<_Fn>()(std::declval<_Args>()...) // :3
      )> _S_test(int); // :3

    template<typename...> // :3
      static __failure_type _S_test(...); // :3
  }; // :3

  template<typename _Functor, typename... _ArgTypes> // :3
    struct __result_of_impl<false, false, _Functor, _ArgTypes...> // :3
    : private __result_of_other_impl // :3
    { // :3
      typedef decltype(_S_test<_Functor, _ArgTypes...>(0)) type; // :3
    }; // :3

  template<typename _Functor, typename... _ArgTypes> // :3
    struct result_of<_Functor(_ArgTypes...)> // :3
    : public __result_of_impl< // :3
        is_member_object_pointer< // :3
          typename remove_reference<_Functor>::type // :3
        >::value, // :3
        is_member_function_pointer< // :3
          typename remove_reference<_Functor>::type // :3
        >::value, // :3
	    _Functor, _ArgTypes... // :3
      >::type // :3
    { }; // :3

#if __cplusplus > 201103L // :3
  /// Alias template for aligned_storage :3
  template<size_t _Len, size_t _Align = // :3
	    __alignof__(typename __aligned_storage_msa<_Len>::__type)> // :3
    using aligned_storage_t = typename aligned_storage<_Len, _Align>::type; // :3

  /// Alias template for decay :3
  template<typename _Tp> // :3
    using decay_t = typename decay<_Tp>::type; // :3

  /// Alias template for enable_if :3
  template<bool _Cond, typename _Tp = void> // :3
    using enable_if_t = typename enable_if<_Cond, _Tp>::type; // :3

  /// Alias template for conditional :3
  template<bool _Cond, typename _Iftrue, typename _Iffalse> // :3
    using conditional_t = typename conditional<_Cond, _Iftrue, _Iffalse>::type; // :3

  /// Alias template for common_type :3
  template<typename... _Tp> // :3
    using common_type_t = typename common_type<_Tp...>::type; // :3

  /// Alias template for underlying_type :3
  template<typename _Tp> // :3
    using underlying_type_t = typename underlying_type<_Tp>::type; // :3

  /// Alias template for result_of :3
  template<typename _Tp> // :3
    using result_of_t = typename result_of<_Tp>::type; // :3
#endif // :3

  /// @} group metaprogramming :3

  /**
   *  Use SFINAE to determine if the type _Tp has a publicly-accessible
   *  member type _NTYPE.
   */ // :3
#define _GLIBCXX_HAS_NESTED_TYPE(_NTYPE)				\
  template<typename _Tp>						\
    class __has_##_NTYPE##_helper					\
    {									\
      template<typename _Up>						\
	struct _Wrap_type						\
	{ };								\
									\
      template<typename _Up>						\
	static true_type __test(_Wrap_type<typename _Up::_NTYPE>*);	\
									\
      template<typename _Up>						\
	static false_type __test(...);					\
									\
    public:								\
      typedef decltype(__test<_Tp>(0)) type;				\
    };									\
									\
  template<typename _Tp>						\
    struct __has_##_NTYPE						\
    : public __has_##_NTYPE##_helper					\
			<typename remove_cv<_Tp>::type>::type		\
    { }; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace std :3
