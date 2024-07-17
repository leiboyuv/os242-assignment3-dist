#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int main(int argc, char *argv[]){

    int pid = getpid();
    char* shmem = malloc(PGSIZE);

    if (fork() == 0) {
        // Child process

        uint64 child_size_before_mapping = (uint64)sbrk(0);                 // This is the virtual address which will be mapped to the shared physical address
        
        char* child_shmem = (char*)map_shared_pages(pid, shmem, PGSIZE);    // Share allocated memory
        
        strcpy(child_shmem, "Hello daddy");                                 // Write to shared memory

        uint64 child_size_after_mapping = (uint64)sbrk(0);

        unmap_shared_pages(child_shmem, PGSIZE);
        uint64 child_size_after_unmapping = (uint64)sbrk(0);

        void* blah = malloc(50*PGSIZE);                                    // Process can allocate memory properly after unmapping
        uint64 child_size_after_malloc = (uint64)sbrk(0);

        printf("Before mapping: %p\nAfter mapping: %p\nAfter unmapping: %p\nAfter malloc: %p\n", 
        child_size_before_mapping, child_size_after_mapping, child_size_after_unmapping, child_size_after_malloc);

        free(blah);    // Free allocated memory
        
        exit(0);
    }

    else {
        sleep(2);                                       // Sleep so child would save his size before mapping

        printf("Parent read: %s\n", (char*)shmem);      // Read from shared memory

        wait(0);          // Wait for child to exit
        free(shmem);      // Free allocated memory
        exit(0);
    }
}