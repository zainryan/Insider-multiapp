// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2017.4.op
// Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

(* CORE_GENERATION_INFO="device_dram_read_resp_multiplexer,hls_ip_2017_4_op,{HLS_INPUT_TYPE=cxx,HLS_INPUT_FLOAT=0,HLS_INPUT_FIXED=0,HLS_INPUT_PART=xcvu9p-flgb2104-2-i,HLS_INPUT_CLOCK=4.000000,HLS_INPUT_ARCH=others,HLS_SYN_CLOCK=2.839250,HLS_SYN_LAT=-1,HLS_SYN_TPT=none,HLS_SYN_MEM=0,HLS_SYN_DSP=0,HLS_SYN_FF=592,HLS_SYN_LUT=455}" *)

module device_dram_read_resp_multiplexer (
        ap_clk,
        ap_rst,
        ap_start,
        ap_done,
ap_continue,
        ap_idle,
        ap_ready,
        device_dram_req_context_V_id_dout,
        device_dram_req_context_V_id_empty_n,
        device_dram_req_context_V_id_read,
        device_dram_req_context_V_num_dout,
        device_dram_req_context_V_num_empty_n,
        device_dram_req_context_V_num_read,
        device_dram_read_resp_V_last_dout,
        device_dram_read_resp_V_last_empty_n,
        device_dram_read_resp_V_last_read,
        device_dram_read_resp_V_data_V_dout,
        device_dram_read_resp_V_data_V_empty_n,
        device_dram_read_resp_V_data_V_read,
        device_dram_read_resp_0_V_last_din,
        device_dram_read_resp_0_V_last_full_n,
        device_dram_read_resp_0_V_last_write,
        device_dram_read_resp_0_V_data_V_din,
        device_dram_read_resp_0_V_data_V_full_n,
        device_dram_read_resp_0_V_data_V_write,
        device_dram_read_resp_1_V_last_din,
        device_dram_read_resp_1_V_last_full_n,
        device_dram_read_resp_1_V_last_write,
        device_dram_read_resp_1_V_data_V_din,
        device_dram_read_resp_1_V_data_V_full_n,
        device_dram_read_resp_1_V_data_V_write,
        device_dram_read_resp_2_V_last_din,
        device_dram_read_resp_2_V_last_full_n,
        device_dram_read_resp_2_V_last_write,
        device_dram_read_resp_2_V_data_V_din,
        device_dram_read_resp_2_V_data_V_full_n,
        device_dram_read_resp_2_V_data_V_write
);

parameter    ap_ST_fsm_state1 = 2'd1;
parameter    ap_ST_fsm_pp0_stage0 = 2'd2;

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output ap_ready; output ap_continue;
input  [7:0] device_dram_req_context_V_id_dout;
input   device_dram_req_context_V_id_empty_n;
output   device_dram_req_context_V_id_read;
input  [7:0] device_dram_req_context_V_num_dout;
input   device_dram_req_context_V_num_empty_n;
output   device_dram_req_context_V_num_read;
input   device_dram_read_resp_V_last_dout;
input   device_dram_read_resp_V_last_empty_n;
output   device_dram_read_resp_V_last_read;
input  [511:0] device_dram_read_resp_V_data_V_dout;
input   device_dram_read_resp_V_data_V_empty_n;
output   device_dram_read_resp_V_data_V_read;
output   device_dram_read_resp_0_V_last_din;
input   device_dram_read_resp_0_V_last_full_n;
output   device_dram_read_resp_0_V_last_write;
output  [511:0] device_dram_read_resp_0_V_data_V_din;
input   device_dram_read_resp_0_V_data_V_full_n;
output   device_dram_read_resp_0_V_data_V_write;
output   device_dram_read_resp_1_V_last_din;
input   device_dram_read_resp_1_V_last_full_n;
output   device_dram_read_resp_1_V_last_write;
output  [511:0] device_dram_read_resp_1_V_data_V_din;
input   device_dram_read_resp_1_V_data_V_full_n;
output   device_dram_read_resp_1_V_data_V_write;
output   device_dram_read_resp_2_V_last_din;
input   device_dram_read_resp_2_V_last_full_n;
output   device_dram_read_resp_2_V_last_write;
output  [511:0] device_dram_read_resp_2_V_data_V_din;
input   device_dram_read_resp_2_V_data_V_full_n;
output   device_dram_read_resp_2_V_data_V_write;

reg ap_idle;

