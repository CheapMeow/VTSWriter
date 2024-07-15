#!/bin/bash

SOLVER_SOURCES=$(find ./src/solver -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -printf "%P\n")

MANUAL_TEST_SOURCES=$(find ./src/manual_test -type f -name "*.cpp" -printf "%P\n")

cmake_template=$(<CMakeLists.txt.template)

solver_sources_formatted=$(echo "$SOLVER_SOURCES" | sed 's/^/\tsrc\/solver\//')
cmake_template=${cmake_template//"# SOLVER_SOURCES_PLACEHOLDER"/"$solver_sources_formatted"}

manual_tests=""
for src in $MANUAL_TEST_SOURCES; do
    exe_name=$(basename "$src" .cpp)
    manual_tests+="
add_executable($exe_name src/manual_test/$src)
target_link_libraries($exe_name PUBLIC Solver)

set_target_properties($exe_name PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY \${CMAKE_BINARY_DIR}/\$<CONFIG>
    ARCHIVE_OUTPUT_DIRECTORY \${CMAKE_BINARY_DIR}/\$<CONFIG>
    LIBRARY_OUTPUT_DIRECTORY \${CMAKE_BINARY_DIR}/\$<CONFIG>
)
"
done
cmake_template=${cmake_template//"# MANUAL_TESTS_PLACEHOLDER"/"$manual_tests"}

echo "$cmake_template" >CMakeLists.txt

mkdir -p build
cd build

if command -v dpcpp &>/dev/null; then
    echo "setvars.sh has already been run. Skipping re-execution."
else
    echo "Initalize oneAPI."
    source /public/software/oneAPI/setvars.sh
fi

gcc_path=$(which gcc)
gxx_path=$(which g++)

if [ -z "$gcc_path" ]; then
    echo "gcc not found in PATH"
else
    echo "gcc found at: $gcc_path"
fi

if [ -z "$gxx_path" ]; then
    echo "g++ not found in PATH"
else
    echo "g++ found at: $gxx_path"
fi

export CC=$gcc_path
export CXX=$gxx_path

CPU_CORES=$(nproc)
echo "Building with $CPU_CORES cores."

time {
    cmake -DCMAKE_CXX_COMPILER=mpicxx -DCMAKE_BUILD_TYPE=Release ..
    make -j$CPU_CORES
}

echo -e '\a'
