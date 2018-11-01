#ifndef PIPELINE_DATA_PASSER_CPP_
#define PIPELINE_DATA_PASSER_CPP_

#include <insider_kernel.h>

void pipeline_data_passer(
			  ST_Queue<Request> &reqs_pipe0_write,
			  ST_Queue<Request> &reqs_pipe1_read,
			  ST_Queue<Request> &reqs_pipe1_write,
			  ST_Queue<Request> &reqs_pipe2_read,
			  ST_Queue<Data> &data_pipe1_write,
			  ST_Queue<Data> &data_pipe2_read,
			  ST_Queue<bool> &data_valid_pipe1_write,
			  ST_Queue<bool> &data_valid_pipe2_read,
			  ST_Queue<unsigned long long> &kbuf_addr_pipe0_write,
			  ST_Queue<unsigned long long> &kbuf_addr_pipe2_read
			  ) {
  bool valid_req0 = false;
  Request data_req0;
  bool valid_req1 = false;
  Request data_req1;
  bool valid_data = false;
  Data data_data;
  bool valid_valid = false;
  bool data_valid;
  bool valid_addr = false;
  unsigned long long data_addr;  
  while (1) {
#pragma HLS pipeline
    if (valid_req0 || (valid_req0 = reqs_pipe0_write.read_nb(data_req0))) {
      if (reqs_pipe1_read.write_nb(data_req0)) {
	valid_req0 = false;
      }
    }
    if (valid_req1 || (valid_req1 = reqs_pipe1_write.read_nb(data_req1))) {
      if (reqs_pipe2_read.write_nb(data_req1)) {
	valid_req1 = false;
      }
    }
    if (valid_data || (valid_data = data_pipe1_write.read_nb(data_data))) {
      if (data_pipe2_read.write_nb(data_data)) {
	valid_data = false;
      }
    }
    if (valid_valid || (valid_valid = data_valid_pipe1_write.read_nb(data_valid))) {
      if (data_valid_pipe2_read.write_nb(data_valid)) {
	valid_valid = false;
      }
    }
    if (valid_addr || (valid_addr = kbuf_addr_pipe0_write.read_nb(data_addr))) {
      if (kbuf_addr_pipe2_read.write_nb(data_addr)) {
	valid_addr = false;
      }
    }
  }
}

#endif
