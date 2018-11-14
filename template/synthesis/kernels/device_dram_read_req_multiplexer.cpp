#ifndef DEVICE_DRAM_READ_REQ_MULTIPLEXER_CPP_
#define DEVICE_DRAM_READ_REQ_MULTIPLEXER_CPP_

#include <insider_kernel.h>

void device_dram_read_req_multiplexer(
    ST_Queue<Device_Dram_Context> &device_dram_req_context,
    ST_Queue<Dram_Read_Req> &device_dram_read_req,
    ST_Queue<Dram_Read_Req> &device_dram_read_req_0,
    ST_Queue<Dram_Read_Req> &device_dram_read_req_1,
    ST_Queue<Dram_Read_Req> &device_dram_read_req_2) {
  unsigned char state = 0;

  while (1) {
#pragma HLS pipeline

    Dram_Read_Req dram_read_req;
    if (state == 0) {
      if (device_dram_read_req_0.read_nb(dram_read_req)) {
        device_dram_read_req.write(dram_read_req);
        Device_Dram_Context context;
        context.id = 0;
        context.num = dram_read_req.num;
        device_dram_req_context.write(context);
      } else if (device_dram_read_req_1.read_nb(dram_read_req)) {
        device_dram_read_req.write(dram_read_req);
        Device_Dram_Context context;
        context.id = 1;
        context.num = dram_read_req.num;
        device_dram_req_context.write(context);
      } else if (device_dram_read_req_2.read_nb(dram_read_req)) {
        device_dram_read_req.write(dram_read_req);
        Device_Dram_Context context;
        context.id = 2;
        context.num = dram_read_req.num;
        device_dram_req_context.write(context);
      }
      state = 1;
    } else if (state == 1) {
      if (device_dram_read_req_1.read_nb(dram_read_req)) {
        device_dram_read_req.write(dram_read_req);
        Device_Dram_Context context;
        context.id = 1;
        context.num = dram_read_req.num;
        device_dram_req_context.write(context);
      } else if (device_dram_read_req_2.read_nb(dram_read_req)) {
        device_dram_read_req.write(dram_read_req);
        Device_Dram_Context context;
        context.id = 2;
        context.num = dram_read_req.num;
        device_dram_req_context.write(context);
      } else if (device_dram_read_req_0.read_nb(dram_read_req)) {
        device_dram_read_req.write(dram_read_req);
        Device_Dram_Context context;
        context.id = 0;
        context.num = dram_read_req.num;
        device_dram_req_context.write(context);
      }
      state = 2;
    } else if (state == 2) {
      if (device_dram_read_req_2.read_nb(dram_read_req)) {
        device_dram_read_req.write(dram_read_req);
        Device_Dram_Context context;
        context.id = 2;
        context.num = dram_read_req.num;
        device_dram_req_context.write(context);
      } else if (device_dram_read_req_0.read_nb(dram_read_req)) {
        device_dram_read_req.write(dram_read_req);
        Device_Dram_Context context;
        context.id = 0;
        context.num = dram_read_req.num;
        device_dram_req_context.write(context);
      } else if (device_dram_read_req_1.read_nb(dram_read_req)) {
        device_dram_read_req.write(dram_read_req);
        Device_Dram_Context context;
        context.id = 1;
        context.num = dram_read_req.num;
        device_dram_req_context.write(context);
      }
      state = 0;
    }
  }
}

#endif
