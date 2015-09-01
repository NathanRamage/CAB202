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

					/* level four movement */
					else if ( game->level == 4 )
					{
						switch ( i )
						{
							case 0:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, -5, alien->sprite[0]->x, -5, 1 );
								break;
							case 1:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, alien->sprite[2]->x, alien->sprite[1]->x, alien->sprite[0]->x, 2 );
								break;
							case 2:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, -5, alien->sprite[2]->x, alien->sprite[1]->x, 3 );
								break;
							case 3:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, alien->sprite[4]->x, alien->sprite[3]->x, -5, 4 );
								break;
							case 4:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, alien->sprite[5]->x, alien->sprite[4]->x, alien->sprite[3]->x, 5 );
								break;
							case 5:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, alien->sprite[6]->x, alien->sprite[5]->x, alien->sprite[4]->x, 6 );
								break;
							case 6:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, -5, alien->sprite[6]->x, alien->sprite[5]->x, 7 );
								break;
							case 7:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, alien->sprite[8]->x, alien->sprite[7]->x, -5, 8 );
								break;
							case 8:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, alien->sprite[9]->x, alien->sprite[8]->x, alien->sprite[7]->x, 9 );
								break;
							case 9:
								level4_movement( alien->sprite, &alien->sprite[i]->x, &alien->sprite[i]->y, &alien->sprite[i]->dx, &alien->sprite[i]->dy, width, height, -5, alien->sprite[9]->x, alien->sprite[8]->x, 10);
								break;
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

					/* level five movement */
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
void level4_movement ( sprite_id aliens[NUM_ALIENS], double * alien_x, double * alien_y, double * dx, double * dy, int width, int height, int infront, int middle, int behind, int alien_num )
{
	if ( alien_num  == 3 || alien_num == 7 || alien_num == 10 )
	{
		bool ok = false;

		while ( !ok )
		{
			int upperBound = width - 1 - middle;
			int lowerBound = ( middle - behind - 1 );
			int r = rand() % ( upperBound + lowerBound ) - lowerBound;

			if ( r == 0 )
			{
				*dx = 0;
			}
			else if ( middle < width - 10 && middle < width - 30 )
			{
				*dx = ( r / 10.0 ) + 3;
			}
			else
			{
				*dx = ( r / 10.0 );
			}

			*alien_x += *dx;
			*alien_y += *dy;

			switch ( alien_num )
			{
				case 3:
					if ( *alien_x > aliens[1]->x + 1 )
					{
						ok = true;
					}
					break;
				case 7:
					if ( *alien_x > aliens[5]->x + 1 )
					{
						ok = true;
					}
					break;
				case 10:
					if ( *alien_x > aliens[8]->x + 1 )
					{
						ok = true;
					}
					break;
			}

			int new_y = (int) round( *alien_y );

			if ( new_y >= height )
			{
				*alien_y = 0;
				*alien_x = round( *alien_x );
			}
		}
	}
	else if ( alien_num == 1 || alien_num == 8 || alien_num == 4 )
	{
		bool ok = false;

		while ( ! ok )
		{
			int upperBound = ( infront - behind - 1 );
			int lowerBound = middle;
			int r = rand() % ( upperBound + lowerBound ) - lowerBound;

			if ( middle <= 2 )
			{
				*dx = 0;
			}
			else if ( middle > 10 && middle < 20 )
			{
				*dx = ( r / 10.0 ) - ( r / 3 );
			}
			else
			{
				*dx = ( r / 10.0 );
			}

			*alien_x += *dx;
			*alien_y += *dy;

			switch ( alien_num )
			{
				case 1:
					if ( *alien_x < aliens[1]->x - 1 )
					{
						ok = true;
					}
					break;
				case 4:
					if ( *alien_x < aliens[4]->x - 1 )
					{
						ok = true;
					}
					break;
				case 8:
					if ( *alien_x < aliens[8]->x - 1 )
					{
						ok = true;
					}
					break;
			}

			int new_y = (int) round( *alien_y );

			if ( new_y >= height )
			{
				*alien_y = 0;
				*alien_x = round( *alien_x );
			}
		}
	}
	else
	{
		bool ok = false;

		while ( !ok )
		{
			int upperBound = ( infront - middle - 1 );
			int lowerBound = ( middle - behind - 1 );
			int r = rand() % ( upperBound + lowerBound ) - lowerBound;

			*dx = ( r / 10.0 );

			*alien_x += *dx;
			*alien_y += *dy;

			switch ( alien_num )
			{
				case 2:
					if ( *alien_x < ( aliens[2]->x - 1 ) && ( *alien_x > aliens[0]->x + 1 ) )
					{
						ok = true;
					}
					break;
				case 5:
					if ( *alien_x < ( aliens[5]->x - 1 ) && ( *alien_x > aliens[3]->x + 1 ) )
					{
						ok = true;
					}
					break;
				case 6:
					if ( *alien_x < ( aliens[6]->x - 1 ) && ( *alien_x > aliens[4]->x + 1 ) )
					{
						ok = true;
					}
					break;
				case 9:
					if ( *alien_x < ( aliens[9]->x - 1 ) && ( *alien_x > aliens[7]->x + 1 ) )
					{
						ok = true;
					}
					break;
			}
		}
	}
}
