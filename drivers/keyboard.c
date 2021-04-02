#include "../utils/common.h"
#include "../cpu/isr.h"
#include "display.h"
#include "../cpu/timer.h"
#include "../utils/shell_funcs.h"

#define ENTER 0x1C
#define TAB 0x0F
#define BACKSPACE 0x0E
#define SC_MAX 57

int ctr = 0;

char tab[] = {
    ' ', ' ', ' ', ' '
};

static char key_buffer[256];

const char scancode_to_char[] = {
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


static void keyboard_callback(registers_t *regs) {
    u8int scancode = inb(0x60);

    if(scancode > SC_MAX) return;

    if(scancode == BACKSPACE){
        if(key_buffer[0] != '\0'){
            print_backspace();
            ctr -=1;
            key_buffer[ctr] = '\0';
        }
    }else if(scancode == ENTER){
        print_nl();
        execute_command(key_buffer);
        key_buffer[0] = '\0';
        ctr = 0;
    }else{
        char letter = scancode_to_char[(int)scancode];
        append(key_buffer, letter, &ctr);
        key_buffer[ctr] = '\0';
        char str[2] = {letter, '\0'};
        printk(str);
    }
}

void init_keyboard()
{
    init_timer(50);
    register_interrupt_handlers(IRQ1, keyboard_callback);
}