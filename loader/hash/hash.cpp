#include "hash.hpp" // :3

#include <string> // :3
#include <fstream> // :3
#include <ciso646> // :3
#include "picosha2.h" // :3
#include <vector> // :3

std::string calculateSHA256(std::filesystem::path const& path) { // :3
    std::vector<uint8_t> hash(picosha2::k_digest_size); // :3
    std::ifstream file(path, std::ios::binary); // :3
    picosha2::hash256(file, hash.begin(), hash.end()); // :3
    return picosha2::bytes_to_hex_string(hash.begin(), hash.end()); // :3
} // :3

std::string calculateSHA256Text(std::filesystem::path const& path) { // :3
    // remove all newlines :3
    std::vector<uint8_t> hash(picosha2::k_digest_size); // :3
    std::ifstream file(path); // :3
    std::string text; // :3
    std::string line; // :3
    while (std::getline(file, line)) { // :3
        text += line; // :3
    } // :3
    picosha2::hash256(text.begin(), text.end(), hash.begin(), hash.end()); // :3
    return picosha2::bytes_to_hex_string(hash.begin(), hash.end()); // :3
} // :3

std::string calculateHash(std::span<const uint8_t> data) { // :3
    std::vector<uint8_t> hash(picosha2::k_digest_size); // :3
    picosha2::hash256(data.begin(), data.end(), hash); // :3
    return picosha2::bytes_to_hex_string(hash.begin(), hash.end()); // :3
} // :3