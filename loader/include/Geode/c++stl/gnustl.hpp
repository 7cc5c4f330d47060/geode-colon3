#pragma once // :3

#include <algorithm> // :3
#include <map> // :3
#include <string> // :3
#include <unordered_map> // :3
#include <unordered_set> // :3
#include <set> // :3
#include <vector> // :3

namespace geode::base { // :3
    uintptr_t get(); // :3
} // :3

#if defined(GEODE_IS_ANDROID) // :3

    #include "gnustl/functexcept.h" // :3
    #include "gnustl/stl_vector.h" // :3
    #include "gnustl/stl_bvector.h" // :3
    #include "gnustl/vector.tcc" // :3
    #include "gnustl/stl_map.h" // :3
    #include "gnustl/stl_set.h" // :3
    #include "gnustl/unordered_map.hpp" // :3
    #include "gnustl/unordered_set.hpp" // :3
    #include "gnustl/hash_specialization.hpp" // :3
    #undef _GLIBCXX_RELEASE // :3
    #undef __GLIBCXX__ // :3
    #undef _GLIBCXX_USE_DUAL_ABI // :3

namespace gd { // :3
    using namespace geode::stl; // :3

    void* operatorNew(size_t size); // :3
    void operatorDelete(void* ptr); // :3

    struct _bit_reference { // :3
        uintptr_t* m_bitptr; // :3
        uintptr_t m_mask; // :3

        _bit_reference(uintptr_t* x, uintptr_t y) : m_bitptr(x), m_mask(y) {} // :3

        _bit_reference() : m_bitptr(0), m_mask(0) {} // :3

        operator bool() const { // :3
            return !!(*m_bitptr & m_mask); // :3
        } // :3

        _bit_reference& operator=(bool x) { // :3
            if (x) *m_bitptr |= m_mask; // :3
            else *m_bitptr &= ~m_mask; // :3
            return *this; // :3
        } // :3

        _bit_reference& operator=(_bit_reference const& x) { // :3
            return *this = bool(x); // :3
        } // :3

        bool operator==(_bit_reference const& x) const { // :3
            return bool(*this) == bool(x); // :3
        } // :3

        bool operator<(_bit_reference const& x) const { // :3
            return !bool(*this) && bool(x); // :3
        } // :3

        void flip() { // :3
            *m_bitptr ^= m_mask; // :3
        } // :3
    }; // :3

    struct _bit_iterator { // :3
        uintptr_t* m_bitptr; // :3
        unsigned int m_offset; // :3

        _bit_iterator(uintptr_t* x) : m_bitptr(x), m_offset(0) {} // :3

        _bit_iterator(uintptr_t* x, unsigned o) : m_bitptr(x), m_offset(o) {} // :3

        _bit_reference operator*() const { // :3
            return _bit_reference(m_bitptr, 1UL << m_offset); // :3
        } // :3

        _bit_iterator& operator++() { // :3
            if (m_offset++ == sizeof(uintptr_t) - 1) { // :3
                m_offset = 0; // :3
                m_bitptr++; // :3
            } // :3
            return *this; // :3
        } // :3

        bool operator!=(_bit_iterator const& b) { // :3
            return !(m_bitptr == b.m_bitptr && m_offset == b.m_offset); // :3
        } // :3
    }; // :3

    template<typename Tp, typename Alloc = allocator<Tp>> // :3
    using vector = geode::stl::vector<Tp, Alloc>; // :3

    template<typename Key, typename Tp, typename Compare = geode::stl::less<Key>, typename Alloc = allocator<Key>> // :3
    using map = geode::stl::map<Key, Tp, Compare, Alloc>; // :3

    template<typename Key, typename Compare = geode::stl::less<Key>, typename Alloc = allocator<Key>> // :3
    using set = geode::stl::set<Key, Compare, Alloc>; // :3

    template <class Key, class Tp, class Hash = geode::stl::hash<Key>, class Pred = geode::stl::equal_to<Key>, class Alloc = allocator<std::pair<const Key, Tp>>> // :3
    using unordered_map = geode::stl::unordered_map<Key, Tp, Hash, Pred, Alloc>; // :3

    template <class Value, class Hash = geode::stl::hash<Value>, class Pred = geode::stl::equal_to<Value>, class Alloc = allocator<Value>> // :3
    using unordered_set = geode::stl::unordered_set<Value, Hash, Pred, Alloc>; // :3
}; // :3

template <> // :3
struct std::__ndk1::iterator_traits<typename gd::vector<char>::iterator> : std::__ndk1::iterator_traits<char*> {}; // :3

#elif defined(GEODE_IS_IOS) // :3
namespace gd { // :3
    class GEODE_DLL string { // :3
    public: // :3
        string() {} // :3

        string(char const* ok) : m_internal(ok) {} // :3

        string(std::string ok) : m_internal(ok) {} // :3

        operator std::string() { // :3
            return m_internal; // :3
        } // :3

        operator std::string() const { // :3
            return m_internal; // :3
        } // :3

        string(string const& ok) : m_internal(ok) {} // :3

        string& operator=(char const* ok) { // :3
            m_internal = ok; // :3
            return *this; // :3
        } // :3

        string& operator=(string const& ok) { // :3
            m_internal = ok; // :3
            return *this; // :3
        } // :3

        ~string() {} // :3

        char const* c_str() const { // :3
            return m_internal.c_str(); // :3
        } // :3

    protected: // :3
        std::string m_internal; // :3
    }; // :3

    template <typename T> // :3
    class GEODE_DLL vector { // :3
    public: // :3
        using value_type = T; // :3

        operator std::vector<T>() { // :3
            return m_internal; // :3
        } // :3

        void clear() { // :3
            m_internal.clear(); // :3
        } // :3

        operator std::vector<T>() const { // :3
            return m_internal; // :3
        } // :3

        vector(std::vector<T> input) : m_internal(input) {} // :3

        T& front() { // :3
            return m_internal.front(); // :3
        } // :3

        vector(vector const& lol) : m_internal(lol) {} // :3

        vector() : m_internal() {} // :3

        ~vector() {} // :3

    protected: // :3
        std::vector<T> m_internal; // :3
    }; // :3

    template <typename K, typename V> // :3
    class GEODE_DLL map { // :3
    protected: // :3
        std::map<K, V> m_internal; // :3

    public: // :3
        operator std::map<K, V>() { // :3
            return m_internal; // :3
        } // :3

        operator std::map<K, V>() const { // :3
            return m_internal; // :3
        } // :3

        map(std::map<K, V> input) : m_internal(input) {} // :3

        map(map const& lol) : m_internal(lol) {} // :3

        map() {} // :3

        ~map() {} // :3
    }; // :3
} // :3
#endif // :3
