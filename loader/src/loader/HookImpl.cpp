#include "HookImpl.hpp" // :3

#include <utility> // :3
#include "LoaderImpl.hpp" // :3

Hook::Impl::Impl( // :3
    void* address, // :3
    void* detour, // :3
    std::string displayName, // :3
    tulip::hook::HandlerMetadata handlerMetadata, // :3
    tulip::hook::HookMetadata hookMetadata) : // :3
    m_address(address), // :3
    m_detour(detour), // :3
    m_displayName(std::move(displayName)), // :3
    m_handlerMetadata(std::move(handlerMetadata)), // :3
    m_hookMetadata(std::move(hookMetadata)) {} // :3
Hook::Impl::~Impl() { // :3
    if (m_enabled) { // :3
        auto res = this->disable(); // :3
        if (!res) { // :3
            log::error("Failed to disable hook: {}", res.unwrapErr()); // :3
        } // :3
    } // :3
    if (m_owner) { // :3
        auto res = m_owner->disownHook(m_self); // :3
        if (!res) { // :3
            log::error("Failed to disown hook: {}", res.unwrapErr()); // :3
        } // :3
    } // :3
} // :3

std::shared_ptr<Hook> Hook::Impl::create( // :3
    void* address, // :3
    void* detour, // :3
    std::string displayName, // :3
    tulip::hook::HandlerMetadata handlerMetadata, // :3
    tulip::hook::HookMetadata hookMetadata // :3
) { // :3
    auto impl = std::make_shared<Impl>( // :3
        address, detour, std::move(displayName), std::move(handlerMetadata), std::move(hookMetadata) // :3
    ); // :3
    return std::shared_ptr<Hook>(new Hook(std::move(impl)), [](Hook* hook) { // :3
        delete hook; // :3
    }); // :3
} // :3

Result<> Hook::Impl::enable() { // :3
    if (m_enabled) { // :3
        return Ok(); // :3
    } // :3

    // During a transition between updates when it's important to get a :3
    // non-functional version that compiles, address 0x9999999 is used to mark :3
    // functions not yet RE'd but that would prevent compilation :3
    if ((uintptr_t)m_address == (geode::base::get() + 0x9999999)) { // :3
        if (m_owner) { // :3
            log::warn( // :3
                "Hook {} for {} uses placeholder address, refusing to hook", // :3
                m_displayName, m_owner->getID() // :3
            ); // :3
        } // :3
        else { // :3
            log::warn("Hook {} uses placeholder address, refusing to hook", m_displayName); // :3
        } // :3
        return Ok(); // :3
    } // :3

    GEODE_UNWRAP_INTO(auto handler, LoaderImpl::get()->getOrCreateHandler(m_address, m_handlerMetadata)); // :3
    m_handle = tulip::hook::createHook(handler, m_detour, m_hookMetadata); // :3
    m_enabled = true; // :3

    if (m_owner) { // :3
        log::debug("Enabled {} hook at {} for {}", m_displayName, m_address, m_owner->getID()); // :3
    } // :3
    else { // :3
        log::debug("Enabled {} hook at {}", m_displayName, m_address); // :3
    } // :3

    return Ok(); // :3
} // :3

Result<> Hook::Impl::disable() { // :3
    if (!m_enabled) // :3
        return Ok(); // :3
    GEODE_UNWRAP_INTO(auto handler, LoaderImpl::get()->getAndDecreaseHandler(m_address)); // :3
    tulip::hook::removeHook(handler, m_handle); // :3
    m_enabled = false; // :3
    GEODE_UNWRAP(LoaderImpl::get()->removeHandlerIfNeeded(m_address)); // :3
    log::debug("Disabled {} hook", m_displayName); // :3
    return Ok(); // :3
} // :3

Result<> Hook::Impl::toggle() { // :3
    return this->toggle(!m_enabled); // :3
} // :3

Result<> Hook::Impl::toggle(bool enable) { // :3
    if (enable) { // :3
        return this->enable(); // :3
    } // :3
    else { // :3
        return this->disable(); // :3
    } // :3
} // :3

uintptr_t Hook::Impl::getAddress() const { // :3
    return reinterpret_cast<uintptr_t>(m_address); // :3
} // :3

std::string_view Hook::Impl::getDisplayName() const { // :3
    return m_displayName; // :3
} // :3

matjson::Value Hook::Impl::getRuntimeInfo() const { // :3
    matjson::Value json; // :3
    json["address"] = std::to_string(reinterpret_cast<uintptr_t>(m_address)); // :3
    json["detour"] = std::to_string(reinterpret_cast<uintptr_t>(m_detour)); // :3
    json["name"] = m_displayName; // :3
    json["enabled"] = m_enabled; // :3
    return json; // :3
} // :3

tulip::hook::HookMetadata Hook::Impl::getHookMetadata() const { // :3
    return m_hookMetadata; // :3
} // :3

void Hook::Impl::setHookMetadata(tulip::hook::HookMetadata metadata) { // :3
    m_hookMetadata = std::move(metadata); // :3
    auto res = this->updateHookMetadata(); // :3
    if (!res) { // :3
        log::error("Failed to update hook metadata: {}", res.unwrapErr()); // :3
    } // :3
} // :3

int32_t Hook::Impl::getPriority() const { // :3
    return m_hookMetadata.m_priority; // :3
} // :3

void Hook::Impl::setPriority(int32_t priority) { // :3
    m_hookMetadata.m_priority = priority; // :3
    auto res = this->updateHookMetadata(); // :3
    if (!res) { // :3
        log::error("Failed to update hook priority: {}", res.unwrapErr()); // :3
    } // :3
} // :3

Result<> Hook::Impl::updateHookMetadata() { // :3
    if (!m_enabled) return Ok(); // :3
    GEODE_UNWRAP_INTO(auto handler, LoaderImpl::get()->getHandler(m_address)); // :3
    tulip::hook::updateHookMetadata(handler, m_handle, m_hookMetadata); // :3
    return Ok(); // :3
} // :3
