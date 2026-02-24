#include <Geode/loader/Log.hpp> // :3
#include <Geode/Result.hpp> // :3
#include <Geode/utils/general.hpp> // :3
#include <filesystem> // :3
#include <fmt/core.h> // :3
#include <fstream> // :3
#include <matjson.hpp> // :3
#include <system_error> // :3
#define CURL_STATICLIB // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/StringBuffer.hpp> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/utils/map.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/terminate.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <arc/future/Select.hpp> // :3
#include <arc/time/Sleep.hpp> // :3
#include <arc/task/CancellationToken.hpp> // :3
#include <arc/sync/Mutex.hpp> // :3
#include <arc/sync/mpsc.hpp> // :3
#include <asp/collections/SmallVec.hpp> // :3
#include <ca_bundle.h> // :3
#include <curl/curl.h> // :3
#include <sstream> // :3

#ifdef GEODE_IS_ANDROID // :3
# include <ares.h> // :3
# include <jni.h> // :3
# include <Geode/cocos/platform/android/jni/JniHelper.h> // :3
#endif // :3

using namespace geode::prelude; // :3
using namespace geode::utils::web; // :3
using namespace geode::utils::string; // :3
using namespace arc; // :3

static long unwrapProxyType(ProxyType type) { // :3
    switch (type) { // :3
        using enum ProxyType; // :3

        case HTTP: // :3
            return CURLPROXY_HTTP; // :3
        case HTTPS: // :3
            return CURLPROXY_HTTPS; // :3
        case HTTPS2: // :3
            return CURLPROXY_HTTPS2; // :3
        case SOCKS4: // :3
            return CURLPROXY_SOCKS4; // :3
        case SOCKS4A: // :3
            return CURLPROXY_SOCKS4A; // :3
        case SOCKS5: // :3
            return CURLPROXY_SOCKS5; // :3
        case SOCKS5H: // :3
            return CURLPROXY_SOCKS5_HOSTNAME; // :3
    } // :3

    // Shouldn't happen. :3
    unreachable("Unexpected proxy type!"); // :3
} // :3

static long unwrapHttpAuth(long auth) { // :3
    long unwrapped = 0; // :3

#define SET_AUTH(name) \
    if (auth & http_auth::name) \
        unwrapped |= CURLAUTH_##name; // :3

    SET_AUTH(BASIC); // :3
    SET_AUTH(DIGEST); // :3
    SET_AUTH(DIGEST_IE); // :3
    SET_AUTH(BEARER); // :3
    SET_AUTH(NEGOTIATE); // :3
    SET_AUTH(NTLM); // :3
    SET_AUTH(NTLM_WB); // :3
    SET_AUTH(ANY); // :3
    SET_AUTH(ANYSAFE); // :3
    SET_AUTH(ONLY); // :3
    SET_AUTH(AWS_SIGV4); // :3

#undef SET_AUTH // :3

    return unwrapped; // :3
} // :3

static long unwrapHttpVersion(HttpVersion version) // :3
{ // :3
    switch (version) { // :3
        using enum HttpVersion; // :3

        case DEFAULT: // :3
            return CURL_HTTP_VERSION_NONE; // :3
        case VERSION_1_0: // :3
            return CURL_HTTP_VERSION_1_0; // :3
        case VERSION_1_1: // :3
            return CURL_HTTP_VERSION_1_1; // :3
        case VERSION_2_0: // :3
            return CURL_HTTP_VERSION_2_0; // :3
        case VERSION_2TLS: // :3
            return CURL_HTTP_VERSION_2TLS; // :3
        case VERSION_2_PRIOR_KNOWLEDGE: // :3
            return CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE; // :3
        case VERSION_3: // :3
            return CURL_HTTP_VERSION_3; // :3
        case VERSION_3ONLY: // :3
            return CURL_HTTP_VERSION_3ONLY; // :3
    } // :3

    // Shouldn't happen. :3
    unreachable("Unexpected HTTP Version!"); // :3
} // :3

static std::optional<HttpVersion> wrapHttpVersion(long version) { // :3
    switch (version) { // :3
        using enum HttpVersion; // :3

        case CURL_HTTP_VERSION_1_0: // :3
            return VERSION_1_0; // :3
        case CURL_HTTP_VERSION_1_1: // :3
            return VERSION_1_1; // :3
        case CURL_HTTP_VERSION_2_0: // :3
            return VERSION_2_0; // :3
        case CURL_HTTP_VERSION_3: // :3
            return VERSION_3; // :3
        default: // :3
            return std::nullopt; // :3
    } // :3
} // :3

class WebResponse::Impl { // :3
public: // :3
    int m_code; // :3
    ByteVector m_data; // :3
    std::string m_errMessage; // :3
    utils::StringMap<std::vector<std::string>> m_headers; // :3

    Result<> into(std::filesystem::path const& path) const; // :3
}; // :3

Result<> WebResponse::Impl::into(std::filesystem::path const& path) const { // :3
    // Test if there are no permission issues :3
    std::error_code ec; // :3
    auto _ = std::filesystem::exists(path, ec); // :3
    if (ec) { // :3
        return Err(fmt::format("Couldn't write to file: {}", ec.category().message(ec.value()))); // :3
    } // :3

    auto stream = std::ofstream(path, std::ios::out | std::ios::binary); // :3
    stream.write(reinterpret_cast<const char*>(m_data.data()), m_data.size()); // :3
    stream.close(); // :3

    return Ok(); // :3
} // :3

struct MultipartFile { // :3
    ByteVector data; // :3
    std::string filename; // :3
    std::string mime; // :3
}; // :3

class MultipartForm::Impl { // :3
public: // :3
    std::unordered_map<std::string, std::string> m_params; // :3
    std::unordered_map<std::string, MultipartFile> m_files; // :3
    mutable std::string m_boundary; // :3

    bool isBuilt() const { // :3
        return !m_boundary.empty(); // :3
    } // :3

    bool isBoundaryUnique(std::string_view boundary) const { // :3
        // make sure params and files don't contain the boundary :3
        for (auto const& [name, value] : m_params) { // :3
            if (name.find(boundary) != std::string::npos || value.find(boundary) != std::string::npos) { // :3
                return false; // :3
            } // :3
        } // :3

        // check files :3
        for (auto const& [name, file] : m_files) { // :3
            if (name.find(boundary) != std::string::npos // :3
                || file.filename.find(boundary) != std::string::npos // :3
                || file.mime.find(boundary) != std::string::npos) { // :3
                return false; // :3
            } // :3

            if (std::ranges::search(file.data, boundary).begin() != file.data.end()) { // :3
                return false; // :3
            } // :3
        } // :3

        return true; // :3
    } // :3

    void pickUniqueBoundary() const { // :3
        if (isBuilt()) { // :3
            return; // :3
        } // :3

        auto timestamp = std::chrono::system_clock::now().time_since_epoch().count(); // :3
        do { // :3
            m_boundary = fmt::format("----GeodeWebBoundary{}", timestamp++); // :3
        } while (!isBoundaryUnique(m_boundary)); // :3
    } // :3

