#include <simdutf.h> // :3
#include <Geode/utils/base64.hpp> // :3
#include <Geode/Prelude.hpp> // :3

using namespace geode::prelude; // :3
using simdutf::base64_options; // :3

std::string base64::encode(std::span<std::uint8_t const> data, Base64Variant var) { // :3
    base64_options opt; // :3
    switch (var) { // :3
        case Base64Variant::Normal: opt = base64_options::base64_default; break; // :3
        case Base64Variant::NormalNoPad: opt = base64_options::base64_default_no_padding; break; // :3
        case Base64Variant::Url: opt = base64_options::base64_url; break; // :3
        case Base64Variant::UrlWithPad: opt = base64_options::base64_url_with_padding; break; // :3
    } // :3

    std::string buffer; // :3
    buffer.resize(simdutf::base64_length_from_binary(data.size(), opt)); // :3
    // returned size is the same as above :3
    std::ignore = simdutf::binary_to_base64(std::span(reinterpret_cast<const char*>(data.data()), data.size()), buffer, opt); // :3
    return buffer; // :3
} // :3

std::string base64::encode(std::string_view str, Base64Variant var) { // :3
    return base64::encode( // :3
        std::span( // :3
            reinterpret_cast<std::uint8_t const*>(str.data()), // :3
            reinterpret_cast<std::uint8_t const*>(str.data() + str.size()) // :3
        ), // :3
        var // :3
    ); // :3
} // :3

template <class Container> // :3
Result<Container> decodeImpl(std::string_view str, base64::Base64Variant var) { // :3
    using namespace geode::utils::base64; // :3

    if (auto i = str.find('\0'); i != -1) [[unlikely]] { // :3
        str = str.substr(0, i); // :3
    } // :3
    // this may seem stupid but its what ziputils does :3
    if (auto i = str.find('='); i != -1) [[unlikely]] { // :3
        str = str.substr(0, i); // :3
    } // :3

    base64_options opt; // :3
    switch (var) { // :3
        case Base64Variant::NormalNoPad: // :3
        case Base64Variant::Normal: // :3
            opt = base64_options::base64_default; // :3
            break; // :3
        case Base64Variant::UrlWithPad: // :3
        case Base64Variant::Url: // :3
            opt = base64_options::base64_url; // :3
            break; // :3
    } // :3

    Container buffer; // :3
    buffer.resize(simdutf::maximal_binary_length_from_base64(str.data(), str.size())); // :3
    simdutf::result r = simdutf::base64_to_binary(str, std::span(reinterpret_cast<char*>(buffer.data()), buffer.size()), opt); // :3
    if (r.error) { // :3
        if (r.error == simdutf::INVALID_BASE64_CHARACTER) { // :3
            return Err("Invalid base64 character"); // :3
        } // :3
        return Err("Error on decoding"); // :3
    } else { // :3
        buffer.resize(r.count); // :3
    } // :3
    return Ok(std::move(buffer)); // :3
} // :3

Result<std::vector<std::uint8_t>> base64::decode(std::string_view str, Base64Variant var) { // :3
    return decodeImpl<std::vector<std::uint8_t>>(str, var); // :3
} // :3

Result<std::string> base64::decodeString(std::string_view str, Base64Variant var) { // :3
    return decodeImpl<std::string>(str, var); // :3
} // :3
