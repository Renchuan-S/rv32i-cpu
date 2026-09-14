#include "Vreg_file.h"
#include "verilated.h"

#include <iostream>
#include <iomanip>
#include <cstdint>

int tests_passed = 0;
int tests_failed = 0;

void tick(Vreg_file* dut) {
    dut->clk = 0;
    dut->eval();

    dut->clk = 1;
    dut->eval();

    dut->clk = 0;
    dut->eval();
}

void check(
    Vreg_file* dut,
    uint8_t rs1,
    uint8_t rs2,
    uint32_t expected1,
    uint32_t expected2,
    const char* name
) {
    dut->rs1 = rs1;
    dut->rs2 = rs2;
    dut->eval();

    uint32_t actual1 = dut->rd1;
    uint32_t actual2 = dut->rd2;

    bool pass =
        (actual1 == expected1) &&
        (actual2 == expected2);

    if (pass) {
        std::cout
            << "[PASS] " << name
            << " rd1=0x"
            << std::hex
            << std::setw(8)
            << std::setfill('0')
            << actual1
            << " rd2=0x"
            << std::setw(8)
            << actual2
            << std::dec
            << std::endl;

        tests_passed++;
    } else {
        std::cout
            << "[FAIL] " << name
            << "\n       expected rd1=0x"
            << std::hex
            << std::setw(8)
            << std::setfill('0')
            << expected1
            << " rd2=0x"
            << std::setw(8)
            << expected2
            << "\n       actual   rd1=0x"
            << std::setw(8)
            << actual1
            << " rd2=0x"
            << std::setw(8)
            << actual2
            << std::dec
            << std::endl;

        tests_failed++;
    }
}

int main(int argc, char** argv) {

    Verilated::commandArgs(argc, argv);

    Vreg_file* dut = new Vreg_file;

    // Initial values
    dut->clk = 0;
    dut->we  = 0;
    dut->rs1 = 0;
    dut->rs2 = 0;
    dut->rd  = 0;
    dut->wd  = 0;

    dut->eval();

    // ------------------------------------------------
    // Test x0 always reads zero
    // ------------------------------------------------

    check(
        dut,
        0,
        0,
        0,
        0,
        "x0 reads zero"
    );

    // ------------------------------------------------
    // Write 0x12345678 to x1
    // ------------------------------------------------

    dut->we = 1;
    dut->rd = 1;
    dut->wd = 0x12345678;

    tick(dut);

    dut->we = 0;

    check(
        dut,
        1,
        0,
        0x12345678,
        0,
        "write/read x1"
    );

    // ------------------------------------------------
    // Write 42 to x2
    // ------------------------------------------------

    dut->we = 1;
    dut->rd = 2;
    dut->wd = 42;

    tick(dut);

    dut->we = 0;

    check(
        dut,
        1,
        2,
        0x12345678,
        42,
        "two read ports"
    );

    // ------------------------------------------------
    // Attempt to overwrite x0
    // ------------------------------------------------

    dut->we = 1;
    dut->rd = 0;
    dut->wd = 0xFFFFFFFF;

    tick(dut);

    dut->we = 0;

    check(
        dut,
        0,
        1,
        0,
        0x12345678,
        "x0 cannot be overwritten"
    );

    // ------------------------------------------------
    // Write enable = 0 should prevent writes
    // ------------------------------------------------

    dut->we = 0;
    dut->rd = 3;
    dut->wd = 0xDEADBEEF;

    tick(dut);

    check(
        dut,
        3,
        2,
        0,
        42,
        "write disabled"
    );

    std::cout << "\n============================\n";
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;

    if (tests_failed == 0) {
        std::cout << "ALL REG_FILE TESTS PASSED\n";
    } else {
        std::cout << "REG_FILE TEST FAILED\n";
    }

    std::cout << "============================\n";

    delete dut;

    return tests_failed == 0 ? 0 : 1;
}