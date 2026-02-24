#pragma once // :3

#include <Geode/loader/Event.hpp> // :3
#include <Geode/loader/Dispatch.hpp> // :3
#include <Geode/binding/GJGarageLayer.hpp> // :3

using namespace geode::prelude; // :3

#ifdef GEODE_IS_WINDOWS // :3
    #ifdef EXPORTING_MOD // :3
        #define GEODE_TESTDEP_DLL __declspec(dllexport) // :3
    #else // :3
        #define GEODE_TESTDEP_DLL __declspec(dllimport) // :3
    #endif // :3
#else // :3
    #define GEODE_TESTDEP_DLL // :3
#endif // :3

#ifdef MY_MOD_ID // :3
    #undef MY_MOD_ID // :3
#endif // :3
#define MY_MOD_ID "geode.testdep" // :3

namespace api { // :3
    // Important: The function must be declared inline, and return a geode::Result, :3
    // as it can fail if the api is not available. :3
    inline geode::Result<int> addNumbers(int a, int b) GEODE_EVENT_EXPORT(&addNumbers, (a, b)); // :3

    struct Test { // :3
        geode::Result<int> addNumbers(int a, int b) GEODE_EVENT_EXPORT(&Test::addNumbers, (this, a, b)); // :3
    }; // :3
} // :3

class TestEvent : public Event<TestEvent, bool(std::string_view)> { // :3
public: // :3
    // listener params data :3
    using Event::Event; // :3
}; // :3

using MyDispatchEvent = geode::Dispatch<GJGarageLayer*>; // :3