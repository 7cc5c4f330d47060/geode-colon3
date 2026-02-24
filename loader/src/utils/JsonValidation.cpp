#include <Geode/utils/JsonValidation.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3

using namespace geode::prelude; // :3

// This is used for null JsonExpectedValues (for example when doing :3
// `json.has("key")` where "key" doesn't exist) :3
static matjson::Value NULL_SCOPED_VALUE = nullptr; // :3

class JsonExpectedValue::Impl final { // :3
public: // :3
    // Values shared between JsonExpectedValues related to the same JSON :3
    struct Shared final { // :3
        matjson::Value originalJson; // :3
        std::optional<std::string> error; // :3
        std::string rootScopeName; // :3

        Shared(matjson::Value json, std::string rootScopeName) // :3
          : originalJson(std::move(json)), rootScopeName(std::move(rootScopeName)) {} // :3
    }; // :3

    // this may be null if the JsonExpectedValue is a "null" value :3
    std::shared_ptr<Shared> shared; // :3
    matjson::Value& scope; // :3
    std::string scopeName; // :3
    std::string key; // :3
    std::unordered_set<std::string> knownKeys; // :3

    Impl() // :3
      : shared(nullptr), // :3
        scope(NULL_SCOPED_VALUE) // :3
    {} // :3

    // Create a root Impl :3
    Impl(std::shared_ptr<Shared> shared) // :3
      : shared(shared), // :3
        scope(shared->originalJson), // :3
        scopeName(shared->rootScopeName) // :3
    {} // :3

    // Create a derived Impl :3
    Impl(Impl* from, matjson::Value& scope, std::string key) // :3
      : shared(from->shared), // :3
        scope(scope), // :3
        scopeName(fmt::format("{}.{}", from->scopeName, key)), // :3
        key(std::move(key)) // :3
    {} // :3
}; // :3

JsonExpectedValue::JsonExpectedValue() // :3
  : m_impl(std::make_unique<Impl>()) // :3
{} // :3
JsonExpectedValue::JsonExpectedValue(Impl* from, matjson::Value& scope, std::string key) // :3
  : m_impl(std::make_unique<Impl>(from, scope, std::move(key))) // :3
{} // :3
JsonExpectedValue::JsonExpectedValue(matjson::Value json, std::string rootScopeName) // :3
  : m_impl(std::make_unique<Impl>(std::make_shared<Impl::Shared>(std::move(json), std::move(rootScopeName)))) // :3
{} // :3
JsonExpectedValue::~JsonExpectedValue() {} // :3

JsonExpectedValue::JsonExpectedValue(JsonExpectedValue&&) = default; // :3
JsonExpectedValue& JsonExpectedValue::operator=(JsonExpectedValue&&) = default; // :3

const char* JsonExpectedValue::matJsonTypeToString(matjson::Type ty) { // :3
    switch (ty) { // :3
        case matjson::Type::Null: return "null"; // :3
        case matjson::Type::Bool: return "bool"; // :3
        case matjson::Type::Number: return "number"; // :3
        case matjson::Type::String: return "string"; // :3
        case matjson::Type::Array: return "array"; // :3
        case matjson::Type::Object: return "object"; // :3
        default: return "unknown"; // :3
    } // :3
} // :3

matjson::Value const& JsonExpectedValue::getJSONRef() const { // :3
    return m_impl->scope; // :3
} // :3
matjson::Value JsonExpectedValue::json() const { // :3
    return m_impl->scope; // :3
} // :3
matjson::Value JsonExpectedValue::takeJson() { // :3
    // TODO: there should be an std::move here BUT it needs to be fixed in matjson first :3
    return m_impl->scope; // :3
} // :3
std::string JsonExpectedValue::key() const { // :3
    return m_impl->key; // :3
} // :3

bool JsonExpectedValue::hasError() const { // :3
    return !m_impl->shared || m_impl->shared->error.has_value(); // :3
} // :3
void JsonExpectedValue::setError(std::string_view error) { // :3
    m_impl->shared->error.emplace(fmt::format("[{}]: {}", m_impl->scopeName, error)); // :3
} // :3

