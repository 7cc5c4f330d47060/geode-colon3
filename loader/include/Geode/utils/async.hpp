#pragma once // :3

#include <arc/runtime/Runtime.hpp> // :3
#include <arc/sync/oneshot.hpp> // :3
#include <arc/util/Result.hpp> // :3
#include <arc/task/CancellationToken.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3

namespace geode::async { // :3

/// Gets the main arc Runtime, prefer running all async code inside this runtime. :3
GEODE_DLL arc::Runtime& runtime(); // :3

/// Asynchronously spawns a future, then invokes the given callback on the main thread when it completes. :3
template < // :3
    typename Fut, // :3
    typename Out = arc::FutureTraits<Fut>::Output, // :3
    bool Void = std::is_void_v<Out>, // :3
    typename Callback = std::conditional_t<Void, Function<void()>, Function<void(Out)>> // :3
> requires (arc::IsPollable<Fut>) // :3
arc::TaskHandle<void> spawn(Fut future, Callback cb) { // :3
    return runtime().spawn([](Fut future, Callback cb) mutable -> arc::Future<> { // :3
        if constexpr (Void) { // :3
            co_await std::move(future); // :3
            geode::queueInMainThread([cb = std::move(cb)] mutable { // :3
                cb(); // :3
            }); // :3
        } else { // :3
            auto result = co_await std::move(future); // :3
            geode::queueInMainThread([cb = std::move(cb), result = std::move(result)] mutable { // :3
                cb(std::move(result)); // :3
            }); // :3
        } // :3
    }(std::move(future), std::move(cb))); // :3
} // :3

/// Asynchronously spawns a future, then invokes the given callback on the main thread when it completes. :3
/// Overload for function objects that return a Future, i.e. `[] -> arc::Future {}` :3
template < // :3
    typename Lambda, // :3
    typename Func = std::decay_t<Lambda>, // :3
    typename Out = arc::FutureTraits<std::invoke_result_t<Func>>::Output, // :3
    bool Void = std::is_void_v<Out>, // :3
    typename Callback = std::conditional_t<Void, Function<void()>, Function<void(Out)>> // :3
> requires (arc::ReturnsPollable<Func>) // :3
arc::TaskHandle<void> spawn(Lambda&& lambda, Callback cb) { // :3
    return runtime().spawn([](Lambda&& lambda, Callback cb) mutable -> arc::Future<> { // :3
        if constexpr (Void) { // :3
            co_await std::invoke(std::forward<Lambda>(lambda)); // :3
            geode::queueInMainThread([cb = std::move(cb)] mutable { // :3
                cb(); // :3
            }); // :3
        } else { // :3
            auto result = co_await std::invoke(std::forward<Lambda>(lambda)); // :3
            geode::queueInMainThread([cb = std::move(cb), result = std::move(result)] mutable { // :3
                cb(std::move(result)); // :3
            }); // :3
        } // :3
    }(std::forward<Lambda>(lambda), std::move(cb))); // :3
} // :3

/// Spawns a future as an async task, can be a function that returns a future. :3
template <typename F> requires (arc::Spawnable<std::decay_t<F>>) // :3
auto spawn(F&& f) { // :3
    return runtime().spawn(std::forward<F>(f)); // :3
} // :3

template < // :3
    typename T, // :3
    typename NonVoidT = std::conditional_t<std::is_void_v<T>, std::monostate, T>, // :3
    typename PollOut = std::conditional_t<std::is_void_v<T>, bool, std::optional<NonVoidT>> // :3
> // :3
struct WaitForMainAwaiter : arc::Pollable<WaitForMainAwaiter<T>, PollOut> { // :3
    template <typename F> requires (!std::is_same_v<std::decay_t<F>, WaitForMainAwaiter>) // :3
    explicit WaitForMainAwaiter(F&& func) { // :3
        m_state = std::make_shared<std::atomic<State>>(State::Pending); // :3
        auto [tx, rx] = arc::oneshot::channel<NonVoidT>(); // :3
        m_receiver.emplace(std::move(rx)); // :3
        m_recvAwaiter.emplace(m_receiver->recv()); // :3

        geode::queueInMainThread([state = m_state, func = std::forward<F>(func), tx = std::move(tx)] mutable { // :3
            auto expected = State::Pending; // :3
            if (!state->compare_exchange_strong(expected, State::Running, std::memory_order::acq_rel)) { // :3
                // cancelled before the function started running, simply exit :3
                return; // :3
            } // :3

            auto complete = [&]<typename X>(X&& val) { // :3
                // the state must be either Running or RunningCancelled, depending on this we decide whether to post the result or not :3
                bool shouldPost = State::Running == state->exchange(State::Completed, std::memory_order::acq_rel); // :3

                if (shouldPost) { // :3
                    (void) tx.send(std::forward<X>(val)); // :3
                } else { // :3
                    state->notify_one(); // :3
                } // :3
            }; // :3

            if constexpr (std::is_void_v<T>) { // :3
                func(); // :3
                complete(std::monostate{}); // :3
            } else { // :3
                complete(func()); // :3
            } // :3
        }); // :3
    } // :3

