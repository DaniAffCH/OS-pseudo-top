#include "action_handler.h"

//Not tested yet

void actionWrapper(pid_t pid, char* action, ListHead* l){
    pt_action act;
    if(strcmp(action, "pause") == 0)
        act = PAUSE;
    else if(strcmp(action, "resume") == 0)
        act = RESUME;
    else if(strcmp(action, "terminate") == 0)
        act = TERMINATE;
    else if(strcmp(action, "kill") == 0)
        act = KILL;
    
    actionHandler(pid, act, l);
}

void actionHandler(pid_t pid, pt_action action, ListHead* l){
    int ret;

    procListItem* item = (procListItem*) List_find_by_pid(l, pid);

    #ifdef DEBUG
    if(!item){
        printf("Pid not found\n");
        endwin();
        exit(-1);
    }
    #endif  

    switch (action)
    {
    case PAUSE:
        if(item->info->state == READY){
            ret = kill(pid, SIGSTOP);
            if(ret == -1){
                printf("Signal send failed\n");
                endwin();
                exit(-1);
            }

            item->info->state = SLEEPING;
        }

        break;
    case RESUME:

        if(item->info->state == SLEEPING){
            ret = kill(pid, SIGCONT);
            if(ret == -1){
                printf("Signal send failed\n");
                endwin();
                exit(-1);
                
            }

            item->info->state = READY;
        }

        break;
    case KILL:
        ret = kill(pid, SIGKILL);
        if(ret == -1){
            printf("Signal send failed\n");
            exit(-1);
        }
        item->info->state = INACTIVE;
        break;
    case TERMINATE:
        ret = kill(pid, SIGTERM);
        if(ret == -1){
            printf("Signal send failed\n");
            endwin();
            exit(-1);
        }
        item->info->state = INACTIVE;
        break;
    default:
        printf("unknown action");
        endwin();
        exit(-1);
        break;
    }
}