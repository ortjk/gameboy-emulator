#pragma once

#include <cstdint>

namespace emulator::alu
{

typedef void (*alu_8b_f)(uint8_t &, const uint8_t &, uint8_t &);
typedef void (*rot_8b_f)(uint8_t &, uint8_t &);

/**@brief Add two 16-bit numbers x and y, writing the result to x.
 *
 *@param x 16-bit two's complement binary number
 *@param y 16-bit two's complement binary number
 *@param flags CPU flags register
 */
void add(uint16_t &x, const uint16_t &y, uint8_t &flags);

/**@brief Add two 8-bit numbers x and y, writing the result to x.
 *
 *@param x 8-bit two's complement binary number
 *@param y 8-bit two's complement binary number
 *@param flags CPU flags register
 */
void add(uint8_t &x, const uint8_t &y, uint8_t &flags);


/**@brief Add a 16-bit number x and signed 8-bit number y, writing the result to x.
 *
 *@param x 16-bit two's complement binary number
 *@param y 8-bit two's complement binary number
 *@param flags CPU flags register
 */
void add(uint16_t &x, const uint8_t &y, uint8_t &flags);

/**@brief Add two 8-bit numbers x and y with a carry in, writing the result to x.
 *
 *@param x 8-bit two's complement binary number
 *@param y 8-bit two's complement binary number
 *@param flags CPU flags register
 */
void adc(uint8_t &x, const uint8_t &y, uint8_t &flags);

/**@brief Subtract one 8-bit number y from another 8-bit number x.
 *
 *@param x 8-bit two's complement binary number
 *@param y 8-bit two's complement binary number
 *@param flags CPU flags register
 */
void sub(uint8_t &x, const uint8_t &y, uint8_t &flags);

/**@brief Subtract one 8-bit number y and a carry in from another 8-bit number x.
 *
 *@param x 8-bit two's complement binary number
 *@param y 8-bit two's complement binary number
 *@param flags CPU flags register
 */
void sbc(uint8_t &x, const uint8_t &y, uint8_t &flags);

/**@brief Bitwise and two 8-bit numbers x and y, writing the result to x
 *
 *@param x 8-bit number
 *@param y 8-bit number
 *@param flags CPU flags register
 */
void _and(uint8_t &x, const uint8_t &y, uint8_t &flags);

/**@brief Bitwise or two 8-bit numbers x and y, writing the result to x
 *
 *@param x 8-bit number
 *@param y 8-bit number
 *@param flags CPU flags register
 */
void _or(uint8_t &x, const uint8_t &y, uint8_t &flags);

/**@brief Bitwise xor two 8-bit numbers x and y, writing the result to x
 *
 *@param x 8-bit number
 *@param y 8-bit number
 *@param flags CPU flags register
 */
void _xor(uint8_t &x, const uint8_t &y, uint8_t &flags);

/**@brief Compare two 8-bit numbers x and y, writing the result to x
 *
 *@param x 8-bit number
 *@param y 8-bit number
 *@param flags CPU flags register
 */
void cp(uint8_t &x, const uint8_t &y, uint8_t &flags);

/**@brief Adds one to register.
 *
 *@param x Register to increment
 *@param flags CPU flags register
 */
void inc(uint16_t &x, uint8_t &flags);

/**@brief Adds one to register.
 *
 *@param x Register to increment
 *@param flags CPU flags register
 */
void inc(uint8_t &x, uint8_t &flags);

/**@brief Subtracts one from register.
 *
 *@param x Register to decrement
 *@param flags CPU flags register
 */
void dec(uint16_t &x, uint8_t &flags);

/**@brief Subtracts one from register.
 *
 *@param x Register to decrement
 *@param flags CPU flags register
 */
void dec(uint8_t &x, uint8_t &flags);

/**@brief Rotate the contents of a register left and store the leftmost bit in the carry flag, and roll it through to the lsb.
 *
 *@param x Register to rotate
 *@param flags CPU flags register
 */
void rlc(uint8_t &x, uint8_t &flags);

/**@brief Rotate the contents of a register left, store the leftmost bit in the carry flag, populate the lsb with the previous carry flag.
 *
 *@param x Register to rotate
 *@param flags CPU flags register
 */
void rl(uint8_t &x, uint8_t &flags);

/**@brief Rotate the contents of a register right and store the rightmost bit in the carry flag, and roll it through to the msb.
 *
 *@param x Register to rotate
 *@param flags CPU flags register
 */
void rrc(uint8_t &x, uint8_t &flags);

/**@brief Rotate the contents of a register right, store the rightmost bit in the carry flag, populate the msb with the previous carry flag.
 *
 *@param x Register to rotate
 *@param flags CPU flags register
 */
void rr(uint8_t &x, uint8_t &flags);

/**@brief Rotate the contents of a register left, store the leftmost bit in the carry flag, reset the lsb.
 *
 *@param x Register to rotate
 *@param flags CPU flags register
 */
void sla(uint8_t &x, uint8_t &flags);

/**@brief Rotate the contents of a register right, store the rightmost bit in the carry flag, unchange the msb.
 *
 *@param x Register to rotate
 *@param flags CPU flags register
 */
void sra(uint8_t &x, uint8_t &flags);

/**@brief Rotate the contents of a register right, store the rightmost bit in the carry flag, reset the msb.
 *
 *@param x Register to rotate
 *@param flags CPU flags register
 */
void srl(uint8_t &x, uint8_t &flags);

/**@brief Swap the upper and lower nibbles of a byte
 *
 *@param x Register to swap
 *@param flags CPU flags register
 */
void swap(uint8_t &x, uint8_t &flags);

/**@brief Tests bit i of byte x. Sets zero flag if bit is 0.
 *
 *@param i Index of bit to test
 *@param x Register to test
 *@param flags CPU flags register
 */
void bit(uint8_t &i, uint8_t &x, uint8_t &flags);

/**@brief Decimal adjust accumulator
 *
 *@param a Accumulator register
 *@param flags CPU flags register
 */
void daa(uint8_t &a, uint8_t &flags);

/**@brief Complement register
 *
 *@param x Register to complement
 *@param flags CPU flags register
 */
void cpl(uint8_t &x, uint8_t &flags);

/**@brief Set carry flag
 *
 *@param flags CPU flags register
 */
void scf(uint8_t &flags);

/**@brief Complement carry flag
 *
 *@param flags CPU flags register
 */
void ccf(uint8_t &flags);

} // namespace emulator::alu
