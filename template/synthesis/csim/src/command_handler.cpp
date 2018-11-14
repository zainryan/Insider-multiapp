#ifndef COMMAND_HANDLER_CPP_
#define COMMAND_HANDLER_CPP_

#include <insider_kernel.h>

void command_handler(
    ST_Queue<Poke_Info> &poke, ST_Queue<Request> &reqs_incoming,
    ST_Queue<unsigned int> &kbuf_addrs,
    ST_Queue<unsigned int> &dma_read_throttle_params,
    ST_Queue<unsigned int> &dma_write_throttle_params,
    ST_Queue<unsigned int> &device_read_delay_cycle_cnts,
    ST_Queue<unsigned int> &device_write_delay_cycle_cnts,
    ST_Queue<unsigned int> &drive_read_throttle_params,
    ST_Queue<unsigned int> &drive_write_throttle_params,
    ST_Queue<unsigned int> &app_file_infos_0,
    ST_Queue<unsigned int> &app_file_infos_1,
    ST_Queue<unsigned int> &app_file_infos_2,
    ST_Queue<unsigned int> &app_buf_addrs_0,
    ST_Queue<unsigned int> &app_buf_addrs_1,
    ST_Queue<unsigned int> &app_buf_addrs_2, ST_Queue<bool> &app_free_buf_0,
    ST_Queue<bool> &app_free_buf_1, ST_Queue<bool> &app_free_buf_2,
    ST_Queue<unsigned int> &app_input_params_0,
    ST_Queue<unsigned int> &app_input_params_1,
    ST_Queue<unsigned int> &app_input_params_2, ST_Queue<bool> &reset_sigs_0,
    ST_Queue<bool> &reset_sigs_1, ST_Queue<bool> &reset_sigs_2) {
  unsigned int tag = 0;
  Request req;
  bool is_read_host_throttle_params = true;
  bool is_read_device_throttle_params = true;

  while (1) {
#pragma HLS pipeline
    Poke_Info info;
    int state = 0;
    if (poke.read_nb(info)) {
      if (info.tag >= TAG(WRITE_CMD_TAG_OFFSET)) {
        state = 1;
      } else if (info.tag >= TAG(READ_CMD_TAG_OFFSET)) {
        state = 2;
      } else if (info.tag == TAG(KBUF_ADDR_TAG)) {
        state = 3;
      } else if (info.tag == TAG(DMA_THROTTLE_PARAM_TAG)) {
        state = 5;
      } else if (info.tag == TAG(DRIVE_DELAY_CYCLE_TAG)) {
        state = 6;
      } else if (info.tag == TAG(DRIVE_THROTTLE_PARAM_TAG)) {
        state = 7;
      } else if (info.tag == TAG(APP_FILE_INFO_0_TAG)) {
        state = 14;
      } else if (info.tag == TAG(APP_FILE_INFO_1_TAG)) {
        state = 15;
      } else if (info.tag == TAG(APP_FILE_INFO_2_TAG)) {
        state = 16;
      } else if (info.tag == TAG(APP_BUF_ADDRS_0_TAG)) {
        state = 17;
      } else if (info.tag == TAG(APP_BUF_ADDRS_1_TAG)) {
        state = 18;
      } else if (info.tag == TAG(APP_BUF_ADDRS_2_TAG)) {
        state = 19;
      } else if (info.tag == TAG(APP_FREE_BUF_0_TAG)) {
        state = 20;
      } else if (info.tag == TAG(APP_FREE_BUF_1_TAG)) {
        state = 21;
      } else if (info.tag == TAG(APP_FREE_BUF_2_TAG)) {
        state = 22;
      } else if (info.tag == TAG(APP_INPUT_PARAM_0_TAG)) {
        state = 23;
      } else if (info.tag == TAG(APP_INPUT_PARAM_1_TAG)) {
        state = 24;
      } else if (info.tag == TAG(APP_INPUT_PARAM_2_TAG)) {
        state = 25;
      } else if (info.tag == TAG(APP_RESET_0_TAG)) {
        state = 26;
      } else if (info.tag == TAG(APP_RESET_1_TAG)) {
        state = 27;
      } else if (info.tag == TAG(APP_RESET_2_TAG)) {
        state = 28;
      }
    }

    if (state == 0) {
      // do nothing
    } else if (state == 1) {
      Request req;
      req.sector_off = info.data >> 3;
      req.sector_num = (info.data & 7) + 1;
      req.rw = WRITE_CMD;
      req.tag = (info.tag >> 2) - WRITE_CMD_TAG_OFFSET;
      reqs_incoming.write(req);
    } else if (state == 2) {
      Request req;
      req.sector_off = info.data >> 3;
      req.sector_num = (info.data & 7) + 1;
      req.rw = READ_CMD;
      req.tag = (info.tag >> 2) - READ_CMD_TAG_OFFSET;
      reqs_incoming.write(req);
    } else if (state == 3) {
      kbuf_addrs.write(info.data);
    } else if (state == 5) {
      if (is_read_host_throttle_params) {
        is_read_host_throttle_params = 0;
        dma_read_throttle_params.write(info.data);
      } else {
        is_read_host_throttle_params = 1;
        dma_write_throttle_params.write(info.data);
      }
    } else if (state == 6) {
      device_read_delay_cycle_cnts.write(info.data);
      device_write_delay_cycle_cnts.write(info.data);
    } else if (state == 7) {
      if (is_read_device_throttle_params) {
        is_read_device_throttle_params = 0;
        drive_read_throttle_params.write(info.data);
      } else {
        is_read_device_throttle_params = 1;
        drive_write_throttle_params.write(info.data);
      }
    } else if (state == 14) {
      app_file_infos_0.write(info.data);
    } else if (state == 15) {
      app_file_infos_1.write(info.data);
    } else if (state == 16) {
      app_file_infos_2.write(info.data);
    } else if (state == 17) {
      app_buf_addrs_0.write(info.data);
    } else if (state == 18) {
      app_buf_addrs_1.write(info.data);
    } else if (state == 19) {
      app_buf_addrs_2.write(info.data);
    } else if (state == 20) {
      app_free_buf_0.write(0);
    } else if (state == 21) {
      app_free_buf_1.write(0);
    } else if (state == 22) {
      app_free_buf_2.write(0);
    } else if (state == 23) {
      app_input_params_0.write(info.data);
    } else if (state == 24) {
      app_input_params_1.write(info.data);
    } else if (state == 25) {
      app_input_params_2.write(info.data);
    } else if (state == 26) {
      reset_sigs_0.write(0);
    } else if (state == 27) {
      reset_sigs_1.write(0);
    } else if (state == 28) {
      reset_sigs_2.write(0);
    }
  }
}

#endif
