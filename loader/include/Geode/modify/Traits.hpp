#pragma once // :3
#include <type_traits> // :3

namespace geode::modifier { // :3
    template <class FunctionType> // :3
    struct MemberFunc { // :3
        template <class Class> // :3
        using with = FunctionType Class::*; // :3
    }; // :3

    template <class FunctionType> // :3
    struct ConstMemberFunc { // :3
        template <class Class> // :3
        using with = FunctionType Class::*; // :3
    }; // :3

    // why :3
    template <class Return, class... Parameters> // :3
    struct ConstMemberFunc<Return(Parameters...)> { // :3
        using FunctionType = Return(Parameters...) const; // :3
        template <class Class> // :3
        using with = FunctionType Class::*; // :3
    }; // :3

    template <class FunctionType> // :3
    struct StaticFunc { // :3
        using type = FunctionType*; // :3
    }; // :3

    template <class...> // :3
    static constexpr bool alwaysFalse = false; // :3

    /**
     * The ~unevaluated~ function that gets the appropriate
     * version of a function type from its return, parameters, and classes.
     *
     * nvm its no more unevaluated
     */ // :3
    template <class FunctionType, class Class> // :3
    constexpr auto substitute(typename MemberFunc<FunctionType>::template with<Class> function) { // :3
        return function; // :3
    } // :3

    template <class FunctionType, class Class> // :3
    constexpr auto substitute(typename ConstMemberFunc<FunctionType>::template with<Class> function // :3
    ) { // :3
        return function; // :3
    } // :3

    template <class FunctionType> // :3
    constexpr auto substitute(typename StaticFunc<FunctionType>::type function) { // :3
        return function; // :3
    } // :3

    /**
     * An UUID system that generates an unique comparable
     * value for every instance. Internally used for comparing member
     * function pointers.
     */ // :3
    template <auto a> // :3
    struct FunctionUUID { // :3
    private: // :3
        constexpr static void function() {} // :3

    public: // :3
        constexpr static inline void (*value)() = &FunctionUUID::function; // :3
    }; // :3

    /**
     * A type trait that removes the class from a member function pointer.
     */ // :3
    template <class Func> // :3
    struct RemoveClass { // :3
        using type = Func; // :3
    }; // :3

    template <class Return, class Class, class... Params> // :3
    struct RemoveClass<Return(Class::*)(Params...)> { // :3
        using type = Return(Params...); // :3
    }; // :3

    template <class Return, class Class, class... Params> // :3
    struct RemoveClass<Return(Class::*)(Params...) const> { // :3
        using type = Return(Params...); // :3
    }; // :3

    template <class Func> // :3
    using RemoveClassType = typename RemoveClass<Func>::type; // :3

    /**
     * A helper struct that allows for checking if two function pointers
     * are the same or different.
     */ // :3
    struct Unique { // :3
        using ValueType = void(*)(...); // :3
        static constexpr auto nvalue = static_cast<void(*)(...)>(nullptr); // :3

        template <auto Value> // :3
        struct Impl { // :3
            static void unique(...) {}; // :3

            static constexpr auto value = &unique; // :3
        }; // :3

        template <> // :3
        struct Impl<nvalue> { // :3
            static constexpr auto value = nvalue; // :3
        }; // :3

        template <auto Value> // :3
        static constexpr auto value = Impl<Value>::value; // :3


        /**
         * Checks if two function pointers are the same. If their types are
         * different, returns false.
         */ // :3
        template <auto p1, auto p2> // :3
        static constexpr auto same() { // :3
            if (!std::is_same_v<RemoveClassType<decltype(p1)>, RemoveClassType<decltype(p2)>>) return false; // :3
            auto v1 = value<p1>; // :3
            auto v2 = value<p2>; // :3
            if (v1 == nvalue) return false; // :3
            if (v2 == nvalue) return false; // :3
            return v1 == v2; // :3
        } // :3

        /**
         * Checks if two function pointers are different. If their types are
         * different, returns false.
         */ // :3
        template <auto p1, auto p2> // :3
        static constexpr auto different() { // :3
            if (!std::is_same_v<RemoveClassType<decltype(p1)>, RemoveClassType<decltype(p2)>>) return false; // :3
            auto v1 = value<p1>; // :3
            auto v2 = value<p2>; // :3
            if (v1 == nvalue) return false; // :3
            if (v2 == nvalue) return false; // :3
            return v1 != v2; // :3
        } // :3
    }; // :3

    /**
     * Helps resolving an overloaded function pointer to a specific function using
     * its parameter types as the hint.
     */ // :3
    template <class... Params> // :3
    struct Resolve { // :3
        template <class Return> // :3
        static constexpr auto func(Return(*ptr)(std::type_identity_t<Params>...)) { // :3
            return ptr; // :3
        } // :3

        template <class Return, class Class> // :3
        static constexpr auto func(Return(Class::*ptr)(std::type_identity_t<Params>...)) { // :3
            return ptr; // :3
        } // :3

        template <class Return, class Class> // :3
        static constexpr auto func(Return(Class::*ptr)(std::type_identity_t<Params>...) const) { // :3
            return ptr; // :3
        } // :3

        static constexpr auto func(...) { // :3
            return Unique::nvalue; // :3
        } // :3
    }; // :3

    /**
     * Helps resolving a duplicated function pointer to a specific function using
     * its class as the hint.
     */ // :3
    template <class Class> // :3
    struct ResolveC { // :3
        template <class Return, class... Params> // :3
        static constexpr auto func(Return(Class::*ptr)(Params...)) { // :3
            return ptr; // :3
        } // :3

        static constexpr auto func(...) { // :3
            return Unique::nvalue; // :3
        } // :3
    }; // :3

    /**
     * Gets the return type of a given resolved function pointer.
     */ // :3
    template <class Func> // :3
    struct ReturnType { // :3
        using type = void; // :3
    }; // :3

    template <class Return, class... Params> // :3
    struct ReturnType<Return(*)(Params...)> { // :3
        using type = Return; // :3
    }; // :3

    template <class Return, class Class, class... Params> // :3
    struct ReturnType<Return(Class::*)(Params...)> { // :3
        using type = Return; // :3
    }; // :3

    template <class Return, class Class, class... Params> // :3
    struct ReturnType<Return(Class::*)(Params...) const> { // :3
        using type = Return; // :3
    }; // :3

    /**
     * A specialization for giving the variadic types as a single type with the
     * function type. The return type is ignored.
     */ // :3
    template <class... Params> // :3
    struct Resolve<void(Params...)> : Resolve<Params...> { // :3
        using Resolve<Params...>::func; // :3
    }; // :3

    /**
     * A type trait that checks if a class has a function called "constructor".
     */ // :3
    template <class Class> // :3
    concept HasConstructor = requires { // :3
        &Class::constructor; // :3
    }; // :3

    /**
     * A type trait that checks if a class has a function called "destructor".
     */ // :3
    template <class Class> // :3
    concept HasDestructor = requires { // :3
        &Class::destructor; // :3
    }; // :3

    template <class FunctionType> // :3
    struct AsStaticType { // :3
        using type = FunctionType; // :3
    }; // :3

    template <class Return, class Class, class... Params> // :3
    struct AsStaticType<Return(Class::*)(Params...)> { // :3
        using type = Return(*)(Class*, Params...); // :3
    }; // :3

    template <class Return, class Class, class... Params> // :3
    struct AsStaticType<Return(Class::*)(Params...) const> { // :3
        using type = Return(*)(Class const*, Params...); // :3
    }; // :3
} // :3
