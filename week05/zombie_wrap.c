// standard libraries
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

// external libraries
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include <cab202_sprites.h>

// structure to hold game data
typedef struct Game
{
	timer_id timer;
	sprite_id zombie;
	int laps;
	bool over;
} Game;

void setup_zombie( Game * game )
{
	game->laps = 0;
	game->over = false;
	game->timer = create_timer( 1100 );
	game->zombie = create_sprite( 8, 9, 1, 1, "Z" );
	game->zombie->dx = 1.14;
}

bool update_zombie( Game * game )
{
	if ( game->over )
	{
	return false;
	}
	else
	{
		int expired = timer_expired( game->timer );
		if ( expired )
		{
			int current_x = (int) round( game->zombie->x );

			game->zombie->x += game->zombie->dx;

			int new_x = (int) round( game->zombie->x );

			if ( new_x >= screen_width() )
			{
				game->zombie->x = 0;
				game->laps++;
				if ( game->laps >= 4 )
				{
					game->over = true;
				}
			}

			if ( new_x != current_x )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

void display_zombie( Game * game )
{
	draw_sprite( game->zombie );
}

// already done
void zombie_wrap( void )
{
	Game game;
	
	setup_zombie( &game );
	
	while ( !game.over )
	{
		if ( update_zombie( &game ) )
		{
			clear_screen();
			display_zombie( &game );
			show_screen();
		}

		timer_pause( 25 );
	}

	timer_pause( 1500 );
}

int main( void )
{
	setup_screen();
	zombie_wrap();
	cleanup_screen();
	return 0;
}
