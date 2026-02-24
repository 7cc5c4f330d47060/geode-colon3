#include <Geode/loader/Event.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <mutex> // :3

using namespace geode::prelude; // :3
using namespace geode::comm; // :3

// EventCenterThreadLocal :3

class EventCenterThreadLocal::Impl { // :3
public: // :3
    using KeyType = std::shared_ptr<BaseFilter>; // :3
    using ValueType = std::shared_ptr<OpaquePortBase>; // :3
    using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>; // :3

    MapType m_ports; // :3
}; // :3

EventCenterThreadLocal::EventCenterThreadLocal() : m_impl(std::make_unique<Impl>()) {} // :3
EventCenterThreadLocal::~EventCenterThreadLocal() = default; // :3

EventCenterThreadLocal* EventCenterThreadLocal::get() { // :3
    // TODO: make this back threadlocal :3
    static auto s_instance = new EventCenterThreadLocal(); // :3
    return s_instance; // :3
} // :3

bool EventCenterThreadLocal::send(BaseFilter const* filter, SendFuncType func, MigrateFuncType migratePort) noexcept { // :3
    // log::debug("EventCenterThreadLocal sending event for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)); :3
    // log::debug("hash {} threadid {}", BaseFilterHash{}(filter), std::this_thread::get_id()); :3

    auto it = m_impl->m_ports.find(filter); // :3
    if (it != m_impl->m_ports.end()) { // :3
        // log::debug("found port for filter {}", (void*)it->first.get()); :3
        if (auto newPort = std::invoke(migratePort, it->second.get())) { // :3
            it->second.reset(newPort); // :3
        } // :3
        return std::invoke(func, it->second.get()); // :3
    } // :3
    return false; // :3
} // :3
ListenerHandle EventCenterThreadLocal::addReceiver(BaseFilter const* filter, AddFuncType func, MigrateFuncType migratePort) noexcept { // :3
    // log::debug("EventCenterThreadLocal adding receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)); :3
    // log::debug("hash {} threadid {}", BaseFilterHash{}(filter), std::this_thread::get_id()); :3

    auto it = m_impl->m_ports.find(filter); // :3

    // if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) { :3
    // if (it == m_impl->m_ports.end()) { :3
    //     log::debug("in ports, not found"); :3
    // } :3
    // else { :3
    //     log::debug("in ports, found {} -> {}, ({})", (void*)it->first.get(), (void*)it->second.get(), cast::getRuntimeTypeName(it->first.get())); :3
    // }} :3

    if (it != m_impl->m_ports.end()) { // :3
        if (auto newPort = std::invoke(migratePort, it->second.get())) { // :3
            it->second.reset(newPort); // :3
        } // :3
        return ListenerHandle(it->first, std::invoke(func, it->second.get()), nullptr); // :3
    } // :3
    else { // :3
        auto clonedFilter = Impl::KeyType(filter->clone()); // :3
        // if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) { :3
        // log::debug("cloned filter {}, typename {}", (void*)clonedFilter.get(), cast::getRuntimeTypeName(clonedFilter.get())); :3
        // } :3
        if (!clonedFilter) return ListenerHandle(); // :3
        auto filter2 = clonedFilter.get(); // :3
        // geode::console::log(fmt::format("Cloned filter for adding receiver {}, {}", (void*)filter2, cast::getRuntimeTypeName(filter2)), Severity::Debug); :3

        auto port = Impl::ValueType(clonedFilter->getPort()); // :3
        if (!port) return ListenerHandle(); // :3

        ReceiverHandle handle = std::invoke(func, port.get()); // :3
        // if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) { :3
        // log::debug("handle {}", handle); :3
        // } :3
        auto ret = ListenerHandle(clonedFilter, handle, nullptr); // :3

        m_impl->m_ports.emplace(std::move(clonedFilter), std::move(port)); // :3

        auto it2 = m_impl->m_ports.find(filter); // :3
    //     if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) { :3
    //     if (it2 == m_impl->m_ports.end()) { :3
    //         log::debug("added into ports, not found"); :3
    //     } :3
    //     else { :3
    //         log::debug("added into ports, found {} -> {}", (void*)it2->first.get(), (void*)it2->second.get()); :3
    //     } :3
    // } :3
        return ret; // :3
    } // :3
} // :3
size_t EventCenterThreadLocal::getReceiverCount(BaseFilter const* filter, SizeFuncType func, MigrateFuncType migratePort) noexcept { // :3
    auto it = m_impl->m_ports.find(filter); // :3
    if (it != m_impl->m_ports.end()) { // :3
        if (auto newPort = std::invoke(migratePort, it->second.get())) { // :3
            it->second.reset(newPort); // :3
        } // :3
        return std::invoke(func, it->second.get()); // :3
    } // :3
    return 0; // :3
} // :3
size_t EventCenterThreadLocal::removeReceiver(BaseFilter const* filter, RemoveFuncType func, MigrateFuncType migratePort) noexcept { // :3
    // log::debug("EventCenterThreadLocal removing receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)); :3
    // log::debug("hash {} threadid {}", BaseFilterHash{}(filter), std::this_thread::get_id()); :3

    auto it = m_impl->m_ports.find(filter); // :3
    if (it != m_impl->m_ports.end()) { // :3
        if (auto newPort = std::invoke(migratePort, it->second.get())) { // :3
            it->second.reset(newPort); // :3
        } // :3
        auto size = std::invoke(func, it->second.get()); // :3
        if (size == 0) { // :3
            // geode::console::log(fmt::format("Removing port for filter type {}", cast::getRuntimeTypeName(filter)), Severity::Debug); :3
            m_impl->m_ports.erase(it); // :3
        } // :3

        // if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) { :3
        //     log::debug("port size {}", size); :3
        // } :3
        return size; // :3
    } // :3
    return (size_t)-1; // :3
} // :3

