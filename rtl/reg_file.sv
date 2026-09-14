module reg_file(
    input logic clk,
    input logic we,
    input logic [4:0]rs1,
    input logic [4:0]rs2,
    input logic [4:0]rd,
    input logic [31:0]wd,
    output logic [31:0]rd1,
    output logic [31:0]rd2
);

logic [31:0] regs [31:0]/* verilator public_flat_rw */;

assign rd1 = (rs1 == 0) ? 32'd0 : regs[rs1];
assign rd2 = (rs2 == 0) ? 32'd0 : regs[rs2];

always_ff @(posedge clk) begin
    if (we && rd != 0) begin
        regs[rd] <= wd;
    end
end

endmodule