(* fsm_encoding = "none" *) reg   [1:0] ap_CS_fsm;
wire    ap_CS_fsm_state1;
reg    device_dram_read_resp_0_V_last_blk_n;
reg    ap_enable_reg_pp0_iter2;
wire    ap_block_pp0_stage0;
reg   [0:0] valid_inflight_req_reg_234;
reg   [0:0] ap_reg_pp0_iter1_valid_inflight_req_reg_234;
reg   [0:0] empty_n_3_reg_421;
reg   [0:0] tmp_2_reg_439;
reg   [0:0] empty_n_4_reg_403;
reg   [0:0] ap_reg_pp0_iter1_empty_n_4_reg_403;
reg    device_dram_read_resp_0_V_data_V_blk_n;
reg    device_dram_read_resp_1_V_last_blk_n;
reg   [0:0] tmp_4_reg_443;
reg    device_dram_read_resp_1_V_data_V_blk_n;
reg    device_dram_read_resp_2_V_last_blk_n;
reg   [0:0] tmp_6_reg_447;
reg    device_dram_read_resp_2_V_data_V_blk_n;
wire    ap_CS_fsm_pp0_stage0;
wire    ap_block_state2_pp0_stage0_iter0;
wire    ap_block_state3_pp0_stage0_iter1;
wire    device_dram_read_resp_2_V_last1_status;
reg    ap_predicate_op59_write_state4;
wire    device_dram_read_resp_1_V_last1_status;
reg    ap_predicate_op62_write_state4;
wire    device_dram_read_resp_0_V_last1_status;
reg    ap_predicate_op65_write_state4;
reg    ap_block_state4_pp0_stage0_iter2;
reg    ap_block_pp0_stage0_11001;
reg   [7:0] p_0_1_reg_246;
reg   [7:0] p_0_0_s_reg_257;
reg   [7:0] p_0_1_3_reg_298;
reg   [7:0] p_0_0_2_reg_312;
wire   [0:0] empty_n_4_fu_341_p1;
reg   [0:0] ap_phi_mux_valid_inflight_req_phi_fu_238_p4;
wire   [0:0] empty_n_3_fu_353_p1;
reg   [0:0] tmp_last_reg_425;
reg   [511:0] tmp_data_V_reg_432;
wire   [0:0] tmp_2_fu_365_p2;
wire   [0:0] tmp_4_fu_371_p2;
wire   [0:0] tmp_6_fu_377_p2;
reg    ap_enable_reg_pp0_iter0;
reg    ap_enable_reg_pp0_iter1;
reg    ap_block_pp0_stage0_subdone;
reg   [0:0] ap_phi_mux_valid_inflight_req_4_phi_fu_331_p6;
reg   [7:0] ap_phi_mux_p_0_1_phi_fu_250_p4;
reg   [7:0] ap_phi_mux_p_0_0_s_phi_fu_261_p4;
reg   [7:0] ap_phi_mux_p_0_1_1_phi_fu_271_p4;
reg   [7:0] ap_phi_reg_pp0_iter1_p_0_1_1_reg_268;
wire   [7:0] ap_phi_reg_pp0_iter0_p_0_1_1_reg_268;
reg   [7:0] ap_phi_mux_p_0_0_1_phi_fu_281_p4;
reg   [7:0] ap_phi_reg_pp0_iter1_p_0_0_1_reg_278;
wire   [7:0] ap_phi_reg_pp0_iter0_p_0_0_1_reg_278;
reg   [0:0] ap_phi_mux_valid_inflight_req_1_phi_fu_291_p4;
reg   [0:0] ap_phi_reg_pp0_iter1_valid_inflight_req_1_reg_288;
wire   [0:0] ap_phi_reg_pp0_iter0_valid_inflight_req_1_reg_288;
wire   [7:0] inflight_req_num_1_fu_383_p2;
reg   [7:0] ap_phi_reg_pp0_iter1_p_0_1_3_reg_298;
wire   [7:0] ap_phi_reg_pp0_iter0_p_0_1_3_reg_298;
reg   [7:0] ap_phi_reg_pp0_iter1_p_0_0_2_reg_312;
wire   [7:0] ap_phi_reg_pp0_iter0_p_0_0_2_reg_312;
wire   [0:0] p_valid_inflight_req_1_fu_396_p2;
reg   [0:0] ap_phi_reg_pp0_iter1_valid_inflight_req_4_reg_327;
wire   [0:0] ap_phi_reg_pp0_iter0_valid_inflight_req_4_reg_327;
reg    device_dram_req_context_V_id0_update;
wire   [0:0] empty_n_2_nbread_fu_188_p3_0;
reg    device_dram_read_resp_V_last0_update;
wire   [0:0] empty_n_nbread_fu_196_p3_0;
reg    device_dram_read_resp_2_V_last1_update;
reg    ap_block_pp0_stage0_01001;
reg    device_dram_read_resp_1_V_last1_update;
reg    device_dram_read_resp_0_V_last1_update;
wire   [0:0] not_s_fu_390_p2;
reg   [1:0] ap_NS_fsm;
reg    ap_idle_pp0;
wire    ap_enable_pp0;
reg    ap_condition_180;
reg    ap_condition_238;
reg    ap_condition_205;
reg    ap_condition_216;
reg    ap_condition_204;

