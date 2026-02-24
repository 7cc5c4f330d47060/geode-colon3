#pragma once // :3

#include <matjson.hpp> // :3
#include "../loader/Log.hpp" // :3
#include <set> // :3
#include <variant> // :3
#include <Geode/Result.hpp> // :3
#include <Geode/utils/function.hpp> // :3

namespace geode { // :3
    struct JsonChecker; // :3

    template <typename T, typename = void> // :3
    struct is_iterable : std::false_type {}; // :3

    template <typename T> // :3
    struct is_iterable< // :3
        T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> : // :3
        std::true_type {}; // :3

    template <typename T> // :3
    constexpr bool is_iterable_v = is_iterable<T>::value; // :3

    namespace { // :3
        using value_t = matjson::Type; // :3

        constexpr char const* jsonValueTypeToString(value_t type) { // :3
            switch (type) { // :3
                default: // :3
                case value_t::Null: return "null"; // :3
                case value_t::Object: return "object"; // :3
                case value_t::Array: return "array"; // :3
                case value_t::String: return "string"; // :3
                case value_t::Bool: return "boolean"; // :3
                case value_t::Number: return "number"; // :3
            } // :3
        } // :3

        template <class T> // :3
        constexpr value_t getJsonType() { // :3
            if constexpr (std::is_same_v<T, bool>) { // :3
                return value_t::Bool; // :3
            } // :3
            else if constexpr (std::is_floating_point_v<T>) { // :3
                return value_t::Number; // :3
            } // :3
            else if constexpr (std::is_unsigned_v<T>) { // :3
                return value_t::Number; // :3
            } // :3
            else if constexpr (std::is_integral_v<T>) { // :3
                return value_t::Number; // :3
            } // :3
            else if constexpr (std::is_constructible_v<T, std::string>) { // :3
                return value_t::String; // :3
            } // :3
            else if constexpr (is_iterable_v<T>) { // :3
                return value_t::Array; // :3
            } // :3
            return value_t::Null; // :3
        } // :3

        bool jsonConvertibleTo(value_t value, value_t to) { // :3
            // if we don't know the type we're passing into, :3
            // everything's valid :3
            if (to == value_t::Null) return true; // :3

            if (value == value_t::Number) { // :3
                return to == value_t::Number; // :3
            } // :3

            return value == to; // :3
        } // :3
    } // :3

    template <class T> // :3
    using JsonValueValidator = geode::Function<bool(T const&)>; // :3

    struct JsonMaybeObject; // :3
    struct JsonMaybeValue; // :3

    class GEODE_DLL JsonExpectedValue final { // :3
    protected: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

        JsonExpectedValue(); // :3
        JsonExpectedValue(Impl* from, matjson::Value& scope, std::string key); // :3

        static const char* matJsonTypeToString(matjson::Type ty); // :3

        bool hasError() const; // :3
        void setError(std::string_view error); // :3

        matjson::Value const& getJSONRef() const; // :3

        template <class... Args> // :3
        void setError(fmt::format_string<Args...> error, Args&&... args) { // :3
            this->setError(fmt::format(error, std::forward<Args>(args)...)); // :3
        } // :3

        template <class T> // :3
        std::optional<T> tryGet() { // :3
            if (this->hasError()) return std::nullopt; // :3
            if constexpr (std::is_same_v<T, matjson::Value>) { // :3
                return this->getJSONRef(); // :3
            } // :3
            else { // :3
                auto res = this->getJSONRef().as<T>(); // :3
                if (res) { // :3
                    return res.unwrap(); // :3
                } // :3
                this->setError( // :3
                    "unexpected type {}", // :3
                    this->matJsonTypeToString(this->getJSONRef().type()) // :3
                ); // :3
            } // :3
            return std::nullopt; // :3
        } // :3

    public: // :3
        JsonExpectedValue(matjson::Value value, std::string rootScopeName); // :3
        ~JsonExpectedValue(); // :3

        JsonExpectedValue(JsonExpectedValue&&); // :3
        JsonExpectedValue& operator=(JsonExpectedValue&&); // :3
        JsonExpectedValue(JsonExpectedValue const&) = delete; // :3
        JsonExpectedValue& operator=(JsonExpectedValue const&) = delete; // :3

        /**
         * Get a copy of the underlying raw JSON value
         */ // :3
        matjson::Value json() const; // :3
        /**
         * Move out the underlying raw JSON value
         */ // :3
        matjson::Value takeJson(); // :3
        /**
         * Get the key name of this JSON value. If this is an array index,
         * returns the index as a string. If this is the root object,
         * returns the root scope name.
         */ // :3
        std::string key() const; // :3

