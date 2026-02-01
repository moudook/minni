#include "../../../../src/core/logic/KnowledgeGraph.h"
#include <iostream>
#include <cassert>
#include <vector>

void test_kg_embeddings() {
    std::cout << "Running KG Embeddings Test..." << std::endl;

    minni::logic::KnowledgeGraph kg;

    kg.add_entity("King");
    kg.add_entity("Queen");
    kg.add_entity("Apple");

    // Simple 2D vectors: Royalty [1, 1], Food [0, 1] - rough approximation for test
    // Let's use 3D: [Royal, Female, Fruit]
    // King: [1, 0, 0]
    // Queen: [1, 1, 0]
    // Apple: [0, 0, 1]

    kg.set_embedding("King", {1.0f, 0.0f, 0.0f});
    kg.set_embedding("Queen", {1.0f, 1.0f, 0.0f}); // Normalized: [0.7, 0.7, 0] approx
    kg.set_embedding("Apple", {0.0f, 0.0f, 1.0f});

    // Query for "Female Royalty" -> [1, 1, 0]
    std::vector<float> query = {1.0f, 1.0f, 0.0f};
    auto results = kg.find_similar_entities(query, 2);

    assert(results.size() == 2);
    // Queen should be top match (exact direction)
    assert(results[0].first == "Queen");
    // King should be second (partial match on Royal dimension)
    assert(results[1].first == "King");

    std::cout << "KG Embeddings Test Passed!" << std::endl;
}

int main() {
    test_kg_embeddings();
    return 0;
}
