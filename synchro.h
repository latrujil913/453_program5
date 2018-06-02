#ifndef SYNC_H
#define SYNC_H

//includes necessary...

//Structures....
typedef struct semaphore_t{
   int value;
}semaphore_t;

typedef struct mutex_t{
   int volatile lock;
   //int* waitList;
   int volatile waitList[8];
   int volatile waiting;
}mutex_t;

//just do nothing functions...
void consumer1(void);
void producer1(void);
void display_bounded_buffer1(void);



//function constructors...
void printtt(void);
void nothing(void);
void just_animations(void);


void mutex_init(struct mutex_t** m);
void mutex_lock(struct mutex_t** m);
void mutex_unlock(struct mutex_t** m);
void display_stats(void);
void blink_V2(void);
void consumer(void);
void producer(void);
void display_bounded_buffer(void);

void seminit(struct semaphore_t* s, int8_t value);
void sem_wait(struct semaphore_t* s);
void sem_signal(struct semaphore_t* s);
void sem_signal_swap(struct semaphore_t* s);



//structures, enums, globals...
enum state {THREAD_RUNNING, THREAD_READY, THREAD_SLEEPING, THREAD_WAITING};
//THREAD_RUNNING - instructions currently being processed by processor
//THREAD_READY - When the thread is ready to run, and waiting on a processor
//THREAD_SLEEPING - when the thread is asleep after a thread_asleep call
//THREAD_WAITING - after a call to mutex or semaphore fails

#endif
