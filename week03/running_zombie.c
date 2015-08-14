#include "cab202_graphics.h"

void running_zombie( void ) {
	// MODIFY THIS PART TO PLACE ZOMBIE
	int x = screen_width( ) * 20 / 100;
	int y = screen_height( ) * 80 / 100;

	// INSERT VARIABLE TO COUNT ELAPSED TURNS
	int turnPassed = 0;
	int score = 0;

	// MODIFY THE FOLLOWING TO DISPLAY SCORE
	draw_string( 0, screen_height( ) - 1, "Menu: 8 = N; 2 = S; 6 = E; 4 = W; 9 = NE; 3 = SE; 7 = NW; 1 = SW; q = Quit." );
	draw_string( 30, 0, "Score: ");
	draw_int( 37, 0, score);
	draw_char( x, y, 'Z' );

	show_screen( );

	// CHANGE NEXT LINE TO WAIT FOR KEY PRESS
	int key = wait_char( );

	while ( key != 'q' && key >= 0 ) {
		if (key == '4')
		{
			x = x - 1;
			score = score + 1;
			turnPassed = turnPassed + 1;
		}
		else if ( key == '6' )
		{
			x = x +1;
			score = score + 1;
			turnPassed = turnPassed + 1;
		}
		else if ( key == '8' )
		{
			y = y - 1;
			score = score + 1;
			turnPassed = turnPassed + 1;
		}
		else if ( key == '2' )
		{
			y = y +1;
			score = score + 1;
			turnPassed = turnPassed + 1;
		}	
		else if ( key == '1' )
		{
			y = y +1;
			x = x - 1;
			score = score + 1;
			turnPassed = turnPassed + 1;
		}
		else if ( key == '3' )
		{
			y = y +1;
			x = x +1;
			score = score + 1;
			turnPassed = turnPassed + 1;
		}
		else if ( key == '7' )
		{
			x = x - 1;
			y = y - 1;
			score = score + 1;
			turnPassed = turnPassed + 1;
		}
		else if ( key == '9' )
		{
			x = x + 1;
			y = y - 1;
			score = score + 1;
			turnPassed = turnPassed + 1;
		}
		else if ( key == '5' )
		{
			x = x;
			y = y;
			turnPassed = turnPassed + 1;
		}
		else{
			x = x;
			y = y;
			turnPassed = turnPassed + 1;
		}
		
		if (turnPassed % 10 == 0)
		{
			score = score + 1;
		}

		// INSERT ADDITIONAL LOGIC TO MOVE DIAGONALLY AND COUNT ELAPSED TURNS

		// MODIFY TO CLEAR THE SCREEN, THEN REDRAW DISPLAY
		clear_screen();
		draw_string( 0, screen_height( ) - 1, "Menu: 8 = N; 2 = S; 6 = E; 4 = W; 9 = NE; 3 = SE; 7 = NW; 1 = SW; q = Quit." );
		draw_string( 30, 0, "Score: ");
		draw_int( 37, 0, score);
		draw_char( x, y, 'Z' );

		// DON'T MESS WITH THE NEXT LINE
		show_screen( );

		// CHANGE NEXT LINE TO WAIT FOR KEY PRESS
		key = wait_char( );
	}
}

int main( void ) {
	setup_screen( );
	running_zombie( );
	cleanup_screen( );
	return 0;
}