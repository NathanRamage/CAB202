#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cab202_timers.h"
#include "cab202_graphics.h"

void draw_rect( int left, int top, int width, int height, char ch )
{
	if( width <= 0 || height <= 0 )
	{
	}
	else if( width == 1 )
	{
		draw_line( left, top, left, top + height - 1, ch );
	}
	else if( height == 1 )
	{
		draw_line( left, top, left + width - 1, top, ch );
	}
	else if( width == 1 && height == 1 )
	{
		draw_char( left, top, ch );
	}
	else
	{
		draw_line( left, top, left + width - 1, top, ch );
		draw_line( left + width - 1, top, left + width - 1, top + height - 1, ch );
		draw_line( left, top + height - 1, left + width - 1, top + height - 1, ch );
		draw_line( left, top, left, top + height - 1, ch );
	}
}

void demo()
{
	// Generate coordinates, draw box.
	int x = rand() % screen_width() / 2;
	int y = rand() % screen_height() / 2;
	int w = rand() % ( screen_width() - x );
	int h = rand() % ( screen_height() - y);
	char * chars = "~!@#$%^&*-+";
	char ch = chars[rand() % strlen( chars )];

	draw_rect( x, y, w, h, ch );

	show_screen();
}

int main ( void )
{
	setup_screen();
	srand( time( NULL ) );
	demo();
	timer_pause( 5000 );
	cleanup_screen();
	return 0;
}
