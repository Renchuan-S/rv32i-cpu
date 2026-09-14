module control_unit(
    input logic [6:0]opcode,
    input logic [2:0]function3,
    input logic [6:0]function7,
    output logic reg_write,
    output logic [3:0] alu_cntr,
    output logic alu_src,
    output logic mem_write,
    output logic mem_read,
    output logic mem_to_reg,
    output logic branch,
    output logic auipc_sel,
    output logic jal,
    output logic jalr
);

always_comb begin

    reg_write = 1'b0;
    alu_src = 1'b0;
    mem_write = 1'b0;
    mem_read = 1'b0;
    mem_to_reg = 1'b0;
    branch = 1'b0;
    alu_cntr = 4'b0000;
    auipc_sel = 1'b0;
    jal = 1'b0;
    jalr = 1'b0;

    case (opcode)
        7'b0110011 : begin  //R-type
            reg_write = 1'b1;
            alu_src = 1'b0;
            mem_write = 1'b0;
            mem_read = 1'b0;
            mem_to_reg = 1'b0;
            case ({function7[5],function3})
                4'b0_000 : alu_cntr = 4'b0000;   //Add
                4'b1_000 : alu_cntr = 4'b1000;   //Sub
                4'b0_100 : alu_cntr = 4'b0100;   //Xor
                4'b0_110 : alu_cntr = 4'b0110;   //Or
                4'b0_111 : alu_cntr = 4'b0111;   //And
                4'b0_001 : alu_cntr = 4'b0001;   //Sll
                4'b0_101 : alu_cntr = 4'b0101;   //Srl
                4'b1_101 : alu_cntr = 4'b1101;   //Sra
                4'b0_010 : alu_cntr = 4'b0010;   //Slt
                4'b0_011 : alu_cntr = 4'b0011;   //Sltu
                default : alu_cntr = 4'b0000;
            endcase
        end

        7'b0010011 : begin  //I-type
            reg_write = 1'b1;
            alu_src = 1'b1;
            mem_write = 1'b0;
            mem_read = 1'b0;
            mem_to_reg = 1'b0;
            case(function3)
                3'b000 : alu_cntr = 4'b0000;   //Addi
                3'b100 : alu_cntr = 4'b0100;   //Xori
                3'b110 : alu_cntr = 4'b0110;   //Ori
                3'b111 : alu_cntr = 4'b0111;   //Andi
                3'b010 : alu_cntr = 4'b0010;   //Slti
                3'b011 : alu_cntr = 4'b0011;   //Sltiu
                3'b001 : alu_cntr = 4'b0001;   //Slli
                3'b101 : begin
                    if(function7[5])
                    alu_cntr = 4'b1101;   //Srai
                    else
                    alu_cntr = 4'b0101;   //Srli
                end
                default : alu_cntr = 4'b0000;
            endcase
        end

        7'b0000011 : begin  //I-type Load
            reg_write = 1'b1;
            alu_src = 1'b1;
            mem_write = 1'b0;
            mem_read = 1'b1;
            mem_to_reg = 1'b1;
            case(function3)
                3'b010 : alu_cntr = 4'b0000;   //Lw
                default : alu_cntr = 4'b0000;
            endcase
        end

        7'b0100011 : begin  //S-type
            reg_write = 1'b0;
            alu_src = 1'b1;
            mem_write = 1'b1;
            mem_read = 1'b0;
            mem_to_reg = 1'b0;   
            case(function3)
                3'b010 : alu_cntr = 4'b0000;   //Sw
                default : alu_cntr = 4'b0000;
            endcase 
        end

        7'b1100011 : begin  //B-type
            alu_src = 1'b0;
            branch = 1'b1;
            case(function3)
                3'b000 : alu_cntr = 4'b1000;    //Beq
                3'b001 : alu_cntr = 4'b1000;    //Bne
                3'b100 : alu_cntr = 4'b0010;    //Blt
                3'b101 : alu_cntr = 4'b0010;    //Bge
                3'b110 : alu_cntr = 4'b0011;    //Bltu
                3'b111 : alu_cntr = 4'b0011;    //Bgeu
                default : alu_cntr = 4'b0000;
            endcase
        end

        7'b0110111 : begin  // U-type Lui
            reg_write = 1'b1;
            alu_src = 1'b1;
            mem_write = 1'b0;
            mem_read = 1'b0;
            mem_to_reg = 1'b0;
            alu_cntr = 4'b1111;
        end

        7'b0010111 : begin  // U-type Auipc
            reg_write = 1'b1;
            alu_src = 1'b1;
            mem_write = 1'b0;
            mem_read = 1'b0;
            mem_to_reg = 1'b0;
            auipc_sel = 1'b1;
            alu_cntr = 4'b0000;
        end

        7'b1101111 : begin  // J-type Jal
            reg_write = 1'b1;
            jal = 1'b1;
            alu_cntr = 4'b0000;
        end

        7'b1100111 : begin  // J-type Jalr
            reg_write = 1'b1;
            alu_src = 1'b1;
            jalr = 1'b1;
            case(function3)
                3'b000 : alu_cntr = 4'b0000;  
                default : alu_cntr = 4'b0000;
            endcase
        end


        default : begin end

    endcase
end

endmodule