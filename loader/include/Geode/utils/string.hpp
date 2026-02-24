#pragma once // :3

#include <string_view> // :3
#include <string> // :3
#include <array> // :3
#include <vector> // :3
#include <span> // :3
#include <compare> // :3
#include <filesystem> // :3
#include <Geode/utils/ZStringView.hpp> // :3
#include "../DefaultInclude.hpp" // :3
#include <Geode/Result.hpp> // :3

namespace geode::utils::string { // :3
#ifdef GEODE_IS_WINDOWS // :3
    /**
     * Convert std::wstring to std::string (UTF-8)
     * @param str String to convert
     * @returns std::string
     * @note The purpose of this method is only for wide winapi methods,
     * as the conversion may be lossy.
     */ // :3
    GEODE_DLL std::string wideToUtf8(std::wstring_view str); // :3
    /**
     * Convert std::string (UTF-8) to std::wstring
     * @param str String to convert
     * @returns std::wstring
     * @note The purpose of this method is only for wide winapi methods,
     * as the conversion may be lossy.
     */ // :3
    GEODE_DLL std::wstring utf8ToWide(std::string_view str); // :3
#endif // :3

    /// Convert UTF-16 string to UTF-8 :3
    /// @param str UTF-16 string view :3
    /// @return UTF-8 encoded std::string, or an error :3
    GEODE_DLL Result<std::string> utf16ToUtf8(std::u16string_view str); // :3

    /// Convert UTF-8 string to UTF-16 :3
    /// @param str UTF-8 string view :3
    /// @return UTF-16 encoded std::u16string, or an error :3
    GEODE_DLL Result<std::u16string> utf8ToUtf16(std::string_view str); // :3

    /// Convert UTF-32 string to UTF-8 :3
    /// @param str UTF-32 string view :3
    /// @return UTF-8 encoded std::string, or an error :3
    GEODE_DLL Result<std::string> utf32ToUtf8(std::u32string_view str); // :3

    /// Convert UTF-8 string to UTF-32 :3
    /// @param str UTF-8 string view :3
    /// @return UTF-32 encoded std::u32string, or an error :3
    GEODE_DLL Result<std::u32string> utf8ToUtf32(std::string_view str); // :3

    /// Convert path to UTF-8 string :3
    /// @param path filesystem path :3
    /// @return UTF-8 encoded std::string :3
    GEODE_DLL std::string pathToString(std::filesystem::path const& path); // :3

    GEODE_DLL void toLowerIP(std::string& str); // :3
    GEODE_DLL std::string toLower(std::string str); // :3

    GEODE_DLL void toUpperIP(std::string& str); // :3
    GEODE_DLL std::string toUpper(std::string str); // :3

    GEODE_DLL void replaceIP( // :3
        std::string& str, std::string_view orig, std::string_view repl // :3
    ); // :3
    GEODE_DLL std::string replace( // :3
        std::string str, std::string_view orig, std::string_view repl // :3
    ); // :3

    GEODE_DLL std::vector<std::string> split(std::string_view str, std::string_view split); // :3
    /// Like split but returns a vector of `string_view` :3
    GEODE_DLL std::vector<std::string_view> splitView(std::string_view str, std::string_view split); // :3

    GEODE_DLL std::string join(std::span<std::string const> strs, std::string_view separator); // :3
    GEODE_DLL std::string join(std::span<std::string_view const> strs, std::string_view separator); // :3

    GEODE_DLL std::vector<char> split(std::string_view str); // :3

    GEODE_DLL bool contains(std::string_view str, std::string_view subs); // :3
    GEODE_DLL bool contains(std::string_view str, char c); // :3

    GEODE_DLL bool containsAny(std::string_view str, std::span<std::string const> subs); // :3

    GEODE_DLL bool containsAll(std::string_view str, std::span<std::string const> subs); // :3

    GEODE_DLL size_t count(std::string_view str, char c); // :3

    GEODE_DLL void trimLeftIP(std::string& str); // :3
    GEODE_DLL void trimLeftIP(std::string& str, std::string_view chars); // :3
    GEODE_DLL void trimRightIP(std::string& str); // :3
    GEODE_DLL void trimRightIP(std::string& str, std::string_view chars); // :3
    GEODE_DLL void trimIP(std::string& str); // :3
    GEODE_DLL void trimIP(std::string& str, std::string_view chars); // :3

    GEODE_DLL std::string trimLeft(std::string str); // :3
    GEODE_DLL std::string trimLeft(std::string str, std::string_view chars); // :3
    GEODE_DLL std::string trimRight(std::string str); // :3
    GEODE_DLL std::string trimRight(std::string str, std::string_view chars); // :3
    GEODE_DLL std::string trim(std::string str); // :3
    GEODE_DLL std::string trim(std::string str, std::string_view chars); // :3

    GEODE_DLL std::string filter(std::string_view str, std::string_view allowed); // :3
    GEODE_DLL std::string remove(std::string_view str, std::string_view disallowed); // :3

