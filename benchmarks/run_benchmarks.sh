#!/bin/bash
# Run benchmarks from the project root directory
# Usage: bash benchmarks/run_benchmarks.sh

# Create output directory for binaries
mkdir -p benchmarks/bin

echo "========================================"
echo "Compiling Quantization Benchmark..."
echo "========================================"

g++ -std=c++17 -O3 -Isrc/core \
    benchmarks/memory/benchmark_quantization.cpp \
    src/core/logic/VectorStore.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    -o benchmarks/bin/benchmark_quantization

if [ $? -eq 0 ]; then
    echo "Compilation success. Running benchmark..."
    ./benchmarks/bin/benchmark_quantization
else
    echo "ERROR: Compilation failed for Quantization Benchmark."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling KnowledgeGraph Benchmark..."
echo "========================================"

g++ -std=c++17 -O3 -Isrc/core \
    benchmarks/memory/benchmark_kg.cpp \
    src/core/logic/KnowledgeGraph.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    -o benchmarks/bin/benchmark_kg

if [ $? -eq 0 ]; then
    echo "Compilation success. Running benchmark..."
    ./benchmarks/bin/benchmark_kg
else
    echo "ERROR: Compilation failed for KnowledgeGraph Benchmark."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling FlatVectorStore Benchmark..."
echo "========================================"

g++ -std=c++17 -O3 -Isrc/core \
    benchmarks/memory/benchmark_flat_vs.cpp \
    src/core/logic/VectorStore.cpp \
    src/core/logic/FlatVectorStore.cpp \
    src/core/platform/MemoryMapper.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    -o benchmarks/bin/benchmark_flat_vs

if [ $? -eq 0 ]; then
    echo "Compilation success. Running benchmark..."
    ./benchmarks/bin/benchmark_flat_vs
else
    echo "ERROR: Compilation failed for FlatVectorStore Benchmark."
    exit 1
fi
