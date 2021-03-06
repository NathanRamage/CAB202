// standard libraries
#include <stdlib.h>

// external libraries
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include <cab202_sprites.h>

typedef struct Game
{
	sprite_id dog;
} Game;

int main( void )
{
	Game game;
	setup_screen();
	game.dog = create_sprite( 0, 0, 1, 1, "D" );
	draw_sprite( game.dog );
	wait_char();
	cleanup_screen();	
}
