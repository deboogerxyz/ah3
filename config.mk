# libraries
LIBS = -lcjson -lGLEW -lm -lpthread -lSDL2

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L
#CFLAGS   = -g -std=c99 -pedantic -Wall -Wno-deprecated-declarations -O0 -fPIC ${CPPFLAGS} -fsanitize=undefined
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -O2 -fPIC ${CPPFLAGS}
LDFLAGS  = -shared ${LIBS}

# compiler and linker
CC = cc
