/**************************************************************************************************
  Filename:       uart.c
  Revised:        $Date: 2009-08-17 10:50:58 -0700 (Mon, 17 Aug 2009) $
  Author:         $Author: jnoxon $

  Description:    This file supports the SimpliciTI-compatible UART driver.

    Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
**************************************************************************************************/

/******************************************************************************
 * INCLUDES
 */
#include "uart.h"

/******************************************************************************
 * MACROS
 */

/******************************************************************************
 * CONSTANTS AND DEFINES
 */

/******************************************************************************
 * TYPEDEFS
 */

/******************************************************************************
 * LOCAL VARIABLES
 */
static uart_get_tx_data_type uart1_tx_handler = NULL;
static uart_get_tx_data_type uart0_tx_handler = NULL;
static uart_put_rx_data_type uart_rx_handler = NULL;

/******************************************************************************
 * LOCAL FUNCTIONS
 */
void uart1_tx_irq( void );
void uart_rx_irq( void );
void uart0_tx_irq( void );

/******************************************************************************
 * IRQs for all 8051 variants using IAR
 */


/* vector all interrupts to common routines */
#pragma vector=UART_IRQ_VECTOR( UART_NUMBER_0, TX )
__idata_reentrant __interrupt void uart_0_tx_irq( void );
#pragma vector=UART_IRQ_VECTOR( UART_NUMBER_0, TX )
__idata_reentrant __interrupt void uart_0_tx_irq( void )
{
  uart0_tx_irq( );
  return;
}

#pragma vector=UART_IRQ_VECTOR( UART_NUMBER_1, TX )
__idata_reentrant __interrupt void uart_1_tx_irq( void );
#pragma vector=UART_IRQ_VECTOR( UART_NUMBER_1, TX )
__idata_reentrant __interrupt void uart_1_tx_irq( void )
{
  uart1_tx_irq( );
  return;
}

#pragma vector=UART_IRQ_VECTOR( UART_NUMBER_0, RX )
__idata_reentrant __interrupt void uart_0_rx_irq( void );
#pragma vector=UART_IRQ_VECTOR( UART_NUMBER_0, RX )
__idata_reentrant __interrupt void uart_0_rx_irq( void )
{
  uart_rx_irq( );
  return;
}

#pragma vector=UART_IRQ_VECTOR( UART_NUMBER_1, RX )
__idata_reentrant __interrupt void uart_1_rx_irq( void );
#pragma vector=UART_IRQ_VECTOR( UART_NUMBER_1, RX )
__idata_reentrant __interrupt void uart_1_rx_irq( void )
{
  uart_rx_irq( );
  return;
}



/******************************************************************************
 * Interrupt Service Routines (ISRs)
 */

/******************************************************************************
 * @fn          uart1_tx_irq
 *
 * @brief       TX interrupt service routine
 *
 * input parameters
 *
 * output parameters
 *
 * @return
 */
void uart1_tx_irq( void )
  {
  unsigned char c;
  uart_get_tx_data_type handler;

  BSP_CRITICAL_STATEMENT( handler = uart1_tx_handler );

  /* if a handler exists */
  if( handler != NULL )
    {
    if( (*handler)( &c ) != false ) /* if this is not the last byte to send */
      {
      bspIState_t intState;
      BSP_ENTER_CRITICAL_SECTION( intState );

      /* only reset the interrupt flag if we have additional data to send
       * that way, if we are done then the interrupt is still pending and
       * will be immediately entered upon re-enabling it.*/
      UART_IRQ_FLAG_CLR( UART_NUMBER_1, UART1_LOCATION, TX ); /* eset the interrupt */

      BSP_EXIT_CRITICAL_SECTION( intState );
      }
    else
      {
      bspIState_t intState;
      BSP_ENTER_CRITICAL_SECTION( intState );

      /* we're done sending data.  since we left the interrupt pending,
       * disable it so we don't re-enter the isr.  the interrupt will be
       * re-enabled when there is another message to send. */
      UART_IRQ_DISABLE( UART_NUMBER_1, UART1_LOCATION, TX );

      /* no more data to send, reset the handler to flag not busy */
      uart1_tx_handler = NULL;

      BSP_EXIT_CRITICAL_SECTION( intState );
      }

    UART_SEND( UART_NUMBER_1, UART1_LOCATION, c ); /* send the byte */
    }
  else /* if no handler exists?!?!?!? */
    /* something went wrong, disable interrupts so we don't get stuck here */
    UART_IRQ_DISABLE( UART_NUMBER_1, UART1_LOCATION, TX );

  return;
}

