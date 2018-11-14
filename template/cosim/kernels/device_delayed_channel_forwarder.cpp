#ifndef DEVICE_DELAYED_CHANNEL_FORWARDER_CPP_
#define DEVICE_DELAYED_CHANNEL_FORWARDER_CPP_

#include <insider_kernel.h>

void device_delayed_channel_forwarder(
    ST_Queue<Dram_Write_Req_Data> &device_dram_write_req_data,
    ST_Queue<Dram_Write_Req_Data> &device_dram_write_req_data_delayed,
    ST_Queue<Dram_Read_Resp> &device_dram_read_resp_delayed,
    ST_Queue<Dram_Read_Resp> &device_dram_read_resp) {
  Dram_Write_Req_Data write_req_data;
  Dram_Read_Resp read_resp;
  bool valid_write_req_data = false;
  bool valid_read_resp = false;

  while (1) {
#pragma HLS pipeline
    if (valid_write_req_data ||
        (valid_write_req_data =
             device_dram_write_req_data.read_nb(write_req_data))) {
      if (device_dram_write_req_data_delayed.write_nb(write_req_data)) {
        valid_write_req_data = false;
      }
    }
    if (valid_read_resp ||
        (valid_read_resp = device_dram_read_resp_delayed.read_nb(read_resp))) {
      if (device_dram_read_resp.write_nb(read_resp)) {
        valid_read_resp = false;
      }
    }
  }
}

#endif
