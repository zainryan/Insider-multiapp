#ifndef KEEPER_PCIE_READ_RESP_CPP_
#define KEEPER_PCIE_READ_RESP_CPP_

#include <insider_kernel.h>

void keeper_pcie_read_resp(
			   ST_Queue<PCIe_Read_Resp> &pcie_read_resp,
			   ST_Queue<PCIe_Read_Resp> &buffered_pcie_read_resp
			   ) {
  while (1) {
#pragma HLS pipeline
    PCIe_Read_Resp read_resp;
    if (pcie_read_resp.read_nb(read_resp)) {
      buffered_pcie_read_resp.write(read_resp);
    }
  }
}

#endif
