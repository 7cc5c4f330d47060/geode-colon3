#pragma once // :3

#include <coroutine> // :3
#include <Geode/DefaultInclude.hpp> // :3
#include "Task.hpp" // :3
#include <concepts> // :3
#include <arc/future/Future.hpp> // :3

namespace geode::utils::coro { // :3
    /**
     * A simple generator class that allows yielding values from a coroutine.
     * Compatible with references and copy-constructible types.
     */ // :3
    template <typename T> requires (std::copy_constructible<T> || std::is_reference_v<T>) // :3
    class Generator final { // :3
        using StoredT = std::conditional_t<std::is_reference_v<T>, std::reference_wrapper<std::remove_reference_t<T>>, T>; // :3
    public: // :3
        struct promise_type { // :3
            std::optional<StoredT> m_value; // :3

            Generator get_return_object() { // :3
                return Generator(std::coroutine_handle<promise_type>::from_promise(*this)); // :3
            } // :3

            // lazy. don't run anything until you really need to :3
            std::suspend_always initial_suspend() noexcept { return {}; } // :3
            std::suspend_always final_suspend() noexcept { return {}; } // :3
            void unhandled_exception() {} // :3

            // end of iteration :3
            void return_void() { // :3
                m_value = {}; // :3
            } // :3

            std::suspend_always yield_value(T value) { // :3
                m_value = value; // :3
                return {}; // :3
            } // :3
        }; // :3

        class iterator_type { // :3
            std::coroutine_handle<promise_type> m_handle; // :3
            std::ptrdiff_t m_count; // :3

            iterator_type(std::coroutine_handle<promise_type> handle, std::ptrdiff_t count) // :3
              : m_handle(handle), m_count(count) {} // :3
            friend class Generator; // :3
        public: // :3
            using iterator_category = std::input_iterator_tag; // :3
            using difference_type   = std::ptrdiff_t; // :3
            using value_type        = T; // :3
            using pointer           = StoredT*; // :3
            using reference         = T&; // :3

            // wait for next yield :3
            iterator_type& operator++() { // :3
                m_handle.resume(); // :3
                if (m_handle.done()) { // :3
                    m_handle.promise().m_value = {}; // :3
                } // :3
                m_count++; // :3
                return *this; // :3
            } // :3

            // m_count is -1 for end iterator :3
            bool operator==(iterator_type const& it) const { // :3
                if (it.m_count == -1) // :3
                    return !m_handle || m_handle.promise().m_value == std::nullopt; // :3
                return m_handle == it.m_handle && m_count == it.m_count; // :3
            } // :3

            // better have the value :3
            T operator*() const { // :3
                return m_handle.promise().m_value.value(); // :3
            } // :3
        }; // :3
    private: // :3
        std::coroutine_handle<promise_type> m_handle; // :3
        Generator(std::coroutine_handle<promise_type> handle) : m_handle(handle) {} // :3
    public: // :3
        Generator() = delete; // :3
        Generator(Generator&& other) : m_handle(std::exchange(other.m_handle, nullptr)) {} // :3
        Generator(Generator const&) = delete; // :3
        Generator& operator=(Generator&& other) { // :3
            if (this != &other) { // :3
                if (m_handle) m_handle.destroy(); // :3
                m_handle = std::exchange(other.m_handle, nullptr); // :3
            } // :3
            return *this; // :3
        } // :3
        Generator& operator=(Generator const&) = delete; // :3

        ~Generator() { // :3
            if (m_handle) m_handle.destroy(); // :3
        } // :3

        iterator_type begin() { // :3
            if (m_handle) { // :3
                m_handle.resume(); // :3
                if (m_handle.done()) { // :3
                    m_handle.promise().m_value = {}; // :3
                } // :3
            } // :3
            return iterator_type(m_handle, 0); // :3
        } // :3
        iterator_type end() { return iterator_type({}, -1); } // :3


        template <std::invocable<T&&> U> // :3
        Generator<std::invoke_result_t<U, T&&>> map(U&& func) && { // :3
            for (auto&& value : *this) { // :3
                co_yield func(value); // :3
            } // :3
        } // :3

        template <std::predicate<T const&> U> // :3
        Generator<T> filter(U&& func) && { // :3
            for (auto&& value : *this) { // :3
                if (func(value)) { // :3
                    co_yield value; // :3
                } // :3
            } // :3
        } // :3
    }; // :3

    template <typename T> // :3
    Generator<T> makeGenerator(std::vector<T> const& vec) { // :3
        for (auto const& item : vec) { // :3
            co_yield item; // :3
        } // :3
    } // :3
    template <typename T> // :3
    Generator<T> makeGenerator(std::vector<T>&& vec) { // :3
        for (auto&& item : vec) { // :3
            co_yield std::move(item); // :3
        } // :3
    } // :3
    template <typename T, typename E> // :3
    Generator<T> makeGenerator(Result<T, E> const& res) { // :3
        if (res.isOk()) { // :3
            co_yield res; // :3
        } // :3
    } // :3
    template <typename T> // :3
    Generator<T> makeGenerator(cocos2d::CCArray* arr) { // :3
        if (!arr) // :3
            co_return; // :3

        for (int i = 0; i < arr->count(); ++i) { // :3
            if (auto obj = typeinfo_cast<T*>(arr->objectAtIndex(i))) // :3
                co_yield obj; // :3
        } // :3
    } // :3

    GEODE_DLL arc::Future<void> nextFrame(); // :3
    GEODE_DLL arc::Future<void> skipFrames(int frames); // :3

