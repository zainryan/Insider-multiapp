#define _GNU_SOURCE
#include <dlfcn.h>
#include <fcntl.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define MAX_CMD_LEN (4096)
#define MAX_PATH_LEN (4096)
#define MAX_CMD_OUTPUT_LEN (4096)
#define PAGE_SIZE (1 << 22)
#define MMIO_SPACE_SIZE (1 << 25)
#define ALLOCATED_BUF_NUM (8)
#define BUF_METADATA_IDX (1 << 21)
#define MAX_EXTENT_NUM (32)
#define PHYSICAL_SECTOR_SIZE (4096)

#define APP_NUM (3)
#define VIRT_FILE_FD_0 (0xFFF0)
#define VIRT_FILE_FD_1 (0xFFF1)
#define VIRT_FILE_FD_2 (0xFFF2)

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

#ifndef PAR_MEMCPY_WORKERS
#define PAR_MEMCPY_WORKERS (1)
#endif

const char DEVICE_NAME[] = "/dev/nvme_fpga";
const char MAPPINGS_FILE[] = ".virt_file_mappings";

const char FILEFRAG_CMD[] = "filefrag -v ";
const char FILEFRAG_FILTER_CMD[] =
    " | cut -d \":\" -f 3,4 | awk 'NR > 3' | sed \"s/.* "
    "\\([0-9]\\+\\)\\.\\..*:\\(.*\\)/\\1 \\2/g\"";
const char DF_CMD[] = "df ";
const char DF_FILTER_CMD[] = " | sed \"2, 2p\" -n | awk '{print $1}'";
const char LS_CMD[] = "ls -l ";
const char LS_FILTER_CMD[] = " | awk '{print $5}'";
const char TOUCH_CMD[] = "touch ";

int mmio_fd;
void *mmio_space;

void *app_bufs[APP_NUM][ALLOCATED_BUF_NUM];
int app_buf_fds[APP_NUM][ALLOCATED_BUF_NUM];
unsigned long long app_buf_phy_addrs[APP_NUM][ALLOCATED_BUF_NUM];

int app_bufs_ptr[APP_NUM];
int is_eop[APP_NUM];
int buf_idx[APP_NUM];
int buf_len[APP_NUM];
int file_finish_reading[APP_NUM];
int first[APP_NUM];

int send_input_params(int fd, unsigned int data) {
  int app_input_param_tag;
  if (fd == VIRT_FILE_FD_0) {
    app_input_param_tag = APP_INPUT_PARAM_0_TAG;
  } else if (fd == VIRT_FILE_FD_1) {
    app_input_param_tag = APP_INPUT_PARAM_1_TAG;
  } else if (fd == VIRT_FILE_FD_2) {
    app_input_param_tag = APP_INPUT_PARAM_2_TAG;
  } else {
    return -1;
  }
  *((volatile unsigned int *)mmio_space + app_input_param_tag) = data;
  return 0;
}

int send_input_params_array(int fd, unsigned int *data_arr, size_t arr_len) {
  int app_input_param_tag;
  if (fd == VIRT_FILE_FD_0) {
    app_input_param_tag = APP_INPUT_PARAM_0_TAG;
  } else if (fd == VIRT_FILE_FD_1) {
    app_input_param_tag = APP_INPUT_PARAM_1_TAG;
  } else if (fd == VIRT_FILE_FD_2) {
    app_input_param_tag = APP_INPUT_PARAM_2_TAG;
  } else {
    return -1;
  }
  int i = 0;
  for (i = 0; i < arr_len; i++) {
    *((volatile unsigned int *)mmio_space + app_input_param_tag) = data_arr[i];
  }
  return 0;
}

__inline__ static void send_control_msg(int tag, unsigned int data) {
  *((volatile unsigned int *)mmio_space + tag) = data;
}

