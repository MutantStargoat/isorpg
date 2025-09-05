#include <stdio.h>
#include "game.h"
#include "keyb.h"
#include "mouse.h"
#include "timer.h"
#include "options.h"
#include "logger.h"

unsigned long time_msec;
static unsigned long start_time;

static int quit;


int main(int argc, char **argv)
{
	unsigned long msec, nframes;

#ifdef __DJGPP__
	__djgpp_nearptr_enable();
#endif

	if(!have_mouse()) {
		fprintf(stderr, "No mouse detected. Make sure the mouse driver is installed\n");
		return 1;
	}

	log_start("game.log");
	init_timer(0);
	kb_init();

	load_options("game.cfg");

	if(game_init() == -1) {
		return 1;
	}

	nframes = 0;
	start_time = 0;	/* TODO */
	while(!quit) {
		time_msec = game_getmsec();
		nframes++;
	}

	msec = game_getmsec();
	printf("shutting down, avg fps: %.2f\n", (float)nframes / ((float)msec / 1000.0f));
	game_cleanup();
	return 0;
}

void game_quit(void)
{
	quit = 1;
}

unsigned long game_getmsec(void)
{
	return get_msec() - start_time;
}
