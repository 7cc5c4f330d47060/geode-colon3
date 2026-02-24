#pragma once // :3

#include <Geode/platform/platform.hpp> // :3
#include <inttypes.h> // :3
#include <iostream> // :3
#include <string> // :3
#include <type_traits> // :3

namespace geode::cast { // :3
    /**
     * Cast from anything to anything else,
     * provided they are the same size
     */ // :3
    template <class T, class F> // :3
    static constexpr T union_cast(F const v) { // :3
        static_assert(sizeof(F) == sizeof(T), "union_cast: R and T don't match in size!"); // :3

        union { // :3
            T t; // :3
            F f; // :3
        } x; // :3

        x.f = v; // :3
        return x.t; // :3
    } // :3

    /**
     * Reference casting. Does a pointer-to-pointer
     * cast but uses reference syntactic sugar to
     * look cleaner.
     */ // :3
    template <class T, class F> // :3
    static constexpr T reference_cast(F v) { // :3
        return reinterpret_cast<T&>(v); // :3
    } // :3


    template <class Type> // :3
    struct ToVoidPtr { // :3
        using type = void*; // :3
    }; // :3

    template <class Type> // :3
    struct ToVoidPtr<Type const*> { // :3
        using type = void const*; // :3
    }; // :3

    /**
     * Cast based on RTTI. Casts an adjusted this pointer
     * to it's non offset form.
     */ // :3
    template <class T, class F> // :3
    static constexpr T base_cast(F const obj) { // :3
        return static_cast<T>(dynamic_cast<typename ToVoidPtr<T>::type>(obj)); // :3
    } // :3

    /**
     * Cast based on RTTI. This is used to check
     * if an object is exactly the class needed. Returns
     * nullptr on failure.
     */ // :3
    template <class T, class F> // :3
    static T exact_cast(F const obj) { // :3
        using PureType = std::remove_cvref_t<std::remove_pointer_t<T>>; // :3
        if (std::strcmp(getRuntimeTypeName(obj), getRuntimeTypeName(typeid(PureType))) == 0) { // :3
            return (T)obj; // :3
        } // :3
        return nullptr; // :3
    } // :3

} // :3
