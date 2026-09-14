#include "Valu.h"
#include "verilated.h"

#include <iostream>
#include <iomanip>
#include <cstdint>

int tests_passed = 0;
int tests_failed = 0;

void check(
    Valu* dut,
    uint32_t A,
    uint32_t B,
    uint8_t alu_cntr,
    uint32_t expected,
    const char* test_name
) {
    dut->A = A;
    dut->B = B;
    dut->alu_cntr = alu_cntr;

    dut->eval();

    uint32_t actual = dut->result;

    if (actual == expected) {
        std::cout
            << "[PASS] "
            << test_name
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
            << "[FAIL] "
            << test_name
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

    Valu* dut = new Valu;

    // ADD
    check(dut, 5, 7, 0b0000, 12, "ADD");

    // SUB
    check(dut, 10, 3, 0b1000, 7, "SUB");

    // AND
    check(dut, 0b1100, 0b1010, 0b0111, 0b1000, "AND");

    // OR
    check(dut, 0b1100, 0b1010, 0b0110, 0b1110, "OR");

    // XOR
    check(dut, 0b1100, 0b1010, 0b0100, 0b0110, "XOR");

    // SLL
    check(dut, 1, 3, 0b0001, 8, "SLL");

    // SRL
    check(
        dut,
        0x80000000,
        1,
        0b0101,
        0x40000000,
        "SRL"
    );

    // SRA
    check(
        dut,
        0x80000000,
        1,
        0b1101,
        0xC0000000,
        "SRA"
    );

    // SLT signed: -1 < 1
    check(
        dut,
        0xFFFFFFFF,
        1,
        0b0010,
        1,
        "SLT signed"
    );

    // SLT signed: 5 < -1 should be false
    check(
        dut,
        5,
        0xFFFFFFFF,
        0b0010,
        0,
        "SLT signed false"
    );

    // SLTU: 1 < 0xFFFFFFFF
    check(
        dut,
        1,
        0xFFFFFFFF,
        0b0011,
        1,
        "SLTU"
    );

    // Pass B - used by LUI
    check(
        dut,
        123,
        0x12345000,
        0b1111,
        0x12345000,
        "PASS B"
    );

    std::cout << "\n============================\n";
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;

    if (tests_failed == 0) {
        std::cout << "ALL ALU TESTS PASSED\n";
    } else {
        std::cout << "ALU TEST FAILED\n";
    }

    std::cout << "============================\n";

    delete dut;

    return tests_failed == 0 ? 0 : 1;
}