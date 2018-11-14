// Amazon FPGA Hardware Development Kit
//
// Copyright 2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Amazon Software License (the "License"). You may not use
// this file except in compliance with the License. A copy of the License is
// located at
//
//    http://aws.amazon.com/asl/
//
// or in the "license" file accompanying this file. This file is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, express or
// implied. See the License for the specific language governing permissions and
// limitations under the License.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef VIVADO_SIM
#include "svdpi.h"
#endif

#include "sh_dpi_tasks.h"

#include "insider_cosim.h"

#define APP_NUM (3)
#define FLIT_SIZE (64)
#define READ_BUF_SIZE (2 * 1024 * 1024)

unsigned char app_input_data_0[384000];
unsigned char app_input_data_1[192000];
unsigned char app_input_data_2[128000];

char random_char() {
  int min_ascii = 65;
  int max_ascii = 90;
  return min_ascii + rand() % (max_ascii - min_ascii + 1);
}

void gen_data() {
  int i;
  for (i = 0; i < sizeof(app_input_data_0); i++) {
    app_input_data_0[i] = random_char();
  }
  for (i = 0; i < sizeof(app_input_data_1); i++) {
    app_input_data_1[i] = random_char();
  }
  for (i = 0; i < sizeof(app_input_data_2); i++) {
    app_input_data_2[i] = random_char();
  }
}

void test_main(uint32_t *exit_code) {

#ifndef VIVADO_SIM
  svScope scope;
  scope = svGetScopeFromName("tb");
  svSetScope(scope);
#endif

  simulator();

  *exit_code = 0;
}

unsigned char *get_ptr_app_input_data(int idx) {
  unsigned char *ptr_app_input_data;
  if (idx == 0) {
    ptr_app_input_data = app_input_data_0;
  } else if (idx == 1) {
    ptr_app_input_data = app_input_data_1;
  } else if (idx == 2) {
    ptr_app_input_data = app_input_data_2;
  }
  return ptr_app_input_data;
}

void user_simulation_function() {
  // PUT YOUR CODE HERE
  int success = 1;
  int i, j;

  gen_data();
  set_physical_file(0, app_input_data_0, sizeof(app_input_data_0));
  set_physical_file(1, app_input_data_1, sizeof(app_input_data_1));
  set_physical_file(2, app_input_data_2, sizeof(app_input_data_2));

  for (i = 0; i < 3000; i++) {
    printf("i = %d\n", i);
    cl_poke(TAG(DEBUG_TAG), 0);
  }

  int fd[APP_NUM], fin_file[APP_NUM] = {0, 0, 0};
  int total_read_bytes[APP_NUM] = {0, 0, 0};
  int fin_file_num = 0;
  int size_app_input_data[APP_NUM] = {sizeof(app_input_data_0),
                                      sizeof(app_input_data_1),
                                      sizeof(app_input_data_2)};

  unsigned char *buf[APP_NUM];
  for (i = 0; i < APP_NUM; i++) {
    fd[i] = iopen(i, "cosim_phy_file", 0);
    buf[i] = (unsigned char *)malloc(READ_BUF_SIZE);
  }

  while (fin_file_num != APP_NUM) {
    for (i = 0; i < APP_NUM; i++) {
      unsigned char *ptr_app_input_data = get_ptr_app_input_data(i);
      if (!fin_file[i]) {
        int read_bytes = 0;
        while (read_bytes != READ_BUF_SIZE) {
          int tmp = iread(fd[i], buf[i], READ_BUF_SIZE - read_bytes);
          if (!tmp) {
            printf("file %d fin.\n", i);
            fin_file[i] = 1;
            fin_file_num++;
            break;
          } else {
            read_bytes += tmp;
          }
        }
        total_read_bytes[i] += read_bytes;
      }
    }
  }
  puts("\n");

  for (i = 0; i < APP_NUM; i++) {
    int expected_total_read_bytes = size_app_input_data[i] % DATA_BUS_WIDTH;
    if (expected_total_read_bytes == 0) {
      expected_total_read_bytes = DATA_BUS_WIDTH;
    }
    printf("total_read_bytes[%c] = %d\n", i + '0', total_read_bytes[i]);
    printf("expected_total_read_bytes[%c] = %d\n", i + '0',
           expected_total_read_bytes);
    success &= (total_read_bytes[i] == expected_total_read_bytes);

    unsigned char *ptr_app_input_data = get_ptr_app_input_data(i);
    for (j = 0; j < expected_total_read_bytes; j++) {
      int expected_data = ptr_app_input_data[size_app_input_data[i] -
                                             expected_total_read_bytes + j];
      if (buf[i][j] != expected_data) {
        printf("idx = %d, expected_data = %d, real_data = %d\n", j,
               expected_data, (int)buf[i][j]);
        success = 0;
        break;
      }
    }
    puts("\n");
  }

  if (success) {
    puts("Test PASSED");
  } else {
    puts("Test FAILED");
  }
}
