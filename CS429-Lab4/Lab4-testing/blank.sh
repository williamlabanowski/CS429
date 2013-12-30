#!/bin/bash

rm -rf cases && mkdir cases
ANON=0

for i in `ls cases.in/*.obj`
do
	base="${i%.*}"
	name="${base#cases.in/}"
	for j in "in" log obj out
	do
		if [ -f $base.$j ]
		then
			if [ $j == "log" ]
			then
				sed "s/(NOP)/()/" < $base.$j | sed -r "s/\s+/ /g" | tr "[:lower:]" "[:upper:]" > cases/$ANON.$j
			else
				cp $base.$j cases/$ANON.$j
			fi
		fi
	done
	let "ANON=ANON+1"
done

chmod -w cases/*
