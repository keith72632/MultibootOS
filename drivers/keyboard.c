#include "../utils/common.h"
#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "display.h"
#include "../cpu/timer.h"\

#define ENTER 0x1C
#define TAB 0x0F
#define BACKSPACE 0x0E
#define SC_MAX 57

u8int ctr = 0;

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

void print_nl() 
{
    int offset = move_offset_to_new_line(get_cursor());
    if(offset >= MAX_ROWS * MAX_COLS * 2){
        offset = scroll_ln(offset);
    }
    set_cursor(offset);
}

void clear_screen() {
    for(int i = 0; i < MAX_COLS * MAX_ROWS; i++){
        print_char(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void execute_command(char *input)
{
    if(compare_string(input, "EXIT") == 0){
        clear_screen();
        printk("Stopping the CPU. Fuck Off\n");
        asm volatile("hlt");
    } else if(compare_string(input, "WHOAMI")==0){
        printk("User: Keith\n");
        printk(">");
    }else{
        printk("Unkown Command: ");
        printk(input);
        printk("\n>");
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

static void keyboard_callback(registers_t *regs) {
    u8int scancode = port_byte_in(0x60);

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