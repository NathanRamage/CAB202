/* External libraries */
#include "player.h"

/* Declare variables */
int score = 0;
int lives = 3;
int level = 1;

void draw_player( int x )
{
	draw_char( x, screen_height() - 4, '$' );
}

int move_player ( int key )
{
	int x = screen_width() / 2;
	key = wait_char();

	while ( key != 'q' && key != 'r' && key >= 0 )
	{
		if ( key == 'a' )
		{
			x = x - 1;
		}
		else if ( key == 'd' )
		{
			x = x + 1;
		}


		if ( x >= 79 )
		{
			x = 79;
		}
		else if ( x <= 0 )
		{
			x = 0;
		}
		update_view( x );
		timer_pause( 100 );
		key = wait_char();
	}

	return key;
}

void update_view( int x )
{
	clear_screen();
	draw_level( level, lives, score );
	draw_player( x );
	move_aliens( 1 );
	show_screen();
}
