#include "../utils/common.h"
#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "display.h"
#include "../cpu/timer.h"\

#define ENTER 0x1C
#define TAB 0x0F
#define BACKSPACE 0x0E

u8int ctr = 0;

char tab[] = {
    ' ', ' ', ' ', ' '
};

char keyboard_buffer[256];

u8int scancodes[] = {
  '?', '?', '1', '2', '3', '4', '5',
  '6', '7', '8', '9', '0', '-', '=',
  '?', '?', 'Q', 'W', 'E', 'R', 'T',
  'Y', 'U', 'I', 'O', 'P', '[', ']',
  '?', '?', 'A', 'S', 'D', 'F', 'G',
  'H', 'J', 'K', 'L', ';', '\'', '`',
  '?', '\\', 'Z', 'X', 'C', 'V', 'B',
  'N', 'M', ',', '.', '/', '?', '?',
  '?', ' '
};


void exectute_command(char *string)
{
    if(strcmp(string, "EXIT") == 0)
    {
        printk("Exiting. Fuck off");
        asm volatile("hlt");
    } else {
        printk(string);
        printk("strcmp = ");
        print_dec(strcmp(string, "EXIT"));
        printk('\n');
    }
}

void append(char *string, char character, int *ptr)
{

    string[*ptr] = character;
    *ptr+=1;
    string[*ptr] = '\0';
}

char get_char_from_video_memory(int offset)
{
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;
    return vidmem[offset];
}

void print_backspace()
{
    int newCursor = get_cursor() -2;
    char character = get_char_from_video_memory(newCursor);
    if(character != '>'){
        print_char(' ', newCursor);
        set_cursor(newCursor);
    }
}

void keyboard_callback()
{
    u8int scancode = port_byte_in(0x60);
    int offset = get_cursor();
    if(scancode > 100) return;

    if(scancode == ENTER){
        exectute_command(keyboard_buffer);
        int offset = get_cursor();
        int new_offset = move_offset_to_new_line(offset);
        set_cursor(new_offset);
        printk(">");
    } else if(scancode == TAB) {
        printk(tab);

    } else if(scancode == BACKSPACE) {
        print_backspace();
        ctr-= 1;
        keyboard_buffer[ctr] = '\0';        
    } else {
        append(keyboard_buffer, scancodes[(int)scancode], &ctr);
        printk(keyboard_buffer);
    }
    //print_dec(scancode);
}

void init_keyboard()
{
    init_timer(50);
    register_interrupt_handlers(IRQ1, keyboard_callback);
}