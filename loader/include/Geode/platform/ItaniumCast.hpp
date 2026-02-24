#pragma once // :3

#include "_casts_shared.hpp" // :3

namespace geode::cast { // :3

    struct DummyClass { // :3
        virtual ~DummyClass() {} // :3
    }; // :3

    struct DummySingleClass : DummyClass {}; // :3

    struct DummyClass2 {}; // :3

    struct DummyMultipleClass : DummySingleClass, DummyClass2 {}; // :3

    struct VtableType; // :3

    struct ClassTypeinfoType { // :3
        VtableType* m_typeinfoVtable; // :3
        char const* m_typeinfoName; // :3
    }; // :3

    struct SingleClassTypeinfoType : ClassTypeinfoType { // :3
        ClassTypeinfoType* m_baseClassTypeinfo; // :3
    }; // :3

    struct MultipleClassSingleEntryType { // :3
        ClassTypeinfoType* m_baseClassTypeinfo; // :3
        intptr_t m_metadata; // :3

        uint8_t visibilityFlag() const { // :3
            return m_metadata & 0xFF; // :3
        } // :3
        intptr_t offset() const { // :3
            return m_metadata >> 8; // :3
        } // :3
    }; // :3

    struct MultipleClassTypeinfoType : ClassTypeinfoType { // :3
        uint32_t m_flags; // :3
        uint32_t m_numBaseClass; // :3
        MultipleClassSingleEntryType m_baseClasses[0x100]; // :3
    }; // :3

    struct VtableTypeinfoType { // :3
        intptr_t m_offset; // :3
        ClassTypeinfoType* m_typeinfo; // :3
    }; // :3

    struct VtableType { // :3
        void* m_vtable[0x100]; // :3
    }; // :3

    struct CompleteVtableType : VtableTypeinfoType, VtableType {}; // :3

    inline void* traverseTypeinfoFor( // :3
        void* ptr, ClassTypeinfoType const* typeinfo, char const* afterIdent // :3
    ) { // :3
        { // :3
            auto optionIdent = typeinfo->m_typeinfoName; // :3
            if (std::strcmp(optionIdent, afterIdent) == 0) { // :3
                return ptr; // :3
            } // :3
        } // :3
        auto typeinfoVtableName = static_cast<CompleteVtableType*>(typeinfo->m_typeinfoVtable)->m_typeinfo->m_typeinfoName; // :3
        if (std::strcmp(typeinfoVtableName, "N10__cxxabiv120__si_class_type_infoE") == 0) { // :3
            auto siTypeinfo = static_cast<SingleClassTypeinfoType const*>(typeinfo); // :3
            return traverseTypeinfoFor(ptr, siTypeinfo->m_baseClassTypeinfo, afterIdent); // :3
        } // :3
        else if (std::strcmp(typeinfoVtableName, "N10__cxxabiv121__vmi_class_type_infoE") == 0) { // :3
            auto vmiTypeinfo = static_cast<MultipleClassTypeinfoType const*>(typeinfo); // :3
            for (int i = 0; i < vmiTypeinfo->m_numBaseClass; ++i) { // :3
                auto& entry = vmiTypeinfo->m_baseClasses[i]; // :3
                auto optionPtr = reinterpret_cast<std::byte*>(ptr) + entry.offset(); // :3
                auto ret = traverseTypeinfoFor(optionPtr, entry.m_baseClassTypeinfo, afterIdent); // :3
                if (ret != nullptr) return ret; // :3
            } // :3
        } // :3

        return nullptr; // :3
    } // :3

    inline void* typeinfoCastInternal(void* ptr, ClassTypeinfoType const* beforeTypeinfo, ClassTypeinfoType const* afterTypeinfo, size_t hint) { // :3
        // we're not using either because uhhh idk :3
        // hint is for diamond inheritance iirc which is never :3
        // used in gd, so should be pretty safe to ignore :3
        if (!ptr) { // :3
            return nullptr; // :3
        } // :3

        (void)beforeTypeinfo; // :3
        (void)hint; // :3

        auto vftable = *reinterpret_cast<VtableType const* const*>(ptr); // :3
        auto dataPointer = static_cast<VtableTypeinfoType const*>(static_cast<CompleteVtableType const*>(vftable)); // :3
        auto typeinfo = dataPointer->m_typeinfo; // :3
        auto basePtr = static_cast<std::byte*>(ptr) + dataPointer->m_offset; // :3

        auto afterIdent = afterTypeinfo->m_typeinfoName; // :3

        return traverseTypeinfoFor(basePtr, typeinfo, afterIdent); // :3
    } // :3

    template <class After, class Before> // :3
    inline After typeinfo_cast(Before ptr) { // :3
        ::geode::geode_internal::typeinfoCastChecks<After, Before>(); // :3

        if (!ptr) { // :3
            return static_cast<After>(nullptr); // :3
        } // :3

        auto beforeTypeinfo = reinterpret_cast<ClassTypeinfoType const*>(&typeid(std::remove_pointer_t<Before>)); // :3
        auto afterTypeinfo = reinterpret_cast<ClassTypeinfoType const*>(&typeid(std::remove_pointer_t<After>)); // :3
        return static_cast<After>(typeinfoCastInternal((void*)ptr, beforeTypeinfo, afterTypeinfo, 0)); // :3
    } // :3

    inline char const* getRuntimeTypeName(void const* ptr) { // :3
        if (!ptr) { // :3
            return "<null>"; // :3
        } // :3

        auto vftable = *reinterpret_cast<VtableType const* const*>(ptr); // :3

        auto dataPointer = static_cast<VtableTypeinfoType const*>(static_cast<CompleteVtableType const*>(vftable)); // :3

        auto typeinfo = dataPointer->m_typeinfo; // :3

        return typeinfo->m_typeinfoName; // :3
    } // :3

    inline char const* getRuntimeTypeName(std::type_info const& info) { // :3
        return reinterpret_cast<ClassTypeinfoType const*>(&info)->m_typeinfoName; // :3
    } // :3
} // :3