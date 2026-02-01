#include "SatSolver.h"
#include <sstream>
#include <iostream>
#include <algorithm>

namespace minni {
namespace logic {

SatSolver::SatSolver() = default;
SatSolver::~SatSolver() = default;

bool SatSolver::initialize() {
    return true;
}

void SatSolver::reset() {
    assignments_.clear();
    cnf_formula_.clear();
}

void SatSolver::set_param(const std::string& key, const std::string& value) {
    // No params supported yet
}

std::unordered_map<std::string, std::string> SatSolver::get_model() {
    std::unordered_map<std::string, std::string> model;
    for (const auto& kv : assignments_) {
        model[kv.first] = kv.second ? "true" : "false";
    }
    return model;
}

// Very basic parser for demonstration.
// Expects CNF clauses separated by '&', literals separated by '|'
// Example: "A|B & !A|C" -> (A or B) AND (NOT A or C)
void SatSolver::parse_formula(const std::string& formula_str) {
    cnf_formula_.clear();
    std::stringstream ss(formula_str);
    std::string segment;

    // Split by '&' into clauses
    while (std::getline(ss, segment, '&')) {
        Clause clause;
        std::stringstream clause_ss(segment);
        std::string literal_str;

        // Split by '|' into literals
        while (std::getline(clause_ss, literal_str, '|')) {
            // Trim whitespace
            literal_str.erase(0, literal_str.find_first_not_of(" \t\r\n"));
            literal_str.erase(literal_str.find_last_not_of(" \t\r\n") + 1);

            if (literal_str.empty()) continue;

            bool is_pos = true;
            if (literal_str[0] == '!') {
                is_pos = false;
                literal_str = literal_str.substr(1);
            }
            clause.push_back({literal_str, is_pos});
        }
        if (!clause.empty()) {
            cnf_formula_.push_back(clause);
        }
    }
}

SolverStatus SatSolver::check(const std::string& formula_str) {
    reset();
    parse_formula(formula_str);

    if (dpll(cnf_formula_, assignments_)) {
        return SolverStatus::SATISFIABLE;
    } else {
        return SolverStatus::UNSATISFIABLE;
    }
}

SatSolver::Literal SatSolver::negate(const Literal& l) {
    return {l.name, !l.is_positive};
}

// Unit Propagation simplification
SatSolver::Formula SatSolver::propagate(const Formula& formula, const Literal& unit_literal) {
    Formula new_formula;
    for (const auto& clause : formula) {
        bool clause_satisfied = false;
        Clause new_clause;

        for (const auto& lit : clause) {
            if (lit == unit_literal) {
                // Clause is true, remove it entirely
                clause_satisfied = true;
                break;
            } else if (lit == negate(unit_literal)) {
                // Literal is false, remove it from clause
                continue;
            } else {
                new_clause.push_back(lit);
            }
        }

        if (!clause_satisfied) {
            new_formula.push_back(new_clause);
        }
    }
    return new_formula;
}

// DPLL Algorithm
bool SatSolver::dpll(Formula formula, std::map<std::string, bool>& assignment) {
    // 1. Base cases
    if (formula.empty()) return true; // All clauses satisfied

    for (const auto& clause : formula) {
        if (clause.empty()) return false; // Empty clause (conflict)
    }

    // 2. Unit Propagation
    // Find a clause with only 1 literal
    for (const auto& clause : formula) {
        if (clause.size() == 1) {
            Literal unit = clause[0];
            assignment[unit.name] = unit.is_positive;
            return dpll(propagate(formula, unit), assignment);
        }
    }

    // 3. Pure Literal Elimination (Skipped for brevity, helps optimization)

    // 4. Splitting (Guessing)
    // Pick the first unassigned variable from the first clause
    Literal pick = formula[0][0];

    // Try True
    assignment[pick.name] = pick.is_positive;
    if (dpll(propagate(formula, pick), assignment)) return true;

    // Try False (Backtrack)
    assignment[pick.name] = !pick.is_positive;
    return dpll(propagate(formula, negate(pick)), assignment);
}

} // namespace logic
} // namespace minni
