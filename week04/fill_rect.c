// Standerd libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// External libraries
#include "cab202_timers.h"
#include "cab202_graphics.h"

void fill_rect( int left, int top, int width, int height, char ch )
{

	int right = left + width - 1;
	int bottom = top + height - 1;

	if( width <= 0 || height <= 0 )
	{
	}
	else if( width == 1 && height == 1 )
	{
		draw_char( left, top, ch );
	}
	else if( width == 1 )
	{
		draw_line( left, top, left, bottom, ch );
	}
	else if ( height == 1 )
	{
		draw_line( left, top, right, top, ch );
	}
	else
	{
		for( int i = 0; i < height; i++ )
		{
			draw_line( left, top + i, right, top + i, ch );
		}
	}
}

// Generate coordinates, draw box.
void demo()
{
	int x = rand() % screen_width() / 2;
	int y = rand() % screen_height() / 2;
	int w = rand() % ( screen_width() - x );
	int h = rand() % ( screen_height() - y );
	char * chars = "~!@#$%^&*-+";
	char ch = chars[rand() % strlen( chars )];

	fill_rect( x, y, w, h, ch );
	
	show_screen();
}

int main( void )
{
	setup_screen();
	srand( time( NULL ) );
	demo();
	timer_pause( 5000 );
	cleanup_screen();
	return 0;
}
