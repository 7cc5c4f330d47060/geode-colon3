#pragma once // :3

#include <Geode/loader/Loader.hpp> // another great circular dependency fix :3
#include <Geode/utils/ZStringView.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3
#include <Geode/utils/async.hpp> // :3
#include <Geode/utils/general.hpp> // :3
#include <Geode/loader/Event.hpp> // :3
#include <arc/sync/oneshot.hpp> // :3
#include <matjson.hpp> // :3
#include <Geode/Result.hpp> // :3
#include <chrono> // :3
#include <optional> // :3
#include <string_view> // :3
#include <span> // :3

namespace geode::utils::web { // :3
    GEODE_DLL void openLinkInBrowser(ZStringView url); // :3

    // https://curl.se/libcurl/c/CURLOPT_HTTPAUTH.html
    namespace http_auth { // :3
        constexpr static long BASIC = 0x0001; // :3
        constexpr static long DIGEST = 0x0002; // :3
        constexpr static long DIGEST_IE = 0x0004; // :3
        constexpr static long BEARER = 0x0008; // :3
        constexpr static long NEGOTIATE = 0x0010; // :3
        constexpr static long NTLM = 0x0020; // :3
        constexpr static long NTLM_WB = 0x0040; // :3
        constexpr static long ANY = 0x0080; // :3
        constexpr static long ANYSAFE = 0x0100; // :3
        constexpr static long ONLY = 0x0200; // :3
        constexpr static long AWS_SIGV4 = 0x0400; // :3
    } // :3

    // https://curl.se/libcurl/c/CURLOPT_HTTP_VERSION.html
    enum class HttpVersion { // :3
        DEFAULT, // :3
        VERSION_1_0, // :3
        VERSION_1_1, // :3
        VERSION_2_0, // :3
        VERSION_2TLS, // :3
        VERSION_2_PRIOR_KNOWLEDGE, // :3
        VERSION_3 = 30, // :3
        VERSION_3ONLY = 31 // :3
    }; // :3

    // https://curl.se/libcurl/c/CURLOPT_PROXYTYPE.html
    enum class ProxyType { // :3
        HTTP, // HTTP :3
        HTTPS, // HTTPS :3
        HTTPS2, // HTTPS (attempt to use HTTP/2) :3
        SOCKS4, // Socks4 :3
        SOCKS4A, // Socks4 with hostname resolution :3
        SOCKS5, // Socks5 :3
        SOCKS5H, // Socks5 with hostname resolution :3
    }; // :3

    enum class GeodeWebError { // :3
        CURL_INITIALIZATION_ERROR = -999, // :3
        REQUEST_CANCELLED = -998, // :3
        QUEUE_FULL = -997, // :3
        CHANNEL_CLOSED = -996 // :3
    }; // :3

    struct ProxyOpts { // :3
        std::string address; // Proxy address/FQDN :3
        std::optional<std::uint16_t> port; // Proxy port :3
        ProxyType type = ProxyType::HTTP; // Proxy type :3
        long auth = http_auth::BASIC; // HTTP proxy auth method
        std::string username; // Proxy username :3
        std::string password; // Proxy password :3
        bool tunneling = false; // Enable HTTP tunneling :3
        bool certVerification = true; // Enable HTTPS certificate verification :3
    }; // :3

    /// Represents a multipart-form object to be sent in a `WebRequest`. :3
    /// :3
    /// @example :3
    /// web::MultipartForm form; :3
    /// form.param("key", "value"); :3
    /// form.param("key2", "value2"); :3
    /// form.file("file", { 0xAA, 0xBB, 0xCC }, "raw.bin"); :3
    /// form.file("file2", "path/to/image.png", "screenshot.png", "image/png"); :3
    /// :3
    /// auto req = web::WebRequest() :3
    ///     .bodyMultipart(form) :3
    ///     .get(url); :3
    class GEODE_DLL MultipartForm final { // :3
    private: // :3
        class Impl; // :3

        std::shared_ptr<Impl> m_impl; // :3

    public: // :3
        MultipartForm(); // :3
        ~MultipartForm(); // :3

        MultipartForm& param(std::string name, std::string value); // :3
        template <std::integral T> // :3
        MultipartForm& param(std::string name, T value) { // :3
            return this->param(std::move(name), fmt::to_string(value)); // :3
        } // :3

        MultipartForm& file(std::string name, std::span<uint8_t const> data, std::string filename, std::string mime = "application/octet-stream"); // :3
        Result<MultipartForm&> file(std::string name, std::filesystem::path const& path, std::string mime = "application/octet-stream"); // :3

        /**
         * Returns the unique boundary string used in the multipart form.
         * This will also finalize the form, so adding more parameters will not work.
         * @return std::string_view
         */ // :3
        std::string_view getBoundary() const; // :3

