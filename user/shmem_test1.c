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
        sleep(1); // Wait for parent to share the memory
        printf("Child process prints: %s\n", shmem);
        exit(0);
    }

    else {

        //Parent process
        map_shared_pages(getpid(), pid, (void*)shmem, PGSIZE);    // Share memory with child

        wait(0);    // Wait for child to print and exit
        free(shmem);
        exit(0);
    }
}