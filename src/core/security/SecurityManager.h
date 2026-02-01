#ifndef MINNI_CORE_SECURITY_SECURITY_MANAGER_H_
#define MINNI_CORE_SECURITY_SECURITY_MANAGER_H_

#include <string>
#include <vector>
#include <cstdint>

namespace minni {
namespace security {

/**
 * Manages data security operations including encryption and decryption.
 * Currently implements a lightweight symmetric stream cipher (Rotating XOR)
 * suitable for mobile devices where low latency is critical.
 *
 * Note: For production use with high-value data, this should be replaced
 * with AES-GCM via Android Keystore / OpenSSL.
 */
class SecurityManager {
public:
    /**
     * Encrypts data using the provided key.
     * @param data Input data buffer.
     * @param key Encryption key.
     * @return Encrypted data.
     */
    static std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data, const std::string& key);

    /**
     * Decrypts data using the provided key.
     * @param data Encrypted data buffer.
     * @param key Decryption key.
     * @return Decrypted data.
     */
    static std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data, const std::string& key);

    /**
     * Encrypts a string.
     */
    static std::string encrypt_string(const std::string& input, const std::string& key);

    /**
     * Decrypts a string.
     */
    static std::string decrypt_string(const std::string& input, const std::string& key);

    /**
     * Generates a random alphanumeric key of specified length.
     */
    static std::string generate_key(size_t length = 32);
};

} // namespace security
} // namespace minni

#endif // MINNI_CORE_SECURITY_SECURITY_MANAGER_H_
