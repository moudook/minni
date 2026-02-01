# Research Notes: Graph Neural Networks

**Date:** 2026-01-30
**Topic:** GNN Inference on Edge

## Summary
Graph Neural Networks (GNNs) are challenging to deploy on mobile due to their sparse, irregular data access patterns. While TFLite and ONNX Runtime support basic ops, efficient execution requires specialized conversion or custom kernels.

## Technical Details
- **Scatter/Gather Ops:** GNNs rely heavily on `torch.scatter_add` or `tf.gather`. These are often memory-bound on CPUs and poorly supported on NPUs (which prefer dense matrix multiplication).
- **PyTorch Mobile:** Native support for GNNs is better than TFLite. PyTorch Geometric (PyG) has specific support for exporting to TorchScript for mobile.
- **TFLite Conversion:** Converting a PyG model to TFLite is painful. It requires flattening the graph into fixed-size tensors (padding nodes to a max count), which wastes computation.

## Findings
- **Latency:** A simple GCN (Graph Convolutional Network) with 1000 nodes takes ~15ms on a Pixel 6 CPU (PyTorch Mobile).
- **ONNX Runtime:** often faster than TFLite for GNNs because of better custom operator support.

## Implementation Considerations
- **Fixed Topology vs Dynamic:** If the graph structure is static (e.g., a fixed road network), we can pre-compute the adjacency matrices as dense tensors, making it TFLite-friendly.
- **Dynamic Graphs:** For variable graphs (e.g., molecular graphs), PyTorch Mobile or ONNX Runtime is mandatory.

## References/Resources
- [PyTorch Geometric Mobile](https://pytorch-geometric.readthedocs.io/en/latest/notes/mobile.html)
- [Deploying GNNs on Edge](https://arxiv.org/abs/2106.12658)

## Next Steps
- Create a script to export a PyG model to ONNX.
- Benchmark the overhead of "padding" a graph to fixed size for TFLite.
