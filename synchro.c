#include "globals.h"
#include "os.h"
#include "synchro.h"
#include <avr/interrupt.h>

extern system_t *memBegin;
extern mutex_t *printLock;
extern volatile int global;
extern int numThreads;
volatile int buffer[10];


void printState(enum state curState);

void producer1(void){
   while(1){
      mutex_lock(&printLock);
      mutex_unlock(&printLock);
   }
}
void display_bounded_buffer1(void){
   while(1){
      mutex_lock(&printLock);
      mutex_unlock(&printLock);
   }
}
void consumer1(void){
   while(1){
      mutex_lock(&printLock);
      mutex_unlock(&printLock);
   }
}

void printtt(void){
   int col = 0, row = 2;
   while(1){
      mutex_lock(&printLock);
      col = 0;
      row = 2;
       if(global < 10)
          clear_screen();
      set_cursor(row++, col);
      print_string("thread name: ");
      print_string(memBegin -> threads[1].tName);
      set_cursor(row++, col);
      print_string("thread state: ");
      printState(memBegin -> threads[1].curState);
      row++;
      set_cursor(row++, col);
      print_string("thread name: ");
      print_string(memBegin -> threads[2].tName);
      set_cursor(row++, col);
      print_string("thread state: ");
      printState(memBegin -> threads[2].curState);

      row = 2;
      col = 35;
      set_cursor(row++, col);
      print_string("waiting = ");
      print_int(printLock -> waiting);
      set_cursor(row++, col);
      print_int(printLock -> waitList[0]);
      set_cursor(row++, col);
      print_int(printLock -> waitList[1]);
      set_cursor(row++, col);
      print_int(printLock -> waitList[2]);
      set_cursor(row++, col);
      print_int(printLock -> waitList[3]);
      set_cursor(row++, col);
      print_int(printLock -> waitList[4]);
      set_cursor(row++, col);
      print_int(printLock -> waitList[5]);
      set_cursor(row++, col);
      print_int(printLock -> waitList[6]);
      set_cursor(row++, col);
      print_int(printLock -> waitList[7]);
      mutex_unlock(&printLock);
   }
}
void nothing(void){
   while(1){
   mutex_lock(&printLock);
   mutex_unlock(&printLock);
   }
}

void printState(enum state curState){
   switch(curState){
      case THREAD_READY:
         print_string("THREAD_READY   ");
         break;
      case THREAD_WAITING:
         print_string("THREAD_WAITING ");
         break;
      case THREAD_RUNNING:
         print_string("THREAD_RUNNING  ");
         break;
      case THREAD_SLEEPING:
         print_string("THREAD_SLEEPING");
         break;
   }
}

void mutex_init(struct mutex_t** m)
{
   //m = (mutex_t*)malloc(sizeof(mutex_t));
   (*m) -> lock = 1;
   (*m) -> waiting = 0;
   //m -> waitList = malloc(sizeof(int) * 8);
   (*m) -> waitList[0] = 0;
   (*m) -> waitList[1] = 0;
   (*m) -> waitList[2] = 0;
   (*m) -> waitList[3] = 0;
   (*m) -> waitList[4] = 0;
   (*m) -> waitList[5] = 0;
   (*m) -> waitList[6] = 0;
   (*m) -> waitList[7] = 0;
   //print_string("   here");
   //print_int(m -> lock);
   //print_string("here   ");
}
void mutex_lock(struct mutex_t** m){
   cli();
   if((*m) -> lock < 1){ //lock unavailable
      //memBegin -> threads[memBegin -> runningThread].curState = THREAD_WAITING;
      (*m) -> waitList[(*m) -> waiting] = memBegin -> runningThread; //add to waitlist
      (*m) -> waiting++;
         //with this implementation, we need to reorder the array in mutex_unlock
      memBegin -> threads[memBegin -> runningThread].curState = THREAD_WAITING;
      yield();
   }
   else{
      (*m) -> lock = 0;
   }
   sei();
}
/*
   Simple function to resort the array after taking
      the first member of the waitlist
*/
void siftArray(struct mutex_t** p, int count){
   int i;
   for(i = 0; i < count - 1; i++){
      (*p) -> waitList[i] = (*p) -> waitList[i + 1];
   }
}
void mutex_unlock(struct mutex_t** m){
   cli();
   (*m) -> lock = 1;
   //change the state of the first thread on waitlist
   //so that get_next_thread will actually call it
   if((*m) -> waiting > 0){
      memBegin -> threads[(*m) -> waitList[0]].curState = THREAD_READY;
      siftArray(m, 8);
      (*m) -> waiting--;

   }
   sei();
}


