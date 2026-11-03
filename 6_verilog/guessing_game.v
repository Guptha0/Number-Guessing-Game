// ==============================================================================
// Tier 6B: Digital Hardware Architecture (Verilog-2001 RTL Model)
// Synthesizable Silicon Circuit: Logic Gates, Flip-Flops, LFSR, and FSM
// Targets FPGA / ASIC Implementations (e.g., Xilinx Artix-7 / Intel Cyclone)
// ==============================================================================

`timescale 1ns / 1ps

module number_guessing_core (
    input  wire        clk,            // System master clock (e.g. 50 MHz)
    input  wire        rst_n,          // Active-low asynchronous reset
    input  wire        btn_submit,     // Pulse input: user submits switch input
    input  wire [6:0]  sw_guess,       // 7-bit parallel DIP switches (0 - 127)
    
    // Physical Output Indicators
    output reg         led_higher,     // Amber LED: Target is greater
    output reg         led_lower,      // Blue LED:  Target is smaller
    output reg         led_win,        // Green LED: Target match
    output reg         led_game_over,  // Red LED:   Attempts exhausted
    output reg  [2:0]  attempts_left,  // 3-bit attempts remaining counter (0-7)
    output wire [6:0]  seg_display     // 7-segment display bus for attempt count
);

    // -------------------------------------------------------------------------
    // 1. Hardware Pseudo-Random Number Generator: 8-bit Fibonacci LFSR
    // Polynomial: x^8 + x^6 + x^5 + x^4 + 1 (Taps at bits 8, 6, 5, 4)
    // -------------------------------------------------------------------------
    reg [7:0] lfsr_reg;
    wire      lfsr_feedback = lfsr_reg[7] ^ lfsr_reg[5] ^ lfsr_reg[4] ^ lfsr_reg[3];

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            lfsr_reg <= 8'hA5; // Non-zero seed
        end else begin
            lfsr_reg <= {lfsr_reg[6:0], lfsr_feedback};
        end
    end

    // -------------------------------------------------------------------------
    // 2. Finite State Machine (FSM) States
    // -------------------------------------------------------------------------
    localparam STATE_IDLE     = 3'b000;
    localparam STATE_LATCH    = 3'b001;
    localparam STATE_AWAIT    = 3'b010;
    localparam STATE_COMPARE  = 3'b011;
    localparam STATE_WIN      = 3'b100;
    localparam STATE_DEFEAT   = 3'b101;

    reg [2:0] current_state, next_state;
    reg [6:0] secret_target;

    // Edge detector for submit button
    reg btn_prev;
    wire btn_pressed = (btn_submit && !btn_prev);

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) btn_prev <= 1'b0;
        else        btn_prev <= btn_submit;
    end

    // -------------------------------------------------------------------------
    // 3. FSM State Transitions & Sequential Logic
    // -------------------------------------------------------------------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            current_state <= STATE_IDLE;
            secret_target <= 7'd0;
            attempts_left <= 3'd7;
            led_higher    <= 1'b0;
            led_lower     <= 1'b0;
            led_win       <= 1'b0;
            led_game_over <= 1'b0;
        end else begin
            current_state <= next_state;

            case (current_state)
                STATE_IDLE: begin
                    // Latch LFSR value mapped to range 1-100
                    secret_target <= (lfsr_reg[6:0] % 7'd100) + 7'd1;
                    attempts_left <= 3'd7;
                    led_higher    <= 1'b0;
                    led_lower     <= 1'b0;
                    led_win       <= 1'b0;
                    led_game_over <= 1'b0;
                end

                STATE_COMPARE: begin
                    attempts_left <= attempts_left - 3'd1;

                    // Digital Comparator Logic
                    if (sw_guess == secret_target) begin
                        led_win    <= 1'b1;
                        led_higher <= 1'b0;
                        led_lower  <= 1'b0;
                    end else if (sw_guess < secret_target) begin
                        led_higher <= 1'b1;
                        led_lower  <= 1'b0;
                    end else begin
                        led_higher <= 1'b0;
                        led_lower  <= 1'b1;
                    end
                end

                STATE_DEFEAT: begin
                    led_game_over <= 1'b1;
                    led_higher    <= 1'b0;
                    led_lower     <= 1'b0;
                end
            endcase
        end
    end

    // Next State Combinational Logic
    always @(*) begin
        next_state = current_state;
        case (current_state)
            STATE_IDLE:    next_state = STATE_AWAIT;
            
            STATE_AWAIT: begin
                if (btn_pressed) next_state = STATE_COMPARE;
            end

            STATE_COMPARE: begin
                if (sw_guess == secret_target) 
                    next_state = STATE_WIN;
                else if (attempts_left <= 3'd1) 
                    next_state = STATE_DEFEAT;
                else 
                    next_state = STATE_AWAIT;
            end

            STATE_WIN:     next_state = STATE_WIN;     // Latch win
            STATE_DEFEAT:  next_state = STATE_DEFEAT;  // Latch defeat
            default:       next_state = STATE_IDLE;
        endcase
    end

    // -------------------------------------------------------------------------
    // 4. Hardware 7-Segment Decoder (Cathode Active-Low)
    // -------------------------------------------------------------------------
    reg [6:0] seg_decoded;
    always @(*) begin
        case (attempts_left)
            3'd0: seg_decoded = 7'b1000000; // 0
            3'd1: seg_decoded = 7'b1111001; // 1
            3'd2: seg_decoded = 7'b0100100; // 2
            3'd3: seg_decoded = 7'b0110000; // 3
            3'd4: seg_decoded = 7'b0011001; // 4
            3'd5: seg_decoded = 7'b0010010; // 5
            3'd6: seg_decoded = 7'b0000010; // 6
            3'd7: seg_decoded = 7'b1111000; // 7
            default: seg_decoded = 7'b1111111;
        endcase
    end
    assign seg_display = seg_decoded;

endmodule
