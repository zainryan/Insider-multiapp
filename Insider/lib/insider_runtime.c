#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <omp.h>

#define MAX_CMD_LEN (4096)
#define MAX_PATH_LEN (4096)
#define MAX_CMD_OUTPUT_LEN (4096)
#define PAGE_SIZE (1 << 22)
#define MMIO_SPACE_SIZE (1 << 25)
#define ALLOCATED_BUF_NUM (8)
#define VIRT_FILE_FD (0xFFFF)
#define BUF_METADATA_IDX (1 << 21)
#define MAX_EXTENT_NUM (32)
#define PHYSICAL_SECTOR_SIZE (4096)

#define APP_FILE_INFO_TAG (10)
#define APP_BUF_ADDRS_TAG (11)
#define APP_FREE_BUF_TAG (12)
#define APP_INPUT_PARAM_TAG (13)
#define RESET_TAG (14)

#ifndef PAR_MEMCPY_WORKERS
#define PAR_MEMCPY_WORKERS (1)
#endif

const char DEVICE_NAME[] = "/dev/nvme_fpga";
const char MAPPINGS_FILE[] = ".virt_file_mappings";

const char FILEFRAG_CMD[] = "filefrag -v ";
const char FILEFRAG_FILTER_CMD[] = " | cut -d \":\" -f 3,4 | awk 'NR > 3' | sed \"s/.* \\([0-9]\\+\\)\\.\\..*:\\(.*\\)/\\1 \\2/g\"";
const char DF_CMD[] = "df ";
const char DF_FILTER_CMD[] = " | sed \"2, 2p\" -n | awk '{print $1}'";
const char LS_CMD[] = "ls -l ";
const char LS_FILTER_CMD[] = " | awk '{print $5}'";
const char TOUCH_CMD[] = "touch ";

int mmio_fd;
void *mmio_space;

void *app_bufs[ALLOCATED_BUF_NUM];
int app_buf_fds[ALLOCATED_BUF_NUM];
unsigned long long app_buf_phy_addrs[ALLOCATED_BUF_NUM];

int app_bufs_ptr = 0;
int is_eop = 0;
int buf_idx = 0;
int buf_len = 0;
int file_finish_reading = 0;
int first = 1;

void send_input_params(unsigned int data) {
  *((volatile unsigned int *)mmio_space + APP_INPUT_PARAM_TAG) = data;
}

void send_input_params_array(unsigned int *data_arr, size_t arr_len) {
  int i = 0;
  for (i = 0; i < arr_len; i ++) {
    *((volatile unsigned int *)mmio_space + APP_INPUT_PARAM_TAG) = data_arr[i];
  }
}

__inline__ static void send_control_msg(int tag, unsigned int data) {
  *((volatile unsigned int *)mmio_space + tag) = data;
}

static void get_file_info(const char *real_file_name, 
			     unsigned int *num_extents, 
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
    (*num_extents) ++;
    filefrag_output = strstr(filefrag_output, "\n");
    if (filefrag_output) {
      filefrag_output ++;
    }
    else {
      break;
    }
  }
  if (fp) pclose(fp);

  // get file size
  cmd[0] = 0;
  strcat(cmd, LS_CMD);
  strcat(cmd, real_file_name);
  strcat(cmd, LS_FILTER_CMD);
  fp = popen(cmd, "r");
  fgets(buf, MAX_CMD_OUTPUT_LEN, fp);
  sscanf(buf, "%llu", file_size);
  if (fp) pclose(fp);
  
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

