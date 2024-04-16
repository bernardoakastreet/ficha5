#include "mycommon.h"
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv){

  sem_unlink("/memaccess");
  sem_unlink("/readavailable");

  sem_t *access_mem, *read_av;

  access_mem = sem_open("/memaccess", O_CREAT | O_RDWR, 0600, 1);
  read_av = sem_open("/readavailable", O_CREAT | O_RDWR, 0600, 0);

  char buffer[SHM_SIZE];

  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0);
  if (shm_fd == -1) handle_error("shm_open");

  char* shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) handle_error("mmap");

  while(1){
    printf(": ");
    fgets(buffer, SHM_SIZE, stdin);
    sem_wait(access_mem);
    strcpy(shm_ptr, buffer);
    sem_post(read_av);
    sem_post(access_mem);
  }

  return 0;
}
