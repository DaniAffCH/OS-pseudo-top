#pragma once

#include "linked_list.h"
#include "types.h"
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>

#define DIR_PROC "/proc"
#define DIR_CPU_PROC "/stat"
#define DIR_MEM_PROC "/statm"
#define DIR_SYS_UPT "/proc/uptime"
#define TIME_OFFT 11
#define START_TIME_OFFT 6
#define SYS_CLK_FREQ sysconf(_SC_CLK_TCK)

void updateProcList(ListHead * l); //side effect on list 