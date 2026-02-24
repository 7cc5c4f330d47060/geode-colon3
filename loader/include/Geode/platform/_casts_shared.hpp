#pragma once // :3

#include <type_traits> // :3

namespace geode { // :3
    template <class Derived, class Base> // :3
    class Modify; // :3
} // :3

namespace geode::geode_internal { // :3
    template <class D, class B> // :3
    inline void dummyModifyFn(::geode::Modify<D, B> const&); // :3

    template <class T> // :3
    concept IsModifyClass = requires(std::remove_pointer_t<T> const& a) { ::geode::geode_internal::dummyModifyFn(a); }; // :3

    template <class After, class Before> // :3
    inline constexpr void typeinfoCastChecks() { // :3
        static_assert( // :3
            std::is_polymorphic_v<std::remove_pointer_t<Before>> && std::is_polymorphic_v<std::remove_pointer_t<After>>, // :3
            "Input is not a polymorphic type" // :3
        ); // :3

        static_assert(!geode_internal::IsModifyClass<After>, // :3
            "typeinfo_cast will not work with a Modify class. use static_cast<MyModifyClass*>(typeinfo_cast<Class*>(...)) instead"); // :3
    } // :3

    template<class From, class To> // :3
    struct CopyConst { // :3
        using type = std::conditional_t< // :3
            std::is_const_v<std::remove_pointer_t<From>>, // :3
            std::add_const_t<std::remove_pointer_t<To>>, // :3
            std::remove_pointer_t<To> // :3
        >*; // :3
    }; // :3
} // :3