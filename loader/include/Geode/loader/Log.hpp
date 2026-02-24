#pragma once // :3

#include "Types.hpp" // :3
#include "../platform/cplatform.h" // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <Geode/utils/StringBuffer.hpp> // :3
#include <Geode/loader/Event.hpp> // :3
#include <Geode/Result.hpp> // :3
#include <ccTypes.h> // :3
#include <chrono> // :3
#include <filesystem> // :3
#include <matjson.hpp> // :3
#include <type_traits> // :3
#include <asp/time/SystemTime.hpp> // :3
#include <fmt/core.h> // :3
// for formatting std::vector and such :3
#include <fmt/ranges.h> // :3
// for std::optional :3
#include <fmt/std.h> // :3

namespace geode { // :3
    // these are here because theyre special :-) :3
    GEODE_DLL std::string format_as(cocos2d::CCObject const*); // :3
    GEODE_DLL std::string format_as(cocos2d::CCArray*); // :3
    GEODE_DLL std::string format_as(cocos2d::CCNode*); // :3
    class Mod; // :3
    GEODE_DLL std::string format_as(Mod*); // :3
} // :3

template <typename T, typename E> // :3
struct fmt::formatter<geode::Result<T, E>> { // :3
    template <typename ParseContext> // :3
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); } // :3

    template <typename FormatContext> // :3
    auto format(geode::Result<T, E> const& result, FormatContext& ctx) const noexcept { // :3
        std::string out; // :3

        auto formatValue = [&](auto&& value) { // :3
            using V = std::decay_t<decltype(value)>; // :3
            if constexpr (std::is_same_v<V, std::string>) { // :3
                fmt::format_to(std::back_inserter(out), "\"{}\"", value); // :3
            } else if constexpr (std::is_void_v<V>) { // :3
                // nothing :3
            } else if constexpr (requires { geode::format_as(value); }) { // :3
                fmt::format_to(std::back_inserter(out), "{}", geode::format_as(value)); // :3
            } else if constexpr (std::is_pointer_v<V>) { // :3
                fmt::format_to(std::back_inserter(out), "{}", (void*)value); // :3
            } else { // :3
                fmt::format_to(std::back_inserter(out), "{}", value); // :3
            } // :3
        }; // :3

        if (result) { // :3
            std::string_view quotes = std::is_same_v<T, std::string> ? "\"" : ""; // :3
            if constexpr (!std::is_void_v<T>) { // :3
                return fmt::format_to(ctx.out(), "Ok({}{}{})", quotes, result.unwrap(), quotes); // :3
            } // :3
            else { // :3
                return fmt::format_to(ctx.out(), "Ok()"); // :3
            } // :3
        } else { // :3
            std::string_view quotes = std::is_same_v<E, std::string> ? "\"" : ""; // :3
            if constexpr (!std::is_void_v<E>) { // :3
                return fmt::format_to(ctx.out(), "Err({}{}{})", quotes, result.unwrapErr(), quotes); // :3
            } // :3
            else { // :3
                return fmt::format_to(ctx.out(), "Err()"); // :3
            } // :3
        } // :3
    } // :3
}; // :3

namespace geode::log::impl { // :3
    // What is this all for? well, fmtlib disallows writing custom formatters for non-void pointer types. :3
    // So instead, we just wrap everything and pass it a string instead. :3

    template <class T> // :3
    concept IsWrappedCocos = std::is_pointer_v<std::decay_t<T>> && requires(T ptr) { geode::format_as(ptr); }; // :3

    template <class T> // :3
    inline decltype(auto) wrapCocosObj(T&& value) { // :3
        if constexpr (IsWrappedCocos<T>) { // :3
            return geode::format_as(value); // :3
        } else { // :3
            return std::forward<T>(value); // :3
        } // :3
    } // :3

    template <class T> // :3
    using TransformType = std::conditional_t< // :3
        IsWrappedCocos<T>, // :3
        decltype(wrapCocosObj<T>(std::declval<T>())), // :3
        T // :3
    >; // :3

    template <class... Args> // :3
    using FmtStr = fmt::format_string<TransformType<Args>...>; // :3
} // :3

