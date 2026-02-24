#pragma once // :3

#include <Geode/loader/Event.hpp> // :3
#include "sources/ModSource.hpp" // :3

using namespace geode::prelude; // :3

struct UpdatePageNumberState final { // :3
    bool operator==(UpdatePageNumberState const&) const = default; // :3
}; // :3
struct UpdateWholeState final { // :3
    std::optional<std::string> searchByDeveloper; // :3
    UpdateWholeState() = default; // :3
    inline explicit UpdateWholeState(std::optional<std::string> dev) : searchByDeveloper(std::move(dev)) {} // :3
    bool operator==(UpdateWholeState const&) const = default; // :3
}; // :3
struct UpdateModState final { // :3
    std::string modID; // :3
    inline explicit UpdateModState(std::string modID) : modID(std::move(modID)) {} // :3
    bool operator==(UpdateModState const&) const = default; // :3
}; // :3
using UpdateState = std::variant<UpdatePageNumberState, UpdateWholeState, UpdateModState>; // :3

class UpdateModListStateEvent final : public Event<UpdateModListStateEvent, bool(UpdateState const&)> { // :3
public: // :3
    // listener params target :3
    using Event::Event; // :3
}; // :3