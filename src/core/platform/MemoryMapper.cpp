#include "MemoryMapper.h"
#include <utility>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace minni {
namespace platform {

MemoryMapper::MemoryMapper() = default;

MemoryMapper::~MemoryMapper() {
    unmap();
}

MemoryMapper::MemoryMapper(MemoryMapper&& other) noexcept {
    *this = std::move(other);
}

MemoryMapper& MemoryMapper::operator=(MemoryMapper&& other) noexcept {
    if (this != &other) {
        unmap();
        mapped_ptr_ = other.mapped_ptr_;
        mapped_size_ = other.mapped_size_;
#ifdef _WIN32
        file_handle_ = other.file_handle_;
        map_handle_ = other.map_handle_;
        other.file_handle_ = nullptr;
        other.map_handle_ = nullptr;
#else
        fd_ = other.fd_;
        other.fd_ = -1;
#endif
        other.mapped_ptr_ = nullptr;
        other.mapped_size_ = 0;
    }
    return *this;
}

bool MemoryMapper::map(const std::string& path) {
    unmap(); // Cleanup existing map if any

#ifdef _WIN32
    // Windows Implementation
    file_handle_ = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_handle_ == INVALID_HANDLE_VALUE || file_handle_ == nullptr) {
        file_handle_ = nullptr;
        return false;
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(file_handle_, &fileSize)) {
        CloseHandle(file_handle_);
        file_handle_ = nullptr;
        return false;
    }
    mapped_size_ = static_cast<size_t>(fileSize.QuadPart);

    // Handle empty files
    if (mapped_size_ == 0) {
        CloseHandle(file_handle_);
        file_handle_ = nullptr;
        return false;
    }

    map_handle_ = CreateFileMappingA(file_handle_, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (map_handle_ == nullptr) {
        CloseHandle(file_handle_);
        file_handle_ = nullptr;
        return false;
    }

    mapped_ptr_ = MapViewOfFile(map_handle_, FILE_MAP_READ, 0, 0, 0);
    if (mapped_ptr_ == nullptr) {
        CloseHandle(map_handle_);
        CloseHandle(file_handle_);
        map_handle_ = nullptr;
        file_handle_ = nullptr;
        mapped_size_ = 0;
        return false;
    }

    return true;

#else
    // POSIX Implementation
    fd_ = open(path.c_str(), O_RDONLY);
    if (fd_ == -1) {
        return false;
    }

    struct stat sb;
    if (fstat(fd_, &sb) == -1) {
        close(fd_);
        fd_ = -1;
        return false;
    }

    mapped_size_ = static_cast<size_t>(sb.st_size);

    if (mapped_size_ == 0) {
        close(fd_);
        fd_ = -1;
        return false;
    }

    mapped_ptr_ = mmap(nullptr, mapped_size_, PROT_READ, MAP_PRIVATE, fd_, 0);

    if (mapped_ptr_ == MAP_FAILED) {
        mapped_ptr_ = nullptr;
        mapped_size_ = 0;
        close(fd_);
        fd_ = -1;
        return false;
    }

    return true;
#endif
}

void MemoryMapper::unmap() {
    if (mapped_ptr_) {
#ifdef _WIN32
        UnmapViewOfFile(mapped_ptr_);
#else
        munmap(mapped_ptr_, mapped_size_);
#endif
        mapped_ptr_ = nullptr;
    }
    mapped_size_ = 0;

#ifdef _WIN32
    if (map_handle_) {
        CloseHandle(map_handle_);
        map_handle_ = nullptr;
    }
    if (file_handle_) {
        CloseHandle(file_handle_);
        file_handle_ = nullptr;
    }
#else
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
#endif
}

const void* MemoryMapper::data() const {
    return mapped_ptr_;
}

size_t MemoryMapper::size() const {
    return mapped_size_;
}

bool MemoryMapper::is_mapped() const {
    return mapped_ptr_ != nullptr;
}

} // namespace platform
} // namespace minni
