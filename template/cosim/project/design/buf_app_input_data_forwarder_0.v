// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2017.4.op
// Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

(* CORE_GENERATION_INFO="buf_app_input_data_forwarder_0,hls_ip_2017_4_op,{HLS_INPUT_TYPE=cxx,HLS_INPUT_FLOAT=0,HLS_INPUT_FIXED=0,HLS_INPUT_PART=xcvu9p-flgb2104-2-i,HLS_INPUT_CLOCK=4.000000,HLS_INPUT_ARCH=others,HLS_SYN_CLOCK=2.678250,HLS_SYN_LAT=-1,HLS_SYN_TPT=none,HLS_SYN_MEM=0,HLS_SYN_DSP=0,HLS_SYN_FF=1595,HLS_SYN_LUT=255}" *)

module buf_app_input_data_forwarder_0 (
        ap_clk,
        ap_rst,
        ap_start,
        ap_done,
ap_continue,
        ap_idle,
        ap_ready,
        buf_app_input_data_V_data_V_dout,
        buf_app_input_data_V_data_V_empty_n,
        buf_app_input_data_V_data_V_read,
        buf_app_input_data_V_len_dout,
        buf_app_input_data_V_len_empty_n,
        buf_app_input_data_V_len_read,
        buf_app_input_data_V_eop_dout,
        buf_app_input_data_V_eop_empty_n,
        buf_app_input_data_V_eop_read,
        buf_read_sig_app_input_data_V_din,
        buf_read_sig_app_input_data_V_full_n,
        buf_read_sig_app_input_data_V_write,
        app_input_data_V_data_V_din,
        app_input_data_V_data_V_full_n,
        app_input_data_V_data_V_write,
        app_input_data_V_len_din,
        app_input_data_V_len_full_n,
        app_input_data_V_len_write,
        app_input_data_V_eop_din,
        app_input_data_V_eop_full_n,
        app_input_data_V_eop_write
);

parameter    ap_ST_fsm_state1 = 2'd1;
parameter    ap_ST_fsm_pp0_stage0 = 2'd2;

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output ap_ready; output ap_continue;
input  [511:0] buf_app_input_data_V_data_V_dout;
input   buf_app_input_data_V_data_V_empty_n;
output   buf_app_input_data_V_data_V_read;
input  [15:0] buf_app_input_data_V_len_dout;
input   buf_app_input_data_V_len_empty_n;
output   buf_app_input_data_V_len_read;
input   buf_app_input_data_V_eop_dout;
input   buf_app_input_data_V_eop_empty_n;
output   buf_app_input_data_V_eop_read;
output   buf_read_sig_app_input_data_V_din;
input   buf_read_sig_app_input_data_V_full_n;
output   buf_read_sig_app_input_data_V_write;
output  [511:0] app_input_data_V_data_V_din;
input   app_input_data_V_data_V_full_n;
output   app_input_data_V_data_V_write;
output  [15:0] app_input_data_V_len_din;
input   app_input_data_V_len_full_n;
output   app_input_data_V_len_write;
output   app_input_data_V_eop_din;
input   app_input_data_V_eop_full_n;
output   app_input_data_V_eop_write;

reg ap_idle;
reg buf_read_sig_app_input_data_V_write;