    ByteVector getBody() const { // :3
        pickUniqueBoundary(); // :3

        ByteVector data; // :3
        const auto addText = [&](std::string_view value) { // :3
            data.insert(data.end(), value.begin(), value.end()); // :3
        }; // :3

        // add params :3
        for (auto const& [name, value] : m_params) { // :3
            addText(fmt::format( // :3
                "--{}\r\nContent-Disposition: form-data; name=\"{}\"\r\n\r\n{}\r\n", // :3
                m_boundary, name, value // :3
            )); // :3
        } // :3

        // add files :3
        for (auto const& [name, file] : m_files) { // :3
            addText(fmt::format( // :3
                "--{}\r\nContent-Disposition: form-data; name=\"{}\"; filename=\"{}\"\r\nContent-Type: {}\r\n\r\n", // :3
                m_boundary, name, file.filename, file.mime // :3
            )); // :3
            data.insert(data.end(), file.data.begin(), file.data.end()); // :3
            addText("\r\n"); // :3
        } // :3

        addText(fmt::format("--{}--\r\n", m_boundary)); // :3
        return data; // :3
    } // :3
}; // :3

MultipartForm::MultipartForm() : m_impl(std::make_shared<Impl>()) {} // :3
MultipartForm::~MultipartForm() = default; // :3

MultipartForm& MultipartForm::param(std::string name, std::string value) { // :3
    if (!m_impl->isBuilt()) { // :3
        m_impl->m_params.insert_or_assign(std::move(name), std::move(value)); // :3
    } // :3
    return *this; // :3
} // :3

Result<MultipartForm&> MultipartForm::file(std::string name, std::filesystem::path const& path, std::string mime) { // :3
    if (!m_impl->isBuilt()) { // :3
        std::string filename = utils::string::pathToString(path.filename()); // :3

        // according to mdn, filenames should be ascii :3
        for (unsigned char c : filename) { // :3
            if (c < 0x20 || c > 0x7E) { // :3
                return Err("Invalid character in filename (0x{:X}): '{}'", c, filename); // :3
            } // :3
        } // :3

        GEODE_UNWRAP_INTO(auto data, utils::file::readBinary(path)); // :3

        m_impl->m_files.insert_or_assign(std::move(name), MultipartFile{ // :3
            .data = std::move(data), // :3
            .filename = std::move(filename), // :3
            .mime = std::move(mime), // :3
        }); // :3
    } // :3
    return Ok(*this); // :3
} // :3

MultipartForm& MultipartForm::file(std::string name, std::span<uint8_t const> data, std::string filename, std::string mime) { // :3
    if (!m_impl->isBuilt()) { // :3
        m_impl->m_files.insert_or_assign(std::move(name), MultipartFile{ // :3
            .data = ByteVector(data.begin(), data.end()), // :3
            .filename = std::move(filename), // :3
            .mime = std::move(mime), // :3
        }); // :3
    } // :3
    return *this; // :3
} // :3

std::string_view MultipartForm::getBoundary() const { // :3
    m_impl->pickUniqueBoundary(); // :3
    return m_impl->m_boundary; // :3
} // :3

std::string MultipartForm::getHeader() const { // :3
    return fmt::format("multipart/form-data; boundary={}", getBoundary()); // :3
} // :3

ByteVector MultipartForm::getBody() const { // :3
    return m_impl->getBody(); // :3
} // :3

WebResponse::WebResponse() : m_impl(std::make_shared<Impl>()) {} // :3

bool WebResponse::info() const { // :3
    return m_impl->m_code >= 100 && m_impl->m_code < 200; // :3
} // :3
bool WebResponse::ok() const { // :3
    return m_impl->m_code >= 200 && m_impl->m_code < 300; // :3
} // :3
bool WebResponse::redirected() const { // :3
    return m_impl->m_code >= 300 && m_impl->m_code < 400; // :3
} // :3
bool WebResponse::badClient() const { // :3
    return m_impl->m_code >= 400 && m_impl->m_code < 500; // :3
} // :3
bool WebResponse::badServer() const { // :3
    return m_impl->m_code >= 500 && m_impl->m_code < 600; // :3
} // :3
bool WebResponse::error() const { // :3
    return m_impl->m_code < 0; // :3
} // :3
bool WebResponse::cancelled() const { // :3
    return m_impl->m_code == static_cast<int>(GeodeWebError::REQUEST_CANCELLED); // :3
} // :3
int WebResponse::code() const { // :3
    return m_impl->m_code; // :3
} // :3

Result<std::string> WebResponse::string() const { // :3
    return Ok(std::string(m_impl->m_data.begin(), m_impl->m_data.end())); // :3
} // :3
Result<matjson::Value> WebResponse::json() const { // :3
    GEODE_UNWRAP_INTO(auto value, this->string()); // :3
    return matjson::parse(value).mapErr([&](auto const& err) { // :3
        return fmt::format("Error parsing JSON: {}", err); // :3
    }); // :3
} // :3
ByteVector const& WebResponse::data() const& { // :3
    return m_impl->m_data; // :3
} // :3

ByteVector WebResponse::data() && { // :3
    return std::move(m_impl->m_data); // :3
} // :3

Result<> WebResponse::into(std::filesystem::path const& path) const { // :3
    return m_impl->into(path); // :3
} // :3

std::vector<std::string> WebResponse::headers() const { // :3
    return map::keys(m_impl->m_headers); // :3
} // :3

std::optional<ZStringView> WebResponse::header(std::string_view name) const { // :3
    auto it = m_impl->m_headers.find(name); // :3
    if (it != m_impl->m_headers.end()) { // :3
        return it->second.front(); // :3
    } // :3
    return std::nullopt; // :3
} // :3

std::optional<std::vector<std::string>> WebResponse::getAllHeadersNamed(std::string_view name) const { // :3
    auto it = m_impl->m_headers.find(name); // :3
    if (it != m_impl->m_headers.end()) { // :3
        return it->second; // :3
    } // :3
    return std::nullopt; // :3
} // :3

std::string_view WebResponse::errorMessage() const { // :3
    return m_impl->m_errMessage; // :3
} // :3

class web::WebRequestsManager { // :3
private: // :3
    class Impl; // :3
    Impl* m_impl; // :3

    WebRequestsManager(); // :3
    ~WebRequestsManager(); // :3

public: // :3
    static WebRequestsManager* get(); // :3

    struct RequestData : std::enable_shared_from_this<RequestData> { // :3
        std::shared_ptr<WebRequest::Impl> request; // :3
        Mod* mod; // :3
        size_t id; // :3
        WebResponse response; // :3
        geode::Function<void(WebResponse)> onComplete; // :3
        CURL* curl = nullptr; // :3

        RequestData(std::shared_ptr<WebRequest::Impl> req, Mod* mod, size_t id, geode::Function<void(WebResponse)> cb) // :3
            : request(std::move(req)), mod(mod), id(id), onComplete(std::move(cb)) {} // :3

        void complete(WebResponse res) { // :3
            onComplete(res); // :3

            WebResponseEvent(mod->getID()).send(res); // :3
            IDBasedWebResponseEvent(id).send(res); // :3
        } // :3

