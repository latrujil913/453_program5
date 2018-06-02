#include <avr/io.h>
#include <stdio.h>
#include "globals.h"
#include "os.h"

// extern system_t *memBegin;
// extern volatile int global;
extern int numThreads;
/*
 * Initialize the serial port.
 */
void serial_init() {
   uint16_t baud_setting;

   UCSR0A = _BV(U2X0);
   baud_setting = 16; //115200 baud

   // assign the baud_setting
   UBRR0H = baud_setting >> 8;
   UBRR0L = baud_setting;

   // enable transmit and receive
   UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
}

/*
 * Return 1 if a character is available else return 0.
 */
uint8_t byte_available() {
   return (UCSR0A & (1 << RXC0)) ? 1 : 0;
}

/*
 * Unbuffered read
 * Return 255 if no character is available otherwise return available character.
 */
uint8_t read_byte() {
   if (UCSR0A & (1 << RXC0)) return UDR0;
   return 255;
}

/*
 * Unbuffered write
 *
 * b byte to write.
 */
uint8_t write_byte(uint8_t b) {
   //loop until the send buffer is empty
   while (((1 << UDRIE0) & UCSR0B) || !(UCSR0A & (1 << UDRE0))) {}

   //write out the byte
   UDR0 = b;
   return 1;
}

/* calls write_byte on every letter
 *
 */
void print_string(char* s) {
    //write_byte(s[0]);
    for(int i = 0; s[i] && s[i] != '\0'; i++)
      write_byte(s[i]);
}
/*
ATTENTION JORDAN
---> ASK PROFESSOR SENGH:
   "Does it matter that my giveInt function accepts uint32_t,
   and my print_int32 and print_int are sending values of type
   uint32_t and uint16_t respectively"

*/
char *giveInt(uint32_t val) {
   int i = 31;
   int lastI = 0;
   int remainder = val;
   static char buffer[32] = {0};
   buffer[i] = '\0';

   for(i = 30; i >= 0, remainder > 0; remainder /= 10, i--) {
      buffer[i] = (remainder % 10);
      buffer[i] = buffer[i] + '0';
      lastI = i;
   }
   return &buffer[lastI];
}
/* given an integer up to 16 bits in size
 * will convert int to equivalent string
 * and call print_string() with said string
 */
void print_int(uint16_t val) {
   if(val == 0){
      write_byte(0x30);
      return;
   }
   char *c = giveInt(val);
   while(*c != '\0'){
      write_byte(*c);
      c++;
   }
}
/* Nearly identical to print_int(),
 * but handles larger numbers
 */
void print_int32(uint32_t val) {
   char *c = giveInt(val);
   while(*c != '\0'){
      write_byte(*c);
      c++;
   }
}
/* Takes an integer, up to 16 bits in size,
 * Converts the integer to its string equivalent
 * Send the string to print_string to print
 */
void print_hex(uint16_t i) {
   char hexChars[] = "0123456789ABCDEF";
   char printMe[32] = {0};
   int index = 31, val = i;
   printMe[index--] = '\0';
   do{
      printMe[index--] = hexChars[val % 16];
      val = val / 16;
   }while(val > 0);

   write_byte(0x30); //0
   write_byte(0x78); //x
   while(index >= 27) //
      printMe[index--] = '0';
   index++;
   while(printMe[index] != '\0')
      write_byte(printMe[index++]);
}
/*
 *
 */
void print_hex32(uint32_t i) {
   char hexChars[] = "0123456789ABCDEF";
   char printMe[32] = {0};
   int index = 31, val = i;
   printMe[index--] = '\0';
   do{
      printMe[index--] = hexChars[val % 16];
      val = val / 16;
   }while(val > 0);

   write_byte(0x30); //0
   write_byte(0x78); //x
   while(index >= 27) //
      printMe[index--] = '0';
   index++;
   while(printMe[index] != '\0')
      write_byte(printMe[index++]);
}

/* given two integers (max size of 8bits)
 * will set the cursor to the position specified by (row, col)
 *
 */
void set_cursor(uint8_t row, uint8_t col) {
   char *r, *c;
   r = giveInt(row);
   c = giveInt(col);

   write_byte(0x1B); //ESC
   write_byte(0x5B); // [
   print_int(row);
/*
   while(*r != '\0') { //{ROW}'s
      write_byte(*r);
      r++;
   }
*/
   write_byte(0x3B); // ;
   print_int(col);
   /*
   while(*c != '\0') { // {COL}'s
      write_byte(*c);
      c++;
   }
   */
   write_byte(0x48); // H
}

/* set the foreground color
 *   <ESC>[{attr1};...;{attrn}m
 */
