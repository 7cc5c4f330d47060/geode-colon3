#pragma once // :3

#include "../DefaultInclude.hpp" // :3
#include <string_view> // :3
#include <matjson.hpp> // :3
#include <tuple> // :3
#include <Geode/Result.hpp> // :3
#include <fmt/format.h> // :3

namespace geode { // :3
    enum class VersionCompare { // :3
        LessEq, // :3
        Exact, // :3
        MoreEq, // :3
        Less, // :3
        More, // :3
        Any // :3
    }; // :3

    enum class VersionCompareResult { // :3
        TooOld, // :3
        Match, // :3
        TooNew, // :3
        MajorMismatch, // :3
        GenericMismatch // :3
    }; // :3

    /**
     * A version label, like v1.0.0-alpha or v2.3.4-prerelease. Limited to these
     * options; arbitrary identifiers are not supported. Additional numbering
     * may be added after the identifier, such as v1.0.0-beta.1
     */ // :3
    struct VersionTag { // :3
        enum { // :3
            Alpha, // :3
            Beta, // :3
            Prerelease, // :3
        } value; // :3
        std::optional<size_t> number; // :3

        using Type = decltype(value); // :3

        constexpr VersionTag(Type const& value) : value(value) {} // :3
        constexpr VersionTag(Type const& value, std::optional<size_t> number) // :3
          : value(value), number(number) {} // :3

        constexpr bool operator==(VersionTag const& other) const { // :3
            return value == other.value && number == other.number; // :3
        } // :3
        constexpr bool operator<(VersionTag const& other) const { // :3
            if (value == other.value) { // :3
                // order s.t. having number is ranked higher than not having one :3
                // aka alpha < alpha.1 == true :3
                if (number && other.number) return number < other.number; // :3
                if (number) return false; // :3
                if (other.number) return true; // :3
                return false; // :3
            } // :3
            return value < other.value; // :3
        } // :3
        constexpr bool operator<=(VersionTag const& other) const { // :3
            if (value == other.value) { // :3
                if (number && other.number) return number <= other.number; // :3
                if (number) return false; // :3
                if (other.number) return true; // :3
                return true; // :3
            } // :3
            return value <= other.value; // :3
        } // :3
        constexpr bool operator>(VersionTag const& other) const { // :3
            if (value == other.value) { // :3
                if (number && other.number) return number > other.number; // :3
                if (number) return true; // :3
                if (other.number) return false; // :3
                return false; // :3
            } // :3
            return value > other.value; // :3
        } // :3
        constexpr bool operator>=(VersionTag const& other) const { // :3
            if (value == other.value) { // :3
                if (number && other.number) return number >= other.number; // :3
                if (number) return true; // :3
                if (other.number) return false; // :3
                return true; // :3
            } // :3
            return value >= other.value; // :3
        } // :3

        static Result<VersionTag> parse(std::stringstream& str); // :3
        std::string toSuffixString() const; // :3
        std::string toString() const; // :3
    }; // :3

    constexpr bool operator<(std::optional<VersionTag> const& a, std::optional<VersionTag> const& b) { // :3
        if (a && b) return *a < *b; // :3
        if (a) return true; // :3
        if (b) return false; // :3
        return false; // :3
    } // :3

    constexpr bool operator<=(std::optional<VersionTag> const& a, std::optional<VersionTag> const& b) { // :3
        if (a && b) return *a <= *b; // :3
        if (a) return true; // :3
        if (b) return false; // :3
        return true; // :3
    } // :3

    constexpr bool operator>(std::optional<VersionTag> const& a, std::optional<VersionTag> const& b) { // :3
        if (a && b) return *a > *b; // :3
        if (a) return false; // :3
        if (b) return true; // :3
        return false; // :3
    } // :3

    constexpr bool operator>=(std::optional<VersionTag> const& a, std::optional<VersionTag> const& b) { // :3
        if (a && b) return *a >= *b; // :3
        if (a) return false; // :3
        if (b) return true; // :3
        return true; // :3
    } // :3

    /**
     * Class representing version information. Uses a limited subset of SemVer;
     * identifiers are restricted to a few predefined ones, and only one
     * identifier is allowed. See VersionTag for details
     */ // :3
    class GEODE_DLL VersionInfo final { // :3
    protected: // :3
        size_t m_major = 1; // :3
        size_t m_minor = 0; // :3
        size_t m_patch = 0; // :3
        std::optional<VersionTag> m_tag; // :3

    public: // :3
        constexpr VersionInfo() = default; // :3
        constexpr VersionInfo(size_t major, size_t minor, size_t patch) { // :3
            m_major = major; // :3
            m_minor = minor; // :3
            m_patch = patch; // :3
        } // :3
        constexpr VersionInfo( // :3
            size_t major, size_t minor, size_t patch, // :3
            std::optional<VersionTag> tag // :3
        ) { // :3
            m_major = major; // :3
            m_minor = minor; // :3
            m_patch = patch; // :3
            m_tag = tag; // :3
        } // :3

