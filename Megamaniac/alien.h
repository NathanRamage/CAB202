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

/* definitions */
#define NUM_ALIENS (10)
#define MAX_NUM_BOMBS (4)
#define ALIEN_UPDATE_TIME (100)
#define BOMB_UPDATE_TIME (3000)

//sprite_id aliens[NUM_ALIENS];

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
	int active_bombs;
} Alien;

void setup_aliens( Alien * alien );

void draw_aliens( Alien * alien );

bool update_aliens( Alien * alien );

void update_bombs( Alien * alien );

void create_bomb( int bomb_num, Alien * alien );

void draw_bombs( Alien * alien );

#endif
