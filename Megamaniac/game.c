#include "game.h"

#define String char*

/* screen width and height to be used through the script */
int previous_width, previous_height;

/* Draw current level */
void setup_level( Game * game )
{
	game->lives = 3;
	game->score = 0;
	game->level = 1;
	game->harmonicMotion = DOWN;

	get_screen_size( previous_width, previous_height );

	game->level_text[0] = " - Basic";
	game->level_text[1] = " - Harmonic Motion";
	game->level_text[2] = " - Falling Motion";
	game->level_text[3] = " - Drunken Motion";
	game->level_text[4] = " - Aggressive Motion";

	int level1_HUD_height = screen_height() - 1;
	int level2_HUD_height = screen_height() - 2;
	int level3_HUD_height = screen_height() - 3;

	//int score_lives_len = snprintf( NULL, 0, "Score: 0   Lives: %d   ", game->lives );
	int level_text_len = snprintf( NULL, 0, "Level: %d%s", game->lives, game->level_text[game->level - 1] );
	int level_len = snprintf( NULL, 0, "Level: %d ", game->level );
	int left_score_lives = screen_width() - level_len - 11;
	int left_level = ( screen_width() / 2 ) - ( level_text_len / 2 );

	draw_formatted( left_level, level1_HUD_height, "Level: %d%s", game->level, game->level_text[game->level - 1] );
	draw_formatted( 0, level2_HUD_height, "Nathan Ramage (n8802530)");
	draw_string( left_score_lives, level2_HUD_height, "Score:");
	draw_int( left_score_lives + 7, level2_HUD_height, game->score );
	draw_formatted( screen_width() - level_len, level2_HUD_height, "Lives: %d", game->lives );
	draw_line( 0, level3_HUD_height, screen_width(), level3_HUD_height, '_');
}

/* draws the level */
void draw_level( Game * game )
{
	int level1_HUD_height = screen_height() - 1;
	int level2_HUD_height = screen_height() - 2;
	int level3_HUD_height = screen_height() - 3;

	//int score_lives_len = snprintf( NULL, 0, "Score: 0   Lives: %d   ", game->lives );
	int level_text_len = snprintf( NULL, 0, "Level: %d%s", game->lives, game->level_text[game->level - 1] );
	int level_len = snprintf( NULL, 0, "Level: %d ", game->level );
	int left_score_lives = screen_width() - level_len - 11;
	int left_level = ( screen_width() / 2 ) - ( level_text_len / 2 );

	draw_formatted( left_level, level1_HUD_height, "Level: %d%s", game->level, game->level_text[game->level - 1] );
	draw_formatted( 0, level2_HUD_height, "Nathan Ramage (n8802530)");
	draw_string( left_score_lives, level2_HUD_height, "Score:");
	draw_int( left_score_lives + 7, level2_HUD_height, game->score );
	draw_formatted( screen_width() - level_len, level2_HUD_height, "Lives: %d", game->lives );
	draw_line( 0, level3_HUD_height, screen_width(), level3_HUD_height, '_');
}

/* calls draw_game, draw_level, draw_bombs and draw_aliens */
void draw_game( Alien * alien, Game * game )
{
	clear_screen();
	draw_bombs( alien );
	draw_aliens( alien );
	draw_player( player );
	draw_level( game );
	show_screen();
}

/* calls cleanup_aliens, cleanup_player */
void cleanup_game( Alien * alien, Game * game )
{
	cleanup_player();
	cleanup_aliens( alien );
	setup_level( game );
	setup_player();
	setup_aliens( alien );
	show_screen();
}

/* checks which event that can change the screen has occured
   or the alien timer has expired */
