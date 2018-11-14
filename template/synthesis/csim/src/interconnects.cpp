#include <insider_itc.h>
#include <iostream>

#include "app_pt_0.cpp"
#include "app_pt_1.cpp"
#include "app_pt_2.cpp"

#include "insider_itc.h"

#include "buf_app_input_data_forwarder_0.cpp"
#include "buf_app_input_data_forwarder_1.cpp"
#include "buf_app_input_data_forwarder_2.cpp"
#include "command_handler.cpp"
#include "device_dram_read_req_multiplexer.cpp"
#include "device_dram_read_resp_multiplexer.cpp"
#include "dram_helper_app_0.cpp"
#include "dram_helper_app_1.cpp"
#include "dram_helper_app_2.cpp"
#include "dram_read_delay_unit.cpp"
#include "dram_read_req_multiplexer.cpp"
#include "dram_read_req_time_marker.cpp"
#include "dram_read_resp_multiplexer.cpp"
#include "dram_read_throttle_unit.cpp"
#include "dram_write_delay_unit.cpp"
#include "dram_write_req_time_marker.cpp"
#include "dram_write_throttle_unit.cpp"
#include "pcie_data_splitter_app_0.cpp"
#include "pcie_data_splitter_app_1.cpp"
#include "pcie_data_splitter_app_2.cpp"
#include "pcie_helper_app_0.cpp"
#include "pcie_helper_app_1.cpp"
#include "pcie_helper_app_2.cpp"
#include "pcie_read_resp_passer.cpp"
#include "pcie_read_throttle_unit.cpp"
#include "pcie_write_multiplexer.cpp"
#include "pcie_write_throttle_unit.cpp"
#include "pipe0_data_handler.cpp"
#include "pipe0_dram_dispatcher.cpp"
#include "pipe1_data_handler.cpp"
#include "pipe1_dram_dispatcher.cpp"
#include "pipe2_data_handler.cpp"
#include "pipeline_data_passer.cpp"
#include "reset_propaganda.cpp"

#define READ_BUF_SIZE (1024 * 1024 * 2)

using namespace std;