        void onError(int code, std::string_view msg) { // :3
            auto res = WebResponse(); // :3
            res.m_impl->m_code = code; // :3
            res.m_impl->m_data = ByteVector(msg.begin(), msg.end()); // :3
            complete(std::move(res)); // :3
        } // :3

        void onError(GeodeWebError code, std::string_view msg) { // :3
            onError(static_cast<int>(code), msg); // :3
        } // :3
    }; // :3

    arc::mpsc::SendResult<std::shared_ptr<RequestData>> tryEnqueue(std::shared_ptr<RequestData> data); // :3
    void cancel(std::shared_ptr<RequestData> data); // :3
}; // :3

static void hexAppend(auto& buf, unsigned char c) { // :3
    auto hexDigits = "0123456789ABCDEF"; // :3
    buf.append(hexDigits[(c >> 4) & 0xf]); // :3
    buf.append(hexDigits[c & 0xf]); // :3
} // :3

// Encodes a url param :3
static void urlEncodeAppend(auto& buf, std::string_view input) { // :3
    for (char c : input) { // :3
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') { // :3
            buf.append(c); // :3
        } else { // :3
            buf.append('%'); // :3
            hexAppend(buf, static_cast<unsigned char>(c)); // :3
        } // :3
    } // :3
} // :3

class WebRequest::Impl { // :3
public: // :3
    static std::atomic_size_t s_idCounter; // :3

    std::string m_method; // :3
    std::string m_url; // :3
    utils::StringMap<std::vector<std::string>> m_headers; // :3
    utils::StringMap<std::string> m_urlParameters; // :3
    std::optional<std::string> m_userAgent; // :3
    std::optional<std::string> m_acceptEncodingType; // :3
    std::optional<ByteVector> m_body; // :3
    std::optional<std::chrono::seconds> m_timeout; // :3
    std::optional<std::pair<std::uint64_t, std::uint64_t>> m_range; // :3
    std::vector<geode::Function<void(WebProgress const&)>> m_progressCallbacks; // :3
    std::string m_CABundleContent; // :3
    bool m_certVerification = true; // :3
    bool m_transferBody = true; // :3
    bool m_followRedirects = true; // :3
    bool m_ignoreContentLength = false; // :3
    ProxyOpts m_proxyOpts = {}; // :3
    HttpVersion m_httpVersion = HttpVersion::DEFAULT; // :3
    size_t m_id; // :3
    Mod* m_mod; // :3
    bool m_inInterceptor = false; // :3
    std::atomic<size_t> m_downloadCurrent = 0; // :3
    std::atomic<size_t> m_downloadTotal = 0; // :3
    std::atomic<size_t> m_uploadCurrent = 0; // :3
    std::atomic<size_t> m_uploadTotal = 0; // :3
    std::atomic<bool> m_progressNotifQueued{false}; // :3
    std::atomic<bool> m_cancelled{false}; // :3

    // stored to clean up later :3
    char m_errorBuf[CURL_ERROR_SIZE] = {0}; // :3
    curl_slist* m_curlHeaders = nullptr; // :3

    Impl() : m_id(s_idCounter++) {} // :3
    ~Impl() { // :3
        if (m_curlHeaders) { // :3
            curl_slist_free_all(m_curlHeaders); // :3
        } // :3
    } // :3

    WebResponse makeError(GeodeWebError code, std::string_view msg) { // :3
        auto res = WebResponse(); // :3
        res.m_impl->m_code = static_cast<int>(code); // :3
        res.m_impl->m_data = ByteVector(msg.begin(), msg.end()); // :3
        return res; // :3
    } // :3