static int is_registered(const char *pathname,
			 unsigned int *num_extents,
			 unsigned int *extents_start_arr,
			 unsigned int *extents_len_arr,
			 unsigned long long *file_size
		  ) {
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
    while (fscanf(fp, "%s %s", real_file_name, virt_file_name) != EOF) {
      if (!strcmp(virt_file_name, relative_pathname)) {
	ret = 1;
	memmove(real_file_name + prefix_len, real_file_name, strlen(real_file_name) + 1);
	memmove(real_file_name, pathname, prefix_len);
	get_file_info(real_file_name, num_extents, extents_start_arr, extents_len_arr, file_size);
	break;
      }
    }
  }
  free(mappings_path);
  free(relative_pathname);
  free(real_file_name);
  free(virt_file_name);
  if (fp) fclose(fp);
  return ret;
}

static int is_virtual_file(const char *pathname, 
			   unsigned int *num_extents,
			   unsigned int *extents_start_arr,
			   unsigned int *extents_len_arr,
			   unsigned long long *file_size
			   ) {
  return is_from_nvme_fpga(pathname) && is_registered(pathname, num_extents, extents_start_arr, extents_len_arr, file_size);
}

static void *allocate_kernel_buf(int *configfd) {
  void *address;
  *configfd = open("/dev/fpga_dma", O_RDWR);
  if (*configfd < 0) {
    perror("Error in dma driver.");
    exit(-1);
  }
  address = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *configfd, 0);
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
  mmio_space = mmap(NULL, MMIO_SPACE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mmio_fd, 0);
  if (mmio_space == MAP_FAILED) {
    perror("Mmap operation failed.");
    exit(-1);
  }
}

__inline__ static void update_metadata(void) {
  unsigned int metadata = 0, flag = 0;
  volatile unsigned char *flag_ptr;
  volatile unsigned char *metadata_ptr;
  do {
    metadata_ptr = (volatile unsigned char *)(app_bufs[app_bufs_ptr] + BUF_METADATA_IDX);
    flag_ptr = (volatile unsigned char *)(app_bufs[app_bufs_ptr] + BUF_METADATA_IDX + sizeof(unsigned int));
    flag = 
      ((*(flag_ptr + 3)) << 24) |
      ((*(flag_ptr + 2)) << 16) |
      ((*(flag_ptr + 1)) << 8)  |
      ((*(flag_ptr + 0)) << 0);
    metadata = 
      ((*(metadata_ptr + 3)) << 24) |
      ((*(metadata_ptr + 2)) << 16) |
      ((*(metadata_ptr + 1)) << 8)  |
      ((*(metadata_ptr + 0)) << 0);
  }
  while (!(flag));
  *flag_ptr = *(flag_ptr + 1) = *(flag_ptr + 2) = *(flag_ptr + 3) = 0;
  buf_len = metadata >> 1;
  is_eop = metadata & 0x1;
}

const char *reg_virt_file(const char *real_path) {
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
  fprintf(fp, "%s %s\n", relative_real_path, virt_file_name);
  // touch virtual file
  FILE* cmd_fp;
  char *cmd = malloc(MAX_CMD_LEN);
  cmd[0] = 0;
  strcat(cmd, TOUCH_CMD);
  strcat(cmd, absolute_virt_file_path);
  cmd_fp = popen(cmd, "r");

  free(mappings_path);
  free(relative_real_path);
  free(virt_file_name);
  if (fp) fclose(fp);
  if (cmd_fp) pclose(cmd_fp);
  return absolute_virt_file_path;
}

