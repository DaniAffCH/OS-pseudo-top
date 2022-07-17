#include "action_handler.h"

//Not tested yet

void actionHandler(pid_t pid, pt_action action, ListHead* l){
    int ret;

    procListItem* item = (procListItem*) List_find_by_pid(l, pid);

    #ifdef DEBUG
    if(!item){
        printf("Pid not found\n");
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
        break;
    case TERMINATE:
        ret = kill(pid, SIGTERM);
        if(ret == -1){
            printf("Signal send failed\n");
            exit(-1);
        }
        break;
    default:
        printf("unknown action");
        exit(-1);
        break;
    }
}