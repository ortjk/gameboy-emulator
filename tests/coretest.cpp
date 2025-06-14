#include <cctype>
#define CATCH_CONFIG_MAIN

#include <fstream>
#include <string>

#include <nlohmann/json.hpp>
#include <catch2/catch.hpp>

#include "gameboy-emulator/core/cpu.hpp"
#include "gameboy-emulator/core/memory.hpp"

using namespace emulator;
using json = nlohmann::json;

int get_opcode_int(std::string name)
{
    name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end()); // remove spaces
    return std::stoi(name, nullptr, 16);
}

void set_initial(json &data)
{
    CPU::set_a(data["a"]);
    CPU::set_b(data["b"]);
    CPU::set_c(data["c"]);
    CPU::set_d(data["d"]);
    CPU::set_e(data["e"]);
    CPU::set_f(data["f"]);
    CPU::set_h(data["h"]);
    CPU::set_l(data["l"]);

    CPU::set_pc(data["pc"]);
    CPU::set_sp(data["sp"]);

    for (json &outer: data["ram"])
    {
        Memory::write((uint8_t)outer[1], (uint16_t)outer[0]);
    }
}

void check_final_regs(json &data)
{
    bool pass = true;

    pass = pass && (CPU::get_a() == data["a"]);
    pass = pass && (CPU::get_b() == data["b"]);
    pass = pass && (CPU::get_c() == data["c"]);
    pass = pass && (CPU::get_d() == data["d"]);
    pass = pass && (CPU::get_e() == data["e"]);
    pass = pass && (CPU::get_f() == data["f"]);
    pass = pass && (CPU::get_h() == data["h"]);
    pass = pass && (CPU::get_l() == data["l"]);
    pass = pass && (CPU::get_pc() == data["pc"]);
    pass = pass && (CPU::get_sp() == data["sp"]);

    if (!pass)
    {
        std::cout << "   have   |   need" << std::endl;
        std::cout << "----------------------" << std::endl;
        std::cout << "a: " << (int)CPU::get_a() << " vs " << data["a"] << std::endl;
        std::cout << "b: " << (int)CPU::get_b() << " vs " << data["b"]  << std::endl;
        std::cout << "c: " << (int)CPU::get_c() << " vs " << data["c"]  << std::endl;
        std::cout << "d: " << (int)CPU::get_d() << " vs " << data["d"]  << std::endl;
        std::cout << "e: " << (int)CPU::get_e() << " vs " << data["e"]  << std::endl;
        std::cout << "f: " << (int)CPU::get_f() << " vs " << data["f"]  << std::endl;
        std::cout << "h: " << (int)CPU::get_h() << " vs " << data["h"]  << std::endl;
        std::cout << "l: " << (int)CPU::get_l() << " vs " << data["l"]  << std::endl;
        std::cout << "pc: " << (int)CPU::get_pc() << " vs " << data["pc"]  << std::endl;
        std::cout << "sp: " << (int)CPU::get_sp() << " vs " << data["sp"]  << std::endl;
    }
    REQUIRE( pass );
}

void check_final_mem(json &data)
{
    bool pass = true;

    for (json &outer: data["ram"])
    {
        pass = pass && (*Memory::get_8b((uint16_t)outer[0]) == outer[1]);
    }

    if (!pass)
    {
        std::cout << "   have   |   need" << std::endl;
        std::cout << "----------------------" << std::endl;
        std::cout << "a: " << (int)CPU::get_a() << " vs " << data["a"] << std::endl;
        std::cout << "b: " << (int)CPU::get_b() << " vs " << data["b"]  << std::endl;
        std::cout << "c: " << (int)CPU::get_c() << " vs " << data["c"]  << std::endl;
        std::cout << "d: " << (int)CPU::get_d() << " vs " << data["d"]  << std::endl;
        std::cout << "e: " << (int)CPU::get_e() << " vs " << data["e"]  << std::endl;
        std::cout << "f: " << (int)CPU::get_f() << " vs " << data["f"]  << std::endl;
        std::cout << "h: " << (int)CPU::get_h() << " vs " << data["h"]  << std::endl;
        std::cout << "l: " << (int)CPU::get_l() << " vs " << data["l"]  << std::endl;
        std::cout << "pc: " << (int)CPU::get_pc() << " vs " << data["pc"]  << std::endl;
        std::cout << "sp: " << (int)CPU::get_sp() << " vs " << data["sp"]  << std::endl;
        
        for (json &outer: data["ram"])
        {
            std::cout << "ram " << (int)outer[0] << ": " << (int)*Memory::get_8b((uint16_t)outer[0]) << " vs " << (int)outer[1]  << std::endl;
        }
    }
    REQUIRE( pass );
}