/******************************************************************************
 * @fn          uart_rx_irq
 *
 * @brief       RX interrupt service routine
 *
 * input parameters
 *
 * output parameters
 *
 * @return
 */
void uart_rx_irq( void )
  {
  uart_put_rx_data_type handler;

  /* read in the received data, this will clear the interrupt also */
  unsigned char c = UART_RECEIVE( UART_NUMBER_1, UART1_LOCATION );

  BSP_CRITICAL_STATEMENT( handler = uart_rx_handler );

  if( handler != NULL ) /* if a handler exists to receive data */
    if( ( *handler)( c ) == false ) /* if the user is done receiveing */
      /* indicate the receiver is available */
      BSP_CRITICAL_STATEMENT( uart_rx_handler = NULL );

  return;
  }

/******************************************************************************
 * GLOBAL FUNCTIONS
 */

/******************************************************************************
 * @fn          uart_init
 *
 * @brief       Configures UART and sets up transmit and receive interrupts
 *
 * input parameters
 *
 * output parameters
 *
 * @return
 */
void uart_init( void )
  {
  volatile unsigned int i;

  /* make sure the handler functions are cleared in case we are re-initialized */
  uart1_tx_handler = NULL;
  uart0_tx_handler = NULL;
  uart_rx_handler = NULL;

  /* initialize the uart interface for operations */
  UART_INIT( UART_NUMBER_1,
             UART_LOCATION_2,
             UART_FLOW_CONTROL,    /* enable/disable flow control */
             UART_PARITY_MODE,     /* enable/disable parity */
             UART_STOP_BITS,       /* number of stop bits */
             UART1_BAUD_RATE );     /* baud rate to use */

  i = UART1_BAUD_RATE >> 5; /* delay approximately 1 bit time */
  while( --i != 0 ) /* give the uart some time to initialize */
      ; /* null statement */

  /* set the interrupt flag so that a transmit interrupt will be pending
   * that way when a message is sent and the irq is enabled, the interrupt
   * will happen immediately to start the transmission */
  UART_IRQ_FLAG_SET( UART_NUMBER_1, UART1_LOCATION, TX ); /* set the interrupt */

  /* enable receive interrupts, they are always welcome. */
  UART_IRQ_ENABLE( UART_NUMBER_1, UART1_LOCATION, RX );

  /* initialize the uart interface for operations */
  UART_INIT( UART_NUMBER_0,
             UART_LOCATION_2,
             UART_FLOW_CONTROL,    /* enable/disable flow control */
             UART_PARITY_MODE,     /* enable/disable parity */
             UART_STOP_BITS,       /* number of stop bits */
             UART0_BAUD_RATE );     /* baud rate to use */

  UART_IRQ_FLAG_SET( UART_NUMBER_0, UART0_LOCATION, TX ); /* set the interrupt */

  UART_IRQ_DISABLE(UART_NUMBER_0,UART_LOCATION_2,RX);

  return;
  }

/******************************************************************************
 * @fn          uart_tx_message
 *
 * @brief       Installs transmit handler if no message currently being sent
 *
 * input parameters
 * @param   handler - UART transmit handler
 *
 * @return   Status of the operation.
 *           true                 Transmit handler successfully installed
 *           false                Message being sent or handler is invalid
 *
 */
bool uart1_tx_message( uart_get_tx_data_type handler )
  {
  bspIState_t  intState;
  bool status = false; /* assume failure initially */

  /* updates required, store interrupt state and disable interrupts */
  BSP_ENTER_CRITICAL_SECTION(intState);

  /* if no message is currently being sent and handler looks valid */
  if( uart1_tx_handler == NULL && handler != NULL )
    {
    uart1_tx_handler = handler; /* install the handler */

    /* once the handler has been setup, enable the interrupt.
     * this will cause the message to begin transmission */
    UART_IRQ_ENABLE( UART_NUMBER_1, UART1_LOCATION, TX );

    status = true; /* indicate success */
    }

  BSP_EXIT_CRITICAL_SECTION(intState); /* restore interrupt state */

  return status; /* indicate status */
  }

