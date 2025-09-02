include ../../config.mk

src = src/byteord.c src/conv.c src/filejpeg.c src/filelbm.c src/filepng.c \
	  src/fileppm.c src/filergbe.c src/filetga.c src/ftmodule.c src/imago2.c \
	  src/imago_gl.c src/modules.c src/quant.c \
	  jpeglib/jcapimin.c jpeglib/jcapistd.c jpeglib/jccoefct.c \
	  jpeglib/jccolor.c jpeglib/jcdctmgr.c jpeglib/jchuff.c jpeglib/jcinit.c \
	  jpeglib/jcmainct.c jpeglib/jcmarker.c jpeglib/jcmaster.c \
	  jpeglib/jcomapi.c jpeglib/jcparam.c jpeglib/jcphuff.c jpeglib/jcprepct.c \
	  jpeglib/jcsample.c jpeglib/jctrans.c jpeglib/jdapimin.c \
	  jpeglib/jdapistd.c jpeglib/jdatadst.c jpeglib/jdatasrc.c \
	  jpeglib/jdcoefct.c jpeglib/jdcolor.c jpeglib/jddctmgr.c jpeglib/jdhuff.c \
	  jpeglib/jdinput.c jpeglib/jdmainct.c jpeglib/jdmarker.c \
	  jpeglib/jdmaster.c jpeglib/jdmerge.c jpeglib/jdphuff.c \
	  jpeglib/jdpostct.c jpeglib/jdsample.c jpeglib/jdtrans.c jpeglib/jerror.c \
	  jpeglib/jfdctflt.c jpeglib/jfdctfst.c jpeglib/jfdctint.c \
	  jpeglib/jidctflt.c jpeglib/jidctfst.c jpeglib/jidctint.c \
	  jpeglib/jidctred.c jpeglib/jmemmgr.c jpeglib/jmemnobs.c \
	  jpeglib/jquant1.c jpeglib/jquant2.c jpeglib/jutils.c \
	  libpng/png.c libpng/pngerror.c libpng/pngget.c libpng/pngmem.c \
	  libpng/pngpread.c libpng/pngread.c libpng/pngrio.c libpng/pngrtran.c \
	  libpng/pngrutil.c libpng/pngset.c libpng/pngtrans.c libpng/pngwio.c \
	  libpng/pngwrite.c libpng/pngwtran.c libpng/pngwutil.c \
	  zlib/adler32.c zlib/compress.c zlib/crc32.c zlib/deflate.c zlib/gzio.c \
	  zlib/infback.c zlib/inffast.c zlib/inflate.c zlib/inftrees.c \
	  zlib/trees.c zlib/uncompr.c zlib/zutil.c
obj = $(src:.c=.o)
alib = ../$(outdir)/libimago.a

CFLAGS = $(arch) $(dbg) $(opt) -Izlib -Ilibpng -Ijpeglib

$(alib): $(obj)
	mkdir -p `dirname $(alib)`
	$(AR) rcs $@ $(obj)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(obj) $(alib)

# vi:ft=make:
