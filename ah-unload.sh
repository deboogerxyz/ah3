#!/bin/sh

pid=$(pgrep "csgo_linux64")
lib="$(pwd)/libah.so"

if [ $(id -u) -ne 0 ];
then
	echo "ah-unload requires root privileges"
	exit 1
fi

if [ -z "$pid" ]
then
	echo "Couldn't find the target process"
	exit 1
fi

if ! grep -q "$lib" "/proc/$pid/maps";
then
	echo "Library is not loaded"
	exit 1
fi


gdb -n -q -batch \
    -ex "attach $pid" \
    -ex "set \$dlopen = (void *(*)(char *, int))dlopen" \
    -ex "set \$dlclose = (int (*)(void *))dlclose" \
    -ex "set \$lib = \$dlopen(\"$lib\", 6)" \
    -ex "call \$dlclose(\$lib)" \
    -ex "call \$dlclose(\$lib)" \
    -ex "detach" \
    -ex "quit"
