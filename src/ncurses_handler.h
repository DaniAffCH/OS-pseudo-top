#pragma once

#include <ncurses.h>
#include <string.h>
#include "types.h"

#define ROW_SIZE 68

void nc_init(void);
void print_centered(int row, const char* text);
void print_header();
void print_stats(ListHead * l, int show_offt);