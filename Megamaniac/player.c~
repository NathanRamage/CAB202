/* External libraries */
#include "player.h"

string death[DEATH_BANNER_SIZE];

/* draws the player */
void draw_player( sprite_id player )
{
	draw_sprite( player );
	draw_sprite( bullet );
}

/* sets intial position for the player */
void setup_player( void )
{
	int left = ( screen_width() / 2 );
	int top = ( screen_height() - 4 );

	player = create_sprite( left, top, 1, 1, "$" );
	bullet = create_sprite( 0, 0, 1, 1, "^" );
	bullet->is_visible = false;
	bullet->dy = 1;
	shot_bullet = false;

	death[0] = "@@@@@                                        @@@@@";
	death[1] = "@@@@@@@                                      @@@@@@@";
	death[2] = " @@@@@@@@       @@@@@@@@@@@@@@@@@@@        @@@@@@@@";
	death[3] = "   @@@@@@     @@@@@@@@@@@@@@@@@@@@@     @@@@@@@";
	death[4] = "       @@@@  @@@@@@@@@@@@@@@@@@@@@@@@@  @@@@";
	death[5] = "            @@@@@@@    @@@@@@    @@@@@@";
	death[6] = "             @@@@@@    @@@@@@    @@@@@";
	death[7] = "              @@@@@@@@@@@  @@@@@@@@@@";
	death[8] = "           @@   @@@@@@@@@@@@@@@@@@@  @@";
	death[9] = "           @@@@  @@@@ @ @ @ @ @@@@  @@@@";
	death[10] = "        @@@@@      @@@@@@@@@@@@@      @@@@@";
	death[11] = "      @@@@          @@@@@@@@@@@          @@@@";
	death[12] = " @@@@@@@              @@@@@@@              @@@@@@@@";
	death[13] = "  @@@@@                                     @@@@@@";
	death[14] = "";
	death[15] = "####################################################";
	death[16] = "##                   You Died!                    ##";
	death[17] = "##  Press 'Q' to quit or 'R' to restart the game  ##";
	death[18] = "####################################################";

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
	else if ( key == 's' )
	{
		if ( !shot_bullet )
		{
			bullet->is_visible = TRUE;
			bullet->x = player->x;
			bullet->y = player->y - 1;
			shot_bullet = true;
		}
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

void reset_player( void )
{
	int left = ( screen_width() / 2 );
	player->x = left;
}

int draw_death_banner( void )
{
	int left = ( screen_width() / 2 ) - ( 52 / 2);
	int top = ( screen_height() / 2 ) - ( DEATH_BANNER_SIZE / 2 );
	clear_screen();
	for( int i = 0; i < DEATH_BANNER_SIZE; i++ )
	{
		draw_formatted( left, top + i, death[i] );
	}
	show_screen();
	
	int key = get_char();
	
	if ( key == 'q' || key == 'Q' )
	{
		return -2;
	}
	else if ( key == 'r' || key == 'R' )
	{
		return -1;
	}
	else 
	{
		return 0;
	}
}

int player_choice( void )
{	
	int key = get_char();
	
	if ( key == 'q' || key == 'Q' )
	{
		return -2;
	}
	else if ( key == 'r' || key == 'R' )
	{
		return -3;
	}
	else 
	{
		return 0;
	}
}
