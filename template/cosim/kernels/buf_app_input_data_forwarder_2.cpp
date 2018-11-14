#ifndef BUF_APP_INPUT_DATA_FORWARDER_2_CPP_
#define BUF_APP_INPUT_DATA_FORWARDER_2_CPP_

#include <insider_kernel.h>

void buf_app_input_data_forwarder_2(ST_Queue<APP_Data> &buf_app_input_data,
                                    ST_Queue<bool> &buf_read_sig_app_input_data,
                                    ST_Queue<APP_Data> &app_input_data) {
  APP_Data app_data;
  bool valid_app_data = false;

  while (1) {
#pragma HLS pipeline
    if (valid_app_data ||
        (valid_app_data = buf_app_input_data.read_nb(app_data))) {
      if (app_input_data.write_nb(app_data)) {
        valid_app_data = false;
        buf_read_sig_app_input_data.write_nb(true);
      }
    }
  }
}

#endif
