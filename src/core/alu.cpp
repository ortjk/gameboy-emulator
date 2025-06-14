#include "gameboy-emulator/core/alu.hpp"

#include "gameboy-emulator/core/bytelib.hpp"

namespace emulator::alu
{

void add(uint16_t &x, const uint16_t &y, uint8_t &flags)
{
    uint32_t z = x + y;
    uint16_t lo = (x & 0x0FFF) + (y & 0x0FFF);

    x = static_cast<uint16_t>(z);

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    if (lo >> 12) { set_bit(5, flags); } else { reset_bit(5, flags); }  // half carry flag
    if (z >> 16) { set_bit(4, flags); } else { reset_bit(4, flags); }  // carry flag
}

void add(uint8_t &x, const uint8_t &y, uint8_t &flags)
{
    uint16_t z = x + y;
    uint8_t lo = (x & 0x0F) + (y & 0x0F);
    
    x = static_cast<uint8_t>(z);

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    reset_bit(6, flags); // subtraction flag
    if (lo >> 4) { set_bit(5, flags); } else { reset_bit(5, flags); }  // half carry flag
    if (z >> 8) { set_bit(4, flags); } else { reset_bit(4, flags); }  // carry flag
}

void add(uint16_t &x, const uint8_t &y, uint8_t &flags)
{
    uint16_t _y = byte_to_16b(y);
    uint8_t _x = static_cast<uint8_t>(x);

    uint16_t z = _x + y;
    uint8_t lo = (x & 0x0F) + (y & 0x0F);

    x = static_cast<uint16_t>(x + _y);
    
    reset_bit(7, flags);
    reset_bit(6, flags);
    if (lo >> 4) { set_bit(5, flags); } else { reset_bit(5, flags); }
    if (z >> 8) { set_bit(4, flags); } else { reset_bit(4, flags); }
}

void adc(uint8_t &x, const uint8_t &y, uint8_t &flags)
{
    uint8_t c = (flags & 0b00010000) >> 4;
    uint16_t z = x + y + c;
    uint8_t lo = (x & 0x0F) + (y & 0x0F) + c;

    x = static_cast<uint8_t>(z);

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    reset_bit(6, flags); // subtraction flag
    if (lo >> 4) { set_bit(5, flags); } else { reset_bit(5, flags); }  // half carry flag
    if (z >> 8) { set_bit(4, flags); } else { reset_bit(4, flags); }  // carry flag
}

void sub(uint8_t &x, const uint8_t &y, uint8_t &flags)
{
    uint16_t z = x - y;
    uint8_t lo = (x & 0x0F) - (y & 0x0F);

    x = static_cast<uint8_t>(z);

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    set_bit(6, flags); // subtraction flag
    if (lo >> 4) { set_bit(5, flags); } else { reset_bit(5, flags); }  // half carry flag
    if (z >> 8) { set_bit(4, flags); } else { reset_bit(4, flags); }  // carry flag
}

void sbc(uint8_t &x, const uint8_t &y, uint8_t &flags)
{
    uint8_t c = (flags & 0b00010000) >> 4;
    uint16_t z = x - y - c;
    uint8_t lo = (x & 0x0F) - (y & 0x0F) - c;

    x = static_cast<uint8_t>(z);

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    set_bit(6, flags); // subtraction flag
    if (lo >> 4) { set_bit(5, flags); } else { reset_bit(5, flags); }  // half carry flag
    if (z >> 8) { set_bit(4, flags); } else { reset_bit(4, flags); }  // carry flag
}

void _and(uint8_t &x, const uint8_t &y, uint8_t &flags)
{
    x = x & y;

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    reset_bit(6, flags); // subtraction flag
    set_bit(5, flags); // half carry flag
    reset_bit(4, flags); // carry flag
}

void _or(uint8_t &x, const uint8_t &y, uint8_t &flags)
{
    x = x | y;

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    reset_bit(4, flags); // carry flag
}

void _xor(uint8_t &x, const uint8_t &y, uint8_t &flags)
{
    x = x ^ y;

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    reset_bit(4, flags); // carry flag
}

void cp(uint8_t &x, const uint8_t &y, uint8_t &flags)
{
    uint16_t z = x - y;
    uint8_t lo = (x & 0x0F) - (y & 0x0F);

    if (z == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    set_bit(6, flags); // subtraction flag
    if (lo >> 4) { set_bit(5, flags); } else { reset_bit(5, flags); }  // half carry flag
    if (z >> 8) { set_bit(4, flags); } else { reset_bit(4, flags); }  // carry flag
}

void inc(uint16_t &x, uint8_t &flags)
{
    x++;
}

void inc(uint8_t &x, uint8_t &flags)
{
    uint8_t lo = (x & 0x0F) + 1;
    x++;

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    reset_bit(6, flags); // subtraction flag
    if (lo >> 4) { set_bit(5, flags); } else { reset_bit(5, flags); }  // half carry flag
}

void dec(uint16_t &x, uint8_t &flags)
{
    x--;
}

void dec(uint8_t &x, uint8_t &flags)
{
    uint8_t lo = (x & 0x0F) - 1;
    x--;

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); }  // zero flag
    set_bit(6, flags); // subtraction flag
    if (lo >> 4) { set_bit(5, flags); } else { reset_bit(5, flags); }  // half carry flag
}

