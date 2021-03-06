/* uart.h - definintions for the NS16550 uart serial hardware */
#ifndef _UART_H
#define _UART_H
#include <stddef.h>
#define UART_BAUD	115200	/* Default console baud rate.		*/
#define	UART_OUT_IDLE	0x0016	/* determine if transmit idle		*/
#define	UART_FIFO_SIZE	64	/* chars in UART onboard output FIFO	*/
				/* (16 for later UART chips)		*/
#define NUART 1
/*
 * Control and Status Register (CSR) definintions for the 16550 UART.
 * The code maps the structure structure directly onto the base address
 * CSR address for the device.
 */
struct	uart_csreg
{
	volatile uint32	buffer;	/* receive buffer (when read)		*/
				/*   OR transmit hold (when written)	*/
	volatile uint32	ier;	/* interrupt enable			*/
	volatile uint32	iir;	/* interrupt identification (when read)	*/
				/*   OR FIFO control (when written)	*/
	volatile uint32	lcr;	/* line control register		*/
	volatile uint32	mcr;	/* modem control register		*/
	volatile uint32	lsr;	/* line status register			*/
	volatile uint32	msr;	/* modem status register		*/
	volatile uint32	spr;	/* scratch register			*/
	volatile uint32 mdr1;
	volatile uint32 res[12];/* unused UART registers		*/
	/*volatile uint32 mdr2;
	volatile uint32 res1[4];
	volatile uint32 uasr;
	volatile uint32 res2;
	volatile uint32 scr;
	volatile uint32 ssr;
	volatile uint32 res3[2];
	volatile uint32 mvr;*/
	volatile uint32 sysc;	/* system configuration register	*/
	volatile uint32 syss;	/* system status register		*/
	volatile uint32 wer;
	volatile uint32 res4;
	volatile uint32 rxfifo_lvl;
	volatile uint32 txfifo_lvl;
	volatile uint32 ier2;
	volatile uint32 isr2;
	volatile uint32 freq_sel;
	volatile uint32 res5[2];
	volatile uint32 mdr3;
	volatile uint32 tx_dma_thresh;
};

/* Alternative names for control and status registers */

#define	rbr	buffer		/* receive buffer (when read)		*/
#define	thr	buffer		/* transmit hold (when written)		*/
#define	fcr	iir		/* FIFO control (when written)		*/
#define	dll	buffer		/* divisor latch (low byte)		*/
#define	dlm	ier		/* divisor latch (high byte)		*/

/* Definintion of individual bits in control and status registers	*/

/* Divisor values for baud rate */

#define	UART_DLL	26	/* value for low byte of divisor latch	*/
				/*	DLAB=0				*/
#define UART_DLM	0	/* value for high byte of divisor latch	*/
				/*	DLAB=1				*/

/* Line control bits */

#define	UART_LCR_DLAB	0x80	/* Divisor latch access bit		*/
#define	UART_LCR_8N1	0x03	/* 8 bits, no parity, 1 stop		*/

/* Interrupt enable bits */

#define UART_IER_ERBFI	0x01	/* Received data interrupt mask		*/
#define UART_IER_ETBEI	0x02	/* Transmitter buffer empty interrupt	*/
#define UART_IER_ELSI	0x04	/* Recv line status interrupt mask	*/
#define UART_IER_EMSI	0x08	/* Modem status interrupt mask		*/

/* Interrupt identification masks */

#define UART_IIR_IRQ	0x01	/* Interrupt pending bit		*/
#define UART_IIR_IDMASK 0x0E	/* 3-bit field for interrupt ID		*/
#define UART_IIR_MSC	0x00	/* Modem status change			*/
#define UART_IIR_THRE	0x02	/* Transmitter holding register empty	*/
#define UART_IIR_RDA	0x04	/* Receiver data available		*/
#define UART_IIR_RLSI	0x06	/* Receiver line status interrupt	*/
#define UART_IIR_RTO	0x0C	/* Receiver timed out			*/

/* FIFO control bits */

#define UART_FCR_EFIFO	0x01	/* Enable in and out hardware FIFOs	*/
#define UART_FCR_RRESET 0x02	/* Reset receiver FIFO			*/
#define UART_FCR_TRESET 0x04	/* Reset transmit FIFO			*/
#define UART_FCR_TRIG0	0x00	/* RCVR FIFO trigger level one char	*/
#define UART_FCR_TRIG1	0x40	/* RCVR FIFO trigger level 1/4		*/
#define UART_FCR_TRIG2	0x80	/* RCVR FIFO trigger level 2/4		*/
#define UART_FCR_TRIG3	0xC0	/* RCVR FIFO trigger level 3/4		*/

/* Modem control bits */

