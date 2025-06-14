add_executable(Emulator emulator.cpp)

include_directories(${OpenGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

target_link_libraries(Emulator PRIVATE OpenGL)
target_link_libraries(Emulator PRIVATE glut)
target_link_libraries(Emulator PRIVATE GLEW)

target_link_libraries(Emulator PRIVATE core_library)
