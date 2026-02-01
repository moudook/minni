#include "SecurityManager.h"
#include <random>
#include <algorithm>

namespace minni {
namespace security {

std::vector<uint8_t> SecurityManager::encrypt(const std::vector<uint8_t>& data, const std::string& key) {
    if (key.empty()) return data;
    std::vector<uint8_t> result(data.size());

    // Simple Vigenère-like XOR cipher
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ static_cast<uint8_t>(key[i % key.size()]);
    }
    return result;
}

std::vector<uint8_t> SecurityManager::decrypt(const std::vector<uint8_t>& data, const std::string& key) {
    // XOR is symmetric, so encryption function works for decryption
    return encrypt(data, key);
}

std::string SecurityManager::encrypt_string(const std::string& input, const std::string& key) {
    std::vector<uint8_t> input_vec(input.begin(), input.end());
    std::vector<uint8_t> enc_vec = encrypt(input_vec, key);
    return std::string(enc_vec.begin(), enc_vec.end());
}

std::string SecurityManager::decrypt_string(const std::string& input, const std::string& key) {
    std::vector<uint8_t> input_vec(input.begin(), input.end());
    std::vector<uint8_t> dec_vec = decrypt(input_vec, key);
    return std::string(dec_vec.begin(), dec_vec.end());
}

std::string SecurityManager::generate_key(size_t length) {
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string key;
    key.reserve(length);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);

    for (size_t i = 0; i < length; ++i) {
        key += charset[dis(gen)];
    }
    return key;
}

} // namespace security
} // namespace minni
