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
	while ( turn_result != TURN_GAME_OVER )
	{
		turn_result = play_turn( &alien );
		if ( turn_result == TURN_SCREEN_CHANGED )
		{
			draw_game( &alien, &game );
		}
		timer_pause( 5 );
	}
	cleanup_screen();
	return 0;
}
