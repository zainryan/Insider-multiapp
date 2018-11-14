#ifndef DRAMB_READ_REQ_MULTIPLEXER_CPP_
#define DRAMB_READ_REQ_MULTIPLEXER_CPP_

#include <insider_kernel.h>

void dramB_read_req_multiplexer(ST_Queue<Dram_Read_Req> &dramB_read_req,
                                ST_Queue<Dram_Read_Req> &host_dramB_read_req,
                                ST_Queue<Dram_Read_Req> &device_dramB_read_req,
                                ST_Queue<bool> &dramB_read_context) {
  while (1) {
#pragma HLS pipeline
    Dram_Read_Req req;
    if (host_dramB_read_req.read_nb(req)) {
      dramB_read_req.write(req);
      dramB_read_context.write(HOST_READ_REQ);
    } else if (device_dramB_read_req.read_nb(req)) {
      dramB_read_req.write(req);
      dramB_read_context.write(DEVICE_READ_REQ);
    }
  }
}

#endif
