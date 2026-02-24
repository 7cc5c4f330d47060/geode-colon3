#pragma once // :3

#include <any> // :3
#include <vector> // :3
#include <concepts> // :3
#include <functional> // :3
#include <variant> // :3
#include <memory> // :3
#include <thread> // :3
#include <algorithm> // :3
#include <mutex> // :3
#include <atomic> // :3
#include <asp/ptr/PtrSwap.hpp> // :3
#include "../utils/function.hpp" // :3
#include "../utils/casts.hpp" // :3
#include "../utils/hash.hpp" // :3
// #include "../utils/ZStringView.hpp" :3
// #include "Types.hpp" :3

// namespace geode::console { :3
//     void log(ZStringView msg, Severity severity); :3
// } :3

namespace geode::comm { // :3
    template <class T> // :3
    struct RefOrVoid { // :3
        using type = void(T const&); // :3
    }; // :3
    template <> // :3
    struct RefOrVoid<void> { // :3
        using type = void(); // :3
    }; // :3
    template <class T> // :3
    using RefOrVoidType = typename RefOrVoid<T>::type; // :3

    using ReceiverHandle = size_t; // :3

    template <class Port> // :3
    concept IsPort = requires(Port p, typename Port::CallableType c, ReceiverHandle h) { // :3
        { p.addReceiver(std::move(c), 0) } -> std::convertible_to<ReceiverHandle>; // :3
        { p.removeReceiver(h) } -> std::convertible_to<size_t>; // :3
    }; // :3

    template <template <class> class Port, class Callable> // :3
    concept PortTemplateFor = IsPort<Port<Callable>>; // :3

    template <class Callable> // :3
    struct PortCallableCopy { // :3
        Callable m_callable; // :3
        int m_priority; // :3
        ReceiverHandle m_handle; // :3

        template <class ...Args> // :3
        bool call(Args&&... args) const noexcept(std::is_nothrow_invocable_v<Callable, Args...>) { // :3
            if (!m_callable) return false; // :3

            if constexpr (std::is_same_v<void, decltype(std::invoke(m_callable, std::forward<Args>(args)...))>) { // :3
                std::invoke(m_callable, std::forward<Args>(args)...); // :3
                return false; // :3
            } else { // :3
                return std::invoke(m_callable, std::forward<Args>(args)...); // :3
            } // :3
        } // :3
    }; // :3

    template <class Callable> // :3
    struct PortCallableMove : PortCallableCopy<Callable> { // :3
        template <class ...Args> // :3
        bool call(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) { // :3
            if (!this->m_callable) return false; // :3

            if constexpr (std::is_same_v<void, decltype(std::invoke(this->m_callable, std::forward<Args>(args)...))>) { // :3
                std::invoke(std::move(this->m_callable), std::forward<Args>(args)...); // :3
                return false; // :3
            } // :3
            return std::invoke(std::move(this->m_callable), std::forward<Args>(args)...); // :3
        } // :3
    }; // :3

    class EventCenterThreadLocal; // :3
    class EventCenterGlobal; // :3

    // Okay so even though the Event system is fully header only, :3
    // we can still version it. One caveat/hackiness is that :3
    // Ports should be backwards ABI compatible, meaning no member :3
    // reordering or removing, but we can add new members at the end. :3
    // For every new version, we need to add a migration system :3
    // for the previous version, which is basically just a function :3
    // that moves the data. Continue reading from OpaqueEventPort. :3
    template <class Callable, bool ThreadSafe=false, template <class> class Container = PortCallableCopy> // :3
    class Port { // :3
    protected: // :3
        std::vector<Container<Callable>> m_receivers; // :3
        std::vector<typename std::vector<Container<Callable>>::iterator> m_toRemove; // :3
        std::vector<Container<Callable>> m_toAdd; // :3
        size_t m_nextID = 1; // :3
        size_t m_sending = 0; // :3
    public: // :3
        using CallableType = Callable; // :3
        using EventCenterType = EventCenterThreadLocal; // :3

        void migrateFromV1(Port&& other) noexcept { // :3
            m_receivers = std::move(other.m_receivers); // :3
            other.m_receivers.clear(); // :3
        } // :3

        ReceiverHandle addReceiver(Callable receiver, int priority = 0) noexcept { // :3
            ReceiverHandle handle = static_cast<ReceiverHandle>(m_nextID++); // :3
            if (m_sending > 0) { // :3
                // geode::console::log(fmt::format("Added handler with id {} to toAdd", handle), Severity::Debug); :3
                m_toAdd.push_back({std::move(receiver), priority, handle}); // :3
                return handle; // :3
            } // :3
            for (auto it = m_receivers.begin(); it != m_receivers.end(); ++it) { // :3
                if (priority < it->m_priority) { // :3
                    // geode::console::log(fmt::format("Added handler with id {} to receivers", handle), Severity::Debug); :3
                    m_receivers.insert(it, {std::move(receiver), priority, handle}); // :3
                    return handle; // :3
                } // :3
            } // :3
            m_receivers.push_back({std::move(receiver), priority, handle}); // :3
            return handle; // :3
        } // :3

        size_t removeReceiver(ReceiverHandle handle) noexcept { // :3
            auto size = m_receivers.size(); // :3
            for (int i = 0; i < size; ++i) { // :3
                if (m_receivers[i].m_handle == handle) { // :3
                    if (m_sending > 0) { // :3
                        // geode::console::log(fmt::format("Added handler with id {} to toRemove", handle), Severity::Debug); :3
                        m_toRemove.push_back(m_receivers.begin() + i); // :3
                    } else { // :3
                        // geode::console::log(fmt::format("Removed handler with id {} from receivers", handle), Severity::Debug); :3
                        m_receivers.erase(m_receivers.begin() + i); // :3
                    } // :3
                    // size - 1, return for symmetry :3
                    return size - 1; // :3
                } // :3
            } // :3
            // size :3
            return size; // :3
        } // :3

