#ifndef _GAME_H_
#define _GAME_H_

/* Standard libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* External libraries */
#include "cab202_graphics.h"
#include "alien.h"
#include "player.h"

/* Definitions */
#define String char*
#define TURN_READY (-1)
#define TURN_GAME_OVER (0)
#define TURN_SCREEN_CHANGED (1)
#define GAME_FINISHED (-2)
#define RESTART (-3)
#define DEAD_ALIEN (30)
#define ALL_ALIENS_DEAD (500)
#define UP (-4)
#define DOWN (-5)
#define WAIT_BOTTOM (-6)
#define WAIT_TOP (-7)

typedef struct Game
{
	int score;
	int lives;
	int level;
	String level_text[5];
	int harmonicMotion;
} Game;

/* sets the default values for properties in Game */
void setup_level( Game * game );

/* checks which event that can change the screen has occured
   or the alien timer has expired */
int play_turn( Alien * alien, Game * game );

/* calls draw_game, draw_level, draw_bombs and draw_aliens */
void draw_game( Alien * alien, Game * game );

/* draws the level */
void draw_level( Game * game );

/* calls cleanup_aliens, cleanup_player */
void cleanup_game ( Alien * alien, Game * game );

/* two cases make this return true, either the alien timer expires or the bomb time expires */
bool update_sprites( Alien * alien, Game * game );

/* move the bombs and destory bombs that have hit the ground or player */
void update_bombs( Alien * alien );

/* checks if the player has died */
bool check_player_died( Game * game );

bool resize_detected( void );

/* level 3 movement */
void level3_movement( double * alien_x, double * alien_y, double * dx, double * dy, int width, int height );

/* level 4 movement */
void level4_movement ( sprite_id aliens[NUM_ALIENS], double * alien_x, double * alien_y, double * dx, double * dy, int width, int height, int infront, int middle, int behind, int alien_num );

#endif
