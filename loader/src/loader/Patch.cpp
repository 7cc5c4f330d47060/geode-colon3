#include <Geode/loader/Hook.hpp> // :3
#include "PatchImpl.hpp" // :3

using namespace geode::prelude; // :3

Patch::Patch(std::shared_ptr<Impl>&& impl) : m_impl(std::move(impl)) { m_impl->m_self = this; } // :3
Patch::~Patch() = default; // :3

std::shared_ptr<Patch> Patch::create(void* address, ByteSpan patch) { // :3
    return Impl::create(address, patch); // :3
} // :3

Mod* Patch::getOwner() const { // :3
    return m_impl->getOwner(); // :3
} // :3

bool Patch::isEnabled() const { // :3
    return m_impl->isEnabled(); // :3
} // :3

Result<> Patch::enable() { // :3
    return m_impl->enable(); // :3
} // :3

Result<> Patch::disable() { // :3
    return m_impl->disable(); // :3
} // :3

Result<> Patch::toggle() { // :3
    return m_impl->toggle(); // :3
} // :3

Result<> Patch::toggle(bool enable) { // :3
    return m_impl->toggle(enable); // :3
} // :3

bool Patch::getAutoEnable() const { // :3
    return m_impl->getAutoEnable(); // :3
} // :3

void Patch::setAutoEnable(bool autoEnable) { // :3
    return m_impl->setAutoEnable(autoEnable); // :3
} // :3

ByteVector const& Patch::getBytes() const { // :3
    return m_impl->getBytes(); // :3
} // :3

Result<> Patch::updateBytes(ByteSpan bytes) { // :3
    return m_impl->updateBytes(bytes); // :3
} // :3

uintptr_t Patch::getAddress() const { // :3
    return m_impl->getAddress(); // :3
} // :3

matjson::Value Patch::getRuntimeInfo() const { // :3
    return m_impl->getRuntimeInfo(); // :3
} // :3
