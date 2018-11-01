#ifndef RESET_PROPAGANDA_CPP_
#define RESET_PROPAGANDA_CPP_

#include <insider_kernel.h>

void reset_propaganda(ST_Queue<bool> &reset_app_loopback,
                      ST_Queue<bool> &reset_sigs,
                      ST_Queue<bool> &reset_dram_helper_app,
                      ST_Queue<bool> &reset_pcie_helper_app,
                      ST_Queue<bool> &reset_pcie_data_splitter_app) {
  while (1) {
#pragma HLS pipeline
    bool dummy;
    if (reset_sigs.read_nb(dummy)) {
      reset_dram_helper_app.write(0);
      reset_pcie_helper_app.write(0);
      reset_pcie_data_splitter_app.write(0);

      reset_app_loopback.write(0);
    }
  }
}
#endif