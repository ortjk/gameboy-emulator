add_executable(Emulator emulator.cpp)

target_link_libraries(Emulator PRIVATE core_library graphics_library)
