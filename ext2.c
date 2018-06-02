#include <avr/io.h>
#include <avr/interrupt.h>
#include "program4.h"
#include "globals.h"
#include "os.h"
#include "synchro.h"
#include <stdint.h>

FILE *fp;

/******************************************************************************
 * Program 4
 * ---------
 * Desc: Program that reads a Linux ext2 filesystem. Program is
 *       able to list the files/directories and display the contents of files.
 *
 * Parameter:
 *    Command line args that will hold:
 *     - file system to be opened
 *     - path to file if '-l' for file or just path for directory
 *     - default root path if no path or file is given
 *****************************************************************************/
// int main(int argc, char *argv[]) {
//    char *fileLocation = 0;
//    struct ext2_super_block sb;
//    struct ext2_inode inode;
//    struct ext2_dir_entry *dirEntry[256];
//    struct ext2_inode inode2;
//    struct ext2_dir_entry *entry;
//    char typeBuffer[256], nameBuffer[256];
//
//    uint32_t indirect[256], doubleIndirect[256];
//    uint32_t i = 0, j = 0;
//
//
//
//    // Take in a command line argument
//    if(argc != 2 && argc != 3 && argc != 4){
//       fprintf(stderr, "Usage: ./ext2reader [name of file system] [path or path to file] \n"/*, 15*/);
//       exit(0);
//    }
//
//    // File I/O
//    if(argc == 4){
//       if((fp = fopen(argv[2], "r")) == NULL){
//          fprintf(stderr, "file not found\n"/*, 15*/);
//          exit(0);
//       }
//       fileLocation = argv[3];
//    }
//    else{
//       if((fp = fopen(argv[1], "r")) == NULL) {
//          fprintf(stderr, "file not found\n"/*, 15*/);
//          exit(0);
//       }
//       if(argc == 2) {
//          fileLocation = "/";
//       } else {
//          fileLocation = argv[2];
//       }
//    }
//
//    // printf("%s\n", fileLocation);
//    read_data(2 * 1, 0, (uint8_t *)&sb, sizeof(struct ext2_super_block));
//
//    char curLocation[256];
//    uint8_t exists = 0;
//
//    // Get root
//    get_inode(&inode, &sb, 2);
//    if(strcmp(fileLocation, "/") == 0) {
//       exists = 1;
//    } else{
//       curLocation[0] = 0;
//       exists = find_file(&inode, &sb, fileLocation, curLocation, 0);
//    }
//
//    if(exists == 1) {
//       if((inode.i_mode & 0xF000) == 0x4000) {
//          uint32_t dirEntryAmt = entry_amount(&inode, dirEntry), i = 0;
//
//          qsort(dirEntry, dirEntryAmt, sizeof(struct ext2_dir_entry *), cmpfunc);
//          printf("name   size  type\n");
//          for(i = 0; i < dirEntryAmt; i++) {
//             entry = dirEntry[i];
//
//             if(1 && entry->inode) {
//                get_inode(&inode2, &sb, entry->inode);
//
//                strncpy(nameBuffer, entry->name, entry->name_len);
//                nameBuffer[entry->name_len] = '\0';
//
//                printf("%s\t", nameBuffer);
//
//                // determine file type to print
//                if((inode2.i_mode & 0xF000) == 0x8000) {
//                   strcpy(typeBuffer, "F");
//                }
//                else if((inode2.i_mode & 0xF000) == 0x4000) {
//                   strcpy(typeBuffer, "D");
//                }
//                else {
//                   strcpy(typeBuffer, "Unsupported type");
//                }
//
//                if((inode2.i_mode & 0xF000) == 0x8000) {
//                   printf("%d\t", inode2.i_size);
//                } else{
//                   printf("%d\t", 0);
//                }
//
//                printf("%s", typeBuffer);
//                printf("\n");
//             }
//             free(entry);
//          }
//
//       }
//       else if((inode.i_mode & 0xF000) == 0x8000) {
//          uint32_t sizeLeft = inode.i_size, blksLeft = inode.i_size / 1024 + 1;
//          // printf("1: %u\n", blksLeft);
//          if(blksLeft != 0) {
//             for(i = 0; sizeLeft > 0 && i < blksLeft && i < EXT2_NDIR_BLOCKS;
//                i++) {
//                read_blk(&sizeLeft, inode.i_block[i]);
//             }
//
//             blksLeft -= i;
//          }
//          // printf("2: %u\n", blksLeft);
//          if(blksLeft != 0) {
//             read_data(inode.i_block[EXT2_IND_BLOCK] * 2, 0,
//                   (uint8_t *) indirect, 512);
//             read_data(inode.i_block[EXT2_IND_BLOCK] * 2 + 1, 0,
//                   (uint8_t *) indirect + 512, 512);
//
//             for(i = 0; sizeLeft > 0 && i < blksLeft &&
//                   i < 256; i++) {
//                read_blk(&sizeLeft, indirect[i]);
//             }
//             blksLeft -= i;
//          }
//          // printf("3: %u\n", blksLeft);
//          if(blksLeft != 0) {
//             read_data(inode.i_block[EXT2_DIND_BLOCK] *  2, 0,
//                (uint8_t *)doubleIndirect, 512);
//             read_data(inode.i_block[EXT2_DIND_BLOCK] *  2 + 1, 0,
//                (uint8_t *) doubleIndirect + 512, 512);
//
//             for(j = 0; sizeLeft > 0 && j < 256; j++) {
//                read_data(doubleIndirect[j] *  2, 0, (uint8_t *) indirect, 512);
//                read_data(doubleIndirect[j] *  2 + 1, 0, (uint8_t *) indirect +
//                   512, 512);
//
//                for(i = 0; sizeLeft > 0 && i < blksLeft && i < 256; i++) {
//                   read_blk(&sizeLeft, indirect[i]);
//                }
//                blksLeft -= i;
//             }
//          }
//       }
//       else{
//          printf("The file type not supported");
//          exit(0);
//       }
//    } else{
//       printf("file not found\n");
//    }
//    fclose(fp);
//
//    return(0);
// }

