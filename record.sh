#!/bin/bash
make
echo --------testing small--------
/usr/bin/time ./um ./umbin/midmark.um > /dev/null
echo --------testing large--------
/usr/bin/time ./um ./umbin/sandmark.umz > /dev/null
echo --------testing instr--------
valgrind --tool=callgrind --dump-instr=yes ./um ./umbin/midmark.um