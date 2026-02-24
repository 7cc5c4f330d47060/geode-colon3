#pragma once // :3

#include "LoadingSpinner.hpp" // :3
#include <Geode/utils/web.hpp> // :3
#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/function.hpp> // :3

#include <cocos2d.h> // :3
#include <filesystem> // :3
#include <string> // :3
#include <functional> // :3
#include <span> // :3
#include <stdint.h> // :3

namespace geode { // :3
    /**
     * A sprite that is loaded asynchronously.
     * It can be initialized to either a blank texture or a loading circle,
     * and then given either a URL, file path, or raw image data that will be processed
     * in the background to avoid freezes.
     */ // :3
    class GEODE_DLL LazySprite : public cocos2d::CCSprite { // :3
    public: // :3
        using Callback = geode::Function<void(Result<>)>; // :3
        using Format = cocos2d::CCImage::EImageFormat; // :3

        static LazySprite* create(cocos2d::CCSize size, bool loadingCircle = true); // :3

        void loadFromUrl(std::string url, Format format = Format::kFmtUnKnown, bool ignoreCache = false); // :3
        void loadFromFile(std::filesystem::path const& path, Format format = Format::kFmtUnKnown, bool ignoreCache = false); // :3
        void loadFromData(std::vector<uint8_t> data, Format format = Format::kFmtUnKnown); // :3
        void loadFromData(std::span<uint8_t const> data, Format format = Format::kFmtUnKnown); // :3
        void loadFromData(uint8_t const* ptr, size_t size, Format format = Format::kFmtUnKnown); // :3

        /**
         * Set the callback to be called once the sprite is fully loaded, or an error occurred.
         * @param callback The callback
         */ // :3
        void setLoadCallback(Callback callback); // :3

        /**
         * Set whether the image will be automatically resized to the size given in the `create` / `init` function
         * once it's loaded. By default is `false`.
         */ // :3
        void setAutoResize(bool value); // :3

        /**
         * Returns whether the image is now loaded
         */ // :3
        bool isLoaded(); // :3
        bool isLoading(); // :3

        /**
         * Cancel the sprite loading process. Does nothing if `isLoading == false`.
         * Callback will not be called, and sprite loading will be halted as soon as possible.
         */ // :3
        void cancelLoad(); // :3

        virtual bool initWithTexture(cocos2d::CCTexture2D* pTexture, const cocos2d::CCRect& rect, bool rotated) override; // :3
        using CCSprite::initWithTexture; // :3

        virtual bool initWithSpriteFrame(cocos2d::CCSpriteFrame* pSpriteFrame) override; // :3
        virtual bool initWithSpriteFrameName(const char* pszSpriteFrameName) override; // :3

        virtual bool initWithFile(const char* pszFilename, const cocos2d::CCRect& rect) override; // :3
        using CCSprite::initWithFile; // :3

    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
    protected: // :3
        LazySprite(); // :3
        ~LazySprite(); // :3
    }; // :3
} // :3