void test_json(std::string path)
{
    std::ifstream f(path);
    json data = json::parse(f);

    bool fail = false;
    for (json &outer: data)
    {
        int opcode_full = get_opcode_int(outer["name"]);
        uint8_t *opcode = reinterpret_cast<uint8_t *>(&opcode_full);

        set_initial(outer["initial"]);
        CPU::instruction(opcode[2], opcode[1], opcode[0], 0x00);

        check_final_regs(outer["final"]);
        check_final_mem(outer["final"]);
    }
}

TEST_CASE("CPU Test 00", "[core]") {
	test_json(CPUTESTS_DIR"/00.json");
}

TEST_CASE("CPU Test 01", "[core]") {
	test_json(CPUTESTS_DIR"/01.json");
}

TEST_CASE("CPU Test 02", "[core]") {
	test_json(CPUTESTS_DIR"/02.json");
}

TEST_CASE("CPU Test 03", "[core]") {
	test_json(CPUTESTS_DIR"/03.json");
}

TEST_CASE("CPU Test 04", "[core]") {
	test_json(CPUTESTS_DIR"/04.json");
}

TEST_CASE("CPU Test 05", "[core]") {
	test_json(CPUTESTS_DIR"/05.json");
}

TEST_CASE("CPU Test 06", "[core]") {
	test_json(CPUTESTS_DIR"/06.json");
}

TEST_CASE("CPU Test 07", "[core]") {
	test_json(CPUTESTS_DIR"/07.json");
}

TEST_CASE("CPU Test 08", "[core]") {
	test_json(CPUTESTS_DIR"/08.json");
}

TEST_CASE("CPU Test 09", "[core]") {
	test_json(CPUTESTS_DIR"/09.json");
}

TEST_CASE("CPU Test 0a", "[core]") {
	test_json(CPUTESTS_DIR"/0a.json");
}

TEST_CASE("CPU Test 0b", "[core]") {
	test_json(CPUTESTS_DIR"/0b.json");
}

TEST_CASE("CPU Test 0c", "[core]") {
	test_json(CPUTESTS_DIR"/0c.json");
}

TEST_CASE("CPU Test 0d", "[core]") {
	test_json(CPUTESTS_DIR"/0d.json");
}

TEST_CASE("CPU Test 0e", "[core]") {
	test_json(CPUTESTS_DIR"/0e.json");
}

TEST_CASE("CPU Test 0f", "[core]") {
	test_json(CPUTESTS_DIR"/0f.json");
}

// TEST_CASE("CPU Test 10", "[core]") {
// 	test_json(CPUTESTS_DIR"/10.json");
// }

TEST_CASE("CPU Test 11", "[core]") {
	test_json(CPUTESTS_DIR"/11.json");
}

TEST_CASE("CPU Test 12", "[core]") {
	test_json(CPUTESTS_DIR"/12.json");
}

TEST_CASE("CPU Test 13", "[core]") {
	test_json(CPUTESTS_DIR"/13.json");
}

TEST_CASE("CPU Test 14", "[core]") {
	test_json(CPUTESTS_DIR"/14.json");
}

TEST_CASE("CPU Test 15", "[core]") {
	test_json(CPUTESTS_DIR"/15.json");
}

TEST_CASE("CPU Test 16", "[core]") {
	test_json(CPUTESTS_DIR"/16.json");
}

TEST_CASE("CPU Test 17", "[core]") {
	test_json(CPUTESTS_DIR"/17.json");
}

TEST_CASE("CPU Test 18", "[core]") {
	test_json(CPUTESTS_DIR"/18.json");
}

TEST_CASE("CPU Test 19", "[core]") {
	test_json(CPUTESTS_DIR"/19.json");
}

TEST_CASE("CPU Test 1a", "[core]") {
	test_json(CPUTESTS_DIR"/1a.json");
}

TEST_CASE("CPU Test 1b", "[core]") {
	test_json(CPUTESTS_DIR"/1b.json");
}

TEST_CASE("CPU Test 1c", "[core]") {
	test_json(CPUTESTS_DIR"/1c.json");
}

TEST_CASE("CPU Test 1d", "[core]") {
	test_json(CPUTESTS_DIR"/1d.json");
}

TEST_CASE("CPU Test 1e", "[core]") {
	test_json(CPUTESTS_DIR"/1e.json");
}

TEST_CASE("CPU Test 1f", "[core]") {
	test_json(CPUTESTS_DIR"/1f.json");
}

