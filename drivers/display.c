#include <stdint.h>
#include "../cpu/ports.h"
#include "display.h"

/*
get_cursor and set_cursor manipulate the display controllers register via I/O ports
cursor postion 16 bit. data register(0x3d5) will hold low byte if control register(0x3d4) is set to 15(0x0f),
and the high byte if the value is set to 14 (0x0e). cursor offset represent video offset/2
*/
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	port_byte_out(0x3D4, 0x0A);
	port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xC0) | cursor_start);
 
	port_byte_out(0x3D4, 0x0B);
	port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xE0) | cursor_end);
}



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
void terminal_set_cursor (int x, int y)
{
    uint16_t pos = y * 80 + x + 1;
    port_byte_out(0x3D4, 0x0F);
    port_byte_out(0x3D5, (uint8_t) (pos & 0xFF));
    port_byte_out(0x3D4, 0x0E);
    port_byte_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
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

void printk(char * string)
{
    enable_cursor(0, 15);
    int offset = get_cursor();
    int i = 0;
    while(string[i] != 0){
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