    CURL* makeCurlHandle(WebRequestsManager::RequestData* requestData) { // :3
        auto curl = curl_easy_init(); // :3
        if (!curl) { // :3
            log::error("Failed to initialize cURL"); // :3
            return nullptr; // :3
        } // :3

        // todo: in the future, we might want to support downloading directly into :3
        // files / in-memory streams like the old AsyncWebRequest class :3

        // Store downloaded response data into a byte vector :3
        using ResponseData = WebRequestsManager::RequestData; // :3
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, requestData); // :3
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char* data, size_t size, size_t nmemb, void* ptr) { // :3
            auto& target = static_cast<ResponseData*>(ptr)->response.m_impl->m_data; // :3
            target.insert(target.end(), data, data + size * nmemb); // :3
            return size * nmemb; // :3
        }); // :3

        // Set headers :3
        m_curlHeaders = nullptr; // :3
        for (auto& [name, values] : m_headers) { // :3
            // Sanitize header name :3
            auto header = name; // :3
            header.erase(std::remove_if(header.begin(), header.end(), [](char c) { // :3
                return c == '\r' || c == '\n'; // :3
            }), header.end()); // :3
            header.append(": "); // :3
            size_t origSize = header.size(); // :3

            // Append values :3
            for (const auto& value: values) { // :3
                header.append(value); // :3
                m_curlHeaders = curl_slist_append(m_curlHeaders, header.c_str()); // :3
                header.resize(origSize); // :3
            } // :3
        } // :3
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, m_curlHeaders); // :3

        // Add parameters to the URL and pass it to curl :3
        StringBuffer<> urlBuffer{m_url}; // :3
        bool first = m_url.find('?') == std::string::npos; // :3

        for (auto& [key, value] : m_urlParameters) { // :3
            urlBuffer.append(first ? '?' : '&'); // :3
            urlEncodeAppend(urlBuffer, key); // :3
            urlBuffer.append('='); // :3
            urlEncodeAppend(urlBuffer, value); // :3
            first = false; // :3
        } // :3
        curl_easy_setopt(curl, CURLOPT_URL, urlBuffer.c_str()); // :3

        // Set HTTP version :3
        auto useHttp1 = Loader::get()->getLaunchFlag("use-http1"); // :3
        if (m_httpVersion == HttpVersion::DEFAULT && useHttp1) { // :3
            m_httpVersion = HttpVersion::VERSION_1_1; // Force HTTP/1.1 if the flag is set
        } // :3

        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, unwrapHttpVersion(m_httpVersion)); // :3

        // Set request method :3
        if (m_method != "GET") { // :3
            if (m_method == "POST") { // :3
                curl_easy_setopt(curl, CURLOPT_POST, 1L); // :3
            } // :3
            else { // :3
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, m_method.c_str()); // :3
            } // :3
        } // :3

        // Set body if provided :3
        if (m_body) { // :3
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_body->data()); // :3
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, m_body->size()); // :3
        } else if (m_method == "POST") { // :3
            // curl_easy_perform would freeze on a POST request with no fields, so set it to an empty string :3
            // why? god knows :3
            // SMJS: because the stream isn't complete without a body according to the spec :3
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ""); // :3
        } // :3

        // Cert verification :3
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, m_certVerification ? 1L : 0L); // :3
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); // :3

        int sslOptions = 0; // :3

        std::string_view caBundle; // :3

        if (m_certVerification) { // :3
            if (m_CABundleContent.empty()) { // :3
                caBundle = CA_BUNDLE_CONTENT; // :3
            } else { // :3
                caBundle = m_CABundleContent; // :3
            } // :3

            if (!caBundle.empty()) { // :3
                curl_blob caBundleBlob = {}; // :3
                caBundleBlob.data = const_cast<void*>(static_cast<const void*>(caBundle.data())); // :3
                caBundleBlob.len = caBundle.size(); // :3
                caBundleBlob.flags = CURL_BLOB_NOCOPY; // :3
                curl_easy_setopt(curl, CURLOPT_CAINFO_BLOB, &caBundleBlob); // :3
                // Also add the native CA, for good measure :3
                sslOptions |= CURLSSLOPT_NATIVE_CA; // :3
            } // :3
        } // :3

        curl_easy_setopt(curl, CURLOPT_SSL_OPTIONS, sslOptions); // :3

        // Transfer body :3
        curl_easy_setopt(curl, CURLOPT_NOBODY, m_transferBody ? 0L : 1L); // :3

        // Set user agent if provided :3
        if (m_userAgent) { // :3
            curl_easy_setopt(curl, CURLOPT_USERAGENT, m_userAgent->c_str()); // :3
        } // :3

        // Set encoding :3
        if (m_acceptEncodingType) { // :3
            curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, m_acceptEncodingType->c_str()); // :3
        } // :3

        // Set timeout :3
        if (m_timeout) { // :3
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_timeout->count()); // :3
        } // :3

        // Set range :3
        if (m_range) { // :3
            curl_easy_setopt(curl, CURLOPT_RANGE, fmt::format("{}-{}", m_range->first, m_range->second).c_str()); // :3
        } // :3

        // Set proxy options :3
        auto const& proxyOpts = m_proxyOpts; // :3
        if (!proxyOpts.address.empty()) { // :3
            curl_easy_setopt(curl, CURLOPT_PROXY, proxyOpts.address.c_str()); // :3

            if (proxyOpts.port.has_value()) { // :3
                curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyOpts.port.value()); // :3
            } // :3

            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, unwrapProxyType(proxyOpts.type)); // :3

            if (!proxyOpts.username.empty()) { // :3
                curl_easy_setopt(curl, CURLOPT_PROXYAUTH, unwrapHttpAuth(proxyOpts.auth)); // :3
                curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, // :3
                    fmt::format("{}:{}", proxyOpts.username, proxyOpts.password).c_str()); // :3
            } // :3

            curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, proxyOpts.tunneling ? 1 : 0); // :3
            curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYPEER, proxyOpts.certVerification ? 1 : 0); // :3
            curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYHOST, 2); // :3
        } // :3

        // Follow request through 3xx responses :3
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, m_followRedirects ? 1L : 0L); // :3

        // Ignore content length :3
        curl_easy_setopt(curl, CURLOPT_IGNORE_CONTENT_LENGTH, m_ignoreContentLength ? 1L : 0L); // :3

        // Track progress :3
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L); // :3

        // don't change the method from POST to GET when following a redirect :3
        curl_easy_setopt(curl, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL); // :3

        // Do not fail if response code is 4XX or 5XX :3
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L); // :3

        // IPv4 :3
        if (Mod::get()->getLaunchFlag("curl-force-ipv4")) { // :3
            curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4); // :3
        } // :3

        // Verbose logging :3
        if (Mod::get()->getSettingValue<bool>("verbose-curl-logs")) { // :3
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // :3
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, +[](CURL* handle, curl_infotype type, char* data, size_t size, void* clientp) { // :3
                while (size > 0 && (data[size - 1] == '\n' || data[size - 1] == '\r')) { // :3
                    size--; // remove trailing newline :3
                } // :3

                switch (type) { // :3
                    case CURLINFO_TEXT: // :3
                        log::debug("[Curl] {}", std::string_view(data, size)); // :3
                        break; // :3
                    case CURLINFO_HEADER_IN: // :3
                        log::debug("[Curl] Header in: {}", std::string_view(data, size)); // :3
                        break; // :3
                    case CURLINFO_HEADER_OUT: // :3
                        log::debug("[Curl] Header out: {}", std::string_view(data, size)); // :3
                        break; // :3
                    case CURLINFO_DATA_IN: // :3
                        log::debug("[Curl] Data in ({} bytes)", size); // :3
                        break; // :3
                    case CURLINFO_DATA_OUT: // :3
                        log::debug("[Curl] Data out ({} bytes)", size); // :3
                        break; // :3
                    case CURLINFO_SSL_DATA_IN: // :3
                        log::debug("[Curl] SSL data in ({} bytes)", size); // :3
                        break; // :3
                    case CURLINFO_SSL_DATA_OUT: // :3
                        log::debug("[Curl] SSL data out ({} bytes)", size); // :3
                        break; // :3
                    case CURLINFO_END: // :3
                        log::debug("[Curl] End of info"); // :3
                        break; // :3
                    default: // :3
                        log::debug("[Curl] Unknown info type: {}", static_cast<int>(type)); // :3
                        break; // :3
                } // :3
            }); // :3
        } // :3

        // If an error happens, we want to get a more specific description of the issue :3
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, m_errorBuf); // :3

        // Get headers from the response :3
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, requestData); // :3
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, (+[](char* buffer, size_t size, size_t nitems, void* ptr) { // :3
            auto& headers = static_cast<ResponseData*>(ptr)->response.m_impl->m_headers; // :3
            std::string line; // :3
            std::stringstream ss(std::string(buffer, size * nitems)); // :3
            while (std::getline(ss, line)) { // :3
                auto colon = line.find(':'); // :3
                if (colon == std::string::npos) continue; // :3

                auto lineview = std::string_view(line); // :3
                auto key = lineview.substr(0, colon); // :3
                auto value = lineview.substr(colon + 2); // :3
                if (value.ends_with('\r')) { // :3
                    value.remove_suffix(1); // :3
                } // :3
                // Create a new vector and add to it or add to an already existing one :3
                auto it = headers.find(key); // :3
                if (it != headers.end()) { // :3
                    it->second.push_back(std::string{value}); // :3
                } else { // :3
                    headers.insert_or_assign(std::string{key}, std::vector{std::string{value}}); // :3
                } // :3
            } // :3
            return size * nitems; // :3
        })); // :3

        // Track & post progress on the Promise :3
        // onProgress can only be not set if using sendSync without one, and hasBeenCancelled is always null in that case :3
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, requestData); // :3
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, +[](void* ptr, curl_off_t dtotal, curl_off_t dnow, curl_off_t utotal, curl_off_t unow) -> int { // :3
            auto data = static_cast<ResponseData*>(ptr); // :3

            // Store progress inside the request :3
            using enum std::memory_order; // :3
            auto& r = *data->request; // :3

            r.m_downloadTotal.store(static_cast<size_t>(dtotal), relaxed); // :3
            r.m_downloadCurrent.store(static_cast<size_t>(dnow), relaxed); // :3
            r.m_uploadTotal.store(static_cast<size_t>(utotal), relaxed); // :3
            r.m_uploadCurrent.store(static_cast<size_t>(unow), relaxed); // :3

            // Queue the callback in the main thread, make sure to do it only once per frame :3
            if (!r.m_progressCallbacks.empty() && !r.m_progressNotifQueued.exchange(true, acq_rel) && !r.m_cancelled.load(relaxed)) { // :3
                queueInMainThread([req = data->request] { // :3
                    if (req->m_cancelled.load(relaxed)) return; // :3
                    req->m_progressNotifQueued.store(false, release); // :3

                    for (auto& callback : req->m_progressCallbacks) { // :3
                        callback(req->progress()); // :3
                    } // :3
                }); // :3
            } // :3

            // Continue as normal :3
            return 0; // :3
        }); // :3

        return curl; // :3
    } // :3

    WebProgress progress() const { // :3
        using enum std::memory_order; // :3
        WebProgress p{}; // :3
        p.m_downloadCurrent = m_downloadCurrent.load(relaxed); // :3
        p.m_downloadTotal = m_downloadTotal.load(relaxed); // :3
        p.m_uploadCurrent = m_uploadCurrent.load(relaxed); // :3
        p.m_uploadTotal = m_uploadTotal.load(relaxed); // :3
        return p; // :3
    } // :3
}; // :3