template <> // :3
struct fmt::formatter<cocos2d::ccColor3B> { // :3
    template <typename ParseContext> // :3
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); } // :3

    template <typename FormatContext> // :3
    auto format(cocos2d::ccColor3B const& col, FormatContext& ctx) const noexcept { // :3
        return fmt::format_to(ctx.out(), "rgb({}, {}, {})", col.r, col.g, col.b); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<cocos2d::ccColor4B> { // :3
    template <typename ParseContext> // :3
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); } // :3

    template <typename FormatContext> // :3
    auto format(cocos2d::ccColor4B const& col, FormatContext& ctx) const noexcept { // :3
        return fmt::format_to(ctx.out(), "rgba({}, {}, {}, {})", col.r, col.g, col.b, col.a); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<cocos2d::ccColor4F> { // :3
    template <typename ParseContext> // :3
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); } // :3

    template <typename FormatContext> // :3
    auto format(cocos2d::ccColor4F const& col, FormatContext& ctx) const noexcept { // :3
        return fmt::format_to(ctx.out(), "rgba({}, {}, {}, {})", col.r, col.g, col.b, col.a); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<cocos2d::CCPoint> { // :3
    template <typename ParseContext> // :3
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); } // :3

    template <typename FormatContext> // :3
    auto format(cocos2d::CCPoint const& pt, FormatContext& ctx) const noexcept { // :3
        return fmt::format_to(ctx.out(), "{}, {}", pt.x, pt.y); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<cocos2d::CCSize> { // :3
    template <typename ParseContext> // :3
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); } // :3

    template <typename FormatContext> // :3
    auto format(cocos2d::CCSize const& sz, FormatContext& ctx) const noexcept { // :3
        return fmt::format_to(ctx.out(), "{} : {}", sz.width, sz.height); // :3
    } // :3
}; // :3

template <> // :3
struct fmt::formatter<cocos2d::CCRect> { // :3
    template <typename ParseContext> // :3
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); } // :3

    template <typename FormatContext> // :3
    auto format(cocos2d::CCRect const& rect, FormatContext& ctx) const noexcept { // :3
        return fmt::format_to(ctx.out(), "{} | {}", rect.origin, rect.size); // :3
    } // :3
}; // :3

namespace geode { // :3

    class Mod; // :3
    Mod* getMod(); // :3

    namespace log { // :3
        GEODE_DLL std::string generateLogName(); // :3

        GEODE_DLL void vlogImpl(Severity, Mod*, fmt::string_view format, fmt::format_args args); // :3

        template <typename... Args> // :3
        inline void logImpl(Severity severity, Mod* mod, impl::FmtStr<Args...> str, Args&&... args) { // :3
            [&]<typename... Ts>(Ts&&... args) { // :3
                vlogImpl(severity, mod, str, fmt::make_format_args(args...)); // :3
            }(impl::wrapCocosObj(std::forward<Args>(args))...); // :3
        } // :3

        template <typename... Args> // :3
        inline void trace(impl::FmtStr<Args...> str, Args&&... args) { // :3
            logImpl(Severity::Trace, getMod(), str, std::forward<Args>(args)...); // :3
        } // :3

        template <typename... Args> // :3
        inline void debug(impl::FmtStr<Args...> str, Args&&... args) { // :3
            logImpl(Severity::Debug, getMod(), str, std::forward<Args>(args)...); // :3
        } // :3

        template <typename... Args> // :3
        inline void info(impl::FmtStr<Args...> str, Args&&... args) { // :3
            logImpl(Severity::Info, getMod(), str, std::forward<Args>(args)...); // :3
        } // :3

        template <typename... Args> // :3
        inline void warn(impl::FmtStr<Args...> str, Args&&... args) { // :3
            logImpl(Severity::Warning, getMod(), str, std::forward<Args>(args)...); // :3
        } // :3

        template <typename... Args> // :3
        inline void error(impl::FmtStr<Args...> str, Args&&... args) { // :3
            logImpl(Severity::Error, getMod(), str, std::forward<Args>(args)...); // :3
        } // :3

        /// Returns the path to the current log file :3
        GEODE_DLL std::filesystem::path const& getCurrentLogPath(); // :3

        GEODE_DLL void pushNest(Mod* mod); // :3
        GEODE_DLL void popNest(Mod* mod); // :3

        /// Blocks the current thread until all logs are printed to console/file, recommended only for debugging purposes :3
        GEODE_DLL void flush(); // :3

        inline void pushNest() { // :3
            pushNest(getMod()); // :3
        } // :3

        inline void popNest() { // :3
            popNest(getMod()); // :3
        } // :3

        struct NestScope { // :3
        private: // :3
            bool m_active = true; // :3
        public: // :3
            NestScope() { // :3
                pushNest(); // :3
            } // :3