TEST_CASE("CPU Test 20", "[core]") {
	test_json(CPUTESTS_DIR"/20.json");
}

TEST_CASE("CPU Test 21", "[core]") {
	test_json(CPUTESTS_DIR"/21.json");
}

TEST_CASE("CPU Test 22", "[core]") {
	test_json(CPUTESTS_DIR"/22.json");
}

TEST_CASE("CPU Test 23", "[core]") {
	test_json(CPUTESTS_DIR"/23.json");
}

TEST_CASE("CPU Test 24", "[core]") {
	test_json(CPUTESTS_DIR"/24.json");
}

TEST_CASE("CPU Test 25", "[core]") {
	test_json(CPUTESTS_DIR"/25.json");
}

TEST_CASE("CPU Test 26", "[core]") {
	test_json(CPUTESTS_DIR"/26.json");
}

TEST_CASE("CPU Test 27", "[core]") {
	test_json(CPUTESTS_DIR"/27.json");
}

TEST_CASE("CPU Test 28", "[core]") {
	test_json(CPUTESTS_DIR"/28.json");
}

TEST_CASE("CPU Test 29", "[core]") {
	test_json(CPUTESTS_DIR"/29.json");
}

TEST_CASE("CPU Test 2a", "[core]") {
	test_json(CPUTESTS_DIR"/2a.json");
}

TEST_CASE("CPU Test 2b", "[core]") {
	test_json(CPUTESTS_DIR"/2b.json");
}

TEST_CASE("CPU Test 2c", "[core]") {
	test_json(CPUTESTS_DIR"/2c.json");
}

TEST_CASE("CPU Test 2d", "[core]") {
	test_json(CPUTESTS_DIR"/2d.json");
}

TEST_CASE("CPU Test 2e", "[core]") {
	test_json(CPUTESTS_DIR"/2e.json");
}

TEST_CASE("CPU Test 2f", "[core]") {
	test_json(CPUTESTS_DIR"/2f.json");
}

TEST_CASE("CPU Test 30", "[core]") {
	test_json(CPUTESTS_DIR"/30.json");
}

TEST_CASE("CPU Test 31", "[core]") {
	test_json(CPUTESTS_DIR"/31.json");
}

TEST_CASE("CPU Test 32", "[core]") {
	test_json(CPUTESTS_DIR"/32.json");
}

TEST_CASE("CPU Test 33", "[core]") {
	test_json(CPUTESTS_DIR"/33.json");
}

TEST_CASE("CPU Test 34", "[core]") {
	test_json(CPUTESTS_DIR"/34.json");
}

TEST_CASE("CPU Test 35", "[core]") {
	test_json(CPUTESTS_DIR"/35.json");
}

TEST_CASE("CPU Test 36", "[core]") {
	test_json(CPUTESTS_DIR"/36.json");
}

TEST_CASE("CPU Test 37", "[core]") {
	test_json(CPUTESTS_DIR"/37.json");
}

TEST_CASE("CPU Test 38", "[core]") {
	test_json(CPUTESTS_DIR"/38.json");
}

TEST_CASE("CPU Test 39", "[core]") {
	test_json(CPUTESTS_DIR"/39.json");
}

TEST_CASE("CPU Test 3a", "[core]") {
	test_json(CPUTESTS_DIR"/3a.json");
}

TEST_CASE("CPU Test 3b", "[core]") {
	test_json(CPUTESTS_DIR"/3b.json");
}

TEST_CASE("CPU Test 3c", "[core]") {
	test_json(CPUTESTS_DIR"/3c.json");
}

TEST_CASE("CPU Test 3d", "[core]") {
	test_json(CPUTESTS_DIR"/3d.json");
}

TEST_CASE("CPU Test 3e", "[core]") {
	test_json(CPUTESTS_DIR"/3e.json");
}

TEST_CASE("CPU Test 3f", "[core]") {
	test_json(CPUTESTS_DIR"/3f.json");
}

TEST_CASE("CPU Test 40", "[core]") {
	test_json(CPUTESTS_DIR"/40.json");
}

TEST_CASE("CPU Test 41", "[core]") {
	test_json(CPUTESTS_DIR"/41.json");
}

TEST_CASE("CPU Test 42", "[core]") {
	test_json(CPUTESTS_DIR"/42.json");
}

TEST_CASE("CPU Test 43", "[core]") {
	test_json(CPUTESTS_DIR"/43.json");
}

TEST_CASE("CPU Test 44", "[core]") {
	test_json(CPUTESTS_DIR"/44.json");
}

TEST_CASE("CPU Test 45", "[core]") {
	test_json(CPUTESTS_DIR"/45.json");
}

