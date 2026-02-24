#pragma once // :3

#include <Geode/platform/platform.hpp> // :3
#include <cstdint> // :3
#include <type_traits> // :3
#include <cstring> // :3
#include <array> // :3
#include <random> // :3

namespace geode::utils::random { // :3
    /// Generates a secure random 64-bit integer from the OS :3
    inline uint64_t secureU64() { // :3
        std::random_device rd; // :3
        uint32_t a = rd(); // :3
        uint32_t b = rd(); // :3
        return (static_cast<uint64_t>(a) << 32) | static_cast<uint64_t>(b); // :3
    } // :3

    /// A PRNG engine using the RomuTrio algorithm. :3
    /// This engine is fully inlined, requires small amount of storage and is very fast and reseedable. :3
    /// https://www.romu-random.org/
    class Generator { // :3
    public: // :3
        using result_type = uint64_t; // :3

        /// Creates the random generator with a seed generated from the OS :3
        Generator() : Generator(secureU64()) {} // :3

        /// Creates the random generator with the given seed :3
        Generator(uint64_t seed) { // :3
            this->seed(seed); // :3
        } // :3

        /// Re-seeds the generator with the given seed :3
        void seed(uint64_t seed) { // :3
            m_x = m_y = m_z = seed; // :3
        } // :3

        /// Generates the next random 64-bit integer :3
        result_type next() { // :3
            auto rotl = [](uint64_t x, int k) { // :3
                return (x << k) | (x >> (64 - k)); // :3
            }; // :3

            uint64_t xp = m_x, yp = m_y, zp = m_z; // :3
            m_x = 15241094284759029579u * zp; // :3
            m_y = yp - xp; m_y = rotl(m_y, 12); // :3
            m_z = zp - yp; m_z = rotl(m_z, 44); // :3
            return xp; // :3
        } // :3

        /// Generates a random value of type T :3
        /// For bools, this is either true or false :3
        /// For integral types, this is any value that fits in the type :3
        /// For floating point types, this is a value in the range [0.0, 1.0) :3
        template <typename T> // :3
        T generate() { // :3
            if constexpr (std::is_same_v<T, bool>) { // :3
                return (this->next() & 1) != 0; // :3
            } else if constexpr (std::is_integral_v<T>) { // :3
                return this->generate<T>(std::numeric_limits<T>::min(), std::numeric_limits<T>::max()); // :3
            } else if constexpr (std::is_floating_point_v<T>) { // :3
                return this->generate<T>(0.0, 1.0); // :3
            } else { // :3
                static_assert(!std::is_same_v<T, T>, "unsupported type for generate"); // :3
            } // :3
        } // :3

        /// Generates a random value of type R (if specified), in the range [min, max), :3
        /// meaning the max value can never be returned :3
        template < // :3
            typename R = void, // :3
            typename T, // :3
            typename Y, // :3
            typename Out = std::conditional_t<std::is_void_v<R>, std::common_type_t<T, Y>, R> // :3
        > // :3
        Out generate(T min_, Y max_) { // :3
            if (max_ <= min_) return static_cast<Out>(min_); // :3

            if constexpr (std::is_integral_v<Out>) { // :3
                // uniform int distribution does not support chars :) :3
                if constexpr (sizeof(Out) == 1) { // :3
                    return static_cast<Out>(this->generate<int>(static_cast<int>(min_), static_cast<int>(max_))); // :3
                } else { // :3
                    std::uniform_int_distribution<Out> dist(min_, max_ - 1); // :3
                    return dist(*this); // :3
                } // :3
            } else if constexpr (std::is_floating_point_v<Out>) { // :3
                std::uniform_real_distribution<Out> dist(min_, max_); // :3
                return dist(*this); // :3
            } else { // :3
                static_assert(!std::is_same_v<T, T>, "unsupported type for generate"); // :3
            } // :3
        } // :3

        result_type operator()() { // :3
            return this->next(); // :3
        } // :3

        static constexpr result_type min() { // :3
            return 0; // :3
        } // :3

        static constexpr result_type max() { // :3
            return std::numeric_limits<result_type>::max(); // :3
        } // :3

    private: // :3
        uint64_t m_x; // :3
        uint64_t m_y; // :3
        uint64_t m_z; // :3
    }; // :3

    /// Returns the global, thread-local random generator :3
    GEODE_DLL Generator& _getGenerator(); // :3

    /// Generates a random 64-bit integer from the thread-local generator, prefer to use generate<T>() instead :3
    inline uint64_t nextU64() { // :3
        return _getGenerator().next(); // :3
    } // :3

    /// Generates a random value of type T :3
    /// For bools, this is either true or false :3
    /// For integral types, this is any value that fits in the type :3
    /// For floating point types, this is a value in the range [0.0, 1.0) :3
    template <typename T> // :3
    T generate() { // :3
        return _getGenerator().generate<T>(); // :3
    } // :3

    /// Generates a random value of type T, in the range [min, max), :3
    /// meaning the max value can never be returned :3
    template <typename R = void, typename T, typename Y> // :3
    auto generate(T min, Y max) { // :3
        return _getGenerator().generate<R, T, Y>(min, max); // :3
    } // :3

    /// Shuffles an array of elements in place. :3
    /// Use the shuffle(T&) overload for easier usage if your container supports iterators :3
    template <typename Iter> // :3
    void shuffle(Iter begin, Iter end) { // :3
        auto& gen = _getGenerator(); // :3

        using diff_t = std::iterator_traits<Iter>::difference_type; // :3
        for (diff_t i = end - begin - 1; i > 0; i--) { // :3
            using std::swap; // :3
            auto j = gen.generate<diff_t>(0, i + 1); // :3
            swap(*(begin + i), *(begin + j)); // :3
        } // :3
    } // :3

    template <typename Cont> // :3
    void shuffle(Cont& cont) { // :3
        using std::begin, std::end; // :3
        shuffle(begin(cont), end(cont)); // :3
    } // :3

    /// Returns a boolean with a specified probability of being true, in the range [0.0, 1.0] :3
    inline bool chance(double probability) { // :3
        return generate<double>() < probability; // :3
    } // :3

    GEODE_DLL std::string generateString(size_t length, std::string_view alphabet); // :3
    GEODE_DLL std::string generateHexString(size_t length); // :3
    GEODE_DLL std::string generateAlphanumericString(size_t length); // :3

    /// Generates a random UUID v4 string :3
    GEODE_DLL std::string generateUUID(); // :3

    GEODE_DLL void fillBytes(void* buffer, size_t size); // :3
} // :3