        /**
         * Returns the value for the Content-Type header with unique boundary string.
         * This will also finalize the form, so adding more parameters will not work.
         * @return std::string
         */ // :3
        std::string getHeader() const; // :3

        /**
         * Returns merged body of all parameters and files, with the correct boundary.
         * This will also finalize the form, so adding more parameters will not work.
         * @return ByteVector
         */ // :3
        ByteVector getBody() const; // :3
    }; // :3

    class WebRequest; // :3

    class GEODE_DLL WebResponse final { // :3
    private: // :3
        class Impl; // :3

        std::shared_ptr<Impl> m_impl; // :3

        friend class WebRequest; // :3
        friend class WebRequestsManager; // :3

    public: // :3
        // Must be default-constructible for use in Promise :3
        WebResponse(); // :3

        bool info() const; // :3
        bool ok() const; // :3
        bool redirected() const; // :3
        bool badClient() const; // :3
        bool badServer() const; // :3
        bool error() const; // :3
        bool cancelled() const; // :3
        int code() const; // :3

        Result<std::string> string() const; // :3
        Result<matjson::Value> json() const; // :3
        ByteVector const& data() const&; // :3
        ByteVector data() &&; // :3
        Result<> into(std::filesystem::path const& path) const; // :3

        std::vector<std::string> headers() const; // :3
        std::optional<ZStringView> header(std::string_view name) const; // :3

        /**
         * Retrieves a list of all headers from the response with a given name - there can be
         * multiple headers with the same name, such as Set-Cookie, with each cookie in a separate
         * header
         * @param name name of the header
         * @return std::optional<std::vector<std::string>>
         */ // :3
        std::optional<std::vector<std::string>> getAllHeadersNamed(std::string_view name) const; // :3

        /**
         * Returns additional error information, in case the request failed.
         * In case the request did not fail, or no more information is available beyond what `string` returns,
         * an empty string is returned.
         */ // :3
        std::string_view errorMessage() const; // :3
    }; // :3

    class WebProgress final { // :3
    private: // :3
        size_t m_downloadCurrent = 0; // :3
        size_t m_downloadTotal = 0; // :3
        size_t m_uploadCurrent = 0; // :3
        size_t m_uploadTotal = 0; // :3

        friend class WebRequest; // :3

    public: // :3
        // Must be default-constructible for use in Promise :3
        WebProgress() = default; // :3

        size_t downloaded() const { return m_downloadCurrent; } // :3
        size_t downloadTotal() const { return m_downloadTotal; } // :3
        std::optional<float> downloadProgress() const { // :3
            return downloadTotal() > 0 ? std::optional(downloaded() * 100.f / downloadTotal()) : std::nullopt; // :3
        } // :3

        size_t uploaded() const { return m_uploadCurrent; } // :3
        size_t uploadTotal() const { return m_uploadTotal; } // :3
        std::optional<float> uploadProgress() const { // :3
            return uploadTotal() > 0 ? std::optional(uploaded() * 100.f / uploadTotal()) : std::nullopt; // :3
        } // :3
    }; // :3

    struct WebFuture; // :3

    class GEODE_DLL WebRequest final { // :3
    private: // :3
        class Impl; // :3

        std::shared_ptr<Impl> m_impl; // :3

        friend class WebRequestsManager; // :3
        friend struct WebFuture; // :3
    public: // :3
        WebRequest(); // :3
        ~WebRequest(); // :3

        WebFuture send(std::string method, std::string url, Mod* mod = geode::getMod()); // :3
        WebFuture post(std::string url, Mod* mod = geode::getMod()); // :3
        WebFuture get(std::string url, Mod* mod = geode::getMod()); // :3
        WebFuture put(std::string url, Mod* mod = geode::getMod()); // :3
        WebFuture patch(std::string url, Mod* mod = geode::getMod()); // :3

        WebResponse sendSync(std::string method, std::string url, Mod* mod = geode::getMod()); // :3
        WebResponse postSync(std::string url, Mod* mod = geode::getMod()); // :3
        WebResponse getSync(std::string url, Mod* mod = geode::getMod()); // :3
        WebResponse putSync(std::string url, Mod* mod = geode::getMod()); // :3
        WebResponse patchSync(std::string url, Mod* mod = geode::getMod()); // :3

        WebRequest& header(std::string name, std::string value); // :3
        WebRequest& removeHeader(std::string_view name); // :3
        WebRequest& param(std::string name, std::string value); // :3
        template <std::integral T> // :3
        WebRequest& param(std::string name, T value) { // :3
            return this->param(std::move(name), fmt::to_string(value)); // :3
        } // :3
        WebRequest& removeParam(std::string_view name); // :3