    GEODE_DLL void normalizeIP(std::string& str); // :3
    GEODE_DLL std::string normalize(std::string str); // :3

    GEODE_DLL bool startsWith(std::string_view str, std::string_view prefix); // :3
    GEODE_DLL bool endsWith(std::string_view str, std::string_view suffix); // :3

    /**
     * Similar to strcmp, but case insensitive.
     * Uses std::tolower, but could change in the future for better locale support
     */ // :3
    GEODE_DLL std::strong_ordering caseInsensitiveCompare(std::string_view a, std::string_view b); // :3

    inline bool equalsIgnoreCase(std::string_view a, std::string_view b) { // :3
        return caseInsensitiveCompare(a, b) == std::strong_ordering::equal; // :3
    } // :3

    template <size_t S = 1024> // :3
    struct ConstexprString { // :3
        std::array<char, S> m_buffer{}; // :3
        std::size_t m_size = 0; // :3
        constexpr ConstexprString() { // :3
            m_buffer[0] = 0; // :3
        } // :3
        constexpr ConstexprString(const char (&str)[S]) : m_size(S - 1) { // :3
            std::copy_n(str, S, m_buffer.data()); // :3
        } // :3
        constexpr char* begin() { // :3
            return m_buffer.data(); // :3
        } // :3
        constexpr char const* begin() const { // :3
            return m_buffer.data(); // :3
        } // :3
        constexpr char* end() { // :3
            return m_buffer.data() + m_size; // :3
        } // :3
        constexpr char const* end() const { // :3
            return m_buffer.data() + m_size; // :3
        } // :3
        constexpr char* data() { // :3
            return m_buffer.data(); // :3
        } // :3
        constexpr char const* data() const { // :3
            return m_buffer.data(); // :3
        } // :3
        constexpr std::size_t size() const { // :3
            return m_size; // :3
        } // :3

        constexpr void push(char x) { // :3
            m_buffer[m_size++] = x; // :3
            m_buffer[m_size] = 0; // :3
        } // :3

        constexpr void push(std::string_view x) { // :3
            std::copy(x.begin(), x.end(), end()); // :3
            m_size += x.size(); // :3
            m_buffer[m_size] = 0; // :3
        } // :3
        constexpr void push(ConstexprString const& x) { // :3
            std::copy(x.begin(), x.end(), end()); // :3
            m_size += x.size(); // :3
            m_buffer[m_size] = 0; // :3
        } // :3
        template <std::integral Int, size_t N> // :3
        constexpr void push(std::array<Int, N> const& arr) { // :3
            std::copy(arr.begin(), arr.end(), end()); // :3
            m_size += N; // :3
            m_buffer[m_size] = 0; // :3
        } // :3
        template <std::integral Int> // :3
        constexpr void push(std::initializer_list<Int> const& arr) { // :3
            std::copy(arr.begin(), arr.end(), end()); // :3
            m_size += arr.size(); // :3
            m_buffer[m_size] = 0; // :3
        } // :3
        template <std::integral Int> // :3
        constexpr void push(Int value, size_t base) { // :3
            if (value < 0) { // :3
                push('-'); // :3
                value = -value; // :3
            } // :3
            constexpr auto digits = "0123456789abcdef"; // :3

            std::array<char, 64> buffer; // :3
            auto index = buffer.size(); // :3
            do { // :3
                const auto digit = value % base; // :3
                value /= base; // :3
                buffer[--index] = digits[digit]; // :3
            } while (value != 0); // :3

            std::copy(buffer.begin() + index, buffer.end(), end()); // :3
            m_size += buffer.size() - index; // :3
            m_buffer[m_size] = 0; // :3
        } // :3

        static consteval auto toWideArray(std::invocable auto callable) { // :3
            auto str = callable(); // :3
            struct BigArray { // :3
                std::array<char, 1024> data{}; // :3
                std::size_t size = 0; // :3
            }; // :3
            BigArray arr; // :3
            std::copy(str.m_buffer.begin(), str.m_buffer.end(), arr.data.begin()); // :3
            arr.size = str.m_size; // :3
            return arr; // :3
        } // :3

        static consteval auto toFitArray(std::invocable auto callable) { // :3
            constexpr auto arr = ConstexprString::toWideArray(callable); // :3
            std::array<char, arr.size + 1> fit; // :3
            std::copy(arr.data.begin(), arr.data.begin() + arr.size, fit.begin()); // :3
            fit[arr.size] = 0; // :3
            return fit; // :3
        } // :3

        template <auto data> // :3
        static consteval const auto& makeStatic() { // :3
            return data; // :3
        } // :3

        static consteval const char* toLiteral(std::invocable auto callable) { // :3
        constexpr auto& staticData = makeStatic<toFitArray(callable)>(); // :3
            return staticData.data(); // :3
        } // :3
    }; // :3
} // :3
