#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"
#include "vidsys.h"
#include "game.h"
#include "options.h"

struct gfxmode *gfx_modes, *gfx_curmode;
int gfx_num_modes;
static struct gfxmode curmode;

struct gfximage *gfx_front, *gfx_back;
static struct vid_color pal[256];

static void *vidmem;


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

int gfx_setmode(int modenum)
{
	struct vid_modeinfo *minf = vid_modeinfo(modenum);
	if(!minf || !(vidmem = vid_setmode(modenum))) {
		gfx_curmode = 0;
		return -1;
	}

	gfx_curmode = &curmode;
	memset(&curmode, 0, sizeof curmode);
	curmode.modeno = modenum;
	curmode.width = minf->width;
	curmode.height = minf->height;
	curmode.bpp = minf->bpp;
	curmode.pitch = minf->pitch;
	curmode.rmask = minf->rmask;
	curmode.gmask = minf->gmask;
	curmode.bmask = minf->bmask;
	curmode.rshift = minf->rshift;
	curmode.gshift = minf->gshift;
	curmode.bshift = minf->bshift;
	return 0;
}

int gfx_setup(int xsz, int ysz, int bpp, unsigned int flags)
{
	int i;

	if(xsz != curmode.width || ysz != curmode.height || bpp != curmode.bpp) {
		fprintf(stderr, "gfx_setup parameters don't match current video mode\n");
		return -1;
	}

	memset(gfx_front, 0, 2 * sizeof *gfx_front);
	for(i=0; i<2; i++) {
		gfx_front[i].width = curmode.width;
		gfx_front[i].height = curmode.height;
		gfx_front[i].bpp = curmode.bpp;
		if(curmode.bpp > 8) {
			gfx_front[i].rmask = curmode.rmask;
			gfx_front[i].gmask = curmode.gmask;
			gfx_front[i].bmask = curmode.bmask;
			gfx_front[i].rshift = curmode.rshift;
			gfx_front[i].gshift = curmode.gshift;
			gfx_front[i].bshift = curmode.bshift;
		} else {
			gfx_front[i].ncolors = 1 << curmode.bpp;
		}
	}

	gfx_front->pitch = curmode.pitch;
	gfx_front->flags = GFX_IMG_VIDMEM;
	gfx_front->pixels = vidmem;

	gfx_back->pitch = xsz * bpp / 8;
	if(!(gfx_back->pixels = malloc(ysz * gfx_back->pitch))) {
		fprintf(stderr, "failed to allocate back buffer\n");
		return -1;
	}

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
	/* TODO make RLE sprite */
	img->ckey = ckey;
}

void gfx_blit(struct gfximage *dest, int x, int y, struct gfximage *src, struct gfxrect *srect)
{
	int i, j, sx, sy, width, height;
	unsigned char *dptr, *sptr;

	sx = srect->x;
	sy = srect->y;
	width = srect->width;
	height = srect->height;

	if(y < 0) {
		sy -= y;
		height += y;
		y = 0;
	}
	if(x < 0) {
		sx -= x;
		width += x;
		x = 0;
	}

	if(x + width > dest->width) {
		width = dest->width - x;
	}
	if(y + height > dest->height) {
		height = dest->height - y;
	}
	if(width <= 0 || height <= 0) return;

	sptr = (unsigned char*)src->pixels + sy * src->pitch + sx;
	dptr = (unsigned char*)dest->pixels + y * dest->pitch + x;
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			dptr[j] = sptr[j];
		}
		dptr += dest->pitch;
		sptr += src->pitch;
	}
}

void gfx_blitkey(struct gfximage *dest, int x, int y, struct gfximage *src, struct gfxrect *srect)
{
	int i, j, sx, sy, width, height;
	unsigned char *dptr, *sptr;

	sx = srect->x;
	sy = srect->y;
	width = srect->width;
	height = srect->height;

	if(y < 0) {
		sy -= y;
		height += y;
		y = 0;
	}
	if(x < 0) {
		sx -= x;
		width += x;
		x = 0;
	}

	if(x + width > dest->width) {
		width = dest->width - x;
	}
	if(y + height > dest->height) {
		height = dest->height - y;
	}
	if(width <= 0 || height <= 0) return;

	sptr = (unsigned char*)src->pixels + sy * src->pitch + sx;
	dptr = (unsigned char*)dest->pixels + y * dest->pitch + x;
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			unsigned char pix = sptr[j];
			if(pix != src->ckey) {
				dptr[j] = sptr[j];
			}
		}
		dptr += dest->pitch;
		sptr += src->pitch;
	}
}

void gfx_swapbuffers(int vsync)
{
	if(vsync) {
		vid_vsync();
	}
	vid_blitfb(gfx_back->pixels, gfx_back->pitch);
}

void gfx_waitvsync(void)
{
	vid_vsync();
}
