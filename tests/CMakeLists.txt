FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0)
FetchContent_MakeAvailable(json)



add_executable(coretest coretest.cpp)
target_link_libraries(coretest PRIVATE core_library Catch2::Catch2 nlohmann_json)
target_include_directories(coretest PRIVATE "${GameboyEmulator_SOURCE_DIR}/tests/CPUTests")
target_compile_definitions(coretest PRIVATE CPUTESTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/CPUTests")
add_test(NAME coretest_test COMMAND coretest)
