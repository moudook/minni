#ifndef MINNI_CORE_PLATFORM_MEMORY_MAPPER_H_
#define MINNI_CORE_PLATFORM_MEMORY_MAPPER_H_

#include <string>
#include <cstddef>

namespace minni {
namespace platform {

/**
 * A utility class for memory mapping files (mmap).
 * Enables zero-copy loading of large model files, essential for Android memory efficiency.
 * RAII-compliant: automatically unmaps on destruction.
 */
class MemoryMapper {
public:
    MemoryMapper();
    ~MemoryMapper();

    // Disable copying to prevent double-free
    MemoryMapper(const MemoryMapper&) = delete;
    MemoryMapper& operator=(const MemoryMapper&) = delete;

    // Allow moving
    MemoryMapper(MemoryMapper&& other) noexcept;
    MemoryMapper& operator=(MemoryMapper&& other) noexcept;

    /**
     * Map a file into memory (Read-Only).
     * @param path Path to the file.
     * @return true if successful.
     */
    bool map(const std::string& path);

    /**
     * Unmap the currently mapped file.
     */
    void unmap();

    /**
     * Get pointer to the mapped data.
     */
    const void* data() const;

    /**
     * Get size of the mapped data.
     */
    size_t size() const;

    /**
     * Check if a file is currently mapped.
     */
    bool is_mapped() const;

private:
    void* mapped_ptr_ = nullptr;
    size_t mapped_size_ = 0;

#ifdef _WIN32
    void* file_handle_ = nullptr; // HANDLE
    void* map_handle_ = nullptr;  // HANDLE
#else
    int fd_ = -1;
#endif
};

} // namespace platform
} // namespace minni

#endif // MINNI_CORE_PLATFORM_MEMORY_MAPPER_H_
