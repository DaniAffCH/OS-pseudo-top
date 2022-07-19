#include "update.h"

void* updateThread(void* p){
    ListHead *l = (ListHead*) p;
    while(1){
        updateProcList(l);
        sleep(1);
    }
}

static int isDigit(const char* s){
    while(*s){
        if(*s < '0' || *s > '9')
            return 0;
        ++s;
    }
    return 1;
}

//with 32 bit overflow may occurs 
static u_int64_t getTotalCpuJiffies(){
    FILE* f = fopen(DIR_SYS_STAT, "r");
    u_int64_t cpuJiffiesComponent[10];
    u_int64_t totJiffies = 0;
    if(!f && errno){
        printf("Error opening /proc/stat file\n");
        exit(-1);
    }

    char* file_buffer = (char*) malloc(sysconf(_SC_PAGESIZE));
    fgets(file_buffer, sysconf(_SC_PAGESIZE), f);

    //EXAMPLE:           cpu  33684 689 19310 3125058 1025 0 1434 0 0 0
    sscanf(file_buffer, "cpu  %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", &cpuJiffiesComponent[0], &cpuJiffiesComponent[1], &cpuJiffiesComponent[2], &cpuJiffiesComponent[3], &cpuJiffiesComponent[4], &cpuJiffiesComponent[5], &cpuJiffiesComponent[6], &cpuJiffiesComponent[7], &cpuJiffiesComponent[8], &cpuJiffiesComponent[9]);

    /* Considering all time components
    for(int i = 0; i<10; ++i)
        totJiffies+= cpuJiffiesComponent[i];
    */
    
    free(file_buffer);
    fclose(f);

    return cpuJiffiesComponent[0];
}

static u_int64_t getProcessCpuJiffies(char* pid, pt_proc_t * proc, u_int8_t * error){
    char file_dir[268];
    char name[16];
    char state;
    snprintf(file_dir, sizeof(file_dir), "%s/%s%s", DIR_PROC, pid, DIR_CPU_PROC);
    FILE* f = fopen(file_dir, "r");
    if(!f && errno){
        //printf("Error opening /proc/[PID]/stat file\n");
        *error = 1;
        return 0;
    }

    unsigned long t1, t2;
    long t3, t4;

    char* file_buffer = (char*) malloc(sysconf(_SC_PAGESIZE));
    fgets(file_buffer, sysconf(_SC_PAGESIZE), f);

    sscanf(file_buffer, "%*d (%[^\t\n()]) %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %ld %ld", name, &state, &t1, &t2, &t3, &t4);

    free(file_buffer);
    fclose(f);

    if(state == 'S' || state == 'D' || state == 'R')
        proc->state=READY;
    else if(state == 'T')
        proc->state=SLEEPING;
    else
        proc->state=INACTIVE;

    strcpy(proc->name, name);

    //return t1+t2+t3+t4; considering all time components
    return t1;
}

static u_int64_t getProcessMemory(char* pid, u_int8_t * error){
    char* file_buffer = (char*) malloc(sysconf(_SC_PAGESIZE));
    char file_dir[268];
    u_int64_t mem;

    snprintf(file_dir, sizeof(file_dir), "%s/%s%s", DIR_PROC, pid, DIR_MEM_PROC);

    FILE* f = fopen(file_dir, "r");

    if(!f && errno){
        //printf("Error opening stat file\n");
        *error = 1;
        return 0;
    }

    fgets(file_buffer, sysconf(_SC_PAGESIZE), f);

    sscanf(file_buffer, "%lu ", &mem);

    fclose(f);
    free(file_buffer);

    return mem;
}

void updateProcList(ListHead * l){
    u_int64_t sysJ_t0 = getTotalCpuJiffies();
    struct dirent *de;
    DIR *dr = opendir(DIR_PROC);
    procListItem *currentListItem = (procListItem*) l->first;
    procListItem *oldListItem = NULL;
    u_int8_t error = 0;

    #ifdef DEBUG
        assert(l && "List Head is null");
    #endif

    while((de=readdir(dr))){
        if(!de && errno){
            continue;
        }

        if(!isDigit(de->d_name))
            continue;

        while(currentListItem == NULL || currentListItem->info->pid != atoi(de->d_name)){

            if(currentListItem == NULL || currentListItem->info->pid > atoi(de->d_name)){
                // new process untracked found
                procListItem * newListItem = (procListItem*) malloc(sizeof(procListItem));
                newListItem->list.next = NULL; newListItem->list.prev = NULL;
                newListItem->info = (pt_proc_t*) malloc(sizeof(pt_proc_t));
                newListItem->info->pid = atoi(de->d_name);
                newListItem->info->state = READY;
                newListItem->info->name = (char*) malloc(TASK_COMM_LEN);

                currentListItem = (procListItem*) List_insert(l, (ListItem*) oldListItem, (ListItem*) newListItem);
            }
            else{
                // a tracked process has been deleted 
                procListItem * oldListItem = currentListItem;
                currentListItem = (procListItem*) currentListItem->list.next;
                List_detach(l, (ListItem*) oldListItem);
                free(oldListItem->info->name);
                free(oldListItem->info);
                free(oldListItem);
            }

        }

        currentListItem->info->pre_jiffies = getProcessCpuJiffies(de->d_name,currentListItem->info, &error);

        if(error){
            error = 0;
            continue;
        }

        currentListItem->info->memory_usage = getProcessMemory(de->d_name, &error);

        if(error){
            error = 0;
            continue;
        }

        oldListItem = currentListItem;
        currentListItem = (procListItem*) currentListItem->list.next;
    }

    sleep(SLEEPING_TIME);

    u_int64_t sysJ_t1 = getTotalCpuJiffies();
    u_int64_t post_jiffies;
    char pid_str[16];

    currentListItem = (procListItem*) l->first;

    while(currentListItem){
        sprintf(pid_str, "%d", currentListItem->info->pid);
        post_jiffies = getProcessCpuJiffies(pid_str,currentListItem->info, &error);

        if(error){
            error = 0;
            continue;
        }

        currentListItem->info->cpu_usage = 100*( (double)(post_jiffies - currentListItem->info->pre_jiffies)/(double)(sysJ_t1 - sysJ_t0)  );

        #ifdef DEBUG
        printf("[%s] (%s) cpu usage:%u memory usage:%lu\n", pid_str, currentListItem->info->name, currentListItem->info->cpu_usage, currentListItem->info->memory_usage);
        #endif

        currentListItem = (procListItem*) currentListItem->list.next;
    }

    closedir(dr);

}