#pragma once

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "types.h"
#include "exit_routine.h"

void actionHandler(pid_t pid, pt_action action, ListHead* l);
void actionWrapper(pid_t pid, char* action, ListHead *l);