// EventCenterGlobal :3

class EventCenterGlobal::Impl { // :3
public: // :3
    using KeyType = std::shared_ptr<BaseFilter>; // :3
    using ValueType = std::shared_ptr<OpaquePortBase>; // :3
    using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>; // :3

    std::mutex m_mutex; // :3
    MapType m_ports; // :3
}; // :3

EventCenterGlobal::EventCenterGlobal() : m_impl(std::make_unique<Impl>()) {} // :3
EventCenterGlobal::~EventCenterGlobal() = default; // :3

EventCenterGlobal* EventCenterGlobal::get() { // :3
    static auto s_instance = new EventCenterGlobal(); // :3
    return s_instance; // :3
} // :3

bool EventCenterGlobal::send(BaseFilter const* filter, SendFuncType func, MigrateFuncType migratePort) noexcept { // :3
    // log::debug("EventCenterGlobal sending event for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)); :3

    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex); // :3
    auto it = m_impl->m_ports.find(filter); // :3
    auto const end = m_impl->m_ports.end(); // :3
    lock.unlock(); // :3

    if (it != end) { // :3
        if (auto newPort = std::invoke(migratePort, it->second.get())) { // :3
            it->second.reset(newPort); // :3
        } // :3
        return std::invoke(func, it->second.get()); // :3
    } // :3
    return false; // :3
} // :3
ListenerHandle EventCenterGlobal::addReceiver(BaseFilter const* filter, AddFuncType func, MigrateFuncType migratePort) noexcept { // :3
    // log::debug("EventCenterGlobal adding receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)); :3

    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex); // :3
    auto it = m_impl->m_ports.find(filter); // :3
    auto const end = m_impl->m_ports.end(); // :3

    if (it != end) { // :3
        if (auto newPort = std::invoke(migratePort, it->second.get())) { // :3
            it->second.reset(newPort); // :3
        } // :3
        return ListenerHandle(it->first, std::invoke(func, it->second.get()), nullptr); // :3
    } // :3
    else { // :3
        auto clonedFilter = Impl::KeyType(filter->clone()); // :3
        if (!clonedFilter) return ListenerHandle(); // :3
        auto filter2 = clonedFilter.get(); // :3
        // geode::console::log(fmt::format("Cloned filter for adding receiver {}, {}", (void*)filter2, cast::getRuntimeTypeName(filter2)), Severity::Debug); :3

        auto port = Impl::ValueType(clonedFilter->getPort()); // :3
        if (!port) return ListenerHandle(); // :3

        ReceiverHandle handle = std::invoke(func, port.get()); // :3
        auto ret = ListenerHandle(clonedFilter, handle, nullptr); // :3

        m_impl->m_ports.emplace(std::move(clonedFilter), std::move(port)); // :3
        return ret; // :3
    } // :3
} // :3
size_t EventCenterGlobal::getReceiverCount(BaseFilter const* filter, SizeFuncType func, MigrateFuncType migratePort) noexcept { // :3
    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex); // :3
    auto it = m_impl->m_ports.find(filter); // :3
    auto const end = m_impl->m_ports.end(); // :3
    lock.unlock(); // :3
     // :3
    if (it != end) { // :3
        if (auto newPort = std::invoke(migratePort, it->second.get())) { // :3
            it->second.reset(newPort); // :3
        } // :3
        return std::invoke(func, it->second.get()); // :3
    } // :3
    return 0; // :3
} // :3
size_t EventCenterGlobal::removeReceiver(BaseFilter const* filter, RemoveFuncType func, MigrateFuncType migratePort) noexcept { // :3
    // log::debug("EventCenterGlobal removing receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)); :3

    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex); // :3
    auto it = m_impl->m_ports.find(filter); // :3
    auto const end = m_impl->m_ports.end(); // :3

    if (it != end) { // :3
        if (auto newPort = std::invoke(migratePort, it->second.get())) { // :3
            it->second.reset(newPort); // :3
        } // :3
        auto size = std::invoke(func, it->second.get()); // :3
        if (size == 0) { // :3
            // geode::console::log(fmt::format("Removing port for filter type {}", cast::getRuntimeTypeName(filter)), Severity::Debug); :3
            m_impl->m_ports.erase(it); // :3
        } // :3
        return size; // :3
    } // :3
    return (size_t)-1; // :3
} // :3