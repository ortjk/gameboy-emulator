#include "gameboy-emulator/graphics/gui.hpp"

#include "gameboy-emulator/core/memory.hpp"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

namespace emulator {

ImGuiIO *GUI::io = nullptr;

void GUI::memory_viewer()
{
    ImGui::Begin("Memory", NULL, ImGuiWindowFlags_NoScrollbar);

    ImGui::Text("        00 01 02 03  04 05 06 07  08 09 0a 0b  0c 0d 0e 0f");

    ImGui::BeginChild("Memory.Scroll");
    for (int i = 0; i < 0xFFFF; i += 16)
    {
        ImGui::Text("0x%.4x: %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x", 
                    i, 
                    Memory::registers[i], Memory::registers[i+1], Memory::registers[i+2], Memory::registers[i+3], 
                    Memory::registers[i+4], Memory::registers[i+5], Memory::registers[i+6], Memory::registers[i+7], 
                    Memory::registers[i+8], Memory::registers[i+9], Memory::registers[i+10], Memory::registers[i+11], 
                    Memory::registers[i+12], Memory::registers[i+13], Memory::registers[i+14], Memory::registers[i+15]);
    }
    ImGui::EndChild();

    ImGui::End();
}

void GUI::object_viewer()
{
    ImGui::Begin("Objects");

    ImGui::End();
}

void GUI::init(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    memory_viewer();
    object_viewer();
}

void GUI::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void GUI::close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

} // namespace emulator