#pragma once // :3

#include "addresser.hpp" // :3
#include <memory> // :3
#include <optional> // :3
#include "../modify/Traits.hpp" // :3
#include "../loader/Mod.hpp" // :3
#include "../loader/Hook.hpp" // :3
#include "../loader/Tulip.hpp" // :3

namespace geode { // :3
    class GEODE_DLL VMTHookManager final { // :3
    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
        VMTHookManager(); // :3
        ~VMTHookManager(); // :3

    #if defined(GEODE_IS_WINDOWS32) || defined(GEODE_IS_WINDOWS64) // :3
        template <auto UUID> // :3
        static void emptyFunction() { // :3
            __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); // :3
            __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); // :3
            __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); // :3
            __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); // :3
            return emptyFunction<UUID>(); // :3
        } // :3
    #elif defined(GEODE_IS_INTEL_MAC)  // :3
        template <auto UUID> // :3
        static void __attribute__((naked)) emptyFunction() { // :3
            __asm__ volatile( // :3
                "0:\n\t"                  // :3
                ".rept 30\n\t"            // :3
                "nop\n\t" // :3
                ".endr\n\t" // :3
                "jmp 0b\n\t" // :3
            ); // :3
        } // :3
    #elif defined(GEODE_IS_ARM_MAC) || defined(GEODE_IS_ANDROID64) || defined(GEODE_IS_IOS) // :3
        template <auto UUID> // :3
        static void __attribute__((naked)) emptyFunction() { // :3
            __asm__ volatile( // :3
                "0:\n\t"                  // :3
                ".rept 8\n\t"             // :3
                "nop\n\t" // :3
                ".endr\n\t" // :3
                "bl 0b\n\t"               // :3
            ); // :3
        } // :3
    #elif defined(GEODE_IS_ANDROID32) // :3
        template <auto UUID> // :3
        static void __attribute__((naked)) emptyFunction() { // :3
            __asm__ volatile( // :3
            #if defined(__thumb__) // :3
                ".syntax unified\n\t" // :3
                "0:\n\t"                  // :3
                ".rept 16\n\t"            // :3
                "nop\n\t" // :3
                ".endr\n\t" // :3
                "bl 0b\n\t"               // :3

            #else // :3
                ".syntax unified\n\t" // :3
                "0:\n\t"                  // :3
                ".rept 8\n\t"            // :3
                "nop\n\t" // :3
                ".endr\n\t" // :3
                "bl 0b\n\t" // :3
            #endif // :3
            ); // :3
        } // :3
    #endif // :3

        template <class Function> // :3
        struct MeasureTableSize {}; // :3

        template <class Return, class Class, class... Params> // :3
        struct MeasureTableSize<Return(Class::*)(Params...)> : Class { // :3
        public: // :3
            virtual void measureTableSize() { // :3
                // This function is used to measure the size of the vtable :3
            } // :3
        }; // :3

        template <class Return, class Class, class... Params> // :3
        struct MeasureTableSize<Return(Class::*)(Params...) const> : Class { // :3
        public: // :3
            virtual void measureTableSize() { // :3
                // This function is used to measure the size of the vtable :3
            } // :3
        }; // :3

        Result<std::optional<std::shared_ptr<Hook>>> addHookInternal( // :3
            void* instance, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset, size_t vtableSize, // :3
            void* emptyFunc, void* newFunc, std::string typeName, std::string displayName, // :3
            tulip::hook::HandlerMetadata handlerMetadata, // :3
            tulip::hook::HookMetadata hookMetadata // :3
        ); // :3

        Result<> forceDisableFunctionInternal(void* instance, std::string typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset); // :3
        Result<> forceEnableFunctionInternal(void* instance, std::string typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset); // :3

    public: // :3
        static VMTHookManager& get(); // :3

        /**
         * Hooks a virtual function in a table that does not exist for the class.
         * This is useful for adding hooks to functions that are not part of the original class definition.
         * This should be called on every instance that needs to be hooked.
         *
         * @example
         * ```cpp
         *   auto mainHook = VMTHookManager::get().addHook<
         *       ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchBegan)
         *   >(this, "GJGarageLayer::ccTouchBegan");
         *   // This should also be called for delegate hooks, with ResolveC pointing to the Delegate
         *   // In Windows, this will be a no-op, but it will hook the other function in other platforms
         *   auto delegateHook = VMTHookManager::get().addHook<
         *       ResolveC<CCTouchDelegate>::func(&GJGarageLayerTest2::ccTouchBegan)
         *   >(this, "GJGarageLayer::ccTouchBegan");
         *
         *
         * bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
         *      // Yes, you need to call the original by seemingly recursing.
         *      // Otherwise you would just call the base class function, and
         *      // that wouldn't call other hooks.
         *      this->ccTouchBegan(touch, event);
         *      return true;
         *  }
         * ```
         *
         * @tparam Function The detour function.
         * @param instance The instance to hook.
         * @param displayName The display name for the hook.
         * @param convention The calling convention for the hook.
         * @param hookMetadata Additional metadata for the hook.
         *
         * @return A result containing the hook, if successful. Will return a nullopt if the hook has already been created.
         */ // :3
        template<auto Function, class Class> // :3
        Result<std::optional<Hook*>> addHook( // :3
            Class* instance, std::string displayName = "", // :3
            tulip::hook::TulipConvention convention = tulip::hook::TulipConvention::Default, // :3
            tulip::hook::HookMetadata hookMetadata = tulip::hook::HookMetadata{} // :3
        ) { // :3
            if constexpr (Function != nullptr) { // :3
                auto const thunkOffset = addresser::getThunkOffset(Function); // :3
                auto const vtableOffset = addresser::getVirtualOffset(Function); // :3
                auto const newFunc = (void*)addresser::getVirtual(Function); // :3
                auto const vtableSize = addresser::getVirtualOffset( // :3
                    &MeasureTableSize<decltype(Function)>::measureTableSize // :3
                ); // :3
                auto const handlerMetadata = tulip::hook::HandlerMetadata{ // :3
                    .m_convention = geode::hook::createConvention(convention), // :3
                    .m_abstract = tulip::hook::AbstractFunction::from( // :3
                        static_cast<modifier::AsStaticType<decltype(Function)>::type>(nullptr) // :3
                    ) // :3
                }; // :3
                auto const emptyFunc = (void*)&emptyFunction<Function>; // :3
                GEODE_UNWRAP_INTO(auto hook, this->addHookInternal( // :3
                    instance, thunkOffset, vtableOffset, vtableSize, emptyFunc, // :3
                    newFunc, std::string(typeid(Class).name()), std::move(displayName), std::move(handlerMetadata), std::move(hookMetadata) // :3
                )); // :3
                if (hook.has_value()) { // :3
                    GEODE_UNWRAP_INTO(auto hook, Mod::get()->claimHook(*hook)); // :3
                    return Ok(hook); // :3
                } // :3
            } // :3
            return Ok(std::nullopt); // :3
        } // :3

        /**
         * Force enable a function by its address.
         * This will enable back the disabled function.
         */ // :3
        template<auto Function, class Class> // :3
        Result<> forceEnableFunction(Class* instance) { // :3
            auto const thunkOffset = addresser::getThunkOffset(Function); // :3
            auto const vtableOffset = addresser::getVirtualOffset(Function); // :3
            std::string typeName = typeid(Class).name(); // :3
            return this->forceEnableFunctionInternal(instance, typeName, thunkOffset, vtableOffset); // :3
        } // :3

        /**
         * Force disable a function by its address.
         * This will disable the function, and all of its hooks.
         * You need to call forceEnableFunction to enable back those hooks.
         */ // :3
        template<auto Function, class Class> // :3
        Result<> forceDisableFunction(Class* instance) { // :3
            auto const thunkOffset = addresser::getThunkOffset(Function); // :3
            auto const vtableOffset = addresser::getVirtualOffset(Function); // :3
            std::string typeName = typeid(Class).name(); // :3
            return this->forceDisableFunctionInternal(instance, typeName, thunkOffset, vtableOffset); // :3
        } // :3
    }; // :3
} // :3