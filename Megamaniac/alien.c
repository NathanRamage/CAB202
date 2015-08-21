/* External libraries */
#include "alien.h"

/* frees the memory allocated for the alien sprite array, bomb_time and alien timer */
void cleanup_aliens( Alien * alien )
{
	free( alien->sprite[NUM_ALIENS] );
	free( alien->bomb_timer );
	free( alien->alien_timer );
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

	//set initial positions for teh aliens
	alien->sprite[0] = create_sprite( 3, 1, 1, 1, alien->body );
	alien->sprite[1] = create_sprite( 9, 1, 1, 1, alien->body );
	alien->sprite[2] = create_sprite( 15, 1, 1, 1, alien->body );
	alien->sprite[3] = create_sprite( 0, 3, 1, 1, alien->body );
	alien->sprite[4] = create_sprite( 6, 3, 1, 1, alien->body );
	alien->sprite[5] = create_sprite( 12, 3, 1, 1, alien->body );
	alien->sprite[6] = create_sprite( 18, 3, 1, 1, alien->body );
	alien->sprite[7] = create_sprite( 3, 5, 1, 1, alien->body );
	alien->sprite[8] = create_sprite( 9, 5, 1, 1, alien->body );
	alien->sprite[9] = create_sprite( 15, 5, 1, 1, alien->body );

	/* set how much the aliens will move across the screen */
	for ( int i = 0; i < NUM_ALIENS; i++ )
	{
		alien->sprite[i]->dx = 0.2;
	}
	
	draw_aliens( alien );
}

/* creates a new bomb */
void create_bomb( int bomb_num, Alien * alien )
{
	int chosen_one;
	
	/* selects a random alien that did not drop the last bomb */
	do
	{
		chosen_one = rand() % ( NUM_ALIENS );
	} while ( alien->last_bomb[chosen_one] );
	
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
	for ( int the_one = chosen_one; the_one < NUM_ALIENS; ++the_one )
	{
		if ( alien->last_bomb[the_one] )
		{
			alien->last_bomb[the_one] = false;
		}
	}

	/* end search */

	/* set new bomb properties */
	alien->bombs[bomb_num]->is_visible = TRUE;
	alien->bombs[bomb_num]->x = alien->sprite[chosen_one]->x;
	alien->bombs[bomb_num]->y = alien->sprite[chosen_one]->y;
	alien->bombs[bomb_num]->dy = 0.2;

	alien->last_bomb[chosen_one] = true; //flag the bomb dropper for later
}

/* move the bombs and destory bombs that have hit the ground or player */
void update_bombs ( Alien * alien )
{
	/* finds number of bombs that can be shown on the screen */
	alien->possible_bombs = 1;
	for ( int i = 0; i < MAX_NUM_BOMBS; i++ )
	{
		if (  alien->bombs[i]->is_visible == TRUE )
		{
			alien->possible_bombs++;
		}
	}
	
	/* define the number of bombs that can be shown on the screen */
	if ( alien->numBombs < MAX_NUM_BOMBS && alien->numBombs < alien->possible_bombs )
	{
		alien->numBombs++;
	}

	int j = 0;

	/* cycle throught the bomb array and show one new bomb */
	for ( int i = 0; i < alien->numBombs; i++ )
	{
		/* to fix an odd case where there are only 2 bombs on the screen thus there are two
		   invisible bombs that can be shown, the variable j stops to from spawning at once and breaks out of the loop */
		if ( !alien->bombs[i]->is_visible )
		{
			if ( j >= 1 )
			{
				break;
			}
			create_bomb( i, alien );
			j++;
		}
	}
}

/* two cases make this return true, either the alien timer expires or the bomb time expires */
bool update_aliens( Alien * alien )
{
	if ( !timer_expired( alien->alien_timer )  )
	{
		return false;
	}
	else if ( timer_expired( alien->bomb_timer ) )
	{
		update_bombs ( alien );
		return true;
	}
	else
	{
		int width = screen_width();	

		/* move all aliens by dx */
		for ( int i = 0; i < NUM_ALIENS; i++ )
		{
			int new_x = (int) round( alien->sprite[i]->x + alien->sprite[i]->dx );

			/* if the new location is greater than the width of the screen then the new x location = 0 */
			if ( new_x >= width )
			{
				alien->sprite[i]->x = 0;
			}

			alien->sprite[i]->x += alien->sprite[i]->dx;
		}

		/* move bombs */
		for ( int i = 0; i < MAX_NUM_BOMBS; i++ )
		{
			if ( alien->bombs[i]->is_visible == TRUE )
			{
				int new_y = (int) round( alien->bombs[i]->y + alien->bombs[i]->dy );
				if ( new_y >= screen_height() - 3 )
				{
					alien->bombs[i]->is_visible = false;
					alien->bombs[i]->x = 0;
					alien->bombs[i]->y = 0;
				}
				else
				{
					alien->bombs[i]->y += alien->bombs[i]->dy;
				}
			}
		}
		return true;
	}
}
