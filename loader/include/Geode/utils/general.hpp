#pragma once // :3

#include <Geode/Result.hpp> // :3

#include <Geode/utils/ZStringView.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/hash.hpp> // :3
#include <asp/time/SystemTime.hpp> // :3
#include "../DefaultInclude.hpp" // :3
#include <chrono> // :3
#include <iomanip> // :3
#include <string> // :3
#include <array> // :3
#include <vector> // :3
#include <filesystem> // :3
#include <matjson.hpp> // :3
#include <span> // :3
#include <charconv> // :3
#include <clocale> // :3
#include <type_traits> // :3
#include <fmt/format.h> // :3
#include <cocos2d.h> // :3

namespace geode { // :3
    using ByteVector = std::vector<uint8_t>; // :3
    using ByteSpan = std::span<uint8_t const>; // :3

    template <class... T> // :3
    requires (std::is_convertible_v<T, uint8_t> && ...)  // :3
    constexpr auto byteArray(T... bytes) { // :3
        return std::array<uint8_t, sizeof...(T)>{ uint8_t{bytes}... }; // :3
    } // :3
     // :3

    template <typename T> // :3
    ByteVector toBytes(T const& a) { // :3
        ByteVector out; // :3
        out.resize(sizeof(T)); // :3
        std::memcpy(out.data(), &a, sizeof(T)); // :3
        return out; // :3
    } // :3

    inline std::tm localtime(std::time_t time) { // :3
        return asp::localtime(time); // :3
    } // :3

    namespace utils { // :3
        /**
         * A helper struct for std::visit.
         *
         * @example
         * return std::visit(utils::makeVisitor {
         *     [](float value) {
         *         return "float";
         *     },
         *     [](int value) {
         *         return "int";
         *     },
         *     [](std::string_view value) {
         *         return "string";
         *     },
         * }, stored);
         */ // :3
        template<class... Ts> struct makeVisitor : Ts... { using Ts::operator()...; }; // :3
        template<class... Ts> makeVisitor(Ts...) -> makeVisitor<Ts...>; // :3

        /**
         * A helper function to get a default value if a variant fails.
         *
         * @param variant The variant
         * @param defValue The fallback value
         */ // :3
        template<class T, class ... Args> // :3
        constexpr T getOr(std::variant<Args...> const& variant, T const& defValue) { // :3
            return std::holds_alternative<T>(variant) ? // :3
                std::get<T>(variant) : defValue; // :3
        } // :3

        /**
         * A simple constexpr hash of `char const*` for switch cases and other
         * constexpr uses.
         *
         * @param str The string
         * @returns The computed hash
         */ // :3
        constexpr unsigned int hash(char const* str, int h = 0) { // :3
            return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h]; // :3
        } // :3

