#include "../../../../src/core/platform/MemoryMapper.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstring>
#include <cstdio> // for remove()

void test_memory_mapping() {
    std::cout << "Running MemoryMapper Test..." << std::endl;
    const std::string filename = "test_mmap.bin";
    const std::string content = "Hello, Memory Mapped World!";

    // 1. Create a dummy file
    {
        std::ofstream out(filename, std::ios::binary);
        out << content;
        out.close();
    }

    // 2. Map file
    minni::platform::MemoryMapper mapper;
    bool success = mapper.map(filename);
    assert(success);
    assert(mapper.is_mapped());
    assert(mapper.size() == content.size());

    // 3. Verify content
    const char* data = static_cast<const char*>(mapper.data());
    std::string mapped_content(data, mapper.size());
    assert(mapped_content == content);

    // 4. Unmap explicitly
    mapper.unmap();
    assert(!mapper.is_mapped());
    assert(mapper.data() == nullptr);

    // 5. Cleanup
    std::remove(filename.c_str());
    std::cout << "MemoryMapper Test Passed!" << std::endl;
}

int main() {
    test_memory_mapping();
    return 0;
}
