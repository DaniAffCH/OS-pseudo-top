#include "input_listener.h"
#include <stdlib.h>
void* listener(void * args){
    thread_il_params_t * args_c = (thread_il_params_t*) args;
    int ch;
    int max_offt;

    while(1){
        if(*(args_c->input_req)){
            sleep(1);
            continue;
        }
        ch = getch();
        if(ch == KEY_UP){
            *(args_c->show_offt) = MAX(0, *(args_c->show_offt) - 1);
        }
        else if(ch == KEY_DOWN){
            max_offt = MAX(0, args_c->list->size - stdscr->_maxy + 3);
            *(args_c->show_offt) = MIN(max_offt, *(args_c->show_offt) + 1);
        }
        else if(ch == 'q'){
            *(args_c->running) = 0;
        }
        else if(ch == 'e'){
            *(args_c->input_req) = 1;
        }
    }
}
