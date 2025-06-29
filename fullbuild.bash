rm -rf ./build/*
cmake -S . -B build -G Ninja \
    -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
    -D CMAKE_C_COMPILER=clang \
    -D CMAKE_CXX_COMPILER=clang++ \
    -D LOG_CPU_INSTRUCTIONS=1 \
    -D LOG_MEMORY_8000_9FFF=1 \
    -D LOG_MEMORY_FF00_FFFF=1
    # -D BUILD_TESTING=1 \

./build.bash