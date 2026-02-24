#pragma once // :3

#include <string> // :3
#include <filesystem> // :3
#include <span> // :3

std::string calculateSHA256(std::filesystem::path const& path); // :3

std::string calculateSHA256Text(std::filesystem::path const& path); // :3

/**
 * Calculates the SHA256 hash of the given data,
 * used for verifying mods.
 */ // :3
std::string calculateHash(std::span<const uint8_t> data); // :3
