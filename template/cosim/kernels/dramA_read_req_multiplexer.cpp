#ifndef DRAMA_READ_REQ_MULTIPLEXER_CPP_
#define DRAMA_READ_REQ_MULTIPLEXER_CPP_

#include <insider_kernel.h>

void dramA_read_req_multiplexer(ST_Queue<Dram_Read_Req> &dramA_read_req,
                                ST_Queue<Dram_Read_Req> &host_dramA_read_req,
                                ST_Queue<Dram_Read_Req> &device_dramA_read_req,
                                ST_Queue<bool> &dramA_read_context) {
  while (1) {
#pragma HLS pipeline
    Dram_Read_Req req;
    if (host_dramA_read_req.read_nb(req)) {
      dramA_read_req.write(req);
      dramA_read_context.write(HOST_READ_REQ);
    } else if (device_dramA_read_req.read_nb(req)) {
      dramA_read_req.write(req);
      dramA_read_context.write(DEVICE_READ_REQ);
    }
  }
}

#endif
