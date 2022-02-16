#!/bin/bash

 > ex5out.txt
while [ -n "$1" ]
do
	echo -n"$1 "
	echo -n "$1 " >> ex5out.txt
	shift
done
echo ""
