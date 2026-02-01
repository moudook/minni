#include "security/SecurityManager.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

using namespace minni::security;

void test_key_generation() {
    std::cout << "Testing Key Generation..." << std::endl;
    std::string key1 = SecurityManager::generate_key(16);
    std::string key2 = SecurityManager::generate_key(16);
    std::string key3 = SecurityManager::generate_key(32);

    assert(key1.length() == 16);
    assert(key2.length() == 16);
    assert(key3.length() == 32);
    assert(key1 != key2); // Extremely unlikely to be same
    std::cout << "  Key Gen passed." << std::endl;
}

void test_string_encryption() {
    std::cout << "Testing String Encryption..." << std::endl;
    std::string original = "Hello, World! This is a secret message.";
    std::string key = SecurityManager::generate_key(16);

    std::string encrypted = SecurityManager::encrypt_string(original, key);
    std::string decrypted = SecurityManager::decrypt_string(encrypted, key);

    assert(original != encrypted);
    assert(original == decrypted);

    // Test with wrong key
    std::string wrong_key = SecurityManager::generate_key(16);
    std::string wrong_decrypted = SecurityManager::decrypt_string(encrypted, wrong_key);
    assert(original != wrong_decrypted);

    std::cout << "  String Encryption passed." << std::endl;
}

void test_binary_encryption() {
    std::cout << "Testing Binary Encryption..." << std::endl;
    std::vector<uint8_t> data = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0xFF, 0x42};
    std::string key = "SecretKey123";

    std::vector<uint8_t> encrypted = SecurityManager::encrypt(data, key);
    std::vector<uint8_t> decrypted = SecurityManager::decrypt(encrypted, key);

    assert(data.size() == encrypted.size());
    assert(data != encrypted);
    assert(data == decrypted);

    std::cout << "  Binary Encryption passed." << std::endl;
}

int main() {
    std::cout << "Running SecurityManager Tests..." << std::endl;
    test_key_generation();
    test_string_encryption();
    test_binary_encryption();
    std::cout << "All SecurityManager tests passed!" << std::endl;
    return 0;
}
