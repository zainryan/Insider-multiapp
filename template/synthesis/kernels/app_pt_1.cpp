#ifndef APP_PT_1_CPP_
#define APP_PT_1_CPP_
#include <insider_kernel.h>

void app_pt_1(ST_Queue<bool> &reset_app_pt_1,
              ST_Queue<APP_Data> &app_input_data,
              ST_Queue<APP_Data> &app_output_data,
              ST_Queue<unsigned int> &app_input_params) {

  bool reset = false;
  unsigned reset_cnt = 0;
  while (1) {
#pragma HLS pipeline
    bool dummy;
    if (reset || (reset = reset_app_pt_1.read_nb(dummy))) {
      unsigned int dummy0;
      app_input_params.read_nb(dummy0);
      struct APP_Data dummy1;
      app_input_data.read_nb(dummy1);
      reset_cnt++;
      if (reset_cnt == RESET_CNT) {
        reset_cnt = 0;
        reset = false;
      }
    } else {
      unsigned int dummy0, dummy1;
      dummy0 = app_input_params.read_nb(dummy1);
      APP_Data data;
      app_input_data.read(data);
      app_output_data.write(data);
    }
  }
}

#endif
