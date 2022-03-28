include config.mk

SRC = ah.c bt.c hk.c intf.c mem.c nv.c sdk.c util.c pch.c
HDR = bt.h cvector.h hk.h intf.h mem.h nv.h sdk.h util.h nuklear.h nuklear_sdl_gl3.h pch.h

OBJ = ${SRC:.c=.o}

all: options libah.so

options:
	@echo ah build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "CC       = ${CC}"
	@echo "LDFLAGS  = ${LDFLAGS}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk Makefile ${HDR}

libah.so: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f libah.so ${OBJ} ah.tar.gz

dist: clean
	mkdir -p ah
	cp -R config.mk Makefile LICENSE \
	LICENSE.cvector README.md ${SRC} \
	${HDR} ah-load.sh ah-unload.sh ah
	tar -cf ah.tar ah
	gzip ah.tar
	rm -rf ah

.PHONY: all options clean dist
