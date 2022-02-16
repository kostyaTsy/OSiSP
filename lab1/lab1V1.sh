#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Number of params should be 3">&2
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
