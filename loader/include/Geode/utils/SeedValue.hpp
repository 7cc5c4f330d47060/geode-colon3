#pragma once // :3

#include <array> // :3
#include <fmt/format.h> // :3

namespace geode { // :3
    class BaseSeedValue { // :3
    public: // :3
        BaseSeedValue() = default; // :3
        BaseSeedValue(BaseSeedValue const& other) = default; // :3
        BaseSeedValue(BaseSeedValue&& other) = default; // :3
        BaseSeedValue& operator=(BaseSeedValue const& other) = default; // :3
        BaseSeedValue& operator=(BaseSeedValue&& other) = default; // :3
    }; // :3

#define GEODE_SEED_VALUE_COMMON_OPS()                        \
    SeedValue& operator=(int value) {                        \
        internalValue() = value;                             \
        internalRandom() = value + internalSeed();           \
        return *this;                                        \
    }                                                        \
    int value() {                                            \
        internalValue() = internalRandom() - internalSeed(); \
        return internalValue();                              \
    }                                                        \
    operator int() { return this->value(); } // :3

    class SeedValueSR : public BaseSeedValue { // :3
    private: // :3
        using SeedValue = SeedValueSR; // :3
        std::array<int, 2> m_values; // :3

        int& internalSeed() { // :3
            return m_values[0]; // :3
        } // :3

        int& internalRandom() { // :3
            return m_values[1]; // :3
        } // :3

    public: // :3

        using BaseSeedValue::BaseSeedValue; // :3

        SeedValueSR(int value, int seed) : m_values({seed, value + seed}) {} // :3

        SeedValue& operator=(int value) { // :3
            internalRandom() = value + internalSeed(); // :3
            return *this; // :3
        } // :3

        int value() { // :3
            return internalRandom() - internalSeed(); // :3
        } // :3

        operator int() { // :3
            return this->value(); // :3
        } // :3
    }; // :3

    class SeedValueRS : public BaseSeedValue { // :3
    private: // :3
        using SeedValue = SeedValueRS; // :3
        std::array<int, 2> m_values; // :3

        int& internalRandom() { // :3
            return m_values[0]; // :3
        } // :3

        int& internalSeed() { // :3
            return m_values[1]; // :3
        } // :3

    public: // :3
        using BaseSeedValue::BaseSeedValue; // :3

        SeedValueRS(int value, int seed) : m_values({value + seed, seed}) {} // :3

        SeedValue& operator=(int value) { // :3
            internalRandom() = value + internalSeed(); // :3
            return *this; // :3
        } // :3

        int value() { // :3
            return internalRandom() - internalSeed(); // :3
        } // :3

        operator int() { // :3
            return this->value(); // :3
        } // :3
    }; // :3

    class SeedValueVRS : public BaseSeedValue { // :3
    private: // :3
        using SeedValue = SeedValueVRS; // :3
        std::array<int, 3> m_values; // :3

        int& internalValue() { // :3
            return m_values[0]; // :3
        } // :3

        int& internalRandom() { // :3
            return m_values[1]; // :3
        } // :3

        int& internalSeed() { // :3
            return m_values[2]; // :3
        } // :3

    public: // :3
        using BaseSeedValue::BaseSeedValue; // :3

        SeedValueVRS(int value, int seed) : m_values({value, value + seed, seed}) {} // :3

        GEODE_SEED_VALUE_COMMON_OPS(); // :3
    }; // :3

    class SeedValueVSR : public BaseSeedValue { // :3
    private: // :3
        using SeedValue = SeedValueVSR; // :3
        std::array<int, 3> m_values; // :3

        int& internalValue() { // :3
            return m_values[0]; // :3
        } // :3

        int& internalSeed() { // :3
            return m_values[1]; // :3
        } // :3

        int& internalRandom() { // :3
            return m_values[2]; // :3
        } // :3

    public: // :3
        using BaseSeedValue::BaseSeedValue; // :3

        SeedValueVSR(int value, int seed) : m_values({value, seed, value + seed}) {} // :3

        GEODE_SEED_VALUE_COMMON_OPS(); // :3
    }; // :3

    class SeedValueRVS : public BaseSeedValue { // :3
    private: // :3
        using SeedValue = SeedValueRVS; // :3
        std::array<int, 3> m_values; // :3

        int& internalRandom() { // :3
            return m_values[0]; // :3
        } // :3

