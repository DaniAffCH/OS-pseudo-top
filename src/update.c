#include "update.h"

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

    for(int i = 0; i<10; ++i)
        totJiffies+= cpuJiffiesComponent[i];
    
    free(file_buffer);
    fclose(f);

    return totJiffies;
}

static u_int64_t getProcessCpuJiffies(char* pid, pt_proc_t * proc){
    char file_dir[268];
    char name[16];
    snprintf(file_dir, sizeof(file_dir), "%s/%s%s", DIR_PROC, pid, DIR_CPU_PROC);
    FILE* f = fopen(file_dir, "r");
    if(!f && errno){
        printf("Error opening /proc/[PID]/stat file\n");
        exit(-1);
    }

    unsigned long t1, t2;
    long t3, t4;

    char* file_buffer = (char*) malloc(sysconf(_SC_PAGESIZE));
    fgets(file_buffer, sysconf(_SC_PAGESIZE), f);

    sscanf(file_buffer, "%*d (%[^\t\n()]) %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %ld %ld", name, &t1, &t2, &t3, &t4);

    free(file_buffer);
    fclose(f);


    strcpy(proc->name, name);

    return t1+t2+t3+t4;
}

static u_int64_t getProcessMemory(char* pid){
    char* file_buffer = (char*) malloc(sysconf(_SC_PAGESIZE));
    char file_dir[268];
    u_int64_t mem;

    snprintf(file_dir, sizeof(file_dir), "%s/%s%s", DIR_PROC, pid, DIR_MEM_PROC);

    FILE* f = fopen(file_dir, "r");

    if(!f && errno){
        printf("Error opening stat file\n");
        exit(-1);
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

    #ifdef DEBUG
        assert(l && "List Head is null");
    #endif

    while((de=readdir(dr))){
        if(errno){
            printf("Error reading proc path\n");
            exit(-1);
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

        currentListItem->info->pre_jiffies = getProcessCpuJiffies(de->d_name,currentListItem->info);

        currentListItem->info->memory_usage = getProcessMemory(de->d_name);

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
        post_jiffies = getProcessCpuJiffies(pid_str,currentListItem->info);

        currentListItem->info->cpu_usage = 100*( (float)(post_jiffies - currentListItem->info->pre_jiffies)/(float)(sysJ_t1 - sysJ_t0)  );

        #ifdef DEBUG
        printf("[%s] (%s) cpu usage:%u memory usage:%lu\n", pid_str, currentListItem->info->name, currentListItem->info->cpu_usage, currentListItem->info->memory_usage);
        #endif

        currentListItem = (procListItem*) currentListItem->list.next;
    }

}

/*
void updateProcList(ListHead * l){
    struct dirent *de;
    char *file_buffer;
    char file_dir[268];
    DIR *dr = opendir(DIR_PROC);
    unsigned int seconds;
    
    procListItem *currentListItem = (procListItem*) l->first;
    procListItem *oldListItem = NULL;

    u_int32_t uptime;
    u_int16_t time_elapsed;
    u_int16_t time_start;

    #ifdef DEBUG
        assert(l && "List Head is null");
    #endif

    //getting system uptime 
    FILE* sysupdate = fopen(DIR_SYS_UPT, "r");
    if(!sysupdate && errno){
            printf("Error opening uptime file\n");
            exit(-1);
    }

    file_buffer = (char*) malloc(sysconf(_SC_PAGESIZE));
    fgets(file_buffer, sysconf(_SC_PAGESIZE), sysupdate);
    uptime = atoi(strtok(file_buffer, " "));

    free(file_buffer);

    while((de=readdir(dr))){
        if(errno){
            printf("Error reading proc path\n");
            exit(-1);
        }

        if(!isDigit(de->d_name))
            continue;
        
        snprintf(file_dir, sizeof(file_dir), "%s/%s%s", DIR_PROC, de->d_name, DIR_CPU_PROC);
        FILE* current = fopen(file_dir, "r");

        if(!current && errno){
            printf("Error opening stat file\n");
            exit(-1);
        }

        if(currentListItem == NULL || currentListItem->info->pid != atoi(de->d_name)){
            //TODO: handle process died 
            procListItem * newListItem = (procListItem*) malloc(sizeof(procListItem));
            newListItem->list.next = NULL; newListItem->list.prev = NULL;

            newListItem->info = (pt_proc_t*) malloc(sizeof(pt_proc_t));
            newListItem->info->pid = atoi(de->d_name);
            newListItem->info->state = READY;
            currentListItem = (procListItem*) List_insert(l, (ListItem*) oldListItem, (ListItem*) newListItem);
        }


        file_buffer = (char*) malloc(sysconf(_SC_PAGESIZE));
        fgets(file_buffer, sysconf(_SC_PAGESIZE), current);

        strtok(file_buffer, " ");

        //The second string (name) may contain spaces, this workaround avoids messing up with it
        char* token = strtok(NULL, " ");
        while(strchr(token, ')') == NULL)
            token = strtok(NULL, " ");

        for(u_int8_t nToken=0; nToken < TIME_OFFT; ++nToken)
            strtok(NULL, " ");

        time_elapsed = atoi(strtok(NULL, " "));
        time_elapsed += atoi(strtok(NULL, " "));

        for(u_int8_t nToken=0; nToken < START_TIME_OFFT; ++nToken)
            strtok(NULL, " ");
        
        time_start = atoi(strtok(NULL, " "));

        //seconds = uptime - (time_start/SYS_CLK_FREQ);
        seconds = time_elapsed/SYS_CLK_FREQ;

        currentListItem->info->cpu_usage = 100 * (((float)time_elapsed / (float)SYS_CLK_FREQ) / (float)seconds);

        fclose(current);

        //getting process memory usage

        snprintf(file_dir, sizeof(file_dir), "%s/%s%s", DIR_PROC, de->d_name, DIR_MEM_PROC);

        current = fopen(file_dir, "r");

        if(!current && errno){
            printf("Error opening stat file\n");
            exit(-1);
        }

        fgets(file_buffer, sysconf(_SC_PAGESIZE), current);

        currentListItem->info->memory_usage = atoi(strtok(file_buffer, " "));

        fclose(current);

        free(file_buffer);

        #ifdef DEBUG
            printf("[%s] CPU: %d MEM: %lu\n", de->d_name, currentListItem->info->cpu_usage, currentListItem->info->memory_usage);
        #endif
        

        oldListItem = currentListItem;
        currentListItem = (procListItem*) currentListItem->list.next;
    }

    #ifdef DEBUG
        List_print(l);
    #endif


}
*/