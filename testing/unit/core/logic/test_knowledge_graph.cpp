#include "../../../../src/core/logic/KnowledgeGraph.h"
#include <iostream>
#include <cassert>
#include <vector>

void test_knowledge_graph_basics() {
    std::cout << "Running KnowledgeGraph Basics Test..." << std::endl;

    minni::logic::KnowledgeGraph kg;

    // Test Entity Addition
    auto alice = kg.add_entity("Alice");
    auto bob = kg.add_entity("Bob");
    assert(alice != bob);
    assert(kg.get_entity_name(alice) == "Alice");
    assert(kg.num_entities() == 2);

    // Test Relation Addition
    auto knows = kg.add_relation("knows");
    assert(kg.get_relation_name(knows) == "knows");

    // Test Fact Addition
    kg.add_fact(alice, knows, bob);
    assert(kg.num_facts() == 1);

    // Test Query (Object)
    std::vector<minni::logic::EntityId> friends_of_alice = kg.query_objects(alice, knows);
    assert(friends_of_alice.size() == 1);
    assert(friends_of_alice[0] == bob);

    // Test Query (Subject)
    std::vector<minni::logic::EntityId> who_knows_bob = kg.query_subjects(knows, bob);
    assert(who_knows_bob.size() == 1);
    assert(who_knows_bob[0] == alice);

    std::cout << "KnowledgeGraph Basics Test Passed!" << std::endl;
}

void test_knowledge_graph_string_api() {
    std::cout << "Running KnowledgeGraph String API Test..." << std::endl;

    minni::logic::KnowledgeGraph kg;
    kg.add_fact("Sky", "is", "Blue");

    assert(kg.has_entity("Sky"));
    assert(kg.has_entity("Blue"));
    assert(kg.num_facts() == 1);

    std::cout << "KnowledgeGraph String API Test Passed!" << std::endl;
}

int main() {
    test_knowledge_graph_basics();
    test_knowledge_graph_string_api();
    return 0;
}
