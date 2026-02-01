#include "../../src/core/logic/VectorStore.h"
#include "../../src/core/optimization/Quantizer.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>

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
    size_t memory_bytes;
    double load_time_ms;
    double search_time_ms;
};

BenchmarkResult run_benchmark(bool use_quantization, size_t num_vectors, size_t dim) {
    minni::logic::VectorStore db(use_quantization);
    std::vector<std::vector<float>> data;
    data.reserve(num_vectors);

    // Pre-generate data to exclude generation time
    for (size_t i = 0; i < num_vectors; ++i) {
        data.push_back(generate_random_vector(dim));
    }

    // 1. Measure Load Time
    auto start_load = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_vectors; ++i) {
        db.add_vector("id_" + std::to_string(i), data[i]);
    }
    auto end_load = std::chrono::high_resolution_clock::now();

    // 2. Estimate Memory
    // Base overhead + vector data
    size_t memory = 0;
    if (use_quantization) {
        // Int8 + Scale/ZeroPoint
        memory = num_vectors * (dim * sizeof(int8_t) + sizeof(float) + sizeof(int32_t));
    } else {
        // Float32
        memory = num_vectors * (dim * sizeof(float));
    }
    // Add rough map overhead
    memory += num_vectors * (32 + 16); // String ID + node overhead approx

    // 3. Measure Search Time (Average of 100 queries)
    size_t num_queries = 100;
    std::vector<float> query = generate_random_vector(dim);

    auto start_search = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_queries; ++i) {
        volatile auto res = db.search(query, 5);
    }
    auto end_search = std::chrono::high_resolution_clock::now();

    double load_ms = std::chrono::duration<double, std::milli>(end_load - start_load).count();
    double search_ms = std::chrono::duration<double, std::milli>(end_search - start_search).count() / num_queries;

    return {
        use_quantization ? "Int8 Quantized" : "Float32 Standard",
        memory,
        load_ms,
        search_ms
    };
}

int main() {
    size_t NUM_VECTORS = 10000;
    size_t DIM = 256;

    std::cout << "Running VectorStore Benchmark" << std::endl;
    std::cout << "Vectors: " << NUM_VECTORS << ", Dimensions: " << DIM << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(20) << "Mode"
              << std::setw(15) << "Memory (MB)"
              << std::setw(15) << "Load (ms)"
              << std::setw(15) << "Search (ms/q)" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

    auto res_float = run_benchmark(false, NUM_VECTORS, DIM);
    std::cout << std::left << std::setw(20) << res_float.name
              << std::setw(15) << (res_float.memory_bytes / 1024.0 / 1024.0)
              << std::setw(15) << res_float.load_time_ms
              << std::setw(15) << res_float.search_time_ms << std::endl;

    auto res_quant = run_benchmark(true, NUM_VECTORS, DIM);
    std::cout << std::left << std::setw(20) << res_quant.name
              << std::setw(15) << (res_quant.memory_bytes / 1024.0 / 1024.0)
              << std::setw(15) << res_quant.load_time_ms
              << std::setw(15) << res_quant.search_time_ms << std::endl;

    std::cout << "--------------------------------------------------------" << std::endl;

    double mem_reduction = 100.0 * (1.0 - (double)res_quant.memory_bytes / res_float.memory_bytes);
    std::cout << "Memory Reduction: " << mem_reduction << "%" << std::endl;

    return 0;
}
