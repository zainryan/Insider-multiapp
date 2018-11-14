#ifndef DEVICE_PIPE1_DRAM_DISPATCHER_CPP_
#define DEVICE_PIPE1_DRAM_DISPATCHER_CPP_

#include <insider_kernel.h>

void device_pipe1_dram_dispatcher(
    ST_Queue<Dram_Read_Resp> &device_dram_read_resp_throttled,
    ST_Queue<Dram_Write_Req_Data> &device_dram_write_req_data_throttled,
    ST_Queue<Dram_Read_Resp> &device_dramA_read_resp,
    ST_Queue<Dram_Write_Req_Data> &device_dramA_write_req_data,
    ST_Queue<Dram_Read_Resp> &device_dramB_read_resp,
    ST_Queue<Dram_Write_Req_Data> &device_dramB_write_req_data,
    ST_Queue<Dram_Read_Resp> &device_dramC_read_resp,
    ST_Queue<Dram_Write_Req_Data> &device_dramC_write_req_data,
    ST_Queue<Dram_Read_Resp> &device_dramD_read_resp,
    ST_Queue<Dram_Write_Req_Data> &device_dramD_write_req_data,
    ST_Queue<Dram_Dispatcher_Write_Req> &device_dram_dispatcher_write_context,
    ST_Queue<Dram_Dispatcher_Read_Req> &device_dram_dispatcher_read_context) {
  unsigned int read_bank_id;
  unsigned int write_bank_id;
  unsigned int read_end_bank_id;
  unsigned int write_end_bank_id;

  ap_uint<PHYSICAL_SECTOR_SIZE_LOG2 - DATA_BUS_WIDTH_LOG2 + 1> dram_read_times =
      0;
  ap_uint<PHYSICAL_SECTOR_SIZE_LOG2 - DATA_BUS_WIDTH_LOG2 + 1>
      dram_write_times = 0;

  ap_uint<PHYSICAL_SECTOR_SIZE_LOG2 - DATA_BUS_WIDTH_LOG2 + 1>
      before_boundry_len = 0; // only used for write

  bool read_is_before_boundry = true;
  bool write_is_before_boundry = true;
  bool has_read_req = false;
  bool has_write_req = false;

  ap_uint<PHYSICAL_SECTOR_SIZE_LOG2 - DATA_BUS_WIDTH_LOG2 + 1> write_cmd_num =
      0;
  ap_uint<PHYSICAL_SECTOR_SIZE_LOG2 - DATA_BUS_WIDTH_LOG2 + 1> read_cmd_num = 0;

  while (1) {
#pragma HLS pipeline
    Dram_Read_Resp read_resp;
    Dram_Dispatcher_Read_Req read_cmd;

    if (!has_read_req) {
      if (device_dram_dispatcher_read_context.read_nb(read_cmd)) {
        read_bank_id = read_cmd.bank_id;
        read_end_bank_id = read_cmd.end_bank_id;
        read_cmd_num = read_cmd.cmd_num;

        read_is_before_boundry = true;
        has_read_req = true;
        read_is_before_boundry = true;
      }
    }
    if (has_read_req) {
      if ((read_bank_id == 0 && read_is_before_boundry == true) ||
          (read_end_bank_id == 0 && read_is_before_boundry == false)) {
        if (device_dramA_read_resp.read_nb(read_resp)) {
          dram_read_times++;
          if (read_resp.last == true && dram_read_times < read_cmd_num) {
            read_is_before_boundry = false;
            read_resp.last = false;
          } else if (read_resp.last == true &&
                     dram_read_times == read_cmd_num) {
            has_read_req = false;
            read_is_before_boundry = true;
            dram_read_times = 0;
          }
          device_dram_read_resp_throttled.write(read_resp);
        }
      } else if ((read_bank_id == 1 && read_is_before_boundry == true) ||
                 (read_end_bank_id == 1 && read_is_before_boundry == false)) {
        if (device_dramB_read_resp.read_nb(read_resp)) {
          dram_read_times++;
          if (read_resp.last == true && dram_read_times < read_cmd_num) {
            read_is_before_boundry = false;
            read_resp.last = false;
          } else if (read_resp.last == true &&
                     dram_read_times == read_cmd_num) {
            has_read_req = false;
            read_is_before_boundry = true;
            dram_read_times = 0;
          }
          device_dram_read_resp_throttled.write(read_resp);
        }
      } else if ((read_bank_id == 2 && read_is_before_boundry == true) ||
                 (read_end_bank_id == 2 && read_is_before_boundry == false)) {
        if (device_dramC_read_resp.read_nb(read_resp)) {
          dram_read_times++;
          if (read_resp.last == true && dram_read_times < read_cmd_num) {
            read_is_before_boundry = false;
            read_resp.last = false;
          } else if (read_resp.last == true &&
                     dram_read_times == read_cmd_num) {
            has_read_req = false;
            read_is_before_boundry = true;
            dram_read_times = 0;
          }
          device_dram_read_resp_throttled.write(read_resp);
        }
      } else if ((read_bank_id == 3 && read_is_before_boundry == true) ||
                 (read_end_bank_id == 3 && read_is_before_boundry == false)) {
        if (device_dramD_read_resp.read_nb(read_resp)) {
          dram_read_times++;
          if (read_resp.last == true && dram_read_times < read_cmd_num) {
            read_is_before_boundry = false;
            read_resp.last = false;
          } else if (read_resp.last == true &&
                     dram_read_times == read_cmd_num) {
            has_read_req = false;
            read_is_before_boundry = true;
            dram_read_times = 0;
          }
          device_dram_read_resp_throttled.write(read_resp);
        }
      }
    }
    Dram_Write_Req_Data write_data;
    Dram_Dispatcher_Write_Req write_cmd;

    if (!has_write_req) {
      if (device_dram_dispatcher_write_context.read_nb(write_cmd)) {
        write_bank_id = write_cmd.bank_id;
        write_end_bank_id = write_cmd.end_bank_id;
        before_boundry_len = write_cmd.before_boundry_num;
        write_cmd_num = write_cmd.cmd_num;
        has_write_req = true;
        dram_write_times = 0;
        write_is_before_boundry = true;
      }
    }
    if (has_write_req) {
      if (device_dram_write_req_data_throttled.read_nb(write_data)) {
        dram_write_times++;
        if ((write_bank_id == 0 && write_is_before_boundry == true) ||
            (write_end_bank_id == 0 && write_is_before_boundry == false)) {
          if (write_data.last == true && dram_write_times == write_cmd_num) {
            has_write_req = false;
            write_is_before_boundry = true;
            dram_write_times = 0;
          } else if (write_is_before_boundry &&
                     dram_write_times == before_boundry_len) {
            write_data.last = true;
            write_is_before_boundry = false;
          }
          device_dramA_write_req_data.write(write_data);
        } else if ((write_bank_id == 1 && write_is_before_boundry == true) ||
                   (write_end_bank_id == 1 &&
                    write_is_before_boundry == false)) {
          if (write_data.last == true && dram_write_times == write_cmd_num) {
            has_write_req = false;
            write_is_before_boundry = true;
            dram_write_times = 0;

          } else if (write_is_before_boundry &&
                     dram_write_times == before_boundry_len) {
            write_data.last = true;
            write_is_before_boundry = false;
          }
          device_dramB_write_req_data.write(write_data);
        } else if ((write_bank_id == 2 && write_is_before_boundry == true) ||
                   (write_end_bank_id == 2 &&
                    write_is_before_boundry == false)) {
          if (write_data.last == true && dram_write_times == write_cmd_num) {
            has_write_req = false;
            write_is_before_boundry = true;
            dram_write_times = 0;
          } else if (write_is_before_boundry &&
                     dram_write_times == before_boundry_len) {
            write_data.last = true;
            write_is_before_boundry = false;
          }
          device_dramC_write_req_data.write(write_data);
        } else if ((write_bank_id == 3 && write_is_before_boundry == true) ||
                   (write_end_bank_id == 3 &&
                    write_is_before_boundry == false)) {
          if (write_data.last == true && dram_write_times == write_cmd_num) {
            has_write_req = false;
            write_is_before_boundry = true;
            dram_write_times = 0;
          } else if (write_is_before_boundry &&
                     dram_write_times == before_boundry_len) {
            write_data.last = true;
            write_is_before_boundry = false;
          }
          device_dramD_write_req_data.write(write_data);
        }
      }
    }
  }
}

#endif
