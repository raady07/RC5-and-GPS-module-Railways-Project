#include <stdio.h>
#include <hc11.h>

#include "print.c"

#pragma interrupt_handler recv_handler

typedef union {
  int i;
  char c[2];
} real2;

typedef union {
  long l;
  char c[4];
} real4;

static char buffer[15];
static char flag = 0;
char gvalid;
real4 glat, glong, gspeed;
real2 gangle;


void recv_handler(void)
{
char i;
 
  /* Interrupt occurred */
  PORTB = 0xFF;

  /* disable interrupt on IC3 */
  TMSK1 &= ~0x01;
  INTR_OFF();

  for(i=0; i<15; i++) {  

    PORTB = 0xFF;
	 
    /* Acknowledge data receive */
    PORTA |= 0x10;

    /* Read data */
    buffer[i] = PORTC;

    /* Clear Acknowledge */
    PORTA &= ~0x10;

    /* Clear flag to enable next interrupt */
    TFLG1 |= 0x01;
	
	PORTB = 0;
	
	if(i < 14) {
  	  while(!(TFLG1 & 0x01));
	}
  }
  
  PORTB = 0xFF;
  
  if(buffer[0] > 0) gvalid = 'I';
  else gvalid = 'V';
  
  glat.c[3] = buffer[1];
  glat.c[2] = buffer[2];
  glat.c[1] = buffer[3];
  glat.c[0] = buffer[4];

  glong.c[3] = buffer[5];
  glong.c[2] = buffer[6];
  glong.c[1] = buffer[7];
  glong.c[0] = buffer[8];

  gspeed.c[3] = buffer[9];
  gspeed.c[2] = buffer[10];
  gspeed.c[1] = buffer[11];
  gspeed.c[0] = buffer[12];
	
  gangle.c[1] = buffer[13];
  gangle.c[0] = buffer[14];

  flag = 1;
  
  PORTB = 0;
} 


void print_latlong(char gvalid, real4 *glat, 
	 real4 *glong, real4 *gspeed, real2 *gangle) 
{
  putchar(gvalid);
  putchar(' ');
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
  setbaud(BAUD9600);

  myprint("\nReceive v1.2\n\n");
  
  PORTB = 0;
  
  /* Set PORTC to read */
  DDRC =0;

  /* Interrupt on rising edge of IC3 */
  TCTL2 |= 0x01;
  TCTL2 &= ~0x02;

  while(1) {
    
	/* Enable interrupt on IC3 */
    TMSK1 |= 0x01;
    INTR_ON();
  
    /* wait till gps data is read */
    while(flag != 1);
	flag = 0;
	
    print_latlong(gvalid, &glat, &glong, &gspeed, &gangle);
  }
}

#include "vectors.c"
