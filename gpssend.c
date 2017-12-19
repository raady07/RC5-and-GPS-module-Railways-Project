#include <stdio.h>
#include <hc11.h>

/*
 * Read GPS Message 1000
 * 10 - Navigation Solution Validity (word:lower byte)
 * 27-28 - Lattitude (2 words)
 * 29-30 - Longitude (2 words)
 * 34-35 - Speed (2 words)
 * 36 - True Course (1 word)  
 */
void gps_read(char *b1)
{
char i, ch1, ch2;

  while(1) {
    ch1 = (char) getchar();
	if(ch1 == 0xFF) {
      ch2 = (char) getchar();
	  if(ch2 == 0x81) {
	    break;
	  }
	}
  }
  
  PORTA |= 0x40;
  
  for(i=0; i<16; i++) getchar();
  *b1++ = (char) getchar();
  for(i=0; i<33; i++) getchar();
  for(i=0; i<8; i++) {
    *b1++ = (char) getchar();
  }
  for(i=0; i<6; i++) getchar();
  for(i=0; i<6; i++) {
    *b1++ = (char) getchar();
  }
}

void send_gps(char *ptr)
{
char i;

  for(i=0; i<15; i++) {
    PORTB = ptr[i];
	PORTA |= 0x20;
	while((PORTA & 0x01) == 0);
    PORTA &= ~0x20;
  }	 
}

void main(void)
{
char b1[15];

  setbaud(BAUD9600);
    
  while(1) {
    gps_read(b1);
	PORTA &= ~0x40;
    send_gps(b1);	
  }  
}
