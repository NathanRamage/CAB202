#include "game.h"

#define String char*

/* Draw current level */
void draw_level (int level, int lives, int score)
{
	String current_level = "Level: ";
	String level_desc = " - Basic";
	String current_score = "Score: ";
	String current_lives = "Lives: ";
	int lives_len = strlen( current_lives );

	draw_string( ( screen_width() / 2 ) - 9, screen_height() - 1, current_level );
	draw_int( screen_width() / 2 - 2, screen_height() - 1, level );
	draw_string( screen_width() / 2 - 1, screen_height() - 1, level_desc );
	

	draw_string( 0, screen_height() - 2, "Nathan Ramage (n8802530)" );
	draw_string( screen_width() - 20, screen_height() - 2, current_score );
	draw_int( screen_width() - lives_len - 6, screen_height() - 2, score );
	draw_string( screen_width() - 9, screen_height() - 2, current_lives );
	draw_int( screen_width() - 2, screen_height() - 2, lives );
	draw_line( 0, screen_height() - 3, screen_width(), screen_height() - 3, '_' );
}