/******************************************************************************
 * Function: get_inode
 * -------------------
 * Desc: retrieves the specified inode
 *
 * Parameter:
 *    inode: where to store the requested inode
 *    sb: super block used for obtaining the correct inode
 *    inodeRequest: the index of the inode to be retreived
 *****************************************************************************/
void get_inode(struct ext2_inode *inode, struct ext2_super_block *sb, int
   inodeRequest) {
   /*
    * From an inode address (remember that they start at 1),
    * we can determine which group the inode is in, by using the formula:
    */
   int inodeGroup = (inodeRequest - 1) / sb->s_inodes_per_group;
   /*
    * Once we know which group an inode resides in, we can look up the actual
    * inode by first retrieving that block group's inode table's starting
    * address(see Block Group Descriptor above). The index of our inode in this
    * block group's inode table can be determined by using the formula:
    */
   int grpOffset = (inodeRequest - 1) % sb->s_inodes_per_group;
   int subBlkOffset = (grpOffset * EXT2_GOOD_OLD_INODE_SIZE) / 512;
   int inodeSubBlkOffset = grpOffset % ( 512 / EXT2_GOOD_OLD_INODE_SIZE);

   // block formula
   int blk = inodeGroup * 2 * sb->s_blocks_per_group + 2 *  5 + subBlkOffset;
   // offset
   int off = inodeSubBlkOffset * sizeof(struct ext2_inode);

   read_data(blk, off, (uint8_t *) inode, sizeof(struct ext2_inode));
   return;
}

/******************************************************************************
 * Function: find_file
 * -------------------
 * Desc: finds the correct file and determines if the file exits
 *
 * Parameter:
 *    inode: inode where file or dir is found
 *    sb: use to
 *    fileLocation: user specified file or dir location
 *    curLocation: current location
 *    pathLen: holds the path length
 *****************************************************************************/
