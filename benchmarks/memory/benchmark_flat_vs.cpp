#include "../../src/core/logic/VectorStore.h"
#include "../../src/core/logic/FlatVectorStore.h"
#include "../../src/core/optimization/Quantizer.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>
#include <cstdio>

// Simple random generator
std::vector<float> generate_random_vector(size_t dim) {
    static std::mt19937 gen(42);
    static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    std::vector<float> vec(dim);
    for (size_t i = 0; i < dim; ++i) {
        vec[i] = dis(gen);
    }
    return vec;
}

struct BenchmarkResult {
    std::string name;
    double load_time_ms;
    double search_time_ms;
};

void run_benchmark() {
    const size_t NUM_VECTORS = 50000; // Large enough to see mmap benefits
    const size_t DIM = 128;
    const std::string filename = "benchmark_flat.bin";

    std::cout << "Preparing Benchmark Data (" << NUM_VECTORS << " vectors)..." << std::endl;

    // 1. Prepare Data
    std::vector<std::vector<float>> data;
    data.reserve(NUM_VECTORS);
    for (size_t i = 0; i < NUM_VECTORS; ++i) {
        data.push_back(generate_random_vector(DIM));
    }

    std::vector<float> query = generate_random_vector(DIM);

    // 2. Create and Save Flat File
    {
        minni::logic::VectorStore db(true); // Quantized
        for (size_t i = 0; i < NUM_VECTORS; ++i) {
            db.add_vector("id_" + std::to_string(i), data[i]);
        }
        db.save_flat(filename);
    }

    std::cout << "Running Comparison..." << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(25) << "Mode"
              << std::setw(15) << "Load (ms)"
              << std::setw(15) << "Search (ms/q)" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

    // 3. Test Heap-Based Load (Standard VectorStore needs standard format,
    //    but let's simulate heap load by reading flat file manually or just re-populating)
    //    Actually, VectorStore::load() supports the custom binary format, not flat format.
    //    So we can't directly compare loading the *same file*.
    //    Instead, we'll measure the time to *populate* the heap vs *map* the file.

    //    Wait, to be fair, we should save in standard format too.
    const std::string filename_std = "benchmark_std.bin";
    {
        minni::logic::VectorStore db(true);
        for (size_t i = 0; i < NUM_VECTORS; ++i) {
            db.add_vector("id_" + std::to_string(i), data[i]);
        }
        db.save(filename_std);
    }

    // Benchmark 1: Heap Load (Standard)
    {
        minni::logic::VectorStore db(true);
        auto start = std::chrono::high_resolution_clock::now();
        bool loaded = db.load(filename_std);
        auto end = std::chrono::high_resolution_clock::now();

        if (!loaded) std::cerr << "Failed to load standard file" << std::endl;

        // Search
        auto start_s = std::chrono::high_resolution_clock::now();
        for(int i=0; i<10; ++i) volatile auto res = db.search(query, 5);
        auto end_s = std::chrono::high_resolution_clock::now();

        std::cout << std::left << std::setw(25) << "Heap (Standard Load)"
                  << std::setw(15) << std::chrono::duration<double, std::milli>(end - start).count()
                  << std::setw(15) << std::chrono::duration<double, std::milli>(end_s - start_s).count() / 10.0
                  << std::endl;
    }

    // Benchmark 2: Zero-Copy Load (Flat/mmap)
    {
        minni::logic::FlatVectorStore db;
        auto start = std::chrono::high_resolution_clock::now();
        bool loaded = db.load(filename);
        auto end = std::chrono::high_resolution_clock::now();

        if (!loaded) std::cerr << "Failed to load flat file" << std::endl;

        // Search
        auto start_s = std::chrono::high_resolution_clock::now();
        for(int i=0; i<10; ++i) volatile auto res = db.search(query, 5);
        auto end_s = std::chrono::high_resolution_clock::now();

        std::cout << std::left << std::setw(25) << "Zero-Copy (mmap)"
                  << std::setw(15) << std::chrono::duration<double, std::milli>(end - start).count()
                  << std::setw(15) << std::chrono::duration<double, std::milli>(end_s - start_s).count() / 10.0
                  << std::endl;
    }

    std::cout << "--------------------------------------------------------" << std::endl;

    // Cleanup
    std::remove(filename.c_str());
    std::remove(filename_std.c_str());
}

int main() {
    run_benchmark();
    return 0;
}
