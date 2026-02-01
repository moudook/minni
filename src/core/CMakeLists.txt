cmake_minimum_required(VERSION 3.18.0)
project(minni_core)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source grouping
set(LOGIC_SOURCES
    logic/KnowledgeGraph.h
    logic/KnowledgeGraph.cpp
    logic/SolverInterface.h
    logic/SatSolver.h
    logic/SatSolver.cpp
    logic/RuleEngine.h
    logic/RuleEngine.cpp
    logic/VectorStore.h
    logic/VectorStore.cpp
    logic/FlatVectorStore.h
    logic/FlatVectorStore.cpp
)

set(SIGNAL_SOURCES
    signal/DSPKernel.h
    signal/DSPKernel.cpp
    signal/SignalProcessor.h
    signal/SignalProcessor.cpp
    signal/KalmanFilter.h
    signal/KalmanFilter.cpp
)

set(OPTIMIZATION_SOURCES
    optimization/Quantizer.h
    optimization/Quantizer.cpp
)

set(SECURITY_SOURCES
    security/SecurityManager.h
    security/SecurityManager.cpp
)

set(PLATFORM_SOURCES
    platform/MemoryMapper.h
    platform/MemoryMapper.cpp
)

set(GENAI_SOURCES
    genai/ModelRunner.h
    genai/ModelRunner.cpp
)

# JNI Bridge
set(JNI_SOURCES
    ../android/core/jni/NativeBridge.cpp
)

set(CORE_SOURCES
    ${LOGIC_SOURCES}
    ${SIGNAL_SOURCES}
    ${OPTIMIZATION_SOURCES}
    ${SECURITY_SOURCES}
    ${PLATFORM_SOURCES}
    ${GENAI_SOURCES}
    ${JNI_SOURCES}
)

# Define the library
add_library(minni_core SHARED ${CORE_SOURCES})

# Include directories
target_include_directories(minni_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Linker flags (standard Android NDK flags usually handled by toolchain file)
if(ANDROID)
    target_link_libraries(minni_core log android)
endif()
