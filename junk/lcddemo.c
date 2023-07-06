/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */
#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"


#define LED_RED BIT6
#define LED_GREEN BIT0
#define LEDS (LED_GREEN | LED_RED )
#define SW1 BIT3

#define TSW1 BIT0 //--confirmed SW1 == BIT0
#define TSW2 BIT1 //--confirmed SW2 == BIT1
#define TSW3 BIT2 //--confirmed SW3 == BIT2
#define TSW4 BIT3 //--confirmed SW4 == BIT3

#define SWITCHES SW1
#define TSWITCHES (TSW1 | TSW2 | TSW3 | TSW4)

u_char p1Position =  screenHeight / 2;


/** Initializes everything, clears the screen, draws "hello" and a square */
int
main() {

 WDTCTL = WDTPW | WDTHOLD;     /* stop watchdog timer */

  /*  Set both LEDS to output bits */
  /*  LEDS initially off */
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;

  P1REN |= SWITCHES;		/* enables resistors for switches */
  P1IE  |= SWITCHES;		/* enable interrupts from switches */
  P1OUT |= SWITCHES;		/* pull-ups for switches */
  P1IES |= SWITCHES;            /* Hi/Lo edge */
  P1DIR &= ~SWITCHES;		/* set switches' bits for input */
  P1IFG &= ~SWITCHES;           /* clear just in case */

  //--- configure the top board
  P2DIR &= ~TSWITCHES;
  P2IFG &= ~TSWITCHES;
  P2REN |=  TSWITCHES;
  P2IE  |=  TSWITCHES;
  P2OUT |=  TSWITCHES;
  P2IES |=  TSWITCHES;
  
  // or_sr(0x18);

  
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);
  // clearScreen(COLOR_RED);

  // drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  // fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  // drawRightLine(10,10,COLOR_RED);
  //drawRectOutline( 2 , 2 , 20 , 20 , COLOR_RED);
  // drawPixel(10 , 100 , COLOR_RED);
  //drawVerticalLine(20, COLOR_RED);
  // drawVerticalLine(-20, COLOR_RED);

  //u_char p1Position = height/2;
  //u_char p2Position;

  // drawVerticalLine(p1Position, COLOR_RED);
  
  or_sr(0x18);

}


void drawVerticalLine (u_char row, u_int colorBGR) {
  for (int i=0; i<5; i++){
    //drawPixel(i , row , colorBGR);
    for (int j=0; j<30; j++){
      drawPixel(i,row+j,colorBGR);
    }
  }
}
  


void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if ( P2IFG & TSW1 ) {
    // P2IFG &= ~TSW1;
    // P1OUT |= LEDS;
    // clearScreen(COLOR_BLUE);
    drawVerticalLine(p1Position,COLOR_RED);
    //p1Position += 1;
   } else if ( P2IFG & TSW2 ) {
     P2IFG &= ~TSW2;
     //buzzer_set_period(1000);
   } else if ( P2IFG & TSW3 ) {
     P2IFG &= ~TSW3;
     // buzzer_set_period(3000);
   } else if ( P2IFG & TSW4 ) {
     P2IFG &= ~TSW4;
     // buzzer_set_period(0);
   }
  
}
