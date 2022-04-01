include config.mk
include files.mk

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
pch.o:  pch.h ${PCH}

libah.so: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f libah.so ${OBJ} ah.tar.gz

.PHONY: all options clean
