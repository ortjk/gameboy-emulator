#include "gameboy-emulator/core/instructions.hpp"
#include "gameboy-emulator/core/bytelib.hpp"

namespace emulator
{

void nop()
{

}

void ld(uint16_t &reg, const uint16_t &val)
{
    reg = val;
}

void ld(uint8_t &reg, const uint8_t &val)
{
    reg = val;
}

void stop()
{

}

void jp(uint16_t &pc, const uint16_t &address)
{
    pc = address;
}

void jr(uint16_t &pc, const uint8_t &offset)
{
    uint8_t _;
    uint16_t d = byte_to_16b(offset);
    alu::add(pc, d, _);
}

void ret(uint16_t &pc, uint16_t &top, uint16_t &sp)
{
    pop(pc, top, sp);
}

void pop(uint16_t &reg, uint16_t &top, uint16_t &sp)
{
    reg = top;
    sp += 2;
}

void push(uint8_t &top, const uint8_t &val, uint16_t &sp)
{
    *(&top - 1) = val;
    sp -= 1;
}

void push(uint16_t &top, const uint16_t &val, uint16_t &sp)
{
    *(&top - 1) = val;
    sp -= 2;
}

void call(uint16_t &pc, uint16_t &top, const uint16_t &val, uint16_t &sp)
{
    push(top, pc+2, sp); // +2 and not +3 because ret instruction increments pc by +1
    pc = val;
}

void rst(uint16_t &pc, const uint8_t &val, uint16_t &top, uint16_t &sp)
{
    push(top, pc, sp);
    pc = val;
}

} // namespace emulator