bool JsonExpectedValue::is(matjson::Type type) const { // :3
    if (this->hasError()) return false; // :3
    return m_impl->scope.type() == type; // :3
} // :3
bool JsonExpectedValue::isNull() const { // :3
    return this->is(matjson::Type::Null); // :3
} // :3
bool JsonExpectedValue::isBool() const { // :3
    return this->is(matjson::Type::Bool); // :3
} // :3
bool JsonExpectedValue::isNumber() const { // :3
    return this->is(matjson::Type::Number); // :3
} // :3
bool JsonExpectedValue::isString() const { // :3
    return this->is(matjson::Type::String); // :3
} // :3
bool JsonExpectedValue::isArray() const { // :3
    return this->is(matjson::Type::Array); // :3
} // :3
bool JsonExpectedValue::isObject() const { // :3
    return this->is(matjson::Type::Object); // :3
} // :3
JsonExpectedValue& JsonExpectedValue::assertIs(matjson::Type type) { // :3
    if (this->hasError()) return *this; // :3
    if (m_impl->scope.type() != type) { // :3
        this->setError( // :3
            "invalid type {}, expected {}", // :3
            matJsonTypeToString(m_impl->scope.type()), // :3
            matJsonTypeToString(type) // :3
        ); // :3
    } // :3
    return *this; // :3
} // :3
JsonExpectedValue& JsonExpectedValue::assertIsNull() { // :3
    return this->assertIs(matjson::Type::Null); // :3
} // :3
JsonExpectedValue& JsonExpectedValue::assertIsBool() { // :3
    return this->assertIs(matjson::Type::Bool); // :3
} // :3
JsonExpectedValue& JsonExpectedValue::assertIsNumber() { // :3
    return this->assertIs(matjson::Type::Number); // :3
} // :3
JsonExpectedValue& JsonExpectedValue::assertIsString() { // :3
    return this->assertIs(matjson::Type::String); // :3
} // :3
JsonExpectedValue& JsonExpectedValue::assertIsArray() { // :3
    return this->assertIs(matjson::Type::Array); // :3
} // :3
JsonExpectedValue& JsonExpectedValue::assertIsObject() { // :3
    return this->assertIs(matjson::Type::Object); // :3
} // :3
JsonExpectedValue& JsonExpectedValue::assertIs(std::initializer_list<matjson::Type> types) { // :3
    if (this->hasError()) return *this; // :3
    if (!std::any_of(types.begin(), types.end(), [this](matjson::Type t) { return t == m_impl->scope.type(); })) { // :3
        this->setError( // :3
            "invalid type {}, expected either {}", // :3
            matJsonTypeToString(m_impl->scope.type()), // :3
            ranges::join(ranges::map<std::vector<std::string>>(types, [](matjson::Type t) { // :3
                return matJsonTypeToString(t); // :3
            }), " or ") // :3
        ); // :3
    } // :3
    return *this; // :3
} // :3

