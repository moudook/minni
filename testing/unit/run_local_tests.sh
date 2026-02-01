#!/bin/bash
# Run this script from the project root directory
# Usage: bash testing/unit/run_local_tests.sh

# Create output directory for binaries
mkdir -p testing/unit/bin

echo "========================================"
echo "Compiling KnowledgeGraph tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_knowledge_graph.cpp \
    src/core/logic/KnowledgeGraph.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    src/core/security/SecurityManager.cpp \
    -o testing/unit/bin/test_kg

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_kg
else
    echo "ERROR: Compilation failed for KnowledgeGraph tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling DSPKernel tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/signal/test_dsp_kernel.cpp \
    src/core/signal/DSPKernel.cpp \
    -o testing/unit/bin/test_dsp

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_dsp
else
    echo "ERROR: Compilation failed for DSPKernel tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling SatSolver tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_sat_solver.cpp \
    src/core/logic/SatSolver.cpp \
    -o testing/unit/bin/test_sat

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_sat
else
    echo "ERROR: Compilation failed for SatSolver tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling RuleEngine tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_rule_engine.cpp \
    src/core/logic/RuleEngine.cpp \
    -o testing/unit/bin/test_rule_engine

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_rule_engine
else
    echo "ERROR: Compilation failed for RuleEngine tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling DSP FFT tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/signal/test_fft.cpp \
    src/core/signal/DSPKernel.cpp \
    -o testing/unit/bin/test_fft

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_fft
else
    echo "ERROR: Compilation failed for FFT tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling SignalProcessor tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/signal/test_signal_processor.cpp \
    src/core/signal/SignalProcessor.cpp \
    src/core/signal/DSPKernel.cpp \
    -o testing/unit/bin/test_processor

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_processor
else
    echo "ERROR: Compilation failed for SignalProcessor tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling DSP Similarity tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/signal/test_similarity.cpp \
    src/core/signal/DSPKernel.cpp \
    -o testing/unit/bin/test_similarity

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_similarity
else
    echo "ERROR: Compilation failed for Similarity tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling Kalman Filter tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/signal/test_kalman.cpp \
    src/core/signal/KalmanFilter.cpp \
    -o testing/unit/bin/test_kalman

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_kalman
else
    echo "ERROR: Compilation failed for Kalman tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling VectorStore tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_vector_store.cpp \
    src/core/logic/VectorStore.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    src/core/security/SecurityManager.cpp \
    -o testing/unit/bin/test_vector_store

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_vector_store
else
    echo "ERROR: Compilation failed for VectorStore tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling KG Embedding tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_kg_embeddings.cpp \
    src/core/logic/KnowledgeGraph.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    src/core/security/SecurityManager.cpp \
    -o testing/unit/bin/test_kg_embeddings

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_kg_embeddings
else
    echo "ERROR: Compilation failed for KG Embedding tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling Quantizer tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/optimization/test_quantizer.cpp \
    src/core/optimization/Quantizer.cpp \
    -o testing/unit/bin/test_quantizer

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_quantizer
else
    echo "ERROR: Compilation failed for Quantizer tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling RuleEngine String tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_rule_engine_strings.cpp \
    src/core/logic/RuleEngine.cpp \
    -o testing/unit/bin/test_rule_engine_strings

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_rule_engine_strings
else
    echo "ERROR: Compilation failed for RuleEngine String tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling VectorStore Quantized tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_vector_store_quantized.cpp \
    src/core/logic/VectorStore.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    src/core/security/SecurityManager.cpp \
    -o testing/unit/bin/test_vector_store_quantized

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_vector_store_quantized
else
    echo "ERROR: Compilation failed for VectorStore Quantized tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling VectorStore Persistence tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_vector_store_persistence.cpp \
    src/core/logic/VectorStore.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    src/core/security/SecurityManager.cpp \
    -o testing/unit/bin/test_vector_store_persistence

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_vector_store_persistence
else
    echo "ERROR: Compilation failed for VectorStore Persistence tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling KnowledgeGraph Quantized tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_kg_quantized.cpp \
    src/core/logic/KnowledgeGraph.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    src/core/security/SecurityManager.cpp \
    -o testing/unit/bin/test_kg_quantized

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_kg_quantized
else
    echo "ERROR: Compilation failed for KG Quantized tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling KnowledgeGraph Persistence tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_kg_persistence.cpp \
    src/core/logic/KnowledgeGraph.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    src/core/security/SecurityManager.cpp \
    -o testing/unit/bin/test_kg_persistence

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_kg_persistence
else
    echo "ERROR: Compilation failed for KG Persistence tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling MemoryMapper tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/platform/test_memory_mapper.cpp \
    src/core/platform/MemoryMapper.cpp \
    -o testing/unit/bin/test_memory_mapper

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_memory_mapper
else
    echo "ERROR: Compilation failed for MemoryMapper tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling FlatVectorStore tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_flat_vector_store.cpp \
    src/core/logic/VectorStore.cpp \
    src/core/logic/FlatVectorStore.cpp \
    src/core/platform/MemoryMapper.cpp \
    src/core/signal/DSPKernel.cpp \
    src/core/optimization/Quantizer.cpp \
    src/core/security/SecurityManager.cpp \
    -o testing/unit/bin/test_flat_vector_store

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_flat_vector_store
else
    echo "ERROR: Compilation failed for FlatVectorStore tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling SecurityManager tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/security/test_security_manager.cpp \
    src/core/security/SecurityManager.cpp \
    -o testing/unit/bin/test_security_manager

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_security_manager
else
    echo "ERROR: Compilation failed for SecurityManager tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling Encrypted Persistence tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/logic/test_encrypted_persistence.cpp \
    src/core/logic/VectorStore.cpp \
    src/core/logic/KnowledgeGraph.cpp \
    src/core/security/SecurityManager.cpp \
    src/core/optimization/Quantizer.cpp \
    src/core/signal/DSPKernel.cpp \
    -o testing/unit/bin/test_encrypted_persistence

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_encrypted_persistence
else
    echo "ERROR: Compilation failed for Encrypted Persistence tests."
    exit 1
fi

echo ""
echo "========================================"
echo "Compiling GenAI ModelRunner tests..."
echo "========================================"

g++ -std=c++17 -Isrc/core \
    testing/unit/core/genai/test_model_runner.cpp \
    src/core/genai/ModelRunner.cpp \
    -o testing/unit/bin/test_model_runner

if [ $? -eq 0 ]; then
    echo "Compilation success. Running tests..."
    ./testing/unit/bin/test_model_runner
else
    echo "ERROR: Compilation failed for GenAI ModelRunner tests."
    exit 1
fi

echo ""
echo "All local unit tests passed!"
