#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "globals.h"
// #include "os.h"
// #include "ext2.h"
#include "SdReader.h"
#include "program5.h"
// #include "program4.h"
// #include "synchro.h"

volatile int isr1;
volatile int isr2;

int main(void) {
   int test = 0;
   isr1 = 0;
   isr2 = 0;
   serial_init();
   // clear_screen();

   uint8_t sd_card_status;

   // using sdInit, doesnt allow printing
   // need the bread board on it
   // sd_card_status = sdInit(1);
   if (( sdInit(1)) == 0) { //initialize the card with slow clock
      print_string("No sd card detected.\n\r");
   } else {
      print_string("Card can be read.\n");
   }
   // print_int(sd_card_status);

   // print_int((uint16_t)sd_card_status);
   // print_string("hello");
   start_system_timer();
   start_audio_pwm();
   while(1){
      print_string("isr1 = ");
      print_int(isr1);
      print_string("isr2 = ");
      print_int(isr2);
   }

   // os_init();

   return 0;
}
