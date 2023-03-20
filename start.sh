#!/bin/sh

# cat Makefile > tempOriginal

SOURCES=$(cat Makefile | grep -n 'C_SOURCES =' | grep -Po '^[^:]*')
INCLUDES=$(cat Makefile | grep -n 'C_INCLUDES =' | grep -Po '^[^:]*')
I=1

while [ 0 -lt 1 ]
do
    LIB=$(ls lib | grep -Po '\w+' | sed "${I}q;d")
    I=$((I+1))

    if [ "$LIB" = "" ]; then break; fi

    IS_INCLUDED=0
    J=$INCLUDES
    while [ 0 -lt 1 ]
    do
        J=$((J+1))
        LINE=$(cat Makefile | sed "${J}q;d")

        if [ "$(echo $LINE | grep -o '^..')" != "-I" ]; then
            break
        fi

        if [ "$(echo $LINE | grep -Po '^-I\K[^\s]*')" == "lib/$LIB" ]; then
            IS_INCLUDED=1
            break
        fi
    done

    if [ $IS_INCLUDED -eq 1 ]; then continue; fi

    echo "Including $LIB"

    J=1
    > temp
    MAX_LINES=$(cat Makefile | wc -l)
    while [ $J -ne $((MAX_LINES+1)) ]
    do
        cat Makefile | sed "${J}q;d" >> temp
        if [ $J -eq $SOURCES ]; then echo "lib/$LIB/$LIB.c \\" >> temp; fi
        if [ $J -eq $INCLUDES ]; then echo "-Ilib/$LIB \\" >> temp; fi
        J=$((J+1))
    done
    INCLUDES=$((INCLUDES+1))

    cat temp > Makefile
    rm temp
done

NAME=$(pwd | grep -o "[^\/]*$")
if make; then
    st-flash --reset write build/$NAME.bin 0x08000000
fi

# cat tempOriginal > Makefile