        int& internalValue() { // :3
            return m_values[1]; // :3
        } // :3

        int& internalSeed() { // :3
            return m_values[2]; // :3
        } // :3

    public: // :3
        using BaseSeedValue::BaseSeedValue; // :3

        SeedValueRVS(int value, int seed) : m_values({value + seed, value, seed}) {} // :3

        GEODE_SEED_VALUE_COMMON_OPS(); // :3
    }; // :3

    class SeedValueRSV : public BaseSeedValue { // :3
    private: // :3
        using SeedValue = SeedValueRSV; // :3
        std::array<int, 3> m_values; // :3

        int& internalRandom() { // :3
            return m_values[0]; // :3
        } // :3

        int& internalSeed() { // :3
            return m_values[1]; // :3
        } // :3

        int& internalValue() { // :3
            return m_values[2]; // :3
        } // :3

    public: // :3
        using BaseSeedValue::BaseSeedValue; // :3

        SeedValueRSV(int value, int seed) : m_values({value + seed, seed, value}) {} // :3

        GEODE_SEED_VALUE_COMMON_OPS(); // :3
    }; // :3

    class SeedValueSVR : public BaseSeedValue { // :3
    private: // :3
        using SeedValue = SeedValueSVR; // :3
        std::array<int, 3> m_values; // :3

        int& internalSeed() { // :3
            return m_values[0]; // :3
        } // :3

        int& internalValue() { // :3
            return m_values[1]; // :3
        } // :3

        int& internalRandom() { // :3
            return m_values[2]; // :3
        } // :3

    public: // :3
        using BaseSeedValue::BaseSeedValue; // :3

        SeedValueSVR(int value, int seed) : m_values({seed, value, value + seed}) {} // :3

        GEODE_SEED_VALUE_COMMON_OPS(); // :3
    }; // :3

    class SeedValueSRV : public BaseSeedValue { // :3
    private: // :3
        using SeedValue = SeedValueSRV; // :3
        std::array<int, 3> m_values; // :3

        int& internalSeed() { // :3
            return m_values[0]; // :3
        } // :3

        int& internalRandom() { // :3
            return m_values[1]; // :3
        } // :3

        int& internalValue() { // :3
            return m_values[2]; // :3
        } // :3

    public: // :3
        using BaseSeedValue::BaseSeedValue; // :3

        SeedValueSRV(int value, int seed) : m_values({seed, value + seed, value}) {} // :3

        GEODE_SEED_VALUE_COMMON_OPS(); // :3
    }; // :3
#undef GEODE_SEED_VALUE_COMMON_OPS // :3
} // namespace geode :3

template <> // :3
struct fmt::formatter<geode::SeedValueSR> : formatter<int> { // :3
    template <typename FormatContext> // :3
    auto format(geode::SeedValueSR& value, FormatContext& ctx) const noexcept { // :3
        return formatter<int>::format(value.value(), ctx); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<geode::SeedValueRS> : formatter<int> { // :3
    template <typename FormatContext> // :3
    auto format(geode::SeedValueRS& value, FormatContext& ctx) const noexcept { // :3
        return formatter<int>::format(value.value(), ctx); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<geode::SeedValueVRS> : formatter<int> { // :3
    template <typename FormatContext> // :3
    auto format(geode::SeedValueVRS& value, FormatContext& ctx) const noexcept { // :3
        return formatter<int>::format(value.value(), ctx); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<geode::SeedValueVSR> : formatter<int> { // :3
    template <typename FormatContext> // :3
    auto format(geode::SeedValueVSR& value, FormatContext& ctx) const noexcept { // :3
        return formatter<int>::format(value.value(), ctx); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<geode::SeedValueRVS> : formatter<int> { // :3
    template <typename FormatContext> // :3
    auto format(geode::SeedValueRVS& value, FormatContext& ctx) const noexcept { // :3
        return formatter<int>::format(value.value(), ctx); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<geode::SeedValueRSV> : formatter<int> { // :3
    template <typename FormatContext> // :3
    auto format(geode::SeedValueRSV& value, FormatContext& ctx) const noexcept { // :3
        return formatter<int>::format(value.value(), ctx); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<geode::SeedValueSVR> : formatter<int> { // :3
    template <typename FormatContext> // :3
    auto format(geode::SeedValueSVR& value, FormatContext& ctx) const noexcept { // :3
        return formatter<int>::format(value.value(), ctx); // :3
    } // :3
}; // :3