#include "game.h"
#include "player.h"
#include "alien.h"

int main( void )
{
	int key = 'r';
	while ( key == 'r' )
	{
		setup_screen();
		draw_player( screen_width() / 2 );
		draw_level( 1, 3, 0 );
		declare_aliens( 0 );
		show_screen();
		key = move_player( key );
	}
	cleanup_screen();
	
	return 0;
}