// power-on initialization
initial begin
#0 ap_CS_fsm = 2'd1;
#0 ap_enable_reg_pp0_iter2 = 1'b0;
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
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter2 <= 1'b0;
    end else begin
        if ((1'b0 == ap_block_pp0_stage0_subdone)) begin
            ap_enable_reg_pp0_iter2 <= ap_enable_reg_pp0_iter1;
        end else if (((ap_start == 1'b1) & (1'b1 == ap_CS_fsm_state1))) begin
            ap_enable_reg_pp0_iter2 <= 1'b0;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_216)) begin
        if (((ap_phi_mux_valid_inflight_req_phi_fu_238_p4 == 1'd0) & (empty_n_4_fu_341_p1 == 1'd1))) begin
            ap_phi_reg_pp0_iter1_p_0_0_1_reg_278 <= device_dram_req_context_V_id_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_p_0_0_1_reg_278 <= ap_phi_reg_pp0_iter0_p_0_0_1_reg_278;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_216)) begin
        if (((ap_phi_mux_valid_inflight_req_phi_fu_238_p4 == 1'd0) & (empty_n_4_fu_341_p1 == 1'd0))) begin
            ap_phi_reg_pp0_iter1_p_0_0_2_reg_312 <= device_dram_req_context_V_id_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_p_0_0_2_reg_312 <= ap_phi_reg_pp0_iter0_p_0_0_2_reg_312;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_216)) begin
        if (((ap_phi_mux_valid_inflight_req_phi_fu_238_p4 == 1'd0) & (empty_n_4_fu_341_p1 == 1'd1))) begin
            ap_phi_reg_pp0_iter1_p_0_1_1_reg_268 <= device_dram_req_context_V_num_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_p_0_1_1_reg_268 <= ap_phi_reg_pp0_iter0_p_0_1_1_reg_268;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_216)) begin
        if (((ap_phi_mux_valid_inflight_req_phi_fu_238_p4 == 1'd0) & (empty_n_4_fu_341_p1 == 1'd0))) begin
            ap_phi_reg_pp0_iter1_p_0_1_3_reg_298 <= device_dram_req_context_V_num_dout;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_p_0_1_3_reg_298 <= ap_phi_reg_pp0_iter0_p_0_1_3_reg_298;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_216)) begin
        if (((ap_phi_mux_valid_inflight_req_phi_fu_238_p4 == 1'd0) & (empty_n_4_fu_341_p1 == 1'd1))) begin
            ap_phi_reg_pp0_iter1_valid_inflight_req_1_reg_288 <= empty_n_2_nbread_fu_188_p3_0;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_valid_inflight_req_1_reg_288 <= ap_phi_reg_pp0_iter0_valid_inflight_req_1_reg_288;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_216)) begin
        if (((ap_phi_mux_valid_inflight_req_phi_fu_238_p4 == 1'd0) & (empty_n_4_fu_341_p1 == 1'd0))) begin
            ap_phi_reg_pp0_iter1_valid_inflight_req_4_reg_327 <= empty_n_2_nbread_fu_188_p3_0;
        end else if ((1'b1 == 1'b1)) begin
            ap_phi_reg_pp0_iter1_valid_inflight_req_4_reg_327 <= ap_phi_reg_pp0_iter0_valid_inflight_req_4_reg_327;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001) & (((empty_n_3_fu_353_p1 == 1'd0) & (empty_n_4_reg_403 == 1'd1)) | ((empty_n_3_fu_353_p1 == 1'd0) & (valid_inflight_req_reg_234 == 1'd1)))) | ((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001) & (((empty_n_4_reg_403 == 1'd1) & (empty_n_3_fu_353_p1 == 1'd1)) | ((empty_n_3_fu_353_p1 == 1'd1) & (valid_inflight_req_reg_234 == 1'd1)))))) begin
        p_0_0_2_reg_312 <= ap_phi_mux_p_0_0_1_phi_fu_281_p4;
    end else if (((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        p_0_0_2_reg_312 <= ap_phi_reg_pp0_iter1_p_0_0_2_reg_312;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_condition_204)) begin
        if ((1'b1 == ap_condition_238)) begin
            p_0_1_3_reg_298 <= ap_phi_mux_p_0_1_1_phi_fu_271_p4;
        end else if ((1'b1 == ap_condition_180)) begin
            p_0_1_3_reg_298 <= inflight_req_num_1_fu_383_p2;
        end else if ((1'b1 == 1'b1)) begin
            p_0_1_3_reg_298 <= ap_phi_reg_pp0_iter1_p_0_1_3_reg_298;
        end
    end
end

always @ (posedge ap_clk) begin
    if (((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        valid_inflight_req_reg_234 <= ap_phi_mux_valid_inflight_req_4_phi_fu_331_p6;
    end else if (((ap_start == 1'b1) & (1'b1 == ap_CS_fsm_state1))) begin
        valid_inflight_req_reg_234 <= 1'd0;
    end
end

always @ (posedge ap_clk) begin
    if (((1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        ap_reg_pp0_iter1_empty_n_4_reg_403 <= empty_n_4_reg_403;
        ap_reg_pp0_iter1_valid_inflight_req_reg_234 <= valid_inflight_req_reg_234;
    end
end

always @ (posedge ap_clk) begin
    if (((1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001) & ((empty_n_4_reg_403 == 1'd1) | (valid_inflight_req_reg_234 == 1'd1)))) begin
        empty_n_3_reg_421 <= empty_n_nbread_fu_196_p3_0;
        tmp_data_V_reg_432 <= device_dram_read_resp_V_data_V_dout;
        tmp_last_reg_425 <= device_dram_read_resp_V_last_dout;
    end
end

always @ (posedge ap_clk) begin
    if (((ap_phi_mux_valid_inflight_req_phi_fu_238_p4 == 1'd0) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        empty_n_4_reg_403 <= empty_n_2_nbread_fu_188_p3_0;
    end
end

always @ (posedge ap_clk) begin
    if (((ap_enable_reg_pp0_iter2 == 1'b1) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        p_0_0_s_reg_257 <= p_0_0_2_reg_312;
        p_0_1_reg_246 <= p_0_1_3_reg_298;
    end
end

always @ (posedge ap_clk) begin
    if (((1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001) & (((empty_n_4_reg_403 == 1'd1) & (empty_n_3_fu_353_p1 == 1'd1)) | ((empty_n_3_fu_353_p1 == 1'd1) & (valid_inflight_req_reg_234 == 1'd1))))) begin
        tmp_2_reg_439 <= tmp_2_fu_365_p2;
    end
end

always @ (posedge ap_clk) begin
    if (((1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001) & (((tmp_2_fu_365_p2 == 1'd0) & (empty_n_4_reg_403 == 1'd1) & (empty_n_3_fu_353_p1 == 1'd1)) | ((tmp_2_fu_365_p2 == 1'd0) & (empty_n_3_fu_353_p1 == 1'd1) & (valid_inflight_req_reg_234 == 1'd1))))) begin
        tmp_4_reg_443 <= tmp_4_fu_371_p2;
    end
end

always @ (posedge ap_clk) begin
    if (((1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001) & (((tmp_4_fu_371_p2 == 1'd0) & (tmp_2_fu_365_p2 == 1'd0) & (empty_n_4_reg_403 == 1'd1) & (empty_n_3_fu_353_p1 == 1'd1)) | ((tmp_4_fu_371_p2 == 1'd0) & (tmp_2_fu_365_p2 == 1'd0) & (empty_n_3_fu_353_p1 == 1'd1) & (valid_inflight_req_reg_234 == 1'd1))))) begin
        tmp_6_reg_447 <= tmp_6_fu_377_p2;
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
    if (((ap_enable_reg_pp0_iter2 == 1'b0) & (ap_enable_reg_pp0_iter1 == 1'b0) & (ap_enable_reg_pp0_iter0 == 1'b0))) begin
        ap_idle_pp0 = 1'b1;
    end else begin
        ap_idle_pp0 = 1'b0;
    end
end

always @ (*) begin
    if (((valid_inflight_req_reg_234 == 1'd1) & (1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_phi_mux_p_0_0_1_phi_fu_281_p4 = ap_phi_mux_p_0_0_s_phi_fu_261_p4;
    end else begin
        ap_phi_mux_p_0_0_1_phi_fu_281_p4 = ap_phi_reg_pp0_iter1_p_0_0_1_reg_278;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter2 == 1'b1))) begin
        ap_phi_mux_p_0_0_s_phi_fu_261_p4 = p_0_0_2_reg_312;
    end else begin
        ap_phi_mux_p_0_0_s_phi_fu_261_p4 = p_0_0_s_reg_257;
    end
end

always @ (*) begin
    if (((valid_inflight_req_reg_234 == 1'd1) & (1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_phi_mux_p_0_1_1_phi_fu_271_p4 = ap_phi_mux_p_0_1_phi_fu_250_p4;
    end else begin
        ap_phi_mux_p_0_1_1_phi_fu_271_p4 = ap_phi_reg_pp0_iter1_p_0_1_1_reg_268;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter2 == 1'b1))) begin
        ap_phi_mux_p_0_1_phi_fu_250_p4 = p_0_1_3_reg_298;
    end else begin
        ap_phi_mux_p_0_1_phi_fu_250_p4 = p_0_1_reg_246;
    end
end

always @ (*) begin
    if (((valid_inflight_req_reg_234 == 1'd1) & (1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_phi_mux_valid_inflight_req_1_phi_fu_291_p4 = valid_inflight_req_reg_234;
    end else begin
        ap_phi_mux_valid_inflight_req_1_phi_fu_291_p4 = ap_phi_reg_pp0_iter1_valid_inflight_req_1_reg_288;
    end
end

always @ (*) begin
    if ((1'b1 == ap_condition_205)) begin
        if ((1'b1 == ap_condition_238)) begin
            ap_phi_mux_valid_inflight_req_4_phi_fu_331_p6 = ap_phi_mux_valid_inflight_req_1_phi_fu_291_p4;
        end else if ((1'b1 == ap_condition_180)) begin
            ap_phi_mux_valid_inflight_req_4_phi_fu_331_p6 = p_valid_inflight_req_1_fu_396_p2;
        end else begin
            ap_phi_mux_valid_inflight_req_4_phi_fu_331_p6 = ap_phi_reg_pp0_iter1_valid_inflight_req_4_reg_327;
        end
    end else begin
        ap_phi_mux_valid_inflight_req_4_phi_fu_331_p6 = ap_phi_reg_pp0_iter1_valid_inflight_req_4_reg_327;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_phi_mux_valid_inflight_req_phi_fu_238_p4 = ap_phi_mux_valid_inflight_req_4_phi_fu_331_p6;
    end else begin
        ap_phi_mux_valid_inflight_req_phi_fu_238_p4 = valid_inflight_req_reg_234;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter2 == 1'b1) & (((ap_reg_pp0_iter1_empty_n_4_reg_403 == 1'd1) & (tmp_2_reg_439 == 1'd1) & (empty_n_3_reg_421 == 1'd1)) | ((tmp_2_reg_439 == 1'd1) & (empty_n_3_reg_421 == 1'd1) & (ap_reg_pp0_iter1_valid_inflight_req_reg_234 == 1'd1))))) begin
        device_dram_read_resp_0_V_data_V_blk_n = device_dram_read_resp_0_V_data_V_full_n;
    end else begin
        device_dram_read_resp_0_V_data_V_blk_n = 1'b1;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter2 == 1'b1) & (ap_predicate_op65_write_state4 == 1'b1) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        device_dram_read_resp_0_V_last1_update = 1'b1;
    end else begin
        device_dram_read_resp_0_V_last1_update = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter2 == 1'b1) & (((ap_reg_pp0_iter1_empty_n_4_reg_403 == 1'd1) & (tmp_2_reg_439 == 1'd1) & (empty_n_3_reg_421 == 1'd1)) | ((tmp_2_reg_439 == 1'd1) & (empty_n_3_reg_421 == 1'd1) & (ap_reg_pp0_iter1_valid_inflight_req_reg_234 == 1'd1))))) begin
        device_dram_read_resp_0_V_last_blk_n = device_dram_read_resp_0_V_last_full_n;
    end else begin
        device_dram_read_resp_0_V_last_blk_n = 1'b1;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter2 == 1'b1) & (((tmp_2_reg_439 == 1'd0) & (tmp_4_reg_443 == 1'd1) & (ap_reg_pp0_iter1_empty_n_4_reg_403 == 1'd1) & (empty_n_3_reg_421 == 1'd1)) | ((tmp_2_reg_439 == 1'd0) & (tmp_4_reg_443 == 1'd1) & (empty_n_3_reg_421 == 1'd1) & (ap_reg_pp0_iter1_valid_inflight_req_reg_234 == 1'd1))))) begin
        device_dram_read_resp_1_V_data_V_blk_n = device_dram_read_resp_1_V_data_V_full_n;
    end else begin
        device_dram_read_resp_1_V_data_V_blk_n = 1'b1;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter2 == 1'b1) & (ap_predicate_op62_write_state4 == 1'b1) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        device_dram_read_resp_1_V_last1_update = 1'b1;
    end else begin
        device_dram_read_resp_1_V_last1_update = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter2 == 1'b1) & (((tmp_2_reg_439 == 1'd0) & (tmp_4_reg_443 == 1'd1) & (ap_reg_pp0_iter1_empty_n_4_reg_403 == 1'd1) & (empty_n_3_reg_421 == 1'd1)) | ((tmp_2_reg_439 == 1'd0) & (tmp_4_reg_443 == 1'd1) & (empty_n_3_reg_421 == 1'd1) & (ap_reg_pp0_iter1_valid_inflight_req_reg_234 == 1'd1))))) begin
        device_dram_read_resp_1_V_last_blk_n = device_dram_read_resp_1_V_last_full_n;
    end else begin
        device_dram_read_resp_1_V_last_blk_n = 1'b1;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter2 == 1'b1) & (((tmp_4_reg_443 == 1'd0) & (tmp_2_reg_439 == 1'd0) & (ap_reg_pp0_iter1_empty_n_4_reg_403 == 1'd1) & (empty_n_3_reg_421 == 1'd1) & (tmp_6_reg_447 == 1'd1)) | ((tmp_4_reg_443 == 1'd0) & (tmp_2_reg_439 == 1'd0) & (empty_n_3_reg_421 == 1'd1) & (tmp_6_reg_447 == 1'd1) & (ap_reg_pp0_iter1_valid_inflight_req_reg_234 == 1'd1))))) begin
        device_dram_read_resp_2_V_data_V_blk_n = device_dram_read_resp_2_V_data_V_full_n;
    end else begin
        device_dram_read_resp_2_V_data_V_blk_n = 1'b1;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter2 == 1'b1) & (ap_predicate_op59_write_state4 == 1'b1) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        device_dram_read_resp_2_V_last1_update = 1'b1;
    end else begin
        device_dram_read_resp_2_V_last1_update = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter2 == 1'b1) & (((tmp_4_reg_443 == 1'd0) & (tmp_2_reg_439 == 1'd0) & (ap_reg_pp0_iter1_empty_n_4_reg_403 == 1'd1) & (empty_n_3_reg_421 == 1'd1) & (tmp_6_reg_447 == 1'd1)) | ((tmp_4_reg_443 == 1'd0) & (tmp_2_reg_439 == 1'd0) & (empty_n_3_reg_421 == 1'd1) & (tmp_6_reg_447 == 1'd1) & (ap_reg_pp0_iter1_valid_inflight_req_reg_234 == 1'd1))))) begin
        device_dram_read_resp_2_V_last_blk_n = device_dram_read_resp_2_V_last_full_n;
    end else begin
        device_dram_read_resp_2_V_last_blk_n = 1'b1;
    end
end

always @ (*) begin
    if ((((device_dram_read_resp_V_last_empty_n & device_dram_read_resp_V_data_V_empty_n) == 1'b1) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001) & ((empty_n_4_reg_403 == 1'd1) | (valid_inflight_req_reg_234 == 1'd1)))) begin
        device_dram_read_resp_V_last0_update = 1'b1;
    end else begin
        device_dram_read_resp_V_last0_update = 1'b0;
    end
end

always @ (*) begin
    if (((ap_phi_mux_valid_inflight_req_phi_fu_238_p4 == 1'd0) & ((device_dram_req_context_V_num_empty_n & device_dram_req_context_V_id_empty_n) == 1'b1) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        device_dram_req_context_V_id0_update = 1'b1;
    end else begin
        device_dram_req_context_V_id0_update = 1'b0;
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

always @ (*) begin
    ap_block_pp0_stage0_01001 = ((ap_enable_reg_pp0_iter2 == 1'b1) & (((device_dram_read_resp_0_V_last1_status == 1'b0) & (ap_predicate_op65_write_state4 == 1'b1)) | ((device_dram_read_resp_1_V_last1_status == 1'b0) & (ap_predicate_op62_write_state4 == 1'b1)) | ((device_dram_read_resp_2_V_last1_status == 1'b0) & (ap_predicate_op59_write_state4 == 1'b1))));
end

always @ (*) begin
    ap_block_pp0_stage0_11001 = ((ap_enable_reg_pp0_iter2 == 1'b1) & (((device_dram_read_resp_0_V_last1_status == 1'b0) & (ap_predicate_op65_write_state4 == 1'b1)) | ((device_dram_read_resp_1_V_last1_status == 1'b0) & (ap_predicate_op62_write_state4 == 1'b1)) | ((device_dram_read_resp_2_V_last1_status == 1'b0) & (ap_predicate_op59_write_state4 == 1'b1))));
end

always @ (*) begin
    ap_block_pp0_stage0_subdone = ((ap_enable_reg_pp0_iter2 == 1'b1) & (((device_dram_read_resp_0_V_last1_status == 1'b0) & (ap_predicate_op65_write_state4 == 1'b1)) | ((device_dram_read_resp_1_V_last1_status == 1'b0) & (ap_predicate_op62_write_state4 == 1'b1)) | ((device_dram_read_resp_2_V_last1_status == 1'b0) & (ap_predicate_op59_write_state4 == 1'b1))));
end

assign ap_block_state2_pp0_stage0_iter0 = ~(1'b1 == 1'b1);

assign ap_block_state3_pp0_stage0_iter1 = ~(1'b1 == 1'b1);

always @ (*) begin
    ap_block_state4_pp0_stage0_iter2 = (((device_dram_read_resp_0_V_last1_status == 1'b0) & (ap_predicate_op65_write_state4 == 1'b1)) | ((device_dram_read_resp_1_V_last1_status == 1'b0) & (ap_predicate_op62_write_state4 == 1'b1)) | ((device_dram_read_resp_2_V_last1_status == 1'b0) & (ap_predicate_op59_write_state4 == 1'b1)));
end

always @ (*) begin
    ap_condition_180 = (((empty_n_4_reg_403 == 1'd1) & (empty_n_3_fu_353_p1 == 1'd1)) | ((empty_n_3_fu_353_p1 == 1'd1) & (valid_inflight_req_reg_234 == 1'd1)));
end

always @ (*) begin
    ap_condition_204 = ((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001));
end

always @ (*) begin
    ap_condition_205 = ((1'b0 == ap_block_pp0_stage0) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0));
end

always @ (*) begin
    ap_condition_216 = ((ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001));
end

always @ (*) begin
    ap_condition_238 = (((empty_n_3_fu_353_p1 == 1'd0) & (empty_n_4_reg_403 == 1'd1)) | ((empty_n_3_fu_353_p1 == 1'd0) & (valid_inflight_req_reg_234 == 1'd1)));
end

assign ap_done = 1'b0;

assign ap_enable_pp0 = (ap_idle_pp0 ^ 1'b1);

assign ap_phi_reg_pp0_iter0_p_0_0_1_reg_278 = 'bx;

assign ap_phi_reg_pp0_iter0_p_0_0_2_reg_312 = 'bx;

assign ap_phi_reg_pp0_iter0_p_0_1_1_reg_268 = 'bx;

assign ap_phi_reg_pp0_iter0_p_0_1_3_reg_298 = 'bx;

assign ap_phi_reg_pp0_iter0_valid_inflight_req_1_reg_288 = 'bx;

assign ap_phi_reg_pp0_iter0_valid_inflight_req_4_reg_327 = 'bx;

always @ (*) begin
    ap_predicate_op59_write_state4 = (((tmp_4_reg_443 == 1'd0) & (tmp_2_reg_439 == 1'd0) & (ap_reg_pp0_iter1_empty_n_4_reg_403 == 1'd1) & (empty_n_3_reg_421 == 1'd1) & (tmp_6_reg_447 == 1'd1)) | ((tmp_4_reg_443 == 1'd0) & (tmp_2_reg_439 == 1'd0) & (empty_n_3_reg_421 == 1'd1) & (tmp_6_reg_447 == 1'd1) & (ap_reg_pp0_iter1_valid_inflight_req_reg_234 == 1'd1)));
end

always @ (*) begin
    ap_predicate_op62_write_state4 = (((tmp_2_reg_439 == 1'd0) & (tmp_4_reg_443 == 1'd1) & (ap_reg_pp0_iter1_empty_n_4_reg_403 == 1'd1) & (empty_n_3_reg_421 == 1'd1)) | ((tmp_2_reg_439 == 1'd0) & (tmp_4_reg_443 == 1'd1) & (empty_n_3_reg_421 == 1'd1) & (ap_reg_pp0_iter1_valid_inflight_req_reg_234 == 1'd1)));
end

always @ (*) begin
    ap_predicate_op65_write_state4 = (((ap_reg_pp0_iter1_empty_n_4_reg_403 == 1'd1) & (tmp_2_reg_439 == 1'd1) & (empty_n_3_reg_421 == 1'd1)) | ((tmp_2_reg_439 == 1'd1) & (empty_n_3_reg_421 == 1'd1) & (ap_reg_pp0_iter1_valid_inflight_req_reg_234 == 1'd1)));
end

assign ap_ready = 1'b0;

assign device_dram_read_resp_0_V_data_V_din = tmp_data_V_reg_432;

assign device_dram_read_resp_0_V_data_V_write = device_dram_read_resp_0_V_last1_update;

assign device_dram_read_resp_0_V_last1_status = (device_dram_read_resp_0_V_last_full_n & device_dram_read_resp_0_V_data_V_full_n);

assign device_dram_read_resp_0_V_last_din = tmp_last_reg_425;

assign device_dram_read_resp_0_V_last_write = device_dram_read_resp_0_V_last1_update;

assign device_dram_read_resp_1_V_data_V_din = tmp_data_V_reg_432;

assign device_dram_read_resp_1_V_data_V_write = device_dram_read_resp_1_V_last1_update;

assign device_dram_read_resp_1_V_last1_status = (device_dram_read_resp_1_V_last_full_n & device_dram_read_resp_1_V_data_V_full_n);

assign device_dram_read_resp_1_V_last_din = tmp_last_reg_425;

assign device_dram_read_resp_1_V_last_write = device_dram_read_resp_1_V_last1_update;

assign device_dram_read_resp_2_V_data_V_din = tmp_data_V_reg_432;

assign device_dram_read_resp_2_V_data_V_write = device_dram_read_resp_2_V_last1_update;

assign device_dram_read_resp_2_V_last1_status = (device_dram_read_resp_2_V_last_full_n & device_dram_read_resp_2_V_data_V_full_n);

assign device_dram_read_resp_2_V_last_din = tmp_last_reg_425;

assign device_dram_read_resp_2_V_last_write = device_dram_read_resp_2_V_last1_update;

assign device_dram_read_resp_V_data_V_read = device_dram_read_resp_V_last0_update;

assign device_dram_read_resp_V_last_read = device_dram_read_resp_V_last0_update;

assign device_dram_req_context_V_id_read = device_dram_req_context_V_id0_update;

assign device_dram_req_context_V_num_read = device_dram_req_context_V_id0_update;

assign empty_n_2_nbread_fu_188_p3_0 = (device_dram_req_context_V_num_empty_n & device_dram_req_context_V_id_empty_n);

assign empty_n_3_fu_353_p1 = empty_n_nbread_fu_196_p3_0;

assign empty_n_4_fu_341_p1 = empty_n_2_nbread_fu_188_p3_0;

assign empty_n_nbread_fu_196_p3_0 = (device_dram_read_resp_V_last_empty_n & device_dram_read_resp_V_data_V_empty_n);

assign inflight_req_num_1_fu_383_p2 = ($signed(ap_phi_mux_p_0_1_1_phi_fu_271_p4) + $signed(8'd255));

assign not_s_fu_390_p2 = ((inflight_req_num_1_fu_383_p2 != 8'd0) ? 1'b1 : 1'b0);

assign p_valid_inflight_req_1_fu_396_p2 = (not_s_fu_390_p2 & ap_phi_mux_valid_inflight_req_1_phi_fu_291_p4);

assign tmp_2_fu_365_p2 = ((ap_phi_mux_p_0_0_1_phi_fu_281_p4 == 8'd0) ? 1'b1 : 1'b0);

assign tmp_4_fu_371_p2 = ((ap_phi_mux_p_0_0_1_phi_fu_281_p4 == 8'd1) ? 1'b1 : 1'b0);

assign tmp_6_fu_377_p2 = ((ap_phi_mux_p_0_0_1_phi_fu_281_p4 == 8'd2) ? 1'b1 : 1'b0);

endmodule //device_dram_read_resp_multiplexer
