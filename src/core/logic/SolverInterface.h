#ifndef MINNI_CORE_LOGIC_SOLVER_INTERFACE_H_
#define MINNI_CORE_LOGIC_SOLVER_INTERFACE_H_

#include <string>
#include <vector>
#include <unordered_map>

namespace minni {
namespace logic {

/**
 * Result status of a solver query.
 */
enum class SolverStatus {
    SATISFIABLE,
    UNSATISFIABLE,
    UNKNOWN,
    TIMEOUT
};

/**
 * Abstract base class for logic solvers (wrappers for Z3, Clingo, etc.).
 * Designed for stateless or stateful usage depending on the implementation.
 */
class SolverInterface {
public:
    virtual ~SolverInterface() = default;

    /**
     * initialize the solver engine.
     * @return true if initialization (loading libraries) succeeded.
     */
    virtual bool initialize() = 0;

    /**
     * Check satisfiability of a given formula.
     * @param formula_str The formula in SMT-LIB 2.0 format or other solver-specific text format.
     * @return SolverStatus indicating the result.
     */
    virtual SolverStatus check(const std::string& formula_str) = 0;

    /**
     * Get the model (variable assignments) if the last check was SATISFIABLE.
     * @return A map of variable names to their string values.
     */
    virtual std::unordered_map<std::string, std::string> get_model() = 0;

    /**
     * Reset the solver state (clear assertions).
     */
    virtual void reset() = 0;

    /**
     * Set a configuration parameter (e.g., "timeout", "random_seed").
     */
    virtual void set_param(const std::string& key, const std::string& value) = 0;
};

} // namespace logic
} // namespace minni

#endif // MINNI_CORE_LOGIC_SOLVER_INTERFACE_H_
