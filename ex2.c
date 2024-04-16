#include <pthread.h>
#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

void myprint(const char *str);
void *myfunc(void *);
sem_t *psem;

int main()
{
  //Close semaphore (for named semaphore)
  //sem_unlink("/mysem");

  //Create named semaphore
  //psem = sem_open("/mysem", O_CREAT | O_RDWR, 0600, 1);

  //Create unnamed semaphore
  //Alloc shared memory
  psem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  sem_init(psem, 1, 1);

  //Change from dynamic memory allocation for both processes to one shared memory block
  //char *buf = (char *)mmap(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  char *buf = (char *) malloc(256);

  int r = fork();
  if(r==0){
    
    sprintf(buf,"%d: Ola -----------------------------------------\n",getpid());  
    myfunc(buf);
    
  } else {
  
    sprintf(buf,"%d: Ole +++++++++++++++++++++++++++++++++++++++++\n",getpid());
    myfunc(buf);
    
  }
      
  return(0);
}

void *myfunc(void *arg){
  while(1) {
    sleep(1);
    sem_wait(psem);
    myprint((char *)arg);
    sem_post(psem);
  }
}


/*
2.2) (1 Execution Process) Unlink "mysem" -> Open or create semaphore ("mysem") with its value =1 -> Alocate 256 bytes of memory to variable "buf"
     -> Fork (Create new child process)

	Child Execution -> Binds var "buf" (Adds text) -> (Entering myfunc) -> sleeps for 1s -> Decrements/Tries to decrement semaphore (race condition with parent process) -> Executes function "myprint", printing in stderr -> Increments semaphore to 1, allowing the other process to execute the "myprint" function
	Parent Execution -> Binds var "buf" (Adds text) -> (Entering myfunc) -> sleeps for 1s -> Decrements/Tries to decrement semaphore (race condition with child process) -> Executes function "myprint", printing in stderr -> Increments semaphore to 1, allowing the child process to execute the "myprint" function


2.3) Basta agora apenas 1 chamada à função "malloc" dado que, com a sincronização das tarefas dos processos pai e filho, ambos os processos conseguem ter o seu espaço de memória dinâmica, com a sua própria string, e tem acesso ordenado à escrita no stderr, não havendo sobreposição de informação

*/
