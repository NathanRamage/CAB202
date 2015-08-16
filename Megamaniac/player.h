#ifndef _PLAYER_H_
#define _PLAYER_H_

/* Standard libraries */
#include <stdbool.h>

/* External libraries */
#include "cab202_graphics.h"
#include "cab202_timers.h"
#include "game.h"
#include "alien.h"

void draw_player (int x );

int move_player ( int key );

void update_view( int x );

#endif