static void get_file_info(const char *real_file_name, unsigned int *num_extents,
                          unsigned int *extents_start_arr,
                          unsigned int *extents_len_arr,
                          unsigned long long *file_size) {
  FILE *fp;
  char *buf = malloc(MAX_CMD_OUTPUT_LEN);
  char *cmd = malloc(MAX_CMD_LEN);

  // get file extents
  cmd[0] = 0;
  strcat(cmd, FILEFRAG_CMD);
  strcat(cmd, real_file_name);
  strcat(cmd, FILEFRAG_FILTER_CMD);
  unsigned int start, len;
  fp = popen(cmd, "r");
  fread(buf, 1, MAX_CMD_OUTPUT_LEN, fp);
  *num_extents = 0;
  char *filefrag_output = buf;
  while (sscanf(filefrag_output, "%u %u", &start, &len) > 0) {
    extents_start_arr[*num_extents] = start;
    extents_len_arr[*num_extents] = len;
    (*num_extents)++;
    filefrag_output = strstr(filefrag_output, "\n");
    if (filefrag_output) {
      filefrag_output++;
    } else {
      break;
    }
  }
  if (fp)
    pclose(fp);

  // get file size
  cmd[0] = 0;
  strcat(cmd, LS_CMD);
  strcat(cmd, real_file_name);
  strcat(cmd, LS_FILTER_CMD);
  fp = popen(cmd, "r");
  fgets(buf, MAX_CMD_OUTPUT_LEN, fp);
  sscanf(buf, "%llu", file_size);
  if (fp)
    pclose(fp);

  free(buf);
  free(cmd);
}

static int is_from_nvme_fpga(const char *pathname) {
  char *cmd = malloc(sizeof(DF_CMD) + MAX_PATH_LEN);
  cmd[0] = 0;
  strcat(cmd, DF_CMD);
  strcat(cmd, pathname);
  strcat(cmd, DF_FILTER_CMD);
  FILE *fp = popen(cmd, "r");
  char *buf = malloc(MAX_CMD_OUTPUT_LEN);
  fgets(buf, MAX_CMD_OUTPUT_LEN, fp);
  free(cmd);
  pclose(fp);
  return !strncmp(buf, DEVICE_NAME, strlen(buf) - 1);
  return 0;
}

static int is_registered(const char *pathname, unsigned int *num_extents,
                         unsigned int *extents_start_arr,
                         unsigned int *extents_len_arr,
                         unsigned long long *file_size,
			 int *app_id) {
  const char *pos = strrchr(pathname, '/');
  char *mappings_path = malloc(MAX_PATH_LEN);
  char *relative_pathname = malloc(MAX_PATH_LEN);
  char *virt_file_name = malloc(MAX_PATH_LEN);
  char *real_file_name = malloc(MAX_PATH_LEN);
  strcpy(relative_pathname, pos + 1);
  size_t prefix_len = (pos == NULL) ? 0 : pos - pathname + 1;
  strncpy(mappings_path, pathname, prefix_len);
  strncpy(mappings_path + prefix_len, MAPPINGS_FILE, sizeof(MAPPINGS_FILE));
  FILE *fp = fopen(mappings_path, "r");
  int ret = 0;
  *num_extents = 0;
  if (fp) {
    while (fscanf(fp, "%s %s %d", real_file_name, virt_file_name, app_id) != EOF) {
      if (!strcmp(virt_file_name, relative_pathname)) {
        ret = 1;
        memmove(real_file_name + prefix_len, real_file_name,
                strlen(real_file_name) + 1);
        memmove(real_file_name, pathname, prefix_len);
        get_file_info(real_file_name, num_extents, extents_start_arr,
                      extents_len_arr, file_size);
        break;
      }
    }
  }
  free(mappings_path);
  free(relative_pathname);
  free(real_file_name);
  free(virt_file_name);
  if (fp)
    fclose(fp);
  return ret;
}

static int is_virtual_file(const char *pathname, unsigned int *num_extents,
                           unsigned int *extents_start_arr,
                           unsigned int *extents_len_arr,
                           unsigned long long *file_size,
			   int *app_id) {
  return is_from_nvme_fpga(pathname) &&
             is_registered(pathname, num_extents, extents_start_arr,
                       extents_len_arr, file_size, app_id);
}

static void *allocate_kernel_buf(int *configfd) {
  void *address;
  *configfd = open("/dev/fpga_dma", O_RDWR);
  if (*configfd < 0) {
    perror("Error in dma driver.");
    exit(-1);
  }
  address =
      mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *configfd, 0);
  if (address == MAP_FAILED) {
    perror("Mmap operation failed.");
    exit(-1);
  }
  return address;
}

static void setup_mmio(void) {
  mmio_fd = open("/sys/devices/pci0000:00/0000:00:1d.0/resource0", O_RDWR);
  if (mmio_fd < 0) {
    perror("Error for mmapping the mmio region,");
  }
  mmio_space = mmap(NULL, MMIO_SPACE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                    mmio_fd, 0);
  if (mmio_space == MAP_FAILED) {
    perror("Mmap operation failed.");
    exit(-1);
  }
}

