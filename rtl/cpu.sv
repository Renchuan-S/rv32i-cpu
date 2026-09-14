module cpu(
    input logic clk,
    input logic reset
);

logic [31:0] pc, pc_next;
logic [31:0] pc_plus4;
logic [31:0] branch_target;
logic branch, branch_taken;
logic [31:0] instr;
logic [31:0] imm;
logic reg_write, alu_src, mem_write, mem_read, mem_to_reg, auipc_sel, jal, jalr;
logic [3:0] alu_cntr;
logic [6:0] opcode;
logic [2:0] function3;
logic [6:0] function7;
logic [4:0] rs1, rs2, rd;
logic [31:0] rd1, rd2;
logic [31:0] alu_result;
logic zero;
logic [31:0] mem_rd;
logic [31:0] mux_mem_to_reg;
logic [31:0] alu_b;
logic [31:0] alu_a;

// branch logic
assign pc_plus4 = pc + 32'd4;
assign branch_target = pc + imm;
always_comb begin
    case (function3)
        3'b000 : branch_taken = branch & zero;              //Beq
        3'b001 : branch_taken = branch & !zero;             //Bne
        3'b100 : branch_taken = branch & alu_result[0];     //Blt
        3'b101 : branch_taken = branch & ~alu_result[0];    //Bge
        3'b110 : branch_taken = branch & alu_result[0];     //Bltu
        3'b111 : branch_taken = branch & ~alu_result[0];    //Bgeu
        default : branch_taken = 1'b0;
    endcase
end
always_comb begin
    if (branch_taken) pc_next = branch_target;
    else if (jal) pc_next = branch_target;
    else if (jalr) pc_next = (rd1 + imm) & ~32'b1;
    else pc_next = pc_plus4;
end

assign opcode = instr[6:0];
assign function3 = instr[14:12];
assign function7 = instr[31:25];
assign rs1 = instr[19:15];
assign rs2 = instr[24:20];
assign rd = instr[11:7];

// write back logic 
always_comb begin
    if (mem_to_reg) mux_mem_to_reg = mem_rd;
    else if (jal || jalr) mux_mem_to_reg = pc_plus4;
    else mux_mem_to_reg = alu_result;
end

assign alu_b = alu_src ? imm : rd2;
assign alu_a = auipc_sel ? pc : rd1; 

pc program_counter (.clk(clk), .reset(reset), .pc(pc), .pc_next(pc_next));

imem instruction_memory (.addr(pc), .instr(instr));

control_unit control_unit (.opcode(opcode), .function3(function3), .function7(function7), .reg_write(reg_write), .alu_cntr(alu_cntr), .alu_src(alu_src), .mem_write(mem_write), .mem_read(mem_read), .mem_to_reg(mem_to_reg), .branch(branch), .auipc_sel(auipc_sel), .jal(jal), .jalr(jalr));

imm_gen immediate_generator (.instr(instr), .opcode(opcode), .imm(imm));

reg_file register_file (.clk(clk), .we(reg_write), .rs1(rs1), .rs2(rs2), .rd(rd), .wd(mux_mem_to_reg), .rd1(rd1), .rd2(rd2));

alu alu (.A(alu_a), .B(alu_b), .alu_cntr(alu_cntr), .result(alu_result), .zero(zero));

dmem data_memory (.clk(clk), .addr(alu_result), .mem_wd(rd2), .mem_read(mem_read), .mem_write(mem_write), .mem_rd(mem_rd));

endmodule