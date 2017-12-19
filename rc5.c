#include <p18cxxx.h>
#include <usart.h>
#include <stdio.h>
#include <timers.h>

#include "rc5.h"

#pragma config OSC = HSPLL
#pragma config WDT = OFF
#pragma config LVP = ON
#pragma config PBADEN = OFF


#define LED_On()		TRISAbits.TRISA0 = 0; LATAbits.LATA0 = 1
#define LED_Off()		TRISAbits.TRISA0 = 0; LATAbits.LATA0 = 0


/** I N T E R R U P T S  *******************************************/

#pragma interruptlow InterruptHandler
void InterruptHandler (void)
{
    if( INTCONbits.RBIF )    // PORTB Change Interrupt
    {
		LED_On();
		RC5_InterruptHandler();
		LED_Off();
		INTCONbits.RBIF = 0;  // Wenn ja Interruptflag löschen
    }
}

#pragma code _HIGH_INTERRUPT_VECTOR = 0x000008
void _high_ISR (void)
{
    _asm
        goto InterruptHandler       // Sprung zur Interruptroutine
    _endasm
}

#pragma code _LOW_INTERRUPT_VECTOR = 0x000018
void _low_ISR (void)
{
    _asm
        goto InterruptHandler       // Sprung zur Interruptroutine
    _endasm
}


#pragma code



void main(void)
{
	uint16 rc5code;
	uint8 dummy;

	TRISC = 0xBF;
 	TRISBbits.RB4=1;
    
	// configure timer 0
	OpenTimer0(TIMER_INT_OFF & T0_8BIT & T0_SOURCE_INT & T0_PS_1_256);

    /* baud rate formula for BRGH = 0: */
    /* BaudRate = FOSC/(16 (X + 1)) */
    /* => BaudRate (64 (X + 1)) = FOSC */
    /* => 16 (X + 1) = FOSC / BaudRate */
    /* => X = (FOSC / BaudRate) / 16 - 1 */
    /* => X = SPBRG = 40000000 / 19200) / 16 - 1 = 25.04166 => 25 */
    OpenUSART(USART_TX_INT_OFF &    /* disables TX interrupt */
              USART_RX_INT_OFF &    /* disables RX interrupt */
              USART_ASYNCH_MODE &   /* sets it in asynchronous mode */
              USART_EIGHT_BIT &     /* sets to use 8-bit data mode */
              USART_CONT_RX &       /* sets the port in continues receive mode */
              USART_BRGH_LOW,      /* uses the high speed Baud rate formula */
              31);

	INTCONbits.RBIE = 1;     // Enable Port-b change interrupt
	dummy = RC5_GetPin();		// clear mismatch
	INTCONbits.RBIF = 0;  // Wenn ja Interruptflag löschen
	INTCONbits.GIE = 1;

	LED_On();


	printf("*** C18 RC5 Demo! ***\n\r");
	LED_Off();

	while(1)
	{
        /* check if new data is available */
        if (RC5_CodeReady())
        {
            /* get code */
            rc5code = RC5_GetCode();
            
            printf("RC5 Code=0x%04X DevAddr=0x%02X Toggle=%d Cmd=0x%02X\n\r",
                   rc5code,
                   RC5_GetDeviceAddr(rc5code),
                   RC5_GetToggleBit(rc5code),
                   RC5_GetCmd(rc5code));
        }
        
        /* this function increments the RC5 timeout timer */
        /* NOTE: Decoding will also work without calling this function, but */
        /*       it could happen that RC5 codes are sometimes not getting */
        /*       recognized because of decoding state machine stucks due */
        /*       to erroneous RC5 signal. */
        RC5_TimeoutIncrement();

	}
}