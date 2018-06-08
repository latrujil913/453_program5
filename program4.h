#ifndef __PROGRAM4_H__
#define __PROGRAM4_H__

#include "ext2.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


/*------------------------------------------------------------------------------
 *                                 Constructors
 *----------------------------------------------------------------------------*/
uint8_t find_file(struct ext2_inode *, struct ext2_super_block *, char *,
   char *, uint32_t );
void get_inode(struct ext2_inode *, struct ext2_super_block *, int );
void read_blk(uint32_t *, uint32_t );
uint32_t read_direct_block_dir(struct ext2_dir_entry **, uint32_t , uint32_t );
uint32_t entry_amount(struct ext2_inode *, struct ext2_dir_entry **);
int cmpfunc(const void *, const void *);
void print_super_block(void);
void read_data(uint32_t , uint16_t , uint8_t *, uint16_t );
void find_bgdt(struct ext2_group_desc *);
void get_super_block(struct ext2_super_block *);

#endif
