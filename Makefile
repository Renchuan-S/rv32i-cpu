SIM = verilator

# --------------------
# CPU
# --------------------

cpu:
	mkdir -p sim
	$(SIM) --cc --trace --build --exe \
		--top-module cpu \
		rtl/*.sv \
		tb/tb_cpu.cpp \
		-o cpu_sim

run-arithmetic:
	./obj_dir/cpu_sim +PROGRAM=tb/programs/arithmetic.hex

run-memory:
	./obj_dir/cpu_sim +PROGRAM=tb/programs/memory.hex

run-control:
	./obj_dir/cpu_sim +PROGRAM=tb/programs/control_flow.hex

# --------------------
# Unit tests
# --------------------

alu:
	$(SIM) --cc --build --exe \
		--top-module alu \
		rtl/alu.sv \
		tb/tb_alu.cpp \
		-o alu_sim
	./obj_dir/alu_sim

imm:
	$(SIM) --cc --build --exe \
		--top-module imm_gen \
		rtl/imm_gen.sv \
		tb/tb_imm_gen.cpp \
		-o imm_sim
	./obj_dir/imm_sim

reg:
	$(SIM) --cc --build --exe \
		--top-module reg_file \
		rtl/reg_file.sv \
		tb/tb_reg_file.cpp \
		-o reg_sim
	./obj_dir/reg_sim

control:
	$(SIM) --cc --build --exe \
		--top-module control_unit \
		rtl/control_unit.sv \
		tb/tb_control_unit.cpp \
		-o control_sim
	./obj_dir/control_sim

# --------------------
# Utilities
# --------------------

wave:
	gtkwave sim/cpu_sim.vcd

clean:
	rm -rf obj_dir
	rm -f sim/*.vcd