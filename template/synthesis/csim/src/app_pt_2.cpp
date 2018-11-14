#ifndef APP_PT_2_CPP_
#define APP_PT_2_CPP_
#include <insider_kernel.h>

#define PROCESSING_PERIOD (4)

/**
   This maximum data processing rate of this kernel
   = 0.25 flit per cycle = 4 gibibyte/s
 */
void app_pt_2(ST_Queue<bool> &reset_app_pt_2,
              ST_Queue<APP_Data> &app_input_data,
              ST_Queue<APP_Data> &app_output_data,
              ST_Queue<unsigned int> &app_input_params) {

  bool reset = false;
  unsigned reset_cnt = 0;
  int state = 0;
  while (1) {
#pragma HLS pipeline
    bool dummy;
    if (reset || (reset = reset_app_pt_2.read_nb(dummy))) {
      unsigned int dummy0;
      app_input_params.read_nb(dummy0);
      struct APP_Data dummy1;
      app_input_data.read_nb(dummy1);
      reset_cnt++;
      if (reset_cnt == RESET_CNT) {
        reset_cnt = 0;
        reset = false;
        state = 0;
      }
    } else {
      unsigned int dummy0, dummy1;
      dummy0 = app_input_params.read_nb(dummy1);
      APP_Data data;
      state++;
      if (state == PROCESSING_PERIOD) {
        if (app_input_data.read_nb(data)) {
          if (data.eop) {
            app_output_data.write(data);
          }
        }
        state = 0;
      }
    }
  }
}

#undef PROCESSING_PERIOD
#endif
