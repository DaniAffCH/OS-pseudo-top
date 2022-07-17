#pragma once

#include <sys/types.h>
#include "linked_list.h"

typedef enum{
  READY,
  SLEEPING
} pt_state;

typedef struct thread_params {
  int* show_offt;
  ListHead* list;
} thread_params_t;


typedef enum{
  PAUSE,
  RESUME,
  TERMINATE,
  KILL
} pt_action;

typedef struct pt_proc
{
    pid_t pid;
    char* name;
    pt_state state;
    u_int32_t pre_jiffies;
    u_int8_t cpu_usage;
    u_int64_t memory_usage;
} pt_proc_t;

typedef struct procListItem{
  ListItem list;
  pt_proc_t *info;
} procListItem;