int iopen(const char *pathname, int flags) {
  unsigned int num_extents;
  unsigned int *extents_start_arr = malloc(sizeof(unsigned int) * MAX_EXTENT_NUM);
  unsigned int *extents_len_arr = malloc(sizeof(unsigned int) * MAX_EXTENT_NUM);
  unsigned long long length;
  long long offset;
  if (!is_virtual_file(pathname, &num_extents, extents_start_arr, extents_len_arr, &length)) {
    return -1;
  }
  else {
    int i;
    setup_mmio();
    for (i = 0; i < ALLOCATED_BUF_NUM; i ++) {
      app_bufs[i] = allocate_kernel_buf(&app_buf_fds[i]);
      app_buf_phy_addrs[i] = *((unsigned long long *)app_bufs[i]);
      send_control_msg(APP_BUF_ADDRS_TAG, app_buf_phy_addrs[i] >> 32);
      send_control_msg(APP_BUF_ADDRS_TAG, app_buf_phy_addrs[i] & 0xFFFFFFFF);
    }
    send_control_msg(APP_FILE_INFO_TAG, num_extents);
    send_control_msg(APP_FILE_INFO_TAG, length >> 32);
    send_control_msg(APP_FILE_INFO_TAG, length & 0xFFFFFFFF);
    offset = length;
    for (i = 0; i < num_extents; i ++) {
      unsigned long long extents_start_in_byte = ((unsigned long long)extents_start_arr[i]) * PHYSICAL_SECTOR_SIZE;      
      send_control_msg(APP_FILE_INFO_TAG, extents_start_in_byte >> 32);
      send_control_msg(APP_FILE_INFO_TAG, extents_start_in_byte & 0xFFFFFFFF);
      unsigned long long extents_len_in_byte = ((unsigned long long)extents_len_arr[i]) * PHYSICAL_SECTOR_SIZE;
      offset -= extents_len_in_byte;
      extents_len_in_byte += (offset > 0) ? 0 : offset;
      send_control_msg(APP_FILE_INFO_TAG, extents_len_in_byte >> 32);
      send_control_msg(APP_FILE_INFO_TAG, extents_len_in_byte & 0xFFFFFFFF);
    }
  }
  file_finish_reading = 0;
  first = 1;
  return VIRT_FILE_FD;
}

void reset(void) {
  app_bufs_ptr = is_eop = buf_idx = buf_len = 0;
  first = 1;
}

void parallel_memcpy(void *dest, const void *src, size_t n) {
  int size_per_worker = (n + PAR_MEMCPY_WORKERS - 1) / PAR_MEMCPY_WORKERS;
  int size_last = n - size_per_worker * (PAR_MEMCPY_WORKERS - 1);
#pragma omp parallel num_threads(PAR_MEMCPY_WORKERS)
  {
    int tid = omp_get_thread_num();
    int copy_size = (tid != (PAR_MEMCPY_WORKERS - 1)) ? size_per_worker : size_last;
    memcpy((unsigned char *)dest + size_per_worker * tid, (unsigned char *)src + size_per_worker * tid, copy_size);
  }
}

ssize_t iread(int fd, void *buf, size_t count) {
  if (fd == VIRT_FILE_FD) {
    if (file_finish_reading) {
      return 0;
    }
    else if (first) {
      update_metadata();
      first = 0;
    }
    unsigned char *app_buf = (unsigned char *)app_bufs[app_bufs_ptr];
    ssize_t read_size;
    if (count >= buf_len - buf_idx) {
      read_size = buf_len - buf_idx;
      if (is_eop) {
	file_finish_reading = 1;
	reset();
	parallel_memcpy(buf, app_buf + buf_idx, read_size);
      }
      else {
	parallel_memcpy(buf, app_buf + buf_idx, read_size);
	send_control_msg(APP_FREE_BUF_TAG, 0);
	app_bufs_ptr = (app_bufs_ptr + 1) & (ALLOCATED_BUF_NUM - 1);	
	buf_idx = 0;
	update_metadata();
      }
    }
    else {
      read_size = count;
      buf_idx += read_size;
      parallel_memcpy(buf, app_buf + buf_idx, read_size);
    }
    return read_size;
  }
  else {
    return -1;
  }
}

int iclose(int fd) {
  if (fd == VIRT_FILE_FD) {
    int i;
    for (i = 0; i < ALLOCATED_BUF_NUM; i ++) {
      if (app_buf_fds[i] > 0) {
  	close(app_buf_fds[i]);
      }
    }
    if (mmio_fd) {
      close(mmio_fd);
    }
    send_control_msg(RESET_TAG, 0);
    return 0;
  }
  else {
    return -1;
  }
  return 0;
}
