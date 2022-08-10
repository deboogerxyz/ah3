include config.mk

SRC = $(shell find -name "*.c")
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

libah3.so: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

-include ${DEP}

.c.o:
	$(CC) -MM -MT $@ -MF $(patsubst %.o, %.d, $@) ${CFLAGS} $<
	${CC} -o $@ -c ${CFLAGS} $<

${OBJ}: config.mk Makefile ${HDR}

clean:
	rm -f libah3.so ${OBJ} ${DEP}

.PHONY: clean
