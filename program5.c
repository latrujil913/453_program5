#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "globals.h"
#include "os.h"
#include "ext2.h"
#include "SdReader.h"
#include "program5.h"
#include "program4.h"
#include "synchro.h"

volatile int isr1;
volatile int isr2;

extern uint8_t errorCode_;


FILE *fp;
uint8_t audio, request = 0;
struct ext2_dir_entry dP;
struct ext2_inode inode;
struct ext2_super_block sb;
uint8_t ramBufferPlayBack[256];
uint8_t ramBufferRead[256];
uint32_t sd_size = 10;
static struct mutex_t printLock;
void print_inode();
void print_directory();
void getBlock(void);


int main(void) {
   isr1 = 0;
   isr2 = 0;
   uint32_t blkAmount;

   serial_init();
   uint8_t sd_card_status;
   if ((sd_card_status = sdInit(1)) == 0) { //initialize the card with slow clock
      print_string("No sd card detected.\n\r");
   } else {
      print_string("Card successfully read.\n\r");
   }
   print_int(sd_card_status);
   mutex_init(&printLock);
   start_system_timer();
   start_audio_pwm();

   sei();
   get_super_block(&sb);
   get_inode_p5(2);

   //sd_card_reader();
   print_super_block();
   //blank_audio();

   delay_ms(2);
   print_inode();
   getBlock();
   print_directory();

   while(1){
      //sd_card_reader();
      //blank_audio();
      //audio_playback();
      // CMDS for keys
   }
   // os_init();
   return 0;
}

void getBlock(void){
   sdReadData(2 * 967, 0, (uint8_t *) &dP, 512);
}


void print_directory(){
   uint8_t row = 10, col = 40;

   set_cursor(row++ , col);
	print_string("inode: ");
   print_int(dP.inode);

   set_cursor(row++ , col);
   print_string("rec_len: ");
   print_int(dP.rec_len);

   set_cursor(row++ , col);
   print_string("name_len: ");
   print_int(dP.name_len);

   set_cursor(row++ , col);
   print_string("name[]: ");
   print_string(dP.name);
}

void print_inode(){
   uint8_t row = 10, col = 0;
   set_cursor(row++ , col);
	print_string("i_mode: ");
   print_int(inode.i_mode);
   set_cursor(row++ , col);
	print_string("i_mode masked: ");
   print_int(inode.i_mode & 0xF000);

   set_cursor(row++ , col);
	print_string("i_uid: ");
   print_int(inode.i_uid);

   set_cursor(row++ , col);
	print_string("i_size: ");
   print_int(inode.i_size);

   set_cursor(row++ , col);
	print_string("i_atime: ");
   print_int(inode.i_atime);

   set_cursor(row++ , col);
	print_string("i_ctime: ");
   print_int(inode.i_ctime);

   set_cursor(row++ , col);
	print_string("i_mtime: ");
   print_int(inode.i_mtime);

   set_cursor(row++ , col);
	print_string("i_dtime: ");
   print_int(inode.i_dtime);

   set_cursor(row++ , col);
	print_string("i_gid: ");
   print_int(inode.i_gid);

   set_cursor(row++ , col);
	print_string("i_links_count: ");
   print_int(inode.i_links_count);

   set_cursor(row++ , col);
	print_string("i_blocks: ");
   print_int(inode.i_blocks);

   set_cursor(row++ , col);
	print_string("i_flags: ");
   print_int(inode.i_flags);

   set_cursor(row++ , col);
   print_string("i_block[0]: ");
   print_int(inode.i_block[0]);

   set_cursor(row++ , col);
   print_string("i_block[1]: ");
   print_int(inode.i_block[1]);

   set_cursor(row++ , col);
   print_string("i_block[2]: ");
   print_int(inode.i_block[2]);

   set_cursor(row++ , col);
   print_string("i_block[3]: ");
   print_int(inode.i_block[3]);


}

void blank_audio(){
   uint8_t first = 200, second = 100;

   if (isr2 % 2 == 0){
      OCR2B =  first;
   }
   else {
      OCR2B = second;
   }
}

void sd_card_reader(void){ // producer
   uint8_t i = 0;
   // if(inode.i_mode & 0xF000 == 0x8000){
   //    print_string("D");
   // } else {
   //    print_string("F");
   // }
   print_string("Root inode size:");
   print_int(inode.i_size);
   print_string("\n\r");
}

void audio_playback(void){ // Consumer
   // uint32_t n = sizeof(int) / sizeof(ramBufferPlayBack[0]);
   uint32_t i =0;
   while (i < 256){
      OCR2B = ramBufferPlayBack[i++];
   }
}

void get_inode_p5(int inodeRequest) {
   /*
    * From an inode address (remember that they start at 1),
    * we can determine which group the inode is in, by using the formula:
    */
   int inodeGroup = (inodeRequest - 1) / sb.s_inodes_per_group;
   /*
    * Once we know which group an inode resides in, we can look up the actual
    * inode by first retrieving that block group's inode table's starting
    * address(see Block Group Descriptor above). The index of our inode in this
    * block group's inode table can be determined by using the formula:
    */
   int grpOffset = (inodeRequest - 1) % sb.s_inodes_per_group;
   int subBlkOffset = (grpOffset * EXT2_GOOD_OLD_INODE_SIZE) / 512;
   int inodeSubBlkOffset = grpOffset % ( 512 / EXT2_GOOD_OLD_INODE_SIZE);

   // block formula
   int blk = inodeGroup * 2 * sb.s_blocks_per_group + 2 *  5 + subBlkOffset;
   // offset
   int off = inodeSubBlkOffset * sizeof(struct ext2_inode);

   sdReadData(blk, off, (uint8_t *)&inode, sizeof(struct ext2_inode));
   return;
}

void get_super_block(struct ext2_super_block *sb) {
   sdReadData(2 * 1, 0, (uint8_t *) sb,
         512);

   return;
}

void idle(void){
   while(1){

   }
}

void print_super_block() {
   uint8_t row = 5, col = 0;


   set_cursor(row++ , col);
   print_int(errorCode_);
	print_string("sdCardSize: ");
   print_int32(sdCardSize());

   set_cursor(row++ , col);
	print_string("Blocks count: ");
   print_int32(sb.s_blocks_count);
   print_int32(sdCardSize());
   set_cursor(row++ , col);
	print_string("Reserved blocks count: ");
   print_int(sb.s_r_blocks_count);
   set_cursor(row++ , col);
	print_string("Free blocks count: ");
   print_int(sb.s_free_blocks_count);
   set_cursor(row++ , col);
	print_string("Free inodes count: ");
   print_int(sb.s_free_inodes_count);
   set_cursor(row++ , col);
	print_string("First data block: ");
   print_int(sb.s_first_data_block);
   set_cursor(row++ , col);
	print_string("Blocks per group: ");
   print_int(sb.s_blocks_per_group);
   set_cursor(row++ , col);
	print_string("Inodes per group: ");
   print_int(sb.s_inodes_per_group);
   set_cursor(row++ , col);
}