uint8_t find_file(struct ext2_inode *inode, struct ext2_super_block
   *sb, char *fileLocation, char *curLocation, uint32_t pathLen) {
   struct ext2_dir_entry *dirEntry[256];
   struct ext2_inode inode2;
   uint8_t exists = 0;
   uint32_t dirEntryAmt = 0, i = 0;
   dirEntryAmt = entry_amount(inode, dirEntry);

   for(i = 0; i < dirEntryAmt; i++) {
      dirEntryAmt = entry_amount(inode, dirEntry);
      get_inode(&inode2, sb, dirEntry[i]->inode);
      curLocation[pathLen] = '/';
      strncpy(curLocation + pathLen + 1, dirEntry[i]->name,
         dirEntry[i]->name_len);
      curLocation[pathLen + 1 + dirEntry[i]->name_len] = '\0';
      if(strcmp(curLocation, fileLocation) == 0) {
         exists = 1;
         memcpy(inode, &inode2, sizeof(struct ext2_inode));
         return exists;
      }
      // Comparing to see if the mode is a directory
      else if((inode2.i_mode & 0xF000) ==  0x4000) {
         // Checking to see if the current location is the requested location
         if(strncmp(curLocation, fileLocation,
            pathLen+1+dirEntry[i]->name_len) == 0) {
            // Checking to see if the file is in the requested directory
            if(find_file(&inode2, sb, fileLocation,
               curLocation, pathLen+1+dirEntry[i]->name_len)) {

               // The file exists!
               exists = 1;
               memcpy(inode, &inode2, sizeof(struct ext2_inode));
               return exists;
            }
         }
      }
   }
   return exists;
}

/******************************************************************************
 * Function: read_blk
 * ------------------
 * Desc: prints contents of a block
 *
 * Parameter:
 *    N/A
 *****************************************************************************/
void read_blk(uint32_t *sizeLeft, uint32_t blk) {
   uint8_t blkbuff[1024];

   read_data(blk * 2, 0, blkbuff,  512);

   if(*sizeLeft >  512){
      *sizeLeft = *sizeLeft - fwrite(blkbuff, 1, 512 , stdout);
   } else {
      *sizeLeft = *sizeLeft - fwrite(blkbuff, 1, *sizeLeft, stdout);
   }
   if(*sizeLeft > 0) {
      read_data(blk * 2 + 1, 0, blkbuff, 512);
      if(*sizeLeft >  512){
         *sizeLeft = *sizeLeft - fwrite(blkbuff, 1, 512 , stdout);
      } else {
         *sizeLeft = *sizeLeft - fwrite(blkbuff, 1, *sizeLeft, stdout);
      }
   }
}

/******************************************************************************
 * Function: read_direct_block_dir
 * -------------------------------
 * Desc: reads data into entries
 *
 * Parameter:
 *    dirEntry:
 *    dirEntryAmt:
 *    blk:
 *****************************************************************************/
uint32_t read_direct_block_dir(struct ext2_dir_entry **dirEntry,
      uint32_t dirEntryAmt, uint32_t blk) {
   struct ext2_dir_entry *entry;
   uint8_t buffer[1024];
   uint32_t sizeRead = 0, i = 0, entryLen = 0;

   read_data(blk *  2, 0, buffer,  512);
   read_data(blk *  2 + 1, 0, buffer +  512,  512);
   for(i = 0; sizeRead < 1024; i++) {
      entry = (struct ext2_dir_entry *) (buffer + sizeRead);
      entryLen = entry->rec_len;
      dirEntry[dirEntryAmt + i] = malloc(entryLen);
      memcpy(dirEntry[dirEntryAmt + i], entry, entryLen);
      sizeRead = sizeRead + entryLen;
   }
   return i;
}

/******************************************************************************
 * Function: entry_amount
 * ----------------------
 * Desc: obtains the amout of entries in a given directory
 *
 * Parameter:
 *    N/A
 *****************************************************************************/
uint32_t entry_amount(struct ext2_inode *dirInode,
      struct ext2_dir_entry **dirEntry) {
   uint8_t blksLeft = dirInode->i_size / 1024;
   uint32_t dirEntryAmt = 0;
   uint32_t i = 0;

   if(blksLeft != 0) {
      for(i = 0; i < blksLeft && i < EXT2_NDIR_BLOCKS; i++) {
         dirEntryAmt = dirEntryAmt + read_direct_block_dir(dirEntry,
            dirEntryAmt, dirInode->i_block[i]);
      }
      blksLeft = blksLeft - i;
   }
   return dirEntryAmt;
}