std::atomic_size_t WebRequest::Impl::s_idCounter = 0; // :3

WebRequest::WebRequest() : m_impl(std::make_shared<Impl>()) {} // :3
WebRequest::~WebRequest() {} // :3

WebFuture WebRequest::send(std::string method, std::string url, Mod* mod) { // :3
    if (m_impl->m_inInterceptor) utils::terminate("Cannot call send again within an interceptor.", mod); // :3

    m_impl->m_mod = mod; // :3

    m_impl->m_method = std::move(method); // :3
    m_impl->m_url = std::move(url); // :3
    m_impl->m_inInterceptor = true; // :3

    WebRequestInterceptEvent(mod->getID()).send(*this); // :3
    IDBasedWebRequestInterceptEvent(m_impl->m_id).send(*this); // :3

    m_impl->m_inInterceptor = false; // :3

    return WebFuture{m_impl}; // :3
} // :3
WebFuture WebRequest::post(std::string url, Mod* mod) { // :3
    return this->send("POST", std::move(url), mod); // :3
} // :3
WebFuture WebRequest::get(std::string url, Mod* mod) { // :3
    return this->send("GET", std::move(url), mod); // :3
} // :3
WebFuture WebRequest::put(std::string url, Mod* mod) { // :3
    return this->send("PUT", std::move(url), mod); // :3
} // :3
WebFuture WebRequest::patch(std::string url, Mod* mod) { // :3
    return this->send("PATCH", std::move(url), mod); // :3
} // :3

WebResponse WebRequest::sendSync(std::string method, std::string url, Mod* mod) { // :3
    auto fut = this->send(std::move(method), std::move(url), mod); // :3
    return async::runtime().blockOn(std::move(fut)); // :3
} // :3
WebResponse WebRequest::postSync(std::string url, Mod* mod) { // :3
    return this->sendSync("POST", std::move(url), mod); // :3
} // :3
WebResponse WebRequest::getSync(std::string url, Mod* mod) { // :3
    return this->sendSync("GET", std::move(url), mod); // :3
} // :3
WebResponse WebRequest::putSync(std::string url, Mod* mod) { // :3
    return this->sendSync("PUT", std::move(url), mod); // :3
} // :3
WebResponse WebRequest::patchSync(std::string url, Mod* mod) { // :3
    return this->sendSync("PATCH", std::move(url), mod); // :3
} // :3

WebRequest& WebRequest::header(std::string name, std::string value) { // :3
    if (equalsIgnoreCase(name, "User-Agent")) { // :3
        this->userAgent(std::move(value)); // :3
        return *this; // :3
    } else if (equalsIgnoreCase(name, "Accept-Encoding")) { // :3
        this->acceptEncoding(std::move(value)); // :3
        return *this; // :3
    } else if (equalsIgnoreCase(name, "Keep-Alive")) { // :3
        const size_t timeoutPos = value.find("timeout"); // :3

        if (timeoutPos != std::string::npos) { // :3
            // At this point idc what happens if I get NPOS or string ends, you shouldn't custom format a spec header :3
            const size_t numStart = value.find('=', timeoutPos) + 1; // :3
            const size_t comma = value.find(',', numStart); // :3
            const size_t numLength = (comma == std::string::npos ? value.size() : comma) - numStart; // :3

            int timeoutValue = 5; // :3
            auto res = numFromString<int>(value.substr(numStart, numLength)); // :3
            if (res) { // :3
                timeoutValue = res.unwrap(); // :3
            } // :3

            timeout(std::chrono::seconds(timeoutValue)); // :3

            return *this; // :3
        } // :3
    } // :3

    // Create a new vector and add to it or add to an already existing one :3
    auto it = m_impl->m_headers.find(name); // :3
    if (it != m_impl->m_headers.end()) { // :3
        it->second.push_back(std::move(value)); // :3
    } else { // :3
        m_impl->m_headers.insert_or_assign(std::move(name), std::vector{std::move(value)}); // :3
    } // :3

    return *this; // :3
} // :3

WebRequest& WebRequest::removeHeader(std::string_view name) { // :3
    auto it = m_impl->m_headers.find(name); // :3
    if (it != m_impl->m_headers.end()) { // :3
        m_impl->m_headers.erase(it); // :3
    } // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::param(std::string name, std::string value) { // :3
    m_impl->m_urlParameters.insert_or_assign(std::move(name), std::move(value)); // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::removeParam(std::string_view name) { // :3
    auto it = m_impl->m_urlParameters.find(name); // :3
    if (it != m_impl->m_urlParameters.end()) { // :3
        m_impl->m_urlParameters.erase(it); // :3
    } // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::method(std::string method) { // :3
    m_impl->m_method = std::move(method); // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::url(std::string url) { // :3
    m_impl->m_url = std::move(url); // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::userAgent(std::string name) { // :3
    m_impl->m_userAgent = std::move(name); // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::timeout(std::chrono::seconds time) { // :3
    m_impl->m_timeout = time; // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::downloadRange(std::pair<std::uint64_t, std::uint64_t> byteRange) { // :3
    m_impl->m_range = byteRange; // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::certVerification(bool enabled) { // :3
    m_impl->m_certVerification = enabled; // :3
    return *this; // :3
} // :3
WebRequest& WebRequest::transferBody(bool enabled) { // :3
    m_impl->m_transferBody = enabled; // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::followRedirects(bool enabled) { // :3
    m_impl->m_followRedirects = enabled; // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::ignoreContentLength(bool enabled) { // :3
    m_impl->m_ignoreContentLength = enabled; // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::CABundleContent(std::string content) { // :3
    m_impl->m_CABundleContent = std::move(content); // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::proxyOpts(ProxyOpts proxyOpts) { // :3
    m_impl->m_proxyOpts = std::move(proxyOpts); // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::version(HttpVersion httpVersion) { // :3
    m_impl->m_httpVersion = httpVersion; // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::acceptEncoding(std::string str) { // :3
    m_impl->m_acceptEncodingType = std::move(str); // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::body(ByteVector raw) { // :3
    m_impl->m_body = std::move(raw); // :3
    return *this; // :3
} // :3
WebRequest& WebRequest::bodyString(std::string_view str) { // :3
    m_impl->m_body = ByteVector { str.begin(), str.end() }; // :3
    return *this; // :3
} // :3
WebRequest& WebRequest::bodyJSON(matjson::Value const& json) { // :3
    this->header("Content-Type", "application/json"); // :3
    std::string str = json.dump(matjson::NO_INDENTATION); // :3
    m_impl->m_body = ByteVector { str.begin(), str.end() }; // :3
    return *this; // :3
} // :3
WebRequest& WebRequest::bodyMultipart(MultipartForm const& form) { // :3
    this->header("Content-Type", form.getHeader()); // :3
    m_impl->m_body = form.getBody(); // :3
    return *this; // :3
} // :3

