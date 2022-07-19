#pragma once

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

void actionHandler(pid_t pid, pt_action action, ListHead* l);
void actionWrapper(pid_t pid, char* action, ListHead *l);