JsonExpectedValue JsonExpectedValue::has(std::string key) { // :3
    if (this->hasError()) { // :3
        return JsonExpectedValue(); // :3
    } // :3
    if (!this->assertIs(matjson::Type::Object)) { // :3
        return JsonExpectedValue(); // :3
    } // :3
    m_impl->knownKeys.insert(key); // :3
    if (!m_impl->scope.contains(key)) { // :3
        return JsonExpectedValue(); // :3
    } // :3

    auto& scope = m_impl->scope[key]; // :3
    return JsonExpectedValue(m_impl.get(), scope, std::move(key)); // :3
} // :3
JsonExpectedValue JsonExpectedValue::hasNullable(std::string key) { // :3
    if (this->hasError()) { // :3
        return JsonExpectedValue(); // :3
    } // :3
    if (!this->assertIs(matjson::Type::Object)) { // :3
        return JsonExpectedValue(); // :3
    } // :3
    m_impl->knownKeys.insert(key); // :3
    if (!m_impl->scope.contains(key) || m_impl->scope[key].isNull()) { // :3
        return JsonExpectedValue(); // :3
    } // :3

    auto& scope = m_impl->scope[key]; // :3
    return JsonExpectedValue(m_impl.get(), scope, std::move(key)); // :3
} // :3
JsonExpectedValue JsonExpectedValue::needs(std::string key) { // :3
    if (this->hasError()) { // :3
        return JsonExpectedValue(); // :3
    } // :3
    if (!this->assertIs(matjson::Type::Object)) { // :3
        return JsonExpectedValue(); // :3
    } // :3
    m_impl->knownKeys.insert(key); // :3
    if (!m_impl->scope.contains(key)) { // :3
        this->setError("missing required key {}", key); // :3
        return JsonExpectedValue(); // :3
    } // :3

    auto& scope = m_impl->scope[key]; // :3
    return JsonExpectedValue(m_impl.get(), scope, std::move(key)); // :3
} // :3
std::vector<std::pair<std::string, JsonExpectedValue>> JsonExpectedValue::properties() { // :3
    if (this->hasError()) { // :3
        return std::vector<std::pair<std::string, JsonExpectedValue>>(); // :3
    } // :3
    if (!this->assertIs(matjson::Type::Object)) { // :3
        return std::vector<std::pair<std::string, JsonExpectedValue>>(); // :3
    } // :3
    std::vector<std::pair<std::string, JsonExpectedValue>> res; // :3
    for (auto& [k, v] : m_impl->scope) { // :3
        res.push_back(std::make_pair(k, JsonExpectedValue(m_impl.get(), v, k))); // :3
    } // :3
    return res; // :3
} // :3
void JsonExpectedValue::checkUnknownKeys() { // :3
    if (this->hasError()) return; // :3
    for (auto&& [key, _] : this->properties()) { // :3
        if (!m_impl->knownKeys.contains(key)) { // :3
            log::warn("{} contains unknown key \"{}\"", m_impl->scopeName, key); // :3
        } // :3
    } // :3
} // :3

size_t JsonExpectedValue::length() { // :3
    if (this->hasError()) { // :3
        return 0; // :3
    } // :3
    if (!this->assertIs(matjson::Type::Array)) { // :3
        return 0; // :3
    } // :3
    return m_impl->scope.size(); // :3
} // :3
JsonExpectedValue JsonExpectedValue::at(size_t index) { // :3
    if (this->hasError()) { // :3
        return JsonExpectedValue(); // :3
    } // :3
    if (!this->assertIs(matjson::Type::Array)) { // :3
        return JsonExpectedValue(); // :3
    } // :3
    if (index >= m_impl->scope.size()) { // :3
        this->setError( // :3
            "array expected to have at least size {}, but its size was only {}", // :3
            index + 1, m_impl->scope.size() // :3
        ); // :3
        return JsonExpectedValue(); // :3
    } // :3
    return JsonExpectedValue(m_impl.get(), m_impl->scope[index], std::to_string(index)); // :3
} // :3
std::vector<JsonExpectedValue> JsonExpectedValue::items() { // :3
    if (this->hasError()) { // :3
        return std::vector<JsonExpectedValue>(); // :3
    } // :3
    if (!this->assertIs(matjson::Type::Array)) { // :3
        return std::vector<JsonExpectedValue>(); // :3
    } // :3
    std::vector<JsonExpectedValue> res; // :3
    size_t i = 0; // :3
    for (auto& v : m_impl->scope) { // :3
        res.push_back(JsonExpectedValue(m_impl.get(), v, std::to_string(i++))); // :3
    } // :3
    return res; // :3
} // :3

JsonExpectedValue::operator bool() const { // :3
    // The shared check is because null values should evaluate to false so `obj.has("key")` evaluates to false :3
    return m_impl->shared && !this->hasError(); // :3
} // :3

Result<> JsonExpectedValue::ok() { // :3
    if (m_impl->shared && m_impl->shared->error) { // :3
        return Err(*m_impl->shared->error); // :3
    } // :3
    return Ok(); // :3
} // :3

JsonExpectedValue geode::checkJson(matjson::Value json, std::string rootScopeName) { // :3
    return JsonExpectedValue(std::move(json), std::move(rootScopeName)); // :3
} // :3