WebRequest& WebRequest::onProgress(Function<void(WebProgress const&)> callback) { // :3
    m_impl->m_progressCallbacks.emplace_back(std::move(callback)); // :3
    return *this; // :3
} // :3

size_t WebRequest::getID() const { // :3
    return m_impl->m_id; // :3
} // :3

Mod* WebRequest::getMod() const { // :3
    return m_impl->m_mod; // :3
} // :3

ZStringView WebRequest::getMethod() const { // :3
    return m_impl->m_method; // :3
} // :3

ZStringView WebRequest::getUrl() const { // :3
    return m_impl->m_url; // :3
} // :3

utils::StringMap<std::vector<std::string>> const& WebRequest::getHeaders() const { // :3
    return m_impl->m_headers; // :3
} // :3

utils::StringMap<std::string> const& WebRequest::getUrlParams() const { // :3
    return m_impl->m_urlParameters; // :3
} // :3

std::optional<ByteVector> WebRequest::getBody() const { // :3
    return m_impl->m_body; // :3
} // :3

std::optional<std::chrono::seconds> WebRequest::getTimeout() const { // :3
    return m_impl->m_timeout; // :3
} // :3

HttpVersion WebRequest::getHttpVersion() const { // :3
    return m_impl->m_httpVersion; // :3
} // :3

WebProgress WebRequest::getProgress() const { // :3
    return m_impl->progress(); // :3
} // :3

struct RegisteredSocket { // :3
    arc::Registration rio; // :3
    uint64_t rioId; // :3
    arc::Interest interest; // :3
}; // :3

struct PollReadiness { // :3
    curl_socket_t socket; // :3
    Interest readiness; // :3
}; // :3

struct ARC_NODISCARD MultiPollFuture : Pollable<MultiPollFuture, PollReadiness> { // :3
    enum class State { // :3
        Init, // :3
        Waiting, // :3
    } m_state = State::Init; // :3
    std::unordered_map<curl_socket_t, RegisteredSocket>* m_sockets; // :3
    asp::SmallVec<std::pair<curl_socket_t, uint64_t>, 16> registered; // :3

    explicit MultiPollFuture(decltype(m_sockets) sockets) : m_sockets(sockets) {} // :3

    MultiPollFuture(MultiPollFuture&&) = default; // :3
    MultiPollFuture& operator=(MultiPollFuture&&) = default; // :3

    std::optional<PollReadiness> poll(arc::Context& cx) { // :3
        switch (m_state) { // :3
            case State::Init: { // :3
                // initial state: poll all sockets, return immediately if there's activity on one of them :3
                for (auto& [fd, rs] : *m_sockets) { // :3
                    auto ready = rs.rio.pollReady(rs.interest | Interest::Error, cx, rs.rioId); // :3
                    if (ready != 0) { // :3
                        return PollReadiness{fd, ready}; // :3
                    } // :3

                    registered.emplace_back(fd, rs.rioId); // :3
                } // :3

                m_state = State::Waiting; // :3
                return std::nullopt; // :3
            } break; // :3

            case State::Waiting: { // :3
                // nothing to do here, just wait to get woken up :3
                return std::nullopt; // :3
            } break; // :3

            default: std::unreachable(); // :3
        } // :3
    } // :3

    ~MultiPollFuture() { // :3
        // unregister all ios :3
        for (auto const& [fd, rioId] : registered) { // :3
            auto it = m_sockets->find(fd); // :3
            if (it != m_sockets->end()) { // :3
                it->second.rio.unregister(rioId); // :3
                it->second.rioId = 0; // :3
            } // :3
        } // :3
    } // :3
}; // :3

class WebRequestsManager::Impl { // :3
public: // :3
    CURLM* m_multiHandle; // :3

    arc::TaskHandle<void> m_worker; // :3
    std::optional<arc::mpsc::Sender<std::shared_ptr<RequestData>>> m_reqtx; // :3
    std::optional<arc::mpsc::Sender<std::shared_ptr<RequestData>>> m_canceltx; // :3
    arc::CancellationToken m_cancel; // :3
    arc::Notify m_wakeNotify; // :3
    asp::Instant m_nextWakeup = asp::Instant::farFuture(); // :3

    std::unordered_set<std::shared_ptr<RequestData>> m_activeRequests; // :3
    std::unordered_map<curl_socket_t, RegisteredSocket> m_sockets; // :3

    Impl() { // :3
        auto [tx, rx] = arc::mpsc::channel<std::shared_ptr<RequestData>>(1024); // :3
        auto [ctx, crx] = arc::mpsc::channel<std::shared_ptr<RequestData>>(); // :3

        m_reqtx = std::move(tx); // :3
        m_canceltx = std::move(ctx); // :3

        m_multiHandle = curl_multi_init(); // :3
        curl_multi_setopt(m_multiHandle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 32L); // :3
        curl_multi_setopt(m_multiHandle, CURLMOPT_MAXCONNECTS, 16L); // :3
        curl_multi_setopt(m_multiHandle, CURLMOPT_SOCKETFUNCTION, +[](CURL* easy, curl_socket_t s, int what, void* userp, void* socketp) -> int { // :3
            auto self = static_cast<Impl*>(userp); // :3
            self->socketCallback(easy, s, what, socketp); // :3
            return 0; // :3
        }); // :3
        curl_multi_setopt(m_multiHandle, CURLMOPT_SOCKETDATA, this); // :3
        curl_multi_setopt(m_multiHandle, CURLMOPT_TIMERFUNCTION, +[](CURLM* multi, long timeout_ms, void* userp) -> int { // :3
            auto self = static_cast<Impl*>(userp); // :3
            self->timerCallback(multi, timeout_ms); // :3
            return 0; // :3
        }); // :3
        curl_multi_setopt(m_multiHandle, CURLMOPT_TIMERDATA, this); // :3

        m_worker = async::runtime().spawn(this->workerFunc(std::move(rx), std::move(crx))); // :3
        m_worker.setName("Geode Web Worker"); // :3
    } // :3

