#ifndef _CONFIG_H

/* RC5 sensor connected to RB4 */
#define RC5_DATA_PIN		PORTBbits.RB4

/* timer 0 is configured to increments every 25.6us, i.e. 1000/26 times per ms */
#define RC5_TICKS_PER_MS	(1000/26)

/* timer 0 used for time measurement of RC5 signal */
#define RC5_GetTimer()		(unsigned char)ReadTimer0()

#endif /* _CONFIG_H */