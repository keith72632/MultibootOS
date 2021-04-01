#include "../cpu/ports.h"
#include "display.h"
#include "../utils/common.h"

/*
get_cursor and set_cursor manipulate the display controllers register via I/O ports
cursor postion 16 bit. data register(0x3d5) will hold low byte if control register(0x3d4) is set to 15(0x0f),
and the high byte if the value is set to 14 (0x0e). cursor offset represent video offset/2
*/

/*******************************************************************************************************************
 *                 Public Functions                                                                                *
 *******************************************************************************************************************
*/

int get_cursor();
void set_cursor(int offset);
void print_char(char character, int offset);
int strln(char * string);
int get_offset(int col, int row);
int scroll_ln(int offset);
int move_offset_to_new_line(int offset);



void printk(char * string)
{
    int offset = get_cursor();
    int i = 0;
    while(string[i] != 0){
        if(offset >= MAX_ROWS * MAX_COLS * 2){
            scroll_ln(offset);
        }
        if(string[i] == '\n'){
            offset = move_offset_to_new_line(offset);
        }else {
            print_char(string[i], offset);
            //move offset 2 bytes to next character cell
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}

void print_dec(u32int n)
{
    int offset = get_cursor();
    if(n == 0)
    {
        print_char(0, offset);
    }
    s32int acc = n;
    char c[32];
    int i = 0;
    while(acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;
    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    printk(c2);
}

int strlen(char * string)
{
    for(int i = 0; string[i] != '\0'; ++i) return i;
}


/*******************************************************************************************************************
 *                 Private Functions                                                                               *
 *******************************************************************************************************************
*/


int get_cursor()
{
    //write data to register 0x3d4 on port requesting byte 14, the high byte of cursor
    port_byte_out(VGA_CTRL_REG, 0x0E);//14 == 0b1110
    //read data from register 0x3d5 left shifter by one byte to the High Byte
    int offset = port_byte_in(VGA_DATA_REG) << 8; //0b0011 -> 0000001100000000
    //request data from register 0x3d4 requesting low byte (15)
    port_byte_out(VGA_CTRL_REG, 0x0F);//0b1111
    //add high byte and low byte togethr 
    offset += port_byte_in(VGA_DATA_REG);//(786)0b1100000000 + (112)0b0001110000 = 0b1101110000
    //VGA cells consist of the character and its control data
    return offset * 2;
}

void set_cursor(int offset)
{
    //memory offset is double cursor offset
    offset/= 2;
    //request data from register 0x3d4 on port 0x0e 14 0b1110
    port_byte_out(VGA_CTRL_REG, 14);
    //request data from register 0x3d5 port right shfited one byte             
    port_byte_out(VGA_DATA_REG, (unsigned char)(offset >> 8));
    //request data from register 0x3d4 on port 0x0f 15 0b1111
    port_byte_out(VGA_CTRL_REG, 15);
    //data from register 0c3d5 set to 0b110000
    port_byte_out(VGA_DATA_REG, (unsigned char)(offset & 0xff));
}

void printc(char character)
{
    int offset = get_cursor();

    print_char(character, offset);

    offset += 2;

    set_cursor(offset);
}

void print_char(char character, int offset)
{
    unsigned char *vidmem = (char *)0xb8000;
    vidmem[offset] = character;
    vidmem[offset + 1] = 0x03;
}

int strln(char * string)
{
    for(int i= 0;string[i] != '\0'; i++) return i;
}

int get_offset(int col, int row)
{
    return 2 * (row * MAX_COLS + col);
}

int get_row_from_offset(int offset)
{
    return offset / (2 * MAX_COLS);
}

int move_offset_to_new_line(int offset)
{
    return get_offset(0, get_row_from_offset(offset) + 1);
}


void mem_cpy(u8int * src, u8int * dest, u32int nbytes)
{
    for(int i = 0; i < nbytes; i++)
        *(dest + i) = *(src + i);
}

int scroll_ln(int offset)
{
    mem_cpy(
        (u8int *)(get_offset(0, 1) + 0xb8000),
        (u8int *)(get_offset(0, 0) + 0xb8000),
        MAX_COLS * (MAX_ROWS -1) * 2
    );
    for(int col = 0; col < MAX_COLS; col++){
        print_char(' ', get_offset(col, MAX_ROWS -1));
    }
    return offset -2 * MAX_COLS;
}