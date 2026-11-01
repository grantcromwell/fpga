module dotprod (
    input  wire        clk,
    input  wire        rst,
    input  wire        start,
    input  wire [20:0] a,
    input  wire [6:0] b,
    input  wire  [7:0] length,
    output reg  [27:0] result,
    output reg  done,
    output reg  busy
);

    localparam IDLE = 2'd0;
    localparam RUN  = 2'd1;
    localparam DONE = 2'd2;

    reg [1:0] state;
    reg [7:0] count;
    reg [27:0] accumulator;

   
    wire [27:0] product = a * b;

    always @(posedge clk) begin
        if (rst) begin
            state       <= IDLE;
            count       <= 8'd0;
            accumulator <= 28'd0;
            result      <= 28'd0;
            done        <= 1'b0;
            busy        <= 1'b0;
        end else begin
            done <= 1'b0;

            case (state)
                IDLE: begin
                    busy <= 1'b0;

                    if (start && (length != 0)) begin
                        count       <= 8'd0;
                        accumulator <= 28'd0;
                        busy        <= 1'b1;
                        state       <= RUN;
                    end
                end

                RUN: begin
                    accumulator <= accumulator + product;

                    if (count == length - 1'b1) begin
                        state  <= DONE;
                    end else begin
                        count <= count + 1'b1;
                    end
                end

                DONE: begin
                    result <= accumulator;
                    done  <= 1'b1;
                    busy  <= 1'b0;
                    state <= IDLE;
                end

                default: begin
                    state <= IDLE;
                    busy  <= 1'b0;
                end
            endcase
        end
    end
endmodule