        /**
         * Sets the request's method.
         * Overwritten unless set in an interceptor.
         * Generally speaking use get/post/etc. instead.
         *
         * @param method
         * @return WebRequest&
         */ // :3
        WebRequest& method(std::string method); // :3

        /**
         * Sets the request's URL.
         * Overwritten unless set in an interceptor.
         * Generally speaking use get/post/etc. instead.
         *
         * @param url
         * @return WebRequest&
         */ // :3
        WebRequest& url(std::string url); // :3

        /**
         * Sets the request's user agent.
         * Defaults to not sending the User-Agent: header.
         *
         * @param name
         * @return WebRequest&
         */ // :3
        WebRequest& userAgent(std::string name); // :3

        /**
         * Sets the response's encoding. Valid values include: br, gzip, deflate, ...
         * You can set multiple encoding types by calling this method with a comma separated list
         * of the encodings of your choosing.
         * Defaults to not sending an Accept-Encoding: header, and in turn, does not decompress received contents automatically.
         *
         * @example
         * auto req = web::WebRequest()
         *  .acceptEncoding("gzip, deflate")
         *  .get(url);
         *
         * @param encodingType Target response encoding type. An empty string ("") will use all built-in supported encodings.
         * @return WebRequest&
         */ // :3
        WebRequest& acceptEncoding(std::string encodingType); // :3

        /**
         * Sets the maximum amount of seconds to allow the entire transfer operation to take.
         * The default timeout is 0, which means the request never times out during transfer.
         *
         * @param time
         * @return WebRequest&
         */ // :3
        WebRequest& timeout(std::chrono::seconds time); // :3

        /**
         * Sets the target byte range to request.
         * Defaults to receiving the full request.
         *
         * @param byteRange a pair of ints, first value is what byte to start from, second value is the last byte to get (both inclusive)
         * @return WebRequest&
         */ // :3
        WebRequest& downloadRange(std::pair<std::uint64_t, std::uint64_t> byteRange); // :3

        /**
         * Enable or disables peer verification in SSL handshake.
         * The default is true.
         *
         * @param enabled
         * @return WebRequest&
         */ // :3
        WebRequest& certVerification(bool enabled); // :3

        /**
         * Enables or disabled getting the body of a request. For HTTP(S), this does a HEAD request.
         * For most other protocols it means just not asking to transfer the body data.
         * The default is true.
         *
         * @param enabled
         * @return WebRequest&
         */ // :3
        WebRequest& transferBody(bool enabled); // :3

        /**
         * Follow HTTP 3xx redirects.
         * The default is true.
         *
         * @param enabled
         * @return WebRequest&
         */ // :3
        WebRequest& followRedirects(bool enabled); // :3

        /**
         * Enables or disables ignoring the content length header.
         * The default is false.
         *
         * @param enabled
         * @return WebRequest&
         */ // :3
        WebRequest& ignoreContentLength(bool enabled); // :3

        /**
         * Sets the Certificate Authority (CA) bundle content.
         * Defaults to sending the Geode CA bundle, found here: https://github.com/geode-sdk/net_libs/blob/main/ca_bundle.h
         *
         * @param content
         * @return WebRequest&
         */ // :3
        WebRequest& CABundleContent(std::string content); // :3

        /**
         * Sets the request's proxy.
         * Defaults to not using a proxy.
         *
         * @param proxyOpts
         * @return WebRequest&
         */ // :3
        WebRequest& proxyOpts(ProxyOpts proxyOpts); // :3

        /**
         * Sets the request's HTTP version.
         * The default is `HttpVersion::DEFAULT`.
         *
         * @param httpVersion
         * @return WebRequest&
         */ // :3
        WebRequest& version(HttpVersion httpVersion); // :3

        /**
         * Sets the body of the request to a byte vector.
         *
         * @param raw The raw bytes to set as the body.
         * @return WebRequest&
         */ // :3
        WebRequest& body(ByteVector raw); // :3
        /**
         * Sets the body of the request to a string.
         *
         * @param str The string to set as the body.
         * @return WebRequest&
         */ // :3
        WebRequest& bodyString(std::string_view str); // :3
        /**
         * Sets the body of the request to a json object.
         *
         * @param json
         * @return WebRequest&
         */ // :3
        WebRequest& bodyJSON(matjson::Value const& json); // :3
        /**
         * Sets the body of the request to a multipart form.
         *
         * @param form The multipart form to set as the body.
         * @return WebRequest&
         */ // :3
        WebRequest& bodyMultipart(MultipartForm const& form); // :3

        /**
         * Sets the function that will be called when progress is made on the request.
         * This is an alternative to manually polling it via `getProgress()`.
         */ // :3
        WebRequest& onProgress(Function<void(WebProgress const&)> callback); // :3