(* fsm_encoding = "none" *) reg   [1:0] ap_CS_fsm;
wire    ap_CS_fsm_state1;
reg   [0:0] valid_app_data_reg_175;
reg   [0:0] app_data_2_s_reg_187;
reg   [15:0] app_data_1_s_reg_198;
reg   [511:0] app_data_0_s_reg_209;
wire   [0:0] empty_n_2_fu_323_p1;
reg   [0:0] empty_n_2_reg_339;
wire    ap_CS_fsm_pp0_stage0;
wire    ap_block_state2_pp0_stage0_iter0;
wire    ap_block_state3_pp0_stage0_iter1;
wire    ap_block_pp0_stage0_11001;
reg   [0:0] ap_phi_mux_valid_app_data_phi_fu_179_p4;
reg    ap_enable_reg_pp0_iter0;
reg    ap_enable_reg_pp0_iter1;
wire    ap_block_pp0_stage0_subdone;
reg   [0:0] ap_phi_mux_valid_app_data_3_phi_fu_312_p6;
wire    ap_block_pp0_stage0;
reg   [0:0] ap_phi_mux_app_data_2_2_phi_fu_267_p6;
reg   [15:0] ap_phi_mux_app_data_1_2_phi_fu_282_p6;
reg   [511:0] ap_phi_mux_app_data_0_2_phi_fu_297_p6;
reg   [0:0] ap_phi_mux_tmp_eop_phi_fu_223_p4;
reg   [0:0] ap_phi_reg_pp0_iter1_tmp_eop_reg_220;
wire   [0:0] ap_phi_reg_pp0_iter0_tmp_eop_reg_220;
reg   [15:0] ap_phi_mux_tmp_len_phi_fu_234_p4;
reg   [15:0] ap_phi_reg_pp0_iter1_tmp_len_reg_231;
wire   [15:0] ap_phi_reg_pp0_iter0_tmp_len_reg_231;
reg   [511:0] ap_phi_mux_tmp_data_V_phi_fu_245_p4;
reg   [511:0] ap_phi_reg_pp0_iter1_tmp_data_V_reg_242;
wire   [511:0] ap_phi_reg_pp0_iter0_tmp_data_V_reg_242;
reg   [0:0] ap_phi_mux_valid_app_data_1_phi_fu_256_p4;
reg   [0:0] ap_phi_reg_pp0_iter1_valid_app_data_1_reg_253;
wire   [0:0] ap_phi_reg_pp0_iter0_valid_app_data_1_reg_253;
reg   [0:0] ap_phi_reg_pp0_iter1_app_data_2_2_reg_263;
wire   [0:0] full_n_nbwrite_fu_154_p7;
wire   [0:0] ap_phi_reg_pp0_iter0_app_data_2_2_reg_263;
reg   [15:0] ap_phi_reg_pp0_iter1_app_data_1_2_reg_278;
wire   [15:0] ap_phi_reg_pp0_iter0_app_data_1_2_reg_278;
reg   [511:0] ap_phi_reg_pp0_iter1_app_data_0_2_reg_293;
wire   [511:0] ap_phi_reg_pp0_iter0_app_data_0_2_reg_293;
reg   [0:0] ap_phi_reg_pp0_iter1_valid_app_data_3_reg_308;
wire   [0:0] ap_phi_reg_pp0_iter0_valid_app_data_3_reg_308;
reg    buf_app_input_data_V_len0_update;
wire   [0:0] empty_n_nbread_fu_144_p4_0;
reg    app_input_data_V_len1_update;
wire    ap_block_pp0_stage0_01001;
reg   [1:0] ap_NS_fsm;
reg    ap_idle_pp0;
wire    ap_enable_pp0;
reg    ap_condition_147;
reg    ap_condition_153;
reg    ap_condition_112;
reg    ap_condition_128;

// power-on initialization
initial begin
#0 ap_CS_fsm = 2'd1;
#0 ap_enable_reg_pp0_iter0 = 1'b0;
#0 ap_enable_reg_pp0_iter1 = 1'b0;
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_CS_fsm <= ap_ST_fsm_state1;
    end else begin
        ap_CS_fsm <= ap_NS_fsm;
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter0 <= 1'b0;
    end else begin
        if (((ap_start == 1'b1) & (1'b1 == ap_CS_fsm_state1))) begin
            ap_enable_reg_pp0_iter0 <= 1'b1;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter1 <= 1'b0;
    end else begin
        if ((1'b0 == ap_block_pp0_stage0_subdone)) begin
            ap_enable_reg_pp0_iter1 <= ap_enable_reg_pp0_iter0;
        end else if (((ap_start == 1'b1) & (1'b1 == ap_CS_fsm_state1))) begin
            ap_enable_reg_pp0_iter1 <= 1'b0;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_128)) begin
        if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (empty_n_2_fu_323_p1 == 1'd0))) begin
            ap_phi_reg_pp0_iter1_app_data_0_2_reg_293 <= buf_app_input_data_V_data_V_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_app_data_0_2_reg_293 <= ap_phi_reg_pp0_iter0_app_data_0_2_reg_293;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_128)) begin
        if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (empty_n_2_fu_323_p1 == 1'd0))) begin
            ap_phi_reg_pp0_iter1_app_data_1_2_reg_278 <= buf_app_input_data_V_len_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_app_data_1_2_reg_278 <= ap_phi_reg_pp0_iter0_app_data_1_2_reg_278;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_128)) begin
        if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (empty_n_2_fu_323_p1 == 1'd0))) begin
            ap_phi_reg_pp0_iter1_app_data_2_2_reg_263 <= buf_app_input_data_V_eop_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_app_data_2_2_reg_263 <= ap_phi_reg_pp0_iter0_app_data_2_2_reg_263;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_128)) begin
        if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (empty_n_2_fu_323_p1 == 1'd1))) begin
            ap_phi_reg_pp0_iter1_tmp_data_V_reg_242 <= buf_app_input_data_V_data_V_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_tmp_data_V_reg_242 <= ap_phi_reg_pp0_iter0_tmp_data_V_reg_242;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_128)) begin
        if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (empty_n_2_fu_323_p1 == 1'd1))) begin
            ap_phi_reg_pp0_iter1_tmp_eop_reg_220 <= buf_app_input_data_V_eop_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_tmp_eop_reg_220 <= ap_phi_reg_pp0_iter0_tmp_eop_reg_220;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_128)) begin
        if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (empty_n_2_fu_323_p1 == 1'd1))) begin
            ap_phi_reg_pp0_iter1_tmp_len_reg_231 <= buf_app_input_data_V_len_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_tmp_len_reg_231 <= ap_phi_reg_pp0_iter0_tmp_len_reg_231;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_128)) begin
        if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (empty_n_2_fu_323_p1 == 1'd1))) begin
            ap_phi_reg_pp0_iter1_valid_app_data_1_reg_253 <= empty_n_nbread_fu_144_p4_0;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_valid_app_data_1_reg_253 <= ap_phi_reg_pp0_iter0_valid_app_data_1_reg_253;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_128)) begin
        if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (empty_n_2_fu_323_p1 == 1'd0))) begin
            ap_phi_reg_pp0_iter1_valid_app_data_3_reg_308 <= empty_n_nbread_fu_144_p4_0;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_valid_app_data_3_reg_308 <= ap_phi_reg_pp0_iter0_valid_app_data_3_reg_308;
        end
    end
end

always @ (posedge ap_clk) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1))) begin
        valid_app_data_reg_175 <= ap_phi_mux_valid_app_data_3_phi_fu_312_p6;
    end else if (((ap_start == 1'b1) & (1'b1 == ap_CS_fsm_state1))) begin
        valid_app_data_reg_175 <= 1'd0;
    end