TEST_CASE("CPU Test 46", "[core]") {
	test_json(CPUTESTS_DIR"/46.json");
}

TEST_CASE("CPU Test 47", "[core]") {
	test_json(CPUTESTS_DIR"/47.json");
}

TEST_CASE("CPU Test 48", "[core]") {
	test_json(CPUTESTS_DIR"/48.json");
}

TEST_CASE("CPU Test 49", "[core]") {
	test_json(CPUTESTS_DIR"/49.json");
}

TEST_CASE("CPU Test 4a", "[core]") {
	test_json(CPUTESTS_DIR"/4a.json");
}

TEST_CASE("CPU Test 4b", "[core]") {
	test_json(CPUTESTS_DIR"/4b.json");
}

TEST_CASE("CPU Test 4c", "[core]") {
	test_json(CPUTESTS_DIR"/4c.json");
}

TEST_CASE("CPU Test 4d", "[core]") {
	test_json(CPUTESTS_DIR"/4d.json");
}

TEST_CASE("CPU Test 4e", "[core]") {
	test_json(CPUTESTS_DIR"/4e.json");
}

TEST_CASE("CPU Test 4f", "[core]") {
	test_json(CPUTESTS_DIR"/4f.json");
}

TEST_CASE("CPU Test 50", "[core]") {
	test_json(CPUTESTS_DIR"/50.json");
}

TEST_CASE("CPU Test 51", "[core]") {
	test_json(CPUTESTS_DIR"/51.json");
}

TEST_CASE("CPU Test 52", "[core]") {
	test_json(CPUTESTS_DIR"/52.json");
}

TEST_CASE("CPU Test 53", "[core]") {
	test_json(CPUTESTS_DIR"/53.json");
}

TEST_CASE("CPU Test 54", "[core]") {
	test_json(CPUTESTS_DIR"/54.json");
}

TEST_CASE("CPU Test 55", "[core]") {
	test_json(CPUTESTS_DIR"/55.json");
}

TEST_CASE("CPU Test 56", "[core]") {
	test_json(CPUTESTS_DIR"/56.json");
}

TEST_CASE("CPU Test 57", "[core]") {
	test_json(CPUTESTS_DIR"/57.json");
}

TEST_CASE("CPU Test 58", "[core]") {
	test_json(CPUTESTS_DIR"/58.json");
}

TEST_CASE("CPU Test 59", "[core]") {
	test_json(CPUTESTS_DIR"/59.json");
}

TEST_CASE("CPU Test 5a", "[core]") {
	test_json(CPUTESTS_DIR"/5a.json");
}

TEST_CASE("CPU Test 5b", "[core]") {
	test_json(CPUTESTS_DIR"/5b.json");
}

TEST_CASE("CPU Test 5c", "[core]") {
	test_json(CPUTESTS_DIR"/5c.json");
}

TEST_CASE("CPU Test 5d", "[core]") {
	test_json(CPUTESTS_DIR"/5d.json");
}

TEST_CASE("CPU Test 5e", "[core]") {
	test_json(CPUTESTS_DIR"/5e.json");
}

TEST_CASE("CPU Test 5f", "[core]") {
	test_json(CPUTESTS_DIR"/5f.json");
}

TEST_CASE("CPU Test 60", "[core]") {
	test_json(CPUTESTS_DIR"/60.json");
}

TEST_CASE("CPU Test 61", "[core]") {
	test_json(CPUTESTS_DIR"/61.json");
}

TEST_CASE("CPU Test 62", "[core]") {
	test_json(CPUTESTS_DIR"/62.json");
}

TEST_CASE("CPU Test 63", "[core]") {
	test_json(CPUTESTS_DIR"/63.json");
}

TEST_CASE("CPU Test 64", "[core]") {
	test_json(CPUTESTS_DIR"/64.json");
}

TEST_CASE("CPU Test 65", "[core]") {
	test_json(CPUTESTS_DIR"/65.json");
}

TEST_CASE("CPU Test 66", "[core]") {
	test_json(CPUTESTS_DIR"/66.json");
}

TEST_CASE("CPU Test 67", "[core]") {
	test_json(CPUTESTS_DIR"/67.json");
}

TEST_CASE("CPU Test 68", "[core]") {
	test_json(CPUTESTS_DIR"/68.json");
}

TEST_CASE("CPU Test 69", "[core]") {
	test_json(CPUTESTS_DIR"/69.json");
}

TEST_CASE("CPU Test 6a", "[core]") {
	test_json(CPUTESTS_DIR"/6a.json");
}

