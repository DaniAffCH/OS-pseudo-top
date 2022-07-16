#include "update.h"

static int isDigit(const char* s){
    while(*s){
        if(*s < '0' || *s > '9')
            return 0;
        ++s;
    }
    return 1;
}

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
