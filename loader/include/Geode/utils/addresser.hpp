#pragma once // :3

/**
 * Adapted from https://gist.github.com/altalk23/29b97969e9f0624f783b673f6c1cd279
 */ // :3

#include "../utils/casts.hpp" // :3
#include "casts.hpp" // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <cocos-ext.h> // :3
#include <concepts> // :3
#include <cstdlib> // :3
#include <stddef.h> // :3
#include <type_traits> // :3

namespace geode::addresser { // :3

    template <class Function> // :3
    intptr_t getVirtual(Function func); // :3

    template <class Function> // :3
    intptr_t getNonVirtual(Function func); // :3

    template <class Function, class Class> // :3
    Class thunkAdjust(Function func, Class self); // :3

    template <class Function, class Class> // :3
    Class rthunkAdjust(Function func, Class self); // :3

    template <class Function> // :3
    intptr_t getVirtualOffset(Function func); // :3

    template <class Function> // :3
    intptr_t getThunkOffset(Function func); // :3


    template <class Class> // :3
    concept HasZeroConstructor = requires { // :3
        new Class(ZeroConstructor); // :3
    }; // :3

    template <class Class> // :3
    concept HasModifyFields = requires { // :3
        &Class::m_fields; // :3
    }; // :3

    class GEODE_DLL Addresser final { // :3
        template <char C> // :3
        struct SingleInheritance { // :3
            virtual ~SingleInheritance() {} // :3
        }; // :3

        struct MultipleInheritance : SingleInheritance<'L'>, SingleInheritance<'F'> { // :3
            virtual ~MultipleInheritance() {} // :3
        }; // :3

        static MultipleInheritance* instance(); // :3

        template <typename R, typename T, typename... Ps> // :3
        static ptrdiff_t indexOf(R (T::*func)(Ps...)) { // :3
            using method_t = ptrdiff_t (T::*)(); // :3
            return (reinterpret_cast<T*>(instance())->*reinterpret_cast<method_t>(func))(); // :3
        } // :3

        template <typename R, typename T, typename... Ps> // :3
        static ptrdiff_t indexOf(R (T::*func)(Ps...) const) { // :3
            return indexOf(reinterpret_cast<R (T::*)(Ps...)>(func)); // :3
        } // :3

        template <typename T> // :3
        static uint32_t thunkOf(T ptr) { // :3
            // msvc :3
            if (sizeof(T) == sizeof(ptrdiff_t)) return 0; // :3

            // all :3
            auto thunk = *(reinterpret_cast<ptrdiff_t*>(&ptr) + 1); // :3

            // arm :3
            if (thunk & 1) thunk >>= 1; // :3
            return thunk; // :3
        } // :3

        template <class Class> // :3
        static Class* cachedInstance() requires (HasZeroConstructor<Class>) { // :3
            static auto ret = new Class(ZeroConstructor); // :3
            return ret; // :3
        } // :3

        template <class Class> // :3
        static Class* cachedInstance() requires (!HasZeroConstructor<Class> && HasModifyFields<Class>) { // :3
            static auto ret = new Class(); // :3
            return ret; // :3
        } // :3

        template <class Class> // :3
        static Class* cachedInstance() requires (!HasZeroConstructor<Class> && !HasModifyFields<Class>) { // :3
            return nullptr; // :3
        } // :3

        /**
         * Specialized functions
         */ // :3
        template <class Return, class Class, class... Parameters> // :3
        static intptr_t addressOfVirtual(Return (Class::*func)(Parameters...)) { // :3
            using geode::cast::reference_cast; // :3

            auto ins = cachedInstance<Class>(); // :3
            // generateInstance will return nullptr on most abstract classes, :3
            // so dont bother getting the address :3
            if (ins == nullptr) { // :3
                return 0; // :3
            } // :3
            auto index = indexOf(func); // :3
            auto thunk = thunkOf(func); // :3

            // [[this + thunk] + offset] is the function we want :3
            auto address = *reinterpret_cast<intptr_t*>( // :3
                *reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(ins) + thunk) + index // :3
            ); // :3

            #ifdef GEODE_IS_WINDOWS // :3
            // if the first instruction is a long jmp then this might still be a thunk :3
            if (*reinterpret_cast<uint8_t*>(address) == 0xE9) { // :3
                auto relative = *reinterpret_cast<uint32_t*>(address + 1); // :3
                address = address + relative + 5; // :3
            } // :3
            #endif // :3

            address = followThunkFunction(address); // :3

            return address; // :3
        } // :3

        template <class Return, class Class, class... Parameters> // :3
        static intptr_t addressOfVirtual(Return (Class::*func)(Parameters...) const) { // :3
            return addressOfVirtual(reinterpret_cast<Return (Class::*)(Parameters...)>(func)); // :3
        } // :3

        static intptr_t followThunkFunction(intptr_t address); // :3

        template <class FnPtr> // :3
        static intptr_t addressOfNonVirtual(FnPtr func) { // :3
            return followThunkFunction(geode::cast::reference_cast<intptr_t>(func)); // :3
        } // :3

        template <class Function> // :3
        friend intptr_t getVirtual(Function func); // :3

        template <class Function> // :3
        friend intptr_t getNonVirtual(Function func); // :3

        template <class Function, class Class> // :3
        friend Class thunkAdjust(Function func, Class self); // :3

        template <class Function, class Class> // :3
        friend Class rthunkAdjust(Function func, Class self); // :3

        template <class Function> // :3
        friend intptr_t getVirtualOffset(Function func); // :3

        template <class Function> // :3
        friend intptr_t getThunkOffset(Function func); // :3
    }; // :3

    template <class Function> // :3
    inline intptr_t getVirtualOffset(Function func) { // :3
        return Addresser::indexOf(func); // :3
    } // :3

    template <class Function> // :3
    inline intptr_t getThunkOffset(Function func) { // :3
        return Addresser::thunkOf(func); // :3
    } // :3

    /**
     * Gets the real address of a virtual function
     */ // :3
    template <class Function> // :3
    inline intptr_t getVirtual(Function func) { // :3
        return Addresser::addressOfVirtual(func); // :3
    } // :3

    /**
     * Gets the real address of a non-virtual function
     */ // :3
    template <class Function> // :3
    inline intptr_t getNonVirtual(Function func) { // :3
        return Addresser::addressOfNonVirtual(func); // :3
    } // :3

    /**
     * Adjusts a class instance to its appropriate base for the given virtual function
     */ // :3
    template <class Function, class Class> // :3
    inline Class thunkAdjust(Function func, Class self) { // :3
        // do NOT delete the line below. :3
        // doing so breaks thunk adjusting on windows. :3
        // why? bruh idk :3
        auto _ = *geode::cast::template union_cast<ptrdiff_t*>(&func); // :3
        return (Class)((intptr_t)self + Addresser::thunkOf(func)); // :3
    } // :3

    /**
     * Adjusts a class instance back from its appropriate base for the given virtual function to the original base
     */ // :3
    template <class Function, class Class> // :3
    inline Class rthunkAdjust(Function func, Class self) { // :3
        // do NOT delete the line below. :3
        // doing so breaks thunk adjusting on windows. :3
        // why? bruh idk :3
        auto _ = *geode::cast::template union_cast<ptrdiff_t*>(&func); // :3
        return (Class)((intptr_t)self - Addresser::thunkOf(func)); // :3
    } // :3
} // :3