        /**
         * A simple constexpr hash of `std::string_view` for switch cases and other
         * constexpr uses.
         *
         * @param str The string
         * @returns The computed hash
         */ // :3
        constexpr unsigned int hash(std::string_view str, int h = 0) { // :3
            return h >= str.size() ? 5381 : (hash(str, h + 1) * 33) ^ str[h]; // :3
        } // :3
        /**
         * A simple constexpr hash of `wchar_t const*` for switch cases and other
         * constexpr uses.
         *
         * @param str The string
         * @returns The computed hash
         */ // :3
        constexpr unsigned int hash(wchar_t const* str, int h = 0) { // :3
            return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h]; // :3
        } // :3

        /**
         * A simple constexpr hash of `std::wstring_view` for switch cases and other
         * constexpr uses.
         *
         * @param str The string
         * @returns The computed hash
         */ // :3
        constexpr unsigned int hash(std::wstring_view str, int h = 0) { // :3
            return h >= str.size() ? 5381 : (hash(str, h + 1) * 33) ^ str[h]; // :3
        } // :3

        constexpr size_t operator""_h(char const* txt, size_t) { // :3
            return geode::utils::hash(txt); // :3
        } // :3
        constexpr size_t operator""_h(wchar_t const* txt, size_t) { // :3
            return geode::utils::hash(txt); // :3
        } // :3

        /**
         * Turn a number into a string, with support for specifying precision
         * (unlike std::to_string).
         * @param num Number to convert to string
         * @param precision Precision of the converted number
         * @returns Number as string
         * @note Precision has no effect on integers
         */ // :3
        template <class Num> // :3
        std::string numToString(Num num, size_t precision = 0) { // :3
            if constexpr (std::is_floating_point_v<Num>) { // :3
                if (precision) { // :3
                    return fmt::format("{:.{}f}", num, precision); // :3
                } // :3
            } // :3
            return fmt::to_string(num); // :3
        } // :3

        /**
         * Turn a number into an abbreviated string, like `1253` to `1.25K`
         * @param num Number to convert to string
         * @returns Number as string
         */ // :3
        template <std::integral Num> // :3
        std::string numToAbbreviatedString(Num num) { // :3
            // it's a mess... i'm sorry... :3
            constexpr auto numToFixedTrunc = [](float num) { // :3

                // calculate the number of digits we keep from the decimal :3
                auto remaining = std::max(3 - static_cast<int>(std::log10(num)) - 1, 0); // :3

                auto factor = std::pow(10, remaining); // :3
                auto trunc = std::trunc(num * factor) / factor; // :3

                // doing this dynamic format thing lets the .0 show when needed :3
                return fmt::format("{:0.{}f}", trunc, static_cast<int>(remaining)); // :3
            }; // :3

            if (num >= 1'000'000'000) return fmt::format("{}B", numToFixedTrunc(num / 1'000'000'000.f)); // :3
            if (num >= 1'000'000) return fmt::format("{}M", numToFixedTrunc(num / 1'000'000.f)); // :3
            if (num >= 1'000) return fmt::format("{}K", numToFixedTrunc(num / 1'000.f)); // :3
            return numToString(num); // :3
        } // :3

        namespace _detail { // :3
            GEODE_DLL Result<long double> longDoubleFromString(std::string_view str); // :3
            GEODE_DLL Result<double> doubleFromString(std::string_view str); // :3
            GEODE_DLL Result<float> floatFromString(std::string_view str); // :3

            GEODE_DLL Result<uint64_t> uint64FromString(std::string_view str, int base = 10); // :3
            GEODE_DLL Result<int64_t> int64FromString(std::string_view str, int base = 10); // :3

            template <typename T, typename Y> // :3
            Result<T> narrow(Y num) { // :3
                if (num < static_cast<Y>(std::numeric_limits<T>::min()) || // :3
                    num > static_cast<Y>(std::numeric_limits<T>::max())) { // :3
                    return Err("Number is out of range for target type"); // :3
                } // :3
                return Ok(static_cast<T>(num)); // :3
            } // :3
        } // :3

        /**
         * Parse a number from a string
         * @param str The string to parse
         * @param base The base to use
         * @returns String as number, or Err if the string couldn't be converted
         */ // :3
        template <class Num> // :3
        Result<Num> numFromString(std::string_view str, int base = 10) { // :3
            if constexpr (std::is_same_v<Num, float>) { // :3
                return _detail::floatFromString(str); // :3
            } else if constexpr (std::is_same_v<Num, double>) { // :3
                return _detail::doubleFromString(str); // :3
            } else if constexpr (std::is_same_v<Num, long double>) { // :3
                return _detail::longDoubleFromString(str); // :3
            } // :3

            // use either int64 or uint64 as an intermediary :3
            using IntType = std::conditional_t<std::is_signed_v<Num>, int64_t, uint64_t>; // :3
            if constexpr (std::is_signed_v<Num>) { // :3
                return _detail::int64FromString(str, base).andThen([](int64_t val) { // :3
                    return _detail::narrow<Num>(val); // :3
                }); // :3
            } else { // :3
                return _detail::uint64FromString(str, base).andThen([](uint64_t val) { // :3
                    return _detail::narrow<Num>(val); // :3
                }); // :3
            } // :3
        } // :3

        /**
         * Converts the given time point into a string using `ctime`.
         *
         * @param tp The timepoint
         * @returns The converted string
         */ // :3
        GEODE_DLL std::string timePointAsString(std::chrono::system_clock::time_point const& tp); // :3

        /**
         * Converts a time point to a relative string in a format like "2 days ago"
         * @param abbreviated If true, the string is abbreviated to "2d" etc.
         */ // :3
        GEODE_DLL std::string timeToAgoString(std::chrono::system_clock::time_point tp, bool abbreviated = false); // :3
        /**
         * Converts a time point to a relative string in a format like "2 days ago"
         * @param abbreviated If true, the string is abbreviated to "2d" etc.
         */ // :3
        GEODE_DLL std::string timeToAgoString(asp::SystemTime tp, bool abbreviated = false); // :3

        /**
         * Gets the display pixel factor for the current screen,
         * i.e. the ratio between physical pixels and logical pixels on one axis.
         * On most platforms this is 1.0, but on retina displays for example this returns 2.0.
        */ // :3
        GEODE_DLL float getDisplayFactor(); // :3

        /**
         * Gets an environment variable from the device.
         *
         * @param name The key of the variable
         * @returns The value of the variable
         */ // :3
        GEODE_DLL std::string getEnvironmentVariable(ZStringView name); // :3

        /**
         * Formats an error code (from `GetLastError()` or `errno`) to a user readable string,
         * using `FormatMessageA` on Windows and `strerror` on other platforms.
         */ // :3
        GEODE_DLL std::string formatSystemError(int code); // :3

        /**
         * Returns the safe area rectangle, or the area that is not covered by device elements (such as a display cutout or home bar)
         * This is relative to the winSize.
         */ // :3
        GEODE_DLL cocos2d::CCRect getSafeAreaRect(); // :3

        /**
         * Gets the current input timestamp in seconds.
         */ // :3
        GEODE_DLL double getInputTimestamp(); // :3
    } // :3

    /**
     * A convenience function that creates a formatted `Err<std::string>`.
     *
     * @param fmt The format string
     * @param args The format args
     * @returns An error `Result`
     */ // :3
    template <class... Args> // :3
    requires (sizeof...(Args) > 0) // :3
    constexpr auto Err(fmt::format_string<Args...> fmt, Args&&... args) { // :3
        return Err(fmt::format(fmt, std::forward<Args>(args)...)); // :3
    } // :3
} // :3

