module final (
    // Clock and control inputs
    input CLOCK_50,           // 50MHz clock input (PIN_M9)
    input [2:0] SW,           // Control switches:


    // Onboard outputs
    output [3:0] LEDR,        // Onboard red LEDs (4-bit binary count)
   
    output [0:6] HEX0,        // 7-segment display (PIN_U21 to PIN_AA22)
   
    // Breadboard outputs
    output [7:0] GPIO_LEDS    // GPIO-connected LEDs (8-bit walking pattern)


);


    // =============================================
    // 1. Frequency Divider (25-bit counter)
    // =============================================
    wire [24:0] div_counter;
   
    lpm_counter freq_divider (
        .clock (CLOCK_50),
        .q (div_counter),
        .aclr (1'b0),
        .aload (1'b0),
        .aset (1'b0),
        .cin (1'b1),
        .clk_en (1'b1),
        .cnt_en (1'b1),
        .cout (),
        .data ({25{1'b0}}),
        .eq (),
        .sclr (1'b0),
        .sload (1'b0),
        .sset (1'b0),
        .updown (1'b1)
    );
   
    defparam
        freq_divider.lpm_direction = "UP",
        freq_divider.lpm_port_updown = "PORT_UNUSED",
        freq_divider.lpm_type = "LPM_COUNTER",
        freq_divider.lpm_width = 25;
   
    // 2. Main 4-bit Counter
    reg [3:0] main_counter;
   
    always @(posedge slow_clock, negedge SW[1]) begin
        if (!SW[1]) begin           // Active-low clear
            main_counter <= 4'b0000;
        end
        else if (SW[0]) begin       // Enable
            if (SW[2])              // Direction (1=up, 0=down)
                main_counter <= main_counter + 1;
            else
                main_counter <= main_counter - 1;
        end
    end
   
        // 3. Decoders
     
    // Binary to 8-bit walking pattern decoder
    wire [7:0] walking_pattern;
    assign walking_pattern = (1 << main_counter[2:0]);
   
    // Hex to 7-segment decoder
    reg [0:6] hex_display;
    always @(*) begin
        case (main_counter)
            4'h0: hex_display = 7'b0000001;  // 0
            4'h1: hex_display = 7'b1001111;  // 1
            4'h2: hex_display = 7'b0010010;  // 2
            4'h3: hex_display = 7'b0000110;  // 3
            4'h4: hex_display = 7'b1001100;  // 4
            4'h5: hex_display = 7'b0100100;  // 5
            4'h6: hex_display = 7'b0100000;  // 6
            4'h7: hex_display = 7'b0001111;  // 7
            4'h8: hex_display = 7'b0000000;  // 8
            4'h9: hex_display = 7'b0000100;  // 9
            4'hA: hex_display = 7'b0001000;  // A
            4'hB: hex_display = 7'b1100000;  // b
            4'hC: hex_display = 7'b0110001;  // C
            4'hD: hex_display = 7'b1000010;  // d
            4'hE: hex_display = 7'b0110000;  // E
            4'hF: hex_display = 7'b0111000;  // F
            default: hex_display = 7'bxxxxxxx;
        endcase
    end
   
    // =============================================
    // 4. Output Assignments
    // =============================================
    assign LEDR = main_counter;      // Binary count on onboard LEDs
    assign HEX0 = hex_display;       // Hex value on 7-segment
    assign GPIO_LEDS = walking_pattern; // Walking pattern on breadboard
   
endmodule
