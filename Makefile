include config.mk

SRC = ah.c \
      hax/bt.c \
      hk.c \
      intf.c \
      mem.c \
      nv.c \
      sdk.c \
      util.c \
      pch.c

HDR = hax/bt.h \
      deps/cvector.h \
      hk.h \
      intf.h \
      mem.h \
      nv.h \
      sdk.h \
      util.h \
      deps/nuklear.h \
      deps/nuklear_sdl_gl3.h \
      pch.h

OBJ = ${SRC:.c=.o}

all: options libah.so

options:
	@echo ah build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "CC       = ${CC}"
	@echo "LDFLAGS  = ${LDFLAGS}"

.c.o:
	${CC} -o $@ -c ${CFLAGS} $<

${OBJ}: config.mk Makefile ${HDR}

libah.so: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f libah.so ${OBJ} ah.tar.gz

.PHONY: all options clean