__inline__ static void update_metadata(int app_id) {
  unsigned int metadata = 0, flag = 0;
  volatile unsigned char *flag_ptr;
  volatile unsigned char *metadata_ptr;
  do {
    metadata_ptr =
        (volatile unsigned char *)(app_bufs[app_id][app_bufs_ptr[app_id]] +
                                   BUF_METADATA_IDX);
    flag_ptr =
        (volatile unsigned char *)(app_bufs[app_id][app_bufs_ptr[app_id]] +
                                   BUF_METADATA_IDX + sizeof(unsigned int));
    flag = ((*(flag_ptr + 3)) << 24) | ((*(flag_ptr + 2)) << 16) |
           ((*(flag_ptr + 1)) << 8) | ((*(flag_ptr + 0)) << 0);
    metadata = ((*(metadata_ptr + 3)) << 24) | ((*(metadata_ptr + 2)) << 16) |
               ((*(metadata_ptr + 1)) << 8) | ((*(metadata_ptr + 0)) << 0);
  } while (!(flag));
  *flag_ptr = *(flag_ptr + 1) = *(flag_ptr + 2) = *(flag_ptr + 3) = 0;
  buf_len[app_id] = metadata >> 1;
  is_eop[app_id] = metadata & 0x1;
}

const char *reg_virt_file(int app_id, const char *real_path) {
  if (!is_from_nvme_fpga(real_path)) {
    return NULL;
  }
  const char *pos = strrchr(real_path, '/');
  char *mappings_path = malloc(MAX_PATH_LEN);
  char *relative_real_path = malloc(MAX_PATH_LEN);
  strcpy(relative_real_path, pos + 1);
  size_t prefix_len = (pos == NULL) ? 0 : pos - real_path + 1;
  strncpy(mappings_path, real_path, prefix_len);
  strncpy(mappings_path + prefix_len, MAPPINGS_FILE, sizeof(MAPPINGS_FILE));
  FILE *fp = fopen(mappings_path, "a");
  char *virt_file_name = malloc(MAX_PATH_LEN);
  char *absolute_virt_file_path = malloc(MAX_PATH_LEN);
  virt_file_name[0] = 0;
  strcat(virt_file_name, "virt_");
  strcat(virt_file_name, relative_real_path);
  while (1) {
    strncpy(absolute_virt_file_path, real_path, prefix_len);
    absolute_virt_file_path[prefix_len] = '\0';
    strcat(absolute_virt_file_path, virt_file_name);
    if (access(absolute_virt_file_path, F_OK)) {
      break;
    }
    strcat(virt_file_name, "_");
  }
  // update metadata
  fprintf(fp, "%s %s %d\n", relative_real_path, virt_file_name, app_id);
  // touch virtual file
  FILE *cmd_fp;
  char *cmd = malloc(MAX_CMD_LEN);
  cmd[0] = 0;
  strcat(cmd, TOUCH_CMD);
  strcat(cmd, absolute_virt_file_path);
  cmd_fp = popen(cmd, "r");

  free(mappings_path);
  free(relative_real_path);
  free(virt_file_name);
  if (fp)
    fclose(fp);
  if (cmd_fp)
    pclose(cmd_fp);
  return absolute_virt_file_path;
}

