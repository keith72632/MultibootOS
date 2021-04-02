#include "../drivers/display.h"

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
