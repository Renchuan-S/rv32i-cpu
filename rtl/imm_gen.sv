module imm_gen(
    input logic [31:0] instr,
    input logic [6:0] opcode,
    output logic [31:0] imm
);

always_comb begin
    case (opcode)
    7'b0010011 : imm = {{20{instr[31]}}, instr[31:20]}; //I-type
    7'b0000011 : imm = {{20{instr[31]}}, instr[31:20]}; //I-type Load
    7'b0100011 : imm = {{20{instr[31]}}, instr[31:25], instr[11:7]}; //S-type Store
    7'b1100011 : imm = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0}; //B-type
    7'b0110111 : imm = {instr[31:12], 12'd0}; //U-type Lui
    7'b0010111 : imm = {instr[31:12], 12'd0}; //U-type Auipc
    7'b1101111 : imm = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0}; //J-type Jal
    7'b1100111 : imm = {{20{instr[31]}}, instr[31:20]};//I-type Jarl

    default : imm = 32'd0;
    endcase

end

endmodule