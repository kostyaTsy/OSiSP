#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Number of params should be 3">&2
	echo "Command format: min size, max size, directory"
	exit 1
fi

# checking for correct dir
if [ ! -d "$3" ]
then
	echo "Incorect argument 3, not the direcrory">&2
	exit 1
fi 

# cheking for a number
if (echo "$1" | grep -E -q "[^0-9]+$")
then
	echo "First argument is not a number">&2
	exit 1
fi

if (echo "$2" | grep -E -q "[^0-9]+$")
then
	echo "Second argument is not a number">&2
	exit 1
fi

if [ "$1" -ge "$2" ]
then
	echo "First param should be less than second">&2
	exit 1
fi

# generating array of files
files=$(find $3 -maxdepth 1 -type f -size +$1c -size -$2c)

# creating empty file
> tmpFile.txt
for file in $files
do
	echo $(wc -c $file) >> tmpFile.txt
done

# sorting file
sort -n tmpFile.txt
rm tmpFile.txt
