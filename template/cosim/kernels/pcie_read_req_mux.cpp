#ifndef PCIE_READ_REQ_MUX_CPP_
#define PCIE_READ_REQ_MUX_CPP_

#include <insider_kernel.h>

void pcie_read_req_mux(
    ST_Queue<PCIe_Read_Req> &pcie_read_req,
    ST_Queue<PCIe_Read_Req> &hw_pcie_read_req,
    ST_Queue<Dram_Read_Req> &cosim_dramA_read_req,
    ST_Queue<Dram_Read_Req> &cosim_dramB_read_req,
    ST_Queue<Dram_Read_Req> &cosim_dramC_read_req,
    ST_Queue<Dram_Read_Req> &cosim_dramD_read_req,
    ST_Queue<unsigned char> &pcie_read_multiplexer_write_context) {
  int last_flag = 2;
  while (1) {
#pragma HLS pipeline
    PCIe_Read_Req read_req;
    PCIe_Read_Req hw_read_req;
    Dram_Read_Req dram_read_req;

    int flag = 0;
    if (hw_pcie_read_req.read_nb(hw_read_req)) {
      flag = 1;
      read_req = hw_read_req;
    } else {
      if (last_flag == 2) {
        if (cosim_dramB_read_req.read_nb(dram_read_req)) {
          flag = 3;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMB_ADDR_OFFSET;
        } else if (cosim_dramC_read_req.read_nb(dram_read_req)) {
          flag = 4;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMC_ADDR_OFFSET;
        } else if (cosim_dramD_read_req.read_nb(dram_read_req)) {
          flag = 5;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMD_ADDR_OFFSET;
        } else if (cosim_dramA_read_req.read_nb(dram_read_req)) {
          flag = 2;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMA_ADDR_OFFSET;
        }
      } else if (last_flag == 3) {
        if (cosim_dramC_read_req.read_nb(dram_read_req)) {
          flag = 4;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMC_ADDR_OFFSET;
        } else if (cosim_dramD_read_req.read_nb(dram_read_req)) {
          flag = 5;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMD_ADDR_OFFSET;
        } else if (cosim_dramA_read_req.read_nb(dram_read_req)) {
          flag = 2;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMA_ADDR_OFFSET;
        } else if (cosim_dramB_read_req.read_nb(dram_read_req)) {
          flag = 3;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMB_ADDR_OFFSET;
        }
      } else if (last_flag == 4) {
        if (cosim_dramD_read_req.read_nb(dram_read_req)) {
          flag = 5;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMD_ADDR_OFFSET;
        } else if (cosim_dramA_read_req.read_nb(dram_read_req)) {
          flag = 2;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMA_ADDR_OFFSET;
        } else if (cosim_dramB_read_req.read_nb(dram_read_req)) {
          flag = 3;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMB_ADDR_OFFSET;
        } else if (cosim_dramC_read_req.read_nb(dram_read_req)) {
          flag = 4;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMC_ADDR_OFFSET;
        }
      } else if (last_flag == 5) {
        if (cosim_dramA_read_req.read_nb(dram_read_req)) {
          flag = 2;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMA_ADDR_OFFSET;
        } else if (cosim_dramB_read_req.read_nb(dram_read_req)) {
          flag = 3;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMB_ADDR_OFFSET;
        } else if (cosim_dramC_read_req.read_nb(dram_read_req)) {
          flag = 4;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMC_ADDR_OFFSET;
        } else if (cosim_dramD_read_req.read_nb(dram_read_req)) {
          flag = 5;
          read_req.num = dram_read_req.num;
          read_req.addr = dram_read_req.addr + COSIM_DRAMD_ADDR_OFFSET;
        }
      }
    }

    if (flag) {
      pcie_read_req.write(read_req);
      pcie_read_multiplexer_write_context.write(flag);
      last_flag = flag;
    }
  }
}
#endif