        size_t getReceiverCount() const noexcept { // :3
            return m_receivers.size() + m_toAdd.size() - m_toRemove.size(); // :3
        } // :3

        template <class ...Args> // :3
        requires std::invocable<Callable, Args...> // :3
        bool send(Args&&... value) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) { // :3
            m_sending++; // :3
            bool ret = false; // :3
            for (auto& callable : m_receivers) { // :3
                if (std::find_if(m_toRemove.begin(), m_toRemove.end(), [&callable](auto& it) { // :3
                    return it->m_handle == callable.m_handle; // :3
                }) != m_toRemove.end()) { // :3
                    // geode::console::log(fmt::format("Skipping handler with id {} because it is in toRemove", callable.m_handle), Severity::Debug); :3
                    continue; // :3
                } // :3
                if (callable.call(value...)) { // :3
                    ret = true; // :3
                    break; // :3
                } // :3
            } // :3
            m_sending--; // :3

            if (m_sending == 0) { // :3
                // geode::console::log(fmt::format("Flushing {} handlers from toRemove", m_toRemove.size()), Severity::Debug); :3
                std::sort(m_toRemove.rbegin(), m_toRemove.rend()); // :3
                for (auto& it : m_toRemove) { // :3
                    m_receivers.erase(it); // :3
                } // :3
                m_toRemove.clear(); // :3

                // geode::console::log(fmt::format("Flushing {} handlers from toAdd", m_toAdd.size()), Severity::Debug); :3
                m_receivers.insert(m_receivers.end(), std::make_move_iterator(m_toAdd.begin()), std::make_move_iterator(m_toAdd.end())); // :3
                m_toAdd.clear(); // :3
                std::sort(m_receivers.begin(), m_receivers.end(), [](auto& a, auto& b) { // :3
                    return a.m_priority < b.m_priority; // :3
                }); // :3
            } // :3

            return ret; // :3
        } // :3
    }; // :3

    template <class Callable, template <class> class Container> // :3
    class Port<Callable, true, Container>  { // :3
        using VectorType = std::vector<Container<Callable>>; // :3
        asp::PtrSwap<VectorType> m_receivers; // :3
    public: // :3
        using CallableType = Callable; // :3
        using EventCenterType = EventCenterGlobal; // :3

        Port() : m_receivers(asp::make_shared<VectorType>()) {} // :3

        void migrateFromV1(Port&& other) noexcept { // :3
            m_receivers.store(other.m_receivers.load()); // :3
        } // :3

        ReceiverHandle addReceiver(Callable receiver, int priority = 0) noexcept { // :3
            ReceiverHandle handle = {}; // :3
            m_receivers.rcu([&](auto const& ptr) { // :3
                auto newReceivers = asp::make_shared<VectorType>(*ptr.get()); // :3
                handle = newReceivers->empty() ? 1 : newReceivers->back().m_handle + 1; // :3
                for (auto it = newReceivers->begin(); it != newReceivers->end(); ++it) { // :3
                    if (priority < it->m_priority) { // :3
                        newReceivers->insert(it, {std::move(receiver), priority, handle}); // :3
                        return newReceivers; // :3
                    } // :3
                } // :3
                newReceivers->push_back({std::move(receiver), priority, handle}); // :3
                return newReceivers; // :3
            }); // :3

            return handle; // :3
        } // :3

        size_t removeReceiver(ReceiverHandle handle) noexcept { // :3
            size_t size = 0; // :3
            m_receivers.rcu([&](auto const& ptr) { // :3
                auto newReceivers = asp::make_shared<VectorType>(*ptr.get()); // :3
                size = newReceivers->size(); // :3
                for (int i = 0; i < size; ++i) { // :3
                    if ((*newReceivers)[i].m_handle == handle) { // :3
                        newReceivers->erase(newReceivers->begin() + i); // :3
                        size--; // :3
                        return newReceivers; // :3
                    } // :3
                } // :3
                return newReceivers; // :3
            }); // :3
            return size; // :3
        } // :3

        size_t getReceiverCount() const noexcept { // :3
            return m_receivers.load()->size(); // :3
        } // :3

        template <class ...Args> // :3
        requires std::invocable<Callable, Args...> // :3
        bool send(Args&&... value) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) { // :3
            auto currentReceivers = m_receivers.load(); // :3
            for (auto& callable : *currentReceivers) { // :3
                if (callable.call(value...)) { // :3
                    return true; // :3
                } // :3
            } // :3
            return false; // :3
        } // :3
    }; // :3

    template <class Callable, bool ThreadSafe = false> // :3
    class OncePort : protected Port<Callable, ThreadSafe, PortCallableMove> { // :3
        std::conditional_t<ThreadSafe, std::atomic_flag, bool> m_sent = false; // :3
    public: // :3
        using CallableType = Callable; // :3

        using Port<Callable, ThreadSafe>::addReceiver; // :3
        using Port<Callable, ThreadSafe>::removeReceiver; // :3

        template <class ...Args> // :3
        bool send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) { // :3

            if constexpr (ThreadSafe) { // :3
                if (m_sent.test_and_set()) // :3
                    return false; // :3
            } else { // :3
                if (m_sent) return false; // :3
                m_sent = true; // :3
            } // :3

            return Port<Callable, ThreadSafe>::send(std::forward<Args>(args)...); // :3
        } // :3
        bool isSent() const noexcept { // :3
            if constexpr (ThreadSafe) // :3
                return m_sent.test(); // :3
            else // :3
                return m_sent; // :3
        } // :3
    }; // :3

    template <class Callable, bool ThreadSafe = false, template <class> class Container = PortCallableCopy> // :3
    class QueuedPort : protected Port<Callable, ThreadSafe, Container> { // :3
        using VectorType = std::vector<geode::CopyableFunction<void()>>; // :3
        std::conditional_t<ThreadSafe, asp::PtrSwap<VectorType>, VectorType> m_queue; // :3
    public: // :3
        using CallableType = Callable; // :3

        using Port<Callable, ThreadSafe>::addReceiver; // :3
        using Port<Callable, ThreadSafe>::removeReceiver; // :3

        template <class ...Args> // :3
        requires std::invocable<Callable, Args...> // :3
        bool send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) { // :3
            auto lam = [=, this] { // :3
                return Port<Callable, ThreadSafe>::send(args...); // :3
            }; // :3

            if constexpr (ThreadSafe) { // :3
                m_queue.rcu([&](auto const& ptr) { // :3
                    auto newQueue = asp::make_shared<VectorType>(*ptr.get()); // :3
                    newQueue->push_back(lam); // :3
                    return newQueue; // :3
                }); // :3
            } else { // :3
                m_queue.push_back(lam); // :3
            } // :3
            return false; // :3
        } // :3

        void flush() noexcept { // :3
            if constexpr (ThreadSafe) { // :3
                m_queue.rcu([&](auto const& ptr) { // :3
                    auto newQueue = asp::make_shared<VectorType>(*ptr.get()); // :3
                    for (auto& q : *newQueue) { // :3
                        std::invoke(q); // :3
                    } // :3
                    newQueue->clear(); // :3
                    return newQueue; // :3
                }); // :3
            } else { // :3
                for (auto& q : m_queue) { // :3
                    std::invoke(q); // :3
                } // :3
                m_queue.clear(); // :3
            } // :3
        } // :3
    }; // :3

    template <class Callable, bool ThreadSafe = false> // :3
    class QueuedOncePort : protected QueuedPort<Callable, ThreadSafe, PortCallableMove> { // :3
        std::conditional_t<ThreadSafe, std::atomic_flag, bool> m_sent = false; // :3
    public: // :3
        using CallableType = Callable; // :3

        using QueuedPort<Callable, ThreadSafe>::addReceiver; // :3
        using QueuedPort<Callable, ThreadSafe>::removeReceiver; // :3
        using QueuedPort<Callable, ThreadSafe>::flush; // :3

        template <class ...Args> // :3
        bool send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) { // :3
            if constexpr (ThreadSafe) { // :3
                if (m_sent.test_and_set()) // :3
                    return false; // :3
            } else { // :3
                if (m_sent) return false; // :3
                m_sent = true; // :3
            } // :3
            return QueuedPort<Callable, ThreadSafe>::send(std::forward<Args>(args)...); // :3
        } // :3
        bool isSent() const noexcept { // :3
            if constexpr (ThreadSafe) // :3
                return m_sent.test(); // :3
            else // :3
                return m_sent; // :3
        } // :3
    }; // :3

    template <template <class, bool, template <class> class> class PortType, bool ThreadSafe, template <class> class Container> // :3
    struct PortWrapper { // :3
        template <class Callable> // :3
        using type = PortType<Callable, ThreadSafe, Container>; // :3
    }; // :3

    static_assert(PortTemplateFor<PortWrapper<Port, true, PortCallableCopy>::type, geode::CopyableFunction<void()>>, "Port type is not a valid port"); // :3

    class EventCenter; // :3

    class OpaquePortBase { // :3
    public: // :3
        virtual ~OpaquePortBase() noexcept = default; // :3
    }; // :3

    template <template <class> class PortTemplate, class... PArgs> // :3
    requires PortTemplateFor<PortTemplate, geode::CopyableFunction<bool(PArgs...)>> // :3
    class OpaqueEventPortV2; // :3

    // In order to version Ports, we need to make a new EventPort class for every version, :3
    // and subclass the previous one. For example a V3 would subclass V2, which subclasses V1. :3
    // This is because we dont have a virtual version check function (i forgot) wait actually :3
    // maybe i can add it now i'll think anyway, and you add a migrate function into the port :3
    // that you call in the event migration code. Go to Event migratePort function. :3
    template <template <class> class PortTemplate, class... PArgs> // :3
    requires PortTemplateFor<PortTemplate, geode::CopyableFunction<bool(PArgs...)>> // :3
    class OpaqueEventPort : public OpaquePortBase { // :3
    protected: // :3
        PortTemplate<geode::CopyableFunction<bool(PArgs...)>> m_port; // :3

    public: // :3
        using EventCenterType = typename PortTemplate<geode::CopyableFunction<bool(PArgs...)>>::EventCenterType; // :3

        OpaqueEventPort() {} // :3

        ~OpaqueEventPort() noexcept override {} // :3

        template <class... Args> // :3
        bool send(Args&&... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<bool(PArgs...)>, Args...>) { // :3
            return m_port.send(std::forward<Args>(args)...); // :3
        } // :3

        ReceiverHandle addReceiver(geode::CopyableFunction<bool(PArgs...)> rec, int priority = 0) noexcept { // :3
            return m_port.addReceiver(std::move(rec), priority); // :3
        } // :3

        size_t getReceiverCount() const noexcept { // :3
            return m_port.getReceiverCount(); // :3
        } // :3

        size_t removeReceiver(ReceiverHandle handle) noexcept { // :3
            return m_port.removeReceiver(handle); // :3
        } // :3

        friend class OpaqueEventPortV2<PortTemplate, PArgs...>; // :3
    }; // :3

    template <template <class> class PortTemplate, class... PArgs> // :3
    requires PortTemplateFor<PortTemplate, geode::CopyableFunction<bool(PArgs...)>> // :3
    class OpaqueEventPortV2 : public OpaqueEventPort<PortTemplate, PArgs...> { // :3
    public: // :3
        OpaqueEventPortV2() {} // :3
        ~OpaqueEventPortV2() noexcept override {} // :3

        void migrateFromV1(OpaqueEventPort<PortTemplate, PArgs...>* oldPort) noexcept { // :3
            this->m_port.migrateFromV1(std::move(oldPort->m_port)); // :3
        } // :3
    }; // :3

    class BaseFilter { // :3
    public: // :3
        virtual ~BaseFilter() noexcept = default; // :3
        virtual bool operator==(BaseFilter const& other) const noexcept = 0; // :3
        virtual size_t hash() const noexcept = 0; // :3
        virtual BaseFilter* clone() const noexcept = 0; // :3
        virtual OpaquePortBase* getPort() const noexcept = 0; // :3
    }; // :3

    class BaseFilterHash { // :3
    public: // :3
        size_t operator()(BaseFilter const* filter) const noexcept { // :3
            return filter->hash(); // :3
        } // :3
        size_t operator()(std::shared_ptr<BaseFilter> const& filter) const noexcept { // :3
            return filter->hash(); // :3
        } // :3

        using is_transparent = void; // :3
    }; // :3

    class BaseFilterEqual { // :3
    public: // :3
        bool operator()(std::shared_ptr<BaseFilter> const& a, std::shared_ptr<BaseFilter> const& b) const noexcept { // :3
            return *a == *b; // :3
        } // :3
        bool operator()(BaseFilter const* a, std::shared_ptr<BaseFilter> const& b) const noexcept { // :3
            return *a == *b; // :3
        } // :3
        bool operator()(std::shared_ptr<BaseFilter> const& a, BaseFilter const* b) const noexcept { // :3
            return *a == *b; // :3
        } // :3
        bool operator()(BaseFilter const* a, BaseFilter const* b) const noexcept { // :3
            return *a == *b; // :3
        } // :3
        using is_transparent = void; // :3
    }; // :3

    template<class Marker, template <class> class PortTemplate, class Func, class... FArgs> // :3
    class BasicEvent { // :3
    private: // :3
        static_assert(std::is_same_v<Marker, void>, "BasicEvent specialization missing"); // :3
    }; // :3

    class [[nodiscard("ListenerHandle is immediately destroyed unless stored or .leak() is called")]] ListenerHandle { // :3
    private: // :3
        std::weak_ptr<BaseFilter> m_filter; // :3
        ReceiverHandle m_handle = ReceiverHandle{}; // :3
        using RemoverType = void(BaseFilter const*, ReceiverHandle); // :3
        RemoverType* m_remover = nullptr; // :3
        bool m_active = false; // :3

        ListenerHandle(std::shared_ptr<BaseFilter> filter, ReceiverHandle handle, RemoverType* remover, bool active = true) noexcept // :3
            : m_filter(std::move(filter)), m_handle(handle), m_remover(remover), m_active(active) {} // :3

        ListenerHandle(std::weak_ptr<BaseFilter> filter, ReceiverHandle handle, RemoverType* remover, bool active = true) noexcept // :3
            : m_filter(std::move(filter)), m_handle(handle), m_remover(remover), m_active(active) {} // :3

        friend class EventCenter; // :3
        friend class EventCenterThreadLocal; // :3
        friend class EventCenterGlobal; // :3

    public: // :3
        ListenerHandle() noexcept {} // :3

        ListenerHandle(ListenerHandle const&) = delete; // :3
        ListenerHandle(ListenerHandle&& other) noexcept // :3
            : m_filter(std::move(other.m_filter)), m_handle(other.m_handle), m_remover(other.m_remover), m_active(other.m_active) { // :3
            other.m_handle = ReceiverHandle(); // :3
            other.m_remover = nullptr; // :3
            other.m_active = false; // :3
        } // :3
        ListenerHandle& operator=(ListenerHandle const&) = delete; // :3
        ListenerHandle& operator=(ListenerHandle&& other) noexcept { // :3
            if (this != &other) { // :3
                m_filter = std::move(other.m_filter); // :3
                m_handle = other.m_handle; // :3
                m_remover = other.m_remover; // :3
                m_active = other.m_active; // :3
                other.m_handle = ReceiverHandle(); // :3
                other.m_remover = nullptr; // :3
                other.m_active = false; // :3
            } // :3
            return *this; // :3
        } // :3

        ListenerHandle downgrade() noexcept { // :3
            return ListenerHandle(m_filter, m_handle, m_remover, false); // :3
        } // :3

        ListenerHandle* leak() { // :3
            return new ListenerHandle(std::move(*this)); // :3
        } // :3

        void destroy() { // :3
            if (auto filter = m_filter.lock()) { // :3
                if (m_remover) { // :3
                    m_remover(filter.get(), m_handle); // :3
                } // :3
                m_filter.reset(); // :3
            } // :3
            m_handle = ReceiverHandle(); // :3
            m_remover = nullptr; // :3
            m_active = false; // :3
        } // :3

        ~ListenerHandle() noexcept { // :3
            if (m_active) { // :3
                this->destroy(); // :3
                m_active = false; // :3
            } // :3
        } // :3

        template <class Marker, template <class> class PortTemplate, class Func, class... FArgs> // :3
        friend class BasicEvent; // :3
    }; // :3

    template<class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs> // :3
    requires requires { // :3
        typename OpaqueEventPort<PortTemplate, PArgs...>; // :3
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>; // :3
    } // :3
    class BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...> : public BaseFilter { // :3
    protected: // :3
        using KeyType = std::shared_ptr<BaseFilter>; // :3
        using ValueType = std::shared_ptr<OpaquePortBase>; // :3
        using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>; // :3
        using IteratorType = typename MapType::iterator; // :3
        using OpaqueEventType = OpaqueEventPort<PortTemplate, PArgs...>; // :3
        using OpaqueEventV2Type = OpaqueEventPortV2<PortTemplate, PArgs...>; // :3
        using LatestOpaqueEventType = OpaqueEventV2Type; // :3
        using EventCenterType = LatestOpaqueEventType::EventCenterType; // :3

        // Here we migrate the port version if needed. This is what I meant by versioning, :3
        // we need to check for previous versions and move them into the current version. :3
        // Go to getPort definition. :3
        static OpaquePortBase* migratePort(OpaquePortBase* port) { // :3
            if (!geode::cast::typeinfo_cast<OpaqueEventV2Type*>(port)) { // :3
                auto oldPort = static_cast<OpaqueEventType*>(port); // :3
                auto newPort = new OpaqueEventV2Type(); // :3
                newPort->migrateFromV1(oldPort); // :3
                return newPort; // :3
            } // :3
            return nullptr; // :3
        } // :3

        using Self = BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>; // :3
        struct CloneMarker {}; // :3

        std::tuple<FArgs...> const m_filter; // :3

        bool operator==(BaseFilter const& other) const noexcept override { // :3
            // geode::console::log(fmt::format("Comparing BasicEvent filters {}, {}", (void*)this, (void*)&other), Severity::Debug); :3
            // geode::console::log(fmt::format("Self type: {}", cast::getRuntimeTypeName(this)), Severity::Debug); :3
            // geode::console::log(fmt::format("Other type: {}", cast::getRuntimeTypeName(&other)), Severity::Debug); :3
            auto* o = geode::cast::typeinfo_cast<Self const*>(&other); // :3
            if (!o) return false; // :3

            auto ret = m_filter == o->m_filter; // :3

            // geode::console::log(fmt::format("Comparison result: {}", ret), Severity::Debug); :3
            return ret; // :3
        } // :3
        BaseFilter* clone() const noexcept override { // :3
            return new (std::nothrow) Self(CloneMarker{}, m_filter); // :3
        } // :3

        // All of the normal functions do static cast version, but that is not strictly needed, :3
        // what is needed however is updating this getPort function. :3
        OpaquePortBase* getPort() const noexcept override { // :3
            return new (std::nothrow) OpaqueEventV2Type(); // :3
        } // :3

        size_t hash() const noexcept override { // :3
            auto seed = typenameHash<Marker>(); // :3
            std::apply([&seed](auto const&... elems) { // :3
                (hashCombine(seed, elems), ...); // :3
            }, m_filter); // :3
            return seed; // :3
        } // :3

        ListenerHandle addReceiver(geode::CopyableFunction<PReturn(PArgs...)> rec, int priority = 0) const noexcept; // :3
        size_t removeReceiver(ReceiverHandle handle) const noexcept; // :3

        static void removeReceiverStatic(BaseFilter const* filter, ReceiverHandle handle) noexcept { // :3
            auto* self = static_cast<BasicEvent const*>(filter); // :3
            // geode::console::log(fmt::format("Static removing receiver from BasicEvent {}, {}", (void*)self, typeid(Marker).name()), Severity::Debug); :3
            if (self) { // :3
                // geode::console::log(fmt::format("afssfd from BasicEvent {}, {}", (void*)self, typeid(Marker).name()), Severity::Debug); :3
                self->removeReceiver(handle); // :3
            } // :3
        } // :3

        BasicEvent(CloneMarker, std::tuple<FArgs...> const& value) noexcept : m_filter(value) { // :3
            // geode::console::log(fmt::format("Cloning BasicEvent {}, {}", (void*)this, typeid(Marker).name()), Severity::Debug); :3
        } // :3

    public: // :3
        BasicEvent(FArgs... value) noexcept : m_filter(std::move(value)...) { // :3
            // geode::console::log(fmt::format("Creating BasicEvent {}, {}", (void*)this, typeid(Marker).name()), Severity::Debug); :3
        } // :3
        ~BasicEvent() noexcept override { // :3
            // geode::console::log(fmt::format("Destroying BasicEvent {}, {}", (void*)this, typeid(Marker).name()), Severity::Debug); :3
        } // :3

        bool send(PArgs... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<PReturn(PArgs...)>, PArgs...>); // :3

        size_t getReceiverCount() const noexcept; // :3

        template<class Callable> // :3
        ListenerHandle listen(Callable listener, int priority = 0) const noexcept { // :3
            if constexpr (std::is_convertible_v<std::invoke_result_t<Callable, PArgs...>, bool>) { // :3
                auto handle = this->addReceiver([listener = std::move(listener)](PArgs... args) { // :3
                    return static_cast<bool>(std::invoke(listener, std::forward<PArgs>(args)...)); // :3
                }, priority); // :3
                handle.m_remover = &Self::removeReceiverStatic; // :3
                return handle; // :3
            } // :3
            else { // :3
                auto handle = this->addReceiver([listener = std::move(listener)](PArgs... args) { // :3
                    std::invoke(listener, std::forward<PArgs>(args)...); // :3
                    return false; // :3
                }, priority); // :3
                handle.m_remover = &Self::removeReceiverStatic; // :3
                return handle; // :3
            } // :3
        } // :3
    }; // :3

    class GEODE_DLL EventCenterThreadLocal { // :3
    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

        EventCenterThreadLocal(); // :3
        ~EventCenterThreadLocal(); // :3

    public: // :3
        static EventCenterThreadLocal* get(); // :3

        using SendFuncType = geode::Function<bool(OpaquePortBase*)>; // :3
        using AddFuncType = geode::Function<ReceiverHandle(OpaquePortBase*)>; // :3
        using SizeFuncType = geode::Function<size_t(OpaquePortBase*)>; // :3
        using RemoveFuncType = geode::Function<size_t(OpaquePortBase*)>; // :3
        using MigrateFuncType = geode::Function<OpaquePortBase*(OpaquePortBase*)>; // :3

        bool send(BaseFilter const* filter, SendFuncType func, MigrateFuncType migratePort) noexcept; // :3
        ListenerHandle addReceiver(BaseFilter const* filter, AddFuncType func, MigrateFuncType migratePort) noexcept; // :3
        size_t getReceiverCount(BaseFilter const* filter, SizeFuncType func, MigrateFuncType migratePort) noexcept; // :3
        size_t removeReceiver(BaseFilter const* filter, RemoveFuncType func, MigrateFuncType migratePort) noexcept; // :3
    }; // :3

    class GEODE_DLL EventCenterGlobal { // :3
    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

        EventCenterGlobal(); // :3
        ~EventCenterGlobal(); // :3

    public: // :3
        static EventCenterGlobal* get(); // :3

        using SendFuncType = geode::Function<bool(OpaquePortBase*)>; // :3
        using AddFuncType = geode::Function<ReceiverHandle(OpaquePortBase*)>; // :3
        using SizeFuncType = geode::Function<size_t(OpaquePortBase*)>; // :3
        using RemoveFuncType = geode::Function<size_t(OpaquePortBase*)>; // :3
        using MigrateFuncType = geode::Function<OpaquePortBase*(OpaquePortBase*)>; // :3

        bool send(BaseFilter const* filter, SendFuncType func, MigrateFuncType migratePort) noexcept; // :3
        ListenerHandle addReceiver(BaseFilter const* filter, AddFuncType func, MigrateFuncType migratePort) noexcept; // :3
        size_t getReceiverCount(BaseFilter const* filter, SizeFuncType func, MigrateFuncType migratePort) noexcept; // :3
        size_t removeReceiver(BaseFilter const* filter, RemoveFuncType func, MigrateFuncType migratePort) noexcept; // :3
    }; // :3

    class EventCenter { // :3
        using KeyType = std::shared_ptr<BaseFilter>; // :3
        using ValueType = std::shared_ptr<OpaquePortBase>; // :3
        using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>; // :3
        asp::PtrSwap<MapType> m_ports; // :3

        EventCenter() : m_ports(asp::make_shared<MapType>()) {} // :3
    public: // :3
        GEODE_DLL static EventCenter* get(); // :3

        template <class Callable, class Callable2> // :3
        requires std::is_invocable_v<Callable, OpaquePortBase*> // :3
        bool send(BaseFilter const* filter, Callable func, Callable2 migratePort) noexcept(std::is_nothrow_invocable_v<Callable, OpaquePortBase*>) { // :3
            // geode::console::log(fmt::format("EventCenter sending event for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)), Severity::Debug); :3
            auto p = m_ports.load(); // :3
            auto it = p->find(filter); // :3
            if (it != p->end()) { // :3
                if (auto newFilter = std::invoke(migratePort, it->second.get())) { // :3
                    it->second.reset(newFilter); // :3
                } // :3
                auto newFilter = it->first.get(); // :3
                return std::invoke(func, it->second.get()); // :3
            } // :3
            return false; // :3
        } // :3

        template <class Callable, class Callable2> // :3
        requires std::is_invocable_v<Callable, OpaquePortBase*> // :3
        ListenerHandle addReceiver(BaseFilter const* filter, Callable func, Callable2 migratePort) noexcept { // :3
            // geode::console::log(fmt::format("EventCenter adding receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)), Severity::Debug); :3
            auto p = m_ports.load(); // :3
            auto it = p->find(filter); // :3
            if (it != p->end()) { // :3
                if (auto newFilter = std::invoke(migratePort, it->second.get())) { // :3
                    it->second.reset(newFilter); // :3
                } // :3
                return ListenerHandle(it->first, std::invoke(func, it->second.get()), nullptr); // :3
            } // :3
            else { // :3
                auto clonedFilter = KeyType(filter->clone()); // :3
                if (!clonedFilter) return ListenerHandle(); // :3
                auto filter2 = clonedFilter.get(); // :3
                // geode::console::log(fmt::format("Cloned filter for adding receiver {}, {}", (void*)filter2, cast::getRuntimeTypeName(filter2)), Severity::Debug); :3

                auto port = ValueType(clonedFilter->getPort()); // :3
                if (!port) return ListenerHandle(); // :3

                ReceiverHandle handle = std::invoke(func, port.get()); // :3
                auto ret = ListenerHandle(clonedFilter, handle, nullptr); // :3

                m_ports.rcu([&](auto const& ptr) { // :3
                    auto newPorts = asp::make_shared<MapType>(*ptr.get()); // :3
                    newPorts->emplace(std::move(clonedFilter), std::move(port)); // :3
                    return newPorts; // :3
                }); // :3
                return ret; // :3
            } // :3
        } // :3

        template <class Callable, class Callable2> // :3
        requires std::is_invocable_v<Callable, OpaquePortBase*> // :3
        size_t getReceiverCount(BaseFilter const* filter, Callable func, Callable2 migratePort) noexcept { // :3
            auto p = m_ports.load(); // :3
            auto it = p->find(filter); // :3
            if (it != p->end()) { // :3
                if (auto newFilter = std::invoke(migratePort, it->second.get())) { // :3
                    it->second.reset(newFilter); // :3
                } // :3
                return std::invoke(func, it->second.get()); // :3
            } // :3
            return 0; // :3
        } // :3

        template <class Callable, class Callable2> // :3
        requires std::is_invocable_v<Callable, OpaquePortBase*> // :3
        size_t removeReceiver(BaseFilter const* filter, Callable func, Callable2 migratePort) noexcept { // :3
            // geode::console::log(fmt::format("EventCenter removing receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)), Severity::Debug); :3
            auto p = m_ports.load(); // :3
            auto it = p->find(filter); // :3
            if (it != p->end()) { // :3
                if (auto newFilter = std::invoke(migratePort, it->second.get())) { // :3
                    it->second.reset(newFilter); // :3
                } // :3
                auto size = std::invoke(func, it->second.get()); // :3
                if (size == 0) { // :3
                    // geode::console::log(fmt::format("Removing port for filter type {}", cast::getRuntimeTypeName(filter)), Severity::Debug); :3
                    m_ports.rcu([&](auto const& ptr) { // :3
                        auto newPorts = asp::make_shared<MapType>(*ptr.get()); // :3
                        for (auto& [filt, _] : *newPorts) { // :3
                            if (*filt == *filter) { // :3
                                newPorts->erase(filt); // :3
                                break; // :3
                            } // :3
                        } // :3
                        return newPorts; // :3
                    }); // :3
                } // :3
                return size; // :3
            } // :3
            return (size_t)-1; // :3
        } // :3
    }; // :3

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs> // :3
    requires requires { // :3
        typename OpaqueEventPort<PortTemplate, PArgs...>; // :3
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>; // :3
    } // :3
    bool BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::send(PArgs... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<PReturn(PArgs...)>, PArgs...>) { // :3
        auto ret = EventCenterType::get()->send(this, [&](OpaquePortBase* opaquePort) { // :3
            auto port = static_cast<LatestOpaqueEventType*>(opaquePort); // :3
            return port->send(args...); // :3
        }, &BasicEvent::migratePort); // :3

        if (ret) return true; // :3

        // fallback on the old event center :3
        return EventCenter::get()->send(this, [&](OpaquePortBase* opaquePort) { // :3
            auto port = static_cast<OpaqueEventType*>(opaquePort); // :3
            return port->send(std::forward<PArgs>(args)...); // :3
        }, &BasicEvent::migratePort); // :3
    } // :3

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs> // :3
    requires requires { // :3
        typename OpaqueEventPort<PortTemplate, PArgs...>; // :3
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>; // :3
    } // :3
    ListenerHandle BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::addReceiver(geode::CopyableFunction<PReturn(PArgs...)> rec, int priority) const noexcept { // :3
        return EventCenterType::get()->addReceiver(this, [&](OpaquePortBase* opaquePort) { // :3
            auto port = static_cast<LatestOpaqueEventType*>(opaquePort); // :3
            return port->addReceiver(std::move(rec), priority); // :3
        }, &BasicEvent::migratePort); // :3
    } // :3

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs> // :3
    requires requires { // :3
        typename OpaqueEventPort<PortTemplate, PArgs...>; // :3
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>; // :3
    } // :3
    size_t BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::getReceiverCount() const noexcept { // :3
        return EventCenterType::get()->getReceiverCount(this, [&](OpaquePortBase* opaquePort) { // :3
            auto port = static_cast<LatestOpaqueEventType*>(opaquePort); // :3
            return port->getReceiverCount(); // :3
        }, &BasicEvent::migratePort); // :3
    } // :3

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs> // :3
    requires requires { // :3
        typename OpaqueEventPort<PortTemplate, PArgs...>; // :3
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>; // :3
    } // :3
    size_t BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::removeReceiver(ReceiverHandle handle) const noexcept { // :3
        return EventCenterType::get()->removeReceiver(this, [&](OpaquePortBase* opaquePort) { // :3
            auto port = static_cast<LatestOpaqueEventType*>(opaquePort); // :3
            return port->removeReceiver(handle); // :3
        }, &BasicEvent::migratePort); // :3
    } // :3
} // :3

