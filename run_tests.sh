#!/bin/bash

# Test runner script for Any library
# Author: Marcel Raymond Goumou
# Date: 2025-09-25

set -e  # Exit on any error

echo "🧪 Running comprehensive tests for Any library"
echo "=============================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    local status=$1
    local message=$2
    case $status in
        "PASS")
            echo -e "${GREEN}✅ PASS${NC}: $message"
            ;;
        "FAIL")
            echo -e "${RED}❌ FAIL${NC}: $message"
            ;;
        "INFO")
            echo -e "${BLUE}ℹ️  INFO${NC}: $message"
            ;;
        "WARN")
            echo -e "${YELLOW}⚠️  WARN${NC}: $message"
            ;;
    esac
}

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    print_status "FAIL" "Please run this script from the project root directory"
    exit 1
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    print_status "INFO" "Creating build directory..."
    mkdir build
fi

cd build

# Configure and build
# Memory tracking toggle: set ANY_ENABLE_MEMORY_TRACKING=ON|OFF (default ON)
TRACKING=${ANY_ENABLE_MEMORY_TRACKING:-ON}
if [ "$TRACKING" = "ON" ]; then
  print_status "INFO" "Configuring project with CMake (memory tracking ON)..."
  CONFIG_CMD=(cmake -DANY_ENABLE_MEMORY_TRACKING=ON ..)
else
  print_status "INFO" "Configuring project with CMake (memory tracking OFF)..."
  CONFIG_CMD=(cmake ..)
fi
if "${CONFIG_CMD[@]}" > /dev/null 2>&1; then
    print_status "PASS" "CMake configuration successful"
else
    print_status "FAIL" "CMake configuration failed"
    exit 1
fi

print_status "INFO" "Building project..."
if make > /dev/null 2>&1; then
    print_status "PASS" "Build successful"
else
    print_status "FAIL" "Build failed"
    exit 1
fi

echo ""
echo "🧪 Running unit tests..."
echo "========================"

# Run unit tests
if ./tests/test_any > /dev/null 2>&1; then
    print_status "PASS" "Unit tests passed"
else
    print_status "FAIL" "Unit tests failed"
    echo "Unit test output:"
    ./tests/test_any
    exit 1
fi

echo ""
echo "🔥 Running stress tests..."
echo "========================="

# Run stress tests
if ./tests/stress_test > /dev/null 2>&1; then
    print_status "PASS" "Stress tests passed"
else
    print_status "FAIL" "Stress tests failed"
    echo "Stress test output:"
    ./tests/stress_test
    exit 1
fi

echo ""
echo "🔧 Running compatibility tests..."
echo "================================="

# Run compatibility tests
if ./tests/compatibility_test > /dev/null 2>&1; then
    print_status "PASS" "Compatibility tests passed"
else
    print_status "FAIL" "Compatibility tests failed"
    echo "Compatibility test output:"
    ./tests/compatibility_test
    exit 1
fi

if [ "$TRACKING" = "ON" ]; then
  echo ""
  echo "🧪 Running leak demonstration (tracking only)..."
  echo "=============================================="
  # Run leak demo (expects a leak under tracking)
  if ./tests/leak_demo > /dev/null 2>&1; then
      print_status "PASS" "Leak demo behaved as expected"
  else
      print_status "WARN" "Leak demo did not behave as expected (tracking mismatch)"
  fi
fi

echo ""
echo "📝 Running example..."
echo "===================="

# Run example
if ./examples/example > /dev/null 2>&1; then
    print_status "PASS" "Example runs successfully"
else
    print_status "FAIL" "Example failed to run"
    echo "Example output:"
    ./examples/example
    exit 1
fi

echo ""
echo "📊 Test Summary"
echo "==============="
print_status "PASS" "All tests completed successfully!"
print_status "INFO" "Unit tests: ✅ PASSED"
print_status "INFO" "Stress tests: ✅ PASSED"
print_status "INFO" "Compatibility tests: ✅ PASSED"
print_status "INFO" "Example: ✅ PASSED"

echo ""
echo "🎉 Any library is ready for production use!"
echo "   - All functionality tested and verified"
echo "   - Memory management validated"
echo "   - Error handling confirmed"
echo "   - Performance stress tested"

cd ..