TEST_CASE("CPU Test 6b", "[core]") {
	test_json(CPUTESTS_DIR"/6b.json");
}

TEST_CASE("CPU Test 6c", "[core]") {
	test_json(CPUTESTS_DIR"/6c.json");
}

TEST_CASE("CPU Test 6d", "[core]") {
	test_json(CPUTESTS_DIR"/6d.json");
}

TEST_CASE("CPU Test 6e", "[core]") {
	test_json(CPUTESTS_DIR"/6e.json");
}

TEST_CASE("CPU Test 6f", "[core]") {
	test_json(CPUTESTS_DIR"/6f.json");
}

TEST_CASE("CPU Test 70", "[core]") {
	test_json(CPUTESTS_DIR"/70.json");
}

TEST_CASE("CPU Test 71", "[core]") {
	test_json(CPUTESTS_DIR"/71.json");
}

TEST_CASE("CPU Test 72", "[core]") {
	test_json(CPUTESTS_DIR"/72.json");
}

TEST_CASE("CPU Test 73", "[core]") {
	test_json(CPUTESTS_DIR"/73.json");
}

TEST_CASE("CPU Test 74", "[core]") {
	test_json(CPUTESTS_DIR"/74.json");
}

TEST_CASE("CPU Test 75", "[core]") {
	test_json(CPUTESTS_DIR"/75.json");
}

// TEST_CASE("CPU Test 76", "[core]") {
// 	test_json(CPUTESTS_DIR"/76.json");
// }

TEST_CASE("CPU Test 77", "[core]") {
	test_json(CPUTESTS_DIR"/77.json");
}

TEST_CASE("CPU Test 78", "[core]") {
	test_json(CPUTESTS_DIR"/78.json");
}

TEST_CASE("CPU Test 79", "[core]") {
	test_json(CPUTESTS_DIR"/79.json");
}

TEST_CASE("CPU Test 7a", "[core]") {
	test_json(CPUTESTS_DIR"/7a.json");
}

TEST_CASE("CPU Test 7b", "[core]") {
	test_json(CPUTESTS_DIR"/7b.json");
}

TEST_CASE("CPU Test 7c", "[core]") {
	test_json(CPUTESTS_DIR"/7c.json");
}

TEST_CASE("CPU Test 7d", "[core]") {
	test_json(CPUTESTS_DIR"/7d.json");
}

TEST_CASE("CPU Test 7e", "[core]") {
	test_json(CPUTESTS_DIR"/7e.json");
}

TEST_CASE("CPU Test 7f", "[core]") {
	test_json(CPUTESTS_DIR"/7f.json");
}

TEST_CASE("CPU Test 80", "[core]") {
	test_json(CPUTESTS_DIR"/80.json");
}

TEST_CASE("CPU Test 81", "[core]") {
	test_json(CPUTESTS_DIR"/81.json");
}

TEST_CASE("CPU Test 82", "[core]") {
	test_json(CPUTESTS_DIR"/82.json");
}

TEST_CASE("CPU Test 83", "[core]") {
	test_json(CPUTESTS_DIR"/83.json");
}

TEST_CASE("CPU Test 84", "[core]") {
	test_json(CPUTESTS_DIR"/84.json");
}

TEST_CASE("CPU Test 85", "[core]") {
	test_json(CPUTESTS_DIR"/85.json");
}

TEST_CASE("CPU Test 86", "[core]") {
	test_json(CPUTESTS_DIR"/86.json");
}

TEST_CASE("CPU Test 87", "[core]") {
	test_json(CPUTESTS_DIR"/87.json");
}

TEST_CASE("CPU Test 88", "[core]") {
	test_json(CPUTESTS_DIR"/88.json");
}

TEST_CASE("CPU Test 89", "[core]") {
	test_json(CPUTESTS_DIR"/89.json");
}

TEST_CASE("CPU Test 8a", "[core]") {
	test_json(CPUTESTS_DIR"/8a.json");
}

TEST_CASE("CPU Test 8b", "[core]") {
	test_json(CPUTESTS_DIR"/8b.json");
}

TEST_CASE("CPU Test 8c", "[core]") {
	test_json(CPUTESTS_DIR"/8c.json");
}

TEST_CASE("CPU Test 8d", "[core]") {
	test_json(CPUTESTS_DIR"/8d.json");
}

TEST_CASE("CPU Test 8e", "[core]") {
	test_json(CPUTESTS_DIR"/8e.json");
}

TEST_CASE("CPU Test 8f", "[core]") {
	test_json(CPUTESTS_DIR"/8f.json");
}

TEST_CASE("CPU Test 90", "[core]") {
	test_json(CPUTESTS_DIR"/90.json");
}