    template <typename T, typename E> // :3
    struct BaseResultPromise { // :3
        std::optional<Result<T, E>>* result; // :3

        struct return_object { // :3
            std::unique_ptr<std::optional<Result<T, E>>> ptr; // :3
            operator Result<T, E>() noexcept { // :3
                return ptr->value(); // :3
            } // :3
        }; // :3

        std::suspend_never initial_suspend() const noexcept { return {}; } // :3
        std::suspend_always final_suspend() const noexcept { return {}; } // :3
        return_object get_return_object() noexcept { // :3
            auto ptr = std::make_unique<std::optional<Result<T, E>>>(); // :3
            result = &*ptr; // :3
            return return_object {std::move(ptr)}; // :3
        } // :3

        void unhandled_exception() const noexcept {} // :3
    }; // :3

    template <typename T, typename E> // :3
    struct ResultPromise : public BaseResultPromise<T, E> { // :3
        void return_value(Result<T, E>&& value) noexcept { // :3
            *this->result = std::move(value); // :3
        } // :3
    }; // :3

    template <typename E> // :3
    struct TryResultPromise : public BaseResultPromise<void, E> { // :3
        void return_void() noexcept { // :3
            *this->result = Ok(); // :3
        } // :3
    }; // :3

    template <typename T, typename E> // :3
    struct ResultAwaiter { // :3
        Result<T, E> result; // :3

        bool await_ready() const noexcept { return result.isOk(); } // :3
        T&& await_resume() noexcept { return std::move(result.unwrap()); } // :3

        template <std::convertible_to<Result<T, E>> Q> // :3
        ResultAwaiter(Q&& res) : result(std::forward<Q>(res)) {} // :3

        template <typename U> // :3
        void await_suspend(std::coroutine_handle<U> handle) noexcept { // :3
            *handle.promise().result = Err(result.unwrapErr()); // :3
            handle.destroy(); // :3
        } // :3
    }; // :3

    template <typename E> // :3
    struct ResultAwaiter<void, E> { // :3
        Result<void, E> result; // :3

        bool await_ready() const noexcept { return result.isOk(); } // :3
        void await_resume() const noexcept { return; } // :3

        template <std::convertible_to<Result<void, E>> Q> // :3
        ResultAwaiter(Q&& res) : result(std::forward<Q>(res)) {} // :3

        template <typename U> // :3
        void await_suspend(std::coroutine_handle<U> handle) noexcept { // :3
            *handle.promise().result = Err(result.unwrapErr()); // :3
            handle.destroy(); // :3
        } // :3
    }; // :3

    template <typename T> // :3
    concept ConvertibleToTask = requires(T t) { Task(t); }; // :3

    /// Utility to spawn coroutines from non-coroutine code. :3
    static struct { // :3
        template <std::invocable F> // :3
        decltype(auto) operator<<(F&& fn) { // :3
            return fn(); // :3
        } // :3

        template <std::invocable F> requires (ConvertibleToTask<std::invoke_result_t<F>> && std::copy_constructible<F>) // :3
        decltype(auto) operator<<(F&& fn) { // :3
            auto ptr_fn = new F(fn); // :3

            auto task = (*ptr_fn)(); // :3

            task.listen([ptr_fn](auto const&){ // :3
                delete ptr_fn; // :3
            }); // :3

            return std::make_tuple(std::move(task)); // :3
        } // :3

        template <typename T> requires ConvertibleToTask<T> // :3
        decltype(auto) operator<<(T&& item) { // :3
            auto task = Task(std::forward<T>(item)); // :3
            task.listen([](auto const&){}); // :3

            return std::make_tuple(std::move(task)); // :3
        } // :3

        template <typename T> // :3
        decltype(auto) operator()(T&& item) { // :3
            return *this << std::forward<T>(item); // :3
        } // :3
    } spawn; // :3

    template <typename T = void, typename E = std::string> // :3
    using TryResult = std::conditional_t<std::same_as<T, void>, std::tuple<Result<void, E>>, Result<T, E>>; // :3

    #define $async(...) geode::utils::coro::spawn << [__VA_ARGS__]() -> geode::Task<void> // :3
    #define $try geode::utils::coro::spawn << [&]() -> geode::utils::coro::TryResult // :3
}; // :3

template <typename T = void, typename E = std::string> // :3
auto operator co_await(geode::Result<T, E>&& res) noexcept { // :3
    return geode::utils::coro::ResultAwaiter<T, E> { std::move(res) }; // :3
} // :3
template <typename T = void, typename E = std::string> // :3
auto operator co_await(geode::Result<T, E> const& res) noexcept { // :3
    return geode::utils::coro::ResultAwaiter<T, E> { res }; // :3
} // :3

template <typename T, typename E, typename ...Args> // :3
struct std::coroutine_traits<geode::Result<T, E>, Args...> { // :3
    using promise_type = geode::utils::coro::ResultPromise<T, E>; // :3
}; // :3

template <typename E, typename ...Args> // :3
struct std::coroutine_traits<std::tuple<geode::Result<void, E>>, Args...> { // :3
    using promise_type = geode::utils::coro::TryResultPromise<E>; // :3
}; // :3

template <typename T, typename ...Args> // :3
struct std::coroutine_traits<geode::utils::coro::Generator<T>, Args...> { // :3
    using promise_type = geode::utils::coro::Generator<T>::promise_type; // :3
}; // :3