void rlc(uint8_t &x, uint8_t &flags)
{
    bool msb = x >> 7;
    x = x << 1;

    if (msb)
    {
        set_bit(0, x); // set lsb
    }

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    if (msb) { set_bit(4, flags); } else { reset_bit(4, flags); } // carry flag
}

void rl(uint8_t &x, uint8_t &flags)
{
    bool msb = x >> 7; // get msb of x
    uint8_t c = (flags >> 4) & 0x01; // get carry flag contents
    x = x << 1; // rotate x left
    x = x | c; // roll through carry flag contents

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    if (msb) { set_bit(4, flags); } else { reset_bit(4, flags); } // carry flag
}

void rrc(uint8_t &x, uint8_t &flags)
{
    bool lsb = x & 0x01;
    x = x >> 1;
    if (lsb)
    {
        set_bit(7, x); // set msb
    }

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    if (lsb) { set_bit(4, flags); } else { reset_bit(4, flags); } // carry flag
}

void rr(uint8_t &x, uint8_t &flags)
{
    bool lsb = x & 0x01;
    bool c = (flags >> 4) & 0x01; // get carry flag contents
    x = x >> 1; // rotate x right
    x = x | (c << 7);

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    if (lsb) { set_bit(4, flags); } else { reset_bit(4, flags); } // carry flag
}

void sla(uint8_t &x, uint8_t &flags)
{
    bool msb = x >> 7;
    x = x << 1;

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    if (msb) { set_bit(4, flags); } else { reset_bit(4, flags); } // carry flag
}

void sra(uint8_t &x, uint8_t &flags)
{
    bool lsb = x & 0x01;
    uint8_t msb = x & 0x80;
    x = x >> 1;
    x = x | msb;

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    if (lsb) { set_bit(4, flags); } else { reset_bit(4, flags); } // carry flag
}

void srl(uint8_t &x, uint8_t &flags)
{
    bool lsb = x & 0x01;
    x = x >> 1;

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    if (lsb) { set_bit(4, flags); } else { reset_bit(4, flags); } // carry flag
}

void swap(uint8_t &x, uint8_t &flags)
{
    uint8_t lower = x & 0x0F;
    x = (x >> 4) | (lower << 4);

    if (x == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    reset_bit(5, flags); // half carry flag
    reset_bit(4, flags); // carry flag
}

void bit(uint8_t &i, uint8_t &x, uint8_t &flags)
{
    bool is_set = x & (0x01 << i);

    if (!is_set) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(6, flags); // subtraction flag
    set_bit(5, flags); // half carry flag
}

// https://blog.ollien.com/posts/gb-daa/
void daa(uint8_t &a, uint8_t &flags)
{
    uint8_t offset = 0;
    bool half_carry = flags & 0b00100000;
    bool carry = flags & 0b00010000;
    bool subtract = flags & 0b01000000;
    
    if ((!subtract && (a & 0x0F) > 0x09) || half_carry)
    {
        offset = offset | 0x06;
    }

    if ((!subtract && a > 0x99) || carry)
    {
        offset = offset | 0x60;
        set_bit(4, flags);
    }

    if (!subtract)
    {
        a += offset;
    }
    else 
    {
        a -= offset;
    }

    if (a == 0) { set_bit(7, flags); } else { reset_bit(7, flags); } // zero flag
    reset_bit(5, flags); // half carry flag
}

void cpl(uint8_t &x, uint8_t &flags)
{
    x = ~x;

    set_bit(6, flags); // subtract flag
    set_bit(5, flags); // half carry flag
}

void scf(uint8_t &flags)
{
    set_bit(4, flags);

    reset_bit(6, flags); // subtract flag
    reset_bit(5, flags); // half carry flag
    set_bit(4, flags); // carry flag
}

void ccf(uint8_t &flags)
{
    bool c = (flags & 0b00010000);

    reset_bit(6, flags); // subtract flag
    reset_bit(5, flags); // half carry flag
    if (c) { reset_bit(4, flags); } else { set_bit(4, flags); } // carry flag
}

} // namespace emulator::alu