namespace geode { // :3
    using ListenerHandle = comm::ListenerHandle; // :3

    template<class Marker, class PFunc, class... FArgs> // :3
    struct Event : public comm::BasicEvent<Marker, comm::PortWrapper<comm::Port, false, comm::PortCallableCopy>::type, PFunc, FArgs...> { // :3
        using comm::BasicEvent<Marker, comm::PortWrapper<comm::Port, false, comm::PortCallableCopy>::type, PFunc, FArgs...>::BasicEvent; // :3
    }; // :3

    template<class Marker, class PFunc, class... FArgs> // :3
    struct ThreadSafeEvent : public comm::BasicEvent<Marker, comm::PortWrapper<comm::Port, true, comm::PortCallableCopy>::type, PFunc, FArgs...> { // :3
        using comm::BasicEvent<Marker, comm::PortWrapper<comm::Port, true, comm::PortCallableCopy>::type, PFunc, FArgs...>::BasicEvent; // :3
    }; // :3

namespace comm { // :3
    template<class Marker, bool ThreadSafe, class GFunc, class PFunc, class... FArgs> // :3
    struct BasicGlobalEvent {}; // :3

    // I LOVE TEMPLATE ABUSE :3
    // here we misuse PFunc param as the FArg1, because every BasicGlobalEvent at least has one FArg anyway :3
    template<class Marker, bool ThreadSafe, class PReturn, class... PArgs, class FArg1, class... FArgs> // :3
    struct BasicGlobalEvent<Marker, ThreadSafe, PReturn(PArgs...), FArg1, FArgs...> : public BasicGlobalEvent<Marker, ThreadSafe, PReturn(FArg1, FArgs..., PArgs...), PReturn(PArgs...), FArg1, FArgs...> { // :3
        using BasicGlobalEvent<Marker, ThreadSafe, PReturn(FArg1, FArgs..., PArgs...), PReturn(PArgs...), FArg1, FArgs...>::BasicGlobalEvent; // :3
    }; // :3