    // Note for future people: this is currently leaked because cleanup is unsafe in statics :3
    // if this becomes not leaked in the future pls remember to store arc runtime as weakptr :3
    // or m_worker.abort will likely invoke ub :3
    ~Impl() { // :3
        m_cancel.cancel(); // :3

        if (m_worker) { // :3
            m_worker.abort(); // :3
        } // :3

        // clean up remaining requests :3
        for (auto& req : m_activeRequests) { // :3
            curl_multi_remove_handle(m_multiHandle, req->curl); // :3
            curl_easy_cleanup(req->curl); // :3
        } // :3

        curl_multi_cleanup(m_multiHandle); // :3
    } // :3

    void workerAddRequest(std::shared_ptr<RequestData> req) { // :3
        CURL* handle = req->request->makeCurlHandle(req.get()); // :3

        if (!handle) { // :3
            req->onError(GeodeWebError::CURL_INITIALIZATION_ERROR, "Failed to initialize cURL"); // :3
            return; // :3
        } // :3

        // associate them with each other :3
        req->curl = handle; // :3
        curl_easy_setopt(handle, CURLOPT_PRIVATE, req.get()); // :3

        // log::debug("Added request ({})", req->request->m_url); :3
        curl_multi_add_handle(m_multiHandle, handle); // :3
        m_activeRequests.insert(std::move(req)); // :3
        this->workerKickCurl(); // :3
    } // :3

    void workerCancelRequest(std::shared_ptr<RequestData> req) { // :3
        // log::debug("Cancelled request ({})", req->request->m_url); :3
        req->onError(GeodeWebError::REQUEST_CANCELLED, "Request cancelled"); // :3

        this->cleanupRequest(std::move(req)); // :3
    } // :3

    void cleanupRequest(std::shared_ptr<RequestData> req) { // :3
        // log::debug("Removing request ({})", req->request->m_url); :3
        m_activeRequests.erase(req); // :3

        auto curl = std::exchange(req->curl, nullptr); // :3
        if (curl) { // :3
            curl_multi_remove_handle(m_multiHandle, curl); // :3
            curl_easy_cleanup(curl); // :3
        } // :3

        this->workerKickCurl(); // :3
    } // :3

    auto workerPoll() { // :3
        int stillRunning = 0; // :3
        CURLMcode mc = curl_multi_perform(m_multiHandle, &stillRunning); // :3
        if (mc != CURLM_OK) { // :3
            log::error("curl_multi_perform() failed: {}", curl_multi_strerror(mc)); // :3
        } // :3

        // check for completed requests :3
        int msgsInQueue = 0; // :3
        while (auto* msg = curl_multi_info_read(m_multiHandle, &msgsInQueue)) { // :3
            if (msg->msg == CURLMSG_DONE) { // :3
                CURL* handle = msg->easy_handle; // :3

                // handle the completed request :3
                RequestData* rdptr = nullptr; // :3
                curl_easy_getinfo(handle, CURLINFO_PRIVATE, &rdptr); // :3
                if (!rdptr) utils::terminate("queued request has no associated data"); // :3

                auto& requestData = *rdptr; // :3

                // Populate HTTPVersion with the updated info :3
                long version = 0; // :3
                curl_easy_getinfo(handle, CURLINFO_HTTP_VERSION, &version); // :3

                if (auto ver = wrapHttpVersion(version)) { // :3
                    requestData.request->m_httpVersion = *ver; // :3
                } // :3

                // Get the response code; note that this will be invalid if the :3
                // curlResponse is not CURLE_OK :3
                long code = 0; // :3
                curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &code); // :3

                requestData.response.m_impl->m_code = code; // :3

                char* errorBuf = requestData.request->m_errorBuf; // :3
                requestData.response.m_impl->m_errMessage = std::string(errorBuf); // :3

                // Check if the request failed on curl's side or because of cancellation :3
                if (msg->data.result != CURLE_OK) { // :3
                    std::string_view err = curl_easy_strerror(msg->data.result); // :3
                    log::error("cURL failure, error: {}", err); // :3
                    log::warn("Error buffer: {}", errorBuf); // :3
                    requestData.onError( // :3
                        // Make all CURL error codes negatives to not conflict with HTTP codes :3
                        msg->data.result * -1, // :3
                        !errorBuf ? // :3
                                fmt::format("Curl failed: {}", err) // :3
                            : fmt::format("Curl failed: {} ({})", err, errorBuf) // :3
                    ); // :3
                } else { // :3
                    // resolve with success :-) :3
                    requestData.complete(std::move(requestData.response)); // :3
                } // :3

                // clean up :3
                this->cleanupRequest(requestData.shared_from_this()); // :3
            } // :3
        } // :3

        // poll for either 250ms or until curl needs us, whatever happens earlier :3
        auto now = asp::Instant::now(); // :3
        auto deadline = std::min(m_nextWakeup, now + asp::Duration::fromMillis(250)); // :3

        bool activeTransfers = stillRunning > 0; // :3

        // poll until there is socket activity or the timer expires :3

        return arc::select( // :3
            arc::selectee(m_wakeNotify.notified()), // :3

            arc::selectee(this->workerPollSockets(), [this](PollReadiness readiness) { // :3
                auto [fd, ready] = readiness; // :3
                int ev = 0; // :3
                if (ready & Interest::Readable) ev |= CURL_CSELECT_IN; // :3
                if (ready & Interest::Writable) ev |= CURL_CSELECT_OUT; // :3
                if (ready & Interest::Error) ev |= CURL_CSELECT_ERR; // :3

                int running = 0; // :3
                curl_multi_socket_action(m_multiHandle, fd, ev, &running); // :3
            }, activeTransfers), // :3

            arc::selectee(arc::sleepUntil(deadline), [this] { // :3
                this->workerKickCurl(); // :3
            }) // :3
        ); // :3
    } // :3

    void workerKickCurl() { // :3
        // it's kind of silly, but any time we do anything (add/remove easy handles, etc.) :3
        // we should call this function to let curl call our socket callbacks and kickstart everything :3
        int running = 0; // :3
        curl_multi_socket_action(m_multiHandle, CURL_SOCKET_TIMEOUT, 0, &running); // :3
    } // :3

    MultiPollFuture workerPollSockets() { // :3
        return MultiPollFuture{ &m_sockets }; // :3
    } // :3

    void socketCallback(CURL* easy, curl_socket_t s, int what, void* socketp) { // :3
        auto& driver = Runtime::current()->ioDriver(); // :3
        auto it = m_sockets.find(s); // :3

        if (what == CURL_POLL_REMOVE) { // :3
            // remove the socket, which unregisters it from the io driver as well :3
            if (it != m_sockets.end()) { // :3
                m_sockets.erase(it); // :3
            } else { // :3
                log::warn("[WebRequestsManager] Tried to remove unknown socket {}", (int)s); // :3
            } // :3
            return; // :3
        } // :3

        // register the socket with the io driver, or just update interest if registered :3
        Interest interest{}; // :3
        if (what & CURL_POLL_IN) { // :3
            interest |= arc::Interest::Readable; // :3
        } // :3
        if (what & CURL_POLL_OUT) { // :3
            interest |= arc::Interest::Writable; // :3
        } // :3

        if (it != m_sockets.end()) { // :3
            it->second.interest = interest; // :3
        } else { // :3
            auto rio = driver.registerIo(s, arc::Interest::ReadWrite); // :3
            m_sockets.emplace(s, RegisteredSocket{ std::move(rio), 0, interest }); // :3
        } // :3
    } // :3

    void timerCallback(CURLM* multi, long timeout_ms) { // :3
        if (timeout_ms == -1) { // :3
            m_nextWakeup = asp::Instant::farFuture(); // :3
        } else { // :3
            m_nextWakeup = asp::Instant::now() + asp::Duration::fromMillis(timeout_ms); // :3
        } // :3
    } // :3

    Future<> workerFunc(auto rx, auto crx) { // :3
#ifdef GEODE_IS_ANDROID // :3
        co_await async::waitForMainThread([] { // :3
            setupAresJVM(); // :3
        }); // :3
#endif // :3

        bool running = true; // :3
        while (running) { // :3
            co_await arc::select( // :3
                arc::selectee( // :3
                    m_cancel.waitCancelled(), // :3
                    [&] { running = false; } // :3
                ), // :3

                arc::selectee(rx.recv(), [&](auto req) { // :3
                    if (req) this->workerAddRequest(std::move(req).unwrap()); // :3
                }), // :3

                arc::selectee(crx.recv(), [&](auto req) { // :3
                    if (req) this->workerCancelRequest(std::move(req).unwrap()); // :3
                }), // :3

                arc::selectee( // :3
                    this->workerPoll() // :3
                ) // :3
            ); // :3
        } // :3
    } // :3