void thread_sleep(uint16_t ticks){

}

void seminit(struct semaphore_t* s, int8_t value){
   s = (struct semaphore_t*)malloc(sizeof(semaphore_t));
   s->value = value;
}

void sem_wait(struct semaphore_t* s){
   cli();
   s->value--;
   if (s->value < 0) {
      // block();
   }
   sei();
}

void sem_signal(struct semaphore_t* s){
   cli();
   s->value++;
   if (s->value <= 0) {
      // wakeup(P);
   }
   sei();
}

void sem_signal_swap(struct semaphore_t* s){

}

void blink_V2(void){

}
/*
   Just the function for the consumer animation.
   call it once to iterate the animation once
*/
void consumer_anim(void){
   uint8_t row = 2, col = 0;
   uint8_t setColor = CYAN, base = WHITE;
   int frame = 0;
   while(1){
      row = 2;
      col = 0;
   //this might not be necessary in the final product, but just incase saved here
   //   if(global < 10)
   //      clear_screen();
      if(frame >= 0 && frame <= 20){
         set_cursor(row++, col);
         print_string("               ");
         set_cursor(row++, col);
         print_string("   #   ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string(" #####  ");
         set_cursor(row++, col);
         print_string("#######");
         set_cursor(row++, col);
         print_string(" #####  ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string("   #   ");
         frame++;
      }
      else if(frame > 20 && frame <= 40){
         set_cursor(row++, col);
         print_string("                ");
         set_cursor(row++, col);
         set_color(setColor);
         print_string("   0   ");
         set_cursor(row++, col);
         set_color(setColor);
         print_string("  0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0  ");
         set_cursor(row++, col);
         print_string(" 0");
         set_color(base);
         print_string("###");
         set_color(setColor);
         print_string("0  ");
         set_cursor(row++, col);
         print_string("0");
         set_color(base);
         print_string("#####");
         set_color(setColor);
         print_string("0");
         set_cursor(row++, col);
         print_string(" 0");
         set_color(base);
         print_string("###");
         set_color(setColor);
         print_string("0  ");
         set_cursor(row++, col);
         print_string("  0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0  ");
         set_cursor(row++, col);
         print_string("   0   ");
         frame++;
      }
      else if(frame > 40 && frame <= 60){
         set_cursor(row++, col);
         print_string("CONSUMING");
         set_cursor(row++, col);
         print_string("   #   ");
         set_cursor(row++, col);
         print_string("  #");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#  ");
         set_cursor(row++, col);
         print_string(" #");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#  ");
         set_cursor(row++, col);
         print_string("#");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("###");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#");
         set_cursor(row++, col);
         print_string(" #");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#  ");
         set_cursor(row++, col);
         print_string("  #");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#  ");
         set_cursor(row++, col);
         print_string("   #   ");
         frame++;
      }
      else if(frame > 60 && frame <= 80){
         set_cursor(row++, col);
         print_string("               ");
         set_cursor(row++, col);
         print_string("   #   ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string(" ##");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("##  ");
         set_cursor(row++, col);
         print_string("##");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("##");
         set_cursor(row++, col);
         print_string(" ##");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("##  ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string("   #   ");
         frame++;
      }
      else if(frame > 80 && frame <= 100){
         set_cursor(row++, col);
         print_string("CONSUMING");
         set_cursor(row++, col);
         print_string("   #   ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string(" #####  ");
         set_cursor(row++, col);
         print_string("###");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("###");
         set_cursor(row++, col);
         print_string(" #####  ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string("   #   ");
         frame++;
      }
      else if(frame == 101){
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         frame++;
      }
      else
         return;
   }
   return;
}
/*

*/
void consumer(void){
   while(1){
      mutex_lock(&printLock);
      // if(memBegin -> threads[memBegin -> runningThread].curState == THREAD_RUNNING)
          consumer_anim();
      mutex_unlock(&printLock);
   }
}
/*

*/
void producer_anim(void){
   uint8_t row = 26, col = 20;
   uint8_t setColor = CYAN, base = WHITE;
   int frame = 0;
   while(1){
      set_color(base);
      row = 26;
      col = 20;
      //if(global < 10)
      //   clear_screen();
      if(frame >= 0 && frame <= 20){
         set_cursor(row++, col);
         print_string("               ");
         set_cursor(row++, col);
         print_string("   #   ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string(" #####  ");
         set_cursor(row++, col);
         print_string("#######");
         set_cursor(row++, col);
         print_string(" #####  ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string("   #   ");
         frame++;
      }
      else if(frame > 20 && frame <= 40){
         set_cursor(row++, col);
         print_string("PRODUCING");
         set_cursor(row++, col);
         print_string("   #   ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string(" #####  ");
         set_cursor(row++, col);
         print_string("###");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("###");
         set_cursor(row++, col);
         print_string(" #####  ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string("   #   ");
         frame++;
      }
      else if(frame > 40 && frame <= 60){
         set_cursor(row++, col);
         print_string("               ");
         set_cursor(row++, col);
         print_string("   #   ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string(" ##");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("##  ");
         set_cursor(row++, col);
         print_string("##");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("##");
         set_cursor(row++, col);
         print_string(" ##");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("##  ");
         set_cursor(row++, col);
         print_string("  ###  ");
         set_cursor(row++, col);
         print_string("   #   ");
         frame++;
      }
      else if(frame > 60 && frame <= 80){
         set_cursor(row++, col);
         print_string("PRODUCING");
         set_cursor(row++, col);
         print_string("   #   ");
         set_cursor(row++, col);
         print_string("  #");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#  ");
         set_cursor(row++, col);
         print_string(" #");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#  ");
         set_cursor(row++, col);
         print_string("#");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("###");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#");
         set_cursor(row++, col);
         print_string(" #");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#  ");
         set_cursor(row++, col);
         print_string("  #");
         set_color(setColor);
         print_string("0");
         set_color(base);
         print_string("#  ");
         set_cursor(row++, col);
         print_string("   #   ");
         frame++;
      }
      else if(frame > 80 && frame <= 100){
         set_cursor(row++, col);
         print_string("                ");
         set_cursor(row++, col);
         set_color(setColor);
         print_string("   0   ");
         set_cursor(row++, col);
         set_color(setColor);
         print_string("  0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0  ");
         set_cursor(row++, col);
         print_string(" 0");
         set_color(base);
         print_string("###");
         set_color(setColor);
         print_string("0  ");
         set_cursor(row++, col);
         print_string("0");
         set_color(base);
         print_string("#####");
         set_color(setColor);
         print_string("0");
         set_cursor(row++, col);
         print_string(" 0");
         set_color(base);
         print_string("###");
         set_color(setColor);
         print_string("0  ");
         set_cursor(row++, col);
         print_string("  0");
         set_color(base);
         print_string("#");
         set_color(setColor);
         print_string("0  ");
         set_cursor(row++, col);
         print_string("   0   ");
         frame++;
      }
      else if(frame == 101){
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         set_cursor(row++, col);
         print_string("        ");
         frame++;
      }
      else
         return;
   }
}

/*
*/
void producer(void){
   int i = 5;
   for (i = 0; i < 5; i++)
      producer_anim();
   return;
}

/*

*/
void display_bounded_buffer(void){
   int row = 1;
      int i;
      int col = 40;
      int count = 0;

      set_cursor(row, col);

      for (i = 0; i < 10; i++){
         buffer[i] = 0;
      }

      while(count != 10){
         row = 1;
         col = 40;
         set_cursor(row , col);

         buffer[count++]  = 1;
         set_cursor(row++, col);
         print_string("+--------------------+");
         set_cursor(row++, col);
         print_string("| Bounded Buffer     |");
         set_cursor(row++, col);
         print_string("+--------------------+");
         if (buffer[0] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);
            print_string("|####################|");
         }
         set_cursor(row++, col);
         print_string("+--------------------+");
         if (buffer[1] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);
            print_string("|####################|");
         }
         set_cursor(row++, col);

         print_string("+--------------------+");
         if (buffer[2] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);
            print_string("|####################|");
         }
         set_cursor(row++, col);
         print_string("+--------------------+");
         if (buffer[3] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);
            print_string("|####################|");
         }
         set_cursor(row++, col);
         print_string("+--------------------+");
         if (buffer[4] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);
            print_string("|####################|");
         }
         set_cursor(row++, col);
         print_string("+--------------------+");
         if (buffer[5] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);
            print_string("|####################|");
         }
         set_cursor(row++, col);
         print_string("+--------------------+");
         if (buffer[6] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);

            print_string("|####################|");
         }
         set_cursor(row++, col);

         print_string("+--------------------+");
         if (buffer[7] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);
            print_string("|####################|");
         }
         set_cursor(row++, col);
         print_string("+--------------------+");
         if (buffer[8] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);
            print_string("|####################|");
         }
         set_cursor(row++, col);
         print_string("+--------------------+");
         if (buffer[9] == 0){
            set_cursor(row++, col);
            print_string("|                    |");
         }
         else{
            set_cursor(row++, col);
            print_string("|####################|");
         }
         set_cursor(row++, col);
         print_string("+--------------------+");

      }

      return;
}

void just_animations(void){
   while(1){
      clear_screen();
      consumer_anim();
      clear_screen();
      producer_anim();
clear_screen();
      display_bounded_buffer();

   }
}
/*

*/
void display_stats(void){
   //start at row 1, for reasons I dont want to explain
   uint8_t row = 4, col = 0, i = 0;

   while(1){
      mutex_lock(&printLock);
      if(global < 10){  //garbage prints immediately, this cleans that up
         clear_screen();
      }
      row = 4;
      set_cursor(0, 0);
      set_color(YELLOW);
      print_string("Timer: ");
      print_int32((global/100));
      print_string("                             ");
      set_cursor(2, 0);
      print_string("Number of Threads: ");
      print_int(memBegin -> numThreads);
      for(i = 0; i < memBegin -> numThreads; i++){ //print stats of each thread
         if(i < 3){
            col = (i % 3) * 33;
            row = 4;
         }
         else if (i > 2){
            col = (i % 3) * 33;
            row = 15;
         }

         set_color(GREEN);
         set_cursor(row++, col);
         print_string("Thread Id: ");
         print_int(memBegin->threads[i].id);
         set_cursor(row++, col);
         print_string("Thread name: ");
         print_string(memBegin->threads[i].tName);
         set_cursor(row++, col);
         print_string("Thread PC: ");
         print_hex(memBegin->threads[i].PC);
         set_cursor(row++, col);
         print_string("Stack Usage: ");
         print_int((uint16_t)memBegin->threads[i].stackEnd -
            (uint16_t)memBegin->threads[i].stackPointer);
         print_string(" bytes   ");
         set_cursor(row++, col);
         print_string("Stack Size: ");
         print_int(memBegin->threads[i].stackSize);
         print_string(" bytes   ");
         set_cursor(row++, col);
         print_string("Current top of stack: ");
         print_hex((uint16_t)memBegin->threads[i].stackPointer);
         set_cursor(row++, col);
         print_string("Stack base: ");
         print_hex((uint16_t)memBegin->threads[i].stackBase);
         set_cursor(row++, col);
         print_string("Stack end: ");
         print_hex((uint16_t)memBegin->threads[i].stackEnd);
         set_cursor(row++, col);
         print_string("Sched. Count: ");
         print_int(memBegin -> threads[memBegin -> runningThread].sched_count);
         set_cursor(row++, col);
         print_string("Thread Status: ");
         printState(memBegin -> threads[i].curState);
         row++;
      }
      // set_cursor(35, 35);
      // print_string("waitlist[0]: ");
      // print_int(printLock -> waitList[0]);
      // print_string("- lock: ");
      // print_int(printLock -> lock);
   mutex_unlock(&printLock);
      // set_cursor(36, 35);
      // print_string("waitlist[0]post unlock: ");
      // print_int(printLock -> waitList[0]);
      // print_string("- lock: ");
      // print_int(printLock -> lock);
   }
}
