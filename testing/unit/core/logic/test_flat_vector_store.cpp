#include "../../../../src/core/logic/VectorStore.h"
#include "../../../../src/core/logic/FlatVectorStore.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include <cstdio> // for remove()

void test_flat_vector_store() {
    std::cout << "Running FlatVectorStore Test..." << std::endl;
    const std::string filename = "test_flat.bin";

    // 1. Create a standard VectorStore and save it as "Flat"
    {
        minni::logic::VectorStore db(false);
        db.add_vector("A", {1.0f, 0.0f});
        db.add_vector("B", {0.0f, 1.0f});
        db.add_vector("C", {-1.0f, 0.0f});

        bool saved = db.save_flat(filename);
        assert(saved);
    }

    // 2. Load with FlatVectorStore (Zero-Copy)
    {
        minni::logic::FlatVectorStore flat_db;
        bool loaded = flat_db.load(filename);
        assert(loaded);
        assert(flat_db.size() == 3);

        // 3. Search
        // Query: [0.707, 0.707] (Should match A and B equally)
        std::vector<float> query = {0.7071f, 0.7071f};
        auto results = flat_db.search(query, 3);

        assert(results.size() == 3);

        std::cout << "Top match: " << results[0].first << " Score: " << results[0].second << std::endl;

        // Results might not be strictly sorted if scores are identical, but let's check values
        bool foundA = false;
        bool foundB = false;
        for(const auto& r : results) {
            if (r.first == "A" && r.second > 0.99f) foundA = true; // wait, dot(A, Q) = 0.707
            if (r.first == "A" && r.second > 0.65f) foundA = true;
            if (r.first == "B" && r.second > 0.65f) foundB = true;
        }

        assert(foundA);
        assert(foundB);
    }

    // Cleanup
    std::remove(filename.c_str());
    std::cout << "FlatVectorStore Test Passed!" << std::endl;
}

int main() {
    test_flat_vector_store();
    return 0;
}
