#include "input_listener.h"
#include <stdlib.h>
void* listener(void * args){
    thread_params_t * args_c = (thread_params_t*) args;
    int ch;
    int max_offt;

    while(1){
        ch = getch();
        if(ch == KEY_UP){
            *(args_c->show_offt) = MAX(0, *(args_c->show_offt) - 1);
        }
        if(ch == KEY_DOWN){
            max_offt = MAX(0, args_c->list->size - stdscr->_maxy);
            *(args_c->show_offt) = MIN(max_offt, *(args_c->show_offt) + 1);
        }
    }
}
