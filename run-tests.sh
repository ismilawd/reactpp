#!/bin/bash
# Bash script to run all tests using CMake presets
# Usage: ./run-tests.sh [preset-name]

PRESET=${1:-all}

echo "Running tests with preset: $PRESET"

# Configure if needed
if [ ! -d "build/default" ]; then
    echo "Configuring build..."
    cmake --preset default
    if [ $? -ne 0 ]; then
        echo "Configuration failed!"
        exit 1
    fi
fi

# Build tests
echo "Building tests..."
cmake --build --preset tests
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Run tests
echo "Running tests..."
cmake --test --preset "$PRESET"
if [ $? -ne 0 ]; then
    echo "Tests failed!"
    exit 1
fi

echo "All tests completed successfully!"

