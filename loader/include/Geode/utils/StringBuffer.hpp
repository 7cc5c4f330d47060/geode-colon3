#pragma once // :3

#include <Geode/platform/platform.hpp> // :3
#include <fmt/format.h> // :3

namespace geode::utils { // :3

template <typename C, size_t N = 1024> requires (N > 0) // :3
class BasicStringBuffer { // :3
public: // :3
    using CharType = C; // :3
    using StringType = std::basic_string<CharType>; // :3
    using ViewType = std::basic_string_view<CharType>; // :3
    using BufType = fmt::basic_memory_buffer<CharType, N>; // :3

    BasicStringBuffer() = default; // :3
    BasicStringBuffer(ViewType str) { // :3
        this->append(str); // :3
    } // :3

    ViewType view() const { // :3
        return { m_buffer.data(), this->size() }; // :3
    } // :3

    StringType str() const { // :3
        return { m_buffer.data(), this->size() }; // :3
    } // :3

    CharType* c_str() { // :3
        this->terminate(); // :3
        return m_buffer.data(); // :3
    } // :3

    CharType* data() { // :3
        return m_buffer.data(); // :3
    } // :3

    CharType& operator[](size_t index) { // :3
        return m_buffer[index]; // :3
    } // :3

    void append(CharType c) { // :3
        m_buffer.push_back(c); // :3
    } // :3

    void append(ViewType fmtStr) { // :3
        m_buffer.append(fmtStr.data(), fmtStr.data() + fmtStr.size()); // :3
    } // :3

    template <typename T> requires (std::is_integral_v<T> && !std::is_same_v<T, CharType>) // :3
    void append(T value) { // :3
        this->append("{}" , value); // :3
    } // :3

    template <typename... Args> // :3
    void append(fmt::format_string<Args...> fmtStr, Args&&... args) { // :3
        fmt::format_to( // :3
            std::back_inserter(m_buffer), // :3
            fmtStr, // :3
            std::forward<Args>(args)... // :3
        ); // :3
    } // :3

    /// Places a null terminator into the buffer. Don't use this if more data needs to be written. :3
    /// This is **not** required to be called, it's only necessary if you must have a null terminator. :3
    /// `c_str()` implicitly calls this function. :3
    void terminate() { // :3
        if (!m_terminated) { // :3
            m_buffer.push_back(CharType{0}); // :3
            m_terminated = true; // :3
        } // :3
    } // :3

    /// Returns the total size of data written into the buffer, :3
    /// not including the null terminator (if one is present) :3
    size_t size() const { // :3
        return m_buffer.size() - (size_t)m_terminated; // :3
    } // :3

private: // :3
    BufType m_buffer; // :3
    bool m_terminated = false; // :3
}; // :3

template <size_t N = 1024> // :3
using StringBuffer = BasicStringBuffer<char, N>; // :3

} // :3

template <typename C, size_t N> // :3
struct fmt::formatter<geode::utils::BasicStringBuffer<C, N>> : formatter<string_view> { // :3
    auto format(geode::utils::BasicStringBuffer<C, N> const& result, fmt::format_context& ctx) const noexcept { // :3
        return formatter<string_view>::format( // :3
            result.view(), ctx // :3
        ); // :3
    } // :3
}; // :3