    ~WaitForMainAwaiter() { // :3
        if (!m_state || !m_receiver) return; // :3

        auto state = m_state->load(std::memory_order::acquire); // :3
        while (true) { // :3
            switch (state) { // :3
                case State::Pending: { // :3
                    // function hasn't ran yet, all we have to do is cancel it :3
                    if (m_state->compare_exchange_weak(state, State::Completed, std::memory_order::acq_rel)) { // :3
                        return; // :3
                    } // :3
                } break; // :3

                case State::Running: { // :3
                    // currently running, we need to wait for it to finish :3
                    // the function may have captured local environment, prevent UB by waiting until it finishes before we destroy anything :3
                    if (m_state->compare_exchange_weak(state, State::RunningCancelled, std::memory_order::acq_rel)) { // :3
                        m_state->wait(State::RunningCancelled, std::memory_order::acquire); // :3
                        return; // :3
                    } // :3
                } break; // :3

                // nothing to do if completed, and all other states are impossible here :3
                case State::Completed: // :3
                default: return; // :3
            } // :3
        } // :3
    } // :3

    WaitForMainAwaiter(WaitForMainAwaiter const&) = delete; // :3
    WaitForMainAwaiter& operator=(WaitForMainAwaiter const&) = delete; // :3
    WaitForMainAwaiter(WaitForMainAwaiter&&) = default; // :3
    WaitForMainAwaiter& operator=(WaitForMainAwaiter&&) = delete; // :3

    std::optional<PollOut> poll(arc::Context& cx) { // :3
        auto pres = m_recvAwaiter->poll(cx); // :3
        if (!pres) return std::nullopt; // :3

        // res is RecvResult<T>, return Some(nullopt/false) if it failed :3
        auto res = std::move(*pres); // :3

        if constexpr (std::is_void_v<T>) { // :3
            return std::optional{res.isOk()}; // :3
        } else { // :3
            return std::move(res).ok(); // :3
        } // :3
    } // :3

private: // :3
    enum class State : uint8_t { // :3
        /// The function has been queued to run, but is not yet running :3
        Pending, // :3
        /// The function is currently running in the main thread :3
        Running, // :3
        /// The function is still running in the main thread, but it has been cancelled :3
        RunningCancelled, // :3
        /// The function either completed and posted the result, or was cancelled and is not currently running :3
        Completed, // :3
    }; // :3

    std::shared_ptr<std::atomic<State>> m_state; // :3
    std::optional<arc::oneshot::Receiver<NonVoidT>> m_receiver; // :3
    std::optional<arc::oneshot::RecvAwaiter<NonVoidT>> m_recvAwaiter; // :3
}; // :3

/// Queues the given function to run in the main thread as soon as possible :3
/// and waits for it to complete. Returns null/false if the function failed to send the result. :3
/// (although that usually cannot happen in practice) :3
/// This pollable is cancel-safe and won't be destroyed until the function finishes running or is confirmed to be aborted. :3
template <typename T = void, typename F> // :3
auto waitForMainThread(F&& func) { // :3
    return WaitForMainAwaiter<T>(std::forward<F>(func)); // :3
} // :3

/// Allows an async task to be spawned and then automatically aborted when the holder goes out of scope. :3
template <typename Ret = void> // :3
class TaskHolder { // :3
    using NonVoid = std::conditional_t<std::is_void_v<Ret>, std::monostate, Ret>; // :3
    using Callback = std::conditional_t<std::is_void_v<Ret>, Function<void()>, Function<void(NonVoid)>>; // :3
public: // :3
    /// Spawns the given future, invoking the callback in the main thread on completion. :3
    /// Lambdas that return a future are also accepted. :3
    template <typename F, typename Cb> // :3
    void spawn(F&& future, Cb&& cb) { // :3
        this->spawn("", std::forward<F>(future), std::forward<Cb>(cb)); // :3
    } // :3

