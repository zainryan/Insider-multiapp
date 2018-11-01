#ifndef DRAMC_READ_REQ_MULTIPLEXER_CPP_
#define DRAMC_READ_REQ_MULTIPLEXER_CPP_

#include <insider_kernel.h>

void dramC_read_req_multiplexer(
				ST_Queue<Dram_Read_Req> &dramC_read_req,
				ST_Queue<Dram_Read_Req> &host_dramC_read_req,
				ST_Queue<Dram_Read_Req> &device_dramC_read_req,
				ST_Queue<bool> &dramC_read_context
				) {
  while (1) {
#pragma HLS pipeline
    Dram_Read_Req req;
    if (host_dramC_read_req.read_nb(req)) {
      dramC_read_req.write(req);
      dramC_read_context.write(HOST_READ_REQ);
    }
    else if (device_dramC_read_req.read_nb(req)) {
      dramC_read_req.write(req);
      dramC_read_context.write(DEVICE_READ_REQ);
    }
  } 
}

#endif
