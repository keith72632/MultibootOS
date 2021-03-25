/*Sources:
 * - https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C
 * */

/*
    By in, what this really means data from device is coming IN to CPU. This is the euqivalent to reading from device.
    __asm__ function is in AT&T format, so reads left as source and right as destination. The info from port will be in 
    dx, which is then stored in al, which has a C variable assigned to it.*/
unsigned char port_byte_in(unsigned short port)
{
	unsigned char result;
    /*intruction : output : input : clobbered*/
	__asm__("in %%dx, %%al" : "a=" (result) : "d" (port));
	return result;
}

/*
    Out is essentially a write operand. the info we store in al is moved into dx, the sent OUT to device. This is sometimes refered to 
    as a request. You are requesteing data from device by sending data, then reciveing it with IN function
*/
void port_byte_out(unsigned short port, unsigned char data)
{
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}