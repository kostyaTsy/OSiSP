#!/bin/bash

if gcc $1 -o $2
then
	./$2
else
	echo "Compile error">&2
fi
