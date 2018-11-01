#ifndef PIPE1_DATA_HANDLER_CPP_
#define PIPE1_DATA_HANDLER_CPP_

#include <insider_kernel.h>

void pipe1_data_handler(
			ST_Queue<Request> &reqs_pipe1_read,
			ST_Queue<Dram_Read_Resp> &host_dram_read_resp,
			ST_Queue<PCIe_Read_Resp> &buffered_pcie_read_resp,
			ST_Queue<Request> &reqs_pipe1_write,
			ST_Queue<bool> &data_valid_pipe1_write,
			ST_Queue<Data> &data_pipe1_write
			) {
  Request pipe1_req;
  bool has_inflight = false;

  while (1) {
#pragma HLS pipeline
    if (!has_inflight) {
      has_inflight = reqs_pipe1_read.read_nb(pipe1_req);
    }
    if (has_inflight) {
      if (pipe1_req.rw == READ_CMD) {
	Dram_Read_Resp read_resp;
	if (host_dram_read_resp.read_nb(read_resp)) {
	  Data data;
	  data.last = read_resp.last;
	  data.data = read_resp.data;
	  data_pipe1_write.write(data);
	  if (read_resp.last) {
	    reqs_pipe1_write.write(pipe1_req);
	    has_inflight = false;
	    data_valid_pipe1_write.write(true);
	  }
	}
      }
      else if (pipe1_req.rw == WRITE_CMD) {
	PCIe_Read_Resp read_resp;
	if (buffered_pcie_read_resp.read_nb(read_resp)) {
	  Data data;
	  data.last = read_resp.last;
	  data.data = read_resp.data;
	  data_pipe1_write.write(data);
	  if (read_resp.last) {
	    reqs_pipe1_write.write(pipe1_req);
	    has_inflight = false;
	    data_valid_pipe1_write.write(true);
	  }
	}
      }
    }
  }
}

#endif
