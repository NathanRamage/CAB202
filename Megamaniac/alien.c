/* External libraries */
#include "alien.h"

void draw_aliens( Alien * alien )
{
	for ( int i = 0; i < NUM_ALIENS; i++ )
	{
		draw_sprite( alien->sprite[i] );
	}
}

void draw_bombs( Alien * alien )
{
	for (int i = 0; i < MAX_NUM_BOMBS; i++ )
	{
		if ( alien->bombs[i]->is_visible == 't' )
		{
			draw_sprite( alien->bombs[i] );
		}
	}
}

void setup_aliens( Alien * alien )
{
	alien->numBombs = 0;
	alien->bomb_timer = create_timer( BOMB_UPDATE_TIME );
	alien->alien_timer = create_timer( ALIEN_UPDATE_TIME );
	alien->body = "@";

	for( int i = 0; i < NUM_ALIENS; i++ )
	{
		alien->alive[i] = true;
		alien->last_bomb[i] = false;
	}

	for( int i = 0; i < MAX_NUM_BOMBS; i++ )
	{
		alien->bombs[i] = create_sprite( 0, 0, 1,  1, "." );
		alien->bombs[i]->is_visible = false;
	}

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

	for ( int i = 0; i < NUM_ALIENS; i++ )
	{
		alien->sprite[i]->dx = 0.2;
	}
	
	draw_aliens( alien );
}

void create_bomb( int bomb_num, Alien * alien )
{
	int chosen_one;
	
	do
	{
		chosen_one = rand() % ( NUM_ALIENS );
	} while ( alien->last_bomb[chosen_one] );
	
	for ( int i = 0; i < chosen_one; i++ )
	{
		if ( alien->last_bomb[i] )
		{
			alien->last_bomb[i] = false;
		}
	}

	for ( int the_one = chosen_one; the_one < NUM_ALIENS; ++the_one )
	{
		if ( alien->last_bomb[the_one] )
		{
			alien->last_bomb[the_one] = false;
		}
	}

	alien->bombs[bomb_num]->is_visible = 't';
	alien->bombs[bomb_num]->x = alien->sprite[chosen_one]->x;
	alien->bombs[bomb_num]->y = alien->sprite[chosen_one]->y;
	alien->bombs[bomb_num]->dy = 0.2;
	alien->last_bomb[chosen_one] = true;
}

void update_bombs ( Alien * alien )
{
	alien->active_bombs = 1;
	for ( int i = 0; i < MAX_NUM_BOMBS; i++ )
	{
		if (  alien->bombs[i]->is_visible == 't' )
		{
			alien->active_bombs++;
		}
	}

	if ( alien->numBombs < MAX_NUM_BOMBS && alien->numBombs < alien->active_bombs )
	{
		alien->numBombs++;
	}

	for ( int i = 0; i < alien->numBombs; i++ )
	{
		if ( !alien->bombs[i]->is_visible )
		{
			create_bomb( i, alien );
		}
	}
}

bool update_aliens(	Alien * alien )
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

		for ( int i = 0; i < NUM_ALIENS; i++ )
		{
			int new_x = (int) round( alien->sprite[i]->x + alien->sprite[i]->dx );

			if ( new_x >= width )
			{
				alien->sprite[i]->x = 0;
			}

			alien->sprite[i]->x += alien->sprite[i]->dx;
		}
		for ( int i = 0; i < MAX_NUM_BOMBS; i++ )
		{
			if ( alien->bombs[i]->is_visible == 't' )
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
