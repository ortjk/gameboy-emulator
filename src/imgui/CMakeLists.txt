set(SOURCE_LIST imgui.cpp
                imgui_draw.cpp
                imgui_tables.cpp
                imgui_widgets.cpp
                backends/imgui_impl_glfw.cpp
                backends/imgui_impl_opengl3.cpp)

set(HEADER_LIST "${GameboyEmulator_SOURCE_DIR}/include/imgui/imgui.h"
                "${GameboyEmulator_SOURCE_DIR}/include/imgui/imgui_internal.h"
                "${GameboyEmulator_SOURCE_DIR}/include/imgui/imconfig.h"
                "${GameboyEmulator_SOURCE_DIR}/include/imgui/imstb_rectpack.h"
                "${GameboyEmulator_SOURCE_DIR}/include/imgui/imstb_textedit.h"
                "${GameboyEmulator_SOURCE_DIR}/include/imgui/imstb_truetype.h"
                "${GameboyEmulator_SOURCE_DIR}/include/imgui/backends/imgui_impl_glfw.h"
                "${GameboyEmulator_SOURCE_DIR}/include/imgui/backends/imgui_impl_opengl3.h"
                "${GameboyEmulator_SOURCE_DIR}/include/imgui/backends/imgui_impl_opengl3_loader.h")

add_library(imgui_library "${SOURCE_LIST}" "${HEADER_LIST}")
target_link_libraries(imgui_library PRIVATE OpenGL GLEW glfw)
target_include_directories(imgui_library PUBLIC "${GameboyEmulator_SOURCE_DIR}/include")
