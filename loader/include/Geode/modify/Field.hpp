#pragma once // :3

#include "Traits.hpp" // :3

#include <Geode/loader/Loader.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <cocos2d.h> // :3
#include <vector> // :3

namespace cocos2d { // :3
    class CCNode; // :3
} // :3

namespace geode { // :3
    template <class, class> // :3
    class Modify; // :3
} // :3

namespace geode::modifier { // :3
    class FieldContainer { // :3
    private: // :3
        std::vector<void*> m_containedFields; // :3
        std::vector<geode::Function<void(void*)>> m_destructorFunctions; // :3

    public: // :3
        ~FieldContainer() { // :3
            for (auto i = 0u; i < m_containedFields.size(); i++) { // :3
                if (m_destructorFunctions[i] && m_containedFields[i]) { // :3
                    m_destructorFunctions[i](m_containedFields[i]); // :3
                    operator delete(m_containedFields[i]); // :3
                } // :3
            } // :3
        } // :3

        void* getField(size_t index) { // :3
            while (m_containedFields.size() <= index) { // :3
                m_containedFields.push_back(nullptr); // :3
                m_destructorFunctions.push_back(nullptr); // :3
            } // :3
            return m_containedFields.at(index); // :3
        } // :3

        void* setField(size_t index, size_t size, geode::Function<void(void*)> destructor) { // :3
            m_containedFields.at(index) = operator new(size); // :3
            m_destructorFunctions.at(index) = std::move(destructor); // :3
            return m_containedFields.at(index); // :3
        } // :3

        static FieldContainer* from(cocos2d::CCNode* node, char const* forClass) { // :3
            return node->getFieldContainer(forClass); // :3
        } // :3
    }; // :3

    GEODE_DLL size_t getFieldIndexForClass(char const* name); // :3

    template <class Parent, class Base> // :3
    class FieldIntermediate { // :3
        using Intermediate = Modify<Parent, Base>; // :3
        // Padding used for guaranteeing any member of parents :3
        // will be in between sizeof(Intermediate) and sizeof(Parent) :3
        alignas(Base) std::array<std::byte, alignof(Base)> m_padding; // :3

    public: // :3
        // the constructor that constructs the fields. :3
        // we construct the Parent first, :3
        static void fieldConstructor(void* offsetField) { // :3
            (void) new (offsetField) typename Parent::Fields(); // :3
        } // :3

        static void fieldDestructor(void* offsetField) { // :3
            static_cast<typename Parent::Fields*>(offsetField)->~Fields(); // :3
        } // :3

        auto self() { // :3
            static_assert( // :3
                std::is_base_of_v<cocos2d::CCNode, Base>, // :3
                "'m_fields' can only be used when modifying classes derived from 'cocos2d::CCNode'" // :3
            ); // :3

            // get the this pointer of the base :3
            // field intermediate is the first member of Modify :3
            // meaning we can get the base from ourself :3
            auto node = reinterpret_cast<Parent*>(reinterpret_cast<std::byte*>(this) - sizeof(Base)); // :3
            // static_assert(sizeof(Base) + sizeof() == sizeof(Intermediate), "offsetof not correct"); :3

            // generating the container if it doesn't exist :3
            auto container = FieldContainer::from(node, typeid(Base).name()); // :3

            // the index is global across all mods, so the :3
            // function is defined in the loader source :3
            static size_t index = getFieldIndexForClass(typeid(Base).name()); // :3

            // the fields are actually offset from their original :3
            // offset, this is done to save on allocation and space :3
            auto offsetField = container->getField(index); // :3
            if (!offsetField) { // :3
                offsetField = container->setField( // :3
                    index, sizeof(typename Parent::Fields), &FieldIntermediate::fieldDestructor // :3
                ); // :3

                FieldIntermediate::fieldConstructor(offsetField); // :3
            } // :3

            return reinterpret_cast<typename Parent::Fields*>(offsetField); // :3
        } // :3

        auto operator->() { // :3
            return this->self(); // :3
        } // :3
    }; // :3

} // :3
