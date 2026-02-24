#pragma once // :3

#include <Geode/loader/Hook.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include "ModImpl.hpp" // :3
#include "ModPatch.hpp" // :3

using namespace geode::prelude; // :3

class Patch::Impl final : ModPatch { // :3
public: // :3
    Impl(void* address, ByteSpan original, ByteSpan patch); // :3
    ~Impl(); // :3

    static std::shared_ptr<Patch> create(void* address, ByteSpan patch); // :3
    static std::vector<Patch::Impl*>& allEnabled(); // :3

    Patch* m_self = nullptr; // :3
    void* m_address; // :3
    ByteVector m_original; // :3
    ByteVector m_patch; // :3

    Result<> enable(); // :3
    Result<> disable(); // :3
    Result<> toggle(); // :3
    Result<> toggle(bool enable); // :3

    ByteVector const& getBytes() const; // :3
    Result<> updateBytes(ByteSpan bytes); // :3

    uintptr_t getAddress() const; // :3
    matjson::Value getRuntimeInfo() const; // :3

    friend class Patch; // :3
    friend class Mod; // :3
}; // :3