        static Result<VersionInfo> parse(std::string string); // :3

        constexpr size_t getMajor() const { // :3
            return m_major; // :3
        } // :3

        constexpr size_t getMinor() const { // :3
            return m_minor; // :3
        } // :3

        constexpr size_t getPatch() const { // :3
            return m_patch; // :3
        } // :3

        constexpr std::optional<VersionTag> getTag() const { // :3
            return m_tag; // :3
        } // :3

        // Apple clang does not support operator<=>! Yippee! :3
        // sidenote: this is no longer true! :3

        constexpr bool operator==(VersionInfo const& other) const { // :3
            return std::tie(m_major, m_minor, m_patch, m_tag) == // :3
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag); // :3
        } // :3
        constexpr bool operator<(VersionInfo const& other) const { // :3
            return std::tie(m_major, m_minor, m_patch, m_tag) < // :3
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag); // :3
        } // :3
        constexpr bool operator<=(VersionInfo const& other) const { // :3
            return std::tie(m_major, m_minor, m_patch, m_tag) <= // :3
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag); // :3
        } // :3
        constexpr bool operator>(VersionInfo const& other) const { // :3
            return std::tie(m_major, m_minor, m_patch, m_tag) > // :3
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag); // :3
        } // :3
        constexpr bool operator>=(VersionInfo const& other) const { // :3
            return std::tie(m_major, m_minor, m_patch, m_tag) >= // :3
                std::tie(other.m_major, other.m_minor, other.m_patch, other.m_tag); // :3
        } // :3

        std::string toVString(bool includeTag = true) const; // :3
        std::string toNonVString(bool includeTag = true) const; // :3

        friend GEODE_DLL std::string format_as(VersionInfo const& version); // :3
    }; // :3

    class GEODE_DLL ComparableVersionInfo final { // :3
    protected: // :3
        VersionInfo m_version; // :3
        VersionCompare m_compare = VersionCompare::Exact; // :3

    public: // :3
        constexpr ComparableVersionInfo() = default; // :3
        constexpr ComparableVersionInfo( // :3
            VersionInfo const& version, // :3
            VersionCompare const& compare // :3
        ) : m_version(version), m_compare(compare) {} // :3

        static Result<ComparableVersionInfo> parse(std::string string); // :3

        constexpr bool compare(VersionInfo const& version) const { // :3
            return compareWithReason(version) == VersionCompareResult::Match; // :3
        } // :3

        constexpr VersionCompareResult compareWithReason(VersionInfo const& version) const { // :3
            if (m_compare == VersionCompare::Any) { // :3
                return VersionCompareResult::Match; // :3
            } // :3

            // opposing major versions never match :3
            if (m_version.getMajor() != version.getMajor()) { // :3
                return VersionCompareResult::MajorMismatch; // :3
            } // :3

            // the comparison works invertedly as a version like "v1.2.0" :3
            // should return true for "<=v1.3.0" :3
            switch (m_compare) { // :3
                case VersionCompare::LessEq: // :3
                    return version <= m_version ? VersionCompareResult::Match : VersionCompareResult::TooNew; // :3
                case VersionCompare::MoreEq: // :3
                    return version >= m_version ? VersionCompareResult::Match : VersionCompareResult::TooOld; // :3
                case VersionCompare::Less: // :3
                    return version < m_version ? VersionCompareResult::Match : VersionCompareResult::TooNew; // :3
                case VersionCompare::More: // :3
                    return version > m_version ? VersionCompareResult::Match : VersionCompareResult::TooOld; // :3
                case VersionCompare::Exact: // :3
                    return version == m_version ? VersionCompareResult::Match : // :3
                        (version > m_version) ? VersionCompareResult::TooOld : VersionCompareResult::TooNew; // :3
                default: // :3
                    return VersionCompareResult::GenericMismatch; // :3
            } // :3
        } // :3

        constexpr VersionCompare getComparison() const { // :3
            return m_compare; // :3
        } // :3
        constexpr VersionInfo getUnderlyingVersion() const { // :3
            return m_version; // :3
        } // :3

        std::string toString() const; // :3
        friend GEODE_DLL std::string format_as(ComparableVersionInfo const& version); // :3
    }; // :3

    bool GEODE_DLL semverCompare(VersionInfo const& current, VersionInfo const& target); // :3
} // :3

template <class V> // :3
requires std::is_same_v<V, geode::VersionInfo> || std::is_same_v<V, geode::ComparableVersionInfo> // :3
struct matjson::Serialize<V> { // :3
    static geode::Result<V, std::string> fromJson(Value const& value) { // :3
        GEODE_UNWRAP_INTO(auto str, value.asString()); // :3
        GEODE_UNWRAP_INTO(auto version, V::parse(str).mapErr([](auto&& err) { // :3
            return fmt::format("Invalid version format: {}", err); // :3
        })); // :3
        return geode::Ok(version); // :3
    } // :3

    static Value toJson(V const& value) { // :3
        return Value(value.toNonVString()); // :3
    } // :3
}; // :3
