!ifdef __UNIX__
gameobj = src/dynarr.obj src/font.obj src/game.obj src/gfx.obj src/level.obj &
	src/leveled.obj src/logger.obj src/options.obj src/rend.obj src/screen.obj &
	src/tiles.obj src/treestor.obj src/ts_text.obj src/util.obj
dosobj = src/dos/maindos.obj src/dos/gfxdos.obj src/dos/vidsys.obj &
	src/dos/drv_vga.obj src/dos/drv_vbe.obj src/dos/drv_s3.obj src/dos/cdpmi.obj &
	src/dos/keyb.obj src/dos/timer.obj src/dos/mouse.obj src/dos/mtrr.obj &
	src/dos/mtrr_s.obj

incpath = -Isrc -Ilibs/imago/src
libpath = libpath libs/dos
libs = libs/dos/imago.lib

!else

gameobj = src\dynarr.obj src\font.obj src\game.obj src\gfx.obj src\level.obj &
	src\leveled.obj src\logger.obj src\options.obj src\rend.obj src\screen.obj &
	src\tiles.obj src\treestor.obj src\ts_text.obj src\util.obj
dosobj = src\dos\maindos.obj src\dos\gfxdos.obj src\dos\vidsys.obj &
	src\dos\drv_vga.obj src\dos\drv_vbe.obj src\dos\drv_s3.obj src\dos\cdpmi.obj &
	src\dos\keyb.obj src\dos\timer.obj src\dos\mouse.obj src\dos\mtrr.obj &
	src\dos\mtrr_s.obj

incpath = -Isrc -Ilibs\imago\src
libpath = libpath libs\dos
libs = libs\dos\imago.lib
!endif

obj = $(gameobj) $(dosobj)
bin = game.exe

opt = -otexan -4 -fp3
dbg = -d3

AS = nasm
CC = wcc386
LD = wlink

CFLAGS = $(dbg) $(opt) $(def) -s -zq -bt=dos $(incpath)
LDFLAGS = option map $(libpath) library { $(libs) }

$(bin): cflags.occ $(obj) $(libs)
	%write objects.lnk $(obj)
	%write ldflags.lnk $(LDFLAGS)
	$(LD) debug all name $@ system dos4g file { @objects } @ldflags

.c: src;src/dos
.asm: src/dos

cflags.occ: Makefile.wat
	%write $@ $(CFLAGS)

.c.obj: .autodepend
	$(CC) -fo=$@ @cflags.occ $[*

.asm.obj:
	nasm -f obj -o $@ $[*.asm

!ifdef __UNIX__
clean: .symbolic
	rm -f $(obj)
	rm -f $(bin)
	rm -f cflags.occ *.lnk

imago.lib:
	cd libs/imago
	wmake -f Makefile.wat
	cd ../..

cleanlibs: .symbolic
	cd libs/imago
	wmake -f Makefile.wat clean

!else
clean: .symbolic
	rm -f $(obj)
	rm -f $(bin)
	rm -f cflags.occ *.lnk

imago.lib:
	cd libs\imago
	wmake -f Makefile.wat
	cd ..\..

cleanlibs: .symbolic
	cd libs\imago
	wmake -f Makefile.wat clean

!endif


# vi:ft=make:
