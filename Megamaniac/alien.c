/* External libraries */
#include "alien.h"

/* draw alien */
void draw_alien( struct Aliens *alien )
{
	/* draw aliens */
	draw_char( alien->x, alien->y, alien->body );
}

/* move aliens one step to the left after 0.5 sec */
void move_aliens()
{
	declare_aliens( 1 );
}

/* declare aliens */
void declare_aliens( int dx )
{
	struct Aliens alien1, alien2, alien3, alien4, alien5, alien6, alien7, alien8, alien9, alien10;
	
	spawn_aliens( &alien1, &alien2, &alien3, &alien4, &alien5, &alien6, &alien7, &alien8, &alien9, &alien10, dx );
}

void spawn_aliens( 
	struct Aliens *alien1, struct Aliens *alien2, struct Aliens *alien3, struct Aliens *alien4, struct Aliens *alien5,
	struct Aliens *alien6, struct Aliens *alien7, struct Aliens *alien8, struct Aliens *alien9, struct Aliens *alien10,
	int dx )
{

	if ( dx == 0 )
	{	
		/* alien 1 specification */
		alien1->x = 3;
		alien1->y = 1;
		alien1->alive = true;
		alien1->body = '@';

		/* alien 2 specification */
		alien2->x = 9;
		alien2->y = 1;
		alien2->alive = true;
		alien2->body = '@';

		/* alien 3 specification */
		alien3->x = 15;
		alien3->y = 1;
		alien3->alive = true;
		alien3->body = '@';

		/* alien 4 specification */
		alien4->x = 0;
		alien4->y = 3;
		alien4->alive = true;
		alien4->body = '@';
	
		/* alien 5 specification */
		alien5->x = 6;
		alien5->y = 3;
		alien5->alive = true;
		alien5->body = '@';

		/* alien 6 specification */
		alien6->x = 12;
		alien6->y = 3;
		alien6->alive = true;
		alien6->body = '@';

		/* alien 7 specification */
		alien7->x = 18;
		alien7->y = 3;
		alien7->alive = true;
		alien7->body = '@';

		/* alien 8 specification */
		alien8->x = 3;
		alien8->y = 5;
		alien8->alive = true;
		alien8->body = '@';

		/* alien 9 specification */
		alien9->x = 9;
		alien9->y = 5;
		alien9->alive = true;
		alien9->body = '@';

		/* alien 10 specification */
		alien10->x = 15;
		alien10->y = 5;
		alien10->alive = true;
		alien10->body = '@';
	}
	/* increment every aliens positon by dx */
	else
	{
		alien1->x += dx;
		++alien2->x;
		++alien3->x;
		++alien4->x;
		++alien5->x;
		++alien6->x;
		++alien7->x;
		++alien8->x;
		++alien9->x;
		++alien10->x;
	}

	/* draw aliens */
	draw_alien( alien1 );
	draw_alien( alien2 );
	draw_alien( alien3 );
	draw_alien( alien4 );
	draw_alien( alien5 );
	draw_alien( alien6 );
	draw_alien( alien7 );
	draw_alien( alien8 );
	draw_alien( alien9 );
	draw_alien( alien10 );
}
