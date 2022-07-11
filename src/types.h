#pragma once

#include <sys/types.h>

typedef enum{
    READY,
    TERMINATED,
    KILLED,
    PAUSE
} pt_state;

typedef struct pt_proc
{
    pid_t pid;
    pt_state state;
    u_int8_t cpu_usage;
    u_int64_t memory_usage; //memory usage is intended as stack size + heap size
} pt_proc_t;
