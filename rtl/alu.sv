module alu(
    input  logic [31:0] A,
    input  logic [31:0] B, 
    input  logic [3:0]  alu_cntr,
    output logic [31:0] result, 
    output logic zero
);

    always_comb begin
        case (alu_cntr)
            4'b0000 : result = A + B;
            4'b1000 : result = A - B;
            4'b0111 : result = A & B; 
            4'b0110 : result = A | B;
            4'b0100 : result = A ^ B;
            4'b0001 : result = A << B[4:0];
            4'b0101 : result = A >> B[4:0];
            4'b1101 : result = $signed(A) >>> B[4:0];
            4'b0010 : result = $signed(A) < $signed(B) ? 1 : 0;
            4'b0011 : result = A < B ? 1 : 0;
            4'b1111 : result = B;
            default : result = 32'b0;
        endcase
    end
   
    assign zero = (result == 32'b0);

endmodule