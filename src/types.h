#pragma once

#include <sys/types.h>
#include "linked_list.h"

typedef enum{
  READY,
  SLEEPING
} pt_state;


typedef enum{
  PAUSE,
  RESUME,
  TERMINATE,
  KILL
} pt_action;

typedef struct pt_proc
{
    pid_t pid;
    pt_state state;
    u_int8_t cpu_usage;
    u_int64_t memory_usage;
} pt_proc_t;

typedef struct procListItem{
  ListItem list;
  pt_proc_t *info;
} procListItem;