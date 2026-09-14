module imem(
    input  logic [31:0] addr,
    output logic [31:0] instr
);

logic [31:0] mem [63:0];
string program_file;

initial begin
    if (!$value$plusargs("PROGRAM=%s", program_file))
        program_file = "tb/programs/arithmetic.hex";

    $display("Loading program: %s", program_file);
    $readmemh(program_file, mem);
end

assign instr = mem[addr[7:2]];

endmodule