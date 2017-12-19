/*
 * Common routines
 */ 
 
void myprint(char *s)
{
  while(*s != '\0') {
    putchar((int) (*s));
	s++;
  }
}

void myprint_hexdigit(char c)
{
//  if(c > 0x0f) c = c & 0x0f;
  if(c < 10) {
    putchar('0' + c);
  } else {
    putchar('A' + c - 10);
  }
}

void myprint_hexchar(char num)
{
char c;

  c = num & 0xf0;
  c = c >> 4;
  myprint_hexdigit(c);
  c = num & 0x0f;
  myprint_hexdigit(c);
}

void myprint_hexint(int num)
{
int n;
char c;

  n = num & 0xff00;
  c = n >> 8;
  myprint_hexchar(c);
  c = num & 0x00ff;
  myprint_hexchar(c);
}

void myprint_hexlong(long num)
{
long l;
int n;

  l = num & 0xffff0000;
  n = l >> 16;
  myprint_hexint(n);
  n = num & 0x0000ffff;
  myprint_hexint(n);
}

void myprint_int(int a)
{
int d;
char buf[8], i, j, r;

  if(a == 0) {
    putchar('0');
	return;
  }
  
  i = 1;	 
  while(a > 0) {
    d = a / 10;
	r = a % 10;
	buf[i++] = '0' + r;
	a = d;
  }
  i--;
  while(i>0) {
    putchar(buf[i]);
	i--;
  }
}

void myprint_long(long a)
{
long d;
char buf[16], i, j, r;

  if(a == 0) {
    putchar('0');
	return;
  }
  
  i = 1;	 
  while(a > 0) {
    d = a / 10;
	r = a % 10;
	buf[i++] = '0' + r;
	a = d;
  }
  i--;
  while(i>0) {
    putchar(buf[i]);
	i--;
  }
}

/*
 * print a decimal number represented by a / 10^scale
 */
void myprint_int_scaled(int a, char scale)
{
int d;
char buf[8], i, j, r;

  if(a == 0) {
    putchar('0');
	putchar('.');
	putchar('0');
	return;
  }
  
  i = 1;	 
  while(a > 0) {
    if(i == scale+1) {
	  buf[i++] = '.';
	}
    d = a / 10;
	r = a % 10;
	buf[i++] = '0' + r;
	a = d;
  }
  if(i == scale+1) {
    buf[i++] = '.';
  }
  i--;
  while(i>0) {
    putchar(buf[i]);
	i--;
  }
}

/*
 * print a decimal number represented by a / 10^scale
 */
void myprint_long_scaled(long a, char scale)
{
long d;
char buf[16], i, j, r;

  if(a == 0) {
    putchar('0');
	putchar('.');
	putchar('0');
	return;
  }
  
  i = 1;	 
  while(a > 0) {
    if(i == scale+1) {
	  buf[i++] = '.';
	}
    d = a / 10;
	r = a % 10;
	buf[i++] = '0' + r;
	a = d;
  }
  if(i == scale+1) {
   buf[i++] = '.';
  }
  i--;
  while(i>0) {
    putchar(buf[i]);
	i--;
  }
}

/*
void print_decimal(double f)
{
char buffer[10], i, j, r;
double d, x;

  i = 0;
  while(f > 0) {
    d = f * 10;
	r = (int) d;
	buffer[i++] = '0' + r;
	x = d - r;
	f = x;
  }
  
  for(j=0; j<i; j++) {
    putchar(buffer[j]);
  }
}

void print_real(double d)
{
double r,f;

  r = (int) d;
  f = d - r;
  print_int(r);
  putchar('.');
  print_decimal(f);
}
*/
