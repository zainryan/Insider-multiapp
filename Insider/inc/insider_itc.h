#ifndef INSIDER_ITC_H_
#define INSIDER_ITC_H_

#include <insider_common.h>
#include <iostream>
#include <staccel_itc.h>

#ifdef CSIM
#include <cstdlib>
#include <insider_itc.h>
#include <iostream>
#include <vector>

#define ALLOCATED_BUF_NUM (8)
#define KBUF_SIZE (1 << 22)
#define BUF_METADATA_IDX (1 << 21)
#define VIRT_FILE_FD_0 (0xFFF0)
#define VIRT_FILE_FD_1 (0xFFF1)
#define VIRT_FILE_FD_2 (0xFFF2)

#define COSIM_DRAMA_ADDR_OFFSET (4LL << 32)
#define COSIM_DRAMB_ADDR_OFFSET (8LL << 32)
#define COSIM_DRAMC_ADDR_OFFSET (12LL << 32)
#define COSIM_DRAMD_ADDR_OFFSET (16LL << 32)

#define MAX_PHYSICAL_FILE_SIZE (1 << 30)

int first[APP_NUM];
unsigned long long app_buf_phy_addrs[APP_NUM][ALLOCATED_BUF_NUM];
unsigned int app_bufs_ptr[APP_NUM];
int is_eop[APP_NUM];
int buf_idx[APP_NUM];
int buf_len[APP_NUM];
int file_finish_reading[APP_NUM];

int iopen(int app_id, const char *pathname, int flags);
ssize_t iread(int fd, void *buf, size_t count);
int iclose(int fd);
void set_physical_file(int app_id, unsigned char *buf, size_t count);
bool send_input_param(int fd, unsigned int data);
void user_simulation_function();

static void init_app_bufs_ptr(int app_id) {
  assert(app_id < APP_NUM);
  int app_buf_addrs_tag;

  if (app_id == 0) {
    app_buf_addrs_tag = APP_BUF_ADDRS_0_TAG;
  } else if (app_id == 1) {
    app_buf_addrs_tag = APP_BUF_ADDRS_1_TAG;
  } else if (app_id == 2) {
    app_buf_addrs_tag = APP_BUF_ADDRS_2_TAG;
  }

  for (int i = 0; i < ALLOCATED_BUF_NUM; i++) {
    if (i == 0) {
      app_buf_phy_addrs[app_id][i] = (unsigned long long)app_id << 32;
    } else {
      app_buf_phy_addrs[app_id][i] =
          app_buf_phy_addrs[app_id][i - 1] + (1 << (APP_BUF_SIZE_LOG2 + 1));
    }
    send_control_msg(TAG(app_buf_addrs_tag),
                     app_buf_phy_addrs[app_id][i] >> 32);
    send_control_msg(TAG(app_buf_addrs_tag),
                     app_buf_phy_addrs[app_id][i] & 0xFFFFFFFF);
  }
}

static void init() {
  for (int i = 0; i < APP_NUM; i++) {
    first[i] = 1;
    file_finish_reading[i] = buf_len[i] = buf_idx[i] = is_eop[i] =
        app_bufs_ptr[i] = 0;
    init_app_bufs_ptr(i);
  }
}

void dram_write(unsigned long long addr, unsigned char data) {
  int dram_id = (addr >> 12) & 3;
  int dram_idx = ((addr >> 14) << 12) + (addr & ((1 << 12) - 1));

  if (dram_id == 0) {
    dramA_space_mutex.lock();
    dramA_space[dram_idx] = data;
    dramA_space_mutex.unlock();
  } else if (dram_id == 1) {
    dramB_space_mutex.lock();
    dramB_space[dram_idx] = data;
    dramB_space_mutex.unlock();
  } else if (dram_id == 2) {
    dramC_space_mutex.lock();
    dramC_space[dram_idx] = data;
    dramC_space_mutex.unlock();
  } else if (dram_id == 3) {
    dramD_space_mutex.lock();
    dramD_space[dram_idx] = data;
    dramD_space_mutex.unlock();
  }
}

int iopen(int app_id, const char *pathname, int flags) {
  if (flags != 0) {
    std::cout << "Error in iopen(): flags must be O_RDONLY, i.e. 0"
              << std::endl;
    exit(-1);
  }

  int fd;
  if (app_id == 0) {
    fd = VIRT_FILE_FD_0;
  } else if (app_id == 1) {
    fd = VIRT_FILE_FD_1;
  } else if (app_id == 2) {
    fd = VIRT_FILE_FD_2;
  }
  return fd;
}

static void update_metadata(int app_id) {
  unsigned int metadata = 0, flag = 0;
  unsigned long long metadata_addr, flag_addr;
  volatile unsigned char *metadata_ptr;
  do {
    metadata_addr =
        app_buf_phy_addrs[app_id][app_bufs_ptr[app_id]] + BUF_METADATA_IDX;
    flag_addr = app_buf_phy_addrs[app_id][app_bufs_ptr[app_id]] +
                BUF_METADATA_IDX + sizeof(unsigned int);
    flag = (pcie_space_read(flag_addr + 3) << 24) |
           (pcie_space_read(flag_addr + 2) << 16) |
           (pcie_space_read(flag_addr + 1) << 8) |
           (pcie_space_read(flag_addr + 0) << 0);
    metadata = (pcie_space_read(metadata_addr + 3) << 24) |
               (pcie_space_read(metadata_addr + 2) << 16) |
               (pcie_space_read(metadata_addr + 1) << 8) |
               (pcie_space_read(metadata_addr + 0) << 0);
  } while (!(flag));
  pcie_space_write(flag_addr + 0, 0);
  pcie_space_write(flag_addr + 1, 0);
  pcie_space_write(flag_addr + 2, 0);
  pcie_space_write(flag_addr + 3, 0);
  buf_len[app_id] = metadata >> 1;
  is_eop[app_id] = metadata & 0x1;
}

