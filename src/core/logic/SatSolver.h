#ifndef MINNI_CORE_LOGIC_SAT_SOLVER_H_
#define MINNI_CORE_LOGIC_SAT_SOLVER_H_

#include "SolverInterface.h"
#include <vector>
#include <map>
#include <set>

namespace minni {
namespace logic {

/**
 * A simple SAT solver based on the DPLL algorithm.
 * Supports CNF (Conjunctive Normal Form) formulas.
 *
 * Input format expects simplified "DIMACS-like" tokens or custom symbolic strings.
 * For this implementation, we parse a simple string format:
 * "(A | B) & (!A | C)"
 */
class SatSolver : public SolverInterface {
public:
    SatSolver();
    ~SatSolver() override;

    bool initialize() override;
    SolverStatus check(const std::string& formula_str) override;
    std::unordered_map<std::string, std::string> get_model() override;
    void reset() override;
    void set_param(const std::string& key, const std::string& value) override;

private:
    // Internal representation of a literal (e.g., A or !A)
    struct Literal {
        std::string name;
        bool is_positive; // true = A, false = !A

        bool operator<(const Literal& other) const {
            if (name != other.name) return name < other.name;
            return is_positive < other.is_positive;
        }
        bool operator==(const Literal& other) const {
             return name == other.name && is_positive == other.is_positive;
        }
    };

    // A clause is a disjunction of literals (A | B | !C)
    using Clause = std::vector<Literal>;
    // A formula is a conjunction of clauses (C1 & C2 & C3)
    using Formula = std::vector<Clause>;

    // Current assignment of variables: true/false/unassigned
    std::map<std::string, bool> assignments_;

    // Parsed formula
    Formula cnf_formula_;

    // Helper methods
    bool dpll(Formula formula, std::map<std::string, bool>& assignment);
    Formula propagate(const Formula& formula, const Literal& unit_literal);
    Literal negate(const Literal& l);

    // Parsing
    void parse_formula(const std::string& formula_str);
};

} // namespace logic
} // namespace minni

#endif // MINNI_CORE_LOGIC_SAT_SOLVER_H_