int main() {
  ST_Queue<Request> reqs_incoming(64);
  ST_Queue<Request> rd_reqs_pipe0_write(64);
  ST_Queue<Request> wr_reqs_pipe0_write(64);
  ST_Queue<Request> rd_reqs_pipe1_read(64);
  ST_Queue<Request> wr_reqs_pipe1_read(64);
  ST_Queue<Request> rd_reqs_pipe1_write(64);
  ST_Queue<Request> wr_reqs_pipe1_write(64);
  ST_Queue<Request> rd_reqs_pipe2_read(64);
  ST_Queue<Request> wr_reqs_pipe2_read(64);

  ST_Queue<Data> data_pipe2(64);
  ST_Queue<bool> rd_data_valid_pipe1_write(64);
  ST_Queue<bool> wr_data_valid_pipe1_write(64);
  ST_Queue<bool> rd_data_valid_pipe2_read(64);
  ST_Queue<bool> wr_data_valid_pipe2_read(64);
  ST_Queue<unsigned long long> rd_kbuf_addr_pipe0_write(64);
  ST_Queue<unsigned long long> rd_kbuf_addr_pipe2_read(64);
  ST_Queue<unsigned long long> wr_kbuf_addr_pipe0_write(64);
  ST_Queue<unsigned long long> wr_kbuf_addr_pipe2_read(64);

  ST_Queue<PCIe_Read_Resp> before_throttle_pcie_read_resp(4);
  ST_Queue<PCIe_Read_Resp> after_throttle_pcie_read_resp(4);

  ST_Queue<unsigned int> kbuf_addrs(16);
  ST_Queue<PCIe_Write_Req_Data> host_data_pcie_write_req_data(64);
  ST_Queue<PCIe_Write_Req_Apply> host_data_pcie_write_req_apply(64);
  ST_Queue<PCIe_Write_Req_Data> host_rdcmd_fin_pcie_write_req_data(4);
  ST_Queue<PCIe_Write_Req_Apply> host_rdcmd_fin_pcie_write_req_apply(64);
  ST_Queue<PCIe_Write_Req_Data> host_wrcmd_fin_pcie_write_req_data(4);
  ST_Queue<PCIe_Write_Req_Apply> host_wrcmd_fin_pcie_write_req_apply(64);
  ST_Queue<unsigned long long> wrcmd_kbuf_addrs(64);

  ST_Queue<unsigned int> dma_read_throttle_params(4);
  ST_Queue<unsigned int> dma_write_throttle_params(4);
  ST_Queue<PCIe_Write_Req_Data> before_throttle_pcie_write_req_data(64);
  ST_Queue<PCIe_Write_Req_Apply> before_throttle_pcie_write_req_apply(64);

  ST_Queue<Dram_Dispatcher_Write_Req> unified_dram_dispatcher_write_context(64);
  ST_Queue<Dram_Dispatcher_Read_Req> unified_dram_dispatcher_read_context(64);

  ST_Queue<Dram_Read_Req> host_dram_read_req(64);
  ST_Queue<Dram_Read_Resp> host_dram_read_resp(4);
  ST_Queue<Dram_Write_Req_Data> host_dram_write_req_data(4096);
  ST_Queue<Dram_Write_Req_Data> before_delay_dram_write_req_data(4);
  ST_Queue<Dram_Write_Req_Apply> host_dram_write_req_apply(64);
  ST_Queue<Dram_Write_Req_Apply_With_Time>
      before_delay_dram_write_req_apply_with_time(64);

  ST_Queue<Device_Dram_Context> device_dram_req_context(64);

  ST_Queue<Dram_Read_Req> device_dram_read_req(64);
  ST_Queue<Dram_Read_Resp> device_dram_read_resp(4);

  ST_Queue<Dram_Read_Req> device_dram_read_req_0(64);
  ST_Queue<Dram_Read_Resp> device_dram_read_resp_0(4);

  ST_Queue<Dram_Read_Req> device_dram_read_req_1(64);
  ST_Queue<Dram_Read_Resp> device_dram_read_resp_1(4);

  ST_Queue<Dram_Read_Req> device_dram_read_req_2(64);
  ST_Queue<Dram_Read_Resp> device_dram_read_resp_2(4);

  ST_Queue<Dram_Read_Req> before_delay_unified_dram_read_req(64);
  ST_Queue<Dram_Read_Req_With_Time>
      before_delay_unified_dram_read_req_with_time(64);
  ST_Queue<Dram_Read_Req> after_delay_unified_dram_read_req(64);
  ST_Queue<Dram_Read_Resp> before_throttle_unified_dram_read_resp(4);
  ST_Queue<Dram_Read_Resp> after_throttle_unified_dram_read_resp(4);
  ST_Queue<Dram_Write_Req_Data> before_throttle_unified_dram_write_req_data(4);
  ST_Queue<Dram_Write_Req_Apply> before_throttle_unified_dram_write_req_apply(
      64);
  ST_Queue<Dram_Write_Req_Data> after_throttle_unified_dram_write_req_data(4);
  ST_Queue<Dram_Write_Req_Apply> after_throttle_unified_dram_write_req_apply(
      64);
  ST_Queue<bool> dram_read_context(64);

  ST_Queue<PCIe_Write_Req_Data> device_pcie_write_req_data_0(64);
  ST_Queue<PCIe_Write_Req_Apply> device_pcie_write_req_apply_0(64);

  ST_Queue<PCIe_Write_Req_Data> device_pcie_write_req_data_1(64);
  ST_Queue<PCIe_Write_Req_Apply> device_pcie_write_req_apply_1(64);

  ST_Queue<PCIe_Write_Req_Data> device_pcie_write_req_data_2(64);
  ST_Queue<PCIe_Write_Req_Apply> device_pcie_write_req_apply_2(64);

  ST_Queue<unsigned int> drive_read_delay_cycle_cnts(4);
  ST_Queue<unsigned int> drive_write_delay_cycle_cnts(4);
  ST_Queue<unsigned int> drive_read_throttle_params(4);
  ST_Queue<unsigned int> drive_write_throttle_params(4);

  ST_Queue<unsigned int> app_file_infos_0(4);
  ST_Queue<unsigned int> app_input_params_0(4);
  ST_Queue<APP_Data> app_input_data_0(1);
  ST_Queue<APP_Data> buf_app_input_data_0(APP_DRAM_RESP_BUFFER_SIZE);
  ST_Queue<bool> buf_read_sig_app_input_data_0(APP_DRAM_RESP_BUFFER_SIZE);

  ST_Queue<unsigned int> app_file_infos_1(4);
  ST_Queue<unsigned int> app_input_params_1(4);
  ST_Queue<APP_Data> app_input_data_1(1);
  ST_Queue<APP_Data> buf_app_input_data_1(APP_DRAM_RESP_BUFFER_SIZE);
  ST_Queue<bool> buf_read_sig_app_input_data_1(APP_DRAM_RESP_BUFFER_SIZE);

  ST_Queue<unsigned int> app_file_infos_2(4);
  ST_Queue<unsigned int> app_input_params_2(4);
  ST_Queue<APP_Data> app_input_data_2(1);
  ST_Queue<APP_Data> buf_app_input_data_2(APP_DRAM_RESP_BUFFER_SIZE);
  ST_Queue<bool> buf_read_sig_app_input_data_2(APP_DRAM_RESP_BUFFER_SIZE);

  ST_Queue<unsigned int> app_buf_addrs_0(4);
  ST_Queue<bool> app_free_buf_0(4);
  ST_Queue<APP_Data> app_output_data_0(4);
  ST_Queue<APP_Data> app_output_data_splitted_0(4);
  ST_Queue<APP_Data_Meta> app_output_data_meta_0(4);

  ST_Queue<unsigned int> app_buf_addrs_1(4);
  ST_Queue<bool> app_free_buf_1(4);
  ST_Queue<APP_Data> app_output_data_1(4);
  ST_Queue<APP_Data> app_output_data_splitted_1(4);
  ST_Queue<APP_Data_Meta> app_output_data_meta_1(4);

  ST_Queue<unsigned int> app_buf_addrs_2(4);
  ST_Queue<bool> app_free_buf_2(4);
  ST_Queue<APP_Data> app_output_data_2(4);
  ST_Queue<APP_Data> app_output_data_splitted_2(4);
  ST_Queue<APP_Data_Meta> app_output_data_meta_2(4);

  ST_Queue<bool> reset_sigs_0(4);
  ST_Queue<bool> reset_sigs_1(4);
  ST_Queue<bool> reset_sigs_2(4);
  ST_Queue<bool> reset_dram_helper_app_0(4);
  ST_Queue<bool> reset_dram_helper_app_1(4);
  ST_Queue<bool> reset_dram_helper_app_2(4);
  ST_Queue<bool> reset_pcie_helper_app_0(4);
  ST_Queue<bool> reset_pcie_helper_app_1(4);
  ST_Queue<bool> reset_pcie_helper_app_2(4);
  ST_Queue<bool> reset_pcie_data_splitter_app_0(4);
  ST_Queue<bool> reset_pcie_data_splitter_app_1(4);
  ST_Queue<bool> reset_pcie_data_splitter_app_2(4);
  ST_Queue<bool> reset_buf_app_input_data_forwarder_0(4);
  ST_Queue<bool> reset_buf_app_input_data_forwarder_1(4);
  ST_Queue<bool> reset_buf_app_input_data_forwarder_2(4);

  ST_Queue<bool> reset_app_pt_0(4);
  ST_Queue<bool> reset_app_pt_1(4);
  ST_Queue<bool> reset_app_pt_2(4);

  ;

  std::thread t0(command_handler, std::ref(poke), std::ref(reqs_incoming),
                 std::ref(kbuf_addrs), std::ref(dma_read_throttle_params),
                 std::ref(dma_write_throttle_params),
                 std::ref(drive_read_delay_cycle_cnts),
                 std::ref(drive_write_delay_cycle_cnts),
                 std::ref(drive_read_throttle_params),
                 std::ref(drive_write_throttle_params),
                 std::ref(app_file_infos_0), std::ref(app_file_infos_1),
                 std::ref(app_file_infos_2), std::ref(app_buf_addrs_0),
                 std::ref(app_buf_addrs_1), std::ref(app_buf_addrs_2),
                 std::ref(app_free_buf_0), std::ref(app_free_buf_1),
                 std::ref(app_free_buf_2), std::ref(app_input_params_0),
                 std::ref(app_input_params_1), std::ref(app_input_params_2),
                 std::ref(reset_sigs_0), std::ref(reset_sigs_1),
                 std::ref(reset_sigs_2));
  std::thread t1(pcie_read_resp_passer, std::ref(pcie_read_resp),
                 std::ref(before_throttle_pcie_read_resp));
  std::thread t2(pcie_read_throttle_unit, std::ref(dma_read_throttle_params),
                 std::ref(before_throttle_pcie_read_resp),
                 std::ref(after_throttle_pcie_read_resp));
  std::thread t3(
      pipe0_data_handler, std::ref(kbuf_addrs), std::ref(reqs_incoming),
      std::ref(host_dram_read_req), std::ref(pcie_read_req),
      std::ref(rd_reqs_pipe0_write), std::ref(wr_reqs_pipe0_write),
      std::ref(rd_kbuf_addr_pipe0_write), std::ref(wr_kbuf_addr_pipe0_write));
  std::thread t4(pipe1_data_handler, std::ref(rd_reqs_pipe1_read),
                 std::ref(wr_reqs_pipe1_read), std::ref(host_dram_read_resp),
                 std::ref(after_throttle_pcie_read_resp),
                 std::ref(rd_reqs_pipe1_write), std::ref(wr_reqs_pipe1_write),
                 std::ref(rd_data_valid_pipe1_write),
                 std::ref(wr_data_valid_pipe1_write), std::ref(data_pipe2),
                 std::ref(host_dram_write_req_data));
  std::thread t5(
      pipe2_data_handler, std::ref(rd_reqs_pipe2_read),
      std::ref(wr_reqs_pipe2_read), std::ref(rd_data_valid_pipe2_read),
      std::ref(wr_data_valid_pipe2_read), std::ref(rd_kbuf_addr_pipe2_read),
      std::ref(wr_kbuf_addr_pipe2_read), std::ref(data_pipe2),
      std::ref(host_data_pcie_write_req_apply),
      std::ref(host_data_pcie_write_req_data),
      std::ref(host_rdcmd_fin_pcie_write_req_apply),
      std::ref(host_rdcmd_fin_pcie_write_req_data),
      std::ref(host_dram_write_req_apply), std::ref(wrcmd_kbuf_addrs));
  std::thread t6(
      pipeline_data_passer, std::ref(rd_reqs_pipe0_write),
      std::ref(wr_reqs_pipe0_write), std::ref(rd_reqs_pipe1_read),
      std::ref(wr_reqs_pipe1_read), std::ref(rd_reqs_pipe1_write),
      std::ref(wr_reqs_pipe1_write), std::ref(rd_reqs_pipe2_read),
      std::ref(wr_reqs_pipe2_read), std::ref(rd_data_valid_pipe1_write),
      std::ref(wr_data_valid_pipe1_write), std::ref(rd_data_valid_pipe2_read),
      std::ref(wr_data_valid_pipe2_read), std::ref(rd_kbuf_addr_pipe0_write),
      std::ref(wr_kbuf_addr_pipe0_write), std::ref(rd_kbuf_addr_pipe2_read),
      std::ref(wr_kbuf_addr_pipe2_read));
  std::thread t7(pcie_write_multiplexer,
                 std::ref(host_data_pcie_write_req_data),
                 std::ref(host_data_pcie_write_req_apply),
                 std::ref(before_throttle_pcie_write_req_data),
                 std::ref(before_throttle_pcie_write_req_apply),
                 std::ref(host_rdcmd_fin_pcie_write_req_data),
                 std::ref(host_rdcmd_fin_pcie_write_req_apply),
                 std::ref(host_wrcmd_fin_pcie_write_req_data),
                 std::ref(host_wrcmd_fin_pcie_write_req_apply),
                 std::ref(device_pcie_write_req_data_0),
                 std::ref(device_pcie_write_req_apply_0),
                 std::ref(device_pcie_write_req_data_1),
                 std::ref(device_pcie_write_req_apply_1),
                 std::ref(device_pcie_write_req_data_2),
                 std::ref(device_pcie_write_req_apply_2));
  std::thread t8(pcie_write_throttle_unit, std::ref(dma_write_throttle_params),
                 std::ref(before_throttle_pcie_write_req_data),
                 std::ref(before_throttle_pcie_write_req_apply),
                 std::ref(pcie_write_req_data), std::ref(pcie_write_req_apply));
  std::thread t9(dram_read_req_multiplexer, std::ref(host_dram_read_req),
                 std::ref(device_dram_read_req),
                 std::ref(before_delay_unified_dram_read_req),
                 std::ref(dram_read_context));
  std::thread t10(dram_read_resp_multiplexer, std::ref(host_dram_read_resp),
                  std::ref(device_dram_read_resp),
                  std::ref(after_throttle_unified_dram_read_resp),
                  std::ref(dram_read_context));
  std::thread t11(
      device_dram_read_req_multiplexer, std::ref(device_dram_req_context),
      std::ref(device_dram_read_req), std::ref(device_dram_read_req_0),
      std::ref(device_dram_read_req_1), std::ref(device_dram_read_req_2));
  std::thread t12(
      device_dram_read_resp_multiplexer, std::ref(device_dram_req_context),
      std::ref(device_dram_read_resp), std::ref(device_dram_read_resp_0),
      std::ref(device_dram_read_resp_1), std::ref(device_dram_read_resp_2));
  std::thread t13(dram_read_throttle_unit, std::ref(drive_read_throttle_params),
                  std::ref(before_throttle_unified_dram_read_resp),
                  std::ref(after_throttle_unified_dram_read_resp));
  std::thread t14(dram_write_req_time_marker,
                  std::ref(host_dram_write_req_data),
                  std::ref(host_dram_write_req_apply),
                  std::ref(before_delay_dram_write_req_data),
                  std::ref(before_delay_dram_write_req_apply_with_time));
  std::thread t15(dram_write_delay_unit, std::ref(drive_write_delay_cycle_cnts),
                  std::ref(before_delay_dram_write_req_data),
                  std::ref(before_delay_dram_write_req_apply_with_time),
                  std::ref(before_throttle_unified_dram_write_req_data),
                  std::ref(before_throttle_unified_dram_write_req_apply));
  std::thread t16(dram_write_throttle_unit,
                  std::ref(drive_write_throttle_params),
                  std::ref(before_throttle_unified_dram_write_req_data),
                  std::ref(before_throttle_unified_dram_write_req_apply),
                  std::ref(after_throttle_unified_dram_write_req_data),
                  std::ref(after_throttle_unified_dram_write_req_apply));
  std::thread t17(dram_read_req_time_marker,
                  std::ref(before_delay_unified_dram_read_req),
                  std::ref(before_delay_unified_dram_read_req_with_time));
  std::thread t18(dram_read_delay_unit, std::ref(drive_read_delay_cycle_cnts),
                  std::ref(before_delay_unified_dram_read_req_with_time),
                  std::ref(after_delay_unified_dram_read_req));
  std::thread t19(pipe0_dram_dispatcher,
                  std::ref(after_delay_unified_dram_read_req),
                  std::ref(after_throttle_unified_dram_write_req_apply),
                  std::ref(dramA_read_req), std::ref(dramA_write_req_apply),
                  std::ref(dramB_read_req), std::ref(dramB_write_req_apply),
                  std::ref(dramC_read_req), std::ref(dramC_write_req_apply),
                  std::ref(dramD_read_req), std::ref(dramD_write_req_apply),
                  std::ref(unified_dram_dispatcher_write_context),
                  std::ref(unified_dram_dispatcher_read_context));
  std::thread t20(pipe1_dram_dispatcher,
                  std::ref(before_throttle_unified_dram_read_resp),
                  std::ref(after_throttle_unified_dram_write_req_data),
                  std::ref(dramA_read_resp), std::ref(dramA_write_req_data),
                  std::ref(dramB_read_resp), std::ref(dramB_write_req_data),
                  std::ref(dramC_read_resp), std::ref(dramC_write_req_data),
                  std::ref(dramD_read_resp), std::ref(dramD_write_req_data),
                  std::ref(unified_dram_dispatcher_write_context),
                  std::ref(unified_dram_dispatcher_read_context),
                  std::ref(host_wrcmd_fin_pcie_write_req_data),
                  std::ref(host_wrcmd_fin_pcie_write_req_apply),
                  std::ref(wrcmd_kbuf_addrs));
  std::thread t21(
      dram_helper_app_0, std::ref(app_file_infos_0),
      std::ref(device_dram_read_req_0), std::ref(device_dram_read_resp_0),
      std::ref(reset_dram_helper_app_0), std::ref(buf_app_input_data_0),
      std::ref(buf_read_sig_app_input_data_0));
  std::thread t22(
      dram_helper_app_1, std::ref(app_file_infos_1),
      std::ref(device_dram_read_req_1), std::ref(device_dram_read_resp_1),
      std::ref(reset_dram_helper_app_1), std::ref(buf_app_input_data_1),
      std::ref(buf_read_sig_app_input_data_1));
  std::thread t23(
      dram_helper_app_2, std::ref(app_file_infos_2),
      std::ref(device_dram_read_req_2), std::ref(device_dram_read_resp_2),
      std::ref(reset_dram_helper_app_2), std::ref(buf_app_input_data_2),
      std::ref(buf_read_sig_app_input_data_2));
  std::thread t24(
      buf_app_input_data_forwarder_0, std::ref(buf_app_input_data_0),
      std::ref(buf_read_sig_app_input_data_0), std::ref(app_input_data_0),
      std::ref(reset_buf_app_input_data_forwarder_0));
  std::thread t25(
      buf_app_input_data_forwarder_1, std::ref(buf_app_input_data_1),
      std::ref(buf_read_sig_app_input_data_1), std::ref(app_input_data_1),
      std::ref(reset_buf_app_input_data_forwarder_1));
  std::thread t26(
      buf_app_input_data_forwarder_2, std::ref(buf_app_input_data_2),
      std::ref(buf_read_sig_app_input_data_2), std::ref(app_input_data_2),
      std::ref(reset_buf_app_input_data_forwarder_2));
  std::thread t27(pcie_helper_app_0, std::ref(app_buf_addrs_0),
                  std::ref(device_pcie_write_req_apply_0),
                  std::ref(device_pcie_write_req_data_0),
                  std::ref(app_output_data_splitted_0),
                  std::ref(app_output_data_meta_0), std::ref(app_free_buf_0),
                  std::ref(reset_pcie_helper_app_0));
  std::thread t28(pcie_data_splitter_app_0, std::ref(app_output_data_0),
                  std::ref(app_output_data_splitted_0),
                  std::ref(app_output_data_meta_0),
                  std::ref(reset_pcie_data_splitter_app_0));
  std::thread t29(pcie_helper_app_1, std::ref(app_buf_addrs_1),
                  std::ref(device_pcie_write_req_apply_1),
                  std::ref(device_pcie_write_req_data_1),
                  std::ref(app_output_data_splitted_1),
                  std::ref(app_output_data_meta_1), std::ref(app_free_buf_1),
                  std::ref(reset_pcie_helper_app_1));
  std::thread t30(pcie_data_splitter_app_1, std::ref(app_output_data_1),
                  std::ref(app_output_data_splitted_1),
                  std::ref(app_output_data_meta_1),
                  std::ref(reset_pcie_data_splitter_app_1));
  std::thread t31(pcie_helper_app_2, std::ref(app_buf_addrs_2),
                  std::ref(device_pcie_write_req_apply_2),
                  std::ref(device_pcie_write_req_data_2),
                  std::ref(app_output_data_splitted_2),
                  std::ref(app_output_data_meta_2), std::ref(app_free_buf_2),
                  std::ref(reset_pcie_helper_app_2));
  std::thread t32(pcie_data_splitter_app_2, std::ref(app_output_data_2),
                  std::ref(app_output_data_splitted_2),
                  std::ref(app_output_data_meta_2),
                  std::ref(reset_pcie_data_splitter_app_2));
  std::thread t33(
      reset_propaganda, std::ref(reset_sigs_0), std::ref(reset_sigs_1),
      std::ref(reset_sigs_2), std::ref(reset_app_pt_0),
      std::ref(reset_app_pt_1), std::ref(reset_app_pt_2),
      std::ref(reset_dram_helper_app_0), std::ref(reset_dram_helper_app_1),
      std::ref(reset_dram_helper_app_2), std::ref(reset_pcie_helper_app_0),
      std::ref(reset_pcie_helper_app_1), std::ref(reset_pcie_helper_app_2),
      std::ref(reset_pcie_data_splitter_app_0),
      std::ref(reset_pcie_data_splitter_app_1),
      std::ref(reset_pcie_data_splitter_app_2),
      std::ref(reset_buf_app_input_data_forwarder_0),
      std::ref(reset_buf_app_input_data_forwarder_1),
      std::ref(reset_buf_app_input_data_forwarder_2));
  std::thread t34(app_pt_0, std::ref(reset_app_pt_0),
                  std::ref(app_input_data_0), std::ref(app_output_data_0),
                  std::ref(app_input_params_0));
  std::thread t35(app_pt_1, std::ref(reset_app_pt_1),
                  std::ref(app_input_data_1), std::ref(app_output_data_1),
                  std::ref(app_input_params_1));
  std::thread t36(app_pt_2, std::ref(reset_app_pt_2),
                  std::ref(app_input_data_2), std::ref(app_output_data_2),
                  std::ref(app_input_params_2));
  std::thread t37(host_pcie_simulator);
  std::thread t38(fpga_dramA_simulator);
  std::thread t39(fpga_dramB_simulator);
  std::thread t40(fpga_dramC_simulator);
  std::thread t41(fpga_dramD_simulator);

  simulator();

  while (1)
    ;
}

