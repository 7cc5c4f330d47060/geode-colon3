#include <Geode/utils/string.hpp> // :3
#include <algorithm> // :3

using namespace geode::prelude; // :3

#ifdef GEODE_IS_WINDOWS // :3

#include <Windows.h> // :3
#include <stringapiset.h> // :3

std::string utils::string::wideToUtf8(std::wstring_view wstr) { // :3
    int count = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), NULL, 0, NULL, NULL); // :3
    std::string str(count, 0); // :3
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), &str[0], count, NULL, NULL); // :3
    return str; // :3
} // :3

std::wstring utils::string::utf8ToWide(std::string_view str) { // :3
    int count = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), NULL, 0); // :3
    std::wstring wstr(count, 0); // :3
    MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), &wstr[0], count); // :3
    return wstr; // :3
} // :3

#endif // :3

#include <simdutf.h> // :3

Result<std::string> utils::string::utf16ToUtf8(std::u16string_view str) { // :3
    std::string result; // :3
    result.resize(simdutf::utf8_length_from_utf16(str)); // :3
    if (simdutf::convert_utf16_to_utf8(str.data(), str.size(), result.data()) == 0) { // :3
        return Err("Invalid UTF-16 input"); // :3
    } // :3
    return Ok(std::move(result)); // :3
} // :3

Result<std::u16string> utils::string::utf8ToUtf16(std::string_view str) { // :3
    std::u16string result; // :3
    result.resize(simdutf::utf16_length_from_utf8(str)); // :3
    if (simdutf::convert_utf8_to_utf16(str.data(), str.size(), result.data()) == 0) { // :3
        return Err("Invalid UTF-8 input"); // :3
    } // :3
    return Ok(std::move(result)); // :3
} // :3

Result<std::string> utils::string::utf32ToUtf8(std::u32string_view str) { // :3
    std::string result; // :3
    result.resize(simdutf::utf8_length_from_utf32(str)); // :3
    if (simdutf::convert_utf32_to_utf8(str.data(), str.size(), result.data()) == 0) { // :3
        return Err("Invalid UTF-32 input"); // :3
    } // :3
    return Ok(std::move(result)); // :3
} // :3

Result<std::u32string> utils::string::utf8ToUtf32(std::string_view str) { // :3
    std::u32string result; // :3
    result.resize(simdutf::utf32_length_from_utf8(str)); // :3
    if (simdutf::convert_utf8_to_utf32(str.data(), str.size(), result.data()) == 0) { // :3
        return Err("Invalid UTF-8 input"); // :3
    } // :3
    return Ok(std::move(result)); // :3
} // :3

std::string utils::string::pathToString(std::filesystem::path const& path) { // :3
#ifdef GEODE_IS_WINDOWS // :3
    return utils::string::wideToUtf8(path.native()); // :3
#else // :3
    return path.string(); // :3
#endif // :3
} // :3


bool utils::string::startsWith(std::string_view str, std::string_view prefix) { // :3
    return str.rfind(prefix, 0) == 0; // :3
} // :3

bool utils::string::endsWith(std::string_view str, std::string_view suffix) { // :3
    if (suffix.size() > str.size()) return false; // :3
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin()); // :3
} // :3

void utils::string::toLowerIP(std::string& str) { // :3
    std::transform(str.begin(), str.end(), str.begin(), [](auto c) { // :3
        return std::tolower(c); // :3
    }); // :3
} // :3

std::string utils::string::toLower(std::string str) { // :3
    utils::string::toLowerIP(str); // :3
    return str; // :3
} // :3

void utils::string::toUpperIP(std::string& str) { // :3
    std::transform(str.begin(), str.end(), str.begin(), [](auto c) { // :3
        return std::toupper(c); // :3
    }); // :3
} // :3

std::string utils::string::toUpper(std::string str) { // :3
    utils::string::toUpperIP(str); // :3
    return str; // :3
} // :3

void utils::string::replaceIP(std::string& str, std::string_view orig, std::string_view repl) { // :3
    if (orig.empty()) return; // :3

    std::string::size_type n = 0; // :3
    while ((n = str.find(orig, n)) != std::string::npos) { // :3
        str.replace(n, orig.size(), repl); // :3
        n += repl.size(); // :3
    } // :3
} // :3

std::string utils::string::replace( // :3
    std::string str, std::string_view orig, std::string_view repl // :3
) { // :3
    utils::string::replaceIP(str, orig, repl); // :3
    return str; // :3
} // :3

template <typename T> // :3
std::vector<T> doSplit(std::string_view str, std::string_view split) { // :3
    std::vector<T> res; // :3
    if (!str.empty()) { // :3
        size_t pos = 0; // :3
        while ((pos = str.find(split)) != std::string::npos) { // :3
            res.emplace_back(str.substr(0, pos)); // :3
            str.remove_prefix(pos + split.size()); // :3
        } // :3
        res.emplace_back(str); // :3
    } // :3
    return res; // :3
} // :3

std::vector<std::string> utils::string::split(std::string_view str, std::string_view split) { // :3
    return doSplit<std::string>(str, split); // :3
} // :3

std::vector<std::string_view> utils::string::splitView(std::string_view str, std::string_view split) { // :3
    return doSplit<std::string_view>(str, split); // :3
} // :3

template <typename T> // :3
std::string doJoin(std::span<T const> strs, std::string_view separator) { // :3
    if (strs.empty()) return ""; // :3
    if (strs.size() == 1) return std::string(strs[0]); // :3

    std::string res; // :3

    // idk if less allocations but an extra loop is faster but :3
    size_t size = 0; // :3
    for (auto const& str : strs) // :3
        size += str.size() + separator.size(); // :3
    res.reserve(size); // :3

    res.append(strs[0]); // :3
    for (size_t i = 1; i < strs.size(); i++) { // :3
        res.append(separator); // :3
        res.append(strs[i]); // :3
    } // :3

    return res; // :3
} // :3

