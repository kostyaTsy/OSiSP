#!/bin/bash

if [ $# -ne 3 ]
then 
	echo "First param: file, second param: dir, third param file extension!"
	exit 1
fi

if [ ! -f "$1" ]
then
	echo "File doesn't exist, enter correct file name" >&2
	exit 1
fi

if [ ! -d "$2" ]
then
	echo "Incorrect directory, enter correct directory" >&2
	exit 1
fi


find $2 -maxdepth 1 -name "*.$3" | sort -n > $1
