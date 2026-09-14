#include "Vcpu.h"
#include "Vcpu___024root.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>

#define MAX_SIM_TIME 100

vluint64_t sim_time = 0;

void check_reg(
    Vcpu* cpu,
    int reg,
    uint32_t expected,
    bool& all_passed
) {
    uint32_t actual =
        cpu->rootp->cpu__DOT__register_file__DOT__regs[reg];

    if (actual == expected) {
        std::cout
            << "[PASS] x" << reg
            << " = 0x"
            << std::hex
            << std::setw(8)
            << std::setfill('0')
            << actual
            << std::dec
            << std::endl;
    } else {
        std::cout
            << "[FAIL] x" << reg
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

        all_passed = false;
    }
}

int main(int argc, char** argv) {

    Verilated::commandArgs(argc, argv);

    Vcpu* cpu = new Vcpu;

    Verilated::traceEverOn(true);

    VerilatedVcdC* vcd = new VerilatedVcdC;
    cpu->trace(vcd, 99);
    vcd->open("sim/cpu_sim.vcd");

    cpu->clk = 0;
    cpu->reset = 1;

    while (sim_time < MAX_SIM_TIME) {

        if (sim_time == 4)
            cpu->reset = 0;

        cpu->clk ^= 1;
        cpu->eval();

        vcd->dump(sim_time);

        sim_time++;
    }

    bool all_passed = true;

    std::string program = "arithmetic";

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg.find("memory.hex") != std::string::npos)
            program = "memory";

        else if (arg.find("control_flow.hex") != std::string::npos)
            program = "control_flow";

        else if (arg.find("arithmetic.hex") != std::string::npos)
            program = "arithmetic";
    }

    std::cout << "\nTesting program: " << program << "\n\n";

    if (program == "arithmetic") {

        check_reg(cpu, 1, 0x00000005, all_passed);
        check_reg(cpu, 2, 0x00000007, all_passed);
        check_reg(cpu, 3, 0x0000000C, all_passed);
        check_reg(cpu, 4, 0x00000002, all_passed);
        check_reg(cpu, 5, 0x00000005, all_passed);
        check_reg(cpu, 6, 0x0000000A, all_passed);
        check_reg(cpu, 7, 0x00000003, all_passed);
        check_reg(cpu, 8, 0x00000003, all_passed);

    } else if (program == "memory") {

        check_reg(cpu, 1, 0x0000000C, all_passed);
        check_reg(cpu, 2, 0x0000000C, all_passed);

    } else if (program == "control_flow") {

        check_reg(cpu, 1, 0x12345000, all_passed);
        check_reg(cpu, 2, 0x00001004, all_passed);
        check_reg(cpu, 3, 0x0000000C, all_passed);
        check_reg(cpu, 4, 0x00000018, all_passed);
        check_reg(cpu, 5, 0x0000000A, all_passed);
        check_reg(cpu, 6, 0xFFFFFFFF, all_passed);
        check_reg(cpu, 7, 0x00000000, all_passed);
        check_reg(cpu, 8, 0x00000000, all_passed);
        check_reg(cpu, 9, 0x0000002A, all_passed);
    }

    if (all_passed)
        std::cout << "\nALL CPU TESTS PASSED\n";
    else
        std::cout << "\nCPU TEST FAILED\n";

    vcd->close();

    delete vcd;
    delete cpu;

    return all_passed ? 0 : 1;
}