#include "../../../../src/core/logic/KnowledgeGraph.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include <cstdio> // for remove()

void test_kg_persistence_float() {
    std::cout << "Running KnowledgeGraph Persistence (Float) Test..." << std::endl;
    const std::string filename = "test_kg_float.bin";

    // 1. Create and populate
    {
        minni::logic::KnowledgeGraph kg(false);
        kg.add_fact("Alice", "knows", "Bob");
        kg.add_fact("Bob", "knows", "Charlie");

        kg.set_embedding("Alice", {1.0f, 0.0f});
        kg.set_embedding("Bob", {0.0f, 1.0f});

        assert(kg.num_entities() == 3);
        assert(kg.num_facts() == 2);

        bool saved = kg.save(filename);
        assert(saved);
    }

    // 2. Load and verify
    {
        minni::logic::KnowledgeGraph kg(false);
        bool loaded = kg.load(filename);
        assert(loaded);

        assert(kg.num_entities() == 3);
        assert(kg.num_facts() == 2);
        assert(kg.has_entity("Alice"));
        assert(kg.has_entity("Charlie"));

        // Check embedding
        auto vec = kg.get_embedding("Alice");
        assert(!vec.empty());
        assert(vec[0] == 1.0f);
        assert(vec[1] == 0.0f);
    }

    // Cleanup
    std::remove(filename.c_str());
    std::cout << "KnowledgeGraph Persistence (Float) Test Passed!" << std::endl;
}

void test_kg_persistence_quantized() {
    std::cout << "Running KnowledgeGraph Persistence (Quantized) Test..." << std::endl;
    const std::string filename = "test_kg_quant.bin";

    // 1. Create and populate
    {
        minni::logic::KnowledgeGraph kg(true); // Quantized
        kg.add_fact("X", "linked_to", "Y");

        // [1.0, -1.0] -> Quantized approx [127, -128]
        kg.set_embedding("X", {1.0f, -1.0f});

        bool saved = kg.save(filename);
        assert(saved);
    }

    // 2. Load and verify
    {
        minni::logic::KnowledgeGraph kg(true);
        bool loaded = kg.load(filename);
        assert(loaded);

        assert(kg.has_entity("X"));
        assert(kg.has_entity("Y"));
        assert(kg.num_facts() == 1);

        // Check embedding (dequantized)
        auto vec = kg.get_embedding("X");
        assert(!vec.empty());
        assert(vec.size() == 2);

        // Error check
        assert(std::abs(vec[0] - 1.0f) < 0.05f);
        assert(std::abs(vec[1] - (-1.0f)) < 0.05f);
    }

    // Cleanup
    std::remove(filename.c_str());
    std::cout << "KnowledgeGraph Persistence (Quantized) Test Passed!" << std::endl;
}

int main() {
    test_kg_persistence_float();
    test_kg_persistence_quantized();
    return 0;
}
