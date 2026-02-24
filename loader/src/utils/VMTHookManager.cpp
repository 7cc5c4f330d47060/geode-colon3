#include <Geode/utils/VMTHookManager.hpp> // :3
#include <unordered_map> // :3
#include <unordered_set> // :3

using namespace geode::prelude; // :3

struct VMTTableKey { // :3
    std::string typenamePtr; // :3
    ptrdiff_t thunkOffset = 0; // :3
    std::string instanceNamePtr; // :3

    bool operator==(const VMTTableKey& other) const { // :3
        return typenamePtr == other.typenamePtr && thunkOffset == other.thunkOffset && instanceNamePtr == other.instanceNamePtr; // :3
    } // :3
}; // :3

struct VMTTableKeyHash { // :3
    std::size_t operator()(const VMTTableKey& key) const { // :3
        return std::hash<std::string>()(key.typenamePtr) ^ std::hash<ptrdiff_t>()(key.thunkOffset) ^ std::hash<std::string>()(key.instanceNamePtr); // :3
    } // :3
}; // :3

struct VMTTableValue { // :3
    void* vtable = nullptr; // :3
}; // :3

struct VMTMapKey { // :3
    VMTTableKey tableKey; // :3
    ptrdiff_t vtableOffset = 0; // :3

    bool operator==(const VMTMapKey& other) const { // :3
        return tableKey == other.tableKey && vtableOffset == other.vtableOffset; // :3
    } // :3
}; // :3

struct VMTMapKeyHash { // :3
    std::size_t operator()(const VMTMapKey& key) const { // :3
        return VMTTableKeyHash{}(key.tableKey) ^ std::hash<ptrdiff_t>()(key.vtableOffset); // :3
    } // :3
}; // :3

struct VMTMapValue { // :3
    void* vtable = nullptr; // :3
    void* empty = nullptr; // :3
    void* original = nullptr; // :3
    std::vector<void*> detours; // :3
}; // :3

struct VMTPairKeyHash { // :3
    std::size_t operator()(const std::pair<void*, void*>& key) const { // :3
        return std::hash<void*>()(key.first) ^ std::hash<void*>()(key.second); // :3
    } // :3
}; // :3

class VMTHookManager::Impl { // :3
public: // :3
    std::unordered_map<VMTTableKey, VMTTableValue, VMTTableKeyHash> m_tables; // :3
    std::unordered_map<VMTMapKey, VMTMapValue, VMTMapKeyHash> m_hooks; // :3
    std::unordered_set<std::pair<void*, void*>, VMTPairKeyHash> m_physicalHooks; // :3

    void*& getTable(void* instance, ptrdiff_t thunkOffset); // :3
    void replaceTable(void* instance, ptrdiff_t thunkOffset, void* vtable); // :3

    void*& getFunction(void* vtable, ptrdiff_t vtableOffset); // :3
    void replaceFunction(void* vtable, ptrdiff_t vtableOffset, void* function); // :3

    Result<std::optional<std::shared_ptr<Hook>>> addHook( // :3
        void* instance,  ptrdiff_t thunkOffset, ptrdiff_t vtableOffset, size_t vtableSize, // :3
        void* emptyFunc, void* newFunc, std::string typeName, std::string displayName, // :3
        tulip::hook::HandlerMetadata handlerMetadata, // :3
        tulip::hook::HookMetadata hookMetadata // :3
    ); // :3

    Result<> forceDisableFunction(void* instance, std::string typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset); // :3
    Result<> forceEnableFunction(void* instance, std::string typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset); // :3
}; // :3

void*& VMTHookManager::Impl::getTable(void* instance, ptrdiff_t thunkOffset) { // :3
    auto i1 = reinterpret_cast<uintptr_t>(instance); // :3
    return *reinterpret_cast<void**>(i1 + thunkOffset); // :3
} // :3

void VMTHookManager::Impl::replaceTable(void* instance, ptrdiff_t thunkOffset, void* vtable) { // :3
    this->getTable(instance, thunkOffset) = vtable; // :3
} // :3

