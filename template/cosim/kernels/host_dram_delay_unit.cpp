#ifndef HOST_DRAM_DELAY_UNIT_CPP_
#define HOST_DRAM_DELAY_UNIT_CPP_

#include <insider_kernel.h>

void host_dram_delay_unit(
    ST_Queue<unsigned int> &host_delay_cycle_cnts,
    ST_Queue<Dram_Read_Req_With_Time> &host_dram_read_req_with_time,
    ST_Queue<Dram_Write_Req_Apply_With_Time>
        &host_dram_write_req_apply_with_time,
    ST_Queue<Dram_Read_Req> &host_dram_read_req_delayed,
    ST_Queue<Dram_Write_Req_Apply> &host_dram_write_req_apply_delayed) {
  unsigned long long time = 0;
  unsigned long long delay_cycle_cnt = 0;
  Dram_Read_Req_With_Time read_req_with_time;
  bool valid_read_req_with_time = false;
  Dram_Write_Req_Apply_With_Time write_req_apply_with_time;
  bool valid_write_req_apply_with_time = false;

  while (1) {
#pragma HLS pipeline
    unsigned int new_delay_cycle_cnt;
    if (host_delay_cycle_cnts.read_nb(new_delay_cycle_cnt)) {
      delay_cycle_cnt = new_delay_cycle_cnt;
    }

    if (valid_read_req_with_time ||
        (valid_read_req_with_time =
             host_dram_read_req_with_time.read_nb(read_req_with_time))) {
      if (read_req_with_time.time + delay_cycle_cnt <= time ||
          read_req_with_time.time > time) {
        if (host_dram_read_req_delayed.write_nb(read_req_with_time.req)) {
          valid_read_req_with_time = false;
        }
      }
    }

    if (valid_write_req_apply_with_time ||
        (valid_write_req_apply_with_time =
             host_dram_write_req_apply_with_time.read_nb(
                 write_req_apply_with_time))) {
      if (write_req_apply_with_time.time + delay_cycle_cnt <= time ||
          write_req_apply_with_time.time > time) {
        if (host_dram_write_req_apply_delayed.write_nb(
                write_req_apply_with_time.req_apply)) {
          valid_write_req_apply_with_time = false;
        }
      }
    }
    time++;
  }
}

#endif
