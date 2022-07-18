#include "ncurses_handler.h"

void nc_init(){
    initscr(); // Start curses mode
    //raw(); // disable line buffering (get as soon as possible keyboard input)
    noecho(); // disable echoing of input
    keypad(stdscr, TRUE); // enable arrows
    start_color(); // allow colors
    curs_set(0); // Hide cursor
}

void print_centered(int row, const char* text){
    WINDOW * win = stdscr;
    size_t halfWin = win->_maxx/2;
    size_t halfText = strlen(text)/2;

    mvprintw(row, halfWin-halfText, text);
    refresh();
}

void print_header(){
    print_centered(2,"|  PID  |              NAME              | CPU |     MEM     |STATE|");
    refresh();
}

void print_stats(ListHead * l, int show_offt){
    procListItem * current = (procListItem*) l->first;
    char row[ROW_SIZE+2];
    int rowCnt = 3;

    char pid_str[18],
         cpu_str[16],
         mem_str[14],
         state_str[] = "R";

    for(int j = 0; j<show_offt; ++j)
        current = (procListItem*) current->list.next;
    
    while(current && rowCnt < stdscr->_maxy){
        move(rowCnt,0);
        clrtoeol();
        sprintf(pid_str, "%d", current->info->pid);
        sprintf(cpu_str, "%hu", current->info->cpu_usage);
        sprintf(mem_str, "%lu", current->info->memory_usage);
        
        //sprintf(pid_str, "%u", current->info->pid);
        
        sprintf(row, "|%7.7s|%32.32s|%4.4s%%|%13.13s|%5.5s|", pid_str, current->info->name, cpu_str, mem_str, state_str);

        print_centered(rowCnt, row);

        current = (procListItem*) current->list.next;
        ++rowCnt;
    }
    refresh();
}