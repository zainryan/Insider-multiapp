#ifndef DRAM_HELPER_APP_CPP_
#define DRAM_HELPER_APP_CPP_

#include <insider_kernel.h>

void dram_helper_app(
			      ST_Queue<unsigned int> &app_file_infos,
			      ST_Queue<Dram_Read_Req> &device_dram_read_req,
			      ST_Queue<Dram_Read_Resp> &device_dram_read_resp,
			      ST_Queue<APP_Data> &app_input_data,
			      ST_Queue<bool> &reset_dram_helper_app
			      ) {
  unsigned int read_info_cnt = 0;
  ap_uint<DRAM_ADDR_BUS_WIDTH> l_addr, r_addr;
  ap_uint<DRAM_ADDR_BUS_WIDTH + 1> length;
  // state = 0: no inflight file read request
  // state = 1: has inflight
  bool state = 0;
  unsigned int num_issued = 0;
  unsigned int num_read = 0;
  bool finish_issuing = false;
  unsigned long long length_left;
  bool reset = false;
  unsigned int reset_cnt = 0;

  while (1) {
#pragma HLS pipeline
    bool dummy;
    if (reset || (reset = reset_dram_helper_app.read_nb(dummy))) {
      read_info_cnt = state = num_issued = num_read = finish_issuing = 0;
      unsigned int dummy0;
      app_file_infos.read_nb(dummy0);
      Dram_Read_Resp dummy1;
      device_dram_read_resp.read_nb(dummy1);
      reset_cnt ++;
      if (reset_cnt == RESET_CNT) {
	reset_cnt = 0;
	reset = false;
      }
    }
    else {
      if (!state) {
	unsigned int info;
	if (app_file_infos.read_nb(info)) {
	  read_info_cnt ++;
	  if (read_info_cnt == 1) {
	    l_addr = ((unsigned long long)info) << 12;
	  }
	  else if (read_info_cnt == 2) {
	    length = info;
	  }
	  else if (read_info_cnt == 3) {
	    length_left = length = (length << 32) | info;
	    r_addr = l_addr + length;
	    state = true;
	    finish_issuing = false;
	    num_issued = num_read = read_info_cnt = 0;
	  }
	}
      }
      else {
	if (length_left) {
	  Dram_Read_Req read_req;
	  bool next_finish_issuing;
	  ap_uint<DRAM_ADDR_BUS_WIDTH> next_l_addr;
	  ap_uint<DRAM_ADDR_BUS_WIDTH + 1> next_length_left;
	  unsigned next_num_issued;
	  read_req.addr = l_addr;
	  if ((DRAM_READ_BATCH_NUM << DATA_BUS_WIDTH_LOG2) < length_left) {
	    read_req.num = DRAM_READ_BATCH_NUM;
	    next_finish_issuing = false;
	    next_length_left = length_left - (DRAM_READ_BATCH_NUM << DATA_BUS_WIDTH_LOG2);
	  }
	  else {
	    read_req.num = (length_left + (DATA_BUS_WIDTH - 1)) >> DATA_BUS_WIDTH_LOG2;
	    next_finish_issuing = true;
	    next_length_left = 0;
	  }
	  next_l_addr = l_addr + (read_req.num << DATA_BUS_WIDTH_LOG2);
	  next_num_issued = num_issued + read_req.num;

	  if (device_dram_read_req.write_nb(read_req)) {
	    l_addr = next_l_addr;
	    num_issued = next_num_issued;
	    finish_issuing = next_finish_issuing;
	    length_left = next_length_left;
	  }
	}
	Dram_Read_Resp read_resp;
	if (device_dram_read_resp.read_nb(read_resp)) {
	  num_read ++;
	  APP_Data app_data;
	  app_data.data = read_resp.data;
	  app_data.eop = false;
	  app_data.len = DATA_BUS_WIDTH;
	  if (num_issued == num_read && finish_issuing) {
	    app_data.eop = true;
	    app_data.len = length & (DATA_BUS_WIDTH - 1);
	    if (app_data.len == 0) {
	      app_data.len = DATA_BUS_WIDTH;
	    }
	    state = false;
	  }
	  app_input_data.write(app_data);
	}
      }
    }
  }
}

#endif
