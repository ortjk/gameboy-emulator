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
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(apps)
add_subdirectory(src)