unsigned char app_input_data_0[328000];
unsigned char app_input_data_1[168000];
unsigned char app_input_data_2[245000];

char random_char() {
  int min_ascii = 65;
  int max_ascii = 90;
  return min_ascii + rand() % (max_ascii - min_ascii + 1);
}

void gen_data() {
  int i;
  for (i = 0; i < sizeof(app_input_data_0); i++) {
    app_input_data_0[i] = random_char();
  }
  for (i = 0; i < sizeof(app_input_data_1); i++) {
    app_input_data_1[i] = random_char();
  }
  for (i = 0; i < sizeof(app_input_data_2); i++) {
    app_input_data_2[i] = random_char();
  }
}

void sub_simulation_function(int app_id) {
  unsigned char *ptr_app_input_data;
  int size_app_input_data;
  if (app_id == 0) {
    ptr_app_input_data = app_input_data_0;
    size_app_input_data = sizeof(app_input_data_0);
  } else if (app_id == 1) {
    ptr_app_input_data = app_input_data_1;
    size_app_input_data = sizeof(app_input_data_1);
  } else if (app_id == 2) {
    ptr_app_input_data = app_input_data_2;
    size_app_input_data = sizeof(app_input_data_2);
  }

  set_physical_file(app_id, ptr_app_input_data, size_app_input_data);

  int fd = iopen(app_id, "csim_phy_file", 0);
  unsigned char *buf = (unsigned char *)malloc(READ_BUF_SIZE);
  bool fin_file = false;

  int total_read_bytes = 0;
  while (!fin_file) {
    int read_bytes = 0;
    while (read_bytes != READ_BUF_SIZE) {
      int tmp = iread(fd, buf, READ_BUF_SIZE - read_bytes);
      if (!tmp) {
        fin_file = true;
        break;
      } else {
        read_bytes += tmp;
      }
    }
    total_read_bytes += read_bytes;
  }

  int expected_total_read_bytes = size_app_input_data % (DATA_BUS_WIDTH);
  if (expected_total_read_bytes == 0) {
    expected_total_read_bytes = DATA_BUS_WIDTH;
  }
  if (total_read_bytes != expected_total_read_bytes) {
    cout << "Failed." << endl;
    cout << "app_id = " << app_id << ", total_read_bytes = " << total_read_bytes
         << ", expected_total_read_bytes = " << expected_total_read_bytes
         << endl;
    exit(-1);
  }
  for (int i = 0; i < total_read_bytes; i++) {
    int expected_data =
        ptr_app_input_data[size_app_input_data - total_read_bytes + i];
    if (buf[i] != expected_data) {
      cout << "idx = " << i << "real_data = " << (int)buf[i]
           << "expected_data = " << expected_data << endl;
      exit(-1);
    }
  }

  iclose(fd);
}

void user_simulation_function() {
  // PUT YOUR CODE HERE
  gen_data();
  std::thread t0(sub_simulation_function, 0);
  std::thread t1(sub_simulation_function, 1);
  std::thread t2(sub_simulation_function, 2);

  t0.join();
  t1.join();
  t2.join();

  sleep(1);
  puts("TEST PASSED.");
}
