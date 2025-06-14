#include "gameboy-emulator/core/cpu.hpp"

#include "gameboy-emulator/core/bytelib.hpp"
#include "gameboy-emulator/core/instructions.hpp"
#include "gameboy-emulator/core/memory.hpp"

namespace emulator {

uint16_t CPU::af = 0x0000;
uint16_t CPU::bc = 0x0000;
uint16_t CPU::de = 0x0000;
uint16_t CPU::hl = 0x0000;
uint16_t CPU::sp = 0xFFFF;
uint16_t CPU::pc = 0x0000;

uint8_t *CPU::r[8] = {
    reinterpret_cast<uint8_t *>(&bc)+1, reinterpret_cast<uint8_t *>(&bc), 
    reinterpret_cast<uint8_t *>(&de)+1, reinterpret_cast<uint8_t *>(&de), 
    reinterpret_cast<uint8_t *>(&hl)+1, reinterpret_cast<uint8_t *>(&hl), 
    nullptr, reinterpret_cast<uint8_t *>(&af)+1
};

uint16_t *CPU::rp[4] = {
    &bc, &de, &hl, &sp
};

uint16_t *CPU::rf[4] = {
    &bc, &de, &hl, &af
};

alu::alu_8b_f CPU::al[8] = {
    CPU::adda, CPU::adca, CPU::suba, CPU::sbca, CPU::anda, CPU::xora, CPU::ora, CPU::cpa
};

alu::rot_8b_f CPU::rot[8] = {
    alu::rlc, alu::rrc, alu::rl, alu::rr, alu::sla, alu::sra, alu::swap, alu::srl
};

void CPU::adda(uint8_t &_, const uint8_t &b, uint8_t &flags) { uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1; alu::add(*a, b, flags); }
void CPU::adca(uint8_t &_, const uint8_t &b, uint8_t &flags) { uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1; alu::adc(*a, b, flags); }
void CPU::suba(uint8_t &_, const uint8_t &b, uint8_t &flags) { uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1; alu::sub(*a, b, flags); }
void CPU::sbca(uint8_t &_, const uint8_t &b, uint8_t &flags) { uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1; alu::sbc(*a, b, flags); }
void CPU::anda(uint8_t &_, const uint8_t &b, uint8_t &flags) { uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1; alu::_and(*a, b, flags); }
void CPU::xora(uint8_t &_, const uint8_t &b, uint8_t &flags) { uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1; alu::_xor(*a, b, flags); }
void CPU::ora(uint8_t &_, const uint8_t &b, uint8_t &flags) { uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1; alu::_or(*a, b, flags); }
void CPU::cpa(uint8_t &_, const uint8_t &b, uint8_t &flags) { uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1; alu::cp(*a, b, flags); }

opcode_values CPU::get_opcode_values(const uint8_t &opcode)
{
    return {
        .x = static_cast<uint8_t>((opcode >> 6)),
        .y = static_cast<uint8_t>((opcode >> 3) & 0b111),
        .z = static_cast<uint8_t>(opcode & 0b111),
        .p = static_cast<uint8_t>((opcode >> 4) & 0b11),
        .q = static_cast<uint8_t>((opcode >> 3) & 0b1)
    };
}

uint16_t CPU::t = 0;

// instruction set meaning:
// 4 byte opcodes (bracketed items may or may not be present)
// either form [prefix byte] opcode [displacement byte] [immediate data]
// OR [two prefix bytes] [displacement byte] opcode
// 
// the opcode indicates the operation to perform (e.g. reg. load, arithmetic, I/O, etc.) and maybe extra parameters for operation
// 
// optional prefix byte(s) change the instruction lookup to a different bank of instructions.
// prefix bytes may have the values 0xcb, 0xdd, 0xed, 0xfd
// if the first byte has any of these values it is ALWAYS a prefix byte
//
// displacement byte is a signed 8-bit integer which specify a displacement added to a given memory address
// after reading the prefix and opcode one has enough information to determine whether a displacement byte is expected
// 
// immediate data consists of 0-2 of additional information specifying explicit parameters for certain instructions
// its presence and number of bytes are also completely determined by the instruction at hand
//
// opcode can be broken down in the following way:
// 7 6 5 4 3 2 1 0      MSB <---> LSB of opcode
// x x y y y z z z
//     p p q
//
// further reading for how to use this information: https://archive.gbdev.io/salvage/decoding_gbz80_opcodes/Decoding%20Gamboy%20Z80%20Opcodes.html
// opcode lookup table: https://clrhome.org/table/
void CPU::instruction(const uint8_t &b3, const uint8_t &b2, const uint8_t &b1, const uint8_t &b0)
{
    uint8_t *f = reinterpret_cast<uint8_t *>(&af); // get flags
    
    // prefixed opcodes
    if (b3 == 0xcb)
    {
        opcode_values ocv = get_opcode_values(b2);
        uint8_t* target;
        if (ocv.z == 0x6 || ocv.z == 0xE)
        {
            target = Memory::get_8b(hl);
            t = 16;
        }
        else
        {
            target = r[ocv.z];
            t = 8;
        }
        switch (ocv.x)
        {
        case 0:
            // rot[y] r[z]
            rot[ocv.y](*target, *f);
            break;
        case 1:
            // BIT y, r[z]
            alu::bit(ocv.y, *target, *f);
            break;
        case 2:
            // RES y, r[z]
            reset_bit(ocv.y, *target);
            break;
        case 3:
            // SET y, r[z]
            set_bit(ocv.y, *target);
            break;
        default:
            break;
        }
        pc += 2;
    }
    else if (b3 == 0xdd || b3 == 0xed || b3 == 0xfd)
    {
        // not implemented by gameboy
    }
    // unprefixed opcode
    else
    {
        opcode_values ocv = get_opcode_values(b3);
        switch (ocv.x)
        {
        case 0:
            switch (ocv.z)
            {
            case 0:
                switch (ocv.y)
                {
                case 0:
                    // NOP
                    nop();
                    t = 4;
                    pc += 1;
                    break;
                case 1:
                    // LD (nn), SP
                    {
                        uint16_t nn = bytes_to_16b(b1, b2);
                        uint16_t *reg = Memory::get_16b(nn);
                        ld(*reg, sp);
                        t = 20;
                        pc += 3;
                    }
                    break;
                case 2:
                    // STOP
                    stop();
                    t = 4;
                    pc += 1;
                    break;
                case 3:
                    // JR d
                    {
                        jr(pc, b2);
                        t = 12;
                        pc += 2;
                    }
                    break;
                case 4:
                    // JR nz, d
                    {
                        bool z = *f >> 7;
                        if (!z)
                        {
                            jr(pc, b2);
                            t = 12;
                        }
                        else 
                        {
                            t = 8;
                        }
                        pc += 2;
                    }
                    break;
                case 5:
                    // JR z, d
                    {
                        bool z = *f >> 7;
                        if (z)
                        {
                            jr(pc, b2);
                            t = 12;
                        }
                        else 
                        {
                            t = 8;
                        }
                        pc += 2;
                    }
                    break;
                case 6:
                    // JR nc, d
                    {
                        bool c = (*f >> 4) & 1;
                        if (!c)
                        {
                            jr(pc, b2);
                            t = 12;
                        }
                        else 
                        {
                            t = 8;
                        }
                        pc += 2;
                    }
                    break;
                case 7:
                    // JR c, d
                    {
                        bool c = (*f >> 4) & 1;
                        if (c)
                        {
                            jr(pc, b2);
                            t = 12;
                        }
                        else 
                        {
                            t = 8;
                        }
                        pc += 2;
                    }
                    break;
                default:
                    break;
                }
                break;
            case 1:
                switch (ocv.q)
                {
                case 0:
                    // LD rp[p], nn
                    {
                        uint16_t nn = bytes_to_16b(b1, b2);
                        ld(*rp[ocv.p], nn);
                        t = 12;
                        pc += 3;
                    }
                    break;
                case 1:
                    // ADD HL, rp[p]
                    {
                        uint8_t _f = *f;
                        alu::add(hl, *rp[ocv.p], *f);
                        *f = (*f & 0x7F) | (_f & 0x80); // different adding rules for HL
                        t = 8;
                        pc += 1;
                    }
                    break;
                default:
                    break;
                }
                break;
            case 2:
                {
                    uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1;
                    switch (ocv.q)
                    {
                    case 0:
                        switch (ocv.p)
                        {
                        case 0:
                            // ld (bc), a
                            {
                                uint8_t *reg = Memory::get_8b(bc);
                                ld(*reg, *a);
                                t = 8;
                                pc += 1;
                            }
                            break;
                        case 1:
                            // ld (de), a
                            {
                                uint8_t *reg = Memory::get_8b(de);
                                ld(*reg, *a);
                                t = 8;
                                pc += 1;
                            }
                            break;
                        case 2:
                            // ld (hl+), a
                            {
                                uint8_t *reg = Memory::get_8b(hl);
                                ld(*reg, *a);
                                alu::inc(hl, *f);
                                t = 8;
                                pc += 1;
                            }
                            break;
                        case 3:
                            // ld (hl-), a
                            {
                                uint8_t *reg = Memory::get_8b(hl);
                                ld(*reg, *a);
                                alu::dec(hl, *f);
                                t = 8;
                                pc += 1;
                            }
                            break;
                        default:
                            break;
                        }
                        break;
                    case 1:
                        switch (ocv.p)
                        {
                        case 0:
                            // ld a, (bc)
                            {
                                uint8_t *reg = Memory::get_8b(bc);
                                ld(*a, *reg);
                                t = 8;
                                pc += 1;
                            }
                            break;
                        case 1:
                            // ld a, (de)
                            {
                                uint8_t *reg = Memory::get_8b(de);
                                ld(*a, *reg);
                                t = 8;
                                pc += 1;
                            }
                            break;
                        case 2:
                            // ld a, (hl+)
                            {
                                uint8_t *reg = Memory::get_8b(hl);
                                ld(*a, *reg);
                                alu::inc(hl, *f);
                                t = 8;
                                pc += 1;
                            }
                            break;
                        case 3:
                            // ld a, (hl-)
                            {
                                uint8_t *reg = Memory::get_8b(hl);
                                ld(*a, *reg);
                                alu::dec(hl, *f);
                                t = 8;
                                pc += 1;
                            }
                            break;
                        default:
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 3:
                switch (ocv.q)
                {
                case 0:
                    // INC rp[p]
                    {
                        uint8_t _ = 0x00;
                        alu::inc(*rp[ocv.p], _);
                        t = 8;
                        pc += 1;
                    }
                    break;
                case 1:
                    // DEC rp[p]
                    {
                        uint8_t _ = 0x00;
                        alu::dec(*rp[ocv.p], _);
                        t = 8;
                        pc += 1;
                    }
                    break;
                default:
                    break;
                }
                break;
            case 4:
                // INC r[y]
                {
                    uint8_t *target;
                    if (ocv.y == 6)
                    {
                        target = Memory::get_8b(hl);
                        t = 12;
                    }
                    else 
                    {
                        target = r[ocv.y];
                        t = 4;
                    }
                    alu::inc(*target, *f);
                    pc += 1;
                }
                break;
            case 5:
                // DEC r[y]
                {
                    uint8_t *target;
                    if (ocv.y == 6)
                    {
                        target = Memory::get_8b(hl);
                        t = 12;
                    }
                    else 
                    {
                        target = r[ocv.y];
                        t = 4;
                    }
                    alu::dec(*target, *f);
                    pc += 1;
                }
                break;
            case 6:
                // LD r[y], n
                {
                    uint8_t *target;
                    if (ocv.y == 6)
                    {
                        target = Memory::get_8b(hl);
                        t = 12;
                    }
                    else 
                    {
                        target = r[ocv.y];
                        t = 8;
                    }
                    ld(*target, b2);
                    pc += 2;
                }
                break;
            case 7:
                {
                    uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1;
                    switch (ocv.y) 
                    {
                    case 0:
                        // RLCA
                        alu::rlc(*a, *f);
                        *f = *f & 0x1F;
                        t = 4;
                        pc += 1;
                        break;
                    case 1:
                        // RRCA
                        alu::rrc(*a, *f);
                        *f = *f & 0x1F;
                        t = 4;
                        pc += 1;
                        break;
                    case 2:
                        // RLA
                        alu::rl(*a, *f);
                        *f = *f & 0x1F;
                        t = 4;
                        pc += 1;
                        break;
                    case 3:
                        // RRA
                        alu::rr(*a, *f);
                        *f = *f & 0x1F;
                        t = 4;
                        pc += 1;
                        break;
                    case 4:
                        // DAA
                        alu::daa(*a, *f);
                        t = 4;
                        pc += 1;
                        break;
                    case 5:
                        // CPL
                        alu::cpl(*a, *f);
                        t = 4;
                        pc += 1;
                        break;
                    case 6:
                        // SCF
                        alu::scf(*f);
                        t = 4;
                        pc += 1;
                        break;
                    case 7:
                        // CCF
                        alu::ccf(*f);
                        t = 4;
                        pc += 1;
                        break;
                    default:
                        break;
                    }
                }
                break;
            default:
                break;
            }
            break;
        case 1:
            if (ocv.z == 6 && ocv.y == 6)
            {
                // exception
            }
            else 
            {
                // LD r[y], r[z]
                uint8_t *target;
                if (ocv.y == 6)
                {
                    target = Memory::get_8b(hl);
                }
                else 
                {
                    target = r[ocv.y];
                }

                uint8_t *val;
                if (ocv.z == 6)
                {
                    val = Memory::get_8b(hl);
                }
                else
                {
                    val = r[ocv.z];
                }

                ld(*target, *val);

                if (ocv.y == 6 || ocv.z == 6)
                {
                    t = 8;
                }
                else 
                {
                    t = 4;
                }
                pc += 1;
            }
            break;
        case 2:
            // ALU [y] r[z]
            {
                uint8_t *target;
                if (ocv.z == 6)
                {
                    target = Memory::get_8b(hl);
                    t = 8;
                }
                else 
                {
                    target = r[ocv.z];
                    t = 4;
                }

                uint8_t _;
                al[ocv.y](_, *target, *f);
                pc += 1;
            }
            break;
        case 3:
            switch (ocv.z)
            {
            case 0:
                switch (ocv.y)
                {
                case 0:
                    // RET nz
                    {
                        bool z = *f >> 7;
                        if (!z)
                        {
                            uint16_t *top = Memory::get_16b(sp);
                            ret(pc, *top, sp);
                            t = 20;
                        }
                        else
                        {
                            t = 8;
                        }
                        pc += 1;
                    }
                    break;
                case 1:
                    // RET z
                    {
                        bool z = *f >> 7;
                        if (z)
                        {
                            uint16_t *top = Memory::get_16b(sp);
                            ret(pc, *top, sp);
                            t = 20;
                        }
                        else
                        {
                            t = 8;
                        }
                        pc += 1;
                    }
                    break;
                case 2:
                    // RET nc
                    {
                        bool c = (*f >> 4) & 1;
                        if (!c)
                        {
                            uint16_t *top = Memory::get_16b(sp);
                            ret(pc, *top, sp);
                            t = 20;
                        }
                        else
                        {
                            t = 8;
                        }
                        pc += 1;
                    }
                    break;
                case 3:
                    // RET c
                    {
                        bool c = (*f >> 4) & 1;
                        if (c)
                        {
                            uint16_t *top = Memory::get_16b(sp);
                            ret(pc, *top, sp);
                            t = 20;
                        }
                        else
                        {
                            t = 8;
                        }
                        pc += 1;
                    }
                    break;
                case 4:
                    // LD (0xFF00 + n), a
                    {
                        uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1;
                        uint8_t *reg = Memory::get_8b(0xFF00 + static_cast<uint16_t>(b2));
                        ld(*reg, *a);
                        t = 12;
                        pc += 2;
                    }
                    break;
                case 5:
                    // ADD SP, d
                    alu::add(sp, b2, *f);
                    t = 16;
                    pc += 2;
                    break;
                case 6:
                    // LD A, (0xFF00 + n)
                    {
                        uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1;
                        uint8_t *reg = Memory::get_8b(0xFF00 + static_cast<uint16_t>(b2));
                        ld(*a, *reg);
                        t = 12;
                        pc += 2;
                    }
                    break;
                case 7:
                    // LD HL, SP + d
                    {
                        uint16_t _sp = sp;
                        alu::add(_sp, b2, *f);
                        ld(hl, _sp);
                        t = 12;
                        pc += 2;
                    }
                    break;
                default:
                    break;
                }
                break;
            case 1:
                switch (ocv.q)
                {
                case 0:
                    // POP rp2[p]
                    {
                        uint16_t *top = Memory::get_16b(sp);
                        pop(*rf[ocv.p], *top, sp);
                        t = 12;
                        pc += 1;

                        if (ocv.p == 3) { *f = *f & 0xF0; }
                    }
                    break;
                case 1:
                    switch (ocv.p)
                    {
                    case 0:
                        // RET
                        {
                            uint16_t *top = Memory::get_16b(sp);
                            ret(pc, *top, sp);
                            t = 16;
                            pc += 1;
                        }
                        break;
                    case 1:
                        // RETI
                        // TODO: implement interrupts
                        {
                            uint16_t *top = Memory::get_16b(sp);
                            ret(pc, *top, sp);
                            t = 16;
                            pc += 1;
                        }
                        break;
                    case 2:
                        // JP HL
                        jp(pc, hl);
                        t = 4;
                        pc += 1;
                        break;
                    case 3:
                        // LD SP, HL
                        ld(sp, hl);
                        t = 8;
                        pc += 1;
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
                break;
            case 2:
                {
                    uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1;
                switch (ocv.y)
                {
                case 0:
                    // JP NZ, nn
                    {
                        bool z = *f >> 7;
                        if (!z)
                        {
                            uint16_t nn = bytes_to_16b(b1, b2);
                            jp(pc, nn);
                            t = 16;
                            pc += 1;
                        }
                        else
                        {
                            t = 12;
                            pc += 3;
                        }
                    }
                    break;
                case 1:
                    // JP Z, nn
                    {
                        bool z = *f >> 7;
                        if (z)
                        {
                            uint16_t nn = bytes_to_16b(b1, b2);
                            jp(pc, nn);
                            t = 16;
                            pc += 1;
                        }
                        else
                        {
                            t = 12;
                            pc += 3;
                        }
                    }
                    break;
                case 2:
                    // JP NC, nn
                    {
                        bool c = (*f >> 4) & 1;
                        if (!c)
                        {
                            uint16_t nn = bytes_to_16b(b1, b2);
                            jp(pc, nn);
                            t = 16;
                            pc += 1;
                        }
                        else
                        {
                            t = 12;
                            pc += 3;
                        }
                    }
                    break;
                case 3:
                    // JP C, nn
                    {
                        bool c = (*f >> 4) & 1;
                        if (c)
                        {
                            uint16_t nn = bytes_to_16b(b1, b2);
                            jp(pc, nn);
                            t = 16;
                            pc += 1;
                        }
                        else
                        {
                            t = 12;
                            pc += 3;
                        }
                    }
                    break;
                case 4:
                    // LD (0xFF00 + C),A
                    {
                        uint8_t *c = reinterpret_cast<uint8_t *>(&bc);
                        uint8_t *reg = Memory::get_8b(0xFF00 + *c);
                        ld(*reg, *a);
                        t = 8;
                        pc += 1;
                    }
                    break;
                case 5:
                    // LD (nn), A
                    {
                        uint16_t nn = bytes_to_16b(b1, b2);
                        uint8_t *reg = Memory::get_8b(nn);
                        ld(*reg, *a);
                        t = 16;
                        pc += 3;
                    }
                    break;
                case 6:
                    // LD A, (0xFF00 + C)
                    {
                        uint8_t *c = reinterpret_cast<uint8_t *>(&bc);
                        uint8_t *reg = Memory::get_8b(0xFF00 + *c);
                        ld(*a, *reg);
                        t = 8;
                        pc += 1;
                    }
                    break;
                case 7:
                    // LD A, (nn)
                    {
                        uint16_t nn = bytes_to_16b(b1, b2);
                        uint8_t *reg = Memory::get_8b(nn);
                        ld(*a, *reg);
                        t = 16;
                        pc += 3;
                    }
                    break;
                default:
                    break;
                }
                }
                break;
            case 3:
                switch (ocv.y)
                {
                case 0:
                    // JP nn
                    {
                        uint16_t nn = bytes_to_16b(b1, b2);
                        jp(pc, nn);
                        t = 16;
                        pc += 1;
                    }
                    break;
                case 6:
                    // DI
                    break;
                case 7:
                    // EI
                    break;
                default:
                    break;
                }
                break;
            case 4:
                {
                    uint16_t *top = Memory::get_16b(sp);
                    uint16_t nn = bytes_to_16b(b1, b2);
                    switch (ocv.y)
                    {
                    case 0:
                        // CALL nz, nn
                        {
                            bool z = *f >> 7;
                            if (!z)
                            {
                                call(pc, *top, nn, sp);
                                t = 24;
                                pc += 1;
                            }
                            else 
                            {
                                t = 12;
                                pc += 3;
                            }
                        }
                        break;
                    case 1:
                        // CALL z, nn
                        {
                            bool z = *f >> 7;
                            if (z)
                            {
                                call(pc, *top, nn, sp);
                                t = 24;
                                pc += 1;
                            }
                            else 
                            {
                                t = 12;
                                pc += 3;
                            }
                        }
                        break;
                    case 2:
                        // CALL nc, nn
                        {
                            bool c = (*f >> 4) & 1;
                            if (!c)
                            {
                                call(pc, *top, nn, sp);
                                t = 24;
                                pc += 1;
                            }
                            else 
                            {
                                t = 12;
                                pc += 3;
                            }
                        }
                        break;
                    case 3:
                        // CALL c, nn
                        {
                            bool c = (*f >> 4) & 1;
                            if (c)
                            {
                                call(pc, *top, nn, sp);
                                t = 24;
                                pc += 1;
                            }
                            else 
                            {
                                t = 12;
                                pc += 3;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 5:
                switch (ocv.q)
                {
                case 0:
                    // PUSH rp2[p]
                    {
                        uint16_t *top = Memory::get_16b(sp);
                        push(*top, *rf[ocv.p], sp);
                        t = 16;
                        pc += 1;
                    }
                    break;
                case 1:
                    if (ocv.p == 0)
                    {
                        // CALL nn
                        uint16_t *top = Memory::get_16b(sp);
                        uint16_t nn = bytes_to_16b(b1, b2);
                        call(pc, *top, nn, sp);
                        t = 24;
                        pc += 1;
                    }
                    break;
                default:
                    break;
                }
                break;
            case 6:
                // alu[y] n
                {
                    uint8_t _;
                    al[ocv.y](_, b2, *f);
                    t = 8;
                    pc += 2;
                }
                break;
            case 7:
                // RST y*8
                {
                    uint16_t *top = Memory::get_16b(sp);
                    rst(pc, ocv.y * 8, *top, sp);
                    t = 16;
                    pc += 1;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}





#ifdef CMAKE_BUILD_TESTING

void CPU::set_a(const uint8_t &z)
{
    uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1;
    *a = z;
}

void CPU::set_b(const uint8_t &z)
{
    uint8_t *b = reinterpret_cast<uint8_t *>(&bc)+1;
    *b = z;
}

void CPU::set_c(const uint8_t &z)
{
    uint8_t *c = reinterpret_cast<uint8_t *>(&bc);
    *c = z;
}

void CPU::set_d(const uint8_t &z)
{
    uint8_t *d = reinterpret_cast<uint8_t *>(&de)+1;
    *d = z;
}

void CPU::set_e(const uint8_t &z)
{
    uint8_t *e = reinterpret_cast<uint8_t *>(&de);
    *e = z;
}

void CPU::set_f(const uint8_t &z)
{
    uint8_t *f = reinterpret_cast<uint8_t *>(&af);
    *f = z;
}

void CPU::set_h(const uint8_t &z)
{
    uint8_t *h = reinterpret_cast<uint8_t *>(&hl)+1;
    *h = z;
}

void CPU::set_l(const uint8_t &z)
{
    uint8_t *l = reinterpret_cast<uint8_t *>(&hl);
    *l = z;
}

void CPU::set_pc(const uint16_t &z)
{
    pc = z;
}

void CPU::set_sp(const uint16_t &z)
{
    sp = z;
}

uint8_t CPU::get_a()
{
    uint8_t *a = reinterpret_cast<uint8_t *>(&af)+1;
    return *a;
}

uint8_t CPU::get_b()
{
    uint8_t *b = reinterpret_cast<uint8_t *>(&bc)+1;
    return *b;
}

uint8_t CPU::get_c()
{
    uint8_t *c = reinterpret_cast<uint8_t *>(&bc);
    return *c;
}

uint8_t CPU::get_d()
{
    uint8_t *d = reinterpret_cast<uint8_t *>(&de)+1;
    return *d;
}

uint8_t CPU::get_e()
{
    uint8_t *e = reinterpret_cast<uint8_t *>(&de);
    return *e;
}

uint8_t CPU::get_f()
{
    uint8_t *f = reinterpret_cast<uint8_t *>(&af);
    return *f;
}

uint8_t CPU::get_h()
{
    uint8_t *h = reinterpret_cast<uint8_t *>(&hl)+1;
    return *h;
}

uint8_t CPU::get_l()
{
    uint8_t *l = reinterpret_cast<uint8_t *>(&hl);
    return *l;
}

uint16_t CPU::get_pc()
{
    return pc;
}

uint16_t CPU::get_sp()
{
    return sp;
}

#endif

} // namespace emulator