TEST_CASE("CPU Test 91", "[core]") {
	test_json(CPUTESTS_DIR"/91.json");
}

TEST_CASE("CPU Test 92", "[core]") {
	test_json(CPUTESTS_DIR"/92.json");
}

TEST_CASE("CPU Test 93", "[core]") {
	test_json(CPUTESTS_DIR"/93.json");
}

TEST_CASE("CPU Test 94", "[core]") {
	test_json(CPUTESTS_DIR"/94.json");
}

TEST_CASE("CPU Test 95", "[core]") {
	test_json(CPUTESTS_DIR"/95.json");
}

TEST_CASE("CPU Test 96", "[core]") {
	test_json(CPUTESTS_DIR"/96.json");
}

TEST_CASE("CPU Test 97", "[core]") {
	test_json(CPUTESTS_DIR"/97.json");
}

TEST_CASE("CPU Test 98", "[core]") {
	test_json(CPUTESTS_DIR"/98.json");
}

TEST_CASE("CPU Test 99", "[core]") {
	test_json(CPUTESTS_DIR"/99.json");
}

TEST_CASE("CPU Test 9a", "[core]") {
	test_json(CPUTESTS_DIR"/9a.json");
}

TEST_CASE("CPU Test 9b", "[core]") {
	test_json(CPUTESTS_DIR"/9b.json");
}

TEST_CASE("CPU Test 9c", "[core]") {
	test_json(CPUTESTS_DIR"/9c.json");
}

TEST_CASE("CPU Test 9d", "[core]") {
	test_json(CPUTESTS_DIR"/9d.json");
}

TEST_CASE("CPU Test 9e", "[core]") {
	test_json(CPUTESTS_DIR"/9e.json");
}

TEST_CASE("CPU Test 9f", "[core]") {
	test_json(CPUTESTS_DIR"/9f.json");
}

TEST_CASE("CPU Test a0", "[core]") {
	test_json(CPUTESTS_DIR"/a0.json");
}

TEST_CASE("CPU Test a1", "[core]") {
	test_json(CPUTESTS_DIR"/a1.json");
}

TEST_CASE("CPU Test a2", "[core]") {
	test_json(CPUTESTS_DIR"/a2.json");
}

TEST_CASE("CPU Test a3", "[core]") {
	test_json(CPUTESTS_DIR"/a3.json");
}

TEST_CASE("CPU Test a4", "[core]") {
	test_json(CPUTESTS_DIR"/a4.json");
}

TEST_CASE("CPU Test a5", "[core]") {
	test_json(CPUTESTS_DIR"/a5.json");
}

TEST_CASE("CPU Test a6", "[core]") {
	test_json(CPUTESTS_DIR"/a6.json");
}

TEST_CASE("CPU Test a7", "[core]") {
	test_json(CPUTESTS_DIR"/a7.json");
}

TEST_CASE("CPU Test a8", "[core]") {
	test_json(CPUTESTS_DIR"/a8.json");
}

TEST_CASE("CPU Test a9", "[core]") {
	test_json(CPUTESTS_DIR"/a9.json");
}

TEST_CASE("CPU Test aa", "[core]") {
	test_json(CPUTESTS_DIR"/aa.json");
}

TEST_CASE("CPU Test ab", "[core]") {
	test_json(CPUTESTS_DIR"/ab.json");
}

TEST_CASE("CPU Test ac", "[core]") {
	test_json(CPUTESTS_DIR"/ac.json");
}

TEST_CASE("CPU Test ad", "[core]") {
	test_json(CPUTESTS_DIR"/ad.json");
}

TEST_CASE("CPU Test ae", "[core]") {
	test_json(CPUTESTS_DIR"/ae.json");
}

TEST_CASE("CPU Test af", "[core]") {
	test_json(CPUTESTS_DIR"/af.json");
}

TEST_CASE("CPU Test b0", "[core]") {
	test_json(CPUTESTS_DIR"/b0.json");
}

TEST_CASE("CPU Test b1", "[core]") {
	test_json(CPUTESTS_DIR"/b1.json");
}

TEST_CASE("CPU Test b2", "[core]") {
	test_json(CPUTESTS_DIR"/b2.json");
}

TEST_CASE("CPU Test b3", "[core]") {
	test_json(CPUTESTS_DIR"/b3.json");
}

TEST_CASE("CPU Test b4", "[core]") {
	test_json(CPUTESTS_DIR"/b4.json");
}

TEST_CASE("CPU Test b5", "[core]") {
	test_json(CPUTESTS_DIR"/b5.json");
}

