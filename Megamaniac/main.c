#include "game.h"
#include "player.h"
#include "alien.h"

int main( void )
{
	Game game;
	Alien alien;
	
	srand( time ( NULL ) );

	setup_screen();
	setup_level( &game );
	setup_player();
	setup_aliens( &alien );
	show_screen();

	int turn_result = TURN_READY;
	bool banner_drawn = false;
	while ( turn_result != GAME_FINISHED )
	{
		while ( turn_result != TURN_GAME_OVER )
		{
			turn_result = play_turn( &alien, &game );
			if ( turn_result == TURN_SCREEN_CHANGED )
			{
				draw_game( &alien, &game );
			}
			else if ( turn_result == GAME_FINISHED )
			{
				break;
			}
		}
		
		if( turn_result == GAME_FINISHED )
		{
			break;
		}
		else
		{
			if( !banner_drawn )
			{
				turn_result = draw_death_banner();
				banner_drawn = true;
			}
			else
			{
				turn_result = player_choice();
				if( turn_result == RESTART )
				{
					cleanup_game( &alien, &game );
					banner_drawn = false;
					turn_result = TURN_READY;
				}
			}
		}
	}
	cleanup_screen();
	return 0;
}
