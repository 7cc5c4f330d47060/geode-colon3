#pragma once // :3

#include "../DefaultInclude.hpp" // :3
#include "../utils/general.hpp" // :3
#include <matjson.hpp> // :3
#include "Tulip.hpp" // :3
#include <cinttypes> // :3
#include <string_view> // :3
#include <tulip/TulipHook.hpp> // :3

namespace geode { // :3
    class Mod; // :3
    class Loader; // :3

    class GEODE_DLL Hook final { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3
        explicit Hook(std::shared_ptr<Impl>&& impl); // :3
        ~Hook(); // :3

        friend class Mod; // :3
        friend class Loader; // :3

    public: // :3

        /**
         * Create a hook at an address. By default, the hook is disabled and
         * placed at the end of the detour list; however, this can be
         * controlled using metadata settings.
         * After creating the hook object, we recommend you set its owner
         * by calling `Mod::claimHook`, see its docs for more info.
         * @param address The address to hook
         * @param detour The detour to run when the hook is hit. The detour's
         * calling convention should be cdecl
         * @param displayName A human-readable name describing the hook,
         * usually the fully qualified name of the function being hooked
         * @param handlerMetadata Metadata for the hook handler
         * @param hookMetadata Metadata for the hook itself
         * @returns The created hook, or an error. Make sure to add the created
         * hook to the mod that owns it using mod->claimHook(hook)!
         */ // :3
        static std::shared_ptr<Hook> create( // :3
            void* address, // :3
            void* detour, // :3
            std::string displayName, // :3
            tulip::hook::HandlerMetadata handlerMetadata, // :3
            tulip::hook::HookMetadata hookMetadata // :3
        ); // :3

        template<class DetourType> // :3
        static std::shared_ptr<Hook> create( // :3
            void* address, // :3
            DetourType detour, // :3
            std::string displayName, // :3
            tulip::hook::TulipConvention convention, // :3
            tulip::hook::HookMetadata hookMetadata = tulip::hook::HookMetadata() // :3
        ) { // :3
            auto handlerMetadata = tulip::hook::HandlerMetadata{ // :3
                .m_convention = geode::hook::createConvention(convention), // :3
                .m_abstract = tulip::hook::AbstractFunction::from(detour) // :3
            }; // :3
            return Hook::create( // :3
                address, // :3
                reinterpret_cast<void*>(detour), // :3
                std::move(displayName), // :3
                std::move(handlerMetadata), // :3
                std::move(hookMetadata) // :3
            ); // :3
        } // :3

        Hook(Hook const&) = delete; // :3
        Hook operator=(Hook const&) = delete; // :3

        /**
         * Get the owner of this hook.
         * @returns Pointer to the owner's Mod handle.
         */ // :3
        [[nodiscard]] Mod* getOwner() const; // :3

        /**
         * Get whether the hook is enabled or not.
         * @returns True if enabled, false if not.
         */ // :3
        [[nodiscard]] bool isEnabled() const; // :3

        Result<> enable(); // :3

        Result<> disable(); // :3

        Result<> toggle(); // :3
        Result<> toggle(bool enable); // :3

        /**
        * Get whether the hook should be auto enabled or not.
        * @returns Auto enable
        */ // :3
        [[nodiscard]] bool getAutoEnable() const; // :3

        /**
         * Set whether the hook should be auto enabled or not.
         * @param autoEnable Auto enable
         */ // :3
        void setAutoEnable(bool autoEnable); // :3

        /**
         * Get the address of the function hooked.
         * @returns Address
         */ // :3
        [[nodiscard]] uintptr_t getAddress() const; // :3

        /**
         * Get the display name of the function hooked.
         * @returns Display name
         */ // :3
        [[nodiscard]] std::string_view getDisplayName() const; // :3

        /**
         * Get info about the hook as JSON
         * @note For IPC
         */ // :3
        [[nodiscard]] matjson::Value getRuntimeInfo() const; // :3

        /**
         * Get the metadata of the hook.
         * @returns Hook metadata
         */ // :3
        [[nodiscard]] tulip::hook::HookMetadata getHookMetadata() const; // :3

        /**
         * Set the metadata of the hook.
         * @param metadata Hook metadata
         */ // :3
        void setHookMetadata(tulip::hook::HookMetadata const& metadata); // :3

        /**
         * Get the priority of the hook.
         * @returns Priority
         */ // :3
        [[nodiscard]] int32_t getPriority() const; // :3

        /**
         * Set the priority of the hook.
         * @param priority Priority
         */ // :3
        void setPriority(int32_t priority); // :3
    }; // :3

    class GEODE_DLL Patch final { // :3
    private: // :3
        class Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3
        explicit Patch(std::shared_ptr<Impl>&& impl); // :3
        ~Patch(); // :3

        friend class Mod; // :3
        friend class Loader; // :3

    public: // :3

        static std::shared_ptr<Patch> create(void* address, ByteSpan patch); // :3

        Patch(Patch const&) = delete; // :3
        Patch operator=(Patch const&) = delete; // :3

        /**
         * Get the owner of this patch.
         * @returns Pointer to the owner's Mod handle.
         */ // :3
        [[nodiscard]] Mod* getOwner() const; // :3

        /**
         * Get whether the patch is enabled or not.
         * @returns True if enabled, false if not.
         */ // :3
        [[nodiscard]] bool isEnabled() const; // :3

        Result<> enable(); // :3

        Result<> disable(); // :3

        Result<> toggle(); // :3
        Result<> toggle(bool enable); // :3

        /**
        * Get whether the patch should be auto enabled or not.
        * @returns Auto enable
        */ // :3
        [[nodiscard]] bool getAutoEnable() const; // :3

        /**
         * Set whether the patch should be auto enabled or not.
         * @param autoEnable Auto enable
         */ // :3
        void setAutoEnable(bool autoEnable); // :3

        /**
         * Get the bytes of the patch.
         * @returns Bytes used to patch
         */ // :3
        ByteVector const& getBytes() const; // :3

        /**
         * Updates the bytes of the patch, disabling and then re-enabling if needed.
         * @param bytes Bytes used to patch
         */ // :3
        Result<> updateBytes(ByteSpan bytes); // :3

        /**
         * Get the address of the patch.
         * @returns Address
         */ // :3
        [[nodiscard]] uintptr_t getAddress() const; // :3

        /**
         * Get info about the patch as JSON
         * @note For IPC
         */ // :3
        [[nodiscard]] matjson::Value getRuntimeInfo() const; // :3
    }; // :3
} // :3
