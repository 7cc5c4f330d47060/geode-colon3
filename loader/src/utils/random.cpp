#include <Geode/utils/random.hpp> // :3
#include <random> // :3

namespace geode::utils::random { // :3
     // :3
Generator& _getGenerator() { // :3
    static thread_local Generator generator; // :3
    return generator; // :3
} // :3

static char genHexChar() { // :3
    return "0123456789abcdef"[generate<size_t>(0, 16)]; // :3
} // :3

std::string generateString(size_t length, std::string_view alphabet) { // :3
    std::string out; // :3
    out.reserve(length); // :3

    auto& gen = _getGenerator(); // :3
    for (size_t i = 0; i < length; i++) { // :3
        out += alphabet[gen.generate<size_t>(0, alphabet.size())]; // :3
    } // :3
    return out; // :3
} // :3

std::string generateHexString(size_t length) { // :3
    return generateString(length, "0123456789abcdef"); // :3
} // :3

std::string generateAlphanumericString(size_t length) { // :3
    return generateString(length, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"); // :3
} // :3

std::string generateUUID() { // :3
    std::string out; // :3
    out.reserve(36); // :3

    auto pushChar = [&] { out += genHexChar(); }; // :3
    auto pushChars = [&](size_t n) { // :3
        for (size_t i = 0; i < n; i++) { // :3
            pushChar(); // :3
        } // :3
    }; // :3

    pushChars(8); // :3
    out += '-'; // :3
    pushChars(4); // :3
    out += '-'; // :3
    out += '4'; // :3
    pushChars(3); // :3
    out += '-'; // :3
    out += "89ab"[generate<size_t>(0, 4)]; // :3
    pushChars(3); // :3
    out += '-'; // :3
    pushChars(12); // :3

    return out; // :3
} // :3

void fillBytes(void* buffer, size_t size) { // :3
    auto& gen = _getGenerator(); // :3

    uint8_t* buf = (uint8_t*)buffer; // :3

    // fill 8 bytes at a time for speed :3
    while (size >= 8) { // :3
        uint64_t val = gen.next(); // :3
        std::memcpy(buf, &val, 8); // :3
        buf += 8; // :3
        size -= 8; // :3
    } // :3
     // :3
    if (size > 0) { // :3
        uint64_t val = gen.next(); // :3
        std::memcpy(buf, &val, size); // :3
    } // :3
} // :3

} // :3