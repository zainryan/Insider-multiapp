#ifndef INSIDER_RUNTIME_HPP
#define INSIDER_RUNTIME_HPP

extern "C" {
int send_input_params(int fd, unsigned int data);
int send_input_params_array(int fd, unsigned int *data_arr, size_t arr_len);
int iopen(const char *pathname, int flags);
ssize_t iread(int fd, void *buf, size_t count);
int iclose(int fd);
const char *reg_virt_file(int app_id, const char *real_path);
}

#endif
