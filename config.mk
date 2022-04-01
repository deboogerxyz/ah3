# Libraries
LIBS = -lcjson -lGL -lGLEW -lm -lpthread -lSDL2

# Flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -O2 -fPIC ${CPPFLAGS} -O0 -g
LDFLAGS  = -shared ${LIBS}

# Compiler and linker
CC = cc