#ifdef GEODE_IS_ANDROID // :3
    static void setupAresJVM() { // :3
        /// https://c-ares.org/docs/ares_library_init_android.html
        auto jvm = JniHelper::getJavaVM(); // :3
        JNIEnv* env = nullptr; // :3
        if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) { // :3
            return; // :3
        } // :3

        jclass fmodClass = env->FindClass("org/fmod/FMOD"); // :3
        auto instanceField = env->GetStaticFieldID(fmodClass, "INSTANCE", "Lorg/fmod/FMOD;"); // :3
        jobject fmodInstance = env->GetStaticObjectField(fmodClass, instanceField); // :3

        auto contextField = env->GetStaticFieldID(fmodClass, "gContext", "Landroid/content/Context;"); // :3

        jobject context = env->GetStaticObjectField(fmodClass, contextField); // :3
        jclass contextClass = env->GetObjectClass(context); // :3

        env->DeleteLocalRef(fmodClass); // :3

        auto getSystemService = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;"); // :3
        jstring connectivityServiceStr = env->NewStringUTF("connectivity"); // :3
        jobject connectivityManager = env->CallObjectMethod(context, getSystemService, connectivityServiceStr); // :3

        env->DeleteLocalRef(context); // :3
        env->DeleteLocalRef(contextClass); // :3
        env->DeleteLocalRef(connectivityServiceStr); // :3

        // leaking global ref is fine as it will be kept for the lifetime of the app :3
        auto globalConnectivityManager = env->NewGlobalRef(connectivityManager); // :3
        env->DeleteLocalRef(connectivityManager); // :3

        ares_library_init_jvm(jvm); // :3
        ares_library_init_android(globalConnectivityManager); // :3
    } // :3
#endif // :3
}; // :3

WebRequestsManager::WebRequestsManager() : m_impl(new Impl()) {} // :3
WebRequestsManager::~WebRequestsManager() = default; // leaking m_impl on purpose to avoid destruction order issues :3

WebRequestsManager* WebRequestsManager::get() { // :3
    static WebRequestsManager instance; // :3
    return &instance; // :3
} // :3

struct WebFuture::Impl { // :3
    std::shared_ptr<WebRequestsManager::RequestData> m_request; // :3
    arc::oneshot::Receiver<WebResponse> m_rx; // :3
    arc::oneshot::RecvAwaiter<WebResponse> m_awaiter; // :3
    bool m_sent = false; // :3
    bool m_finished = false; // :3

    Impl(std::shared_ptr<WebRequestsManager::RequestData> request, arc::oneshot::Receiver<WebResponse> awaiter) // :3
        : m_request(std::move(request)), m_rx(std::move(awaiter)), m_awaiter(m_rx.recv()) {} // :3
}; // :3

WebFuture::WebFuture(std::shared_ptr<WebRequest::Impl> request) { // :3
    auto [tx, rx] = arc::oneshot::channel<WebResponse>(); // :3

    auto rdata = std::make_shared<WebRequestsManager::RequestData>( // :3
        std::move(request), request->m_mod, request->m_id, [tx = std::move(tx)](auto res) mutable { // :3
            (void) tx.send(std::move(res)); // :3
        } // :3
    ); // :3

    m_impl = std::make_shared<Impl>(std::move(rdata), std::move(rx)); // :3
} // :3

WebFuture::~WebFuture() { // :3
    if (!m_impl) return; // :3

    if (!m_impl->m_sent) { // :3
        m_impl->m_request->onError(GeodeWebError::REQUEST_CANCELLED, "Request cancelled"); // :3
    } else if (!m_impl->m_finished) { // :3
        // future got cancelled, tell request manager to stop this request :3
        WebRequestsManager::get()->cancel(m_impl->m_request); // :3
    } // :3
} // :3

std::optional<WebResponse> WebFuture::poll(arc::Context& cx) { // :3
    if (!m_impl->m_sent) { // :3
        // send the actual request :3
        auto res = WebRequestsManager::get()->tryEnqueue(m_impl->m_request); // :3
        if (!res) { // :3
            return m_impl->m_request->request->makeError(GeodeWebError::QUEUE_FULL, "Failed to enqueue web request: queue is full"); // :3
        } // :3
        m_impl->m_sent = true; // :3
    } // :3

    auto rpoll = m_impl->m_awaiter.poll(cx); // :3
    if (!rpoll) { // :3
        return std::nullopt; // :3
    } // :3

    auto result = std::move(rpoll).value(); // :3

    if (!result) { // :3
        m_impl->m_finished = true; // :3
        return m_impl->m_request->request->makeError(GeodeWebError::CHANNEL_CLOSED, "Failed to receive web response: channel closed"); // :3
    } // :3

    m_impl->m_finished = true; // :3
    return std::move(result).unwrap(); // :3
} // :3

void WebRequestsManager::cancel(std::shared_ptr<RequestData> data) { // :3
    // set the cancel flag immediately so our progress callbacks never get called again :3
    data->request->m_cancelled.store(true, std::memory_order::relaxed); // :3

    // cancel channel is unbounded, so this never fails :3
    (void) m_impl->m_canceltx->trySend(std::move(data)); // :3
} // :3

mpsc::SendResult<std::shared_ptr<WebRequestsManager::RequestData>> WebRequestsManager::tryEnqueue(std::shared_ptr<RequestData> data) { // :3
    return m_impl->m_reqtx->trySend(std::move(data)); // :3
} // :3