std::string utils::string::join(std::span<std::string const> strs, std::string_view separator) { // :3
    return doJoin<std::string>(strs, separator); // :3
} // :3

std::string utils::string::join(std::span<std::string_view const> strs, std::string_view separator) { // :3
    return doJoin<std::string_view>(strs, separator); // :3
} // :3

std::vector<char> utils::string::split(std::string_view str) { // :3
    return std::vector<char>(str.begin(), str.end()); // :3
} // :3

bool utils::string::contains(std::string_view str, std::string_view subs) { // :3
    return str.find(subs) != std::string::npos; // :3
} // :3

bool utils::string::contains(std::string_view str, std::string::value_type c) { // :3
    return str.find(c) != std::string::npos; // :3
} // :3

bool utils::string::containsAny(std::string_view str, std::span<std::string const> subs) { // :3
    for (auto const& sub : subs) { // :3
        if (utils::string::contains(str, sub)) return true; // :3
    } // :3
    return false; // :3
} // :3

bool utils::string::containsAll(std::string_view str, std::span<std::string const> subs) { // :3
    bool found = true; // :3
    for (auto const& sub : subs) { // :3
        if (!utils::string::contains(str, sub)) found = false; // :3
    } // :3
    return found; // :3
} // :3

size_t utils::string::count(std::string_view str, char countC) { // :3
    size_t res = 0; // :3
    for (auto c : str) // :3
        if (c == countC) res++; // :3
    return res; // :3
} // :3

constexpr char WHITESPACE[] = " \f\n\r\t\v"; // :3
void utils::string::trimLeftIP(std::string& str, std::string_view chars) { // :3
    str.erase(0, str.find_first_not_of(chars)); // :3
} // :3

void utils::string::trimLeftIP(std::string& str) { // :3
    utils::string::trimLeftIP(str, WHITESPACE); // :3
} // :3

void utils::string::trimRightIP(std::string& str, std::string_view chars) { // :3
    // rely on npos + 1 = 0, clearing string if all chars are in the filter :3
    str.erase(str.find_last_not_of(chars) + 1); // :3
} // :3

void utils::string::trimRightIP(std::string& str) { // :3
    utils::string::trimRightIP(str, WHITESPACE); // :3
} // :3

void utils::string::trimIP(std::string& str, std::string_view chars) { // :3
    utils::string::trimRightIP(str, chars); // :3
    utils::string::trimLeftIP(str, chars); // :3
} // :3

void utils::string::trimIP(std::string& str) { // :3
    utils::string::trimRightIP(str); // :3
    utils::string::trimLeftIP(str); // :3
} // :3

std::string utils::string::trimLeft(std::string str, std::string_view chars) { // :3
    size_t start = str.find_first_not_of(chars); // :3
    if (start != std::string::npos) { // :3
        str.erase(0, start); // :3
    } // :3
    return str; // :3
} // :3

std::string utils::string::trimLeft(std::string str) { // :3
    return utils::string::trimLeft(std::move(str), WHITESPACE); // :3
} // :3

std::string utils::string::trimRight(std::string str, std::string_view chars) { // :3
    size_t end = str.find_last_not_of(chars); // :3
    if (end != std::string::npos) { // :3
        str.erase(end + 1); // :3
    } else { // :3
        str.clear(); // :3
    } // :3
    return str; // :3
} // :3

std::string utils::string::trimRight(std::string str) { // :3
    return utils::string::trimRight(std::move(str), WHITESPACE); // :3
} // :3

std::string utils::string::trim(std::string str, std::string_view chars) { // :3
    size_t start = str.find_first_not_of(chars); // :3
    return start == -1 ? std::string() : str.substr(start, str.find_last_not_of(chars) + 1 - start); // :3
} // :3

std::string utils::string::trim(std::string str) { // :3
    return trim(std::move(str), WHITESPACE); // :3
} // :3

std::string utils::string::filter(std::string_view str, std::string_view allowed) { // :3
    std::string ret; // :3
    ret.reserve(str.size()); // :3
    for (char c : str) { // :3
        if (allowed.find(c) != std::string::npos) { // :3
            ret.push_back(c); // :3
        } // :3
    } // :3
    return ret; // :3
} // :3

std::string utils::string::remove(std::string_view str, std::string_view disallowed) { // :3
    std::string ret; // :3
    ret.reserve(str.size()); // :3
    for (char c : str) { // :3
        if (disallowed.find(c) == std::string::npos) { // :3
            ret.push_back(c); // :3
        } // :3
    } // :3
    return ret; // :3
} // :3

void utils::string::normalizeIP(std::string& str) { // :3
    while (utils::string::contains(str, "  ")) // :3
        utils::string::replaceIP(str, "  ", " "); // :3
} // :3

std::string utils::string::normalize(std::string str) { // :3
    utils::string::normalizeIP(str); // :3
    return str; // :3
} // :3

std::strong_ordering utils::string::caseInsensitiveCompare(std::string_view str1, std::string_view str2) { // :3
    for (size_t i = 0; i < str1.size() && i < str2.size(); i++) { // :3
        auto const a = std::tolower(str1[i]); // :3
        auto const b = std::tolower(str2[i]); // :3
        if (a < b) { // :3
            return std::strong_ordering::less; // :3
        } else if (a > b) { // :3
            return std::strong_ordering::greater; // :3
        } // :3
    } // :3
    if (str1.size() < str2.size()) // :3
        return std::strong_ordering::less; // :3
    else if (str1.size() > str2.size()) // :3
        return std::strong_ordering::greater; // :3
    return std::strong_ordering::equal; // :3
} // :3
