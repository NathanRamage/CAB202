#ifndef _ALIEN_H_
#define _ALIEN_H_

/* Standard libraries */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* External libraries */
#include "cab202_graphics.h"
#include "cab202_timers.h"
#include "cab202_sprites.h"
#include "player.h"

/* definitions */
#define NUM_ALIENS (10)
#define MAX_NUM_BOMBS (4)
#define ALIEN_UPDATE_TIME (100)
#define BOMB_UPDATE_TIME (3000)
#define TRUE 't'

typedef struct Alien
{
	timer_id alien_timer;
	sprite_id sprite[NUM_ALIENS];
	bool alive[NUM_ALIENS];
	char* body;
	int numBombs;
	sprite_id bombs[MAX_NUM_BOMBS];
	timer_id bomb_timer;
	bool last_bomb[NUM_ALIENS];
	int possible_bombs;
	int alien_moved;
} Alien;

/* defines deault values for alien struct and draws them in their initial positions */
void setup_aliens( Alien * alien );

/* draws aliens */
void draw_aliens( Alien * alien );

/* creates a new bomb */
void create_bomb( int bomb_num, Alien * alien );

/* draws bombs */
void draw_bombs( Alien * alien );

/* frees memory allocated for the alien structure */
void cleanup_aliens( Alien * alien );

#endif
