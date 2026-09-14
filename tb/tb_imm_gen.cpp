#include "Vimm_gen.h"
#include "verilated.h"

#include <iostream>
#include <iomanip>
#include <cstdint>

int tests_passed = 0;
int tests_failed = 0;

void check(
    Vimm_gen* dut,
    uint32_t instr,
    uint8_t opcode,
    uint32_t expected,
    const char* name
) {
    dut->instr = instr;
    dut->opcode = opcode;

    dut->eval();

    uint32_t actual = dut->imm;

    if (actual == expected) {
        std::cout
            << "[PASS] " << name
            << " -> 0x"
            << std::hex
            << std::setw(8)
            << std::setfill('0')
            << actual
            << std::dec
            << std::endl;

        tests_passed++;
    } else {
        std::cout
            << "[FAIL] " << name
            << " expected 0x"
            << std::hex
            << std::setw(8)
            << std::setfill('0')
            << expected
            << " but got 0x"
            << std::setw(8)
            << actual
            << std::dec
            << std::endl;

        tests_failed++;
    }
}

int main(int argc, char** argv) {

    Verilated::commandArgs(argc, argv);

    Vimm_gen* dut = new Vimm_gen;

    // ADDI x1, x0, 5
    check(
        dut,
        0x00500093,
        0b0010011,
        0x00000005,
        "I-type positive"
    );

    // ADDI x6, x0, -1
    check(
        dut,
        0xFFF00313,
        0b0010011,
        0xFFFFFFFF,
        "I-type negative"
    );

    // SW x3, 0(x0)
    check(
        dut,
        0x00302023,
        0b0100011,
        0x00000000,
        "S-type"
    );

    // BLT x6, x5, 8
    check(
        dut,
        0x00534463,
        0b1100011,
        0x00000008,
        "B-type"
    );

    // LUI x1, 0x12345
    check(
        dut,
        0x123450B7,
        0b0110111,
        0x12345000,
        "U-type LUI"
    );

    // AUIPC x2, 0x1
    check(
        dut,
        0x00001117,
        0b0010111,
        0x00001000,
        "U-type AUIPC"
    );

    // JAL x3, 12
    check(
        dut,
        0x00C001EF,
        0b1101111,
        0x0000000C,
        "J-type JAL"
    );

    // JALR x4, 4(x3)
    check(
        dut,
        0x00418267,
        0b1100111,
        0x00000004,
        "I-type JALR"
    );

    std::cout << "\n============================\n";
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;

    if (tests_failed == 0) {
        std::cout << "ALL IMM_GEN TESTS PASSED\n";
    } else {
        std::cout << "IMM_GEN TEST FAILED\n";
    }

    std::cout << "============================\n";

    delete dut;

    return tests_failed == 0 ? 0 : 1;
}