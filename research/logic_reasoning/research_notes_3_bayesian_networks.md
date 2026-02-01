# Research Notes: Bayesian Networks

**Date:** 2026-01-30
**Topic:** Probabilistic Reasoning

## Summary
Bayesian Networks (BNs) provide a way to reason under uncertainty, which is common in mobile contexts (noisy GPS, uncertain user activity). While full MCMC inference is too slow for real-time mobile use, **Variational Inference (VI)** and **Exact Inference on small graphs** are feasible.

## Technical Details
- **Exact Inference:** Algorithms like Variable Elimination or Junction Tree are exponential in the treewidth. feasible for small networks (< 50 nodes).
- **Approximate Inference:**
  - **Loopy Belief Propagation:** Good for many practical cases, iterative, easy to parallelize.
  - **Variational Inference (VI):** Converts inference into an optimization problem. Can be accelerated by Neural Nets (Amortized VI).

## Findings
- **Libraries:** There is no standard "Android Bayesian Library".
  - **Smile (Java):** Has BN support, runs on JVM.
  - **pgmpy (Python):** Good for prototyping, not for deployment.
  - **Custom C++:** For performance, a lightweight C++ engine implementing Variable Elimination is often the best path for specific, static networks.

## Implementation Considerations
- **Probabilistic Programming:** Running a full PPL (like Pyro/Stan) is heavy. Better to train/learn the model offline and export the conditional probability tables (CPTs) to a lightweight runtime JSON format.

## References/Resources
- [SMILE (Structural Modeling, Inference, and Learning Engine)](https://github.com/haifengl/smile)
- [Introduction to Probabilistic Graphical Models](https://cs.stanford.edu/people/ermon/cs228/index.html)

## Next Steps
- Define a JSON schema for importing Bayesian Networks (Nodes + CPTs).
- Implement a C++ `BayesianNetwork` class using Variable Elimination.
