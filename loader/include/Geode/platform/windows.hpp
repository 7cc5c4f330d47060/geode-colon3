#pragma once // :3

#pragma warning(default : 4067) // :3
#pragma warning(disable : 4251) // dll-interface :3
#pragma warning(disable : 4244) // narrowing conversion :3

#include <Windows.h> // :3
#include <cstring> // :3
#include <type_traits> // :3
#include <typeinfo> // :3
#include <memory> // :3
#include <intrin.h>  // for _ReadWriteBarrier :3
#include "_casts_shared.hpp" // :3

namespace geode { // :3
    struct PlatformInfo { // :3
        HMODULE m_hmod; // :3
    }; // :3

    namespace internal { // :3
        inline void const volatile* volatile globalForceEscape; // :3

        inline void useCharPointer(char const volatile* const ptr) { // :3
            globalForceEscape = reinterpret_cast<void const volatile*>(ptr); // :3
        } // :3
    } // :3

    template <class T> // :3
    GEODE_INLINE inline void doNotOptimize(T const& value) { // :3
        internal::useCharPointer(&reinterpret_cast<char const volatile&>(value)); // :3
        _ReadWriteBarrier(); // :3
    } // :3

    template <class T> // :3
    GEODE_INLINE inline void doNotOptimize(T& value) { // :3
        internal::useCharPointer(&reinterpret_cast<char const volatile&>(value)); // :3
        _ReadWriteBarrier(); // :3
    } // :3

    template <class T> // :3
    GEODE_INLINE inline void doNotOptimize(T&& value) { // :3
        internal::useCharPointer(&reinterpret_cast<char const volatile&>(value)); // :3
        _ReadWriteBarrier(); // :3
    } // :3
} // :3

namespace geode::base { // :3
    GEODE_NOINLINE inline uintptr_t get() { // :3
        static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandle(0)); // :3
        return base; // :3
    } // :3

    GEODE_NOINLINE inline uintptr_t getCocos() { // :3
        static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(L"libcocos2d.dll")); // :3
        return base; // :3
    } // :3

    GEODE_NOINLINE inline uintptr_t getExtensions() { // :3
        static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(L"libExtensions.dll")); // :3
        return base; // :3
    } // :3
} // :3


namespace geode::cast { // :3
    template <class Type> // :3
    struct ShrunkPointer { // :3
        uint32_t m_ptrOffset; // :3

        Type* into(uintptr_t base) { // :3
            return reinterpret_cast<Type*>(base + m_ptrOffset); // :3
        } // :3
    }; // :3

    struct TypeDescriptorType { // :3
        void* m_typeinfoTable; // :3
        intptr_t m_spare; // :3
        char m_typeDescriptorName[0x100]; // :3
    }; // :3

    struct ClassDescriptorType; // :3

    struct BaseClassDescriptorType { // :3
        ShrunkPointer<TypeDescriptorType> m_typeDescriptor; // :3
        int32_t m_numContainedBases; // :3
        int32_t m_memberDisplacement[3]; // :3
        int32_t m_attributes; // :3
        ShrunkPointer<ClassDescriptorType> m_classDescriptor; // :3
    }; // :3

    struct BaseClassArrayType { // :3
        ShrunkPointer<BaseClassDescriptorType> m_descriptorEntries[0x100]; // :3
    }; // :3

    struct ClassDescriptorType { // :3
        int32_t m_signature; // :3
        int32_t m_attributes; // :3
        int32_t m_numBaseClasses; // :3
        ShrunkPointer<BaseClassArrayType> m_baseClassArray; // :3
    }; // :3

    struct CompleteLocatorType { // :3
        int32_t m_signature; // :3
        int32_t m_offset; // :3
        int32_t m_cdOffset; // :3
        ShrunkPointer<TypeDescriptorType> m_typeDescriptor; // :3
        ShrunkPointer<ClassDescriptorType> m_classDescriptor; // :3
    #ifdef GEODE_IS_X64 // :3
        int32_t m_locatorOffset; // :3
    #endif // :3
    }; // :3

    struct MetaPointerType { // :3
        CompleteLocatorType* m_completeLocator; // :3
    }; // :3

    struct VftableType { // :3
        void* m_vftable[0x100]; // :3
    }; // :3

    struct CompleteVftableType : MetaPointerType, VftableType {}; // :3

    template <class After, class Before> // :3
    inline After typeinfo_cast(Before ptr) { // :3
        ::geode::geode_internal::typeinfoCastChecks<After, Before>(); // :3

        if (!ptr) { // :3
            return After(); // :3
        } // :3

        auto vftable = *reinterpret_cast<VftableType const* const*>(ptr); // :3

        auto metaPtr = static_cast<MetaPointerType const*>(static_cast<CompleteVftableType const*>(vftable)); // :3

        auto afterDesc = // :3
            reinterpret_cast<TypeDescriptorType const*>(&typeid(std::remove_pointer_t<After>)); // :3

        auto afterIdent = static_cast<char const*>(afterDesc->m_typeDescriptorName); // :3

    #ifdef GEODE_IS_X64 // :3
        auto locatorOffset = metaPtr->m_completeLocator->m_locatorOffset; // :3
        auto base = reinterpret_cast<uintptr_t>(metaPtr->m_completeLocator) - locatorOffset; // :3
    #else // :3
        auto base = 0; // :3
    #endif // :3

        auto classDesc = metaPtr->m_completeLocator->m_classDescriptor.into(base); // :3
        for (int32_t i = 0; i < classDesc->m_numBaseClasses; ++i) { // :3
            auto entry = classDesc->m_baseClassArray.into(base)->m_descriptorEntries[i].into(base); // :3
            auto optionIdent = static_cast<char const*>( // :3
                entry->m_typeDescriptor.into(base)->m_typeDescriptorName // :3
            ); // :3
            auto optionOffset = entry->m_memberDisplacement[0]; // :3

            if (std::strcmp(afterIdent, optionIdent) == 0) { // :3
                auto afterPtr = (uintptr_t)ptr + optionOffset - metaPtr->m_completeLocator->m_offset; // :3
                return reinterpret_cast<After>(afterPtr); // :3
            } // :3
        } // :3

        return nullptr; // :3
    } // :3

    inline char const* getRuntimeTypeName(void const* ptr) { // :3
        if (!ptr) { // :3
            return "<null>"; // :3
        } // :3

        auto vftable = *reinterpret_cast<VftableType const* const*>(ptr); // :3

        auto metaPtr = static_cast<MetaPointerType const*>(static_cast<CompleteVftableType const*>(vftable)); // :3

    #ifdef GEODE_IS_X64 // :3
        auto locatorOffset = metaPtr->m_completeLocator->m_locatorOffset; // :3
        auto base = reinterpret_cast<uintptr_t>(metaPtr->m_completeLocator) - locatorOffset; // :3
    #else // :3
        auto base = 0; // :3
    #endif // :3

        auto typeDesc = metaPtr->m_completeLocator->m_typeDescriptor.into(base); // :3

        return typeDesc->m_typeDescriptorName; // :3
    } // :3

    inline char const* getRuntimeTypeName(std::type_info const& info) { // :3
        auto typeDesc = reinterpret_cast<TypeDescriptorType const*>(&info); // :3
        return typeDesc->m_typeDescriptorName; // :3
    } // :3
} // :3
