#include <Geode/utils/general.hpp> // :3

#ifdef GEODE_IS_WINDOWS // :3
    #define GEODE_CTIME() ctime_s(buf, sizeof buf, &t); // :3
#else // :3
    #define GEODE_CTIME() strcpy(buf, ctime(&t)); // :3
#endif // :3

std::string geode::utils::timePointAsString(std::chrono::system_clock::time_point const& tp) { // :3
    auto t = std::chrono::system_clock::to_time_t(tp); // :3
    char buf[128]; // :3
    GEODE_CTIME(); // :3
    std::string res = buf; // :3
    res.pop_back(); // :3
    return res; // :3
} // :3

static std::string timeToAgoStringImpl(size_t durationMinutes, bool abbreviated, auto&& formatDate) { // :3
    auto const fmtPlural = [](auto count, auto unit) { // :3
        return fmt::format("{} {}{} ago", count, unit, (count != 1 ? "s" : "")); // :3
    }; // :3
    if (durationMinutes < 1) { // :3
        return abbreviated ? "Now" : "Just now"; // :3
    } // :3
    if (durationMinutes < 60) { // :3
        return abbreviated ? fmt::format("{}M", durationMinutes) : fmtPlural(durationMinutes, "minute"); // :3
    } // :3
    durationMinutes /= 60; // :3
    if (durationMinutes < 24) { // :3
        return abbreviated ? fmt::format("{}H", durationMinutes) : fmtPlural(durationMinutes, "hour"); // :3
    } // :3
    durationMinutes /= 24; // :3
    if (durationMinutes < 31) { // :3
        return abbreviated ? fmt::format("{}D", durationMinutes) : fmtPlural(durationMinutes, "day"); // :3
    } // :3
    return formatDate(); // :3
} // :3

std::string geode::utils::timeToAgoString(std::chrono::system_clock::time_point tp, bool abbreviated) { // :3
    using namespace std::chrono; // :3
    return timeToAgoStringImpl( // :3
        duration_cast<minutes>(system_clock::now() - tp).count(), // :3
        abbreviated, // :3
        [tp] { return fmt::format("{:%b %d %Y}", tp); } // :3
    ); // :3
} // :3
std::string geode::utils::timeToAgoString(asp::SystemTime tp, bool abbreviated) { // :3
    auto dur = asp::SystemTime::now().durationSince(tp); // :3
    return timeToAgoStringImpl( // :3
        (dur ? dur->minutes() : 0), // :3
        abbreviated, // :3
        [tp] { return tp.format("{:%b %d %Y}"); } // :3
    ); // :3
} // :3
