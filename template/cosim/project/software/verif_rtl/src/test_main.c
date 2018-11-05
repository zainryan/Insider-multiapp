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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

void user_simulation_function() {
  // PUT YOUR CODE HERE
  int success = 1;
  int i, j;

  gen_data();
  set_physical_file(0, app_input_data_0, sizeof(app_input_data_0));
  set_physical_file(1, app_input_data_1, sizeof(app_input_data_1));
  set_physical_file(2, app_input_data_2, sizeof(app_input_data_2));
  
  for (i = 0; i < 3000; i ++) {
    printf("i = %d\n", i);
    cl_poke(TAG(DEBUG_TAG), 0);
  }

  int fd[APP_NUM], fin_file[APP_NUM] = {0, 0, 0};
  int total_read_bytes[APP_NUM] = {0, 0, 0};
  int fin_file_num = 0;

  for (i = 0; i < APP_NUM; i++) {
    fd[i] = iopen(i, "cosim_phy_file", 0);
  }

  unsigned char *buf = (unsigned char *)malloc(READ_BUF_SIZE);

  while (fin_file_num != APP_NUM) {
    for (i = 0; i < APP_NUM; i++) {
      unsigned char* ptr_app_input_data;
      if (i == 0) {
  	ptr_app_input_data = app_input_data_0;
      }
      else if (i == 1) {
  	ptr_app_input_data = app_input_data_1;
      }
      else if (i == 2) {
  	ptr_app_input_data = app_input_data_2;
      }
      if (!fin_file[i]) {
  	int read_bytes = 0;
  	while (read_bytes != READ_BUF_SIZE) {
  	  int tmp = iread(fd[i], buf, READ_BUF_SIZE - read_bytes);
  	  if (!tmp) {
	    printf("file %d fin.\n", i);
  	    fin_file[i] = 1;
	    fin_file_num++;
  	    break;
  	  }
  	  else {
  	    read_bytes += tmp;
  	  }
  	}
  	printf("read_bytes = %d\n", read_bytes);
  	for (j = 0; j < read_bytes; j++) {
	  int idx = total_read_bytes[i] + j;
  	  if (buf[j] != ptr_app_input_data[idx]) {
  	    printf("app id = %d, index = %d, real = %d, expected = %d\n",
		   i, idx, (int)buf[j], (int)ptr_app_input_data[idx]);
  	    puts("Failed!");
  	    exit(-1);
  	  }
  	}
  	total_read_bytes[i] += read_bytes;
      }
    }
  }

  printf("total_read_bytes[0] = %d\n", total_read_bytes[0]);
  printf("sizeof(app_input_data_0) = %d\n", sizeof(app_input_data_0));
  success &= (total_read_bytes[0] == sizeof(app_input_data_0));

  printf("total_read_bytes[1] = %d\n", total_read_bytes[1]);
  printf("sizeof(app_input_data_1) = %d\n", sizeof(app_input_data_1));
  success &= (total_read_bytes[1] == sizeof(app_input_data_1));

  printf("total_read_bytes[2] = %d\n", total_read_bytes[2]);
  printf("sizeof(app_input_data_2) = %d\n", sizeof(app_input_data_2));
  success &= (total_read_bytes[2] == sizeof(app_input_data_2));

  if (success) {
    puts("Test PASSED");
  } else {
    puts("Test FAILED");
  }
}
