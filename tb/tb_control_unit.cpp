#include "Vcontrol_unit.h"
#include "verilated.h"

#include <iostream>
#include <iomanip>
#include <cstdint>

int tests_passed = 0;
int tests_failed = 0;

struct Expected {
    uint8_t reg_write;
    uint8_t alu_cntr;
    uint8_t alu_src;
    uint8_t mem_write;
    uint8_t mem_read;
    uint8_t mem_to_reg;
    uint8_t branch;
    uint8_t auipc_sel;
    uint8_t jal;
    uint8_t jalr;
};

void check(
    Vcontrol_unit* dut,
    uint8_t opcode,
    uint8_t function3,
    uint8_t function7,
    Expected exp,
    const char* name
) {
    dut->opcode = opcode;
    dut->function3 = function3;
    dut->function7 = function7;

    dut->eval();

    bool pass =
        dut->reg_write  == exp.reg_write &&
        dut->alu_cntr   == exp.alu_cntr &&
        dut->alu_src    == exp.alu_src &&
        dut->mem_write  == exp.mem_write &&
        dut->mem_read   == exp.mem_read &&
        dut->mem_to_reg == exp.mem_to_reg &&
        dut->branch     == exp.branch &&
        dut->auipc_sel  == exp.auipc_sel &&
        dut->jal        == exp.jal &&
        dut->jalr       == exp.jalr;

    if (pass) {
        std::cout << "[PASS] " << name << std::endl;
        tests_passed++;
    } else {
        std::cout << "[FAIL] " << name << std::endl;

        std::cout
            << "       reg_write  expected " << int(exp.reg_write)
            << " got " << int(dut->reg_write) << "\n"

            << "       alu_cntr   expected 0x"
            << std::hex << int(exp.alu_cntr)
            << " got 0x" << int(dut->alu_cntr) << std::dec << "\n"

            << "       alu_src    expected " << int(exp.alu_src)
            << " got " << int(dut->alu_src) << "\n"

            << "       mem_write  expected " << int(exp.mem_write)
            << " got " << int(dut->mem_write) << "\n"

            << "       mem_read   expected " << int(exp.mem_read)
            << " got " << int(dut->mem_read) << "\n"

            << "       mem_to_reg expected " << int(exp.mem_to_reg)
            << " got " << int(dut->mem_to_reg) << "\n"

            << "       branch     expected " << int(exp.branch)
            << " got " << int(dut->branch) << "\n"

            << "       auipc_sel  expected " << int(exp.auipc_sel)
            << " got " << int(dut->auipc_sel) << "\n"

            << "       jal        expected " << int(exp.jal)
            << " got " << int(dut->jal) << "\n"

            << "       jalr       expected " << int(exp.jalr)
            << " got " << int(dut->jalr) << std::endl;

        tests_failed++;
    }
}

int main(int argc, char** argv) {

    Verilated::commandArgs(argc, argv);

    Vcontrol_unit* dut = new Vcontrol_unit;

    // ------------------------------------------------
    // R-type
    // ------------------------------------------------

    check(
        dut,
        0b0110011,
        0b000,
        0b0000000,
        {1, 0b0000, 0, 0, 0, 0, 0, 0, 0, 0},
        "ADD"
    );

    check(
        dut,
        0b0110011,
        0b000,
        0b0100000,
        {1, 0b1000, 0, 0, 0, 0, 0, 0, 0, 0},
        "SUB"
    );

    check(
        dut,
        0b0110011,
        0b111,
        0b0000000,
        {1, 0b0111, 0, 0, 0, 0, 0, 0, 0, 0},
        "AND"
    );

    check(
        dut,
        0b0110011,
        0b010,
        0b0000000,
        {1, 0b0010, 0, 0, 0, 0, 0, 0, 0, 0},
        "SLT"
    );

    // ------------------------------------------------
    // I-type ALU
    // ------------------------------------------------

    check(
        dut,
        0b0010011,
        0b000,
        0b0000000,
        {1, 0b0000, 1, 0, 0, 0, 0, 0, 0, 0},
        "ADDI"
    );

    check(
        dut,
        0b0010011,
        0b100,
        0b0000000,
        {1, 0b0100, 1, 0, 0, 0, 0, 0, 0, 0},
        "XORI"
    );

    check(
        dut,
        0b0010011,
        0b101,
        0b0000000,
        {1, 0b0101, 1, 0, 0, 0, 0, 0, 0, 0},
        "SRLI"
    );

    check(
        dut,
        0b0010011,
        0b101,
        0b0100000,
        {1, 0b1101, 1, 0, 0, 0, 0, 0, 0, 0},
        "SRAI"
    );

    // ------------------------------------------------
    // Load / Store
    // ------------------------------------------------

    check(
        dut,
        0b0000011,
        0b010,
        0,
        {1, 0b0000, 1, 0, 1, 1, 0, 0, 0, 0},
        "LW"
    );

    check(
        dut,
        0b0100011,
        0b010,
        0,
        {0, 0b0000, 1, 1, 0, 0, 0, 0, 0, 0},
        "SW"
    );

    // ------------------------------------------------
    // Branches
    // ------------------------------------------------

    check(
        dut,
        0b1100011,
        0b000,
        0,
        {0, 0b1000, 0, 0, 0, 0, 1, 0, 0, 0},
        "BEQ"
    );

    check(
        dut,
        0b1100011,
        0b100,
        0,
        {0, 0b0010, 0, 0, 0, 0, 1, 0, 0, 0},
        "BLT"
    );

    check(
        dut,
        0b1100011,
        0b110,
        0,
        {0, 0b0011, 0, 0, 0, 0, 1, 0, 0, 0},
        "BLTU"
    );

    // ------------------------------------------------
    // U-type
    // ------------------------------------------------

    check(
        dut,
        0b0110111,
        0,
        0,
        {1, 0b1111, 1, 0, 0, 0, 0, 0, 0, 0},
        "LUI"
    );

    check(
        dut,
        0b0010111,
        0,
        0,
        {1, 0b0000, 1, 0, 0, 0, 0, 1, 0, 0},
        "AUIPC"
    );

    // ------------------------------------------------
    // Jumps
    // ------------------------------------------------

    check(
        dut,
        0b1101111,
        0,
        0,
        {1, 0b0000, 0, 0, 0, 0, 0, 0, 1, 0},
        "JAL"
    );

    check(
        dut,
        0b1100111,
        0b000,
        0,
        {1, 0b0000, 1, 0, 0, 0, 0, 0, 0, 1},
        "JALR"
    );

    // ------------------------------------------------
    // Unknown opcode should return defaults
    // ------------------------------------------------

    check(
        dut,
        0b1111111,
        0,
        0,
        {0, 0b0000, 0, 0, 0, 0, 0, 0, 0, 0},
        "UNKNOWN OPCODE"
    );

    std::cout << "\n============================\n";
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;

    if (tests_failed == 0) {
        std::cout << "ALL CONTROL UNIT TESTS PASSED\n";
    } else {
        std::cout << "CONTROL UNIT TEST FAILED\n";
    }

    std::cout << "============================\n";

    delete dut;

    return tests_failed == 0 ? 0 : 1;
}