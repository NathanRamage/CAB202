#ifndef _GAME_H_
#define _GAME_H_

/* Standard libraries */
#include <stdio.h>
#include <string.h>

/* External libraries */
#include "cab202_graphics.h"
#include "player.h"
#include "alien.h"

/* Definitions */
#define String char*
#define TURN_READY (5)
#define TURN_GAME_OVER (0)
#define TURN_SCREEN_CHANGED (1)

typedef struct Game
{
	int score;
	int lives;
	int level;
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

#endif
