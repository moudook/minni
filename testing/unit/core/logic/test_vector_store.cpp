#include "../../../../src/core/logic/VectorStore.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

bool float_eq(float a, float b, float epsilon = 1e-4) {
    return std::abs(a - b) < epsilon;
}

void test_vector_store_search() {
    std::cout << "Running VectorStore Search Test..." << std::endl;

    minni::logic::VectorStore db;

    // Define some simple embeddings (2D for easy visualization)
    // A: [1, 0] (East)
    // B: [0, 1] (North)
    // C: [-1, 0] (West)
    db.add_vector("A", {1.0f, 0.0f});
    db.add_vector("B", {0.0f, 1.0f});
    db.add_vector("C", {-1.0f, 0.0f});

    assert(db.size() == 3);

    // Query: [0.707, 0.707] (North-East, 45 degrees)
    // Should be closest to A and B (score ~0.707), far from C (score -0.707)
    std::vector<float> query = {0.7071f, 0.7071f};
    auto results = db.search(query, 3);

    assert(results.size() == 3);

    // Print results for debugging
    for (const auto& p : results) {
        std::cout << "ID: " << p.first << ", Score: " << p.second << std::endl;
    }

    // Top result should be A or B (approx equal)
    // Cosine sim of [1,0] and [0.7, 0.7] is 0.707
    assert(results[0].second > 0.7f);
    assert(results[1].second > 0.7f);
    // Last result should be C (negative correlation)
    assert(results[2].first == "C");
    assert(results[2].second < 0.0f);

    std::cout << "VectorStore Search Test Passed!" << std::endl;
}

void test_vector_store_empty() {
    std::cout << "Running VectorStore Empty/Mismatch Test..." << std::endl;
    minni::logic::VectorStore db;

    // Add 2D vector
    bool added = db.add_vector("A", {1.0f, 2.0f});
    assert(added);

    // Try adding 3D vector (should fail)
    bool added_mismatch = db.add_vector("B", {1.0f, 2.0f, 3.0f});
    assert(!added_mismatch);

    assert(db.size() == 1);
    std::cout << "VectorStore Empty/Mismatch Test Passed!" << std::endl;
}

int main() {
    test_vector_store_search();
    test_vector_store_empty();
    return 0;
}
