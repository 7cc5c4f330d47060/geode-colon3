#pragma once // :3

#include <Geode/loader/Hook.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/casts.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <vector> // :3
#include "ModImpl.hpp" // :3
#include "ModPatch.hpp" // :3

using namespace geode::prelude; // :3

class Hook::Impl final : ModPatch { // :3
public: // :3
    Impl( // :3
        void* address, // :3
        void* detour, // :3
        std::string displayName, // :3
        tulip::hook::HandlerMetadata handlerMetadata, // :3
        tulip::hook::HookMetadata hookMetadata // :3
    ); // :3
    ~Impl(); // :3

    static std::shared_ptr<Hook> create( // :3
        void* address, // :3
        void* detour, // :3
        std::string displayName, // :3
        tulip::hook::HandlerMetadata handlerMetadata, // :3
        tulip::hook::HookMetadata hookMetadata // :3
    ); // :3

    template<class DetourType> // :3
    static std::shared_ptr<Hook> create( // :3
        void* address, // :3
        DetourType detour, // :3
        std::string displayName, // :3
        tulip::hook::TulipConvention convention, // :3
        tulip::hook::HookMetadata hookMetadata // :3
    ); // :3

    Hook* m_self = nullptr; // :3
    void* m_address; // :3
    void* m_detour; // :3
    std::string m_displayName; // :3
    tulip::hook::HandlerMetadata m_handlerMetadata; // :3
    tulip::hook::HookMetadata m_hookMetadata; // :3
    tulip::hook::HookHandle m_handle = 0; // :3

    Result<> enable(); // :3
    Result<> disable(); // :3
    Result<> toggle(); // :3
    Result<> toggle(bool enable); // :3

    uintptr_t getAddress() const; // :3
    std::string_view getDisplayName() const; // :3
    matjson::Value getRuntimeInfo() const; // :3
    tulip::hook::HookMetadata getHookMetadata() const; // :3
    void setHookMetadata(tulip::hook::HookMetadata metadata); // :3
    int32_t getPriority() const; // :3
    void setPriority(int32_t priority); // :3

    Result<> updateHookMetadata(); // :3

    friend class Hook; // :3
    friend class Mod; // :3
}; // :3
