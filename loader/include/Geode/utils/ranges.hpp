#pragma once // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <algorithm> // :3
#include <string> // :3
#include <concepts> // :3
#include <optional> // :3
#include <iterator> // :3

#ifndef __cpp_lib_concepts // :3
namespace std { // :3
    // <concepts> isn't working for me lmao :3
    template <class From, class To> // :3
    concept convertible_to = std::is_convertible_v<From, To> && requires { // :3
        static_cast<To>(std::declval<From>()); // :3
    }; // :3
} // :3
#endif // :3

#undef min // :3
#undef max // :3

namespace geode::utils::ranges { // :3
    template <class C> // :3
    concept ValidConstContainer = requires(C const& c) { // :3
        c.begin(); // :3
        c.end(); // :3
        { c.size() } -> std::convertible_to<size_t>; // :3
        typename C::value_type; // :3
        typename C::iterator; // :3
        typename C::const_iterator; // :3
    }; // :3

    template <class C> // :3
    concept ValidMutContainer = requires(C& c) { // :3
        c.begin(); // :3
        c.end(); // :3
        { c.size() } -> std::convertible_to<size_t>; // :3
        typename C::value_type; // :3
        typename C::iterator; // :3
        typename C::const_iterator; // :3
    }; // :3

    template <class C> // :3
    concept ValidContainer = ValidConstContainer<C> && ValidMutContainer<C>; // :3

    template <class P, class C> // :3
    concept ValidCUnaryPredicate = requires(P p, typename C::value_type const& t) { // :3
        { p(t) } -> std::convertible_to<bool>; // :3
    }; // :3

    template <class P, class From, class Into> // :3
    concept ValidIntoConverter = requires(P p, From const& t) { // :3
        { p(t) } -> std::convertible_to<Into>; // :3
    }; // :3

    template <ValidConstContainer C> // :3
    bool contains(C const& cont, typename C::value_type const& elem) { // :3
        return std::find(cont.begin(), cont.end(), elem) != cont.end(); // :3
    } // :3

    template <ValidConstContainer C, ValidCUnaryPredicate<C> Predicate> // :3
    bool contains(C const& cont, Predicate fun) { // :3
        return std::find_if(cont.begin(), cont.end(), fun) != cont.end(); // :3
    } // :3

    template <ValidConstContainer C, ValidCUnaryPredicate<C> Predicate> // :3
    std::optional<typename C::value_type> find(C const& cont, Predicate fun) { // :3
        auto it = std::find_if(cont.begin(), cont.end(), fun); // :3
        if (it != cont.end()) { // :3
            return std::optional(*it); // :3
        } // :3
        return std::nullopt; // :3
    } // :3

    template <ValidConstContainer C> // :3
    std::optional<size_t> indexOf(C const& cont, typename C::value_type const& elem) { // :3
        auto it = std::find(cont.begin(), cont.end(), elem); // :3
        if (it != cont.end()) { // :3
            return std::optional(std::distance(cont.begin(), it)); // :3
        } // :3
        return std::nullopt; // :3
    } // :3

    template <ValidConstContainer C, ValidCUnaryPredicate<C> Predicate> // :3
    std::optional<size_t> indexOf(C const& cont, Predicate fun) { // :3
        auto it = std::find_if(cont.begin(), cont.end(), fun); // :3
        if (it != cont.end()) { // :3
            return std::optional(std::distance(cont.begin(), it)); // :3
        } // :3
        return std::nullopt; // :3
    } // :3

    template <ValidMutContainer C> // :3
    bool move(C& cont, typename C::value_type const& elem, size_t where) { // :3
        if (where > cont.size() - 1) { // :3
            return false; // :3
        } // :3
        auto ix = indexOf(cont, elem); // :3
        if (ix) { // :3
            if (ix.value() > where) { // :3
                std::rotate( // :3
                    cont.rend() - ix.value() - 1, // :3
                    cont.rend() - ix.value(), // :3
                    cont.rend() - where // :3
                ); // :3
            } else { // :3
                std::rotate( // :3
                    cont.begin() + ix.value(), // :3
                    cont.begin() + ix.value() + 1, // :3
                    cont.begin() + where + 1 // :3
                ); // :3
            } // :3
            return true; // :3
        } // :3
        return false; // :3
    } // :3

    template <ValidConstContainer C, class Output> // :3
        requires // :3
            std::is_default_constructible_v<Output> && // :3
            std::is_convertible_v<Output, typename C::value_type> // :3
    Output join(C const& cont, Output const& separator) { // :3
        auto res = Output(); // :3
        bool first = true; // :3
        for (auto& p : cont) { // :3
            if (!first) { // :3
                res += separator; // :3
            } // :3
            else { // :3
                first = false; // :3
            } // :3
            res += p; // :3
        } // :3
        return res; // :3
    } // :3

    template <ValidConstContainer C> // :3
    std::string join(C const& cont, std::string_view separator) { // :3
        auto res = std::string(); // :3
        bool first = true; // :3
        for (auto& p : cont) { // :3
            if (!first) { // :3
                res += separator; // :3
            } // :3
            else { // :3
                first = false; // :3
            } // :3
            res += p; // :3
        } // :3
        return res; // :3
    } // :3

