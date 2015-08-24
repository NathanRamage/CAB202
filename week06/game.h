/* standard libraries */
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/* external libraries */
#include "cab202_sprites.h"
#include "cab202_graphics.h"
#include "cab202_timers.h"

#define N_ALIENS (10)

typedef struct Game 
{
	bool over;
	int score;
	int event_loop_delay;
	
	sprite_id ship;
	
	sprite_id bullet;
	timer_id  bullet_timer;
	
	sprite_id aliens[N_ALIENS];
	timer_id alien_timer;
} Game;

void setup_game( Game * game );

void draw_game( Game * game );

void setup_ship( Game * game );

bool update_ship ( Game * game, int key_code );

void draw_ship( Game * game );

void cleanup_ship( Game * game );

void launch_bullet( Game * game );

bool inside_rect( int x, int y, int left, int top, int width, int height );

bool move_bullet( Game * game );

bool update_bullet( Game * game, int key_code );

void draw_bullet( Game * game );

void cleanup_bullet( Game * game );

void setup_aliens( Game * game, int rock_x[], int rock_y[] );

void draw_aliens( Game * game );

bool move_aliens( sprite_id alien );

bool update_aliens( Game * game );

void cleanup_aliens( Game * game );

int rand_between( int first, int last );

void setup_all( Game * game );

void draw_all( Game * game );

void cleanup_all( Game * game );

void event_loop( void );