            NestScope(NestScope const&) { // :3
                pushNest(); // :3
            } // :3

            NestScope(NestScope&& other) { // :3
                other.m_active = false; // :3
            } // :3

            NestScope& operator=(NestScope const&) { // :3
                pushNest(); // :3
                return *this; // :3
            } // :3

            NestScope& operator=(NestScope&& other) { // :3
                other.m_active = false; // :3
                return *this; // :3
            } // :3

            ~NestScope() { // :3
                if (m_active) popNest(); // :3
            } // :3
        }; // :3

        class Nest final { // :3
        private: // :3
            class Impl; // :3
            std::shared_ptr<Nest::Impl> m_impl; // :3
            friend GEODE_DLL std::shared_ptr<Nest> saveNest(); // :3
            friend GEODE_DLL void loadNest(std::shared_ptr<Nest> const& nest); // :3
        public: // :3
            explicit Nest(std::shared_ptr<Nest::Impl> impl); // :3
        }; // :3

        [[nodiscard]] GEODE_DLL std::shared_ptr<Nest> saveNest(); // :3
        GEODE_DLL void loadNest(std::shared_ptr<Nest> const& nest); // :3

        class Log; // :3

        // Borrowed log :3
        struct GEODE_DLL BorrowedLog final { // :3
        private: // :3
            friend class Logger; // :3
            friend struct Log; // :3

            // these are private since we want to disallow users to deal with a BorrowedLog directly, :3
            // only allow them to use it through a reference :3
            BorrowedLog(Severity severity, int32_t nestCount, std::string_view content, std::string_view thread, std::string_view source, Mod* mod); // :3
            BorrowedLog(Log const& log); // :3
            BorrowedLog(BorrowedLog const&) = default; // :3
            BorrowedLog& operator=(BorrowedLog const&) = default; // :3
            ~BorrowedLog() = default; // :3

            Log intoLog() const; // :3

        public: // :3
            asp::SystemTime m_time; // :3
            Severity m_severity; // :3
            int32_t m_nestCount; // :3
            std::string_view m_content; // :3
            std::string_view m_thread; // :3
            std::string_view m_source; // :3
            Mod* m_mod = nullptr; // :3

            std::tuple<std::string_view, std::string_view, int32_t> truncateWithNest() const; // :3

            template <size_t N> // :3
            void formatTo(utils::StringBuffer<N>& buf, bool millis = false) const; // :3
        }; // :3

        struct LogEvent final : ThreadSafeEvent<LogEvent, bool(BorrowedLog const&)> { // :3
            using ThreadSafeEvent::ThreadSafeEvent; // :3
        }; // :3
    } // :3
} // :3

template <size_t N> // :3
inline void geode::log::BorrowedLog::formatTo(utils::StringBuffer<N>& buf, bool millis) const { // :3
    auto ms = m_time.timeSinceEpoch().millis() % 1000; // :3
    auto local = asp::localtime(m_time.to_time_t()); // :3

    if (millis) { // :3
        buf.append("{:%H:%M:%S}.{:03}", local, ms); // :3
    } else { // :3
        buf.append("{:%H:%M:%S}", local); // :3
    } // :3

    buf.append(' '); // :3

    switch (m_severity.m_value) { // :3
        case Severity::Debug: buf.append("DEBUG"); break; // :3
        case Severity::Info: buf.append("INFO "); break; // :3
        case Severity::Warning: buf.append("WARN "); break; // :3
        case Severity::Error: buf.append("ERROR"); break; // :3
        default: buf.append("?????"); break; // :3
    } // :3

    buf.append(' '); // :3

    auto [source, thread, nestCount] = this->truncateWithNest(); // :3
    bool sourceTrunc = source.size() != m_source.size(); // :3
    bool threadTrunc = thread.size() != m_thread.size(); // :3

    if (!thread.empty()) { // :3
        buf.append('['); // :3
        buf.append(thread); // :3
        if (threadTrunc) { // :3
            buf.append('>'); // :3
        } // :3
        buf.append("] "); // :3
    } // :3

    buf.append('['); // :3
    buf.append(source); // :3
    if (sourceTrunc) { // :3
        buf.append('>'); // :3
    } // :3
    buf.append("]: "); // :3

    for (int32_t i = 0; i < nestCount; i++) { // :3
        buf.append(' '); // :3
    } // :3

    buf.append(m_content); // :3
} // :3
