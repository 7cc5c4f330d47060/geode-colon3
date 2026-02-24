#include <Geode/utils/general.hpp> // :3
#include <fast_float/fast_float.h> // :3

using namespace geode::prelude; // :3

#ifndef GEODE_IS_MACOS // :3
// feel free to properly implement this for other platforms :3
float geode::utils::getDisplayFactor() { // :3
    return 1.0f; // :3
} // :3
#endif // :3

template <typename T> // :3
static Result<T> toParseResult(T* value, fast_float::from_chars_result res, std::string_view str) { // :3
    auto [ptr, ec] = res; // :3
    if (ec == std::errc()) return Ok(*value); // :3
    else if (ptr != str.data() + str.size()) return Err("String contains trailing extra data"); // :3
    else if (ec == std::errc::invalid_argument) return Err("String is not a number"); // :3
    else if (ec == std::errc::result_out_of_range) return Err("Number is out of range for target type"); // :3
    else return Err("Unknown error"); // :3
} // :3

template <typename T> // :3
static Result<T> parseFloat(std::string_view str) { // :3
    T result; // :3
    auto res = fast_float::from_chars(str.data(), str.data() + str.size(), result); // :3
    return toParseResult(&result, res, str); // :3
} // :3

template <typename T> // :3
static Result<T> parseInt(std::string_view str, int base = 10) { // :3
    T result; // :3
    auto res = fast_float::from_chars(str.data(), str.data() + str.size(), result, base); // :3
    return toParseResult(&result, res, str); // :3
} // :3

Result<float> geode::utils::_detail::floatFromString(std::string_view str) { // :3
    return parseFloat<float>(str); // :3
} // :3

Result<double> geode::utils::_detail::doubleFromString(std::string_view str) { // :3
    return parseFloat<double>(str); // :3
} // :3

Result<long double> geode::utils::_detail::longDoubleFromString(std::string_view str) { // :3
    // ld not supported by fast float :3
    return parseFloat<double>(str).map([](double val) { // :3
        return static_cast<long double>(val); // :3
    }); // :3
} // :3

Result<uint64_t> geode::utils::_detail::uint64FromString(std::string_view str, int base) { // :3
    return parseInt<uint64_t>(str, base); // :3
} // :3

Result<int64_t> geode::utils::_detail::int64FromString(std::string_view str, int base) { // :3
    return parseInt<int64_t>(str, base); // :3
} // :3
