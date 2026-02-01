#include "../../../../src/core/logic/RuleEngine.h"
#include <iostream>
#include <cassert>
#include <map>

void test_rule_engine_basic() {
    std::cout << "Running Rule Engine Basic Test..." << std::endl;

    minni::logic::RuleEngine engine;

    // A simple rule set simulating the Iris decision tree format
    // Note: The parser expects "IF (var op val) & ... THEN key=val"
    std::string rules =
        "IF (petal_width <= 0.80) THEN Class=0\n"
        "IF (petal_width > 0.80) & (petal_width <= 1.75) THEN Class=1\n"
        "IF (petal_width > 1.75) THEN Class=2\n";

    bool success = engine.load_rules(rules);
    assert(success);

    // Case 1: Class 0
    std::map<std::string, double> inputs1;
    inputs1["petal_width"] = 0.5;
    auto result1 = engine.evaluate(inputs1);
    assert(result1.has_value());
    assert(result1.value() == "0");

    // Case 2: Class 1
    std::map<std::string, double> inputs2;
    inputs2["petal_width"] = 1.5;
    auto result2 = engine.evaluate(inputs2);
    assert(result2.has_value());
    assert(result2.value() == "1");

    // Case 3: Class 2
    std::map<std::string, double> inputs3;
    inputs3["petal_width"] = 2.0;
    auto result3 = engine.evaluate(inputs3);
    assert(result3.has_value());
    assert(result3.value() == "2");

    std::cout << "Rule Engine Basic Test Passed!" << std::endl;
}

void test_rule_engine_complex() {
    std::cout << "Running Rule Engine Complex Test..." << std::endl;

    minni::logic::RuleEngine engine;

    // Test multiple conditions
    std::string rules =
        "IF (temp > 37.5) & (cough == 1.0) THEN Diagnosis=Flu\n"
        "IF (temp <= 37.5) & (cough == 1.0) THEN Diagnosis=Cold\n"
        "IF (cough == 0.0) THEN Diagnosis=Healthy\n";

    engine.load_rules(rules);

    std::map<std::string, double> patient1 = {{"temp", 38.0}, {"cough", 1.0}};
    assert(engine.evaluate(patient1).value() == "Flu");

    std::map<std::string, double> patient2 = {{"temp", 37.0}, {"cough", 1.0}};
    assert(engine.evaluate(patient2).value() == "Cold");

    std::map<std::string, double> patient3 = {{"temp", 39.0}, {"cough", 0.0}};
    assert(engine.evaluate(patient3).value() == "Healthy");

    std::cout << "Rule Engine Complex Test Passed!" << std::endl;
}

int main() {
    test_rule_engine_basic();
    test_rule_engine_complex();
    return 0;
}
