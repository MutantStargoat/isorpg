#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"
#include "vidsys.h"
#include "game.h"
#include "options.h"

struct gfxmode *gfx_modes, *gfx_curmode;
int gfx_num_modes;

struct gfximage *gfx_front, *gfx_back;
static struct vid_color pal[256];


int gfx_init(void)
{
	if(!(gfx_front = calloc(2, sizeof *gfx_front))) {
		fprintf(stderr, "failed to allocate memory\n");
		return -1;
	}
	gfx_back = gfx_front + 1;

	if(vid_init() == -1) {
		return -1;
	}
	return 0;
}

void gfx_destroy(void)
{
	vid_cleanup();
}

int gfx_findmode(int xsz, int ysz, int bpp, int rate)
{
	return vid_findmode(xsz, ysz, bpp);
}

int gfx_setmode(int modeidx)
{
	if(!(gfx_curmode = vid_setmode(modeidx))) {
		return -1;
	}
	return 0;
}

int gfx_setup(int xsz, int ysz, int bpp, unsigned int flags)
{
	int i;

	for(i=0; i<2; i++) {
		gfx_front[i].width = xsz;
		gfx_front[i].height = ysz;
		gfx_front[i].flags = i == 0 ? GFX_IMG_VIDMEM : 0;
	}
	gfx_front->bpp = bpp;

	if(bpp <= 8) {
		/* if colormaps are involved at all, initialize the default palette */
		for(i=0; i<256; i++) {
			unsigned char c = i & 0xe0;
			pal[i].r = c | (c >> 3) | (c >> 6);
			c = (i << 3) & 0xe0;
			pal[i].g = c | (c >> 3) | (c >> 6);
			c = (i << 5) & 0xc0;
			pal[i].b = c | (c >> 2) | (c >> 4) | (c >> 6);
		}
		vid_setpal(0, 256, pal);
	}

	return 0;
}

void gfx_setcolor(int idx, int r, int g, int b)
{
	pal[idx].r = r;
	pal[idx].g = g;
	pal[idx].b= b;

	vid_setpal(idx, 1, pal + idx);
}

void gfx_setcolors(int start, int count, struct gfxcolor *colors)
{
	int i, idx;
	for(i=0; i<count; i++) {
		idx = start + i;
		pal[idx].r = colors->r;
		pal[idx].g = colors->g;
		pal[idx].b = colors->b;
		colors++;
	}

	vid_setpal(start, count, pal + start);
}

int gfx_imginit(struct gfximage *img, int x, int y, int bpp)
{
	memset(img, 0, sizeof *img);
	img->width = x;
	img->height = y;
	img->bpp = bpp;
	img->ckey = -1;

	if(!(img->pixels = malloc(x * y * bpp / 8))) {
		fprintf(stderr, "failed to allocate image %dx%d %dbpp\n", x, y, bpp);
		return -1;
	}
	return 0;
}

void gfx_imgdestroy(struct gfximage *img)
{
	free(img->pixels);
}

void *gfx_imgstart(struct gfximage *img)
{
}

void gfx_imgend(struct gfximage *img)
{
}

void gfx_imgdebug(struct gfximage *img)
{
}

void gfx_fill(struct gfximage *img, unsigned int color, struct gfxrect *rect)
{
	/* TODO */
}

void gfx_imgkey(struct gfximage *img, int ckey)
{
	/* make RLE sprite */
}

void gfx_blit(struct gfximage *dest, int x, int y, struct gfximage *src, struct gfxrect *srect)
{
}

void gfx_blitkey(struct gfximage *dest, int x, int y, struct gfximage *src, struct gfxrect *srect)
{
}

void gfx_swapbuffers(int vsync)
{
}

void gfx_waitvsync(void)
{
}
