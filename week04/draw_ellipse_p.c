// Standard libraries
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// External libraries
#include "cab202_graphics.h"
#include "cab202_timers.h"

/**
 *		P_norm, used by solution but implemented elsewhere
 */
double p_norm( double x, double y, double p );

void draw_ellipse_p( double x0, double y0, double xr, double yr, double p, char ch )
{
	if ( xr > 0 && yr > 0 && p > 0 )
	{
		for ( int i = 0; i < screen_width(); i++ )
		{
			for ( int j = 0; j < screen_height(); j++ )
			{
				double x = ( i - x0 ) / xr;
				double y = ( j - y0 ) / yr;
				double inEllipse = p_norm( x, y, p );
			
				if ( inEllipse <= 1 )
				{
					draw_char( i, j, ch );
				}
			}
		}
	}
}

void demo()
{
	const int N = 5;
	const double X_MAX = screen_width();
	const double Y_MAX = screen_height();
	
	double p[] = { 0.5, 1, 1.5, 2, 3 };

	for ( int i = 0; i < N; i++ )
	{
		clear_screen();
		double x0 = X_MAX / 2;
		double y0 = Y_MAX / 2;
		double xr = X_MAX / 3;
		double yr = Y_MAX / 3;
		draw_ellipse_p( x0, y0, xr, yr, p[i], '*' );
		draw_string( 0, 0, "p = " );
		draw_double( 4, 0, p[i] );
		show_screen();
		timer_pause( 5000 );
	}
}

int main( void )
{
	srand( time( NULL ) );
	setup_screen();
	demo();
	cleanup_screen();
	return 0;
}

/**
 *		Returns the p-norm of 2-D vector (x,y)
 */

double p_norm( double x, double y, double p )
{
	if ( isinf( x ) || isinf( y ) || p == 0 )
	{
		fprintf( stderr, "Illegal arguments in p_norm: x = %f, y = %f, p = %f", x, y, p );
		abort ();
	}

	double t1 = fabs( x );
	double t2 = fabs( y );
	return pow( pow( t1, p ) + pow( t2, p ), 1 / p );
}
