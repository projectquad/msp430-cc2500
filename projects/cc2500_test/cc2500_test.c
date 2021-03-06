/** @file cc2500_test.c
*
* @brief Fade between RGB colors and send out via radio
*
* @author Alvaro Prieto
*/
#include <stdint.h>
#include "device.h"
#include "cc2500.h"

uint8_t txBuffer[6];
static uint8_t rgb[3] = {128, 128, 128};

uint8_t rx_callback( uint8_t*, uint8_t );

int main(void)
{
  int8_t r, g ,b;
  r = 2;
  g = -3;
  b = 1;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  // Setup oscillator for 16MHz operation
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ;

  // Wait for changes to take effect
  __delay_cycles(4000);

  setup_cc2500(rx_callback);

  LED_PxOUT &= ~(LED1); //Outputs
  LED_PxDIR = LED1; //Outputs

  __bis_SR_register(GIE);       // Enter LPM3, enable interrupts

  // Fade through colors
  for(;;)
  {

    // Bounce back and forth between 0 and 255
    if( (rgb[0] > 250) & (r > 0) )
    {
      r = -r;
    }
    if( (rgb[0] < 5) & (r < 0) )
    {
      r = -r;
    }

    if( (rgb[1] > 250) & (g > 0) )
    {
      g = -g;
    }
    if( (rgb[1] < 5) & (g < 0) )
    {
      g = -g;
    }

    if( (rgb[2] > 250) & (b > 0) )
    {
      b = -b;
    }
    if( (rgb[2] < 5) & (b < 0) )
    {
      b = -b;
    }

    // Update current colors
    rgb[0] += r;
    rgb[1] += g;
    rgb[2] += b;

    //Build packet
    txBuffer[0] = 4;            // Packet length
    txBuffer[1] = 0x01;         // Packet address
    txBuffer[2] = rgb[0];       // red
    txBuffer[3] = rgb[1];       // green
    txBuffer[4] = rgb[2];       // blue

    __delay_cycles(100000);
    __delay_cycles(100000);
    __delay_cycles(100000);
    __delay_cycles(100000);
    __delay_cycles(100000);

    // Send message to LED controller!
    cc2500_tx(txBuffer, 5);

    // Toggle local LED to signal transmission
    LED_PxOUT ^= 0x03;
  }

}

// This function is called to process the received packet
uint8_t rx_callback( uint8_t* buffer, uint8_t length )
{
  // Blink the LEDs after receiving a packet
	LED_PxOUT ^= 0x03;

  return 0;
}