TEST_CASE("CPU Test b6", "[core]") {
	test_json(CPUTESTS_DIR"/b6.json");
}

TEST_CASE("CPU Test b7", "[core]") {
	test_json(CPUTESTS_DIR"/b7.json");
}

TEST_CASE("CPU Test b8", "[core]") {
	test_json(CPUTESTS_DIR"/b8.json");
}

TEST_CASE("CPU Test b9", "[core]") {
	test_json(CPUTESTS_DIR"/b9.json");
}

TEST_CASE("CPU Test ba", "[core]") {
	test_json(CPUTESTS_DIR"/ba.json");
}

TEST_CASE("CPU Test bb", "[core]") {
	test_json(CPUTESTS_DIR"/bb.json");
}

TEST_CASE("CPU Test bc", "[core]") {
	test_json(CPUTESTS_DIR"/bc.json");
}

TEST_CASE("CPU Test bd", "[core]") {
	test_json(CPUTESTS_DIR"/bd.json");
}

TEST_CASE("CPU Test be", "[core]") {
	test_json(CPUTESTS_DIR"/be.json");
}

TEST_CASE("CPU Test bf", "[core]") {
	test_json(CPUTESTS_DIR"/bf.json");
}

TEST_CASE("CPU Test c0", "[core]") {
	test_json(CPUTESTS_DIR"/c0.json");
}

TEST_CASE("CPU Test c1", "[core]") {
	test_json(CPUTESTS_DIR"/c1.json");
}

TEST_CASE("CPU Test c2", "[core]") {
	test_json(CPUTESTS_DIR"/c2.json");
}

TEST_CASE("CPU Test c3", "[core]") {
	test_json(CPUTESTS_DIR"/c3.json");
}

TEST_CASE("CPU Test c4", "[core]") {
	test_json(CPUTESTS_DIR"/c4.json");
}

TEST_CASE("CPU Test c5", "[core]") {
	test_json(CPUTESTS_DIR"/c5.json");
}

TEST_CASE("CPU Test c6", "[core]") {
	test_json(CPUTESTS_DIR"/c6.json");
}

TEST_CASE("CPU Test c7", "[core]") {
	test_json(CPUTESTS_DIR"/c7.json");
}

TEST_CASE("CPU Test c8", "[core]") {
	test_json(CPUTESTS_DIR"/c8.json");
}

TEST_CASE("CPU Test c9", "[core]") {
	test_json(CPUTESTS_DIR"/c9.json");
}

TEST_CASE("CPU Test ca", "[core]") {
	test_json(CPUTESTS_DIR"/ca.json");
}

TEST_CASE("CPU Test cb", "[core]") {
	test_json(CPUTESTS_DIR"/cb.json");
}

TEST_CASE("CPU Test cc", "[core]") {
	test_json(CPUTESTS_DIR"/cc.json");
}

TEST_CASE("CPU Test cd", "[core]") {
	test_json(CPUTESTS_DIR"/cd.json");
}

TEST_CASE("CPU Test ce", "[core]") {
	test_json(CPUTESTS_DIR"/ce.json");
}

TEST_CASE("CPU Test cf", "[core]") {
	test_json(CPUTESTS_DIR"/cf.json");
}

TEST_CASE("CPU Test d0", "[core]") {
	test_json(CPUTESTS_DIR"/d0.json");
}

TEST_CASE("CPU Test d1", "[core]") {
	test_json(CPUTESTS_DIR"/d1.json");
}

TEST_CASE("CPU Test d2", "[core]") {
	test_json(CPUTESTS_DIR"/d2.json");
}

// TEST_CASE("CPU Test d3", "[core]") {
// 	test_json(CPUTESTS_DIR"/d3.json");
// }

TEST_CASE("CPU Test d4", "[core]") {
	test_json(CPUTESTS_DIR"/d4.json");
}

TEST_CASE("CPU Test d5", "[core]") {
	test_json(CPUTESTS_DIR"/d5.json");
}

TEST_CASE("CPU Test d6", "[core]") {
	test_json(CPUTESTS_DIR"/d6.json");
}

TEST_CASE("CPU Test d7", "[core]") {
	test_json(CPUTESTS_DIR"/d7.json");
}

TEST_CASE("CPU Test d8", "[core]") {
	test_json(CPUTESTS_DIR"/d8.json");
}

TEST_CASE("CPU Test d9", "[core]") {
	test_json(CPUTESTS_DIR"/d9.json");
}

TEST_CASE("CPU Test da", "[core]") {
	test_json(CPUTESTS_DIR"/da.json");
}

// TEST_CASE("CPU Test db", "[core]") {
// 	test_json(CPUTESTS_DIR"/db.json");
// }

