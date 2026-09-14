module dmem(
    input logic clk,
    input logic [31:0]addr,
    input logic [31:0]mem_wd,
    input logic mem_read,
    input logic mem_write,
    output logic [31:0]mem_rd
);

logic [31:0] mem [63:0];

always_ff @(posedge clk) begin
    if (mem_write) mem[addr[7:2]] <= mem_wd;
end

always_comb begin
if (mem_read) mem_rd = mem[addr[7:2]];
else mem_rd = 32'd0;
end

endmodule