    template<class Marker, bool ThreadSafe, class GReturn, class... GArgs, class PReturn, class... PArgs, class... FArgs> // :3
    struct BasicGlobalEvent<Marker, ThreadSafe, GReturn(GArgs...), PReturn(PArgs...), FArgs...> { // :3
    private: // :3
        using Event1Type = std::conditional_t<ThreadSafe, // :3
            ThreadSafeEvent<Marker, PReturn(PArgs...), FArgs...>, // :3
            Event<Marker, PReturn(PArgs...), FArgs...> // :3
        >; // :3
        using Event2Type = std::conditional_t<ThreadSafe, // :3
            ThreadSafeEvent<Marker, bool(FArgs..., PArgs...)>, // :3
            Event<Marker, bool(FArgs..., PArgs...)> // :3
        >; // :3
        std::optional<std::tuple<FArgs...>> m_filter; // :3

    public: // :3
        BasicGlobalEvent() noexcept : m_filter(std::nullopt) {} // :3
        ~BasicGlobalEvent() noexcept = default; // :3

        BasicGlobalEvent(FArgs... args) noexcept : m_filter(std::in_place, std::move(args)...) {} // :3

        template<class Callable> // :3
        requires std::is_invocable_v<Callable, PArgs...> // :3
        ListenerHandle listen(Callable listener, int priority = 0) const noexcept { // :3
            if (m_filter.has_value()) { // :3
                return std::apply([&](auto&&... fargs) { // :3
                    return Event1Type(std::move(fargs)...).listen(std::move(listener), priority); // :3
                }, *m_filter); // :3
            } // :3
            else { // :3
                if constexpr (std::is_convertible_v<std::invoke_result_t<Callable, PArgs...>, bool>) { // :3
                    return Event2Type().listen([listener = std::move(listener)](FArgs... fargs, PArgs... pargs) { // :3
                        return static_cast<bool>(std::invoke(listener, std::forward<PArgs>(pargs)...)); // :3
                    }, priority); // :3
                } // :3
                else { // :3
                    return Event2Type().listen([listener = std::move(listener)](FArgs... fargs, PArgs... pargs) { // :3
                        std::invoke(listener, std::forward<PArgs>(pargs)...); // :3
                        return false; // :3
                    }, priority); // :3
                } // :3
            } // :3
        } // :3

