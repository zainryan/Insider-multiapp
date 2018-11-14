#ifndef BUF_APP_INPUT_DATA_FORWARDER_1_CPP_
#define BUF_APP_INPUT_DATA_FORWARDER_1_CPP_

#include <insider_kernel.h>

void buf_app_input_data_forwarder_1(
    ST_Queue<APP_Data> &buf_app_input_data,
    ST_Queue<bool> &buf_read_sig_app_input_data,
    ST_Queue<APP_Data> &app_input_data,
    ST_Queue<bool> &reset_buf_app_input_data_forwarder_1) {
  APP_Data app_data;
  bool reset = false;
  unsigned reset_cnt = 0;

  while (1) {
#pragma HLS pipeline
    bool dummy;
    if (reset ||
        (reset = reset_buf_app_input_data_forwarder_1.read_nb(dummy))) {
      APP_Data dummy0;
      buf_app_input_data.read_nb(dummy0);
      reset_cnt++;
      if (reset_cnt == RESET_CNT) {
        reset_cnt = 0;
        reset = false;
      }
    } else {
      if (buf_app_input_data.read_nb(app_data)) {
        app_input_data.write(app_data);
        buf_read_sig_app_input_data.write_nb(true);
      }
    }
  }
}

#endif
