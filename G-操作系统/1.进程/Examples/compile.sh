#!/bin/sh

if [ ! $1 ]
then
    echo "no input file!"
    exit 0
fi

#gcc $1 -I ././../../A.lib/*h -L . ././../../A.lib/*.c

gcc $1 -I ../../A.lib/*.h -L . ../../A.lib/*.c
