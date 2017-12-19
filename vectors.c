/* As is, all interrupts except reset jumps to 0xffff, which is most
 * likely not going to useful. To replace an entry, declare your function,
 * and then change the corresponding entry in the table. For example,
 * if you have a SCI handler (which must be defined with 
 * #pragma interrupt_handler ...) then in this file:
 * add
 *	extern void SCIHandler();
 * before th table.
 * In the SCI entry, change:
 *	DUMMY_ENTRY,
 * to
 *  SCIHandler, 
 */

extern void _start(void);	/* entry point in crt??.s */
#define DUMMY_ENTRY	(void (*)(void))0xFFFF

#pragma abs_address:0xffd6

/* change the above address if your vector starts elsewhere
 */


void (*interrupt_vectors[])(void) = 
	{
	/* to cast a constant, say 0xb600, use
	   (void (*)())0xb600
	 */
	DUMMY_ENTRY,	/* SCI */
	DUMMY_ENTRY,	/* SPI */
	DUMMY_ENTRY,	/* PAIE */
	DUMMY_ENTRY,	/* PAO */
	DUMMY_ENTRY,	/* TOF */
	DUMMY_ENTRY,	/* TOC5 */	
	DUMMY_ENTRY,	/* TOC4 */
	DUMMY_ENTRY,	/* TOC3 */
	DUMMY_ENTRY,	/* TOC2 */
	DUMMY_ENTRY,	/* TOC1 */
	recv_handler,	/* TIC3 */
	DUMMY_ENTRY,	/* TIC2 */
	DUMMY_ENTRY,	/* TIC1 */
	DUMMY_ENTRY,	/* RTI */
	DUMMY_ENTRY,	/* IRQ */
	DUMMY_ENTRY,	/* XIRQ */
	DUMMY_ENTRY,	/* SWI */
	DUMMY_ENTRY,	/* ILLOP */
	DUMMY_ENTRY,	/* COP */
	DUMMY_ENTRY,	/* CLM */
	_start	/* RESET */
	};
#pragma end_abs_address