int play_turn( Alien * alien, Game * game )
{
	int result = TURN_READY;
	int key = get_char();

	if ( resize_detected() )
	{
		cleanup_game( alien, game );
		return result;
	}

	if( check_player_died( game ) )
	{
		result = TURN_GAME_OVER;
	}

	if ( key == 'q' || key == 'Q')
	{
		result = GAME_FINISHED;
	}
	else if ( key == 'r' || key == 'R' )
	{
		cleanup_player();
		cleanup_aliens( alien );
		setup_player();
		setup_aliens( alien );
		draw_game( alien, game );
		result = TURN_READY;
	}
	else if ( key == 'l' || key == 'L' )
	{
		if ( game->level == 5 )
		{
			game->level = 1;
		}
		else
		{
			game->level++;
		}
		cleanup_player();
		cleanup_aliens( alien );
		setup_player();
		setup_aliens( alien );
		draw_game( alien, game );
		result = TURN_READY;
	}

	if ( update_player( key ) )
	{
		result = TURN_SCREEN_CHANGED;
	}
	else if ( update_sprites( alien, game ) )
	{
		result = TURN_SCREEN_CHANGED;
	}

	return result;
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
bool update_sprites( Alien * alien, Game * game )
{
	int dead_aliens = 0;
	for ( int i = 0; i < NUM_ALIENS; i++ )
	{
		if ( !alien->sprite[i]->is_visible )
		{
			dead_aliens++;
		}
	}

	if ( dead_aliens == NUM_ALIENS )
	{
		setup_aliens( alien );
		return true;
	}

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
		int height = screen_height() - 3;

		if ( game->harmonicMotion == WAIT_TOP || game->harmonicMotion == WAIT_BOTTOM )
		{

			if ( game->harmonicMotion == WAIT_BOTTOM )
			{
				for ( int i = 0; i < NUM_ALIENS; i++ )
				{
					alien->sprite[i]->x++;
					int current_x = (int) round( alien->sprite[i]->x );
					game->harmonicMotion = UP;

					if ( current_x > width - 1 )
					{
						alien->sprite[i]->x = 0;
						alien->sprite[i]->y -= alien->sprite[i]->dy;
					}
				}
			}
			else if ( game->harmonicMotion == WAIT_TOP )
			{
				for ( int i = 0; i < NUM_ALIENS; i++ )
				{
					alien->sprite[i]->x++;
					int current_x = (int) round( alien->sprite[i]->x );
					game->harmonicMotion = DOWN;

					if ( current_x > width - 1 )
					{
						alien->sprite[i]->x = 0;
						alien->sprite[i]->y -= alien->sprite[i]->dy;
					}
				}
			}
		}

		/* move all aliens by dx */
		for ( int i = 0; i < NUM_ALIENS; i++ )
		{
			int current_x = (int) round( alien->sprite[i]->x );
			int current_y = (int) round( alien->sprite[i]->y );

			if ( alien->alive[i] )
			{
				if( current_x == bullet->x && current_y == bullet->y && alien->sprite[i]->is_visible )
				{
					shot_bullet = false;
					alien->sprite[i]->is_visible = false;
					bullet->is_visible = false;
					bullet->x = 0;
					bullet->y = 0;
					if ( dead_aliens == NUM_ALIENS - 1 )
					{
						game->score += ALL_ALIENS_DEAD;
					}
					else
					{
						game->score += 30;
					}
					alien->alive[i] = false;
				}
				else
				{
					/* Level one movement */
					if ( game->level == 1 )
					{
						alien->sprite[i]->x += alien->sprite[i]->dx;
						int current_x = (int) alien->sprite[i]->x;
						if ( current_x >= width )
						{
							alien->sprite[i]->x = 0;
						}
					}
					/* level two movement */
					else if ( game->level == 2 )
					{
						if ( game->harmonicMotion == DOWN )
						{
							alien->sprite[i]->x += alien->sprite[i]->dx;
							alien->sprite[i]->y += alien->sprite[i]->dy;
							int current_x = (int) round( alien->sprite[i]->x );
							alien->alien_moved++;

							if ( current_x > width - 1 )
							{
								alien->sprite[i]->x = 0;
								alien->sprite[i]->y -= alien->sprite[i]->dy;
							}

							if ( alien->alien_moved == 250 )
							{
								game->harmonicMotion = WAIT_BOTTOM;
								alien->alien_moved = 0;
							}
						}
						else if ( game->harmonicMotion == UP )
						{
							alien->sprite[i]->x += alien->sprite[i]->dx;
							alien->sprite[i]->y -= alien->sprite[i]->dy;
							int current_x = (int) round( alien->sprite[i]->x );
							alien->alien_moved++;

							if ( current_x > width - 1 )
							{
								alien->sprite[i]->x = 0;
								alien->sprite[i]->y -= alien->sprite[i]->dy;
							}

							if ( alien->alien_moved == 250 )
							{
								game->harmonicMotion = WAIT_TOP;
								alien->alien_moved = 0;
							}
						}
					}
					/* level three movement */
					else if ( game->level == 3 )
					{
						level3_movement( &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height );

						int player_x = (int) round( player->x );
						int player_y = (int) round( player->y );

						if ( player_x == current_x && player_y == current_y )
						{
							game->lives--;
							reset_player( game->level, (int) round( alien->sprite[6]->x ), (int) round( alien->sprite[3]->x ) );
							alien->sprite[i]->is_visible = false;
						}
					}

						int player_x = (int) round( player->x );
						int player_y = (int) round( player->y );

						if ( player_x == current_x && player_y == current_y )
						{
							game->lives--;
							reset_player( game->level, (int) round( alien->sprite[6]->x ), (int) round( alien->sprite[3]->x ) );
							alien->sprite[i]->is_visible = false;
						}
					}
			}
		}


		/* level five movement */
		if ( game->level == 5 )
		{
			if ( !alien->elite_alien )
			{
				int chosen_one = rand() % 10;
				alien->sprite[chosen_one]->dx += 1;

				for ( int i = 0; i < NUM_ALIENS; i++ )
				{
					alien->sprite[i]->x += alien->sprite[i]->dx;
					alien->sprite[i]->y += alien->sprite[i]->dy;
				}
			}
		}

		/* level 4 movement */
		if ( game->level == 4 )
		{
			int width = screen_width();
			int height = screen_height() - 3;
			level4_movement( alien->sprite, width, height, alien->level4_timer, &alien->expand, alien->level4_update );

			for ( int i = 0; i < NUM_ALIENS; i++ )
			{
				int current_y = (int) round( alien->sprite[i]->y );

				if ( current_y >= height )
				{
					alien->sprite[i]->y = 0;
				}
			}
		}

		/* move bombs */
		for ( int i = 0; i < MAX_NUM_BOMBS; i++ )
		{
			if ( alien->bombs[i]->is_visible == TRUE )
			{
				int new_y = (int) round( alien->bombs[i]->y + alien->bombs[i]->dy );
				int current_x = (int) round( alien->bombs[i]->x );
				if ( new_y >= screen_height() - 3 )
				{
					alien->bombs[i]->is_visible = false;
					alien->bombs[i]->x = 0;
					alien->bombs[i]->y = 0;
				}
				else if ( new_y == player->y && current_x == player->x )
				{
					game->lives--;
					reset_player( game->level, (int) round( alien->sprite[6]->x ), (int) round( alien->sprite[3]->x ) );
					for ( int i = 0; i < MAX_NUM_BOMBS; i++ )
					{
						alien->bombs[i]->is_visible = false;
						alien->bombs[i]->x = 0;
						alien->bombs[i]->y = 0;
					}
					i = MAX_NUM_BOMBS;
				}
				else
				{
					alien->bombs[i]->y += alien->bombs[i]->dy;
				}
			}
		}

		if ( shot_bullet )
		{
			int new_y = (int) round( bullet->y + bullet->dy );
			if ( new_y <= 0 )
			{
				bullet->is_visible = false;
				bullet->x = 0;
				bullet->y = 0;
				shot_bullet = false;
			}
			else
			{
				bullet->y -= bullet->dy;
			}
		}
		return true;
	}
}