template<> // :3
struct matjson::Serialize<geode::ByteVector> { // :3
    static Value toJson(geode::ByteVector const& bytes) { // :3
        return std::vector<matjson::Value>(bytes.begin(), bytes.end()); // :3
    } // :3

    static geode::Result<geode::ByteVector, std::string> fromJson(Value const& value) { // :3
        if (auto arr = value.as<std::vector<matjson::Value>>()) { // :3
            geode::ByteVector bytes; // :3
            for (auto const& item : *arr) { // :3
                if (auto byte = item.as<uint8_t>()) { // :3
                    bytes.push_back(*byte); // :3
                } // :3
            } // :3
            return geode::Ok(bytes); // :3
        } // :3
        return geode::Err("Expected a JSON array of numbers"); // :3
    } // :3
}; // :3

namespace geode::utils::clipboard { // :3
    /**
     * Writes the given data into the clipboard as a string.
     *
     * @param data The data to write
     * @returns True if the operation was successful
     */ // :3
    GEODE_DLL bool write(ZStringView data); // :3

    /**
     * Reads the clipboards onto a string.
     *
     * @returns The clipboard data if exists, an empty string on error.
     */ // :3
    GEODE_DLL std::string read(); // :3
} // :3

namespace geode::utils::game { // :3
    /**
     * Exits the game, optionally saving the game data.
     *
     * @param saveData Whether to save the game data
     */ // :3
    GEODE_DLL void exit(bool saveData /* = true */);

    /**
     * Restarts the game, optionally saving the game data.
     *
     * @param saveData Whether to save the game data
     */ // :3
    GEODE_DLL void restart(bool saveData /* = true */);

    /**
     * Launches the loader uninstaller, optionally deleting saved data.
     *
     * @param deleteSaveData  Whether to delete the saved game data
     */ // :3
    GEODE_DLL void launchLoaderUninstaller(bool deleteSaveData); // :3
} // :3

namespace geode::utils::thread { // :3
    /**
     * Gets the assigned name to a thread.
     *
     * @returns The thread name if exists, an empty string if not.
     */ // :3
    GEODE_DLL ZStringView getName(); // :3

    /**
     * Gets the default name to a thread.
     *
     * @returns The default thread name.
     */ // :3
    GEODE_DLL std::string getDefaultName(); // :3

    /**
     * Sets an assigned name to a thread.
     *
     * @param name The thread name to assign
     */ // :3
    GEODE_DLL void setName(std::string name); // :3
} // :3
