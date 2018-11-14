#ifndef DEVICE_DRAM_READ_RESP_MULTIPLEXER_CPP_
#define DEVICE_DRAM_READ_RESP_MULTIPLEXER_CPP_

#include <insider_kernel.h>

void device_dram_read_resp_multiplexer(
    ST_Queue<Device_Dram_Context> &device_dram_req_context,
    ST_Queue<Dram_Read_Resp> &device_dram_read_resp,
    ST_Queue<Dram_Read_Resp> &device_dram_read_resp_0,
    ST_Queue<Dram_Read_Resp> &device_dram_read_resp_1,
    ST_Queue<Dram_Read_Resp> &device_dram_read_resp_2) {
  bool valid_inflight_req = false;
  Device_Dram_Context inflight_req;

  while (1) {
#pragma HLS pipeline

    if (valid_inflight_req ||
        (valid_inflight_req = device_dram_req_context.read_nb(inflight_req))) {
      Dram_Read_Resp dram_read_resp;
      if (device_dram_read_resp.read_nb(dram_read_resp)) {
        if (inflight_req.id == 0) {
          device_dram_read_resp_0.write(dram_read_resp);
        } else if (inflight_req.id == 1) {
          device_dram_read_resp_1.write(dram_read_resp);
        } else if (inflight_req.id == 2) {
          device_dram_read_resp_2.write(dram_read_resp);
        }
        inflight_req.num--;
        if (inflight_req.num == 0) {
          valid_inflight_req = false;
        }
      }
    }
  }
}

#endif
