#ifndef RESET_PROPAGANDA_CPP_
#define RESET_PROPAGANDA_CPP_

#include <insider_kernel.h>

void reset_propaganda(ST_Queue<bool> &reset_sigs_0,
                      ST_Queue<bool> &reset_sigs_1,
                      ST_Queue<bool> &reset_sigs_2,
                      ST_Queue<bool> &reset_app_pt_0,
                      ST_Queue<bool> &reset_app_pt_1,
                      ST_Queue<bool> &reset_app_pt_2,
                      ST_Queue<bool> &reset_dram_helper_app_0,
                      ST_Queue<bool> &reset_dram_helper_app_1,
                      ST_Queue<bool> &reset_dram_helper_app_2,
                      ST_Queue<bool> &reset_pcie_helper_app_0,
                      ST_Queue<bool> &reset_pcie_helper_app_1,
                      ST_Queue<bool> &reset_pcie_helper_app_2,
                      ST_Queue<bool> &reset_pcie_data_splitter_app_0,
                      ST_Queue<bool> &reset_pcie_data_splitter_app_1,
                      ST_Queue<bool> &reset_pcie_data_splitter_app_2) {
  while (1) {
#pragma HLS pipeline
    bool dummy;
    if (reset_sigs_0.read_nb(dummy)) {
      reset_app_pt_0.write(0);
      reset_dram_helper_app_0.write(0);
      reset_pcie_helper_app_0.write(0);
      reset_pcie_data_splitter_app_0.write(0);
    }

    if (reset_sigs_1.read_nb(dummy)) {
      reset_app_pt_1.write(0);
      reset_dram_helper_app_1.write(0);
      reset_pcie_helper_app_1.write(0);
      reset_pcie_data_splitter_app_1.write(0);
    }

    if (reset_sigs_2.read_nb(dummy)) {
      reset_app_pt_2.write(0);
      reset_dram_helper_app_2.write(0);
      reset_pcie_helper_app_2.write(0);
      reset_pcie_data_splitter_app_2.write(0);
    }
  }
}
#endif
