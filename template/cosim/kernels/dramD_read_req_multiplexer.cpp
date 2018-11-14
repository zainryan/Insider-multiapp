#ifndef DRAMD_READ_REQ_MULTIPLEXER_CPP_
#define DRAMD_READ_REQ_MULTIPLEXER_CPP_

#include <insider_kernel.h>

void dramD_read_req_multiplexer(ST_Queue<Dram_Read_Req> &dramD_read_req,
                                ST_Queue<Dram_Read_Req> &host_dramD_read_req,
                                ST_Queue<Dram_Read_Req> &device_dramD_read_req,
                                ST_Queue<bool> &dramD_read_context) {
  while (1) {
#pragma HLS pipeline
    Dram_Read_Req req;
    if (host_dramD_read_req.read_nb(req)) {
      dramD_read_req.write(req);
      dramD_read_context.write(HOST_READ_REQ);
    } else if (device_dramD_read_req.read_nb(req)) {
      dramD_read_req.write(req);
      dramD_read_context.write(DEVICE_READ_REQ);
    }
  }
}

#endif
