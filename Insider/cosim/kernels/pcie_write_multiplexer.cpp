#ifndef PCIE_WRITE_MULTIPLEXER_CPP_
#define PCIE_WRITE_MULTIPLEXER_CPP_

#include <insider_kernel.h>

void pcie_write_multiplexer(
		    ST_Queue<PCIe_Write_Req_Data>& host_data_pcie_write_req_data,
		    ST_Queue<PCIe_Write_Req_Apply>& host_data_pcie_write_req_apply,    
		    ST_Queue<PCIe_Write_Req_Data>& pcie_write_req_data,
		    ST_Queue<PCIe_Write_Req_Apply>& pcie_write_req_apply,
		    ST_Queue<PCIe_Write_Req_Data>& host_fin_pcie_write_req_data,
		    ST_Queue<PCIe_Write_Req_Apply>& host_fin_pcie_write_req_apply,
		    ST_Queue<PCIe_Write_Req_Data>& device_pcie_write_req_data,
		    ST_Queue<PCIe_Write_Req_Apply>& device_pcie_write_req_apply,
		    ST_Queue<Dram_Write_Req_Data>& cosim_dramA_write_req_data,
		    ST_Queue<Dram_Write_Req_Apply>& cosim_dramA_write_req_apply,
		    ST_Queue<Dram_Write_Req_Data>& cosim_dramB_write_req_data,
		    ST_Queue<Dram_Write_Req_Apply>& cosim_dramB_write_req_apply,
		    ST_Queue<Dram_Write_Req_Data>& cosim_dramC_write_req_data,
		    ST_Queue<Dram_Write_Req_Apply>& cosim_dramC_write_req_apply,
		    ST_Queue<Dram_Write_Req_Data>& cosim_dramD_write_req_data,
		    ST_Queue<Dram_Write_Req_Apply>& cosim_dramD_write_req_apply
		    ) {
  // state = 0: no outstanding pcie write transaction
  // state = 1: outstanding req from host-side data_pcie interface
  // state = 2: outstanding req from host-side fin_pcie interface
  // state = 3: outstanding req from device-side interface
  // state = 4: outstanding req from cosim-dramA interface
  // state = 5: outstanding req from cosim-dramB interface
  // state = 6: outstanding req from cosim-dramC interface
  // state = 7: outstanding req from cosim-dramD interface
  unsigned char state = 0;
  int device_pcie_write_req_lefting_num;
  while (1) {
#pragma HLS pipeline
    PCIe_Write_Req_Data write_data;
    if (state == 0) {
      PCIe_Write_Req_Apply req_apply;
      Dram_Write_Req_Apply dram_req_apply;
      /* fin interface has higher priority */
      if (host_fin_pcie_write_req_apply.read_nb(req_apply)) {
	state = 2;
	pcie_write_req_apply.write(req_apply);
      }
      else if (host_data_pcie_write_req_apply.read_nb(req_apply)) {
	state = 1;
	pcie_write_req_apply.write(req_apply);
      }
      else if (device_pcie_write_req_apply.read_nb(req_apply)) {
	state = 3;
	pcie_write_req_apply.write(req_apply);
	device_pcie_write_req_lefting_num = req_apply.num;
      }
      else if (cosim_dramA_write_req_apply.read_nb(dram_req_apply)) {
	state = 4;
	req_apply.addr = dram_req_apply.addr + COSIM_DRAMA_ADDR_OFFSET;
	req_apply.num = dram_req_apply.num;
	pcie_write_req_apply.write(req_apply);
      }
      else if (cosim_dramB_write_req_apply.read_nb(dram_req_apply)) {
	state = 5;
	req_apply.addr = dram_req_apply.addr + COSIM_DRAMB_ADDR_OFFSET;
	req_apply.num = dram_req_apply.num;
	pcie_write_req_apply.write(req_apply);
      }
      else if (cosim_dramC_write_req_apply.read_nb(dram_req_apply)) {
	state = 6;
	req_apply.addr = dram_req_apply.addr + COSIM_DRAMC_ADDR_OFFSET;
	req_apply.num = dram_req_apply.num;
	pcie_write_req_apply.write(req_apply);
      }
      else if (cosim_dramD_write_req_apply.read_nb(dram_req_apply)) {
	state = 7;
	req_apply.addr = dram_req_apply.addr + COSIM_DRAMD_ADDR_OFFSET;
	req_apply.num = dram_req_apply.num;
	pcie_write_req_apply.write(req_apply);
      }
    }

    if (state == 1) {
      if (host_data_pcie_write_req_data.read_nb(write_data)) {
	pcie_write_req_data.write(write_data);
	if (write_data.last) {
	  state = 0;
	}
      }
    }
    else if (state == 2) {
      if (host_fin_pcie_write_req_data.read_nb(write_data)) {
	pcie_write_req_data.write(write_data);
	if (write_data.last) {
	  state = 0;
	}
      }
    }
    else if (state == 3) {
      if (device_pcie_write_req_data.read_nb(write_data)) {
	device_pcie_write_req_lefting_num --;
	if (!device_pcie_write_req_lefting_num) {
	  write_data.last = true;
	}
	pcie_write_req_data.write(write_data);if (write_data.last) {
	  state = 0;
	}
      }
    }
    else if (state == 4) {
      Dram_Write_Req_Data dram_write_data;
      if (cosim_dramA_write_req_data.read_nb(dram_write_data)) {
	write_data.last = dram_write_data.last;
	write_data.data = dram_write_data.data;
	pcie_write_req_data.write(write_data);
	if (write_data.last) {
	  state = 0;
	}
      }
    }
    else if (state == 5) {
      Dram_Write_Req_Data dram_write_data;
      if (cosim_dramB_write_req_data.read_nb(dram_write_data)) {
	write_data.last = dram_write_data.last;
	write_data.data = dram_write_data.data;
	pcie_write_req_data.write(write_data);
	if (write_data.last) {
	  state = 0;
	}
      }
    }
    else if (state == 6) {
      Dram_Write_Req_Data dram_write_data;
      if (cosim_dramC_write_req_data.read_nb(dram_write_data)) {
	write_data.last = dram_write_data.last;
	write_data.data = dram_write_data.data;
	pcie_write_req_data.write(write_data);
	if (write_data.last) {
	  state = 0;
	}
      }
    }
    else if (state == 7) {
      Dram_Write_Req_Data dram_write_data;
      if (cosim_dramD_write_req_data.read_nb(dram_write_data)) {
	write_data.last = dram_write_data.last;
	write_data.data = dram_write_data.data;
	pcie_write_req_data.write(write_data);
	if (write_data.last) {
	  state = 0;
	}
      }
    }
  }
}
#endif
