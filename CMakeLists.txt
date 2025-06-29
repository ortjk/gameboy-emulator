cmake_minimum_required(VERSION 3.16...4.0)

project(GameboyEmulator)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    if (BUILD_TESTING)
        set(CMAKE_BUILD_TESTING ON)
        add_compile_definitions(CMAKE_BUILD_TESTING)
        include(FetchContent)
        include(CTest)
        add_subdirectory(tests)
    endif()

    if (LOG_CPU_INSTRUCTIONS)
        set(CMAKE_LOG_CPU_INSTRUCTIONS ON)
        add_compile_definitions(CMAKE_LOG_CPU_INSTRUCTIONS)
    endif()

    if (LOG_MEMORY_8000_9FFF)
        set(CMAKE_LOG_MEMORY_8000_9FFF ON)
        add_compile_definitions(CMAKE_LOG_MEMORY_8000_9FFF)
    endif()

    if (LOG_MEMORY_FF00_FFFF)
        set(CMAKE_LOG_MEMORY_FF00_FFFF ON)
        add_compile_definitions(CMAKE_LOG_MEMORY_FF00_FFFF)
    endif()
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(apps)
add_subdirectory(src)
