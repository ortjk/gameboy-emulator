rm -rf ./build/*
cmake -S . -B build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DBUILD_TESTING=1
./build.bash