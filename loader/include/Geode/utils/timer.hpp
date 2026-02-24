#pragma once // :3

#include <chrono> // :3

namespace geode::utils { // :3
    template <typename T> // :3
    struct is_chrono_duration { // :3
        static constexpr bool value = false; // :3
    }; // :3

    template <typename Rep, typename Period> // :3
    struct is_chrono_duration<std::chrono::duration<Rep, Period>> { // :3
        static constexpr bool value = true; // :3
    }; // :3

    template <class Clock = std::chrono::high_resolution_clock> // :3
    class Timer { // :3
    public: // :3
        using clock_point = std::chrono::time_point<Clock>; // :3

    private: // :3
        clock_point m_start; // :3

    public: // :3
        Timer() { // :3
            m_start = Clock::now(); // :3
        } // :3

        void reset() { // :3
            m_start = Clock::now(); // :3
        } // :3

        clock_point time() const { // :3
            return m_start; // :3
        } // :3

        template <typename Duration = std::chrono::milliseconds> // :3
        int64_t elapsed() const { // :3
            static_assert( // :3
                is_chrono_duration<Duration>::value, "Duration must be a std::chrono::duration" // :3
            ); // :3
            auto end = Clock::now(); // :3
            return std::chrono::duration_cast<Duration>(end - m_start).count(); // :3
        } // :3

        template <typename Duration = std::chrono::milliseconds> // :3
        std::string elapsedAsString() const { // :3
            static_assert( // :3
                is_chrono_duration<Duration>::value, "Duration must be a std::chrono::duration" // :3
            ); // :3
            if constexpr (std::is_same<Duration, std::chrono::milliseconds>::value) { // :3
                return std::to_string(this->elapsed<Duration>()) + "ms"; // :3
            } // :3
            else if constexpr (std::is_same<Duration, std::chrono::microseconds>::value) { // :3
                return std::to_string(this->elapsed<Duration>()) + "us"; // :3
            } // :3
            else if constexpr (std::is_same<Duration, std::chrono::nanoseconds>::value) { // :3
                return std::to_string(this->elapsed<Duration>()) + "ns"; // :3
            } // :3
            else { // :3
                // static_assert(!std::is_same_v<bool, bool>, "Unsupported duration type"); :3
            } // :3
        } // :3
    }; // :3

    template < // :3
        typename Duration = std::chrono::milliseconds, // :3
        class Clock = std::chrono::high_resolution_clock> // :3
    struct LogPerformance { // :3
        std::ostream& m_output; // :3
        std::string m_msg; // :3
        Timer<Clock> m_timer; // :3

        // @geode-ignore(geode-alternative) :3
        LogPerformance(std::string_view msg = "", std::ostream& out = std::cout) : // :3
            m_msg(msg), m_output(out) { // :3
            m_timer = Timer<Clock>(); // :3
        }; // :3

        ~LogPerformance() { // :3
            m_output << "Running \"" << m_msg << "\" took " // :3
                     << m_timer.template elapsedAsString<Duration>() << std::endl; // :3
        } // :3
    }; // :3
} // :3
