#include "exit_routine.h"

void my_exit(int code, char* msg){
    extern ListHead * lh;
    extern int nFree; extern int nMalloc;
    procListItem * current = (procListItem*) lh->first;
    
    endwin();

    while(current){
        procListItem * old = current;
        current = (procListItem*) current->list.next;
        List_detach(lh, (ListItem*) old);
        free(old->info);
        free(old);
        ++nFree;
    }

    free(lh);

    if(msg)
        perror(msg);

    printf("pseudo_top exited successfully.\n#Malloc=%d #Free=%d\n", nMalloc, nFree);
    
    exit(code);
}