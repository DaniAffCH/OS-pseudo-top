#include "ncurses_handler.h"
#include <stdlib.h>
#include "linked_list.h"
#include "update.h"
#include "input_listener.h"
#include <pthread.h>

int main()
{	  
    int ret,
        show_offt = 0;
    pthread_t th;

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

    thread_params_t param = {.show_offt=&show_offt, .list=lh};

    ret = pthread_create(&th, NULL, listener, &param);

    if(ret != 0){
        printf("thread initialization failed \n");
        exit(1);
    }

    while(1){
        updateProcList(lh);
        print_stats(lh, show_offt);
        sleep(0.2);
    }

    free(lh);
	//getch();			/* Wait for user input */
	//endwin();			/* End curses mode */

	return 0;
}