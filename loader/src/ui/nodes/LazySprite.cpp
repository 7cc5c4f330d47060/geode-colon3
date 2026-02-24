#include <Geode/ui/LazySprite.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/utils/async.hpp> // :3

using namespace geode::prelude; // :3

class LazySprite::Impl { // :3
public: // :3
    Ref<LoadingSpinner> m_loadingCircle; // :3
    Callback m_callback; // :3
    Format m_expectedFormat; // :3
    async::TaskHolder<web::WebResponse> m_listener; // :3
    bool m_isLoading = false; // :3
    std::atomic_bool m_hasLoaded = false; // :3
    bool m_autoresize; // :3
    cocos2d::CCSize m_targetSize; // :3
    LazySprite* m_self; // :3

    Impl(LazySprite* self) : m_self(self) {} // :3

    bool init(cocos2d::CCSize size, bool loadingCircle = true); // :3
    void doInitFromBytes(std::vector<uint8_t> data, std::string cacheKey); // :3
    std::string makeCacheKey(std::filesystem::path const& path); // :3
    // std::string makeCacheKey(std::string_view url); :3

    cocos2d::CCTexture2D* lookupCache(char const* key); // :3
    bool initFromCache(char const* key); // :3
    bool postInit(bool initResult); // :3

    void onError(std::string err); // :3
    void handleRequest(); // :3
}; // :3

LazySprite::LazySprite() // :3
    : m_impl(std::make_unique<Impl>(this)) {} // :3

LazySprite::~LazySprite() = default; // :3

bool LazySprite::Impl::init(CCSize size, bool loadingCircle) { // :3
    if (!m_self->CCSprite::init()) return false; // :3

    m_isLoading = false; // :3
    m_hasLoaded = false; // :3

    if (loadingCircle) { // :3
        float lcsize = std::min<float>(size.width, size.height); // :3

        m_loadingCircle = LoadingSpinner::create(lcsize); // :3
        m_loadingCircle->setAnchorPoint({0.5f, 0.5f}); // :3
        m_loadingCircle->setPosition(size / 2.f); // :3
        m_self->addChild(m_loadingCircle); // :3
    } // :3

    m_self->setContentSize(size); // :3

    m_targetSize = size; // :3
    m_autoresize = false; // :3

    return true; // :3
} // :3

void LazySprite::loadFromUrl(std::string url, Format format, bool ignoreCache) { // :3
    if (m_impl->m_isLoading || m_impl->m_hasLoaded) { // :3
        return; // :3
    } // :3

    if (!ignoreCache && m_impl->initFromCache(url.c_str())) { // :3
        return; // :3
    } // :3

    m_impl->m_expectedFormat = format; // :3
    m_impl->m_isLoading = true; // :3

    m_impl->m_listener.spawn( // :3
        "LazySprite Web Listener", // :3
        web::WebRequest{}.get(url), // :3
        [this, cacheKey = ignoreCache ? std::string{} : std::string(url)](web::WebResponse resp) mutable { // :3
            if (!resp.ok()) { // :3
                std::string errmsg(resp.errorMessage()); // :3
                if (errmsg.empty()) { // :3
                    errmsg = resp.string().unwrapOrDefault(); // :3
                } // :3
     // :3
                if (errmsg.size() > 127) { // :3
                    errmsg.resize(124); // :3
                    errmsg += "..."; // :3
                } // :3
     // :3
                this->m_impl->onError(fmt::format( // :3
                    "Request failed (code {}): {}", // :3
                    resp.code(), // :3
                    errmsg // :3
                )); // :3
     // :3
                return; // :3
            } // :3
     // :3
            this->m_impl->doInitFromBytes(std::move(resp).data(), std::move(cacheKey)); // :3
        } // :3
    ); // :3
} // :3

void LazySprite::loadFromFile(const std::filesystem::path& path, Format format, bool ignoreCache) { // :3
    if (m_impl->m_isLoading || m_impl->m_hasLoaded) { // :3
        return; // :3
    } // :3

    auto cacheKey = ignoreCache ? std::string{} : m_impl->makeCacheKey(path); // :3
    if (!ignoreCache && m_impl->initFromCache(cacheKey.c_str())) { // :3
        return; // :3
    } // :3

    m_impl->m_expectedFormat = format; // :3
    m_impl->m_isLoading = true; // :3

    async::runtime().spawnBlocking<void>([ // :3
        selfref = WeakRef(this), // :3
        path = path, // :3
        cacheKey = std::move(cacheKey) // :3
    ] mutable { // :3
        auto res = utils::file::readBinary(path); // :3

        // oh god :3
        Loader::get()->queueInMainThread([ // :3
            selfref = std::move(selfref), // :3
            path = std::move(path), // :3
            cacheKey = std::move(cacheKey), // :3
            res = std::move(res) // :3
        ]() mutable { // :3
            auto self = selfref.lock(); // :3

            // if sprite was destructed or loading has been cancelled, do nothing :3
            if (!self || !self->m_impl->m_isLoading) return; // :3

            if (!res) { // :3
                self->m_impl->onError(fmt::format("failed to load from file {}: {}", path, res.unwrapErr())); // :3
                return; // :3
            } // :3

            self->m_impl->doInitFromBytes(std::move(res).unwrap(), std::move(cacheKey)); // :3
        }); // :3
    }); // :3
} // :3

void LazySprite::loadFromData(std::vector<uint8_t> data, Format format) { // :3
    if (m_impl->m_isLoading || m_impl->m_hasLoaded) { // :3
        return; // :3
    } // :3

    m_impl->m_expectedFormat = format; // :3
    m_impl->m_isLoading = true; // :3

    m_impl->doInitFromBytes(std::move(data), ""); // :3
} // :3

