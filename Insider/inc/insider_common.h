#ifndef INSIDER_COMMON_H_
#define INSIDER_COMMON_H_

#include <staccel_type.h>

#define TAG(x) (x << 2)
#define KBUF_ADDR_TAG (4)
#define DMA_DELAY_CYCLE_TAG (5)
#define DMA_THROTTLE_PARAM_TAG (6)
#define DRIVE_DELAY_CYCLE_TAG (7)
#define DRIVE_THROTTLE_PARAM_TAG (8)
#define PRESERVER_CHEAT_TAG (9)
#define DEBUG_TAG (15)
#define APP_NUM (3)
#define APP_FILE_INFO_0_TAG (16)
#define APP_FILE_INFO_1_TAG (17)
#define APP_FILE_INFO_2_TAG (18)
#define APP_BUF_ADDRS_0_TAG (19)
#define APP_BUF_ADDRS_1_TAG (20)
#define APP_BUF_ADDRS_2_TAG (21)
#define APP_FREE_BUF_0_TAG (22)
#define APP_FREE_BUF_1_TAG (23)
#define APP_FREE_BUF_2_TAG (24)
#define APP_INPUT_PARAM_0_TAG (25)
#define APP_INPUT_PARAM_1_TAG (26)
#define APP_INPUT_PARAM_2_TAG (27)
#define APP_RESET_0_TAG (28)
#define APP_RESET_1_TAG (29)
#define APP_RESET_2_TAG (30)
#define READ_CMD_TAG_OFFSET (64)
#define WRITE_CMD_TAG_OFFSET (128)

#define READ_CMD (0)
#define WRITE_CMD (1)
#define HOST_READ_REQ (0)
#define DEVICE_READ_REQ (1)
#define PHYSICAL_SECTOR_SIZE (4096)
#define PHYSICAL_SECTOR_SIZE_LOG2 (12)
#define DRAM_ADDR_BUS_WIDTH (36) // 64 GB
#define APP_BUF_SIZE (2 * 1024 * 1024)
#define APP_BUF_SIZE_LOG2 (21)
#define APP_DRAM_RESP_BUFFER_SIZE (512)
#define APP_DRAM_RESP_BUFFER_SIZE_LOG2 (9)
#define MAX_THROTTLE_CONTIGUOUS_LOG2 (24)
#define DEVICE_REQ_QUEUE_DEPTH (64)
#define DATA_BUS_WIDTH (0x40)
#define DATA_BUS_WIDTH_LOG2 (6)
#define ALLOCATED_BUF_NUM (8)       // should be the power of 2
#define PCIE_WRITE_BATCH_NUM (0x40) // should be the power of 2
#define DRAM_READ_BATCH_NUM (0x40)
#define RESET_CNT (2048)

struct Request {
  unsigned int sector_off;
  unsigned int sector_num;
  unsigned int tag;
  bool rw; // 0 read, 1 write
};

struct Data {
  bool last;
  ap_uint<512> data;
};

struct Dram_Read_Req_With_Time {
  Dram_Read_Req req;
  unsigned long long time;
};

struct Dram_Write_Req_Apply_With_Time {
  Dram_Write_Req_Apply req_apply;
  unsigned long long time;
};

struct Dram_Dispatcher_Write_Req {
  unsigned char bank_id;
  unsigned char end_bank_id;
  unsigned int before_boundry_num;
  unsigned int cmd_num;
};

struct Dram_Dispatcher_Read_Req {
  unsigned char bank_id;
  unsigned char end_bank_id;
  unsigned int cmd_num;
};

struct Device_Dram_Context {
  unsigned char id;
  unsigned char num;
};

struct APP_Data {
  ap_uint<512> data;
  unsigned short len;
  bool eop;
};

struct APP_Data_Meta {
  unsigned int num;
  bool eop;
};

#endif
