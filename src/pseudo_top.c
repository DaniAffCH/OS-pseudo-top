#include "ncurses_handler.h"
#include <stdlib.h>
#include "linked_list.h"
#include "update.h"
#include "input_listener.h"
#include "action_handler.h"
#include <pthread.h>

int main()
{	  
    int ret;
    int show_offt = 0,
        running = 1,
        input_req = 0;
    pthread_t th_li;
    #ifdef TH
    pthread_t th_up;
    #endif
    pid_t pid;
    char action[30];
    char input[120];

    nc_init();

    init_pair(1, COLOR_BLACK, COLOR_MAGENTA);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        endwin();
        exit(1);
    }

    attron(COLOR_PAIR(1));

	print_centered(0, "PSEUDO TOP");

    attroff(COLOR_PAIR(1));

    ListHead * lh = (ListHead*) malloc(sizeof(ListHead));
    List_init(lh);

    print_header();

    thread_il_params_t param = {.show_offt=&show_offt, .list=lh, .running=&running, .input_req=&input_req};

    ret = pthread_create(&th_li, NULL, listener, &param);

    if(ret != 0){
        printf("thread initialization failed \n");
        endwin();
        exit(-1);
    }

    #ifdef TH
    ret = pthread_create(&th_up, NULL, updateThread, (void*)lh);

    if(ret != 0){
        printf("thread initialization failed \n");
        endwin();
        exit(-1);
    }
    #endif

    while(running){
        if(input_req){
            echo();
            move(stdscr->_maxy,0);
            curs_set(1);
            getstr(input);

            sscanf(input, "%s %d", action, &pid);
            actionWrapper(pid, action, lh);

            noecho();
            move(stdscr->_maxy,0);
            clrtoeol();
            input_req = 0;
        }
        else{
            print_stats(lh, show_offt);
            #ifndef TH
            updateProcList(lh);
            #endif
        }
    }

	endwin();			/* End curses mode */

	return 0;
}