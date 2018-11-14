#ifndef DEVICE_DRAM_THROTTLE_UNIT_CPP_
#define DEVICE_DRAM_THROTTLE_UNIT_CPP_

#include <insider_kernel.h>

void device_dram_throttle_unit(
    ST_Queue<Dram_Write_Req_Data> &device_dram_write_req_data_throttled,
    ST_Queue<Dram_Write_Req_Data> &device_dram_write_req_data_delayed,
    ST_Queue<Dram_Read_Resp> &device_dram_read_resp_throttled,
    ST_Queue<Dram_Read_Resp> &device_dram_read_resp_delayed,
    ST_Queue<unsigned int> &device_throttle_params) {
  unsigned int throttle_threshold = 0;
  unsigned int throttle_cnt = 0;

  bool write_req_stall = false;
  ap_uint<MAX_THROTTLE_CONTIGUOUS_LOG2 + 1> write_req_contiguous_times = 0;
  int write_req_stall_times = 0;
  Dram_Write_Req_Data write_req_data;
  bool valid_write_req_data = false;

  bool read_resp_stall = false;
  ap_uint<MAX_THROTTLE_CONTIGUOUS_LOG2 + 1> read_resp_contiguous_times = 0;
  int read_resp_stall_times = 0;
  Dram_Read_Resp read_resp;
  bool valid_read_resp = false;

  while (1) {
#pragma HLS pipeline
    unsigned int new_param;
    if (device_throttle_params.read_nb(new_param)) {
      throttle_threshold = new_param >> 16;
      throttle_cnt = new_param & 0xFFFF;
      write_req_stall = false;
      write_req_contiguous_times = write_req_stall_times = 0;
      read_resp_stall = false;
      read_resp_contiguous_times = read_resp_stall_times = 0;
    }

    if (!valid_write_req_data) {
      if (write_req_stall) {
        write_req_stall_times++;
        if (write_req_stall_times == throttle_cnt) {
          write_req_stall = false;
          write_req_stall_times = 0;
        }
      } else {
        if (device_dram_write_req_data_delayed.read_nb(write_req_data)) {
          valid_write_req_data = true;
          write_req_contiguous_times++;
          if (throttle_threshold != 0 && throttle_cnt != 0 &&
              write_req_contiguous_times == throttle_threshold) {
            write_req_stall = true;
            write_req_contiguous_times = 0;
          }
        } else {
          write_req_contiguous_times = 0;
        }
      }
    }
    if (valid_write_req_data) {
      if (device_dram_write_req_data_throttled.write_nb(write_req_data)) {
        valid_write_req_data = false;
      }
    }

    if (!valid_read_resp) {
      if (read_resp_stall) {
        read_resp_stall_times++;
        if (read_resp_stall_times == throttle_cnt) {
          read_resp_stall = false;
          read_resp_stall_times = 0;
        }
      } else {
        if (device_dram_read_resp_throttled.read_nb(read_resp)) {
          valid_read_resp = true;
          read_resp_contiguous_times++;
          if (throttle_threshold != 0 && throttle_cnt != 0 &&
              read_resp_contiguous_times == throttle_threshold) {
            read_resp_stall = true;
            read_resp_contiguous_times = 0;
          }
        } else {
          read_resp_contiguous_times = 0;
        }
      }
    }
    if (valid_read_resp) {
      if (device_dram_read_resp_delayed.write_nb(read_resp)) {
        valid_read_resp = false;
      }
    }
  }
}

#endif
