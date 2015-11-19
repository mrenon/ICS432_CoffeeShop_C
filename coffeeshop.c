/********************************
* This is a C program that sumulates a coffeeshop bathroom
* takes in 2 command line arguments
*
* @author Mauricio Renon
* adapted from Henri Casanova
*
* ICS 432
* programming assignment 3
*
********************************/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define SIZE 100
int key = 2;

struct arguments {
  int threadName;
  pthread_mutex_t *lock;
  pthread_cond_t *cond;
};

void *do_work(void *arg);

// This program has ten threads run a coffee shop bathroom simulation
// in which no more than 2 threads can use the bathroom

int main(int argc, char **argv) {

  int numThreads = atoi(argv[1]);
  pthread_t worker_thread[numThreads];
  pthread_mutex_t mutex;
  pthread_cond_t condition;
  struct arguments *arg;
  pthread_cond_init(&condition,NULL); //initializing the locks
  pthread_mutex_init(&mutex,NULL);

  srand(atoi(argv[2]));


  // Creating child threads
  //printf("Thread ", id, "enters the coffee shop"); print what thread entered
  for (int t=0; t < numThreads; t++) {
    arg = (struct arguments *)calloc(1, sizeof(struct arguments));
    arg->threadName = t;
    arg->lock = &mutex;
    arg->cond = &condition;
    if (pthread_create(&worker_thread[t], NULL, do_work, (void *)arg)) { 
      fprintf(stderr,"Error while creating thread #%d\n",arg->threadName);
      exit(1);
    }
    //fprintf(stdout,"Waiting for child thread to complete\n");
  }
  
  // Joining with child threads
  for (int t=0; t < 10; t++) {
    if (pthread_join(worker_thread[t], NULL)) {
      fprintf(stderr,"Error while joining with child thread #%d\n",arg->threadName);
      exit(1);
    }
  }
  exit(0); 
}

void *do_work(void *arg) {
  struct arguments *thread;
  pthread_mutex_t *mutexLock;
  pthread_cond_t *conditionVar;
  thread = (struct arguments*)arg;
  mutexLock = thread->lock;
  conditionVar = thread->cond;


  printf("Thread %d has enters the coffee shop\n" , thread-> threadName); //enter
  
  for(int i = 0; i < 10; i++){
  
    printf("Thread %d is drinking coffee\n" , thread-> threadName); //drink
    usleep(rand()% 3000000 + 2000000);


    pthread_mutex_lock(mutexLock);
    while(key == 0){
      printf("Thread %d is waiting for a key\n", thread->threadName); //wait for key
      pthread_cond_wait(conditionVar, mutexLock);
    }
    key--;
    pthread_mutex_unlock(mutexLock);

     printf("Thread %d got a key\n", thread-> threadName);  //get key
     printf("Thread %d is using the bathroom\n", thread-> threadName); //use bathroom
     usleep(rand()% 3000000 + 2000000);
     
     pthread_mutex_lock(mutexLock);
     key++;
     pthread_mutex_unlock(mutexLock);

    printf("Thread %d put a key back on the board\n", thread-> threadName); //release key
    pthread_cond_broadcast(conditionVar);
  }
  printf("Thread %d leaves the coffee shop\n",  thread-> threadName); //leave
  return NULL;
}

