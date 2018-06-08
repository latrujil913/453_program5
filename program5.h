#ifndef PROG_H
#define PROG_H

void start_audio_pwm(void); // Implement
void start_system_timer();
void sd_card_reader(void);
void audio_playback(void);
void get_inode_p5(int inodeRequest);
void blank_audio();
// void print_super_block(struct ext2_super_block *sb);
#endif