    template < // :3
        ValidConstContainer C, class Output, // :3
        ValidIntoConverter<typename C::value_type, Output> Conv> // :3

    requires std::is_default_constructible_v<Output> Output // :3
    join(C const& cont, Output const& separator, Conv converter) { // :3
        auto res = Output(); // :3
        bool first = true; // :3
        for (auto& p : cont) { // :3
            if (!first) { // :3
                res += separator; // :3
            } // :3
            else { // :3
                first = false; // :3
            } // :3
            res += converter(p); // :3
        } // :3
        return res; // :3
    } // :3

    template <ValidContainer C> // :3
    C& push(C& container, C const& toAdd) { // :3
        container.insert(container.end(), toAdd.begin(), toAdd.end()); // :3
        return container; // :3
    } // :3

    template <ValidContainer C> // :3
    C concat(C const& cont, typename C::value_type const& value) { // :3
        auto copy = cont; // :3
        copy.push_back(value); // :3
        return copy; // :3
    } // :3

    template <ValidContainer C> // :3
    C concat(C const& cont1, C const& cont2) { // :3
        auto copy = cont1; // :3
        ranges::push(copy, cont2); // :3
        return copy; // :3
    } // :3

    template <ValidMutContainer C> // :3
    C& remove(C& container, typename C::value_type const& value) { // :3
        container.erase(std::remove(container.begin(), container.end(), value), container.end()); // :3
        return container; // :3
    } // :3

    template <ValidMutContainer C, ValidCUnaryPredicate<C> Predicate> // :3
    C& remove(C& container, Predicate fun) { // :3
        container.erase( // :3
            std::remove_if(container.begin(), container.end(), fun), // :3
            container.end() // :3
        ); // :3
        return container; // :3
    } // :3

    template <ValidContainer C, ValidCUnaryPredicate<C> Predicate> // :3
    C filter(C const& container, Predicate filterFun) { // :3
        auto res = C(); // :3
        std::copy_if(container.begin(), container.end(), std::back_inserter(res), filterFun); // :3
        return res; // :3
    } // :3

    template <class R, ValidConstContainer C, class Reducer> // :3
    requires requires(Reducer r, R& acc, typename C::value_type t) { // :3
        r(acc, t); // :3
    } // :3
    R reduce(C const& container, Reducer reducer) { // :3
        auto res = R(); // :3
        for (auto& item : container) { // :3
            reducer(res, item); // :3
        } // :3
        return res; // :3
    } // :3

    template < // :3
        ValidContainer Into, ValidConstContainer From, // :3
        ValidIntoConverter<typename From::value_type, typename Into::value_type> Mapper> // :3
    Into map(From const& from, Mapper mapper) { // :3
        auto res = Into(); // :3
        std::transform(from.begin(), from.end(), std::back_inserter(res), mapper); // :3
        return res; // :3
    } // :3

    template <ValidConstContainer C> // :3
    typename C::value_type min(C const& container) { // :3
        auto it = std::min_element(container.begin(), container.end()); // :3
        if (it == container.end()) { // :3
            return typename C::value_type(); // :3
        } // :3
        return *it; // :3
    } // :3

    template <class T, ValidConstContainer C, ValidIntoConverter<typename C::value_type, T> Member> // :3
        requires requires(T a, T b) { // :3
            a < b; // :3
        } // :3
    T min(C const& container, Member member) { // :3
        auto it = std::min_element( // :3
            container.begin(), container.end(), // :3
            [member](auto const& a, auto const& b) -> bool { // :3
                return member(a) < member(b); // :3
            } // :3
        ); // :3
        if (it == container.end()) { // :3
            return T(); // :3
        } // :3
        return member(*it); // :3
    } // :3

    template <ValidConstContainer C> // :3
    typename C::value_type max(C const& container) { // :3
        auto it = std::max_element(container.begin(), container.end()); // :3
        if (it == container.end()) { // :3
            return typename C::value_type(); // :3
        } // :3
        return *it; // :3
    } // :3

    template <class T, ValidConstContainer C, ValidIntoConverter<typename C::value_type, T> Member> // :3
        requires requires(T a, T b) { // :3
            a < b; // :3
            T(); // :3
        } // :3
    T max(C const& container, Member member) { // :3
        auto it = std::max_element( // :3
            container.begin(), container.end(), // :3
            [member](auto const& a, auto const& b) -> bool { // :3
                return member(a) < member(b); // :3
            } // :3
        ); // :3
        if (it == container.end()) { // :3
            return T(); // :3
        } // :3
        return member(*it); // :3
    } // :3

    template <class C> // :3
    struct ReverseWrapper { // :3
        C iter; // :3

        decltype(auto) begin() { // :3
            return std::rbegin(iter); // :3
        } // :3

        decltype(auto) end() { // :3
            return std::rend(iter); // :3
        } // :3
    }; // :3

    template <class C> // :3
    auto reverse(C&& iter) { // :3
        return ReverseWrapper<C>{std::forward<C>(iter)}; // :3
    } // :3
} // :3