int iopen(const char *pathname, int flags) {
  unsigned int num_extents;
  unsigned int *extents_start_arr =
      malloc(sizeof(unsigned int) * MAX_EXTENT_NUM);
  unsigned int *extents_len_arr = malloc(sizeof(unsigned int) * MAX_EXTENT_NUM);
  unsigned long long length;
  long long offset;
  int app_id;

  if (!is_virtual_file(pathname, &num_extents, extents_start_arr,
                       extents_len_arr, &length, &app_id)) {
    return -1;
  } else {
    int app_file_info_tag, app_buf_addrs_tag;
    if (app_id == 0) {
      app_file_info_tag = APP_FILE_INFO_0_TAG;
      app_buf_addrs_tag = APP_BUF_ADDRS_0_TAG;
    } else if (app_id == 1) {
      app_file_info_tag = APP_FILE_INFO_1_TAG;
      app_buf_addrs_tag = APP_BUF_ADDRS_1_TAG;
    } else if (app_id == 2) {
      app_file_info_tag = APP_FILE_INFO_2_TAG;
      app_buf_addrs_tag = APP_BUF_ADDRS_2_TAG;
    } else {
      return -1;
    }

    int i;
    setup_mmio();
    for (i = 0; i < ALLOCATED_BUF_NUM; i++) {
      app_bufs[app_id][i] = allocate_kernel_buf(&app_buf_fds[app_id][i]);
      app_buf_phy_addrs[app_id][i] =
          *((unsigned long long *)app_bufs[app_id][i]);
      send_control_msg(app_buf_addrs_tag, app_buf_phy_addrs[app_id][i] >> 32);
      send_control_msg(app_buf_addrs_tag,
                       app_buf_phy_addrs[app_id][i] & 0xFFFFFFFF);
    }
    send_control_msg(app_file_info_tag, num_extents);
    send_control_msg(app_file_info_tag, length >> 32);
    send_control_msg(app_file_info_tag, length & 0xFFFFFFFF);
    offset = length;
    for (i = 0; i < num_extents; i++) {
      unsigned long long extents_start_in_byte =
          ((unsigned long long)extents_start_arr[i]) * PHYSICAL_SECTOR_SIZE;
      send_control_msg(app_file_info_tag, extents_start_in_byte >> 32);
      send_control_msg(app_file_info_tag, extents_start_in_byte & 0xFFFFFFFF);
      unsigned long long extents_len_in_byte =
          ((unsigned long long)extents_len_arr[i]) * PHYSICAL_SECTOR_SIZE;
      offset -= extents_len_in_byte;
      extents_len_in_byte += (offset > 0) ? 0 : offset;
      send_control_msg(app_file_info_tag, extents_len_in_byte >> 32);
      send_control_msg(app_file_info_tag, extents_len_in_byte & 0xFFFFFFFF);
    }
  }
  app_bufs_ptr[app_id] = is_eop[app_id] = buf_idx[app_id] = buf_len[app_id] =
      file_finish_reading[app_id] = 0;
  first[app_id] = 1;

  int fd = 0;
  if (app_id == 0) {
    fd = VIRT_FILE_FD_0;
  } else if (app_id == 1) {
    fd = VIRT_FILE_FD_1;
  } else if (app_id == 2) {
    fd = VIRT_FILE_FD_2;
  }
  return fd;
}

void reset(int app_id) {
  app_bufs_ptr[app_id] = is_eop[app_id] = buf_idx[app_id] = buf_len[app_id] = 0;
  first[app_id] = 1;
}

void parallel_memcpy(void *dest, const void *src, size_t n) {
  int size_per_worker = (n + PAR_MEMCPY_WORKERS - 1) / PAR_MEMCPY_WORKERS;
  int size_last = n - size_per_worker * (PAR_MEMCPY_WORKERS - 1);
#pragma omp parallel num_threads(PAR_MEMCPY_WORKERS)
  {
    int tid = omp_get_thread_num();
    int copy_size =
        (tid != (PAR_MEMCPY_WORKERS - 1)) ? size_per_worker : size_last;
    memcpy((unsigned char *)dest + size_per_worker * tid,
           (unsigned char *)src + size_per_worker * tid, copy_size);
  }
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
  unsigned char *app_buf =
      (unsigned char *)app_bufs[app_id][app_bufs_ptr[app_id]];
  ssize_t read_size;
  if (count >= buf_len[app_id] - buf_idx[app_id]) {
    read_size = buf_len[app_id] - buf_idx[app_id];
    if (is_eop[app_id]) {
      parallel_memcpy(buf, app_buf + buf_idx[app_id], read_size);
      file_finish_reading[app_id] = 1;
      reset(app_id);
    } else {
      parallel_memcpy(buf, app_buf + buf_idx[app_id], read_size);
      send_control_msg(app_free_buf_tag, 0);
      app_bufs_ptr[app_id] =
          (app_bufs_ptr[app_id] + 1) & (ALLOCATED_BUF_NUM - 1);
      buf_idx[app_id] = 0;
      update_metadata(app_id);
    }
  } else {
    read_size = count;
    parallel_memcpy(buf, app_buf + buf_idx[app_id], read_size);
    buf_idx[app_id] += read_size;
  }
  return read_size;
}

int iclose(int fd) {
  int app_id, app_reset_tag;
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

  int i;
  for (i = 0; i < ALLOCATED_BUF_NUM; i++) {
    if (app_buf_fds[app_id][i] > 0) {
      close(app_buf_fds[app_id][i]);
    }
  }
  if (mmio_fd) {
    close(mmio_fd);
  }
  send_control_msg(app_reset_tag, 0);
  return 0;
}
