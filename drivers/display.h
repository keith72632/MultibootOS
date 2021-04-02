#include "../utils/common.h"

#define VGA_CTRL_REG 0x3d4
#define VGA_DATA_REG 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define MAX_ROWS 25
#define MAX_COLS 80

#define VIDEO_ADDRESS 0xb8000

int strln(char * string);
void printk(char * string);
void print_dec(u32int n);
int get_cursor();
void print_char(char character, int offset);
void printc(char character);
int move_offset_to_new_line(int offset);
void set_cursor(int offset);



