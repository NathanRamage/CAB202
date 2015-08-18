#ifndef _GAME_H_
#define _GAME_H_

/* Standard libraries */
#include <stdio.h>
#include <string.h>

/* External libraries */
#include "cab202_graphics.h"
#include "cab202_timers.h"
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

void setup_level( Game * game );

int play_turn( Alien * alien, Game * game );

void draw_game( Alien * alien, Game * game );

void draw_level( Game * game );

void cleanup_game ( Alien * alien, Game * game );

#endif
