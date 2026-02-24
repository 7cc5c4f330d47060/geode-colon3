#pragma once // :3

#include "Event.hpp" // :3
#include "../utils/function.hpp" // :3
#include "../modify/Traits.hpp" // :3
#include <Geode/utils/function.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3

#include <functional> // :3
#include <string> // :3
#include <tuple> // :3

namespace geode { // :3
    template <class... Args> // :3
    class Dispatch : public ThreadSafeEvent<Dispatch<Args...>, bool(Args...), std::string> { // :3
    public: // :3
        using ThreadSafeEvent<Dispatch<Args...>, bool(Args...), std::string>::ThreadSafeEvent; // :3
    }; // :3

    template<class... Args> // :3
    using DispatchEvent = Dispatch<Args...>; // :3

} // :3

// - Macros for exporting functions via events - :3

// You can use these to easily export functions to other mods :3
// without being a required dependency. :3
// # Example Usage: :3
/*
```
// (In your api distributed header file)
#pragma once

#include <Geode/loader/Dispatch.hpp>
// You must **manually** declare the mod id, as macros like GEODE_MOD_ID will not
// behave correctly to other mods using your api.
#define MY_MOD_ID "dev.my-api"

namespace api {
// Important: The function must be declared inline, and return a geode::Result,
// as it can fail if the api is not available.
inline geode::Result<int> addNumbers(int a, int b) GEODE_EVENT_EXPORT(&addNumbers, (a, b));
}
```
*/ // :3
// Then, in **one** of your source files, you must define the exported functions: :3
/*
```
// MUST be defined before including the header.
#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/api.hpp"

Result<int> api::addNumbers(int a, int b) {
    return Ok(a + b);
}
```
*/ // :3

// once this is set in stone we should not change it ever :3
#define GEODE_EVENT_EXPORT_ID_FOR(fnPtrStr, callArgsStr) \
    (std::string(MY_MOD_ID "/") + (fnPtrStr[0] == '&' ? &fnPtrStr[1] : fnPtrStr)) // :3

namespace geode::geode_internal { // :3
    template <class Fn> // :3
    inline auto callEventExportListener(Fn fnPtr, auto eventID) { // :3
        using StaticType = geode::modifier::AsStaticType<Fn>::type; // :3
        Fn ptr = nullptr; // :3
        geode::Dispatch<Fn*>(std::move(eventID)).send(&ptr); // :3
        return geode::Function<std::remove_pointer_t<StaticType>>(ptr); // :3
    } // :3

    template <class Fn> // :3
    inline bool getEventExportListener(Fn fnPtr, auto eventID) { // :3
        geode::Dispatch<Fn*>(std::move(eventID)).listen([=](Fn* ptr) { // :3
            *ptr = fnPtr; // :3
            return geode::ListenerResult::Stop; // :3
        }).leak(); // :3
        return true; // :3
    } // :3
} // :3

#define GEODE_EVENT_EXPORT_CALL(fnPtr, callArgs, eventID)                                       \
    {                                                                                           \
        static auto storage = geode::geode_internal::callEventExportListener(fnPtr, eventID);   \
        if (!storage) return geode::Err("Unable to call method");                               \
        return storage callArgs;                                                                \
    } // :3

#define GEODE_EVENT_EXPORT_CALL_NORES(fnPtr, callArgs, eventID)                                 \
    {                                                                                           \
        static auto storage = geode::geode_internal::callEventExportListener(fnPtr, eventID);   \
        if (!storage) return geode::utils::function::Return<decltype(fnPtr)>();                \
        return storage callArgs;                                                                \
    } // :3


#define GEODE_EVENT_EXPORT_DEFINE(fnPtr, callArgs, eventID)                                             \
    ;                                                                                                   \
    template <auto>                                                                                     \
    struct EventExportDefine;                                                                           \
    template <>                                                                                         \
    struct EventExportDefine<geode::modifier::FunctionUUID<fnPtr>::value> {                             \
        static inline bool val = geode::geode_internal::getEventExportListener(fnPtr, eventID);         \
        static inline auto nonOmitted = &val;                                                           \
    }; // :3

#ifndef GEODE_DEFINE_EVENT_EXPORTS // :3

    #define GEODE_EVENT_EXPORT(fnPtr, callArgs) \
        GEODE_EVENT_EXPORT_CALL(fnPtr, callArgs, GEODE_EVENT_EXPORT_ID_FOR(#fnPtr, #callArgs)) // :3

    #define GEODE_EVENT_EXPORT_ID(fnPtr, callArgs, eventID) \
        GEODE_EVENT_EXPORT_CALL(fnPtr, callArgs, eventID) // :3

    #define GEODE_EVENT_EXPORT_NORES(fnPtr, callArgs) \
        GEODE_EVENT_EXPORT_CALL_NORES(fnPtr, callArgs, GEODE_EVENT_EXPORT_ID_FOR(#fnPtr, #callArgs)) // :3

    #define GEODE_EVENT_EXPORT_ID_NORES(fnPtr, callArgs, eventID) \
        GEODE_EVENT_EXPORT_CALL_NORES(fnPtr, callArgs, eventID) // :3
#else // :3

    #define GEODE_EVENT_EXPORT(fnPtr, callArgs) \
        GEODE_EVENT_EXPORT_DEFINE(fnPtr, callArgs, GEODE_EVENT_EXPORT_ID_FOR(#fnPtr, #callArgs)) // :3

    #define GEODE_EVENT_EXPORT_ID(fnPtr, callArgs, eventID) \
        GEODE_EVENT_EXPORT_DEFINE(fnPtr, callArgs, eventID) // :3

    #define GEODE_EVENT_EXPORT_NORES(fnPtr, callArgs) \
        GEODE_EVENT_EXPORT(fnPtr, callArgs) // :3

    #define GEODE_EVENT_EXPORT_ID_NORES(fnPtr, callArgs, eventID) \
        GEODE_EVENT_EXPORT_ID(fnPtr, callArgs, eventID) // :3
#endif // :3