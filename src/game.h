#ifndef GAME_H_
#define GAME_H_

#include "level.h"

#define XRES	640
#define YRES	480

#define TILE_XSZ	128
#define TILE_YSZ	64

extern unsigned long time_msec;		/* defined in main*.c */

extern struct level lvl;
extern struct font *fnt;
extern int showdbg;

/* special keys */
enum {
	KEY_BACKSP = 8,
	KEY_ESC = 27,
	KEY_DEL = 127,

	KEY_NUM_0 = 256, KEY_NUM_1, KEY_NUM_2, KEY_NUM_3, KEY_NUM_4,
	KEY_NUM_5, KEY_NUM_6, KEY_NUM_7, KEY_NUM_8, KEY_NUM_9,
	KEY_NUM_DOT, KEY_NUM_DIV, KEY_NUM_MUL, KEY_NUM_MINUS, KEY_NUM_PLUS, KEY_NUM_ENTER, KEY_NUM_EQUALS,
	KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT,
	KEY_INS, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
	KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
	KEY_F13, KEY_F14, KEY_F15,
	KEY_NUMLK, KEY_CAPSLK, KEY_SCRLK,
	KEY_RSHIFT, KEY_LSHIFT, KEY_RCTRL, KEY_LCTRL, KEY_RALT, KEY_LALT,
	KEY_RMETA, KEY_LMETA, KEY_LSUPER, KEY_RSUPER, KEY_MODE, KEY_COMPOSE,
	KEY_HELP, KEY_PRINT, KEY_SYSRQ, KEY_BREAK
};

#ifndef KEY_ANY
#define KEY_ANY		(-1)
#define KEY_ALT		(-2)
#define KEY_CTRL	(-3)
#define KEY_SHIFT	(-4)
#endif

enum {
	KEY_MOD_SHIFT	= 1,
	KEY_MOD_CTRL	= 4,
	KEY_MOD_ALT	= 8
};


int game_init(void);
void game_cleanup(void);

void game_draw(void);

void game_keyboard(int key, int press);
void game_mousebtn(int bn, int st, int x, int y);
void game_mousemove(int x, int y);

/* defined in main*.c */
void game_quit(void);
unsigned long game_getmsec(void);

#endif	/* GAME_H_ */
