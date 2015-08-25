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

	if ( key == 'q' || key == 'Q' )
	{
		result = GAME_FINISHED;
	}
	else if ( key == 'r' || key == 'R' )
	{
		cleanup_game( alien, game );
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
		//int height = screen_height();

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
						if ( current_x > width )
						{
							alien->sprite[i]->x = 0;
						}
					}
					/* level two movement */
					else if ( game->level == 2 )
					{
					}
					/* level three movement */

					/* level four movement */

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
					reset_player();
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
