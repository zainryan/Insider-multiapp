#ifndef PCIE_READ_RESP_MUX_CPP_
#define PCIE_READ_RESP_MUX_CPP_

#include <insider_kernel.h>

void pcie_read_resp_mux(
    ST_Queue<PCIe_Read_Resp> &pcie_read_resp,
    ST_Queue<PCIe_Read_Resp> &hw_pcie_read_resp,
    ST_Queue<Dram_Read_Resp> &cosim_dramA_read_resp,
    ST_Queue<Dram_Read_Resp> &cosim_dramB_read_resp,
    ST_Queue<Dram_Read_Resp> &cosim_dramC_read_resp,
    ST_Queue<Dram_Read_Resp> &cosim_dramD_read_resp,
    ST_Queue<unsigned char> &pcie_read_multiplexer_read_context) {

  unsigned char state = 0;
  while (1) {
#pragma HLS pipeline
    if (!state) {
      unsigned char tmp;
      if (pcie_read_multiplexer_read_context.read_nb(tmp)) {
        state = tmp;
      }
    }
    if (state) {
      PCIe_Read_Resp read_resp;
      Dram_Read_Resp dram_read_resp;
      if (pcie_read_resp.read_nb(read_resp)) {
        if (state == 1) {
          hw_pcie_read_resp.write(read_resp);
        } else if (state == 2) {
          dram_read_resp.last = read_resp.last;
          dram_read_resp.data = read_resp.data;
          cosim_dramA_read_resp.write(dram_read_resp);
        } else if (state == 3) {
          dram_read_resp.last = read_resp.last;
          dram_read_resp.data = read_resp.data;
          cosim_dramB_read_resp.write(dram_read_resp);
        } else if (state == 4) {
          dram_read_resp.last = read_resp.last;
          dram_read_resp.data = read_resp.data;
          cosim_dramC_read_resp.write(dram_read_resp);
        } else if (state == 5) {
          dram_read_resp.last = read_resp.last;
          dram_read_resp.data = read_resp.data;
          cosim_dramD_read_resp.write(dram_read_resp);
        }
        if (read_resp.last) {
          state = 0;
        }
      }
    }
  }
}
#endif
