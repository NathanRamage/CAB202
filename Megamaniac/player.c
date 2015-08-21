/* External libraries */
#include "player.h"

/* draws the player */
void draw_player( sprite_id player )
{
	draw_sprite( player );
}

/* sets intial position for the player */
void setup_player( void )
{
	int left = ( screen_width() / 2 );
	int top = ( screen_height() - 4 );

	player = create_sprite( left, top, 1, 1, "$" );

	draw_player( player );
}

/* updates the position of the player */
bool update_player( int key )
{
	bool player_moved = true;

	int width, height;
	get_screen_size( width, height );
	
	/* if a is pressed move left else if a is pressed more right
	   the player cannot move out of left and right bounds of the screen */
	if ( key == 'd' && player->x < width - 1 )
	{
		player->x++;
	}
	else if ( key == 'a' && player->x > 0 )
	{
		player->x--;
	}
	else
	{
		player_moved = false;
	}
	return player_moved;
}

/* frees the memory allocated for player sprite */
void cleanup_player( void )
{
	free( player );
}
