#!/bin/bash

if [ -z "$1" -o -z "$2" -o -z "$3" ]
then 
	echo "First param: file, second param: dir, third param file extension!"
	exit 1
fi

if [ ! -f "$1" ]
then
	echo "File error" >&2
	exit 1
fi

if [ ! -d "$2" ]
then
	echo "Incorrect data" >&2
	exit 1
fi


find $2 -maxdepth 1 -name "*.$3" | sort -n > $1