/* checks if the player has died */
bool check_player_died ( Game * game )
{
	if ( game->lives <= 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* check if the screen size has chnaged */
bool resize_detected( void )
{
	int new_width = screen_width();
	int new_height = screen_height();

	bool resize = previous_width != new_width || previous_height != new_height;

	return resize;
}

/* level 3 movement */
void level3_movement ( double * alien_x, double * alien_y, double * dx, double * dy, int width, int height )
{
	int new_x = (int) round( *alien_x + *dx );
	int new_y = (int) round( *alien_y + *dy );
	if ( new_x > width )
	{
		*alien_x = 0;
		*alien_y = round( *alien_y );
	}
	else if ( new_y > height )
	{
		*alien_y = 0;
		*alien_x = round( *alien_x );
	}
	else
	{
		*alien_x += *dx;
		*alien_y += *dy;
	}
}

/* level 4 movement */
void level4_movement ( sprite_id aliens[NUM_ALIENS], int width, int height, timer_id level4_timer, bool * expand, timer_id level4_update )
{
	if ( check_level4_timer( level4_timer ) )
	{
		if ( *expand )
		{
			int upperBound;
			int lowerBound;

			// start 3 position alien
			upperBound = ( width - (int) round( aliens[2]->x ) - 1 );
			lowerBound = ( (int) round( aliens[2]->x - aliens[1]->x ) );

			int r = randomNumber( upperBound, lowerBound );

			aliens[2]->dx = ( r / 10.0 ) + 3;

			aliens[2]->x += aliens[2]->dx;
			aliens[2]->y += aliens[2]->dy;

			int new_x = (int) round( aliens[2]->x );
			int new_y = (int) round( aliens[2]->y );

			if ( new_x >= width - 1 )
			{
				aliens[2]->x = width - 1;
			}

			if ( new_y >= height )
			{
				aliens[2]->y = 0;
			}
			//end 3 position alien

			//start 7 postion alien
			upperBound = ( width - (int) round( aliens[6]->x ) - 1 );
			lowerBound = ( (int) round( aliens[6]->x - aliens[5]->x ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[6]->dx = ( r / 10.0 ) + 3;

			aliens[6]->x += aliens[6]->dx;
			aliens[6]->y += aliens[6]->dy;

			new_x = (int) round( aliens[6]->x );
			new_y = (int) round( aliens[6]->y );

			if ( new_x >= width - 1 )
			{
				aliens[6]->x = width - 1;
			}

			if ( new_y >= height )
			{
				aliens[6]->y = 0;
			}
			//end 7 position alien

			//start 10 position alien
			upperBound = ( width - (int) round( aliens[9]->x ) - 1 );
			lowerBound = ( (int) round( aliens[9]->x - aliens[8]->x ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[9]->dx = ( r / 10.0 ) + 3;

			aliens[9]->x += aliens[9]->dx;
			aliens[9]->y += aliens[9]->dy;

			new_x = (int) round( aliens[9]->x );
			new_y = (int) round( aliens[9]->y );

			if ( new_x >= width - 1 )
			{
				aliens[9]->x = width - 1;
			}

			if ( new_y >= height )
			{
				aliens[9]->y = 0;
			}
			//end 10 position alien

			//start 1 position alien
			upperBound = ( (int) round( aliens[1]->x - aliens[0]->x - 1 ) );
			lowerBound = ( (int) round( aliens[0]->x ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[0]->dx = ( r / 10.0 ) - 3;

			aliens[0]->x += aliens[0]->dx;
			aliens[0]->y += aliens[0]->dy;

			new_x = (int) round( aliens[0]->x );
			new_y = (int) round( aliens[0]->y );

			if ( new_x <= 0)
			{
				aliens[0]->x = 0;
			}

			if ( new_y >= height )
			{
				aliens[0]->y = 0;
			}
			//end 1 position alien

			//start 4 position alien
			upperBound = ( (int) round( aliens[4]->x - aliens[3]->x - 1 ) );
			lowerBound = ( (int) round( aliens[3]->x ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[3]->dx = ( r / 10.0 ) - 3;

			aliens[3]->x += aliens[3]->dx;
			aliens[3]->y += aliens[3]->dy;

			new_x = (int) round( aliens[3]->x );
			new_y = (int) round( aliens[3]->y );

			if ( new_x <= 0)
			{
				aliens[3]->x = 0;
			}

			if ( new_y >= height )
			{
				aliens[3]->y = 0;
			}
			//end 4 position alien

			//start 8 position alien
			upperBound = ( (int) round( aliens[8]->x - aliens[7]->x - 1 ) );
			lowerBound = ( (int) round( aliens[7]->x ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[7]->dx = ( r / 10.0 ) - 3;

			aliens[7]->x += aliens[7]->dx;
			aliens[7]->y += aliens[7]->dy;

			new_x = (int) round( aliens[7]->x );
			new_y = (int) round( aliens[7]->y );

			if ( new_x <= 0)
			{
				aliens[7]->x = 0;
			}

			if ( new_y >= height )
			{
				aliens[7]->y = 0;
			}
			//end 8 position

			//start position 2 alien
			upperBound = 3;
			lowerBound = -3;

			r = rand() % 2;

			if ( r == 1 )
			{
				aliens[1]->dx = upperBound;
			}
			else
			{
				aliens[1]->dx = lowerBound;
			}

			aliens[1]->x += aliens[1]->dx;
			aliens[1]->y += aliens[1]->dy;

			new_x = (int) round( aliens[1]->x );
			new_y = (int) round( aliens[1]->y );

			if ( new_x >= (int) round( aliens[2]->x - 2 ) )
			{
				aliens[1]->x = aliens[2]->x - 2;
			}
			else if ( new_x <= (int) round( aliens[0]->x + 2 ) )
			{
				aliens[1]->x = aliens[0]->x + 2;
			}

			if ( new_y >= height )
			{
				aliens[1]->y = 0;
			}
			//end position 2 alien

			//start position 9 alien
			upperBound = 3;
			lowerBound = -3;

			r = rand() % 2;

			if ( r == 1 )
			{
				aliens[8]->dx = upperBound;
			}
			else
			{
				aliens[8]->dx = lowerBound;
			}

			aliens[8]->x += aliens[8]->dx;
			aliens[8]->y += aliens[8]->dy;

			new_x = (int) round( aliens[8]->x );
			new_y = (int) round( aliens[8]->y );

			if ( new_x >= (int) round( aliens[9]->x - 2 ) )
			{
				aliens[8]->x = aliens[9]->x - 2;
			}
			else if ( new_x <= (int) round( aliens[7]->x + 2 ) )
			{
				aliens[8]->x = aliens[9]->x + 2;
			}

			if ( new_y >= height )
			{
				aliens[8]->y = 0;
			}
			//end position 9 alien

			//start 6 postion alien
			upperBound = ( (int) round( aliens[6]->x - aliens[5]->x - 1 ) );
			lowerBound = ( (int) round( aliens[5]->x - ( width / 2 ) + 3 ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[5]->dx = ( r / 10.0 ) + 1;

			aliens[5]->x += aliens[5]->dx;
			aliens[5]->y += aliens[5]->dy;

			new_x = (int) round( aliens[5]->x );
			new_y = (int) round( aliens[5]->y );

			if ( new_x >= aliens[6]->x - 2 )
			{
				aliens[5]->x = aliens[6]->x - 2;
			}

			if ( new_y >= height )
			{
				aliens[5]->y = 0;
			}
			//end 6 position alien

			//start 5 position alien
			upperBound = ( (int) round( ( width / 2 ) - aliens[4]->x - 1 ) );
			lowerBound = ( (int) round( aliens[4]->x - aliens[3]->x + 1 ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[4]->dx = ( r / 10.0 ) - 1.5;

			aliens[4]->x += aliens[4]->dx;
			aliens[4]->y += aliens[4]->dy;

			new_x = (int) round( aliens[4]->x );
			new_y = (int) round( aliens[4]->y );

			if ( new_x <= aliens[3]->x + 2)
			{
				aliens[4]->x = aliens[3]->x + 2;
			}

			if ( new_y >= height )
			{
				aliens[4]->y = 0;
			}
			//end 5 position alien
			*expand = false;
		}
		else
		{
			int upperBound;
			int lowerBound;
			// 3, 7 and 10 position aliens
			upperBound = ( (int) round( aliens[2]->x ) );
			lowerBound = ( (int) round( aliens[2]->x - aliens[1]->x + 2 ) );

			int r = randomNumber( upperBound, lowerBound );

			aliens[2]->dx = ( r / 10.0 ) - 3;

			aliens[2]->x += aliens[2]->dx;
			aliens[2]->y += aliens[2]->dy;

			int new_x = (int) round( aliens[2]->x );
			int new_y = (int) round( aliens[2]->y );

			if ( new_x <= (int) round( aliens[1]->x ) + 2 )
			{
				aliens[2]->x = aliens[1]->x + 2;
			}

			if ( new_y >= height )
			{
				aliens[2]->y = 0;
			}
			//end position 3 alien

			//start position 7 alien
			upperBound = ( (int) round( aliens[6]->x ) );
			lowerBound = ( (int) round( aliens[6]->x - aliens[5]->x + 2 ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[6]->dx = ( r / 10.0 ) - 1.5;

			aliens[6]->x += aliens[6]->dx;
			aliens[6]->y += aliens[6]->dy;

			new_x = (int) round( aliens[6]->x );
			new_y = (int) round( aliens[6]->y );

			if ( new_x <= (int) round( aliens[5]->x ) + 2 )
			{
				aliens[6]->x = aliens[5]->x + 2;
			}

			if ( new_y >= height )
			{
				aliens[6]->y = 0;
			}
			//end position 7 alien

			//start position 10 alien
			upperBound = ( (int) round( aliens[9]->x ) );
			lowerBound = ( (int) round( aliens[9]->x - aliens[8]->x + 2 ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[9]->dx = ( r / 10.0 ) - 1.5;

			aliens[9]->x += aliens[9]->dx;
			aliens[9]->y += aliens[9]->dy;

			new_x = (int) round( aliens[9]->x );
			new_y = (int) round( aliens[9]->y );

			if ( new_x <= (int) round( aliens[8]->x ) + 2 )
			{
				aliens[9]->x = aliens[8]->x + 2;
			}

			if ( new_y >= height )
			{
				aliens[9]->y = 0;
			}
			//end positoin 10 alien

			//start 1 position alien
			upperBound = ( (int) round( aliens[1]->x - 2 - aliens[0]->x ) );
			lowerBound = ( (int) round( aliens[0]->x ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[0]->dx = ( r / 10.0 ) + 3;

			aliens[0]->x += aliens[0]->dx;
			aliens[0]->y += aliens[0]->dy;

			new_x = (int) round( aliens[0]->x );
			new_y = (int) round( aliens[0]->y );

			if ( new_x >= (int) round( aliens[1]->x - 2 ) )
			{
				aliens[0]->x = aliens[1]->x - 2;
			}

			if ( new_y >= height )
			{
				aliens[0]->y = 0;
			}
			//end position 1 alien

			//start position 4 alien
			upperBound = ( (int) round( aliens[4]->x - 2 - aliens[3]->x ) );
			lowerBound = ( (int) round( aliens[3]->x ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[3]->dx = ( r / 10.0 ) + 1.5;

			aliens[3]->x += aliens[3]->dx;
			aliens[3]->y += aliens[3]->dy;

			new_x = (int) round( aliens[3]->x );
			new_y = (int) round( aliens[3]->y );

			if ( new_x >= (int) round( aliens[4]->x - 2 ) )
			{
				aliens[3]->x = aliens[4]->x - 2;
			}

			if ( new_y >= height )
			{
				aliens[3]->y = 0;
			}
			//end position 4 alien

			//start position 8 alien
			upperBound = ( (int) round( aliens[8]->x - 2 - aliens[7]->x ) );
			lowerBound = ( (int) round( aliens[7]->x ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[7]->dx = ( r / 10.0 ) + 3;

			aliens[7]->x += aliens[7]->dx;
			aliens[7]->y += aliens[7]->dy;

			new_x = (int) round( aliens[7]->x );
			new_y = (int) round( aliens[7]->y );

			if ( new_x >= (int) round( aliens[8]->x - 2 ) )
			{
				aliens[7]->x = aliens[8]->x - 2;
			}

			if ( new_y >= height )
			{
				aliens[7]->y = 0;
			}
			//end position 8 alien

			//start position 2 alien
			upperBound = -3;
			lowerBound = 3;

			r = rand() % 2;

			if ( r == 1 )
			{
				aliens[1]->dx = upperBound;
			}
			else
			{
				aliens[1]->dx = lowerBound;
			}

			aliens[1]->x += aliens[1]->dx;
			aliens[1]->y += aliens[1]->dy;

			new_x = (int) round( aliens[1]->x );
			new_y = (int) round( aliens[1]->y );

			if ( new_x >= (int) round( aliens[2]->x - 2 ) )
			{
				aliens[1]->x = aliens[2]->x - 2;
			}
			else if ( new_x <= (int) round( aliens[0]->x + 2 ) )
			{
				aliens[1]->x = aliens[0]->x + 2;
			}

			if ( new_y >= height )
			{
				aliens[1]->y = 0;
			}
			//end position 2 alien

			//start position 9 alien
			upperBound = 3;
			lowerBound = -3;

			r = rand() % 2;

			if ( r == 1 )
			{
				aliens[8]->dx = upperBound;
			}
			else
			{
				aliens[8]->dx = lowerBound;
			}

			aliens[8]->x += aliens[8]->dx;
			aliens[8]->y += aliens[8]->dy;

			new_x = (int) round( aliens[8]->x );
			new_y = (int) round( aliens[8]->y );

			if ( new_x >= (int) round( aliens[9]->x - 2 ) )
			{
				aliens[8]->x = aliens[9]->x - 2;
			}
			else if ( new_x <= (int) round( aliens[7]->x + 2 ) )
			{
				aliens[8]->x = aliens[7]->x + 2;
			}

			if ( new_y >= height )
			{
				aliens[8]->y = 0;
			}
			//end position 9 alien

			//start position 6 alien
			upperBound = ( (int) round( aliens[6]->x - aliens[5]->x - 1 ) );
			lowerBound = ( (int) round( aliens[5]->x - ( width / 2 ) + 3 ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[5]->dx = ( r / 10.0 ) - 3;

			aliens[5]->x += aliens[5]->dx;
			aliens[5]->y += aliens[5]->dy;

			new_x = (int) round( aliens[5]->x );
			new_y = (int) round( aliens[5]->y );

			if ( new_x <= (int) round( ( width / 2 ) + 3 ) )
			{
				aliens[5]->x = ( width / 2 ) + 3;
			}

			if ( new_y >= height )
			{
				aliens[5]->y = 0;
			}
			//end position 6 alien

			//start position 5 alien
			upperBound = ( (int) round( ( width / 2 ) - 3 - aliens[4]->x ) );
			lowerBound = ( (int) round( aliens[4]->x - aliens[3]->x + 1 ) );

			r = randomNumber( upperBound, lowerBound );

			aliens[4]->dx = ( r / 10.0 ) + 3;

			aliens[4]->x += aliens[4]->dx;
			aliens[4]->y += aliens[4]->dy;

			new_x = (int) round( aliens[4]->x );
			new_y = (int) round( aliens[4]->y );

			if ( new_x >= (int) round( aliens[5]->x - 2 ) )
			{
				aliens[4]->x = aliens[5]->x - 2;
			}

			if ( new_y >= height )
			{
				aliens[4]->y = 0;
			}//end position 5 alien
		*expand = true;
		}
	}
	else
	{
		if ( timer_expired( level4_update ) )
		{
			for ( int i = 0; i <  NUM_ALIENS; i++ )
			{
				if ( i == 0)
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x <= 0 )
					{
						aliens[i]->x = 0;
					}
					else if ( new_x >= aliens[1]->x - 2)
					{
						aliens[i]->x = aliens[1]->x - 2;
					}
				}
				else if ( i == 3)
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x <= 0 )
					{
						aliens[i]->x = 0;
					}
					else if ( new_x >= aliens[4]->x - 2)
					{
						aliens[i]->x = aliens[4]->x - 2;
					}
				}
				else if ( i == 4)
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x <= aliens[3]->x + 2 )
					{
						aliens[i]->x = aliens[3]->x + 2;
					}
					else if ( new_x >= ( width / 2 ) - 3)
					{
						aliens[i]->x = ( width / 2 ) - 3;
					}
				}
				else if ( i == 5)
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x <= ( width / 2 ) + 3 )
					{
						aliens[i]->x = ( width / 2 ) + 3;
					}
					else if ( new_x >= aliens[6]->x - 2)
					{
						aliens[i]->x = aliens[6]->x - 2;
					}
				}
				else if ( i == 7)
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x <= 0 )
					{
						aliens[i]->x = 0;
					}
					else if ( new_x >= aliens[8]->x - 2)
					{
						aliens[i]->x = aliens[8]->x - 2;
					}
				}
				else if ( i == 2)
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x >= width )
					{
						aliens[i]->x = width - 1;
					}
					else if ( new_x <= aliens[1]->x + 2)
					{
						aliens[i]->x = aliens[1]->x + 2;
					}
				}
				else if ( i == 6)
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x >= width - 1 )
					{
						aliens[i]->x = width - 1;
					}
					else if ( new_x <= aliens[5]->x + 2)
					{
						aliens[i]->x = aliens[5]->x + 2;
					}
				}
				else if ( i == 9)
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x >= width - 1 )
					{
						aliens[i]->x = width - 1;
					}
					else if ( new_x <= aliens[8]->x + 2)
					{
						aliens[i]->x = aliens[8]->x + 2;
					}
				}
				else if ( i == 6 )
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x >= width - 1 )
					{
						aliens[i]->x = width - 1;
					}
					else if ( new_x <= aliens[5]->x + 2)
					{
						aliens[i]->x = aliens[5]->x + 2;
					}
				}
				else if ( i == 2 )
				{
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x >= width - 1 )
					{
						aliens[i]->x = width - 1;
					}
					else if ( new_x <= aliens[1]->x + 2)
					{
						aliens[i]->x = aliens[1]->x + 2;
					}
				}
				else if ( i == 1)
				{
					int	r  = rand() % 2;

					if ( r == 1 )
					{
						aliens[i]->dx = 3;
					}
					else
					{
						aliens[i]->dx = -3;
					}
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x >= (int) round( aliens[2]->x - 2 ) )
					{
						aliens[i]->x = aliens[2]->x - 2;
					}
					else if ( new_x <= (int) round( aliens[0]->x + 2 ) )
					{
						aliens[i]->x = aliens[0]->x + 2;
					}
				}
				else if ( i == 8 )
				{
					int	r  = rand() % 2;

					if ( r == 1 )
					{
						aliens[i]->dx = 3;
					}
					else
					{
						aliens[i]->dx = -3;
					}
					aliens[i]->x += aliens[i]->dx;
					aliens[i]->y += aliens[i]->dy;

					int new_x = (int) round( aliens[i]->x );

					if ( new_x >= (int) round( aliens[9]->x - 2 ) )
					{
						aliens[i]->x = aliens[9]->x - 2;
					}
					else if ( new_x <= (int) round( aliens[7]->x + 2 ) )
					{
						aliens[i]->x = aliens[7]->x + 2;
					}
				}
				else
				{
					aliens[i]->y += aliens[i]->dy;
				}
			}
		}
	}
}

bool check_level4_timer ( timer_id level4_timer )
{
	if( timer_expired( level4_timer ))
	{
		free ( level4_timer );
	 	int r = ( rand() % 9 ) + 1;
		r *= 100;
		level4_timer = create_timer( r );
		return true;
	}
	else
	{
		return false;
	}
}

int randomNumber ( int upperBound, int lowerBound)
{
	return ( rand() % ( upperBound + lowerBound ) ) - lowerBound;
}
