#pragma once

#include "types.h"
#include <ncurses.h>
#include <unistd.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))


void* listener(void * args);