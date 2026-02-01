#include "../../../../src/core/logic/RuleEngine.h"
#include <iostream>
#include <cassert>
#include <map>
#include <variant>

void test_string_rules() {
    std::cout << "Running RuleEngine String Support Test..." << std::endl;

    minni::logic::RuleEngine engine;

    // Rule with string comparison
    // IF (user_role == 'admin') & (action == 'delete') THEN allow=true
    std::string rules = "IF (user_role == 'admin') & (action == 'delete') THEN allow=true\n"
                        "IF (user_role == 'guest') THEN allow=false";

    bool loaded = engine.load_rules(rules);
    assert(loaded);

    // Test Case 1: Admin Delete
    std::map<std::string, minni::logic::RuleValue> inputs1;
    inputs1["user_role"] = "admin";
    inputs1["action"] = "delete";

    auto result1 = engine.evaluate(inputs1);
    assert(result1.has_value());
    assert(result1.value() == "true");

    // Test Case 2: Guest
    std::map<std::string, minni::logic::RuleValue> inputs2;
    inputs2["user_role"] = "guest";
    inputs2["action"] = "view"; // Irrelevant for the guest rule as written, but good to have

    auto result2 = engine.evaluate(inputs2);
    assert(result2.has_value());
    assert(result2.value() == "false");

    // Test Case 3: Mixed Types (Age > 18 AND country == 'US')
    std::string mixed_rules = "IF (age >= 18) & (country == 'US') THEN can_vote=true";
    engine.load_rules(mixed_rules);

    std::map<std::string, minni::logic::RuleValue> inputs3;
    inputs3["age"] = 20.0;
    inputs3["country"] = "US";

    auto result3 = engine.evaluate(inputs3);
    assert(result3.has_value());
    assert(result3.value() == "true");

    std::cout << "RuleEngine String Support Test Passed!" << std::endl;
}

int main() {
    test_string_rules();
    return 0;
}
