#include "ncurses_handler.h"
#include <stdlib.h>
#include "linked_list.h"
#include "update.h"
#include "input_listener.h"
#include <pthread.h>

int main()
{	  
    int ret;
    volatile int
        show_offt = 0,
        running = 1,
        input_req = 0;
    pthread_t th_li;
    char input[120];

    nc_init();

    init_pair(1, COLOR_BLACK, COLOR_MAGENTA);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
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
        exit(1);
    }

    //ret = pthread_create(&th_up, NULL, updateThread, (void*) lh);

    if(ret != 0){
        printf("thread initialization failed \n");
        exit(1);
    }

    while(running){
        if(input_req){
            echo();
            move(stdscr->_maxy,0);
            curs_set(1);
            getstr(input);

            //chiama l'handler

            noecho();
            move(stdscr->_maxy,0);
            clrtoeol();
            input_req = 0;
        }
        else{
            updateProcList(lh);
            print_stats(lh, show_offt);
        }
    }

    free(lh);
	endwin();			/* End curses mode */

	return 0;
}