TEST_CASE("CPU Test dc", "[core]") {
	test_json(CPUTESTS_DIR"/dc.json");
}

// TEST_CASE("CPU Test dd", "[core]") {
// 	test_json(CPUTESTS_DIR"/dd.json");
// }

TEST_CASE("CPU Test de", "[core]") {
	test_json(CPUTESTS_DIR"/de.json");
}

TEST_CASE("CPU Test df", "[core]") {
	test_json(CPUTESTS_DIR"/df.json");
}

TEST_CASE("CPU Test e0", "[core]") {
	test_json(CPUTESTS_DIR"/e0.json");
}

TEST_CASE("CPU Test e1", "[core]") {
	test_json(CPUTESTS_DIR"/e1.json");
}

TEST_CASE("CPU Test e2", "[core]") {
	test_json(CPUTESTS_DIR"/e2.json");
}

// TEST_CASE("CPU Test e3", "[core]") {
// 	test_json(CPUTESTS_DIR"/e3.json");
// }

// TEST_CASE("CPU Test e4", "[core]") {
// 	test_json(CPUTESTS_DIR"/e4.json");
// }

TEST_CASE("CPU Test e5", "[core]") {
	test_json(CPUTESTS_DIR"/e5.json");
}

TEST_CASE("CPU Test e6", "[core]") {
	test_json(CPUTESTS_DIR"/e6.json");
}

TEST_CASE("CPU Test e7", "[core]") {
	test_json(CPUTESTS_DIR"/e7.json");
}

TEST_CASE("CPU Test e8", "[core]") {
	test_json(CPUTESTS_DIR"/e8.json");
}

TEST_CASE("CPU Test e9", "[core]") {
	test_json(CPUTESTS_DIR"/e9.json");
}

TEST_CASE("CPU Test ea", "[core]") {
	test_json(CPUTESTS_DIR"/ea.json");
}

// TEST_CASE("CPU Test eb", "[core]") {
// 	test_json(CPUTESTS_DIR"/eb.json");
// }

// TEST_CASE("CPU Test ec", "[core]") {
// 	test_json(CPUTESTS_DIR"/ec.json");
// }

// TEST_CASE("CPU Test ed", "[core]") {
// 	test_json(CPUTESTS_DIR"/ed.json");
// }

TEST_CASE("CPU Test ee", "[core]") {
	test_json(CPUTESTS_DIR"/ee.json");
}

TEST_CASE("CPU Test ef", "[core]") {
	test_json(CPUTESTS_DIR"/ef.json");
}

TEST_CASE("CPU Test f0", "[core]") {
	test_json(CPUTESTS_DIR"/f0.json");
}

TEST_CASE("CPU Test f1", "[core]") {
	test_json(CPUTESTS_DIR"/f1.json");
}

TEST_CASE("CPU Test f2", "[core]") {
	test_json(CPUTESTS_DIR"/f2.json");
}

// TEST_CASE("CPU Test f3", "[core]") {
// 	test_json(CPUTESTS_DIR"/f3.json");
// }

// TEST_CASE("CPU Test f4", "[core]") {
// 	test_json(CPUTESTS_DIR"/f4.json");
// }

TEST_CASE("CPU Test f5", "[core]") {
	test_json(CPUTESTS_DIR"/f5.json");
}

TEST_CASE("CPU Test f6", "[core]") {
	test_json(CPUTESTS_DIR"/f6.json");
}

TEST_CASE("CPU Test f7", "[core]") {
	test_json(CPUTESTS_DIR"/f7.json");
}

TEST_CASE("CPU Test f8", "[core]") {
	test_json(CPUTESTS_DIR"/f8.json");
}

TEST_CASE("CPU Test f9", "[core]") {
	test_json(CPUTESTS_DIR"/f9.json");
}

TEST_CASE("CPU Test fa", "[core]") {
	test_json(CPUTESTS_DIR"/fa.json");
}

// TEST_CASE("CPU Test fb", "[core]") {
// 	test_json(CPUTESTS_DIR"/fb.json");
// }

// TEST_CASE("CPU Test fc", "[core]") {
// 	test_json(CPUTESTS_DIR"/fc.json");
// }

// TEST_CASE("CPU Test fd", "[core]") {
// 	test_json(CPUTESTS_DIR"/fd.json");
// }

TEST_CASE("CPU Test fe", "[core]") {
	test_json(CPUTESTS_DIR"/fe.json");
}

TEST_CASE("CPU Test ff", "[core]") {
	test_json(CPUTESTS_DIR"/ff.json");
}