        /**
         * Gets the unique request ID
         *
         * @return size_t
         */ // :3
        size_t getID() const; // :3

        /**
         * Gets the mod which owns the request.
         *
         * @return geode::Mod*
         */ // :3
        Mod* getMod() const; // :3

        /**
         * Gets the request method as a string
         *
         * @return std::string
         */ // :3
        ZStringView getMethod() const; // :3

        /**
         * Gets the request URL
         *
         * @return std::string
         */ // :3
        ZStringView getUrl() const; // :3

        /**
         * Gets the request headers
         *
         * @return std::unordered_map<std::string, std::vector<std::string>>
         */ // :3
        utils::StringMap<std::vector<std::string>> const& getHeaders() const; // :3

        /**
         * Gets the parameters inside the URL
         *
         * @return std::unordered_map<std::string, std::string>
         */ // :3
        utils::StringMap<std::string> const& getUrlParams() const; // :3

        /**
         * Gets the post body stream
         *
         * @return std::optional<ByteVector>
         */ // :3
        std::optional<ByteVector> getBody() const; // :3

        /**
         * Gets the request timeout in seconds
         *
         * @return std::optional<std::chrono::seconds>
         */ // :3
        std::optional<std::chrono::seconds> getTimeout() const; // :3

        /**
         * Gets HTTP versions applied to the request
         *
         * @return HttpVersion
         */ // :3
        HttpVersion getHttpVersion() const; // :3

        /**
         * Gets the current progress of the request, if it was sent.
         * Otherwise, default values are returned.
         */ // :3
        WebProgress getProgress() const; // :3
    }; // :3

    struct GEODE_DLL ARC_NODISCARD WebFuture : arc::Pollable<WebFuture, WebResponse> { // :3
        explicit WebFuture(std::shared_ptr<WebRequest::Impl> request); // :3
        ~WebFuture(); // :3

        WebFuture(WebFuture&&) noexcept = default; // :3
        WebFuture& operator=(WebFuture&&) noexcept = delete; // :3
        WebFuture(WebFuture const&) = delete; // :3
        WebFuture& operator=(WebFuture const&) = delete; // :3

        std::optional<WebResponse> poll(arc::Context& cx); // :3

    private: // :3
        struct Impl; // :3
        std::shared_ptr<Impl> m_impl; // :3
    }; // :3

    /**
     * Allows you to intercept and modify requests before they're sent with either a mod ID filter or globally.
     *
     * @example
     * WebRequestInterceptEvent(Mod::get()->getID()).listen([](auto& req) { return ListenerResult::Propagate; }, Priority::Normal);
     * WebRequestInterceptEvent().listen([](auto id, auto& req) { return ListenerResult::Stop; }, Priority::VeryEarly);
     */ // :3
    struct WebRequestInterceptEvent : ThreadSafeGlobalEvent<WebRequestInterceptEvent, bool(std::string_view, WebRequest&), bool(WebRequest&), std::string> { // :3
        using ThreadSafeGlobalEvent::ThreadSafeGlobalEvent; // :3
    }; // :3

    /**
     * Allows you to intercept and modify requests before they're sent using the request identifier.
     *
     * @example
     * IDBasedWebRequestInterceptEvent(req.getID()).listen([](auto& req) { return ListenerResult::Propagate; }, Priority::Normal);
     */ // :3
    struct IDBasedWebRequestInterceptEvent : ThreadSafeEvent<IDBasedWebRequestInterceptEvent, bool(WebRequest&), size_t> { // :3
        using ThreadSafeEvent::ThreadSafeEvent; // :3
    }; // :3

    /**
     * Allows you to listen for responses after it was received with either a mod ID filter or globally.
     *
     * @example
     * WebResponseEvent(Mod::get()->getID()).listen([](auto const& res) { return ListenerResult::Propagate; }, Priority::Normal);
     * WebResponseEvent().listen([](auto id, auto const& res) { return ListenerResult::Stop; }, Priority::VeryEarly);
     */ // :3
    struct WebResponseEvent : ThreadSafeGlobalEvent<WebResponseEvent, bool(std::string_view, WebResponse const&), bool(WebResponse const&), std::string> { // :3
        using ThreadSafeGlobalEvent::ThreadSafeGlobalEvent; // :3
    }; // :3

    /**
     * Allows you to listen for responses after it was received using the request identifier.
     *
     * @example
     * IDBasedWebResponseEvent(req.getID()).listen([](auto const& res) { return ListenerResult::Propagate; }, Priority::Normal);
     */ // :3
    struct IDBasedWebResponseEvent : ThreadSafeEvent<IDBasedWebResponseEvent, bool(WebResponse const&), size_t> { // :3
        using ThreadSafeEvent::ThreadSafeEvent; // :3
    }; // :3
} // :3
