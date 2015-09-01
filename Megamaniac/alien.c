/* External libraries */
#include "alien.h"

/* frees the memory allocated for the alien sprite array, bomb_time and alien timer */
void cleanup_aliens( Alien * alien )
{
	free( alien->bomb_timer );
	free( alien->alien_timer );

	for( int i = 0; i < NUM_ALIENS; i++ )
	{
		free( alien->sprite[i] );
	}

	for( int i = 0; i < MAX_NUM_BOMBS; i++ )
	{
		free( alien->bombs[i] );
	}
}

/* draws aliens */
void draw_aliens( Alien * alien )
{
	for ( int i = 0; i < NUM_ALIENS; i++ )
	{
		draw_sprite( alien->sprite[i] );
	}
}

/* draws bombs */
void draw_bombs( Alien * alien )
{
	for (int i = 0; i < MAX_NUM_BOMBS; i++ )
	{
		if ( alien->bombs[i]->is_visible == TRUE )
		{
			draw_sprite( alien->bombs[i] );
		}
	}
}

/* defines default values for the properties in the alien structure and draws them in their intial positions */
void setup_aliens( Alien * alien )
{
	alien->numBombs = 0;
	alien->bomb_timer = create_timer( BOMB_UPDATE_TIME );
	alien->alien_timer = create_timer( ALIEN_UPDATE_TIME );
	alien->body = "@";
	alien->alien_moved = 0;

	/* sets all aliens to be alive and sets that none of the have dropped a bomb */
	for( int i = 0; i < NUM_ALIENS; i++ )
	{
		alien->alive[i] = true;
		alien->last_bomb[i] = false;
	}

	/* creates sprites for the total amount of bombs that can be on the screen at once */
	for( int i = 0; i < MAX_NUM_BOMBS; i++ )
	{
		alien->bombs[i] = create_sprite( 0, 0, 1,  1, "." );
		alien->bombs[i]->is_visible = false; //make them invisible
	}

	int width = screen_width() / 2;

	//set initial positions for the aliens
	alien->sprite[0] = create_sprite( width - 6, 1, 1, 1, alien->body );
	alien->sprite[1] = create_sprite( width, 1, 1, 1, alien->body );
	alien->sprite[2] = create_sprite( width + 6, 1, 1, 1, alien->body );
	alien->sprite[3] = create_sprite( width - 9, 3, 1, 1, alien->body );
	alien->sprite[4] = create_sprite( width - 3, 3, 1, 1, alien->body );
	alien->sprite[5] = create_sprite( width + 3, 3, 1, 1, alien->body );
	alien->sprite[6] = create_sprite( width + 9, 3, 1, 1, alien->body );
	alien->sprite[7] = create_sprite( width - 6, 5, 1, 1, alien->body );
	alien->sprite[8] = create_sprite( width, 5, 1, 1, alien->body );
	alien->sprite[9] = create_sprite( width + 6, 5, 1, 1, alien->body );

	//set up an order of aliens to loop through for level 4 movement
	alien->split_aliens[0] = 6;
	alien->split_aliens[1] = 2;
	alien->split_aliens[2] = 9;
	alien->split_aliens[3] = 5;
	alien->split_aliens[4] = 3;
	alien->split_aliens[5] = 0;
	alien->split_aliens[6] = 7;
	alien->split_aliens[7] = 4;
	alien->split_aliens[8] = 1;
	alien->split_aliens[9] = 8;


	/* set how much the aliens will move across the screen */
	for ( int i = 0; i < NUM_ALIENS; i++ )
	{
		alien->sprite[i]->dx = 0.2;
		alien->sprite[i]->dy = 0.2;
	}

	draw_aliens( alien );
}

/* creates a new bomb */
void create_bomb( int bomb_num, Alien * alien )
{
	int chosen_one;
	int dead_aliens = 0;

	for ( int i = 0; i < NUM_ALIENS; i++ )
	{
		if ( !alien->alive[i] )
		{
			dead_aliens++;
		}
	}

	if ( dead_aliens == NUM_ALIENS - 1 )
	{
		for ( int i = 0; i < NUM_ALIENS; i++ )
		{
			if ( alien->alive[i] )
			{
				chosen_one = i;
			}
		}
	}
	else
	{
		/* selects a random alien that did not drop the last bomb */
		bool found_alive_alien = false;
		while ( !found_alive_alien )
		{
			chosen_one = rand() % ( NUM_ALIENS );
			if ( !alien->last_bomb[chosen_one] && alien->alive[chosen_one] )
			{
				found_alive_alien = true;
			}
		}
	}

	/* search for who dropped the last bomb to rest them so that they can be eligible
	   to drop the next bomb after the current on has been dropped */

	/* check if the last bomb dropper is before the new bomb dropper */
	for ( int i = 0; i < chosen_one; i++ )
	{
		if ( alien->last_bomb[i] )
		{
			alien->last_bomb[i] = false;
		}
	}

	/* check if the last bomb dropper is after the new bomb dropper */
	for ( int i = chosen_one; i < NUM_ALIENS; ++i )
	{
		if ( alien->last_bomb[i] )
		{
			alien->last_bomb[i] = false;
		}
	}

	/* end search */

	/* set new bomb properties */
	alien->bombs[bomb_num]->is_visible = TRUE;
	alien->bombs[bomb_num]->x = alien->sprite[chosen_one]->x;
	alien->bombs[bomb_num]->y = alien->sprite[chosen_one]->y;
	alien->bombs[bomb_num]->dy = 0.3;

	alien->last_bomb[chosen_one] = true; //flag the bomb dropper for later
}
