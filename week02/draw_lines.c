#include "cab202_graphics.h"

// draws lines using '%'
void draw_lines ( void )
{
	draw_line( 35, 13, 50, 15, '%' );
	draw_line( 50, 15, 32, 3, '%' );
	draw_line( 32, 3, 24, 3, '%' );
	draw_line( 24, 3, 35, 13, '%');

	show_screen();
}

void main ( void )
{
	setup_screen();

	draw_lines();
	
	draw_string( 0, screen_height() - 1, "Press any key to finish...");
	wait_char();
	cleanup_screen();
}
