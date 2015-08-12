#include "cab202_graphics.h"

// draws balls at specific points on the screen
void draw_balls( void )
{
	draw_char(63, 23, 'O');
	draw_char(41, 4, 'O');
	draw_char(52, 20, 'O');
	draw_char(55, 5, 'O');

	show_screen();
}

int main( void )
{
	setup_screen();
	
	draw_balls();

	draw_string( 0, screen_height() - 1, "Press any key to finish...");
	wait_char();
	cleanup_screen();
}
