/*
 * serial_debug.c
 *
 *  Created on: Aug 25, 2020
 *      Author: Joe Krachey
 */
#include "serial_debug.h"
#include "circular_buffer.h"
Circular_Buffer *Tx_Buffer;

volatile char buffer[RX_BUFFER_MAX_SIZE];
volatile int buffer_size;
volatile bool rx_done;
volatile bool accepting_user_input;

const char linefeed = 0x0A;
const char carriage_return = 0x0D;

//****************************************************************************
// Helper function used to configure the eUSCI
//****************************************************************************
static void serial_debug_init_uart(void)
{
    // Configure the IO pins used for the UART and set 2-UAR pin as secondary function
    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | EUSCI_B_CTLW0_SSEL__SMCLK; // keep in reset, configure clk src

    // Baud rate calculation = clk_freq/(16*115200) = 13.020833
    // fractional portion * 16 is still 0 when cast to int
    EUSCI_A0->BRW = 26; // Whole number part
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16; // fractional portion of baud rate, turn on oversampling

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Enable eUSCI in UART mode

    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG | EUSCI_A_IFG_TXIFG); // Clear outstanding interrupts

    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;    // Enable Rx interrupt
    NVIC_EnableIRQ(EUSCIA0_IRQn);     // Enable EUSCIA0 in NVIC

    // Init globals
    buffer_size = 0;
    rx_done = false;
    allow_user_input(false);

    // Prime the pump
    EUSCI_A0->TXBUF = 0;
}
//****************************************************************************
// Initializes the EUSCI_A0 peripheral to be a UART with a baud rate of 115200
//
// NOTE: This function assumes that the SMCLK has been configured to run at
// 24MHz.  Please change __SYSTEM_CLOCK in system_msp432p401r.c to the
// correct value of 24000000
// ****************************************************************************/
void serial_debug_init(void)
{
    // Initialize the UART
    serial_debug_init_uart();
    Tx_Buffer = circular_buffer_init(80);
}

void transmit_character(char c){
    // overhead of 2 bits for each byte, so each byte actually takes 10 bits
    // To transmit 1 byte of data, takes '10*(1/115200)*clk_freq' clock cycles
    while(EUSCI_A0->STATW & EUSCI_A_STATW_BUSY);
    EUSCI_A0->TXBUF = c;
}

//****************************************************************************
// Prints a string to the serial debug UART
// ****************************************************************************/
void serial_debug_put_string(char * s)
{
    while(*s)
    {
        transmit_character(*s++);
    }
}

//****************************************************************************
// This function is called from MicroLIB's stdio library.  By implementing
// this function, MicroLIB's putchar(), puts(), printf(), etc will now work.
// ****************************************************************************/
int fputc(int c, FILE* stream)
{
    // Busy wait while the circular buffer is full -- ADD CODE
    while(circular_buffer_full(Tx_Buffer));
    // globally disable interrupts
    __disable_irq();
    // add the character to the circular buffer  -- ADD CODE
    circular_buffer_add(Tx_Buffer, (char)c);
    // globally enable interrupts
      __enable_irq();

    // Enable Tx Empty Interrupts  -- ADD CODE
    EUSCI_A0->IE |= EUSCI_A_IE_TXIE;
    return 0;
}

//****************************************************************************
// UART interrupt service routine
// ****************************************************************************/
void EUSCIA0_IRQHandler(void)
{
    if(accepting_user_input && EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        // Reading from RXBUF automatically clears Rx interrupt
        const char inbound = EUSCI_A0->RXBUF;
        transmit_character(inbound);
        buffer[buffer_size] = inbound;
        if(inbound == linefeed || inbound == carriage_return){
            // Carriage return or linefeed in terminal causes overwritten data when printed :(
            buffer[buffer_size] = '\0';
            rx_done = true;
            accepting_user_input = false;
        }
        buffer_size++;
    }

    // Check for Tx Interrupts
    if (EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)
    {
        //Check to see if the Tx circular buffer is empty
        if(circular_buffer_empty(Tx_Buffer))
        {
            // Disable Tx Interrupts -- ADD CODE
            EUSCI_A0->IE &= ~EUSCI_A_IE_TXIE;
        }
        else
        {
            // Get the next char from the circular buffer -- ADD CODE
            char inbound = circular_buffer_remove(Tx_Buffer);
            // Transmit the character
            transmit_character(inbound);
        }
    }
}

// Clear buffer
void clear_rx_buffer(void){
    rx_done = false;
    memset((void*)buffer, 0, RX_BUFFER_MAX_SIZE);
    buffer_size = 0;
}

// Begin taking user input, which is disabled upon receiving a return
void allow_user_input(bool permitted){
    accepting_user_input = permitted;
}
