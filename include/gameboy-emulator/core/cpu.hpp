#pragma once

#include <cstdint>

#include "gameboy-emulator/core/alu.hpp"
#include "gameboy-emulator/core/instructions.hpp"
#include "gameboy-emulator/core/bytelib.hpp"
#include "gameboy-emulator/core/memory.hpp"

namespace emulator {

struct opcode_values {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t p;
    uint8_t q;
};

class CPU {
private:
    static uint16_t af; // lower 8 bits flags register
    static uint16_t bc;
    static uint16_t de;
    static uint16_t hl;
    static uint16_t sp; // stack pointer
    static uint16_t pc; // program counter

    // 8-bit register table
    static uint8_t *r[8];

    // register pairs featuring sp
    static uint16_t *rp[4];

    // register pairs featuring af
    static uint16_t *rf[4];

    // arithmetic/logic operations
    static alu::alu_8b_f al[8];

    // rotation/shift operations
    static alu::rot_8b_f rot[8];

    // wrapper functions for alu functions on a
    static void adda(uint8_t &_, const uint8_t &b, uint8_t &flags);
    static void adca(uint8_t &_, const uint8_t &b, uint8_t &flags);
    static void suba(uint8_t &_, const uint8_t &b, uint8_t &flags);
    static void sbca(uint8_t &_, const uint8_t &b, uint8_t &flags);
    static void anda(uint8_t &_, const uint8_t &b, uint8_t &flags);
    static void xora(uint8_t &_, const uint8_t &b, uint8_t &flags);
    static void ora(uint8_t &_, const uint8_t &b, uint8_t &flags);
    static void cpa(uint8_t &_, const uint8_t &b, uint8_t &flags);

    static struct opcode_values get_opcode_values(const uint8_t &opcode);

public:
    static uint16_t t;

    /**@brief Emulate a GameBoy Z80 instruction.
     *
     * @param b3 First byte of instruction
     * @param b2 Second byte of instruction
     * @param b1 Third byte of instruction
     * @param b0 Fourth byte of instruction
     */
    static void instruction(const uint8_t &b3, const uint8_t &b2, const uint8_t &b1, const uint8_t &b0);

#ifdef CMAKE_BUILD_TESTING
    /**@brief Helper function for testing to directly set a
     *
     *@param z Byte to load
     */
    static void set_a(const uint8_t &z);

    /**@brief Helper function for testing to directly set b
     *
     *@param z Byte to load
     */
    static void set_b(const uint8_t &z);

    /**@brief Helper function for testing to directly set c
     *
     *@param z Byte to load
     */
    static void set_c(const uint8_t &z);

    /**@brief Helper function for testing to directly set d
     *
     *@param z Byte to load
     */
    static void set_d(const uint8_t &z);

    /**@brief Helper function for testing to directly set e
     *
     *@param z Byte to load
     */
    static void set_e(const uint8_t &z);

    /**@brief Helper function for testing to directly set f
     *
     *@param z Byte to load
     */
    static void set_f(const uint8_t &z);

    /**@brief Helper function for testing to directly set h
     *
     *@param z Byte to load
     */
    static void set_h(const uint8_t &z);

    /**@brief Helper function for testing to directly set l
     *
     *@param z Byte to load
     */
    static void set_l(const uint8_t &z);

    /**@brief Helper function for testing to directly set pc
     *
     *@param z Byte to load
     */
    static void set_pc(const uint16_t &z);

    /**@brief Helper function for testing to directly set sp
     *
     *@param z Byte to load
     */
    static void set_sp(const uint16_t &z);

    /**@brief Helper function for testing to directly get a
     */
    static uint8_t get_a();

    /**@brief Helper function for testing to directly get b
     */
    static uint8_t get_b();

    /**@brief Helper function for testing to directly get c
     */
    static uint8_t get_c();

    /**@brief Helper function for testing to directly get d
     */
    static uint8_t get_d();

    /**@brief Helper function for testing to directly get e
     */
    static uint8_t get_e();

    /**@brief Helper function for testing to directly get f
     */
    static uint8_t get_f();

    /**@brief Helper function for testing to directly get h
     */
    static uint8_t get_h();

    /**@brief Helper function for testing to directly get l
     */
    static uint8_t get_l();

    /**@brief Helper function for testing to directly get pc
     */
    static uint16_t get_pc();

    /**@brief Helper function for testing to directly get sp
     */
    static uint16_t get_sp();
#endif
};

} // namespace emulator
