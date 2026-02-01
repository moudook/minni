#include "../../../../src/core/logic/VectorStore.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

bool approx_eq(float a, float b, float epsilon = 0.05f) { // Higher epsilon for quantization error
    return std::abs(a - b) < epsilon;
}

void test_quantized_storage() {
    std::cout << "Running VectorStore Quantization Test..." << std::endl;

    // Initialize with quantization enabled
    minni::logic::VectorStore db(true);

    // Add vectors (same as regular test)
    // A: [1, 0]
    // B: [0, 1]
    // C: [-1, 0]
    db.add_vector("A", {1.0f, 0.0f});
    db.add_vector("B", {0.0f, 1.0f});
    db.add_vector("C", {-1.0f, 0.0f});

    assert(db.size() == 3);

    // Query: [0.707, 0.707]
    std::vector<float> query = {0.7071f, 0.7071f};
    auto results = db.search(query, 3);

    assert(results.size() == 3);

    // Print results
    for (const auto& p : results) {
        std::cout << "ID: " << p.first << ", Score: " << p.second << std::endl;
    }

    // Scores should be roughly similar to float32 version, but with some quantization error.
    // Ideally > 0.65 for A and B.
    assert(results[0].second > 0.65f);
    assert(results[1].second > 0.65f);
    assert(results[2].first == "C");
    assert(results[2].second < 0.0f);

    std::cout << "VectorStore Quantization Test Passed!" << std::endl;
}

int main() {
    test_quantized_storage();
    return 0;
}
