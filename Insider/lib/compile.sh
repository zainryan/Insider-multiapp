#!/bin/bash

gcc -DPAR_MEMCPY_WORKERS=3 -fopenmp insider_runtime.c -Wall -O3 -shared -fPIC -shared -o libinsider_runtime.so
