#include <insider_itc.h>

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

void interconnects() {
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

  command_handler(poke, reqs_incoming, kbuf_addrs, dma_read_throttle_params,
                  dma_write_throttle_params, drive_read_delay_cycle_cnts,
                  drive_write_delay_cycle_cnts, drive_read_throttle_params,
                  drive_write_throttle_params, app_file_infos_0,
                  app_file_infos_1, app_file_infos_2, app_buf_addrs_0,
                  app_buf_addrs_1, app_buf_addrs_2, app_free_buf_0,
                  app_free_buf_1, app_free_buf_2, app_input_params_0,
                  app_input_params_1, app_input_params_2, reset_sigs_0,
                  reset_sigs_1, reset_sigs_2);

  pcie_read_resp_passer(pcie_read_resp, before_throttle_pcie_read_resp);
  pcie_read_throttle_unit(dma_read_throttle_params,
                          before_throttle_pcie_read_resp,
                          after_throttle_pcie_read_resp);
  pipe0_data_handler(kbuf_addrs, reqs_incoming, host_dram_read_req,
                     pcie_read_req, rd_reqs_pipe0_write, wr_reqs_pipe0_write,
                     rd_kbuf_addr_pipe0_write, wr_kbuf_addr_pipe0_write);
  pipe1_data_handler(rd_reqs_pipe1_read, wr_reqs_pipe1_read,
                     host_dram_read_resp, after_throttle_pcie_read_resp,
                     rd_reqs_pipe1_write, wr_reqs_pipe1_write,
                     rd_data_valid_pipe1_write, wr_data_valid_pipe1_write,
                     data_pipe2, host_dram_write_req_data);
  pipe2_data_handler(
      rd_reqs_pipe2_read, wr_reqs_pipe2_read, rd_data_valid_pipe2_read,
      wr_data_valid_pipe2_read, rd_kbuf_addr_pipe2_read,
      wr_kbuf_addr_pipe2_read, data_pipe2, host_data_pcie_write_req_apply,
      host_data_pcie_write_req_data, host_rdcmd_fin_pcie_write_req_apply,
      host_rdcmd_fin_pcie_write_req_data, host_dram_write_req_apply,
      wrcmd_kbuf_addrs);
  pipeline_data_passer(rd_reqs_pipe0_write, wr_reqs_pipe0_write,
                       rd_reqs_pipe1_read, wr_reqs_pipe1_read,
                       rd_reqs_pipe1_write, wr_reqs_pipe1_write,
                       rd_reqs_pipe2_read, wr_reqs_pipe2_read,
                       rd_data_valid_pipe1_write, wr_data_valid_pipe1_write,
                       rd_data_valid_pipe2_read, wr_data_valid_pipe2_read,
                       rd_kbuf_addr_pipe0_write, wr_kbuf_addr_pipe0_write,
                       rd_kbuf_addr_pipe2_read, wr_kbuf_addr_pipe2_read);

  pcie_write_multiplexer(
      host_data_pcie_write_req_data, host_data_pcie_write_req_apply,
      before_throttle_pcie_write_req_data, before_throttle_pcie_write_req_apply,
      host_rdcmd_fin_pcie_write_req_data, host_rdcmd_fin_pcie_write_req_apply,
      host_wrcmd_fin_pcie_write_req_data, host_wrcmd_fin_pcie_write_req_apply,
      device_pcie_write_req_data_0, device_pcie_write_req_apply_0,
      device_pcie_write_req_data_1, device_pcie_write_req_apply_1,
      device_pcie_write_req_data_2, device_pcie_write_req_apply_2);

  pcie_write_throttle_unit(dma_write_throttle_params,
                           before_throttle_pcie_write_req_data,
                           before_throttle_pcie_write_req_apply,
                           pcie_write_req_data, pcie_write_req_apply);

  dram_read_req_multiplexer(host_dram_read_req, device_dram_read_req,
                            before_delay_unified_dram_read_req,
                            dram_read_context);
  dram_read_resp_multiplexer(host_dram_read_resp, device_dram_read_resp,
                             after_throttle_unified_dram_read_resp,
                             dram_read_context);

  device_dram_read_req_multiplexer(
      device_dram_req_context, device_dram_read_req, device_dram_read_req_0,
      device_dram_read_req_1, device_dram_read_req_2);
  device_dram_read_resp_multiplexer(
      device_dram_req_context, device_dram_read_resp, device_dram_read_resp_0,
      device_dram_read_resp_1, device_dram_read_resp_2);

  dram_read_throttle_unit(drive_read_throttle_params,
                          before_throttle_unified_dram_read_resp,
                          after_throttle_unified_dram_read_resp);
  dram_write_req_time_marker(host_dram_write_req_data,
                             host_dram_write_req_apply,
                             before_delay_dram_write_req_data,
                             before_delay_dram_write_req_apply_with_time);
  dram_write_delay_unit(drive_write_delay_cycle_cnts,
                        before_delay_dram_write_req_data,
                        before_delay_dram_write_req_apply_with_time,
                        before_throttle_unified_dram_write_req_data,
                        before_throttle_unified_dram_write_req_apply);
  dram_write_throttle_unit(drive_write_throttle_params,
                           before_throttle_unified_dram_write_req_data,
                           before_throttle_unified_dram_write_req_apply,
                           after_throttle_unified_dram_write_req_data,
                           after_throttle_unified_dram_write_req_apply);
  dram_read_req_time_marker(before_delay_unified_dram_read_req,
                            before_delay_unified_dram_read_req_with_time);
  dram_read_delay_unit(drive_read_delay_cycle_cnts,
                       before_delay_unified_dram_read_req_with_time,
                       after_delay_unified_dram_read_req);
  pipe0_dram_dispatcher(
      after_delay_unified_dram_read_req,
      after_throttle_unified_dram_write_req_apply, dramA_read_req,
      dramA_write_req_apply, dramB_read_req, dramB_write_req_apply,
      dramC_read_req, dramC_write_req_apply, dramD_read_req,
      dramD_write_req_apply, unified_dram_dispatcher_write_context,
      unified_dram_dispatcher_read_context);
  pipe1_dram_dispatcher(
      before_throttle_unified_dram_read_resp,
      after_throttle_unified_dram_write_req_data, dramA_read_resp,
      dramA_write_req_data, dramB_read_resp, dramB_write_req_data,
      dramC_read_resp, dramC_write_req_data, dramD_read_resp,
      dramD_write_req_data, unified_dram_dispatcher_write_context,
      unified_dram_dispatcher_read_context, host_wrcmd_fin_pcie_write_req_data,
      host_wrcmd_fin_pcie_write_req_apply, wrcmd_kbuf_addrs);

  dram_helper_app_0(app_file_infos_0, device_dram_read_req_0,
                    device_dram_read_resp_0, reset_dram_helper_app_0,
                    buf_app_input_data_0, buf_read_sig_app_input_data_0);
  dram_helper_app_1(app_file_infos_1, device_dram_read_req_1,
                    device_dram_read_resp_1, reset_dram_helper_app_1,
                    buf_app_input_data_1, buf_read_sig_app_input_data_1);
  dram_helper_app_2(app_file_infos_2, device_dram_read_req_2,
                    device_dram_read_resp_2, reset_dram_helper_app_2,
                    buf_app_input_data_2, buf_read_sig_app_input_data_2);

  buf_app_input_data_forwarder_0(
      buf_app_input_data_0, buf_read_sig_app_input_data_0, app_input_data_0,
      reset_buf_app_input_data_forwarder_0);
  buf_app_input_data_forwarder_1(
      buf_app_input_data_1, buf_read_sig_app_input_data_1, app_input_data_1,
      reset_buf_app_input_data_forwarder_1);
  buf_app_input_data_forwarder_2(
      buf_app_input_data_2, buf_read_sig_app_input_data_2, app_input_data_2,
      reset_buf_app_input_data_forwarder_2);

  pcie_helper_app_0(app_buf_addrs_0, device_pcie_write_req_apply_0,
                    device_pcie_write_req_data_0, app_output_data_splitted_0,
                    app_output_data_meta_0, app_free_buf_0,
                    reset_pcie_helper_app_0);
  pcie_data_splitter_app_0(app_output_data_0, app_output_data_splitted_0,
                           app_output_data_meta_0,
                           reset_pcie_data_splitter_app_0);

  pcie_helper_app_1(app_buf_addrs_1, device_pcie_write_req_apply_1,
                    device_pcie_write_req_data_1, app_output_data_splitted_1,
                    app_output_data_meta_1, app_free_buf_1,
                    reset_pcie_helper_app_1);
  pcie_data_splitter_app_1(app_output_data_1, app_output_data_splitted_1,
                           app_output_data_meta_1,
                           reset_pcie_data_splitter_app_1);

  pcie_helper_app_2(app_buf_addrs_2, device_pcie_write_req_apply_2,
                    device_pcie_write_req_data_2, app_output_data_splitted_2,
                    app_output_data_meta_2, app_free_buf_2,
                    reset_pcie_helper_app_2);
  pcie_data_splitter_app_2(app_output_data_2, app_output_data_splitted_2,
                           app_output_data_meta_2,
                           reset_pcie_data_splitter_app_2);

  ST_Queue<bool> reset_app_pt_0(4);
  ST_Queue<bool> reset_app_pt_1(4);
  ST_Queue<bool> reset_app_pt_2(4);
  reset_propaganda(
      reset_sigs_0, reset_sigs_1, reset_sigs_2, reset_app_pt_0, reset_app_pt_1,
      reset_app_pt_2, reset_dram_helper_app_0, reset_dram_helper_app_1,
      reset_dram_helper_app_2, reset_pcie_helper_app_0, reset_pcie_helper_app_1,
      reset_pcie_helper_app_2, reset_pcie_data_splitter_app_0,
      reset_pcie_data_splitter_app_1, reset_pcie_data_splitter_app_2,
      reset_buf_app_input_data_forwarder_0,
      reset_buf_app_input_data_forwarder_1,
      reset_buf_app_input_data_forwarder_2);

  ;
  app_pt_0(reset_app_pt_0, app_input_data_0, app_output_data_0,
           app_input_params_0);
  app_pt_1(reset_app_pt_1, app_input_data_1, app_output_data_1,
           app_input_params_1);
  app_pt_2(reset_app_pt_2, app_input_data_2, app_output_data_2,
           app_input_params_2);
}
