# Libraries
LIBS = -lpthread

# Flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os -fPIC ${CPPFLAGS}
LDFLAGS  = -shared ${LIBS}

# Compiler and linker
CC = cc