static void kernel_user_memcpy(void *user_buf,
                               unsigned long long kernel_buf_addr,
                               size_t count) {
  for (size_t i = 0; i < count; i++) {
    ((unsigned char *)user_buf)[i] = pcie_space_read(kernel_buf_addr + i);
  }
}

static void reset(int app_id) {
  app_bufs_ptr[app_id] = is_eop[app_id] = buf_idx[app_id] = buf_len[app_id] = 0;
  first[app_id] = 1;
}

ssize_t iread(int fd, void *buf, size_t count) {
  int app_id;
  int app_free_buf_tag;
  if (fd == VIRT_FILE_FD_0) {
    app_id = 0;
    app_free_buf_tag = APP_FREE_BUF_0_TAG;
  } else if (fd == VIRT_FILE_FD_1) {
    app_id = 1;
    app_free_buf_tag = APP_FREE_BUF_1_TAG;
  } else if (fd == VIRT_FILE_FD_2) {
    app_id = 2;
    app_free_buf_tag = APP_FREE_BUF_2_TAG;
  } else {
    return -1;
  }

  if (file_finish_reading[app_id]) {
    return 0;
  } else if (first[app_id]) {
    update_metadata(app_id);
    first[app_id] = 0;
  }
  unsigned long long kbuf_addr =
      app_buf_phy_addrs[app_id][app_bufs_ptr[app_id]];
  ssize_t read_size;
  if (count >= buf_len[app_id] - buf_idx[app_id]) {
    read_size = buf_len[app_id] - buf_idx[app_id];
    if (is_eop[app_id]) {
      kernel_user_memcpy(buf, kbuf_addr + buf_idx[app_id], read_size);
      file_finish_reading[app_id] = 1;
      reset(app_id);
    } else {
      kernel_user_memcpy(buf, kbuf_addr + buf_idx[app_id], read_size);
      send_control_msg(TAG(app_free_buf_tag), 0);
      app_bufs_ptr[app_id] =
          (app_bufs_ptr[app_id] + 1) & (ALLOCATED_BUF_NUM - 1);
      buf_idx[app_id] = 0;
      update_metadata(app_id);
    }
  } else {
    read_size = count;
    kernel_user_memcpy(buf, kbuf_addr + buf_idx[app_id], read_size);
    buf_idx[app_id] += read_size;
  }
  return read_size;
}

int iclose(int fd) {
  int app_id;
  int app_reset_tag;
  if (fd == VIRT_FILE_FD_0) {
    app_id = 0;
    app_reset_tag = APP_RESET_0_TAG;
  } else if (fd == VIRT_FILE_FD_1) {
    app_id = 1;
    app_reset_tag = APP_RESET_1_TAG;
  } else if (fd == VIRT_FILE_FD_2) {
    app_id = 2;
    app_reset_tag = APP_RESET_2_TAG;
  } else {
    return -1;
  }

  send_control_msg(TAG(app_reset_tag), 0);
  file_finish_reading[app_id] = 0;
  return 0;
}

void set_physical_file(int app_id, unsigned char *buf, size_t count) {
  if (count > MAX_PHYSICAL_FILE_SIZE) {
    std::cerr << "In set_physical_file(), count is larger than "
                 "MAX_PHYSICAL_FILE_SIZE."
              << std::endl;
    return;
  }
  int app_file_info_tag;
  if (app_id == 0) {
    app_file_info_tag = APP_FILE_INFO_0_TAG;
  } else if (app_id == 1) {
    app_file_info_tag = APP_FILE_INFO_1_TAG;
  } else if (app_id == 2) {
    app_file_info_tag = APP_FILE_INFO_2_TAG;
  } else {
    return;
  }

  unsigned long long base_addr =
      (unsigned long long)app_id * MAX_PHYSICAL_FILE_SIZE;
  for (size_t i = 0; i < count; i++) {
    dram_write(i + base_addr, buf[i]);
  }
  send_control_msg(TAG(app_file_info_tag), 1);
  send_control_msg(TAG(app_file_info_tag), count >> 32);
  send_control_msg(TAG(app_file_info_tag), count & 0xFFFFFFFF);
  send_control_msg(TAG(app_file_info_tag), base_addr >> 32);
  send_control_msg(TAG(app_file_info_tag), base_addr & 0xFFFFFFFF);
  send_control_msg(TAG(app_file_info_tag), count >> 32);
  send_control_msg(TAG(app_file_info_tag), count & 0xFFFFFFFF);
}

static void simulator() {
  init();
  user_simulation_function();
}

bool send_input_param(int fd, unsigned int data) {
  int app_input_param_tag;
  if (fd == VIRT_FILE_FD_0) {
    app_input_param_tag = APP_INPUT_PARAM_0_TAG;
  } else if (fd == VIRT_FILE_FD_1) {
    app_input_param_tag = APP_INPUT_PARAM_1_TAG;
  } else if (fd == VIRT_FILE_FD_2) {
    app_input_param_tag = APP_INPUT_PARAM_2_TAG;
  } else {
    return false;
  }

  send_control_msg(TAG(app_input_param_tag), data);
  return true;
}
#endif

#ifdef INSIDER_COMPILER
ST_Queue<APP_Data> app_input_data(1);
ST_Queue<APP_Data> app_output_data(1);
ST_Queue<unsigned int> app_input_params(1);
#endif

#endif
