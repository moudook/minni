#include "RuleEngine.h"
#include <sstream>
#include <iostream>
#include <algorithm>

namespace minni {
namespace logic {

RuleEngine::RuleEngine() = default;
RuleEngine::~RuleEngine() = default;

// Helper to trim string
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return str;
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool RuleEngine::load_rules(const std::string& rules_content) {
    rules_.clear();
    std::stringstream ss(rules_content);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty()) {
            parse_line(line);
        }
    }
    return !rules_.empty();
}

RuleEngine::Operator RuleEngine::parse_operator(const std::string& op_str) {
    if (op_str == "<") return Operator::LESS_THAN;
    if (op_str == "<=") return Operator::LESS_EQUAL;
    if (op_str == ">") return Operator::GREATER_THAN;
    if (op_str == ">=") return Operator::GREATER_EQUAL;
    if (op_str == "==") return Operator::EQUAL;
    if (op_str == "!=") return Operator::NOT_EQUAL;
    return Operator::EQUAL; // Default fallback
}

void RuleEngine::parse_line(const std::string& line) {
    // Expected format: "IF (var <= val) & (var2 > val2) THEN Class=1"

    // 1. Split into Premise (IF ...) and Consequent (THEN ...)
    size_t then_pos = line.find("THEN");
    if (then_pos == std::string::npos) return;

    std::string premise_str = line.substr(0, then_pos);
    std::string consequent_str = line.substr(then_pos + 4); // Skip "THEN"

    // Remove "IF" from premise
    size_t if_pos = premise_str.find("IF");
    if (if_pos != std::string::npos) {
        premise_str = premise_str.substr(if_pos + 2);
    }

    Rule rule;

    // 2. Parse Premise conditions split by '&'
    std::stringstream premise_ss(premise_str);
    std::string segment;
    while (std::getline(premise_ss, segment, '&')) {
        segment = trim(segment);
        // segment looks like "(petal width (cm) <= 0.80)"

        // Remove parens
        if (segment.front() == '(') segment = segment.substr(1);
        if (segment.back() == ')') segment.pop_back();

        // Find operator (naive implementation, checks longest first)
        std::string op_str;
        size_t op_pos = std::string::npos;

        std::vector<std::string> ops = {"<=", ">=", "==", "!=", "<", ">"};
        for (const auto& op : ops) {
            op_pos = segment.find(op);
            if (op_pos != std::string::npos) {
                op_str = op;
                break;
            }
        }

        if (op_pos != std::string::npos) {
            std::string var_name = trim(segment.substr(0, op_pos));
            std::string val_str = trim(segment.substr(op_pos + op_str.length()));

            RuleValue val;
            bool valid = false;

            // Check for string literal (simple check for quotes)
            if (val_str.size() >= 2 &&
                ((val_str.front() == '\'' && val_str.back() == '\'') ||
                 (val_str.front() == '"' && val_str.back() == '"'))) {
                val = val_str.substr(1, val_str.size() - 2);
                valid = true;
            } else {
                // Try parsing as double
                try {
                    val = std::stod(val_str);
                    valid = true;
                } catch (...) {
                    // Ignore parse errors
                }
            }

            if (valid) {
                rule.conditions.push_back({var_name, parse_operator(op_str), val});
            }
        }
    }

    // 3. Parse Consequent "Class=1"
    consequent_str = trim(consequent_str);
    size_t eq_pos = consequent_str.find('=');
    if (eq_pos != std::string::npos) {
        rule.result_key = trim(consequent_str.substr(0, eq_pos));
        rule.result_value = trim(consequent_str.substr(eq_pos + 1));
        rules_.push_back(rule);
    }
}

bool RuleEngine::check_condition(const Condition& cond, const RuleValue& input_val) {
    // 1. Double vs Double
    if (std::holds_alternative<double>(cond.value) && std::holds_alternative<double>(input_val)) {
        double c_val = std::get<double>(cond.value);
        double i_val = std::get<double>(input_val);

        switch (cond.op) {
            case Operator::LESS_THAN: return i_val < c_val;
            case Operator::LESS_EQUAL: return i_val <= c_val;
            case Operator::GREATER_THAN: return i_val > c_val;
            case Operator::GREATER_EQUAL: return i_val >= c_val;
            case Operator::EQUAL: return std::abs(i_val - c_val) < 1e-9;
            case Operator::NOT_EQUAL: return std::abs(i_val - c_val) > 1e-9;
        }
    }
    // 2. String vs String
    else if (std::holds_alternative<std::string>(cond.value) && std::holds_alternative<std::string>(input_val)) {
        std::string c_val = std::get<std::string>(cond.value);
        std::string i_val = std::get<std::string>(input_val);

        switch (cond.op) {
            case Operator::EQUAL: return i_val == c_val;
            case Operator::NOT_EQUAL: return i_val != c_val;
            // String comparison for <, >, etc. could be added if needed
            default: return false;
        }
    }

    // Type mismatch or unsupported operator
    return false;
}

std::optional<std::string> RuleEngine::evaluate(const std::map<std::string, RuleValue>& inputs) {
    for (const auto& rule : rules_) {
        bool all_met = true;
        for (const auto& cond : rule.conditions) {
            auto it = inputs.find(cond.variable);
            if (it == inputs.end()) {
                // Missing variable
                all_met = false;
                break;
            }

            if (!check_condition(cond, it->second)) {
                all_met = false;
                break;
            }
        }

        if (all_met) {
            return rule.result_value;
        }
    }
    return std::nullopt;
}

std::optional<std::string> RuleEngine::evaluate(const std::map<std::string, double>& inputs) {
    // Convert to RuleValue map
    std::map<std::string, RuleValue> mixed_inputs;
    for (const auto& [key, val] : inputs) {
        mixed_inputs[key] = val;
    }
    return evaluate(mixed_inputs);
}

} // namespace logic
} // namespace minni
