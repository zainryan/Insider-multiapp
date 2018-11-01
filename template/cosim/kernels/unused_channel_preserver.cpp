#ifndef UNUSED_CHANNEL_PRESERVER_CPP_
#define UNUSED_CHANNEL_PRESERVER_CPP_

#include <insider_kernel.h>

void unused_channel_preserver(
			      ST_Queue<Dram_Write_Req_Data> &device_dram_write_req_data,
			      ST_Queue<Dram_Write_Req_Apply> &device_dram_write_req_apply,
			      ST_Queue<bool> &preserver_cheat_signals
) {
  while (1) {
#pragma HLS pipeline
    bool dummy;
    if (preserver_cheat_signals.read_nb(dummy)) {
      Dram_Write_Req_Data req_data;
      Dram_Write_Req_Apply req_apply;
      device_dram_write_req_data.write(req_data);
      device_dram_write_req_apply.write(req_apply);
    }
  }
}

#endif
