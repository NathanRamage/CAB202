#include <cab202_graphics.h>

void draw_border( void )
{
	draw_line((screen_width() - screen_width()), (screen_height() - screen_height() + 2), (screen_width() - 1), (screen_height() - screen_height() + 2), '&');
	draw_line((screen_width() - 1), (screen_height() - screen_height() + 2), (screen_width() - 1), (screen_height() - 2), '&');
	draw_line((screen_width() - 1), (screen_height() - 2), (screen_width() - screen_width()), (screen_height() - 2), '&');
	draw_line((screen_width() - screen_width()), (screen_height() - screen_height() + 2), (screen_width() - screen_width())), (screen_height() - 2), '&');
	
	show_screen();
}

int main( void )
{
	setup_screen();
	draw_border();
	wait_char();
	cleanup_screen();
	return 0;
}
