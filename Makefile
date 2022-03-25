include config.mk

SRC = ah.c intf.c mem.c util.c
OBJ = ${SRC:.c=.o}

all: options libah.so

options:
	@echo ah build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "CC       = ${CC}"
	@echo "LDFLAGS  = ${LDFLAGS}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk Makefile intf.h mem.h util.h

libah.so: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f libah.so ${OBJ}

.PHONY: all options clean
