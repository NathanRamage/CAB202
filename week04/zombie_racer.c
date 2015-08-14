// Standard libraries
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// External libraries
#include "cab202_timers.h"
#include "cab202_graphics.h"

// Delcare constnts
#define OUTER_PERCENT (90)
#define NORMAL_WIDTH (6)
#define DEFAULT_P (10)

// Declare helper functions
double p_norm( double x, double y, double p );

void get_racetrack_params( 
	int *x0, int *xr_outer, int *xr_inner,
	int *y0, int *yr_outer, int *yr_inner,
	double *p );

void get_start_pos( int *start_x, int *start_y );

// Implement game
bool inside_ellipse( double x, double y, double x0, double y0, double xr, double yr, double p )
{
	double x1 = ((x - x0)/xr);
	double y1 = ((y - y0)/yr);
	
	double inEllipse = p_norm( x1, y1, p );
	
	if ( inEllipse <= 1 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool on_racetrack( int x, int y )
{
	int x0, xr_outer, xr_inner;
	int y0, yr_outer, yr_inner;
	double p;
	
	get_racetrack_params( &x0, &xr_outer, &xr_inner, &y0, &yr_outer, &yr_inner, &p );

	bool inSmallEllipse = inside_ellipse( x, y, x0, y0, xr_inner, yr_inner, p );
	bool inBigEllipse = inside_ellipse( x, y, x0, y0, xr_outer, yr_outer, p );
	
	if ( !inSmallEllipse && inBigEllipse )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void draw_racetrack()
{
	bool raceTrack = false;
	for ( int i = 0; i <= screen_width(); i++ )
	{
		for (int j = 0; j <= screen_height(); j++ )
		{
			raceTrack = on_racetrack( i, j );
			if ( !raceTrack )
			{
				draw_char( i, j, '^' );
			}
		}
	}
}

void draw_zombie( int x, int y, bool zombie_alive )
{
	if ( zombie_alive )
	{
		draw_char( x, y, 'Z' );
	}
	else
	{
		draw_char( x, y, 'X' );
	}
}

void update_view( int zombie_x, int zombie_y, bool zombie_alive )
{
	clear_screen( );
	draw_racetrack( );
	draw_zombie( zombie_x, zombie_y, zombie_alive );
	show_screen( );
}

void zombie_racer( int start_x, int start_y )
{
	int x = start_x;
	int y = start_y;
	
	int dir = 0;

	bool alive = on_racetrack( x, y );
	update_view( x, y, alive );
	int key = wait_char();

	while ( key != 'q' && key >= 0 && alive )
	{
		// Change direction
		if ( key == '1' )
		{
			dir = dir - 45;
			if ( dir == -45 )
			{
				dir = 315;
			}
		}
		else if ( key == '3' )
		{
			dir = dir + 45;
			if ( dir == 360 )
			{
				dir = 0;
			}
		}

		// Take on step forward in the current direction
		if ( dir == 0 )
		{
			y = y - 1;
		}
		else if ( dir == 45 )
		{
			x = x + 1;
			y = y - 1;
		}
		else if ( dir == 90 )
		{
			x = x + 1;
		}
		else if ( dir == 135 )
		{
			x = x + 1;
			y = y + 1;
		}
		else if ( dir == 180 )
		{
			y = y + 1;
		}
		else if ( dir == 225 )
		{
			x = x - 1;
			y = y + 1;
		}
		else if ( dir == 270 )
		{
			x = x - 1;
		}
		else if ( dir == 315 )
		{
			x = x - 1;
			y = y - 1;
		}	

		alive = on_racetrack( x, y );
		update_view( x, y, alive );
		timer_pause( 100 );
		key = wait_char();
	}
}

//---------------------------------------------------------------------------

int main( void )
{
	srand( time( NULL ) );
	setup_screen();

	int start_x;
	int start_y;
	
	get_start_pos( &start_x, &start_y );
	zombie_racer( start_x, start_y );
	
	cleanup_screen();
	return 0;
}

// Helper function implementation

void get_start_pos( int *start_x, int *start_y )
{
	// Get the geometry of the race track
	int x0, y0, xr_outer, yr_outer, xr_inner, yr_inner;
	double p;
	get_racetrack_params( &x0, &xr_outer, &xr_inner, &y0, &yr_outer, &yr_inner, &p );

	// Start at the left, close to the middle of the track
	*start_x = x0 - xr_outer + ( xr_outer - xr_inner ) / 2;
	*start_y = y0;
}

double p_norm( double x, double y, double p )
{
	// Error if bad input
	if ( isinf( x ) || isinf( y ) || p == 0 )
	{
		fprintf( stderr, "Illegal argumentd in p_norm: x = %f, y = %f, p = %f\n", x, y, p );
		abort();
	}

	// Compute p_norm(x,y,p)
	double t1 = fabs( x );
	double t2 = fabs( y );
	return pow( pow( t1, p ) + pow( t2, p ), 1/p );
}

void get_racetrack_params(
	int *x0, int *xr_outer, int *xr_inner,
	int *y0, int *yr_outer, int *yr_inner,
	double *p )
{
	(*x0) = screen_width() / 2;
	(*xr_outer) = OUTER_PERCENT * (screen_width() / 2) / 100;
	(*xr_inner) = (*xr_outer) - NORMAL_WIDTH;

	(*y0) = screen_height() / 2;
	(*yr_outer) = OUTER_PERCENT * (screen_height() / 2) / 100;
	(*yr_inner) = (*yr_outer) - NORMAL_WIDTH;

	(*p) = DEFAULT_P; 
}

