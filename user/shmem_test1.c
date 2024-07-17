#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"


int main(int argc, char *argv[]){
    char* shmem = malloc(PGSIZE);
    int pid;
    strcpy(shmem, "Hello child");

    if ((pid = fork()) == 0) {
        // Child process 

        char* child_shmem  = (char*)map_shared_pages(getpid(), (void*)shmem, PGSIZE);

        printf("Child process prints: %s\n", child_shmem);
        exit(0);
    }

    else {
        wait(0);    // Wait for child to print and exit
        free(shmem);
        exit(0);
    }
}