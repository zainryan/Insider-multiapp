#ifndef PIPE2_DATA_HANDLER_CPP_
#define PIPE2_DATA_HANDLER_CPP_

#include <insider_kernel.h>

void pipe2_data_handler(
    ST_Queue<Request> &reqs_pipe2_read, ST_Queue<bool> &data_valid_pipe2_read,
    ST_Queue<unsigned long long> &kbuf_addr_pipe2_read,
    ST_Queue<Data> &data_pipe2_read,
    ST_Queue<PCIe_Write_Req_Apply> &host_data_pcie_write_req_apply,
    ST_Queue<PCIe_Write_Req_Apply> &host_fin_pcie_write_req_apply,
    ST_Queue<PCIe_Write_Req_Data> &host_data_pcie_write_req_data,
    ST_Queue<Dram_Write_Req_Apply> &host_dram_write_req_apply,
    ST_Queue<Dram_Write_Req_Data> &host_dram_write_req_data,
    ST_Queue<PCIe_Write_Req_Data> &host_fin_pcie_write_req_data,
    ST_Queue<bool> &host_dram_write_resp) {
  Request pipe2_req;
  unsigned long long kbuf_addr;
  bool first = true;
  bool has_reqs = false;
  bool has_data_valid = false;
  bool has_kbuf_addr = false;
  bool reset = false;
  bool is_write_last = false;

  while (1) {
#pragma HLS pipeline
    if (reset) {
      first = true;
      is_write_last = has_reqs = has_data_valid = has_kbuf_addr = reset = false;
    }
    if (!(has_reqs && has_data_valid && has_kbuf_addr)) {
      bool dummy;
      if (!has_reqs) {
        has_reqs = reqs_pipe2_read.read_nb(pipe2_req);
      }
      if (!has_data_valid) {
        has_data_valid = data_valid_pipe2_read.read_nb(dummy);
      }
      if (!has_kbuf_addr) {
        has_kbuf_addr = kbuf_addr_pipe2_read.read_nb(kbuf_addr);
      }
    }
    if (has_reqs && has_data_valid && has_kbuf_addr) {
      if (pipe2_req.rw == READ_CMD) {
        Data data;
        if (data_pipe2_read.read_nb(data)) {
          if (first) {
            PCIe_Write_Req_Apply write_apply;
            write_apply.addr = kbuf_addr;
            write_apply.num = pipe2_req.sector_num << 3;
            host_data_pcie_write_req_apply.write(write_apply);
            first = false;
          }
          PCIe_Write_Req_Data write_data;
          write_data.last = data.last;
          write_data.data = data.data;
          host_data_pcie_write_req_data.write(write_data);
          if (data.last) {
            reset = true;
            PCIe_Write_Req_Apply fin_write_apply;
            fin_write_apply.num = 1;
            fin_write_apply.addr = kbuf_addr + PHYSICAL_SECTOR_SIZE;
            host_fin_pcie_write_req_apply.write(fin_write_apply);
            PCIe_Write_Req_Data fin_write_data;
            fin_write_data.last = true;
            for (int idx = 0; idx < 512; idx += 8) {
#pragma HLS unroll
              fin_write_data.data(idx + 7, idx) = 0xFF;
            }
            host_fin_pcie_write_req_data.write(fin_write_data);
          }
        }
      } else if (pipe2_req.rw == WRITE_CMD) {
        if (first) {
          Dram_Write_Req_Apply write_apply;
          write_apply.addr = ((unsigned long long)pipe2_req.sector_off) << 9;
          write_apply.num = pipe2_req.sector_num << 3;
          host_dram_write_req_apply.write(write_apply);
          first = false;
        }
        Data data;
        if (!is_write_last) {
          if (data_pipe2_read.read_nb(data)) {
            Dram_Write_Req_Data write_data;
            write_data.last = data.last;
            write_data.data = data.data;
            host_dram_write_req_data.write(write_data);
            is_write_last = data.last;
          }
        } else {
          bool dummy;
          if (host_dram_write_resp.read_nb(dummy)) {
            reset = true;
            PCIe_Write_Req_Apply fin_write_apply;
            fin_write_apply.num = 1;
            fin_write_apply.addr = kbuf_addr + PHYSICAL_SECTOR_SIZE;
            host_fin_pcie_write_req_apply.write(fin_write_apply);
            PCIe_Write_Req_Data fin_write_data;
            fin_write_data.last = true;
            for (int idx = 0; idx < 512; idx += 8) {
#pragma HLS unroll
              fin_write_data.data(idx + 7, idx) = 0xFF;
            }
            host_fin_pcie_write_req_data.write(fin_write_data);
          }
        }
      }
    }
  }
}

#endif
