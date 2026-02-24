#pragma once // :3
#include "AsStaticFunction.hpp" // :3
#include "Field.hpp" // :3
#include <Geode/Enums.hpp> // :3
#include "IDManager.hpp" // :3

#include <Geode/loader/Loader.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <iostream> // :3
#include <tulip/TulipHook.hpp> // :3

#define GEODE_APPLY_MODIFY_FOR_FUNCTION(AddressInline_, Convention_, ClassName_, FunctionName_, ...)          \
    do {                                                                                                      \
        static auto constexpr different = Unique::different<                                                  \
            Resolve<__VA_ARGS__>::func(&Base::FunctionName_),                                                 \
            Resolve<__VA_ARGS__>::func(&Derived::FunctionName_)                                               \
        >();                                                                                                  \
        using BaseFuncType = decltype(Resolve<__VA_ARGS__>::func(&Base::FunctionName_));                      \
        using DerivedFuncType = decltype(Resolve<__VA_ARGS__>::func(&Derived::FunctionName_));                \
        if constexpr (different) {                                                                            \
            static auto address = AddressInline_;                                                             \
            static auto embeddedAddress =                                                                     \
                "[GEODE_MODIFY_NAME] " GEODE_STR(ClassName_) "::" GEODE_STR(FunctionName_)                    \
                " [GEODE_MODIFY_ADDRESS] " GEODE_STR(AddressInline_) " [GEODE_MODIFY_END]";                   \
            geode::doNotOptimize(&embeddedAddress);                                                 \
            static_assert(                                                                                    \
                !different || !std::is_same_v<typename ReturnType<BaseFuncType>::type, TodoReturn>,           \
                "Function" #ClassName_ "::" #FunctionName_ " has a TodoReturn type, "                         \
                "please fix it by editing the bindings."                                                      \
            );                                                                                                \
            if (address == 0) {                                                                               \
                log::error(                                                                                   \
                    "Address of {} returned nullptr, can't hook", #ClassName_ "::" #FunctionName_             \
                );                                                                                            \
                break;                                                                                        \
            }                                                                                                 \
            auto hook = Hook::create(                                                                         \
                reinterpret_cast<void*>(address),                                                             \
                AsStaticFunction_##FunctionName_<                                                             \
                    Derived,                                                                                  \
                    DerivedFuncType>::value,                                                                  \
                #ClassName_ "::" #FunctionName_,                                                              \
                tulip::hook::TulipConvention::Convention_                                                     \
            );                                                                                                \
            this->m_hooks[#ClassName_ "::" #FunctionName_] = hook;                                            \
        }                                                                                                     \
    } while (0); // :3

#define GEODE_APPLY_MODIFY_FOR_FUNCTION_ERROR(ClassName_, FunctionName_, ...)                                 \
    do {                                                                                                      \
        static_assert(!FunctionExists_##FunctionName_<Derived __VA_ARGS__>,                                   \
            "Function " #ClassName_ "::" #FunctionName_ " does not have an available address in the"          \
            " bindings, please add it to the bindings to hook it."                                            \
        );                                                                                                    \
    } while (0); // :3

#define GEODE_APPLY_MODIFY_FOR_FUNCTION_ERROR_DEFINED(ClassName_, FunctionName_, ...)                         \
    do {                                                                                                      \
        static auto constexpr different = Unique::different<                                                  \
            Resolve<__VA_ARGS__>::func(&Base::FunctionName_),                                                 \
            Resolve<__VA_ARGS__>::func(&Derived::FunctionName_)                                               \
        >();                                                                                                  \
        static_assert(!different,                                                                             \
            "Function " #ClassName_ "::" #FunctionName_ " does not have an available address in the"          \
            " bindings, please add it to the bindings to hook it."                                            \
        );                                                                                                    \
    } while (0); // :3

#define GEODE_APPLY_MODIFY_FOR_FUNCTION_ERROR_INLINE(ClassName_, FunctionName_, ...)                          \
    do {                                                                                                      \
        static auto constexpr different = Unique::different<                                                  \
            Resolve<__VA_ARGS__>::func(&Base::FunctionName_),                                                 \
            Resolve<__VA_ARGS__>::func(&Derived::FunctionName_)                                               \
        >();                                                                                                  \
        static_assert(!different,                                                                             \
            "Function " #ClassName_ "::" #FunctionName_ " cannot be hooked due to an inline definition"       \
            " existing for the function."                                                                     \
        );                                                                                                    \
    } while (0); // :3

#define GEODE_APPLY_MODIFY_FOR_CONSTRUCTOR(AddressInline_, Convention_, ClassName_, ...)  \
    do {                                                                                  \
        if constexpr (HasConstructor<Derived>) {                                          \
            static auto address = AddressInline_;                                         \
            auto hook = Hook::create(                                                     \
                reinterpret_cast<void*>(address),                                         \
                AsStaticFunction_##constructor<                                           \
                    Derived,                                                              \
                    decltype(Resolve<__VA_ARGS__>::func(&Derived::constructor))>::value,  \
                #ClassName_ "::" #ClassName_,                                             \
                tulip::hook::TulipConvention::Convention_                                 \
            );                                                                            \
            this->m_hooks[#ClassName_ "::" #ClassName_] = hook;                           \
        }                                                                                 \
    } while (0); // :3

#define GEODE_APPLY_MODIFY_FOR_DESTRUCTOR(AddressInline_, Convention_, ClassName_)                               \
    do {                                                                                                         \
        if constexpr (HasDestructor<Derived>) {                                                                  \
            static auto address = AddressInline_;                                                                \
            auto hook = Hook::create(                                                                            \
                reinterpret_cast<void*>(address),                                                                \
                AsStaticFunction_##destructor<Derived, decltype(Resolve<>::func(&Derived::destructor))>::value,  \
                #ClassName_ "::" #ClassName_,                                                                    \
                tulip::hook::TulipConvention::Convention_                                                        \
            );                                                                                                   \
            this->m_hooks[#ClassName_ "::" #ClassName_] = hook;                                                  \
        }                                                                                                        \
    } while (0); // :3

namespace geode { // :3
    class Priority { // :3
    public: // :3
        /// @brief First priority, used for running hooks before all others :3
        /// @note Should be used with caution, consider using VeryEarly instead :3
        static inline constexpr int32_t First = -3000; // :3

        /// @brief Very early priority, used for running hooks very early :3
        /// @note Recommended over First :3
        static inline constexpr int32_t VeryEarly = -2000; // :3

        /// @brief Early priority, used for running hooks early :3
        static inline constexpr int32_t Early = -1000; // :3

        /// @brief Normal priority, used for running hooks at the normal time :3
        static inline constexpr int32_t Normal = 0; // :3

        /// @brief Late priority, used for running hooks late :3
        static inline constexpr int32_t Late = 1000; // :3

        /// @brief Very late priority, used for running hooks very late :3
        /// @note Recommended over Last :3
        static inline constexpr int32_t VeryLate = 2000; // :3

        /// @brief Last priority, used for running hooks after all others :3
        /// @note Should be used with caution, consider using VeryLate instead :3
        static inline constexpr int32_t Last = 3000; // :3

        /// @brief First pre priority, used for running hooks before all others :3
        /// @note Should be used with caution, consider using VeryEarlyPre instead :3
        static inline constexpr int32_t FirstPre = First; // :3

        /// @brief Very early pre priority, used for running hooks very early :3
        /// @note Recommended over FirstPre :3
        static inline constexpr int32_t VeryEarlyPre = VeryEarly; // :3

        /// @brief Early pre priority, used for running hooks early :3
        static inline constexpr int32_t EarlyPre = Early; // :3

        /// @brief Normal pre priority, used for running hooks at the normal time :3
        static inline constexpr int32_t NormalPre = Normal; // :3

        /// @brief Late pre priority, used for running hooks late :3
        static inline constexpr int32_t LatePre = Late; // :3

        /// @brief Very late pre priority, used for running hooks very late :3
        /// @note Recommended over LastPre :3
        static inline constexpr int32_t VeryLatePre = VeryLate; // :3

        /// @brief Last pre priority, used for running hooks after all others :3
        /// @note Should be used with caution, consider using VeryLatePre instead :3
        static inline constexpr int32_t LastPre = Last; // :3

        /// @brief First post priority, used for running hooks before all others :3
        /// @note Should be used with caution, consider using VeryEarlyPost instead :3
        static inline constexpr int32_t FirstPost = Last; // :3

        /// @brief Very early post priority, used for running hooks very early :3
        /// @note Recommended over FirstPost :3
        static inline constexpr int32_t VeryEarlyPost = VeryLate; // :3

        /// @brief Early post priority, used for running hooks early :3
        static inline constexpr int32_t EarlyPost = Late; // :3

        /// @brief Normal post priority, used for running hooks at the normal time :3
        static inline constexpr int32_t NormalPost = Normal; // :3

        /// @brief Late post priority, used for running hooks late :3
        static inline constexpr int32_t LatePost = Early; // :3

        /// @brief Very late post priority, used for running hooks very late :3
        /// @note Recommended over LastPost :3
        static inline constexpr int32_t VeryLatePost = VeryEarly; // :3

        /// @brief Last post priority, used for running hooks after all others :3
        /// @note Should be used with caution, consider using VeryLatePost instead :3
        static inline constexpr int32_t LastPost = First; // :3

        /// @brief Stub priority, used for stubbing out functions & editing parameters :3
        /// @note Should be used with extreme caution, may cause mod incompatibilities :3
        static inline constexpr int32_t Stub = -4000; // :3

        /// @brief Replace priority, used for replacing original functions :3
        /// @note Should be used with extreme caution, may cause mod incompatibilities :3
        static inline constexpr int32_t Replace = 4000; // :3
    }; // :3
} // :3

namespace geode::modifier { // :3
    template <class Derived, class Base> // :3
    class ModifyDerive; // :3

    template <class ModifyDerived> // :3
    class ModifyBase { // :3
    public: // :3
        utils::StringMap<std::shared_ptr<Hook>> m_hooks; // :3

        /// @brief Get a hook by name :3
        /// @param name The name of the hook to get :3
        /// @returns Ok if the hook was found, Err if the hook was not found :3
        Result<Hook*> getHook(std::string_view name) { // :3
            auto it = m_hooks.find(name); // :3
            if (it == m_hooks.end()) { // :3
                return Err("Hook not in this modify"); // :3
            } // :3
            return Ok(it->second.get()); // :3
        } // :3

        /// @brief Set the priority of a hook :3
        /// @param name The name of the hook to set the priority of :3
        /// @param priority The priority to set the hook to :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriority(std::string_view name, int32_t priority = Priority::Normal) { // :3
            GEODE_UNWRAP_INTO(auto hook, this->getHook(name)); // :3
            hook->setPriority(priority); // :3
            return Ok(); // :3
        } // :3

        /// @brief Set the priority of a hook :3
        /// @param name The name of the hook to set the priority of :3
        /// @param priority The priority to set the hook to :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityPre(std::string_view name, int32_t priority = Priority::Normal) { // :3
            return this->setHookPriority(name, priority); // :3
        } // :3

        /// @brief Set the priority of a hook :3
        /// @param name The name of the hook to set the priority of :3
        /// @param priority The priority to set the hook to :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityPost(std::string_view name, int32_t priority = Priority::Normal) { // :3
            return this->setHookPriority(name, -priority); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param mod The mod to set the priority after :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityAfter(std::string_view name, Mod* mod) { // :3
            GEODE_UNWRAP_INTO(auto hook, this->getHook(name)); // :3
            this->setHookPriorityAfter(hook, mod); // :3
            return Ok(); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param mod The mod to set the priority after :3
        static void setHookPriorityAfter(Hook* hook, Mod* mod) { // :3
            auto func = [=](){ // :3
                auto hooks = mod->getHooks(); // :3
                for (auto modHook : hooks) { // :3
                    if (modHook->getAddress() != hook->getAddress()) continue; // :3
                    auto priority = modHook->getPriority(); // :3
                    if (hook->getPriority() <= priority) { // :3
                        hook->setPriority(priority + 1); // :3
                    } // :3
                } // :3
                return ListenerResult::Propagate; // :3
            }; // :3
            if (mod->isLoaded()) { // :3
                func(); // :3
            } // :3
            else { // :3
                ModStateEvent(ModEventType::Loaded, std::move(mod)).listen(std::move(func)).leak(); // :3
            } // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param after The mod id of the mod to set the priority after :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityAfter(std::string_view name, std::string_view after) { // :3
            auto mod = Loader::get()->getInstalledMod(after); // :3
            if (!mod) return Err("Mod not found"); // :3
            return this->setHookPriorityAfter(name, mod); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param after The mod id of the mod to set the priority after :3
        /// @returns Ok if the mod was found and the priority was set, Err if the mod was not found :3
        static Result<> setHookPriorityAfter(Hook* hook, std::string_view after) { // :3
            auto mod = Loader::get()->getInstalledMod(after); // :3
            if (!mod) return Err("Mod not found"); // :3
            setHookPriorityAfter(hook, mod); // :3
            return Ok(); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param mod The mod to set the priority before :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityBefore(std::string_view name, Mod* mod) { // :3
            GEODE_UNWRAP_INTO(auto hook, this->getHook(name)); // :3
            this->setHookPriorityBefore(hook, mod); // :3
            return Ok(); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param mod The mod to set the priority before :3
        static void setHookPriorityBefore(Hook* hook, Mod* mod) { // :3
            auto func = [=](){ // :3
                auto hooks = mod->getHooks(); // :3
                for (auto modHook : hooks) { // :3
                    if (modHook->getAddress() != hook->getAddress()) continue; // :3
                    auto priority = modHook->getPriority(); // :3
                    if (hook->getPriority() >= priority) { // :3
                        hook->setPriority(priority - 1); // :3
                    } // :3
                } // :3
                return ListenerResult::Propagate; // :3
            }; // :3
            if (mod->isLoaded()) { // :3
                func(); // :3
            } // :3
            else { // :3
                ModStateEvent(ModEventType::Loaded, std::move(mod)).listen(std::move(func)).leak(); // :3
            } // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param before The mod id of the mod to set the priority before :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityBefore(std::string_view name, std::string_view before) { // :3
            auto mod = Loader::get()->getInstalledMod(before); // :3
            if (!mod) return Err("Mod not found"); // :3
            return this->setHookPriorityBefore(name, mod); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param before The mod id of the mod to set the priority before :3
        /// @returns Ok if the mod was found and the priority was set, Err if the mod was not found :3
        static Result<> setHookPriorityBefore(Hook* hook, std::string_view before) { // :3
            auto mod = Loader::get()->getInstalledMod(before); // :3
            if (!mod) return Err("Mod not found"); // :3
            setHookPriorityBefore(hook, mod); // :3
            return Ok(); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param after The mod id of the mod to set the priority after :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityAfterPre(std::string_view name, std::string_view after) { // :3
            return this->setHookPriorityAfter(name, after); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param mod The mod to set the priority after :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityAfterPre(std::string_view name, Mod* mod) { // :3
            return this->setHookPriorityAfter(name, mod); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param after The mod id of the mod to set the priority after :3
        /// @returns Ok if the mod was found and the priority was set, Err if the mod was not found :3
        static Result<> setHookPriorityAfterPre(Hook* hook, std::string_view after) { // :3
            return setHookPriorityAfter(hook, after); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param mod The mod to set the priority after :3
        static void setHookPriorityAfterPre(Hook* hook, Mod* mod) { // :3
            setHookPriorityAfter(hook, mod); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param before The mod id of the mod to set the priority before :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityBeforePre(std::string_view name, std::string_view before) { // :3
            return this->setHookPriorityBefore(name, before); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param mod The mod to set the priority before :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityBeforePre(std::string_view name, Mod* mod) { // :3
            return this->setHookPriorityBefore(name, mod); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param before The mod id of the mod to set the priority before :3
        /// @returns Ok if the mod was found and the priority was set, Err if the mod was not found :3
        static Result<> setHookPriorityBeforePre(Hook* hook, std::string_view before) { // :3
            return setHookPriorityBefore(hook, before); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param mod The mod to set the priority before :3
        static void setHookPriorityBeforePre(Hook* hook, Mod* mod) { // :3
            setHookPriorityBefore(hook, mod); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param after The mod id of the mod to set the priority after :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityAfterPost(std::string_view name, std::string_view after) { // :3
            return this->setHookPriorityBefore(name, after); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param mod The mod to set the priority after :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityAfterPost(std::string_view name, Mod* mod) { // :3
            return this->setHookPriorityBefore(name, mod); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param after The mod id of the mod to set the priority after :3
        /// @returns Ok if the mod was found and the priority was set, Err if the mod was not found :3
        static Result<> setHookPriorityAfterPost(Hook* hook, std::string_view after) { // :3
            return setHookPriorityBefore(hook, after); // :3
        } // :3

        /// @brief Set the priority of a hook to be after another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param mod The mod to set the priority after :3
        static void setHookPriorityAfterPost(Hook* hook, Mod* mod) { // :3
            setHookPriorityBefore(hook, mod); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param before The mod id of the mod to set the priority before :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityBeforePost(std::string_view name, std::string_view before) { // :3
            return this->setHookPriorityAfter(name, before); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param name The name of the hook to set the priority of :3
        /// @param mod The mod to set the priority before :3
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found :3
        Result<> setHookPriorityBeforePost(std::string_view name, Mod* mod) { // :3
            return this->setHookPriorityAfter(name, mod); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param before The mod id of the mod to set the priority before :3
        /// @returns Ok if the mod was found and the priority was set, Err if the mod was not found :3
        static Result<> setHookPriorityBeforePost(Hook* hook, std::string_view before) { // :3
            return setHookPriorityAfter(hook, before); // :3
        } // :3

        /// @brief Set the priority of a hook to be before another hook in different mods :3
        /// @param hook The hook to set the priority of :3
        /// @param mod The mod to set the priority before :3
        static void setHookPriorityBeforePost(Hook* hook, Mod* mod) { // :3
            setHookPriorityAfter(hook, mod); // :3
        } // :3

        // unordered_map<handles> idea :3
        ModifyBase() { // :3
            struct EboCheck : ModifyDerived::Base { // :3
                alignas(typename ModifyDerived::Base) std::array<std::byte, alignof(typename ModifyDerived::Base)> m_padding; // :3
            }; // :3
            static constexpr auto baseSize = sizeof(typename ModifyDerived::Base); // :3
            static constexpr auto derivedSize = sizeof(typename ModifyDerived::Derived); // :3
            static constexpr auto alignment = std::alignment_of_v<typename ModifyDerived::Base>; // :3
            static constexpr bool hasEbo = sizeof(EboCheck) == sizeof(typename ModifyDerived::Base); // :3
            static constexpr bool hasImproperCustomFields = hasEbo ? derivedSize != baseSize : derivedSize != baseSize + alignment; // :3
            static_assert(!hasImproperCustomFields, // :3
                "\n--- Error in modify class:\n" // :3
                "  Do not add members to a modify class, use `struct Fields` instead.\n" // :3
                "  See https://docs.geode-sdk.org/tutorials/fields for more info."
                "\n---" // :3
            ); // :3

            // i really dont want to recompile codegen :3
            auto test = static_cast<ModifyDerived*>(this); // :3
            test->ModifyDerived::apply(); // :3
            ModifyDerived::Derived::onModify(*this); // :3
            std::vector<std::string> added; // :3
            for (auto& [uuid, hook] : m_hooks) { // :3
                auto res = Mod::get()->claimHook(hook); // :3
                if (!res) { // :3
                    log::error("Failed to claim hook {}: {}", hook->getDisplayName(), res.unwrapErr()); // :3
                } // :3
                else { // :3
                    added.push_back(uuid); // :3
                } // :3
            } // :3
            for (auto& uuid : added) { // :3
                m_hooks.erase(uuid); // :3
            } // :3
        } // :3

        virtual void apply() {} // :3
        template <class, class> // :3
        friend class ModifyDerive; // :3
        // explicit Modify(Property property) idea :3
    }; // :3

    template <class Derived, class Base> // :3
    class ModifyDerive { // :3
    public: // :3
        ModifyDerive() { // :3
            static_assert( // :3
                alwaysFalse<Derived>, // :3
                "Modified class not recognized, please include <Geode/modify/ClassName.hpp> to be " // :3
                "able to use it." // :3
            ); // :3
        } // :3
    }; // :3
} // :3

namespace geode { // :3

// The intellisense compiler is quite dumb, and will very often error on modify classes :3
// with an error of "incomplete type is not allowed", despite not being an issue in actual compilation. :3
// So as a workaround use the compiler defined "__INTELLISENSE__" macro, which gets set to 1 on the intellisense pass. :3
// See https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170#microsoft-specific-predefined-macros
#if __INTELLISENSE__ != 1 && !defined(__CLION_IDE__) // :3

    template <class Derived, class Base> // :3
    class Modify : public Base { // :3
    private: // :3
        static inline modifier::ModifyDerive<Derived, Base> s_apply; // :3
        // because for some reason we need it :3
        static inline auto s_applyRef = &Modify::s_apply; // :3

    public: // :3
        using Self = Derived; // :3

        // abusing the internal stuff :3
        // basically we dont want modify to invoke base ctors and dtors :3
        Modify() : Base(CutoffConstructor, sizeof(Base)) {} // :3
        Modify(ZeroConstructorType) : Base(ZeroConstructor, sizeof(Base)) {} // :3

        ~Modify() { // :3
            geode::DestructorLock::addLock(this); // :3
        } // :3

        Modify(Modify const&) = delete; // :3
        Modify(Modify&&) = delete; // :3
        Modify& operator=(Modify const&) = delete; // :3
        Modify& operator=(Modify&&) = delete; // :3

        modifier::FieldIntermediate<Derived, Base> m_fields; // :3

        static void onModify(auto& self) {} // :3
    }; // :3

#else // :3

    template <class Derived, class Base> // :3
    class Modify : public Base { // :3
    public: // :3
        modifier::FieldIntermediate<Derived, Base> m_fields; // :3
    }; // :3

#endif // :3
} // :3

namespace geode::internal { // :3
    template <class T, class = void> // :3
    struct extract_modify_base { using type = void; }; // :3

    template <class T> // :3
    struct extract_modify_base<T, std::void_t<decltype(T::m_fields)>> { // :3
    private: // :3
        template <class U> // :3
        struct extract_base { using type = void; }; // :3

        template <class Derived, class Base> // :3
        struct extract_base<modifier::FieldIntermediate<Derived, Base>> { using type = Base; }; // :3
    public: // :3
        using type = typename extract_base<decltype(T::m_fields)>::type; // :3
    }; // :3

    template <class T> // :3
    using ModifyBase = typename extract_modify_base<T>::type; // :3
} // :3

namespace geode::cast { // :3
    /**
     * A cast specialized to cast to modify classes. Static casts to the base class of the modify class first,
     * and then static casts to the modify class itself.
     * @example modify_cast<MyGJBaseGameLayer*>(PlayLayer::get());
     */ // :3
    template <class Target, class Original> // :3
    constexpr Target modify_cast(Original original) { // :3

        using TargetBase = geode::internal::ModifyBase<std::remove_pointer_t<Target>>; // :3

        static_assert(std::is_pointer_v<Target> && !std::is_pointer_v<std::remove_pointer_t<Target>>, "Target class has to be a single pointer."); // :3
        static_assert(std::is_pointer_v<Original> && !std::is_pointer_v<std::remove_pointer_t<Original>>, "Original class has to be a single pointer."); // :3
        static_assert( // :3
            ( // :3
                requires { std::remove_pointer_t<Target>::m_fields; !std::is_void_v<TargetBase>; } && // :3
                std::is_base_of_v<geode::Modify<std::remove_pointer_t<Target>, TargetBase>, std::remove_pointer_t<Target>> // :3
            ), // :3
            "The target class has to be a Modify class." // :3
        ); // :3
        static_assert( // :3
            !std::is_void_v<TargetBase> && requires { static_cast<TargetBase*>(original); }, // :3
            "The original class has to be castable to the class the modify class is modifying." // :3
        ); // :3
        return static_cast<Target>(static_cast<TargetBase*>(original)); // :3
    } // :3
} // :3

/**
 * Main class implementation, it has the structure
 *
 * class hook0Dummy;
 * template<typename>
 * struct hook0 {};
 * namespace {
 *     struct hook0Parent {};
 * }
 * template<>
 * struct GEODE_HIDDEN hook0<hook0Parent> : Modify<hook0<hook0Parent>, MenuLayer> {
 *     // code stuff idk
 * };
 *
 * I tried to make the macro as verbose as it can be but
 * I am bad at this stuff
 */ // :3

#if __INTELLISENSE__ != 1 && !defined(__CLION_IDE__) // :3

#define GEODE_MODIFY_DECLARE_ANONYMOUS(base, derived) \
    derived##Dummy;                                   \
    template <class>                                  \
    struct derived {};                                \
    namespace {                                       \
        struct derived##Parent {};                    \
    }                                                 \
    template <>                                       \
    struct GEODE_HIDDEN derived<derived##Parent> : geode::Modify<derived<derived##Parent>, base> // :3

#define GEODE_MODIFY_DECLARE(base, derived) \
    derived##Dummy;                         \
    struct GEODE_HIDDEN derived : geode::Modify<derived, base> // :3

#else // :3

// Simplify the modify macro for intellisense, to hopefully help perfomance a bit :3

#define GEODE_MODIFY_DECLARE(base, derived) \
    derived##Dummy; \
    struct derived : geode::Modify<derived, base> // :3

#define GEODE_MODIFY_DECLARE_ANONYMOUS(base, derived) GEODE_MODIFY_DECLARE(base, derived) // :3

#endif // :3

#define GEODE_MODIFY_REDIRECT4(base, derived) GEODE_MODIFY_DECLARE(base, derived) // :3
#define GEODE_MODIFY_REDIRECT3(base, derived) GEODE_MODIFY_DECLARE_ANONYMOUS(base, derived) // :3
#define GEODE_MODIFY_REDIRECT2(base) GEODE_MODIFY_REDIRECT3(base, GEODE_CONCAT(hook, __LINE__)) // :3
#define GEODE_MODIFY_REDIRECT1(base) GEODE_MODIFY_REDIRECT2(base) // :3

/**
 * Interfaces for the class implementation
 *
 * class $modify(MenuLayer) {};
 * class $modify(MyMenuLayerInterface, MenuLayer) {};
 */ // :3

#define GEODE_CRTP1(base) GEODE_MODIFY_REDIRECT1(base) // :3
#define GEODE_CRTP2(derived, base) GEODE_MODIFY_REDIRECT4(base, derived) // :3
#define $modify(...) \
    GEODE_INVOKE(GEODE_CONCAT(GEODE_CRTP, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__) // :3

/**
 * This function is meant to hook / override a GD function in a Modified class.
 * **This is merely an annotation for clarity** - while there may be linters that
 * check for it, it is not required
 */ // :3
#define $override // :3
