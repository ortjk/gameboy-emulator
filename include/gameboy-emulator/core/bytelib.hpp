#pragma once

#include <cstdint>

namespace emulator
{

/**@brief Convert two bytes into one 16-bit number.
 *
 *@param msB Most significant byte
 *@param lsB Least significant byte
 */
uint16_t bytes_to_16b(const uint8_t &msB, const uint8_t &lsB);

/**@brief Convert a two's complement 8-bit number into a two's complement 16-bit number.
 *
 *@param b Byte to convert
 */
uint16_t byte_to_16b(const uint8_t &b);

/**@brief Set a bit within an 8-bit number.
 *
 *@param i Index of bit
 *@param b Number to edit
 */
void set_bit(int i, uint8_t &b);

/**@brief Set a bit within a 16-bit number.
 *
 *@param i Index of bit
 *@param b Number to edit
 */
void set_bit(int i, uint16_t &b);

/**@brief Reset a bit within an 8-bit number.
 *
 *@param i Index of bit
 *@param b Number to edit
 */
void reset_bit(int i, uint8_t &b);

/**@brief Reset a bit within a 16-bit number.
 *
 *@param i Index of bit
 *@param b Number to edit
 */
void reset_bit(int i, uint16_t &b);

bool check_bit(int i, const uint8_t &b);

} // namespace emulator
