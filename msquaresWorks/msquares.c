#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include <string.h>
#include <stdlib.h>
#include "libTimer.h"

#ifndef buzzer_included
#define buzzer_included

void buzzer_init();
void buzzer_set_period(short cycles);

#endif // included



typedef struct {
  short col, row;
} Pos;

Pos positions[] = {
  {screenWidth/2 , screenHeight/2},	      /* Balls startign position */
  {10,screenHeight/2-15}, 	              /* Player 1 board position */
  {screenWidth-10,screenHeight/2-15},         /* Player 2 board position */
};


#define NUM_POSITIONS 5  

//unsigned short sqColors[] = {COLOR_RED, COLOR_GREEN, COLOR_ORANGE, COLOR_BLUE};

#define NUM_SQCOLORS 4
#define BG_COLOR COLOR_BLACK

char current_position = 0, current_color = 0;
  
// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */
#define SWITCHES 15

int redrawScreen = 1;
int barWidth = 5;
int barHeight = 30;
int barStep = 10;
#define PONG_SPEED 5

/* my functions start here */
void drawVerticalLine(int pNum , u_char col , u_int colorBGR);
void drawBar(int x , int y, int width , int height, u_int colorBGR);    
int checkBlock(int pNum, int width , int height);
void siren_leds();
void sleep(double);
void goalPattern(int x);
void buzzer_init();
void update_shape();

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();

  configureClocks();
  buzzer_init();
  
  WDTCTL = WDTPW | WDTHOLD;     /* stop watchdog timer */

}

int switches = 0;

void
switch_interrupt_handler()
{
  int topOfScreen = 20;
  int botOfScreen = screenHeight - barHeight;
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;

  if (switches & SWITCHES) { 	/* a switch is depresssed */
    redrawScreen = 1;
    for (char swNum = 0; swNum < 4; swNum++) { /* respond to lowest button pressed */
      int swFlag = 1 << swNum;
      if (switches & swFlag) {
	switch (swNum) {
	case 0:
	  drawBar(positions[1].col, positions[1].row + barHeight - barStep, barWidth , barStep, COLOR_BLACK);
	  positions[1].row -= barStep;
	  if ( positions[1].row <= topOfScreen ) positions[1].row = topOfScreen;
	  break;
	case 1:
	  drawBar(positions[1].col, positions[1].row , barWidth, barStep , COLOR_BLACK);
	  positions[1].row += barStep;
	  if ( positions[1].row >= botOfScreen ) positions[1].row = botOfScreen;
	  break;
	case 2:
	  drawBar(positions[2].col, positions[2].row + barHeight - barStep, barWidth , barStep, COLOR_BLACK);
	  
	  positions[2].row -= barStep;
	  if ( positions[2].row <= topOfScreen ) positions[2].row = topOfScreen; 
	  break;
	case 3:
	  drawBar(positions[2].col, positions[2].row , barWidth, barStep , COLOR_BLACK);
	  positions[2].row += barStep;
	  if ( positions[2].row >= botOfScreen ) positions[2].row = botOfScreen;

	  break;
	default:
	  break;
	}
	break;
      }
    }
  }
}

void wdt_c_handler()
{
  static int sec2Count = 0;
  static int sec1Count = 0;
  if (sec2Count++ >= 125) {		/* 2/sec */
    sec2Count = 0;
    current_color = (current_color+1) % NUM_SQCOLORS;
    redrawScreen = 1;
  }
  if (sec1Count++ >= 250) {		/* 1/sec */
    sec1Count = 0;
    current_position = (current_position+1) % NUM_POSITIONS;
    redrawScreen = 1;
  }
}
  
void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(BG_COLOR);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

