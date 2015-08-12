#include "cab202_graphics.h"
#include <stdio.h>

void draw_paddles( void )
{
	// define the position of the left paddle
	int leftPadx = 3;	
	int leftPadyStart = screen_height()/2 - 7;
	int leftPadyEnd = leftPadyStart + 9;
	
	// draw the left paddle
	draw_line( leftPadx, leftPadyStart, leftPadx, leftPadyEnd, '$');

	// define the postion of the right paddle
	int rightPadx = screen_width() - 4;
	int rightPadyStart = screen_height()/2 - 1;
	int rightPadyEnd = rightPadyStart + 9;

	// draw right paddle
	draw_line( rightPadx, rightPadyStart, rightPadx, rightPadyEnd, '$' );
	show_screen();
}

void main( void )
{
	setup_screen();

	draw_paddles();

	draw_string( 0, screen_height() - 1, "Press any key to finish...");
	show_screen();
	wait_char();
	cleanup_screen();
}
