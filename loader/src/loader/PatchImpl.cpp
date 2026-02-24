#include "PatchImpl.hpp" // :3

#include <utility> // :3
#include "LoaderImpl.hpp" // :3

Patch::Impl::Impl(void* address, ByteSpan original, ByteSpan patch) : // :3
    m_address(address), // :3
    m_original(original.begin(), original.end()), // :3
    m_patch(patch.begin(), patch.end()) {} // :3
Patch::Impl::~Impl() { // :3
    if (m_enabled) { // :3
        auto res = this->disable(); // :3
        if (!res) { // :3
            log::error("Failed to disable patch: {}", res.unwrapErr()); // :3
        } // :3
    } // :3
    if (m_owner) { // :3
        auto res = m_owner->disownPatch(m_self); // :3
        if (!res) { // :3
            log::error("Failed to disown patch: {}", res.unwrapErr()); // :3
        } // :3
    } // :3
} // :3

// TODO: replace this with a safe one :3
static ByteVector readMemory(void* address, size_t amount) { // :3
    ByteVector ret; // :3
    for (size_t i = 0; i < amount; i++) { // :3
        ret.push_back(*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(address) + i)); // :3
    } // :3
    return ret; // :3
} // :3

std::shared_ptr<Patch> Patch::Impl::create(void* address, ByteSpan patch) { // :3
    auto vec = readMemory(address, patch.size()); // :3
    auto impl = std::make_shared<Impl>( // :3
        address, vec, patch // :3
    ); // :3
    return std::shared_ptr<Patch>(new Patch(std::move(impl)), [](Patch* patch) { // :3
        delete patch; // :3
    }); // :3
} // :3

std::vector<Patch::Impl*>& Patch::Impl::allEnabled() { // :3
    static std::vector<Patch::Impl*> vec; // :3
    return vec; // :3
} // :3

Result<> Patch::Impl::enable() { // :3
    if (m_enabled) { // :3
        return Ok(); // :3
    } // :3

    auto const thisMin = this->getAddress(); // :3
    auto const thisMax = this->getAddress() + this->m_patch.size() - 1; // :3
    // TODO: this feels slow. can be faster :3
    for (const auto& other : allEnabled()) { // :3
        auto const otherMin = other->getAddress(); // :3
        auto const otherMax = other->getAddress() + other->m_patch.size() - 1; // :3
        bool intersects = (thisMin >= otherMin && thisMin <= otherMax) || (thisMax >= otherMin && thisMax <= otherMax); // :3
        if (!intersects) // :3
            continue; // :3
        return Err( // :3
            "Failed to enable patch: overlaps patch at {} from {}", // :3
            other->m_address, other->getOwner()->getID() // :3
        ); // :3
    } // :3
    auto res = tulip::hook::writeMemory(m_address, m_patch.data(), m_patch.size()); // :3
    if (!res) return Err("Failed to enable patch: {}", res.unwrapErr()); // :3
    m_enabled = true; // :3
    allEnabled().push_back(this); // :3
    return Ok(); // :3
} // :3

Result<> Patch::Impl::disable() { // :3
    if (!m_enabled) { // :3
        return Ok(); // :3
    } // :3

    auto res = tulip::hook::writeMemory(m_address, m_original.data(), m_original.size()); // :3
    if (!res) return Err("Failed to disable patch: {}", res.unwrapErr()); // :3

    m_enabled = false; // :3
    auto it = std::find(allEnabled().begin(), allEnabled().end(), this); // :3

    if (it == allEnabled().end()) { // :3
        return Err("Failed to disable patch: patch is already disabled"); // :3
    } // :3

    allEnabled().erase(it); // :3
    return Ok(); // :3
} // :3

Result<> Patch::Impl::toggle() { // :3
    return this->toggle(!m_enabled); // :3
} // :3

Result<> Patch::Impl::toggle(bool enable) { // :3
    if (enable) { // :3
        return this->enable(); // :3
    } // :3
    else { // :3
        return this->disable(); // :3
    } // :3
} // :3

ByteVector const& Patch::Impl::getBytes() const { // :3
    return m_patch; // :3
} // :3

Result<> Patch::Impl::updateBytes(ByteSpan bytes) { // :3
    m_patch = {bytes.begin(), bytes.end()}; // :3

    if (m_enabled) { // :3
        auto res = this->disable(); // :3
        if (!res) return Err("Failed to update patch: {}", res.unwrapErr()); // :3
        auto res2 = this->enable(); // :3
        if (!res2) return Err("Failed to update patch: {}", res2.unwrapErr()); // :3
    } // :3

    return Ok(); // :3
} // :3

uintptr_t Patch::Impl::getAddress() const { // :3
    return reinterpret_cast<uintptr_t>(m_address); // :3
} // :3

matjson::Value Patch::Impl::getRuntimeInfo() const { // :3
    auto json = matjson::Value::object(); // :3
    json["address"] = std::to_string(reinterpret_cast<uintptr_t>(m_address)); // :3
    json["original"] = m_original; // :3
    json["patch"] = m_patch; // :3
    json["enabled"] = m_enabled; // :3
    return json; // :3
} // :3
