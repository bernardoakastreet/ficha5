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

int main(int argc, char** argv) {

  sem_t *access_mem, *read_av;
  access_mem = sem_open("/memaccess", O_CREAT | O_RDWR, 0600, NULL);
  read_av = sem_open("/readavailable", O_CREAT | O_RDWR, 0600, NULL);

  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) handle_error("shm_open");

  int r = ftruncate(shm_fd, SHM_SIZE);
  if (r == -1) handle_error("ftruncate");

  char* shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) handle_error("mmap");

  strcpy(shm_ptr, "");
  
  while (1) {
    sem_wait(access_mem);
    if(!sem_trywait(read_av)) printf("Dados: %s\n", shm_ptr);
    sem_post(access_mem);
    sleep(2);
  }
   
  return 0;
}