#define UART_MCR_OUT2	0x08	/* User-defined OUT2			*/
#define UART_MCR_RTS    0x02    /* RTS complement 			*/
#define UART_MCR_DTR    0x01    /* DTR complement 			*/
#define UART_MCR_LOOP	0x10	/* Enable loopback test mode		*/

/* Line status bits */

#define	UART_LSR	5	/* Input" Line Status Register		*/

#define UART_LSR_DR	0x01	/* Data ready				*/
#define	UART_LSR_BI	0x10	/* Break interrupt indicator		*/
#define UART_LSR_THRE	0x20	/* Transmit-hold-register empty		*/
#define UART_LSR_TEMT	0x40	/* Transmitter empty			*/

#define	UART_TX		0	/* offset of transmit buffer		*/

/* MDR1 bits	*/
#define UART_MDR1_16X	0x00000000
#define UART_MDR1_16XAB	0x00000002
#define UART_MDR1_13X	0x00000003

/* SYSC register bits */

#define UART_SYSC_SOFTRESET	0x00000002

/* SYSS register bits */

#define UART_SYSS_RESETDONE	0x00000001

/* UART1 Clock control */
#define UART1_CLKCTRL_ADDR	0x44e0006c
#define UART1_CLKCTRL_EN	0x00000002

/* Pad control addresses and modes */
#define UART0_PADRX_ADDR	0x44E10970
#define UART0_PADTX_ADDR	0x44E10974
#define UART0_PADRX_MODE	0
#define UART0_PADTX_MODE	0
#define UART1_PADRX_ADDR	0x44E10980
#define UART1_PADTX_ADDR	0x44E10984
#define UART1_PADRX_MODE	0
#define UART1_PADTX_MODE	0

/* UART Buffer lengths */
#define UART_IBLEN      1024
#define UART_OBLEN      1024

#define UART_BAUD       115200  /**< Default console baud rate.         */

/**
 * UART control block
 */
struct uart
{
    /* Pointers to associated structures */
    void *csr;                  /**< Control & status registers         */
    struct dentry *dev;                /**< Dev structure                      */

    /* Statistical Counts */
    uint cout;                  /**< Characters output                  */
    uint cin;                   /**< Characters input                   */
    uint lserr;                 /**< Receiver error count               */
    uint ovrrn;                 /**< Characters overrun                 */
    uint iirq;                  /**< Input IRQ count                    */
    uint oirq;                  /**< Output IRQ count                   */

    /* UART input fields */
    uchar iflags;               /**< Input flags                        */
    sid32 isema;            /**< Count of input bytes ready         */
    ushort istart;              /**< Index of first byte                */
    ushort icount;              /**< Bytes in buffer                    */
    uchar in[UART_IBLEN];       /**< Input buffer                       */

    /* UART output fields */
    uchar oflags;               /**< Output flags                       */
    sid32 osema;            /**< Count of buffer space free         */
    ushort ostart;              /**< Index of first byte                */
    ushort ocount;              /**< Bytes in buffer                    */
    uchar out[UART_OBLEN];      /**< Output buffer                      */
    volatile bool oidle;        /**< UART transmitter idle              */
};

extern struct uart uarttab[];

/* UART input flags */
#define UART_IFLAG_NOBLOCK    0x0001 /**< do non-blocking input         */
#define UART_IFLAG_ECHO       0x0002 /**< echo input                    */

/* UART output flags */
#define UART_OFLAG_NOBLOCK    0x0001 /**< do non-blocking output        */

/* uartControl() functions  */
#define UART_CTRL_SET_IFLAG   0x0010 /**< set input flags               */
#define UART_CTRL_CLR_IFLAG   0x0011 /**< clear input flags             */
#define UART_CTRL_GET_IFLAG   0x0012 /**< get input flags               */
#define UART_CTRL_SET_OFLAG   0x0013 /**< set output flags              */
#define UART_CTRL_CLR_OFLAG   0x0014 /**< clear output flags            */
#define UART_CTRL_GET_OFLAG   0x0015 /**< get output flags              */
#define UART_CTRL_OUTPUT_IDLE 0x0016 /**< determine if transmit idle    */
#define TTY_ICRNL 0x08
/* Driver functions */
devcall uartInit(struct dentry *);
devcall uartRead(struct dentry *, void *, uint);
devcall uartWrite(struct dentry *, const void *, uint);
devcall uartGetc(struct dentry *);
devcall uartPutc(struct dentry *, char);
devcall uartControl(struct dentry *, int, long, long);
interrupt uartInterrupt(void);
void uartStat(ushort);

/**
 * @ingroup uarthardware
 *
 * Initialize the UART hardware.
 */
devcall uartHwInit(struct dentry *);

/**
 * @ingroup uarthardware
 *
 * Immediately put a character to the UART.
 */
void uartHwPutc(void *, uchar);

/**
 * @ingroup uarthardware
 *
 * Print hardware-specific statistics about the UART.
 */
void uartHwStat(void *);
#endif