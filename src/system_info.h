#pragma once 

#include <stdint.h>
#include <stdio.h>
#include <errno.h> 

#define CPU_INFO_PATH "/proc/stat"

uint8_t getCpuUsage();