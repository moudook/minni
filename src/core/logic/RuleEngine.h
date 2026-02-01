#ifndef MINNI_CORE_LOGIC_RULE_ENGINE_H_
#define MINNI_CORE_LOGIC_RULE_ENGINE_H_

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <variant>

namespace minni {
namespace logic {

using RuleValue = std::variant<double, std::string>;

/**
 * A lightweight rule engine for executing decision tree rules on mobile.
 * Designed to parse simple text rules exported from Python/Scikit-learn.
 *
 * Format: "IF (var <= val) & (var2 == 'string_val') THEN output=val"
 */
class RuleEngine {
public:
    RuleEngine();
    ~RuleEngine();

    // Load rules from a raw string (one rule per line)
    bool load_rules(const std::string& rules_content);

    // Evaluate against a set of inputs. Returns the result string if a rule matches.
    // Returns std::nullopt if no rule matches.
    std::optional<std::string> evaluate(const std::map<std::string, RuleValue>& inputs);

    // Legacy overload for backward compatibility with existing tests/Java code
    std::optional<std::string> evaluate(const std::map<std::string, double>& inputs);

private:
    enum class Operator {
        LESS_THAN,
        LESS_EQUAL,
        GREATER_THAN,
        GREATER_EQUAL,
        EQUAL,
        NOT_EQUAL
    };

    struct Condition {
        std::string variable;
        Operator op;
        RuleValue value;
    };

    struct Rule {
        std::vector<Condition> conditions;
        std::string result_key;
        std::string result_value;
    };

    std::vector<Rule> rules_;

    // Helpers
    void parse_line(const std::string& line);
    Operator parse_operator(const std::string& op_str);
    bool check_condition(const Condition& cond, const RuleValue& input_val);
};

} // namespace logic
} // namespace minni

#endif // MINNI_CORE_LOGIC_RULE_ENGINE_H_
