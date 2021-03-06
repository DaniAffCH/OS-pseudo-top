#pragma once

#include "linked_list.h"
#include "types.h"
#include "exit_routine.h"
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <sys/prctl.h>
#include <ncurses.h>
#include <pthread.h>

#define DIR_PROC "/proc"
#define DIR_CPU_PROC "/stat"
#define DIR_MEM_PROC "/statm"
#define DIR_SYS_STAT "/proc/stat"
#define TIME_OFFT 11
#define START_TIME_OFFT 6
#define SLEEPING_TIME 1


void updateProcList(ListHead * l); //side effect on list 

void* updateThread(void* p);