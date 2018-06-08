#include "globals.h"
#include "os.h"
#include "synchro.h"
#include <avr/interrupt.h>

extern system_t *memBegin;
extern mutex_t printLock;
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

void mutex_init(struct mutex_t* m)
{
   m -> lock = 1; //1 = unlocked, 0 = locked
   m -> waiting = 0;
   m -> owner = 0;
}


void mutex_lock(struct mutex_t* m){
   cli();
   //print_string("  \"mutex-lock - CLI\"  ");
   if(!m -> lock){ //lock unavailable
      m -> waiting = memBegin -> runningThread;
      memBegin -> threads[memBegin -> runningThread].curState = THREAD_WAITING;
      yieldGiven();
      //after we return, WE own the lock
      m -> lock = 0;
      m -> owner = memBegin -> runningThread;
      m -> waiting = 0;
   }
   else{
      m -> lock = 0;
      m -> owner = memBegin -> runningThread;
      memBegin -> threads[m -> waiting].curState = THREAD_READY;
   }
   sei();
   //print_string("  \"mutex-lock - SEI\"  ");
}


void mutex_unlock(struct mutex_t* m){
   cli();
   //print_string("  \"mutex-unlock - CLI\"  ");
   if(m -> owner == memBegin -> runningThread){
      m -> lock = 1;
      memBegin -> threads[m -> waiting].curState = THREAD_READY;
      yieldGiven();
      m -> waiting = 0;
   }
   sei();
   //print_string("  \"mutex-unlock - SEI\"  ");
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