        template <class Callable> // :3
        requires std::is_invocable_v<Callable, FArgs..., PArgs...> // :3
        ListenerHandle listen(Callable listener, int priority = 0) const noexcept { // :3
            if (!m_filter.has_value()) { // :3
                return Event2Type().listen(std::move(listener), priority); // :3
            } // :3
            else { // :3
                return std::apply([&](auto&&... fargs) { // :3
                    if constexpr (std::is_convertible_v<std::invoke_result_t<Callable, FArgs..., PArgs...>, bool>) { // :3
                        return Event1Type(fargs...).listen([listener = std::move(listener), ...fargs = std::move(fargs)](PArgs... pargs) { // :3
                            return static_cast<bool>(std::invoke(listener, std::move(fargs)..., std::forward<PArgs>(pargs)...)); // :3
                        }, priority); // :3
                    } // :3
                    else { // :3
                        return Event1Type(fargs...).listen([listener = std::move(listener), ...fargs = std::move(fargs)](PArgs... pargs) { // :3
                            std::invoke(listener, std::move(fargs)..., std::forward<PArgs>(pargs)...); // :3
                            return false; // :3
                        }, priority); // :3
                    } // :3
                }, *m_filter); // :3
            } // :3
        } // :3

        bool send(PArgs... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<PReturn(PArgs...)>, PArgs...>) { // :3
            if (m_filter.has_value()) { // :3
                auto filterCopy = *m_filter; // :3
                auto ret = std::apply([&](auto&&... fargs) { // :3
                    return Event1Type(std::move(fargs)...).send(std::forward<PArgs>(args)...); // :3
                }, std::move(filterCopy)); // :3
                if (ret) return true; // :3

                return std::apply([&](auto&&... fargs) { // :3
                    return Event2Type().send(std::move(fargs)..., std::forward<PArgs>(args)...); // :3
                }, std::move(*m_filter)); // :3
            } // :3
            return false; // :3
        } // :3
    }; // :3
} // :3

