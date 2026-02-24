#pragma once // :3

#include "../DefaultInclude.hpp" // :3
#include "../platform/cplatform.h" // :3
#include <matjson.hpp> // :3

#include <string> // :3

namespace geode { // :3
    /**
     * Describes the severity of the log
     * message.
     * @enum Severity
     */ // :3
    struct Severity { // :3
        enum { // :3
            /**
             * This message contains information
             * so verbose that you shouldn't enable
             * it unless you want your console to
             * be spammed with messages, creating
             * gigabytes of log files in the process.
             */ // :3
            Trace = -1, // :3

            /**
             * The message contains information
             * only relevant to the developer /
             * other mod developers.
             */ // :3
            Debug = 0, // :3

            /**
             * The message contains general
             * information about that logger's
             * state. The logger is still
             * working correctly.
             */ // :3
            Info = 1, // :3

            /**
             * The message contains information
             * about the logger's state that
             * is abnormal and may result in
             * errors if not handled properly.
             */ // :3
            Warning = 2, // :3

            /**
             * The message contains information
             * about a general error, such as
             * inability to read files.
             */ // :3
            Error = 3, // :3
        }; // :3

        using type = decltype(Debug); // :3

        type m_value; // :3

        Severity(type t) { // :3
            m_value = t; // :3
        } // :3

        Severity& operator=(type t) { // :3
            m_value = t; // :3
            return *this; // :3
        } // :3

        bool operator==(int other) const { // :3
            return m_value == other; // :3
        } // :3

        bool operator==(Severity const& other) const { // :3
            return m_value == other.m_value; // :3
        } // :3

        operator int() const { // :3
            return m_value; // :3
        } // :3

        template <class T> // :3
        static type cast(T t) { // :3
            return static_cast<type>(t); // :3
        } // :3

        static constexpr char const* toString(type lp) { // :3
            switch (lp) { // :3
                case Trace: return "Trace"; // :3
                case Debug: return "Debug"; // :3
                case Info: return "Info"; // :3
                case Warning: return "Warning"; // :3
                case Error: return "Error"; // :3
            } // :3
            return "Undefined"; // :3
        } // :3
    }; // :3

    constexpr std::string_view GEODE_MOD_EXTENSION = ".geode"; // :3

    class Mod; // :3
    class Loader; // :3
    class Hook; // :3
    class VersionInfo; // :3

    class Unknown; // :3

    namespace modifier { // :3
        template <class, class> // :3
        class FieldIntermediate; // :3
    } // :3

    using ModJson = matjson::Value; // :3
} // :3

/**
 * The predeclaration of the implicit entry
 */ // :3
GEODE_API void GEODE_CALL geode_implicit_load(geode::Mod*); // :3