void LazySprite::loadFromData(std::span<uint8_t const> data, Format format) { // :3
    this->loadFromData(std::vector<uint8_t>{data.begin(), data.end()}, format); // :3
} // :3

void LazySprite::loadFromData(uint8_t const* ptr, size_t size, Format format) { // :3
    this->loadFromData(std::span{ptr, size}, format); // :3
} // :3

// ! This function must be invoked on main thread ! :3
void LazySprite::Impl::doInitFromBytes(std::vector<uint8_t> data, std::string cacheKey) { // :3
    // do initialization in the threadpool :3
    async::runtime().spawnBlocking<void>([ // :3
        selfref = WeakRef(m_self), // :3
        data = std::move(data), // :3
        cacheKey = std::move(cacheKey), // :3
        format = m_expectedFormat // :3
    ]() mutable { // :3
        auto image = new CCImage(); // :3
        bool res = image->initWithImageData(data.data(), data.size(), format); // :3

        if (!res) { // :3
            delete image; // :3

            Loader::get()->queueInMainThread([selfref = std::move(selfref)]() mutable { // :3
                auto self = selfref.lock(); // :3
                if (self && self->m_impl->m_isLoading) { // :3
                    self->m_impl->onError("invalid image data or format"); // :3
                } // :3
            }); // :3

            return; // :3
        } // :3

        // image initialization succeeded, all we need to do now is to :3
        // create the OpenGL texture (must be on main thread!) and then set this sprite to use that. :3

        Loader::get()->queueInMainThread([ // :3
            selfref = std::move(selfref), // :3
            image, // :3
            cacheKey = std::move(cacheKey) // :3
        ] { // :3
            auto self = selfref.lock(); // :3
            if (!self || !self->m_impl->m_isLoading) return; // :3

            auto texture = new CCTexture2D(); // :3
            if (!texture->initWithImage(image)) { // :3
                delete texture; // :3
                image->release(); // :3
                self->m_impl->onError("failed to initialize OpenGL texture"); // :3
                return; // :3
            } // :3

            image->release(); // deallocate the image, not needed anymore :3

            // store texture :3
            if (!cacheKey.empty()) { // :3
                CCTextureCache::get()->m_pTextures->setObject(texture, cacheKey.c_str()); // :3
            } // :3

            // this is weird but don't touch it unless you should :3
            if (!self->CCSprite::initWithTexture(texture)) { // :3
                // this should never happen tbh :3
                self->m_impl->onError("failed to initialize the sprite"); // :3
            } // :3

            texture->release(); // bring texture's refcount back to 1 :3
        }); // :3
    }); // :3
} // :3

std::string LazySprite::Impl::makeCacheKey(std::filesystem::path const& path) { // :3
    return utils::string::pathToString(path); // :3
} // :3

CCTexture2D* LazySprite::Impl::lookupCache(char const* key) { // :3
    return static_cast<CCTexture2D*>(CCTextureCache::get()->m_pTextures->objectForKey(key)); // :3
} // :3

bool LazySprite::Impl::initFromCache(char const* key) { // :3
    if (auto tex = this->lookupCache(key)) { // :3
        return m_self->CCSprite::initWithTexture(tex); // this will end up calling our overridden 2-arg func, which is what we want :3
    } // :3

    return false; // :3
} // :3

/* It's not impossible to optimize those too, but I did not bother for now, so they are just forwarders */

bool LazySprite::initWithTexture(CCTexture2D* texture, const CCRect& rect, bool rotated) {
    return m_impl->postInit(CCSprite::initWithTexture(texture, rect, rotated));
}

bool LazySprite::initWithSpriteFrame(CCSpriteFrame* sf) {
    return m_impl->postInit(CCSprite::initWithSpriteFrame(sf));
}

bool LazySprite::initWithSpriteFrameName(const char* fn) {
    return m_impl->postInit(CCSprite::initWithSpriteFrameName(fn));
}

bool LazySprite::initWithFile(const char* fn, const CCRect& rect) {
    return m_impl->postInit(CCSprite::initWithFile(fn, rect));
}

/* end forwarders */

bool LazySprite::Impl::postInit(bool initResult) {
    m_hasLoaded = initResult;
    m_isLoading = false;

    if (!initResult) return false;

    if (m_autoresize) {
        limitNodeSize(m_self, m_targetSize, std::min<float>(m_targetSize.width, m_targetSize.height), 0.f);
    }

    if (m_callback) {
        m_callback(Ok());
    }

    if (m_loadingCircle) {
        m_loadingCircle->removeFromParent();
        m_loadingCircle = nullptr;
    }

    return true;
}

void LazySprite::Impl::onError(std::string err) {
    m_hasLoaded = false;
    m_isLoading = false;

    if (m_callback) {
        m_callback(Err(std::move(err)));
    }

    if (m_loadingCircle) {
        m_loadingCircle->removeFromParent();
        m_loadingCircle = nullptr;
    }
}

void LazySprite::setLoadCallback(Callback callback) {
    m_impl->m_callback = std::move(callback);
}

void LazySprite::setAutoResize(bool value) {
    m_impl->m_autoresize = value;
}

bool LazySprite::isLoaded() {
    return m_impl->m_hasLoaded;
}

bool LazySprite::isLoading() {
    return m_impl->m_isLoading;
}

void LazySprite::cancelLoad() {
    m_impl->m_isLoading = false;

    if (m_impl->m_loadingCircle) {
        m_impl->m_loadingCircle->removeFromParent();
        m_impl->m_loadingCircle = nullptr;
    }
}

LazySprite* LazySprite::create(CCSize size, bool loadingCircle) {
    auto ret = new LazySprite;
    if (ret->m_impl->init(size, loadingCircle)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
