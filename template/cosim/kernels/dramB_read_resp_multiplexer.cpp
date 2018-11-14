#ifndef DRAMB_READ_RESP_MULTIPLEXER_CPP_
#define DRAMB_READ_RESP_MULTIPLEXER_CPP_

#include <insider_kernel.h>

void dramB_read_resp_multiplexer(
    ST_Queue<Dram_Read_Resp> &dramB_read_resp,
    ST_Queue<Dram_Read_Resp> &host_dramB_read_resp,
    ST_Queue<Dram_Read_Resp> &device_dramB_read_resp,
    ST_Queue<bool> &dramB_read_context) {
  bool valid_state = 0;
  bool data_state;
  bool valid_read_resp = 0;
  Dram_Read_Resp data_read_resp;

  while (1) {
#pragma HLS pipeline
    if (!valid_state) {
      valid_state = dramB_read_context.read_nb(data_state);
    }
    if (!valid_read_resp) {
      valid_read_resp = dramB_read_resp.read_nb(data_read_resp);
    }

    if (valid_state && valid_read_resp) {
      valid_read_resp = false;
      if (data_state == HOST_READ_REQ) {
        host_dramB_read_resp.write(data_read_resp);
      } else {
        device_dramB_read_resp.write(data_read_resp);
      }
      if (data_read_resp.last) {
        valid_state = false;
      }
    }
  }
}

#endif
