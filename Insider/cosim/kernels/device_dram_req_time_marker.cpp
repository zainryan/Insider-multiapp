#ifndef DEVICE_DRAM_REQ_TIME_MARKER_CPP_
#define DEVICE_DRAM_REQ_TIME_MARKER_CPP_

#include <insider_kernel.h>

void device_dram_req_time_marker(
				 ST_Queue<Dram_Read_Req> &device_dram_read_req,
				 ST_Queue<Dram_Write_Req_Apply> &device_dram_write_req_apply,
				 ST_Queue<Dram_Read_Req_With_Time> &device_dram_read_req_with_time,
				 ST_Queue<Dram_Write_Req_Apply_With_Time> &device_dram_write_req_apply_with_time
				 ) {
  unsigned long long time = 0;
  bool valid_read_req = false;
  bool valid_write_req_apply = false;
  Dram_Read_Req read_req;
  Dram_Write_Req_Apply write_req_apply;

  while (1) {
#pragma HLS pipeline
    if (valid_read_req || (valid_read_req = device_dram_read_req.read_nb(read_req))) {
      Dram_Read_Req_With_Time read_req_with_time;
      read_req_with_time.req = read_req;
      read_req_with_time.time = time;
      valid_read_req = true;
      if (device_dram_read_req_with_time.write_nb(read_req_with_time)) {
	valid_read_req = false;
      }
    }
  
    if (valid_write_req_apply || (valid_write_req_apply = device_dram_write_req_apply.read_nb(write_req_apply))) {
      Dram_Write_Req_Apply_With_Time write_req_apply_with_time;
      write_req_apply_with_time.req_apply = write_req_apply;
      write_req_apply_with_time.time = time;
      valid_write_req_apply = true;
      if (device_dram_write_req_apply_with_time.write_nb(write_req_apply_with_time)) {
	valid_write_req_apply = false;
      }
    }
    time ++;  
  }
}

#endif
