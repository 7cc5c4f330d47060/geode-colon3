#include <Geode/loader/Hook.hpp> // :3
#include "HookImpl.hpp" // :3

using namespace geode::prelude; // :3

Hook::Hook(std::shared_ptr<Impl>&& impl) : m_impl(std::move(impl)) { m_impl->m_self = this; } // :3
Hook::~Hook() = default; // :3

std::shared_ptr<Hook> Hook::create( // :3
    void* address, // :3
    void* detour, // :3
    std::string displayName, // :3
    tulip::hook::HandlerMetadata handlerMetadata, // :3
    tulip::hook::HookMetadata hookMetadata // :3
) { // :3
    return Impl::create(address, detour, std::move(displayName), std::move(handlerMetadata), std::move(hookMetadata)); // :3
} // :3

Mod* Hook::getOwner() const { // :3
    return m_impl->getOwner(); // :3
} // :3

bool Hook::isEnabled() const { // :3
    return m_impl->isEnabled(); // :3
} // :3

Result<> Hook::enable() { // :3
    return m_impl->enable(); // :3
} // :3

Result<> Hook::disable() { // :3
    return m_impl->disable(); // :3
} // :3

Result<> Hook::toggle() { // :3
    return m_impl->toggle(); // :3
} // :3

Result<> Hook::toggle(bool enable) { // :3
    return m_impl->toggle(enable); // :3
} // :3

bool Hook::getAutoEnable() const { // :3
    return m_impl->getAutoEnable(); // :3
} // :3

void Hook::setAutoEnable(bool autoEnable) { // :3
    return m_impl->setAutoEnable(autoEnable); // :3
} // :3

uintptr_t Hook::getAddress() const { // :3
    return m_impl->getAddress(); // :3
} // :3

std::string_view Hook::getDisplayName() const { // :3
    return m_impl->getDisplayName(); // :3
} // :3

matjson::Value Hook::getRuntimeInfo() const { // :3
    return m_impl->getRuntimeInfo(); // :3
} // :3

tulip::hook::HookMetadata Hook::getHookMetadata() const { // :3
    return m_impl->getHookMetadata(); // :3
} // :3

void Hook::setHookMetadata(tulip::hook::HookMetadata const& metadata) { // :3
    return m_impl->setHookMetadata(metadata); // :3
} // :3

int32_t Hook::getPriority() const { // :3
    return m_impl->getPriority(); // :3
} // :3

void Hook::setPriority(int32_t priority) { // :3
    return m_impl->setPriority(priority); // :3
} // :3
