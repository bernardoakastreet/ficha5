#include <pthread.h>
#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void myprint(const char *str);
void *mythread(void *);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
  char *buf;
  pthread_t tid1, tid2;
  if(pthread_mutex_init(&mutex, NULL)) printf("Não foi possível criar mutex");

  printf("Identificador do processo: %d\n", getpid());

  buf = (char *) malloc(256);
  sprintf(buf, "%d: Ola -------------------------------------------------------------\n",getpid());
  printf("Nova thread criada (tid1), fazendo a chamada à função mythread(buf) \n");
  pthread_create(&tid1, NULL, mythread, buf);

  //experimente também o programa com a linha seguinte comentada
  buf = (char *) malloc(256);
  sprintf(buf, "%d: Ole +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n",getpid());
  printf("Nova thread criada (tid2), fazendo a chamada à função mythread(buf) \n");
  pthread_create(&tid2, NULL, mythread, buf);

  printf("Pausa de 5 segundos na thread principal\n");
  sleep(5);

  printf("Thread principal vai aguardar que a thread identificada por tid1 termine.\n");
  //experimente também o programa com a linha seguinte comentada
  pthread_join(tid1, NULL);

  return(0);
}

void *mythread(void *arg){
  while(1) {
    sleep(1);
    pthread_mutex_lock(&mutex);
    myprint((char *)arg);
    pthread_mutex_unlock(&mutex);
  }
}

/*

*/