end

always @ (posedge ap_clk) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1))) begin
        app_data_0_s_reg_209 <= ap_phi_mux_app_data_0_2_phi_fu_297_p6;
        app_data_1_s_reg_198 <= ap_phi_mux_app_data_1_2_phi_fu_282_p6;
        app_data_2_s_reg_187 <= ap_phi_mux_app_data_2_2_phi_fu_267_p6;
    end
end

always @ (posedge ap_clk) begin
    if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        empty_n_2_reg_339 <= empty_n_nbread_fu_144_p4_0;
    end
end

always @ (*) begin
    if (((ap_start == 1'b0) & (1'b1 == ap_CS_fsm_state1))) begin
        ap_idle = 1'b1;
    end else begin
        ap_idle = 1'b0;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter0 == 1'b0) & (ap_enable_reg_pp0_iter1 == 1'b0))) begin
        ap_idle_pp0 = 1'b1;
    end else begin
        ap_idle_pp0 = 1'b0;
    end
end

always @ (*) begin
    if ((((1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0) & (((full_n_nbwrite_fu_154_p7 == 1'd0) & (empty_n_2_reg_339 == 1'd1)) | ((full_n_nbwrite_fu_154_p7 == 1'd0) & (valid_app_data_reg_175 == 1'd1)))) | ((1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0) & (((empty_n_2_reg_339 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1)) | ((valid_app_data_reg_175 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1)))))) begin
        ap_phi_mux_app_data_0_2_phi_fu_297_p6 = ap_phi_mux_tmp_data_V_phi_fu_245_p4;
    end else begin
        ap_phi_mux_app_data_0_2_phi_fu_297_p6 = ap_phi_reg_pp0_iter1_app_data_0_2_reg_293;
    end
end

always @ (*) begin
    if ((((1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0) & (((full_n_nbwrite_fu_154_p7 == 1'd0) & (empty_n_2_reg_339 == 1'd1)) | ((full_n_nbwrite_fu_154_p7 == 1'd0) & (valid_app_data_reg_175 == 1'd1)))) | ((1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0) & (((empty_n_2_reg_339 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1)) | ((valid_app_data_reg_175 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1)))))) begin
        ap_phi_mux_app_data_1_2_phi_fu_282_p6 = ap_phi_mux_tmp_len_phi_fu_234_p4;
    end else begin
        ap_phi_mux_app_data_1_2_phi_fu_282_p6 = ap_phi_reg_pp0_iter1_app_data_1_2_reg_278;
    end
end

always @ (*) begin
    if ((((1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0) & (((full_n_nbwrite_fu_154_p7 == 1'd0) & (empty_n_2_reg_339 == 1'd1)) | ((full_n_nbwrite_fu_154_p7 == 1'd0) & (valid_app_data_reg_175 == 1'd1)))) | ((1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0) & (((empty_n_2_reg_339 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1)) | ((valid_app_data_reg_175 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1)))))) begin
        ap_phi_mux_app_data_2_2_phi_fu_267_p6 = ap_phi_mux_tmp_eop_phi_fu_223_p4;
    end else begin
        ap_phi_mux_app_data_2_2_phi_fu_267_p6 = ap_phi_reg_pp0_iter1_app_data_2_2_reg_263;
    end
end

always @ (*) begin
    if (((valid_app_data_reg_175 == 1'd1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0))) begin
        ap_phi_mux_tmp_data_V_phi_fu_245_p4 = app_data_0_s_reg_209;
    end else begin
        ap_phi_mux_tmp_data_V_phi_fu_245_p4 = ap_phi_reg_pp0_iter1_tmp_data_V_reg_242;
    end
end

always @ (*) begin
    if (((valid_app_data_reg_175 == 1'd1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0))) begin
        ap_phi_mux_tmp_eop_phi_fu_223_p4 = app_data_2_s_reg_187;
    end else begin
        ap_phi_mux_tmp_eop_phi_fu_223_p4 = ap_phi_reg_pp0_iter1_tmp_eop_reg_220;
    end
end

always @ (*) begin
    if (((valid_app_data_reg_175 == 1'd1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0))) begin
        ap_phi_mux_tmp_len_phi_fu_234_p4 = app_data_1_s_reg_198;
    end else begin
        ap_phi_mux_tmp_len_phi_fu_234_p4 = ap_phi_reg_pp0_iter1_tmp_len_reg_231;
    end
end

always @ (*) begin
    if (((valid_app_data_reg_175 == 1'd1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0))) begin
        ap_phi_mux_valid_app_data_1_phi_fu_256_p4 = valid_app_data_reg_175;
    end else begin
        ap_phi_mux_valid_app_data_1_phi_fu_256_p4 = ap_phi_reg_pp0_iter1_valid_app_data_1_reg_253;
    end
end

always @ (*) begin
    if ((1'b1 == ap_condition_112)) begin
        if ((1'b1 == ap_condition_153)) begin
            ap_phi_mux_valid_app_data_3_phi_fu_312_p6 = ap_phi_mux_valid_app_data_1_phi_fu_256_p4;
        end else if ((1'b1 == ap_condition_147)) begin
            ap_phi_mux_valid_app_data_3_phi_fu_312_p6 = 1'd0;
        end else begin
            ap_phi_mux_valid_app_data_3_phi_fu_312_p6 = ap_phi_reg_pp0_iter1_valid_app_data_3_reg_308;
        end
    end else begin
        ap_phi_mux_valid_app_data_3_phi_fu_312_p6 = ap_phi_reg_pp0_iter1_valid_app_data_3_reg_308;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0))) begin
        ap_phi_mux_valid_app_data_phi_fu_179_p4 = ap_phi_mux_valid_app_data_3_phi_fu_312_p6;
    end else begin
        ap_phi_mux_valid_app_data_phi_fu_179_p4 = valid_app_data_reg_175;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0) & ((app_input_data_V_len_full_n & app_input_data_V_eop_full_n & app_input_data_V_data_V_full_n) == 1'b1) & (ap_enable_reg_pp0_iter1 == 1'b1) & ((empty_n_2_reg_339 == 1'd1) | (valid_app_data_reg_175 == 1'd1)))) begin
        app_input_data_V_len1_update = 1'b1;
    end else begin
        app_input_data_V_len1_update = 1'b0;
    end
end

always @ (*) begin
    if (((ap_phi_mux_valid_app_data_phi_fu_179_p4 == 1'd0) & (1'b0 == ap_block_pp0_stage0_11001) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & ((buf_app_input_data_V_len_empty_n & buf_app_input_data_V_eop_empty_n & buf_app_input_data_V_data_V_empty_n) == 1'b1))) begin
        buf_app_input_data_V_len0_update = 1'b1;
    end else begin
        buf_app_input_data_V_len0_update = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0) & (buf_read_sig_app_input_data_V_full_n == 1'b1) & (ap_enable_reg_pp0_iter1 == 1'b1) & (((empty_n_2_reg_339 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1)) | ((valid_app_data_reg_175 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1))))) begin
        buf_read_sig_app_input_data_V_write = 1'b1;
    end else begin
        buf_read_sig_app_input_data_V_write = 1'b0;
    end
end

always @ (*) begin
    case (ap_CS_fsm)
        ap_ST_fsm_state1 : begin
            if (((ap_start == 1'b1) & (1'b1 == ap_CS_fsm_state1))) begin
                ap_NS_fsm = ap_ST_fsm_pp0_stage0;
            end else begin
                ap_NS_fsm = ap_ST_fsm_state1;
            end
        end
        ap_ST_fsm_pp0_stage0 : begin
            ap_NS_fsm = ap_ST_fsm_pp0_stage0;
        end
        default : begin
            ap_NS_fsm = 'bx;
        end
    endcase
end

assign ap_CS_fsm_pp0_stage0 = ap_CS_fsm[32'd1];

assign ap_CS_fsm_state1 = ap_CS_fsm[32'd0];

assign ap_block_pp0_stage0 = ~(1'b1 == 1'b1);

assign ap_block_pp0_stage0_01001 = ~(1'b1 == 1'b1);

assign ap_block_pp0_stage0_11001 = ~(1'b1 == 1'b1);

assign ap_block_pp0_stage0_subdone = ~(1'b1 == 1'b1);

assign ap_block_state2_pp0_stage0_iter0 = ~(1'b1 == 1'b1);

assign ap_block_state3_pp0_stage0_iter1 = ~(1'b1 == 1'b1);

always @ (*) begin
    ap_condition_112 = ((1'b1 == ap_CS_fsm_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b0 == ap_block_pp0_stage0));
end

always @ (*) begin
    ap_condition_128 = ((1'b0 == ap_block_pp0_stage0_11001) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0));
end

always @ (*) begin
    ap_condition_147 = (((empty_n_2_reg_339 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1)) | ((valid_app_data_reg_175 == 1'd1) & (full_n_nbwrite_fu_154_p7 == 1'd1)));
end

always @ (*) begin
    ap_condition_153 = (((full_n_nbwrite_fu_154_p7 == 1'd0) & (empty_n_2_reg_339 == 1'd1)) | ((full_n_nbwrite_fu_154_p7 == 1'd0) & (valid_app_data_reg_175 == 1'd1)));
end

assign ap_done = 1'b0;

assign ap_enable_pp0 = (ap_idle_pp0 ^ 1'b1);

assign ap_phi_reg_pp0_iter0_app_data_0_2_reg_293 = 'bx;

assign ap_phi_reg_pp0_iter0_app_data_1_2_reg_278 = 'bx;

assign ap_phi_reg_pp0_iter0_app_data_2_2_reg_263 = 'bx;

assign ap_phi_reg_pp0_iter0_tmp_data_V_reg_242 = 'bx;

assign ap_phi_reg_pp0_iter0_tmp_eop_reg_220 = 'bx;

assign ap_phi_reg_pp0_iter0_tmp_len_reg_231 = 'bx;

assign ap_phi_reg_pp0_iter0_valid_app_data_1_reg_253 = 'bx;

assign ap_phi_reg_pp0_iter0_valid_app_data_3_reg_308 = 'bx;

assign ap_ready = 1'b0;

assign app_input_data_V_data_V_din = ap_phi_mux_tmp_data_V_phi_fu_245_p4;

assign app_input_data_V_data_V_write = app_input_data_V_len1_update;

assign app_input_data_V_eop_din = ap_phi_mux_tmp_eop_phi_fu_223_p4;

assign app_input_data_V_eop_write = app_input_data_V_len1_update;

assign app_input_data_V_len_din = ap_phi_mux_tmp_len_phi_fu_234_p4;

assign app_input_data_V_len_write = app_input_data_V_len1_update;

assign buf_app_input_data_V_data_V_read = buf_app_input_data_V_len0_update;

assign buf_app_input_data_V_eop_read = buf_app_input_data_V_len0_update;

assign buf_app_input_data_V_len_read = buf_app_input_data_V_len0_update;

assign buf_read_sig_app_input_data_V_din = 1'd1;

assign empty_n_2_fu_323_p1 = empty_n_nbread_fu_144_p4_0;

assign empty_n_nbread_fu_144_p4_0 = (buf_app_input_data_V_len_empty_n & buf_app_input_data_V_eop_empty_n & buf_app_input_data_V_data_V_empty_n);

assign full_n_nbwrite_fu_154_p7 = (app_input_data_V_len_full_n & app_input_data_V_eop_full_n & app_input_data_V_data_V_full_n);

endmodule //buf_app_input_data_forwarder_0
