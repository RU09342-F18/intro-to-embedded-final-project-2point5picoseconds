#include <msp430.h> 


/**
 * Morse Code Translator V2
 * Author: Jordan Alberico & Tim Duong
 * Date Created: 12/12/18
 * Last Modified: 12/18/18
 */

extern void TimerASetup();
extern void UARTSetup();
extern void LEDSetup();
extern void ButtonSetup();
extern void Compare();

int volatile dash = 800;
int volatile time = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    TimerASetup();              // Setup Timers
    ButtonSetup();              // Setup Button
    UARTSetup();                // Setup UART
    LEDSetup();                 // Setup LEDs
    BCSCTL3 |= LFXT1S_2;        // Needed for G2 to function in Flash mode
    __bis_SR_register(GIE);      // Enable global interrupt
    while(1)
        {
            //do nothing
        }
}

void LEDSetup()
{
    P1SEL &= ~BIT0; //configure port 1 for GPIO
    P1OUT &= ~BIT0; //Turn LED 1.0 off
    P1DIR |= BIT0; //sets port 1.0 as an output
}

void ButtonSetup()
{
    P1REN &= ~BIT3; //enable pull down resistor on 1.3
    P1DIR &= ~BIT3; //sets port 1.3 as an input
    P1IE |= BIT3; //enables interrupt on port 1.3
    P1IES |= BIT3; //sets as falling edge
    P1IFG &= ~BIT3; //clear interrupt flag
}


void TimerASetup()
{
    //***********Dash Timer*************
    TA0CTL = MC_0 + TASSEL_1 + ID_3; //ACLK up mode, stopped
    TA0CCR0 = 0;

    //**********End Character Timer************
    TA1CTL = MC_0 + TASSEL_1 + ID_3;        //stop clock, ACLOCK, and Interal divider of 3
    TA1CCTL0 = CCIE;                        //CCR0 is configured as an interrupt
    TA1CCR0 = 2*dash;                       //Time until an end character is sent
}

void UARTSetup()
{
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;
    P1SEL = BIT1 + BIT2;                      // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2;
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 104;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}


void Compare()
{
    while (!(IFG2 & UCA0TXIFG));
    if (time > dash)
        UCA0TXBUF = 0x01;   // Send a dot if the stored time is less than the preset dash time
    if (time <= dash)
        UCA0TXBUF = 0x00;   // Send a dash if the stored time is greater than the dash time
    P1IFG &= ~BIT3;         //clear interrupt flag
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if ((P1IES & BIT3) == BIT3) //checks if button press is on falling edge
    {
        TA1CTL = MC_0 + TASSEL_1 + ID_3; // Stop clock, ACLOCK, and Interal divider of 3
        P1IES &= ~BIT3;                  // Sets as rising edge
        TA0CTL = TACLR;                  // Clear TimerA
        TA0CTL = MC_2 + TASSEL_1 + ID_3; // Sets as continuous, ACLOCK, and Interal divider of 3
        P1OUT ^= BIT0;                   // Toggle light for feedback
    }
    else
    {
        TA0CTL = MC_0 + TASSEL_1 + ID_3;    // Stop clock, ACLOCK, and Interal divider of 3
        P1IES |= BIT3;                      // Sets as falling edge
        time = TA0R;                        // Time varible set to current timer value
        Compare();
        TA0CTL = MC_1 + TASSEL_1 + ID_3;    // Up mode, ACLOCK, Internal Divider of 3
        P1OUT ^= BIT0;

        //******Clear & Start 'End Character' Timer******
        TA1CTL |= TACLR;                 //clear TimerA
        TA1CTL = MC_2 + TASSEL_1 + ID_3; //up mode, ACLOCK, Internal Divider of 3
    }
    __delay_cycles(1000);
    P1IFG &= ~BIT3;         //clear interrupt flag
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    //***************'End Character' Timer************************
    TA1CTL |= MC_0 + TASSEL_1 + ID_3; // Stop clock, ACLOCK, and Internal divider of 3
    TA1CTL |= TACLR;                  // Clear TimerA
    UCA0TXBUF = 0x02;                 // Transmit 2 for a space
    TA1CTL = ~TAIFG;                  // Clear Flag
    P1IFG &= ~BIT3;                   // Clear interrupt flag
}
