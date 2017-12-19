#include <stdio.h>
#include <hc11.h>

#include "print.c"

typedef union {
  int i;
  char c[2];
} real2;

typedef union {
  long l;
  char c[4];
} real4;


void myprint_real4(real4 *r, char scale)
{
  myprint_long_scaled(r->l, scale); putchar(' ');
}

void myprint_real2(real2 *r, char scale)
{
  myprint_int_scaled(r->i, scale); putchar(' ');
}

 
/*
 * Look for the Message 1000 
 *  (Geodetic Position Status Output Message)
 */
void gps_sync(void)
{
char ch1, ch2, i;
  
  while(1) {
    ch1 = (char) getchar();
	if(ch1 == 0xff) {
	  ch2 = (char) getchar();
      if(ch2 == 0x81) {
	    break;
	  }
	}
  }
  for(i=0; i<108; i++) {
    getchar();
  }
}

/*
 * Read GPS Message 1000
 */
void gps_read(char *b1)          fscanf
{
char i, ch;

  *b1++ = (char) getchar();
  PORTA |= 0x40;
  for(i=0; i<109; i++) {
    *b1++ = (char) getchar();
  }
}

/*
 * Print the lat/long/speed/heading data
 */
void print_latlong(real4 *glat, real4 *glong, 
	 real4 *gspeed, real2 *gangle, char gvalid) 
{
  if((gvalid & 0x1F) > 0) {
    putchar('I');
  } else {
    putchar('V');
  }

  myprint_hexlong(glat->l);
  myprint_hexlong(glong->l);
  myprint_hexlong(gspeed->l);
  myprint_hexint(gangle->i);
  
/*
  myprint_real4(glat, 8);
  myprint_real4(glong, 8);
  myprint_real4(gspeed, 2);
  myprint_real2(gangle, 3);
*/ 
  putchar('\n');
}

void send_long(real4 *num)
{
char i,temp;
unsigned int j;

  for(i=0; i<4;i++) {
    PORTB = num->c[i];
    PORTA |= 0x20;
	while (1)
	{
	temp= PORTA;
	if ( (temp & 0x01) == 0x01)
	 {
	  PORTA &= ~0x20;
	  break;
	 }
	}  
	
  }
}  
     
void send_int(real2 *num)
{
char i,temp;
unsigned int j;

  for(i=0; i<2;i++) {
   PORTB = num->c[i];
   PORTA |= 0x20;
	while (1)
	{
	temp= PORTA;
	if ( (temp & 0x01) == 0x01)
	 {
	  PORTA &= ~0x20;
	  break;
	 }
	}  
  }
}  
  

void main(void)
{
real4 glat, glong, gspeed;
real2 gangle;
char b1[110], gvalid;

  setbaud(BAUD9600);
//  myprint("\nComplete LATLONG GPS data (binary) v1.0\n\n");
    
  gps_sync();
  while(1) {
    gps_read(b1);
	PORTA &= ~0x40;
	
	gvalid = b1[18];
	
	glat.c[3] = b1[52];
	glat.c[2] = b1[53];
	glat.c[1] = b1[54];
	glat.c[0] = b1[55];

	glong.c[3] = b1[56];
	glong.c[2] = b1[57];
	glong.c[1] = b1[58];
	glong.c[0] = b1[59];

	gspeed.c[3] = b1[66];
	gspeed.c[2] = b1[67];
	gspeed.c[1] = b1[68];
	gspeed.c[0] = b1[69];
	
	gangle.c[1] = b1[70];
	gangle.c[0] = b1[71];

    print_latlong(&glat, &glong, &gspeed, &gangle, gvalid);

    PORTA |= 0x40;
    send_long(&glat);
	send_long(&glong);
	send_long(&gspeed);
	send_int(&gangle);
	PORTA &= ~0x40;	
  }  
}