/******************************************************************************
 * Function: cmpfunc
 * ------------------------
 * Desc: used for qsort ordering the directories
 *
 * Parameter:
 *    dir1 & dir2: entries to compare for ordering in qsort
 *
 *****************************************************************************/
int cmpfunc(const void *dir1, const void *dir2) {
   struct ext2_dir_entry *tmp = *(struct ext2_dir_entry **) dir1,
      *tmp2 = *(struct ext2_dir_entry **) dir2;
   uint8_t nameLen = 0;
   if(strncmp(tmp->name, ".", 1) == 0) {
      if(tmp->name_len == 1) {
         return -1;
      }
   }
   if(strncmp(tmp2->name, ".", 1) == 0) {
      if(tmp2->name_len == 1) {
         return 1;
      }
   }
   if(strncmp(tmp->name, "..", 2) == 0) {
      if(tmp->name_len == 2) {
         return -1;
      }
   }
   if(strncmp(tmp2->name, "..", 2) == 0) {
      if(tmp2->name_len == 2) {
         return 1;
      }
   }

   // to compare the name
   if(tmp->name_len < tmp2->name_len) {
       nameLen =
         tmp->name_len ;
   } else{
       nameLen =  tmp2->name_len;
   }
   int8_t check = strncmp(tmp->name, tmp2->name, nameLen);
   // if the names match
   if(check == 0) {
      if(tmp->name_len < tmp2->name_len) {
         return -1;
      }
      else if(tmp->name_len > tmp2->name_len) {
         return 1;
      }
      else{
         return 0;
      }
   }
   else{
      return check;
   }
}


/******************************************************************************
 * Function: print_bgdt
 * --------------------
 * Desc: prints the contents of a block group descriptor table
 *
 * Parameter:
 *    bgdt: block group descriptor table to print
 *
 *****************************************************************************/
void print_bgdt(struct ext2_group_desc *bgdt){
	printf("Block bitmap: %u\n", bgdt->bg_block_bitmap);
	printf("Inode bitmap: %u\n",bgdt->bg_inode_bitmap);
	printf("Inode table: %u\n",bgdt->bg_inode_table);
	printf("Free blocks count  : %u\n",bgdt->bg_free_blocks_count);
	printf("Free inodes count  : %u\n",bgdt->bg_free_inodes_count);
	printf("Used directories count  : %u\n\n", bgdt->bg_used_dirs_count);
}

/******************************************************************************
 * Function: print_super_block
 * ---------------------------
 * Desc: prints the contents of a superblock
 *
 * Parameter:
 *    sb: superblock to print
 *****************************************************************************/
void print_super_block(struct ext2_super_block *sb) {
	printf("Inodes count: %u\n", sb->s_inodes_count);
	printf("Blocks count: %u\n", sb->s_blocks_count);
	printf("Reserved blocks count: %u\n", sb->s_r_blocks_count);
	printf("Free blocks count: %u\n", sb->s_free_blocks_count);
	printf("Free inodes count: %u\n", sb->s_free_inodes_count);
	printf("First data block: %u\n", sb->s_first_data_block);
	printf("Blocks per group: %u\n",  sb->s_blocks_per_group);
	printf("Inodes per group: %u\n", sb->s_inodes_per_group);
}

/******************************************************************************
 * Function: find_bgdt
 * -------------------
 * Desc: finds the block group descriptor table
 *
 * Parameter:
 *    bgdt: block group descriptor struct to store
 *
 *****************************************************************************/
void find_bgdt(struct ext2_group_desc *bgdt){
	read_data(2 * 2, 0, (uint8_t *) bgdt, sizeof(struct ext2_group_desc));

	return;
}

/******************************************************************************
 * Function: read_data
 * -------------------
 * Desc: Reads the sectors from the file system.
 *       The block argument is in terms of SD card 512 byte sectors
 *
 * Parameter:
 *    block: block to read from
 *    offset: offset to find exact location
 *    data: the data to be stored
 *    size: size of the data to read
 *****************************************************************************/
void read_data(uint32_t block, uint16_t offset, uint8_t* data, uint16_t size) {
   if(offset > 511) {
      printf ("Offset greater than 511.\n");
      fclose(fp);
      exit(0);
   }

   fseek(fp,block*512 + offset,SEEK_SET);
   fread(data,size,1,fp);
}