void set_color(uint8_t color) {
    write_byte(0x1B); //ESC
    write_byte(0x5B); // [
    write_byte(0x33); //{attr}
    write_byte(color); //{attr}
    write_byte(0x6D); // m
}

void lightOn(void) {
   asm volatile("ldi r31, 0x00");

   asm volatile("ldi r30, 0x24"); //move Z ptr to DDRB == 0x0024
   asm volatile("ld r21, Z");  //save value in DDRB to r21
   asm volatile("ldi r22, 0x80"); //prepare r22 value, soon to be in DDRB
   asm volatile("st Z, r22"); //st r22 value into DDRB

   asm volatile("ldi r30, 0x25"); //move Z ptr to PORTB
   asm volatile("ld r21, Z");  //save value in PORTB to r21
   asm volatile("ldi r22, 0x80");  //prepare r22 value, soon to be PORTB
   asm volatile("st Z, r22");
}
void lightOff(void) {
   asm volatile("ldi r31, 0x00");

   asm volatile("ldi r30, 0x24"); //move Z ptr to DDRB
   asm volatile("ld r21, Z");  //save value in DDRB to r21
   asm volatile("ldi r22, 0x80"); //prepare r22 value, soon to be in DDRB
   asm volatile("st Z, r22"); //st r22 value into DDRB

   asm volatile("ldi r30, 0x25"); //move Z ptr to PORTB
   asm volatile("ld r21, Z");  //save value in PORTB to r21
   asm volatile("ldi r22, 0x00");  //prepare r22 value, soon to be PORTB
   asm volatile("st Z, r22");
}
void delay_ms(int ms) {
   int i;
   for(i = 0; i < ms; i++)
      _delay_ms(1);
}

void blink(uint16_t *rate) {
   uint8_t keyboardValue = 'o';
   int blinkRate = (int)*rate;
   while(1){
      // set_cursor(10,10);
      // print_int(blinkRate);
      keyboardValue = read_byte();
      lightOn();
      delay_ms(blinkRate);
      lightOff();
      delay_ms(blinkRate);
      if(keyboardValue == 'a'){
         //blinkRate = blinkRate > 20 ? blinkRate - 20 : blinkRate;
         blinkRate -= 20;
      }
      if(keyboardValue == 'z'){
         //blinkRate = blinkRate < 1000 ? blinkRate + 20 : blinkRate;
         blinkRate += 20;
      }
   }
}//end blink

void clear_screen(void) {
   write_byte(0x1B); //ESC
   write_byte(0x5B); // [
   write_byte(0x32); // 2
   write_byte(0x4A); // J
}

void nada(void){
   while(1){
      //do nothing
   }
}

// void printStats(void){
//    //start at row 1, for reasons I dont want to explain
//    uint8_t row = 4, i = 0;
//
//    while(1){
//       if(global < 10){  //garbage prints immediately, this cleans that up
//          clear_screen();
//       }
//       row = 4;
//       set_cursor(0, 0);
//       set_color(YELLOW);
//       print_string("Timer: ");
//       print_int32((global/100));
//       print_string("                             ");
//       set_cursor(2, 0);
//       print_string("Number of Threads: ");
//       print_int(memBegin -> numThreads);
//       for(int i = 0; i < memBegin -> numThreads; i++){ //print stats of each thread
//          set_color(GREEN);
//          set_cursor(row++, 0);
//          print_string("Thread Id: ");
//          print_int(memBegin->threads[i].id);
//          set_cursor(row++, 0);
//          print_string("Thread name: ");
//          print_string(memBegin->threads[i].tName);
//          set_cursor(row++, 0);
//          print_string("Thread PC: ");
//          print_hex(memBegin->threads[i].PC);
//          set_cursor(row++, 0);
//          print_string("Stack Usage: ");
//          print_int((uint16_t)memBegin->threads[i].stackEnd -
//             (uint16_t)memBegin->threads[i].stackPointer);
//          print_string(" bytes   ");
//          set_cursor(row++, 0);
//          print_string("Stack Size: ");
//          print_int(memBegin->threads[i].stackSize);
//          print_string(" bytes   ");
//          set_cursor(row++, 0);
//          print_string("Current top of stack: ");
//          print_hex((uint16_t)memBegin->threads[i].stackPointer);
//          set_cursor(row++, 0);
//          print_string("Stack base: ");
//          print_hex((uint16_t)memBegin->threads[i].stackBase);
//          set_cursor(row++, 0);
//          print_string("Stack end: ");
//          print_hex((uint16_t)memBegin->threads[i].stackEnd);
//          row++;
//       }
//    }
// }
