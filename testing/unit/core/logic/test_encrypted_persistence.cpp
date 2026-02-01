#include "logic/VectorStore.h"
#include "logic/KnowledgeGraph.h"
#include "security/SecurityManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cassert>

using namespace minni::logic;

void test_vector_store_encryption() {
    std::cout << "Testing VectorStore Encryption..." << std::endl;
    std::string filename = "test_encrypted_vs.bin";
    std::string key = "MySecretKey";

    // 1. Create and populate store
    {
        VectorStore store(false);
        store.add_vector("item1", {0.1f, 0.2f, 0.3f});
        bool saved = store.save(filename, key);
        assert(saved);
    }

    // 2. Try loading without key (should fail)
    {
        VectorStore store(false);
        bool loaded = store.load(filename);
        assert(!loaded);
    }

    // 3. Try loading with wrong key (should fail or produce garbage that fails validation)
    {
        VectorStore store(false);
        bool loaded = store.load(filename, "WrongKey");
        // Depending on implementation, decrypt might succeed but header check fails
        assert(!loaded);
    }

    // 4. Load with correct key
    {
        VectorStore store(false);
        bool loaded = store.load(filename, key);
        assert(loaded);
        assert(store.size() == 1);
        auto results = store.search({0.1f, 0.2f, 0.3f}, 1);
        assert(!results.empty());
        assert(results[0].first == "item1");
    }

    std::remove(filename.c_str());
    std::cout << "  VectorStore Encryption passed." << std::endl;
}

void test_knowledge_graph_encryption() {
    std::cout << "Testing KnowledgeGraph Encryption..." << std::endl;
    std::string filename = "test_encrypted_kg.bin";
    std::string key = "AnotherSecretKey";

    // 1. Create and populate graph
    {
        KnowledgeGraph kg(false);
        kg.add_fact("Alice", "knows", "Bob");
        kg.add_fact("Bob", "knows", "Charlie");
        bool saved = kg.save(filename, key);
        assert(saved);
    }

    // 2. Try loading without key
    {
        KnowledgeGraph kg(false);
        bool loaded = kg.load(filename);
        assert(!loaded);
    }

    // 3. Try loading with wrong key
    {
        KnowledgeGraph kg(false);
        bool loaded = kg.load(filename, "WrongKey");
        assert(!loaded);
    }

    // 4. Load with correct key
    {
        KnowledgeGraph kg(false);
        bool loaded = kg.load(filename, key);
        assert(loaded);
        assert(kg.num_facts() == 2);
        assert(kg.has_entity("Alice"));
    }

    std::remove(filename.c_str());
    std::cout << "  KnowledgeGraph Encryption passed." << std::endl;
}

int main() {
    test_vector_store_encryption();
    test_knowledge_graph_encryption();
    std::cout << "All Encryption Persistence tests passed!" << std::endl;
    return 0;
}
