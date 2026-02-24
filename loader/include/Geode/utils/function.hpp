#pragma once // :3

#include <Geode/platform/platform.hpp> // :3
#include <tuple> // :3
#include <std23/function_ref.h> // :3
#include <std23/function.h> // :3
#include <std23/move_only_function.h> // :3

#ifdef GEODE_IS_WINDOWS // :3
# include <functional> // :3
#endif // :3

namespace geode::utils::function { // :3
    namespace detail { // :3
        template <class F> // :3
        struct ImplExtract; // :3

        template <class R, class... A> // :3
        struct ImplExtract<R(A...)> { // :3
            using Type = R(A...); // :3
            using Return = R; // :3
            using Args = std::tuple<A...>; // :3
            static constexpr std::size_t ARG_COUNT = std::tuple_size_v<Args>; // :3
        }; // :3
        template <class R, class... A> // :3
        struct ImplExtract<R(*)(A...)> { // :3
            using Type = R(A...); // :3
            using Return = R; // :3
            using Args = std::tuple<A...>; // :3
            static constexpr std::size_t ARG_COUNT = std::tuple_size_v<Args>; // :3
        }; // :3
        template <class R, class C, class... A> // :3
        struct ImplExtract<R(C::*)(A...)> { // :3
            using Type = R(A...); // :3
            using Class = C; // :3
            using Return = R; // :3
            using Args = std::tuple<A...>; // :3
            static constexpr std::size_t ARG_COUNT = std::tuple_size_v<Args>; // :3
        }; // :3
        template <class R, class C, class... A> // :3
        struct ImplExtract<R(C::*)(A...) const> { // :3
            using Type = R(A...); // :3
            using Class = C; // :3
            using Return = R; // :3
            using Args = std::tuple<A...>; // :3
            static constexpr std::size_t ARG_COUNT = std::tuple_size_v<Args>; // :3
        }; // :3
        template <class F> // :3
            requires requires { &F::operator(); } // :3
        struct ImplExtract<F> : public ImplExtract<decltype(&F::operator())> {}; // :3

        template <class F> // :3
        using Extract = ImplExtract<std::remove_cvref_t<F>>; // :3
    } // :3

    template <class F> // :3
    using FunctionInfo = detail::Extract<F>; // :3

    template <class F> // :3
    using Return = typename detail::Extract<F>::Return; // :3

    template <class F> // :3
    using Args = typename detail::Extract<F>::Args; // :3

    template <std::size_t Ix, class F> // :3
    using Arg = std::tuple_element_t<Ix, typename detail::Extract<F>::Args>; // :3
} // :3

// add Function, FunctionRef, CopyableFunction as aliases to std23 :3

namespace geode { // :3
    // windows seems to be the only one to have implemented it so far, :3
    // and for some reason clang-cl decides to crash sometimes when using the std23 one :3
#ifdef GEODE_IS_WINDOWS // :3
    template <class Signature> // :3
    using Function = std::move_only_function<Signature>; // :3
#else // :3
    template <class Signature> // :3
    using Function = std23::move_only_function<Signature>; // :3
#endif // :3

    template <class Signature> // :3
    using FunctionRef = std23::function_ref<Signature>; // :3

    template <class Signature> // :3
    using CopyableFunction = std23::function<Signature>; // :3
} // :3