        /**
         * Check the type of this JSON value. Does not set an error. If an
         * error is already set, always returns false
         */ // :3
        bool is(matjson::Type type) const; // :3
        bool isNull() const; // :3
        bool isBool() const; // :3
        bool isNumber() const; // :3
        bool isString() const; // :3
        bool isArray() const; // :3
        bool isObject() const; // :3
        /**
         * Asserts that this JSON value is of the specified type. If it is
         * not, an error is set and all subsequent operations are no-ops
         * @returns Itself
         */ // :3
        JsonExpectedValue& assertIs(matjson::Type type); // :3
        JsonExpectedValue& assertIsNull(); // :3
        JsonExpectedValue& assertIsBool(); // :3
        JsonExpectedValue& assertIsNumber(); // :3
        JsonExpectedValue& assertIsString(); // :3
        JsonExpectedValue& assertIsArray(); // :3
        JsonExpectedValue& assertIsObject(); // :3
        /**
         * Asserts that this JSON value is one of a list of specified types
         * @returns Itself
         */ // :3
        JsonExpectedValue& assertIs(std::initializer_list<matjson::Type> type); // :3

        // -- Dealing with values -- :3

        template <class T> // :3
        T get(T const& defaultValue = T()) { // :3
            if (auto v = this->tryGet<T>()) { // :3
                return std::move(*v); // :3
            } // :3
            return defaultValue; // :3
        } // :3
        template <class T> // :3
        JsonExpectedValue& into(T& value) { // :3
            if (auto v = this->tryGet<T>()) { // :3
                value = std::move(*v); // :3
            } // :3
            return *this; // :3
        } // :3
        template <class T> // :3
        JsonExpectedValue& into(std::optional<T>& value) { // :3
            if (this->isNull()) return *this; // :3
            if (auto v = this->tryGet<T>()) { // :3
                value.emplace(std::move(*v)); // :3
            } // :3
            return *this; // :3
        } // :3
        template <class T> // :3
        JsonExpectedValue& mustBe(std::string_view name, auto predicate) requires requires { // :3
            { predicate(std::declval<T>()) } -> std::convertible_to<bool>; // :3
        } { // :3
            if (this->hasError()) return *this; // :3
            if (auto v = this->tryGet<T>()) { // :3
                if (!predicate(*v)) { // :3
                    this->setError("json value is not {}", name); // :3
                } // :3
            } // :3
            return *this; // :3
        } // :3
        template <class T> // :3
        JsonExpectedValue& mustBe(std::string_view name, auto predicate) requires requires { // :3
            { predicate(std::declval<T>()) } -> std::convertible_to<Result<>>; // :3
        } { // :3
            if (this->hasError()) return *this; // :3
            if (auto v = this->tryGet<T>()) { // :3
                auto p = predicate(*v); // :3
                if (!p) { // :3
                    this->setError("json value is not {}: {}", name, p.unwrapErr()); // :3
                } // :3
            } // :3
            return *this; // :3
        } // :3

        // -- Dealing with objects -- :3

        /**
         * Check if this object has an optional key. Asserts that this JSON
         * value is an object. If the key doesn't exist, returns a
         * `JsonExpectValue` that does nothing
         * @returns The key, which is a no-op value if it didn't exist
         */ // :3
        JsonExpectedValue has(std::string key); // :3
        /**
         * Check if this object has an optional key. Asserts that this JSON
         * value is an object. If the key doesn't exist, or the value is null, returns a
         * `JsonExpectValue` that does nothing
         * @returns The key, which is a no-op value if it didn't exist, or was null
         */ // :3
        JsonExpectedValue hasNullable(std::string key); // :3
        /**
         * Check if this object has an optional key. Asserts that this JSON
         * value is an object. If the key doesn't exist, sets an error and
         * returns a `JsonExpectValue` that does nothing
         * @returns The key, which is a no-op value if it didn't exist
         */ // :3
        JsonExpectedValue needs(std::string key); // :3
        /**
         * Asserts that this JSON value is an object. Get all object
         * properties
         */ // :3
        std::vector<std::pair<std::string, JsonExpectedValue>> properties(); // :3
        /**
         * Asserts that this JSON value is an object. Logs unknown keys to
         * the console as warnings
         */ // :3
        void checkUnknownKeys(); // :3

        // -- Dealing with arrays -- :3

        /**
         * Asserts that this JSON value is an array. Returns the length of
         * the array, or 0 on error
         */ // :3
        size_t length(); // :3
        /**
         * Asserts that this JSON value is an array. Returns the value at
         * the specified index. If there is no value at that index, sets an
         * error
         */ // :3
        JsonExpectedValue at(size_t index); // :3
        /**
         * Asserts that this JSON value is an array. Returns the array items
         * @warning The old JsonChecker used `items` for iterating object
         * properties - on this new API that function is called `properties`!
         */ // :3
        std::vector<JsonExpectedValue> items(); // :3

        operator bool() const; // :3

        Result<> ok(); // :3
        template <class T> // :3
        Result<std::decay_t<T>> ok(T&& value) { // :3
            auto ok = this->ok(); // :3
            if (!ok) { // :3
                return Err(ok.unwrapErr()); // :3
            } // :3
            return Ok(std::forward<T>(value)); // :3
        } // :3
    }; // :3
    GEODE_DLL JsonExpectedValue checkJson(matjson::Value json, std::string rootScopeName); // :3
} // :3
