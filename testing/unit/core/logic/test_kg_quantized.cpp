#include "../../../../src/core/logic/KnowledgeGraph.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

void test_kg_quantization() {
    std::cout << "Running KnowledgeGraph Quantization Test..." << std::endl;

    // Initialize with quantization enabled
    minni::logic::KnowledgeGraph kg(true);

    kg.add_entity("King");
    kg.add_entity("Queen");
    kg.add_entity("Apple");

    // Vectors: [Royal, Female, Fruit]
    // King: [1, 0, 0]
    // Queen: [1, 1, 0] -> Normalized approx [0.707, 0.707, 0]
    // Apple: [0, 0, 1]

    kg.set_embedding("King", {1.0f, 0.0f, 0.0f});
    kg.set_embedding("Queen", {0.7071f, 0.7071f, 0.0f});
    kg.set_embedding("Apple", {0.0f, 0.0f, 1.0f});

    // Check retrieval (dequantized)
    auto vec_king = kg.get_embedding("King");
    assert(!vec_king.empty());
    // Error due to quantization should be small
    assert(std::abs(vec_king[0] - 1.0f) < 0.05f);

    // Query for "Female Royalty" -> [0.707, 0.707, 0]
    std::vector<float> query = {0.7071f, 0.7071f, 0.0f};
    auto results = kg.find_similar_entities(query, 2);

    assert(results.size() == 2);

    std::cout << "Top match: " << results[0].first << " Score: " << results[0].second << std::endl;
    std::cout << "2nd match: " << results[1].first << " Score: " << results[1].second << std::endl;

    // Queen should be top match
    assert(results[0].first == "Queen");
    // Score should be close to 1.0
    assert(results[0].second > 0.95f);

    // King should be second (partial match)
    assert(results[1].first == "King");
    // Cosine([0.707, 0.707, 0], [1, 0, 0]) = 0.707
    assert(results[1].second > 0.65f);

    std::cout << "KnowledgeGraph Quantization Test Passed!" << std::endl;
}

int main() {
    test_kg_quantization();
    return 0;
}
