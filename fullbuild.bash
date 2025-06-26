rm -rf ./build/*
cmake -S . -B build -G Ninja \
    -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
    -D CMAKE_C_COMPILER=clang \
    -D CMAKE_CXX_COMPILER=clang++ # -DBUILD_TESTING=1
./build.bash