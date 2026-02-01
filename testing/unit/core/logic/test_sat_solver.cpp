#include "../../../../src/core/logic/SatSolver.h"
#include <iostream>
#include <cassert>

void test_sat_solver_simple() {
    std::cout << "Running SAT Solver Simple Test..." << std::endl;

    minni::logic::SatSolver solver;

    // (A or B) AND (Not A) -> Must be B=True, A=False
    std::string formula = "A | B & !A";
    auto status = solver.check(formula);

    assert(status == minni::logic::SolverStatus::SATISFIABLE);
    auto model = solver.get_model();

    assert(model["A"] == "false");
    assert(model["B"] == "true");

    std::cout << "SAT Solver Simple Test Passed!" << std::endl;
}

void test_sat_solver_unsat() {
    std::cout << "Running SAT Solver UNSAT Test..." << std::endl;

    minni::logic::SatSolver solver;

    // A AND Not A -> Impossible
    std::string formula = "A & !A";
    auto status = solver.check(formula);

    assert(status == minni::logic::SolverStatus::UNSATISFIABLE);

    std::cout << "SAT Solver UNSAT Test Passed!" << std::endl;
}

int main() {
    test_sat_solver_simple();
    test_sat_solver_unsat();
    return 0;
}
