#include "gameboy-emulator/graphics/gui.hpp"

#include "gameboy-emulator/core/cpu.hpp"
#include "gameboy-emulator/core/memory.hpp"
#include "gameboy-emulator/graphics/gpu.hpp"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

#include <string>

namespace emulator {

const char *GUI::opcodes[256] = {
    "NOP",              "LD BC,d16",    "LD (BC),A",        "INC BC",       "INC B",        "DEC B",        "LD B,d8",      "RLCA", 
    "LD (u16),SP",      "ADD HL,BC",    "LD A,(BC)",        "DEC BC",       "INC C",        "DEC C",        "LD C,d8",      "RRCA", 
    "STOP",             "LD DE,d16",    "LD (DE),A",        "INC DE",       "INC D",        "DEC D",        "LD D,u8",      "RLA", 
    "JR i8",            "ADD HL,DE",    "LD A,(DE)",        "DEC DE",       "INC E",        "DEC E",        "LD E,d8",      "RRA", 
    "JR NZ,r8",         "LD HL,d16",    "LD (HL+),A",       "INC HL",       "INC H",        "DEC H",        "LD H,d8",      "DAA", 
    "JR Z,r8",          "ADD HL,HL",    "LD A,(HL+)",       "DEC HL",       "INC L",        "DEC L",        "LD L,d8",      "CPL", 
    "JR NC,r8",         "LD SP,d16",    "LD (HL-),A",       "INC SP",       "INC (HL)",     "DEC (HL)",     "LD (HL),d8",   "SCF", 
    "JR C,r8",          "ADD HL,SP",    "LD A,(HL-)",       "DEC SP",       "INC A",        "DEC A",        "LD A,d8",      "CCF",
    "LD B,B",           "LD B,C",       "LD B,D",           "LD B,E",       "LD B,H",       "LD B,L",       "LD B,(HL)",    "LD B,A", 
    "LD C,B",           "LD C,C",       "LD C,D",           "LD C,E",       "LD C,H",       "LD C,L",       "LD C,(HL)",    "LD C,A", 
    "LD D,B",           "LD D,C",       "LD D,D",           "LD D,E",       "LD D,H",       "LD D,L",       "LD D,(HL)",    "LD D,A", 
    "LD E,B",           "LD E,C",       "LD E,D",           "LD E,E",       "LD E,H",       "LD E,L",       "LD E,(HL)",    "LD E,A", 
    "LD H,B",           "LD H,C",       "LD H,D",           "LD H,E",       "LD H,H",       "LD H,L",       "LD H,(HL)",    "LD H,A", 
    "LD L,B",           "LD L,C",       "LD L,D",           "LD L,E",       "LD L,H",       "LD L,L",       "LD L,(HL)",    "LD L,A", 
    "LD (HL),B",        "LD (HL),C",    "LD (HL),D",        "LD (HL),E",    "LD (HL),H",    "LD (HL),L",    "HALT",         "LD (HL),A", 
    "LD A,B",           "LD A,C",       "LD A,D",           "LD A,E",       "LD A,H",       "LD A,L",       "LD A,(HL)",    "LD A,A",
    "ADD A,B",          "ADD A,C",      "ADD A,D",          "ADD A,E",      "ADD A,H",      "ADD A,L",      "ADD A,(HL)",   "ADD A,A", 
    "ADC A,B",          "ADC A,C",      "ADC A,D",          "ADC A,E",      "ADC A,H",      "ADC A,L",      "ADC A,(HL)",   "ADC A,A", 
    "SUB B",            "SUB C",        "SUB D",            "SUB E",        "SUB H",        "SUB L",        "SUB (HL)",     "SUB A", 
    "SBC A,B",          "SBC A,C",      "SBC A,D",          "SBC A,E",      "SBC A,H",      "SBC A,L",      "SBC A,(HL)",   "SBC A,A", 
    "AND B",            "AND C",        "AND D",            "AND E",        "AND H",        "AND L",        "AND (HL)",     "AND A", 
    "XOR B",            "XOR C",        "XOR D",            "XOR E",        "XOR H",        "XOR L",        "XOR (HL)",     "XOR A", 
    "OR B",             "OR C",         "OR D",             "OR E",         "OR H",         "OR L",         "OR (HL)",      "OR A", 
    "CP B",             "CP C",         "CP D",             "CP E",         "CP H",         "CP L",         "CP (HL)",      "CP A", 
    "RET NZ",           "POP BC",       "JP NZ,a16",        "JP a16",       "CALL NZ,a16",  "PUSH BC",      "ADD A,d8",     "RST 00H", 
    "RET Z",            "RET",          "JP Z,a16",         "PREFIX CB",    "CALL Z,a16",   "CALL a16",     "ADC A,d8",     "RST 08H", 
    "RET NC",           "POP DE",       "JP NC,a16",        "",             "CALL NC,a16",  "PUSH DE",      "SUB d8",       "RST 10H", 
    "RET C",            "RETI",         "JP C,a16",         "",             "CALL C,a16",   "",             "SBC A,d8",     "RST 18H",
    "LD (FF00+a8),A",   "POP HL",       "LD (FF00+C),A",    "",             "",             "PUSH HL",      "AND d8",       "RST 20H", 
    "ADD SP,r8",        "JP HL",        "LD (a16),A",       "",             "",             "",             "XOR d8",       "RST 28H",
    "LD A,(FF00+a8)",   "POP AF",       "LD A,(FF00+C)",    "DI",           "",             "PUSH AF",      "OR d8",        "RST 30H", 
    "LD HL,SP+r8",      "LD SP,HL",     "LD A,(a16)",       "EI",           "",             "",             "CP d8",        "RST 38H",
};

ImGuiIO *GUI::io = nullptr;
std::unique_ptr<Sprite> GUI::objects[40] = { nullptr };
char GUI::breakpoint_temp[5] = { 0 };

bool GUI::paused = false;
bool GUI::step = false;
bool GUI::breakpoints[65536] = { false };

void GUI::memory_viewer()
{
    ImGui::Begin("Memory", NULL, ImGuiWindowFlags_NoScrollbar);

    ImGui::Text("        00 01 02 03  04 05 06 07  08 09 0a 0b  0c 0d 0e 0f");

    ImGui::BeginChild("Memory.Scroll");
    for (int i = 0; i < 0xFFFF; i += 16)
    {
        ImGui::Text("0x%.4x: %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x", 
                    i, 
                    Memory::registers[i],       Memory::registers[i+1],     Memory::registers[i+2],     Memory::registers[i+3], 
                    Memory::registers[i+4],     Memory::registers[i+5],     Memory::registers[i+6],     Memory::registers[i+7], 
                    Memory::registers[i+8],     Memory::registers[i+9],     Memory::registers[i+10],    Memory::registers[i+11], 
                    Memory::registers[i+12],    Memory::registers[i+13],    Memory::registers[i+14],    Memory::registers[i+15]);
    }
    ImGui::EndChild();

    ImGui::End();
}

void GUI::object_viewer()
{
    ImGui::Begin("Objects");

    if (ImGui::BeginTable("table1", 4))
    {
        for (int r = 0; r < 10; r++) // row
        {
            ImGui::TableNextRow();
            for (int c = 0; c < 4; c++) // column
            {
                ImGui::TableSetColumnIndex(c);

                uint8_t i = c + r * 4;
                uint8_t pixels[8 * 8];
                GPU::get_object_pixels(pixels, i);
                objects[i]->update(pixels);
                ImGui::Image((ImTextureID)(intptr_t)objects[i]->get_texture(), ImVec2(64, 64));
            }
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

void GUI::program_editor()
{
    ImGui::Begin("Program");

    ImGui::SeparatorText("CPU Internal Registers:");
    ImGui::Text("pc: 0x%.4x  sp: 0x%.4x", CPU::pc, CPU::sp);
    ImGui::Text("af: 0x%.4x  bc: 0x%.4x", CPU::af, CPU::bc);
    ImGui::Text("de: 0x%.4x  hl: 0x%.4x", CPU::de, CPU::hl);
    char flags[4] = { '-', '-', '-', '-' };
    if ((CPU::af & 0x80) != 0)
    {
        flags[0] = 'Z';
    }
    if ((CPU::af & 0x40) != 0)
    {
        flags[1] = 'N';
    }
    if ((CPU::af & 0x20) != 0)
    {
        flags[2] = 'H';
    }
    if ((CPU::af & 0x10) != 0)
    {
        flags[3] = 'C';
    }
    ImGui::Text("Flags: %c %c %c %c", flags[0], flags[1], flags[2], flags[3]);

    ImGui::SeparatorText("Instructions");
    ImGui::Text("b0: 0x%.2x - %s", Memory::registers[CPU::pc], opcodes[Memory::registers[CPU::pc]]);
    ImGui::Text("b1: 0x%.2x", Memory::registers[CPU::pc+1]);
    ImGui::Text("b2: 0x%.2x", Memory::registers[CPU::pc+2]);
    ImGui::Text("b3: 0x%.2x", Memory::registers[CPU::pc+3]);

    ImGui::SeparatorText("Options");
    ImGui::Checkbox("Pause Emulation", &paused);

    bool temp_step = ImGui::Button("Step");
    if (paused && temp_step)
    {
        paused = false;
        step = true;
    }

    ImGuiInputTextFlags addr_flags = ImGuiInputTextFlags_CharsHexadecimal
                                        | ImGuiInputTextFlags_CharsNoBlank;
    ImGui::Text("Breakpoint Address:");
    ImGui::Text("0x"); ImGui::SameLine();
    ImGui::InputText("##Breakpoint Address Enter", breakpoint_temp, 5, addr_flags); ImGui::SameLine();
    bool set_breakpoint = ImGui::Button("Set Breakpoint");
    
    if (set_breakpoint)
    {
        uint16_t addr = std::stoi(breakpoint_temp, 0, 16);
        breakpoints[addr] = true;
        for (int i = 0; i < 4; i++)
        {
            breakpoint_temp[i] = '\0';
        }
    }

    ImGui::Text("Existing Breakpoints:");
    ImGui::BeginChild("Breakpoints.Scroll");
    for (int i = 0; i <= 0xFFFF; i++)
    {
        if (breakpoints[i])
        {
            ImGui::PushID(i);
            ImGui::Text("0x%.4x", i); ImGui::SameLine();
            bool del_breakpoint = ImGui::Button("Delete");
            if (del_breakpoint)
            {
                breakpoints[i] = false;
            }
            ImGui::PopID();
        }
    }
    ImGui::EndChild();

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

    for (int i = 0; i < 40; i++)
    {
        objects[i] = std::make_unique<Sprite>(8, 8);
    }
}

void GUI::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    memory_viewer();
    object_viewer();
    program_editor();
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