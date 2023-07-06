/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  drawString5x7(10,10, "hello!", COLOR_GREEN, COLOR_BLACK);
  drawString8x12(10,20, "hello!", COLOR_GREEN , COLOR_BLACK);
  drawString11x16(10,30,"hello!", COLOR_GREEN, COLOR_BLACK);
  //fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  
}
