#include "kernel/types.h"
#include "user/user.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "kernel/fcntl.h"

#include "kernel/crypto.h"

int main(void) {
  if(open("console", O_RDWR) < 0){
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  printf("crypto_srv: starting\n");
  if(getpid() != 2){//task2.2
    exit(1);
  }
  // TODO: implement the cryptographic server here
  while(1){//task2.3
    void* addr;
    uint64 size;
    if(take_shared_memory_request(&addr, &size) == -1){//2.3
      printf("take has failed\n");
      exit(1);
    }

    struct crypto_op* op = (struct crypto_op*) addr;
    if(op->type == CRYPTO_OP_TYPE_ENCRYPT || op->type == CRYPTO_OP_TYPE_DECRYPT){//task2.4
      char* key = (char*)op->payload;
      int j = 0;
      char* data = (char*)op->payload + op->key_size;
      for(int i = 0; i < op->data_size; i++){//task2.4
        data[i] = data[i] ^ key[j];
        j = (j + 1) % op->key_size;
      }
    }
    asm volatile("fence rw, rw" : : : "memory");//task2.6
    op->state = CRYPTO_OP_STATE_DONE;//task2.5

    if(remove_shared_memory_request(addr, size) == -1){//2.3
      printf("remove has failed\n");
      exit(1);
    }
  }
  exit(0);
}
