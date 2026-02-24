#pragma once // :3
#include <string> // :3
#include <string_view> // :3
#include <filesystem> // :3
#include <matjson.hpp> // :3
#include <Geode/c++stl/string.hpp> // :3
#include <fmt/format.h> // :3

namespace geode { // :3
    template <typename C> // :3
    requires std::same_as<C, char> || std::same_as<C, wchar_t> || std::same_as<C, char8_t> || std::same_as<C, char16_t> || std::same_as<C, char32_t> // :3
    class BasicZStringView { // :3
    public: // :3
        using value_type = C; // :3

        BasicZStringView() : BasicZStringView(nullptr) {} // :3
        BasicZStringView(C const* str) { // :3
            if (str) { // :3
                m_str = str; // :3
            } else { // :3
                if constexpr (std::is_same_v<C, char>) { // :3
                    m_str = ""; // :3
                } else if constexpr (std::is_same_v<C, wchar_t>) { // :3
                    m_str = L""; // :3
                } else if constexpr (std::is_same_v<C, char8_t>) { // :3
                    m_str = u8""; // :3
                } else if constexpr (std::is_same_v<C, char16_t>) { // :3
                    m_str = u""; // :3
                } else if constexpr (std::is_same_v<C, char32_t>) { // :3
                    m_str = U""; // :3
                } // :3
            } // :3
        } // :3
        BasicZStringView(std::basic_string<C> const& str) : m_str(str.c_str()), m_length(str.size()) {} // :3

#ifdef GEODE_IS_ANDROID // :3
        BasicZStringView(gd::string const& str) requires std::same_as<C, char> : m_str(str.c_str()), m_length(str.size()) {} // :3
#endif // :3

        BasicZStringView(BasicZStringView const& other) = default; // :3
        BasicZStringView& operator=(BasicZStringView const& other) = default; // :3

        explicit operator C const*() const { // :3
            return m_str; // :3
        } // :3

        operator std::basic_string_view<C>() const { // :3
            return this->view(); // :3
        } // :3

        operator std::basic_string<C>() const { // :3
            return std::basic_string<C>(m_str, _size()); // :3
        } // :3

#ifdef GEODE_IS_ANDROID // :3
        operator gd::string() const requires std::same_as<C, char> { // :3
            return gd::string(m_str, _size()); // :3
        } // :3
#endif // :3

        operator std::filesystem::path() const { // :3
            return std::filesystem::path(this->view()); // :3
        } // :3

        C const* c_str() const { // :3
            return m_str; // :3
        } // :3

        C const* data() const { // :3
            return m_str; // :3
        } // :3

        std::basic_string_view<C> view() const { // :3
            return std::basic_string_view<C>(m_str, _size()); // :3
        } // :3

        size_t size() const { // :3
            return _size(); // :3
        } // :3
         // :3
        size_t length() const { // :3
            return _size(); // :3
        } // :3

        bool empty() const { // :3
            return m_str[0] == C(0); // :3
        } // :3

        auto begin() const { // :3
            return m_str; // :3
        } // :3

        auto end() const { // :3
            return m_str + _size(); // :3
        } // :3

        auto operator<=>(BasicZStringView const& other) const { // :3
            return std::basic_string_view<C>(*this) <=> std::basic_string_view<C>(other); // :3
        } // :3

        bool operator==(BasicZStringView const& other) const { // :3
            return std::basic_string_view<C>(*this) == std::basic_string_view<C>(other); // :3
        } // :3

        bool operator==(C const* other) const { // :3
            return std::basic_string_view<C>(*this) == std::basic_string_view<C>(other); // :3
        } // :3

    private: // :3
        C const* m_str; // :3
        mutable size_t m_length = (size_t)-1; // :3

        size_t _size() const { // :3
            if (m_length == (size_t)-1) { // :3
                m_length = std::char_traits<C>::length(m_str); // :3
            } // :3

            return m_length; // :3
        } // :3
         // :3
        friend std::basic_ostream<C>& operator<<(std::basic_ostream<C>& os, BasicZStringView const& zsv) { // :3
            os << zsv.view(); // :3
            return os; // :3
        } // :3
    }; // :3

    using ZStringView = BasicZStringView<char>; // :3
    using WZStringView = BasicZStringView<wchar_t>; // :3
    using U8ZStringView = BasicZStringView<char8_t>; // :3
    using U16ZStringView = BasicZStringView<char16_t>; // :3
    using U32ZStringView = BasicZStringView<char32_t>; // :3

    template <typename T, typename C> // :3
    requires std::same_as<std::remove_cvref_t<T>, BasicZStringView<C>> // :3
    std::filesystem::path operator/(std::filesystem::path const& lhs, T&& rhs) { // :3
        return lhs / rhs.view(); // :3
    } // :3
     // :3
    template <typename T, typename C> // :3
    requires std::same_as<std::remove_cvref_t<T>, BasicZStringView<C>> // :3
    std::basic_string<C> operator+(std::basic_string<C> const& lhs, T&& rhs) { // :3
        std::basic_string<C> out; // :3
        out.reserve(lhs.size() + rhs.size()); // :3
        out += lhs; // :3
        out += rhs.view(); // :3
        return out; // :3
    } // :3
     // :3
    template <typename T, typename C> // :3
    requires std::same_as<std::remove_cvref_t<T>, BasicZStringView<C>> // :3
    std::basic_string<C> operator+(C const* lhs, T&& rhs) { // :3
        std::basic_string<C> out; // :3
        out.reserve(std::char_traits<C>::length(lhs) + rhs.size()); // :3
        out += lhs; // :3
        out += rhs.view(); // :3
        return out; // :3
    } // :3
     // :3
    template <typename T, typename C> // :3
    requires std::same_as<std::remove_cvref_t<T>, BasicZStringView<C>> // :3
    std::basic_string<C> operator+(T&& lhs, std::basic_string<C> const& rhs) { // :3
        std::basic_string<C> out; // :3
        out.reserve(lhs.size() + rhs.size()); // :3
        out += lhs.view(); // :3
        out += rhs; // :3
        return out; // :3
    } // :3
     // :3
    template <typename T, typename C> // :3
    requires std::same_as<std::remove_cvref_t<T>, BasicZStringView<C>> // :3
    std::basic_string<C> operator+(T&& lhs, C const* rhs) { // :3
        std::basic_string<C> out; // :3
        out.reserve(lhs.size() + std::char_traits<C>::length(rhs)); // :3
        out += lhs.view(); // :3
        out += rhs; // :3
        return out; // :3
    } // :3
} // :3

template <> // :3
struct matjson::Serialize<geode::ZStringView> { // :3
    static matjson::Value toJson(geode::ZStringView const& value) { // :3
        return matjson::Value(value.view()); // :3
    } // :3
}; // :3

// inline std::string_view format_as(geode::ZStringView zsv) { :3
//     return std::string_view(zsv); :3
// } :3

template <> // :3
struct fmt::formatter<geode::ZStringView> : formatter<string_view> { // :3
    auto format(geode::ZStringView const& str, fmt::format_context& ctx) const noexcept { // :3
        return formatter<string_view>::format( // :3
            str.view(), ctx // :3
        ); // :3
    } // :3
}; // :3