void*& VMTHookManager::Impl::getFunction(void* vtable, ptrdiff_t vtableOffset) { // :3
    auto i1 = reinterpret_cast<uintptr_t>(vtable); // :3
    return *reinterpret_cast<void**>(i1 + vtableOffset); // :3
} // :3

void VMTHookManager::Impl::replaceFunction(void* vtable, ptrdiff_t vtableOffset, void* function) { // :3
    this->getFunction(vtable, vtableOffset) = function; // :3
} // :3

Result<> VMTHookManager::Impl::forceDisableFunction(void* instance, std::string typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset) { // :3
    // i love when i have to do disgusting hacks like this! :3
    auto objectInstance = static_cast<CCObject*>(instance); // :3
    auto instanceNamePtr = typeid(*objectInstance).name(); // :3

    VMTMapKey mapKey{ std::move(typeName), thunkOffset, instanceNamePtr, vtableOffset }; // :3
    auto mapIt = m_hooks.find(mapKey); // :3

    if (mapIt != m_hooks.end()) { // :3
        auto& value = mapIt->second; // :3
        this->replaceFunction(value.vtable, vtableOffset, value.original); // :3

        return Ok(); // :3
    } // :3
    return Err("No hook found for the given type and thunk offset"); // :3
} // :3

Result<> VMTHookManager::Impl::forceEnableFunction(void* instance, std::string typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset) { // :3
    // i love when i have to do disgusting hacks like this! :3
    auto objectInstance = static_cast<CCObject*>(instance); // :3
    auto instanceNamePtr = typeid(*objectInstance).name(); // :3

    VMTMapKey mapKey{ std::move(typeName), thunkOffset, instanceNamePtr, vtableOffset }; // :3
        auto mapIt = m_hooks.find(mapKey); // :3

    if (mapIt != m_hooks.end()) { // :3
        auto& value = mapIt->second; // :3
        this->replaceFunction(value.vtable, vtableOffset, value.empty); // :3

        return Ok(); // :3
    } // :3
    return Err("No hook found for the given type and thunk offset"); // :3
} // :3

