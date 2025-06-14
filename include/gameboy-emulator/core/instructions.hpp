#pragma once

#include <cstdint>

namespace emulator
{

/**@brief No operation is performed.
 */
void nop();

/**@brief Load value into register.
 *
 *@param reg Register to load into
 *@param val Data to load into register
 */
void ld(uint16_t &reg, const uint16_t &val);

/**@brief Load value into register.
 *
 *@param reg Register to load into
 *@param val Data to load into register
 */
void ld(uint8_t &reg, const uint8_t &val);

/**@brief Switch GameBoy into low power mode.
 */
void stop();

/**@brief Jump absolutely the program counter to another address.
 *
 *@param pc Program counter reference
 *@param address Address to jump to
 */
void jp(uint16_t &pc, const uint16_t &address);

/**@brief Jump relatively the program counter to another address.
 *
 *@param pc Program counter reference
 *@param offset Jump amount (signed value)
 *@param af CPU af register
 */
void jr(uint16_t &pc, const uint8_t &offset);


/**@brief Pop the top stack entry into the program counter. Wraps pop.
 *
 *@param pc Program counter reference
 *@param top Reference to the top of the stack
 *@param sp Stack pointer reference
 */
void ret(uint16_t &pc, uint16_t &top, uint16_t &sp);

/**@brief Pop the top stack entry into a register.
 *
 *@param reg Register to pop into
 *@param top Reference to the top of the stack
 *@param sp Stack pointer reference
 */
void pop(uint16_t &reg, uint16_t &top, uint16_t &sp);

/**@brief Push a value onto the stack.
 *
 *@param top Reference to the top of the stack
 *@param val Value to push onto the stack
 *@param sp Stack pointer reference
 */
void push(uint8_t &top, const uint8_t &val, uint16_t &sp);

/**@brief Push a value onto the stack.
 *
 *@param top Reference to the top of the stack
 *@param val Value to push onto the stack
 *@param sp Stack pointer reference
 */
void push(uint16_t &top, const uint16_t &val, uint16_t &sp);

/**@brief Push the current pc value plus 3 onto the stack, then load it with a new value.
 * 
 *@param pc Reference to the program counter
 *@param top Reference to the top of the stack
 *@param val Value to load into pc
 *@param sp Reference to the stack pointer
 */
void call(uint16_t &pc, uint16_t &top, const uint16_t &val, uint16_t &sp);
 
/**@brief Push the current pc value plus 1 onto the stack, then load it with a number.
 *
 *@param pc Reference to the program counter
 *@param val New value to load into pc
 *@param top Reference to the top of the stack
 *@param sp Reference to the stack pointer
 */
void rst(uint16_t &pc, const uint8_t &val, uint16_t &top, uint16_t &sp);

} // namespace emulator