/******************************************************************************
 * @fn          uart_rx_message
 *
 * @brief       Installs receive handler if no message currently being received
 *
 * input parameters
 * @param   handler - UART receive handler
 *
 * @return   Status of the operation.
 *           true                 Receive handler successfully installed
 *           false                Message being received or handler is invalid
 *
 */
bool uart_rx_message( uart_put_rx_data_type handler )
  {
  bspIState_t intState;
  bool status = false;  /* assume failure initially */

  /* updates required, store interrupt state and disable interrupts */
  BSP_ENTER_CRITICAL_SECTION(intState);

  /* if no message is being received and the handler looks valid */
  if( uart_rx_handler == NULL && handler != NULL )
    {
    uart_rx_handler = handler; /* install the handler */

    status = true; /* indicate success */
    }

  BSP_EXIT_CRITICAL_SECTION(intState); /* restore interrupt state */

  return status; /* indicate status */
  }

/******************************************************************************
 * @fn          uart1_tx_irq
 *
 * @brief       TX interrupt service routine
 *
 * input parameters
 *
 * output parameters
 *
 * @return
 */
void uart0_tx_irq( void )
  {
  unsigned char c;
  uart_get_tx_data_type handler;

  BSP_CRITICAL_STATEMENT( handler = uart0_tx_handler );

  /* if a handler exists */
  if( handler != NULL )
    {
    if( (*handler)( &c ) != false ) /* if this is not the last byte to send */
      {
      bspIState_t intState;
      BSP_ENTER_CRITICAL_SECTION( intState );

      /* only reset the interrupt flag if we have additional data to send
       * that way, if we are done then the interrupt is still pending and
       * will be immediately entered upon re-enabling it.*/
      UART_IRQ_FLAG_CLR( UART_NUMBER_0, UART0_LOCATION, TX ); /* eset the interrupt */

      BSP_EXIT_CRITICAL_SECTION( intState );
      }
    else
      {
      bspIState_t intState;
      BSP_ENTER_CRITICAL_SECTION( intState );

      /* we're done sending data.  since we left the interrupt pending,
       * disable it so we don't re-enter the isr.  the interrupt will be
       * re-enabled when there is another message to send. */
      UART_IRQ_DISABLE( UART_NUMBER_0, UART0_LOCATION, TX );

      /* no more data to send, reset the handler to flag not busy */
      uart0_tx_handler = NULL;

      BSP_EXIT_CRITICAL_SECTION( intState );
      }

    UART_SEND( UART_NUMBER_0, UART0_LOCATION, c ); /* send the byte */
    }
  else /* if no handler exists?!?!?!? */
    /* something went wrong, disable interrupts so we don't get stuck here */
    UART_IRQ_DISABLE( UART_NUMBER_0, UART0_LOCATION, TX );

  return;
}


/******************************************************************************
 * @fn          uart_tx_message
 *
 * @brief       Installs transmit handler if no message currently being sent
 *
 * input parameters
 * @param   handler - UART transmit handler
 *
 * @return   Status of the operation.
 *           true                 Transmit handler successfully installed
 *           false                Message being sent or handler is invalid
 *
 */
bool uart0_tx_message( uart_get_tx_data_type handler )
  {
  bspIState_t  intState;
  bool status = false; /* assume failure initially */

  /* updates required, store interrupt state and disable interrupts */
  BSP_ENTER_CRITICAL_SECTION(intState);

  /* if no message is currently being sent and handler looks valid */
  if( uart0_tx_handler == NULL && handler != NULL )
    {
    uart0_tx_handler = handler; /* install the handler */

    /* once the handler has been setup, enable the interrupt.
     * this will cause the message to begin transmission */
    UART_IRQ_ENABLE( UART_NUMBER_0, UART0_LOCATION, TX );

    status = true; /* indicate success */
    }

  BSP_EXIT_CRITICAL_SECTION(intState); /* restore interrupt state */

  return status; /* indicate status */
  }
