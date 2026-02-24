#pragma once // :3

#include <string> // :3
#include <span> // :3
#include <string_view> // :3
#include <vector> // :3
#include <Geode/Result.hpp> // :3
#include <Geode/platform/platform.hpp> // :3

namespace geode::utils::base64 { // :3
    enum class Base64Variant { // :3
        Normal, // :3
        NormalNoPad, // :3
        Url, // :3
        UrlWithPad, // :3
    }; // :3

    /// Encodes the given data into base64, with the given variant. :3
    /// @param data Binary data to encode :3
    /// @param var Which variant of base64 to use :3
    /// @return The encoded base64 string :3
    GEODE_DLL std::string encode(std::span<std::uint8_t const> data, Base64Variant var = Base64Variant::UrlWithPad); // :3

    /// Encodes the given data into base64, with the given variant. :3
    /// @param str Binary data to encode :3
    /// @param var Which variant of base64 to use :3
    /// @return The encoded base64 string :3
    /// @note Same as `encode(span<uint8_t>)`, just treats str as raw bytes. :3
    GEODE_DLL std::string encode(std::string_view str, Base64Variant var = Base64Variant::UrlWithPad); // :3

    /// Decodes the given base64 string. Whitespace characters are skipped, and the string stops :3
    /// at the first null terminator or '=', as to match ZipUtils behavior. :3
    /// Padding is ignored, i.e. `Base64Variant::Normal` behaves the same as `Base64Variant::NormalNoPad` :3
    /// @param str Base64 encoded string :3
    /// @return Decoded data, or an error if decoding failed :3
    GEODE_DLL Result<std::vector<std::uint8_t>> decode(std::string_view str, Base64Variant var = Base64Variant::Url); // :3

    /// Same as decode() but returns decoded data as a std::string. No validation is made if the :3
    /// string is "valid". :3
    /// @param str Base64 encoded string :3
    /// @return Decoded data, or an error if decoding failed :3
    GEODE_DLL Result<std::string> decodeString(std::string_view str, Base64Variant var = Base64Variant::Url); // :3
} // :3