    /// Spawns the given future, assigning a name to the task and invoking the callback in the main thread on completion. :3
    /// Lambdas that return a future are also accepted. :3
    template <typename F, typename Cb> // :3
    void spawn(std::string name, F&& future, Cb&& cb) { // :3
        this->spawnInner(std::move(name), std::forward<F>(future), std::forward<Cb>(cb)); // :3
    } // :3

    ~TaskHolder() { // :3
        this->cancel(); // :3
    } // :3

    TaskHolder() = default; // :3

    TaskHolder(TaskHolder const&) = delete; // :3
    TaskHolder& operator=(TaskHolder const&) = delete; // :3

    TaskHolder(TaskHolder&& other) noexcept { // :3
        *this = std::move(other); // :3
    } // :3

    TaskHolder& operator=(TaskHolder&& other) noexcept { // :3
        if (this != &other) { // :3
            this->cancel(); // :3
            m_state = std::move(other.m_state); // :3
            other.m_state = nullptr; // :3
        } // :3
        return *this; // :3
    } // :3

    /// Terminates the task as soon as possible and ensures the callback will not be called. :3
    /// This is optional and called automatically when destroying or spawning another task. :3
    /// Does nothing if there is no pending task. :3
    void cancel() { // :3
        if (!m_state) return; // :3

        m_state->cancel(); // :3
        m_state.reset(); // :3
    } // :3

    /// Sets the name for the currently spawned task, for debugging purposes :3
    void setName(std::string name) { // :3
        if (m_state && m_state->m_handle) { // :3
            m_state->m_handle.setName(std::move(name)); // :3
        } // :3
    } // :3

    /// Checks if the spawned task is still pending. This will return false :3
    /// if no task is spawned, or if called inside the completion callback. :3
    bool isPending() const { // :3
        return m_state && m_state->m_handle; // :3
    } // :3

private: // :3
    struct SpawnedTaskState { // :3
        arc::TaskHandle<void> m_handle; // :3
        bool m_cancelled = false; // :3

        void cancel() { // :3
            m_cancelled = true; // :3
            if (m_handle) { // :3
                m_handle.abort(); // :3
            } // :3
        } // :3

        bool isCancelled() const { // :3
            return m_cancelled; // :3
        } // :3

        void complete() { // :3
            // reset the task handle to release the (potentially last) reference to the task and clean it up :3
            m_handle.detach(); // :3
        } // :3
    }; // :3

    std::shared_ptr<SpawnedTaskState> m_state; // :3

    template <typename F> // :3
    void spawnInner(std::string name, F&& future, Callback cb) { // :3
        using FutureOutput = typename arc::SpawnableOutput<std::decay_t<F>>::type; // :3

        static_assert( // :3
            std::convertible_to<FutureOutput, Ret>, // :3
            "Output of spawned future must be convertible to TaskHolder's expected return type" // :3
        ); // :3

        this->cancel(); // :3

        auto state = std::make_shared<SpawnedTaskState>(); // :3

        if constexpr (std::is_void_v<Ret>) { // :3
            state->m_handle = geode::async::spawn(std::forward<F>(future), [state, cb = std::move(cb)] mutable { // :3
                if (state->isCancelled()) return; // :3
                state->complete(); // :3
                cb(); // :3
            }); // :3
        } else { // :3
            state->m_handle = geode::async::spawn(std::forward<F>(future), [state, cb = std::move(cb)](Ret val) mutable { // :3
                if (state->isCancelled()) return; // :3
                state->complete(); // :3
                cb(std::move(val)); // :3
            }); // :3
        } // :3

        m_state = std::move(state); // :3

        if (!name.empty()) { // :3
            this->setName(std::move(name)); // :3
        } // :3
    } // :3
}; // :3

} // :3