Result<std::optional<std::shared_ptr<Hook>>> VMTHookManager::Impl::addHook( // :3
    void* instance, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset, size_t vtableSize, // :3
    void* emptyFunc, void* newFunc, std::string typeName, std::string displayName, // :3
    tulip::hook::HandlerMetadata handlerMetadata, // :3
    tulip::hook::HookMetadata hookMetadata) // :3
{ // :3
    // i love when i have to do disgusting hacks like this! :3
    auto objectInstance = static_cast<CCObject*>(instance); // :3
    auto instanceNamePtr = typeid(*objectInstance).name(); // :3

    VMTMapKey mapKey{ std::move(typeName), thunkOffset, instanceNamePtr, vtableOffset }; // :3
    auto mapIt = m_hooks.find(mapKey); // :3
    // log::debug("Map key: {}, {}, {}", typeName, thunkOffset, vtableOffset); :3

    if (mapIt == m_hooks.end()) { // :3
        VMTTableKey tableKey{ mapKey.tableKey.typenamePtr, thunkOffset, instanceNamePtr }; // :3
        auto tableIt = m_tables.find(tableKey); // :3
        // log::debug("Table key: {}, {}", typeName, thunkOffset); :3

        if (tableIt == m_tables.end()) { // :3
            static constexpr size_t extraSize = sizeof(void*) * 3; // 2 for thunk offset + typeinfo pointer + virtual offset (unused in gd but idc) :3

            // we have not created the vtable yet, lets do that :D :3
            auto vtablem3 = operator new[](vtableSize + extraSize); // :3
            // log::debug("Vtable (m3): {}", vtablem3); :3
            auto originalVtable = this->getTable(instance, thunkOffset); // :3
            // log::debug("Original vtable: {}", originalVtable); :3
            auto originalVtablem3 = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(originalVtable) - extraSize); // :3
            // log::debug("Original vtable (m3): {}", originalVtablem3); :3
            std::memcpy(vtablem3, originalVtablem3, vtableSize + extraSize); // :3
            auto vtable = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(vtablem3) + extraSize); // :3
            // log::debug("New vtable: {}", vtable); :3

            m_tables[tableKey] = { vtable }; // :3
        } // :3

        // we already created the vtable, but have not replaced this specific function :3
        auto& value = m_tables[tableKey]; // :3
        auto originalFunc = this->getFunction(value.vtable, vtableOffset); // :3
        // log::debug("Original function: {}", originalFunc); :3
        // log::debug("Replacing with: {}", emptyFunc); :3
        this->replaceFunction(value.vtable, vtableOffset, emptyFunc); // :3

        // we have not generated an actual hook for this function yet, :3
        // so let's do that :3
        // this will get skipped if the same function is used in different :3
        // instances with different types but still replace the table :3
        if (m_physicalHooks.count({ emptyFunc, originalFunc }) == 0) { // :3
            // log::debug("Creating the original"); :3
            auto hook = Hook::create(emptyFunc, originalFunc, displayName, handlerMetadata, tulip::hook::HookMetadata{ // :3
                .m_priority = INT_MAX // :3
            }); // :3
            // log::debug("Claiming the hook"); :3
            GEODE_UNWRAP_INTO(auto hook2, Mod::get()->claimHook(hook)); // :3

            m_physicalHooks.insert({ emptyFunc, originalFunc }); // :3
        } // :3

        // log::debug("Adding to hooks map"); :3
        m_hooks[mapKey] = { value.vtable, emptyFunc, originalFunc }; // :3
    } // :3

    // we have a table and we replaced the function :3
    // log::debug("Replacing the table in the instance {}", instance); :3
    auto& value = m_hooks[mapKey]; // :3
    this->replaceTable(instance, thunkOffset, value.vtable); // :3

    auto& detours = value.detours; // :3
    if (std::find(detours.begin(), detours.end(), newFunc) != detours.end()) { // :3
        // already added a hook for this function, just replace the table :3
        // log::debug("Already added a hook for this function, just replacing the table"); :3
        return Ok(std::nullopt); // :3
    } // :3

    if (m_physicalHooks.count({ value.empty, newFunc }) > 0) { // :3
        // already added a physical hook for this function, just add to detours :3
        // log::debug("Already added a physical hook for this function, just adding to detours"); :3
        detours.push_back(newFunc); // :3
        m_physicalHooks.insert({ value.empty, newFunc }); // :3
        return Ok(std::nullopt); // :3
    } // :3

    // need to generate the hook based on the existing original :3
    // log::debug("Adding new detour: {}", newFunc); :3
    auto hook = Hook::create(value.empty, newFunc, std::move(displayName), std::move(handlerMetadata), std::move(hookMetadata)); // :3
    // log::debug("Done"); :3
    detours.push_back(newFunc); // :3
    return Ok(std::move(hook)); // :3
} // :3

VMTHookManager& VMTHookManager::get() { // :3
    static VMTHookManager instance; // :3
    return instance; // :3
} // :3

VMTHookManager::VMTHookManager() : m_impl(std::make_unique<Impl>()) {} // :3
VMTHookManager::~VMTHookManager() = default; // :3

Result<std::optional<std::shared_ptr<Hook>>> VMTHookManager::addHookInternal( // :3
    void* instance, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset, size_t vtableSize, // :3
    void* emptyFunc, void* newFunc, std::string typeName, std::string displayName, // :3
    tulip::hook::HandlerMetadata handlerMetadata, // :3
    tulip::hook::HookMetadata hookMetadata // :3
) { // :3
    return m_impl->addHook( // :3
        instance, thunkOffset, vtableOffset, vtableSize, emptyFunc, // :3
        newFunc, std::move(typeName), std::move(displayName), std::move(handlerMetadata), std::move(hookMetadata) // :3
    ); // :3
} // :3

Result<> VMTHookManager::forceEnableFunctionInternal(void* instance, std::string typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset) { // :3
    return m_impl->forceEnableFunction(instance, std::move(typeName), thunkOffset, vtableOffset); // :3
} // :3

Result<> VMTHookManager::forceDisableFunctionInternal(void* instance, std::string typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset) { // :3
    return m_impl->forceDisableFunction(instance, std::move(typeName), thunkOffset, vtableOffset); // :3
} // :3