#include "../../../../src/core/logic/VectorStore.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include <cstdio> // for remove()

void test_persistence_float() {
    std::cout << "Running VectorStore Persistence (Float) Test..." << std::endl;
    const std::string filename = "test_vs_float.bin";

    // 1. Create and populate
    {
        minni::logic::VectorStore db(false);
        db.add_vector("A", {1.0f, 0.0f});
        db.add_vector("B", {0.0f, 1.0f});
        assert(db.size() == 2);

        bool saved = db.save(filename);
        assert(saved);
    }

    // 2. Load and verify
    {
        minni::logic::VectorStore db(false);
        bool loaded = db.load(filename);
        assert(loaded);
        assert(db.size() == 2);

        // Search
        std::vector<float> query = {1.0f, 0.0f};
        auto results = db.search(query, 1);
        assert(results.size() == 1);
        assert(results[0].first == "A");
        assert(std::abs(results[0].second - 1.0f) < 1e-5);
    }

    // Cleanup
    std::remove(filename.c_str());
    std::cout << "VectorStore Persistence (Float) Test Passed!" << std::endl;
}

void test_persistence_quantized() {
    std::cout << "Running VectorStore Persistence (Quantized) Test..." << std::endl;
    const std::string filename = "test_vs_quant.bin";

    // 1. Create and populate
    {
        minni::logic::VectorStore db(true);
        db.add_vector("A", {1.0f, 0.0f}); // [127, -128] approx
        db.add_vector("B", {0.0f, 1.0f});
        assert(db.size() == 2);

        bool saved = db.save(filename);
        assert(saved);
    }

    // 2. Load and verify
    {
        minni::logic::VectorStore db(true);
        bool loaded = db.load(filename);
        assert(loaded);
        assert(db.size() == 2);

        // Search
        std::vector<float> query = {1.0f, 0.0f};
        auto results = db.search(query, 1);
        assert(results.size() == 1);
        assert(results[0].first == "A");
        // Quantization noise is expected, but should be high similarity
        assert(results[0].second > 0.9f);
    }

    // Cleanup
    std::remove(filename.c_str());
    std::cout << "VectorStore Persistence (Quantized) Test Passed!" << std::endl;
}

int main() {
    test_persistence_float();
    test_persistence_quantized();
    return 0;
}
