#include "exit_routine.h"

void my_exit(int code, char* msg){
    extern ListHead * lh;

    procListItem * current = (procListItem*) lh->first;
    
    endwin();

    while(current){
        procListItem * old = current;
        current = (procListItem*) current->list.next;
        List_detach(lh, (ListItem*) old);
        free(old->info);
        free(old);
    }

    free(lh);

    if(msg)
        perror(msg);
    
    exit(code);
}