void
update_shape()
{
  buzzer_set_period(0);
  static int dx = PONG_SPEED, dy = 0;
  static int goalsP1 = 0, goalsP2 = 0;
  static char scoreP1[16], scoreP2[16];

  short col = positions[0].col;
  short row = positions[0].row;

  drawPixel(col , row, COLOR_BLACK);
  col += dx;
  row += 0;

  /* using 5x7 font */
  //drawString5x7(screenWidth/2-10 , 10, "Pong" , COLOR_GREEN, COLOR_BLACK);
  //drawString5x7(screenWidth/2-30 , 10, itoa(goalsP1, scoreP1, 16) , COLOR_GREEN , COLOR_BLACK);
  //drawString5x7(screenWidth/2+30 , 10, itoa(goalsP2, scoreP2, 16) , COLOR_GREEN , COLOR_BLACK);

  /* using 8x12 font */
  //drawString8x12(screenWidth/2-26 , 10, "Pong" , COLOR_GREEN, COLOR_BLACK);
  //drawString8x12(5 , 10, itoa(goalsP1, scoreP1, 16) , COLOR_GREEN , COLOR_BLACK);
  //drawString8x12(screenWidth-25 , 10, itoa(goalsP2, scoreP2, 16) , COLOR_GREEN , COLOR_BLACK);

  /* using 11x16 font */
  drawString11x16(screenWidth/2-22 , 10, "Pong" , COLOR_GREEN, COLOR_BLACK);
  drawString11x16(10 , 10, itoa(goalsP1, scoreP1, 16) , COLOR_GREEN , COLOR_BLACK);
  drawString11x16(screenWidth - 24 , 10, itoa(goalsP2, scoreP2, 16) , COLOR_GREEN , COLOR_BLACK);
  
  drawPixel( col , row , COLOR_GREEN);

  drawBar(positions[1].col,positions[1].row , barWidth , barHeight , COLOR_RED);
  drawBar(positions[2].col,positions[2].row , barWidth , barHeight , COLOR_BLUE);
 
  positions[0].col = col;
  positions[0].row = row;

  if (checkBlock(1 , barWidth , barHeight)){
    //srand(time(0));
    int n = (rand() % PONG_SPEED + 1);
    dx = -dx;
    dy = n;
    return;
  }
  if (checkBlock(2 , barWidth , barHeight)){
    //srand(time(0));
    int n = (rand() % PONG_SPEED + 1);
    dx = -dx;
    dy = -n;

    return;
  }

  if ( positions[0].col >= screenWidth ){    /* if arrives here goal is scored by p1 */
    dx = -5;
    drawPixel(col , row , COLOR_BLACK);
    positions[0].col = screenWidth/2;
    positions[0].row = screenHeight/2;
    goalsP1 += 1;
    goalPattern(2000);
  }
  
  if (positions[0].col <= 0 ){       /* if arrives here goal is scored by p2 */
    dx = 5;
    drawPixel(col , row , COLOR_BLACK);
    positions[0].col = screenWidth/2;
    positions[0].row = screenHeight/2;
    goalsP2 += 1;
    goalPattern(2000);
  }

  if ( positions[0].row <= 10 ) {
    dy = -dy;
    positions[0].row = 10;
  }
  if ( positions[0].row >= screenHeight - 10){
    dy = -dy;
    positions[0].row = screenHeight - 10;
  }

}

/* Function to draw a players Bar for Pong game */
void drawBar(int x , int y, int width , int height, u_int colorBGR) {
  for (int i = x; i<x+width; i++){
    for (int j=y; j<y+height; j++){
      drawPixel(i,j,colorBGR);
    }
  }
}

/* Function to check if ball colided with a players bar */
int checkBlock(int pNum, int width , int height){
  int top = positions[pNum].row;
  int bottom = positions[pNum].row+height;
  int left = positions[pNum].col;
  int right = positions[pNum].col+width;

  /* if both true collision occured */
  if (positions[0].row >= top && positions[0].row <= bottom){
    if(positions[0].col >= left && positions[0].col <= right){
      return 1;
    }
  }
  /* no collision */
  return 0;
}

void goalPattern(int x){
  
  for (int i=0; i<10; ++ i ) {
    if ( x/(i+1) < 10 ) break;
    buzzer_set_period(x/(i+1));
    sleep(1000);
  }
}

void sleep(double n){
  while(n>0){
    n--;
  }
}

void buzzer_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */
}

void buzzer_set_period(short cycles) /* buzzer clock = 2MHz.  (period of 1k results in 2kHz tone) */
{
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}

/* Switch on S2 */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
