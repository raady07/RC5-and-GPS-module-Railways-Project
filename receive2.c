#include <stdio.h>
#include <hc11.h>

#include "print.c"

#pragma interrupt_handler recv_handler

static char counter=0;
static char buffer[14];

typedef union {
  int i;
  char c[2];
} real2;

typedef union {
  long l;
  char c[4];
} real4;


void recv_handler(void)
{ 
  /* Acknowledge data receive */
  PORTA |= 0x10;

  /* Read data */
  buffer[counter] = PORTC;
  counter++;

  /* Clear Acknowledge */
  PORTA &= ~0x10;
  
  /* Clear flag to enable next interrupt */
  TFLG1 |= 0x01;
  
  /* Are we getting an interrupt ?? */
  PORTA |= 0x40;
} 

void print_latlong(real4 *glat, real4 *glong, 
	 real4 *gspeed, real2 *gangle) 
{
  
  myprint_hexlong(glat->l);
  putchar(' ');
  myprint_hexlong(glong->l);
  putchar(' ');
  myprint_hexlong(gspeed->l);
  putchar(' ');
  myprint_hexint(gangle->i);
  putchar(' ');
  
  putchar('\n');
}
 
/*
 * Inturrupt driven read of GPS processed data
 */
void main (void)
{
char temp;
real4 glat, glong, gspeed;
real2 gangle;

  setbaud(BAUD9600);
 
  /* Enable interrupt on IC3 */
  TMSK1 |= 0x01;
  
  /* Interrupt on rising edge of IC3 */
  TCTL2 |= 0x01;
  TCTL2 &= ~0x02;
  
  /* Set PORTC to read */
  DDRC =0;
 
  INTR_ON();
  
  while(1) {
  
    /* wait till the buffer is full */
    while(counter < 14);

    /* Process buffer */
    counter=0;
    
	glat.c[3] = buffer[3];
	glat.c[2] = buffer[2];
	glat.c[1] = buffer[1];
	glat.c[0] = buffer[0];

	glong.c[3] = buffer[7];
	glong.c[2] = buffer[6];
	glong.c[1] = buffer[5];
	glong.c[0] = buffer[4];

	gspeed.c[3] = buffer[11];
	gspeed.c[2] = buffer[10];
	gspeed.c[1] = buffer[9];
	gspeed.c[0] = buffer[8];
	
	gangle.c[1] = buffer[13];
	gangle.c[0] = buffer[12];

    print_latlong(&glat, &glong, &gspeed, &gangle);
  }
}

#include "vectors.c"