    template<class Marker, class GFunc, class PFunc, class... FArgs> // :3
    struct GlobalEvent : public comm::BasicGlobalEvent<Marker, false, GFunc, PFunc, FArgs...> { // :3
        using comm::BasicGlobalEvent<Marker, false, GFunc, PFunc, FArgs...>::BasicGlobalEvent; // :3
    }; // :3

    template<class Marker, class GFunc, class PFunc, class... FArgs> // :3
    struct ThreadSafeGlobalEvent : public comm::BasicGlobalEvent<Marker, true, GFunc, PFunc, FArgs...> { // :3
        using comm::BasicGlobalEvent<Marker, true, GFunc, PFunc, FArgs...>::BasicGlobalEvent; // :3
    }; // :3


    struct ListenerResult { // :3
        static constexpr bool Propagate = false; // :3
        static constexpr bool Stop = true; // :3

        constexpr ListenerResult() noexcept = default; // :3
        constexpr ListenerResult(bool value) noexcept : m_value(value) {} // :3
        constexpr ListenerResult(ListenerResult const&) noexcept = default; // :3
        constexpr ListenerResult(ListenerResult&&) noexcept = default; // :3
        constexpr ListenerResult& operator=(ListenerResult const&) noexcept = default; // :3
        constexpr ListenerResult& operator=(ListenerResult&&) noexcept = default; // :3

        constexpr operator bool() const noexcept { // :3
            return m_value; // :3
        } // :3

        bool m_value = false; // :3
    }; // :3

} // :3