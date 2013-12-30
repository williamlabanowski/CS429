#!/bin/bash

source "tf.sh"

if [ "$SUMMARY" == "" ]
then
	SUMMARY=/dev/stdout
fi

for def in `ls $DEFDIR`
do
	deffile=$DEFDIR/$def
	for trace in `ls $TRACEDIR`
	do
		testfunc $def-$trace  "./$PROG $DEFDIR/$def $TRACEDIR/$trace" "$TMPDIR/$def-$trace" "$REFDIR/$def-$trace"
	done
done

let "SCORE=TOTALNUMBER-WA"

echo -e '\E[;32m'"\033[1m $SCORE OUT OF $TOTALNUMBER NUMBERS ARE CORRECT\033[0m" 

if [ "$SUMMARY" != "" ]
then
	echo -en $UTEID'\t' >> $SUMMARY
fi

echo "scale=2; 10*$SCORE/$TOTALNUMBER"| bc >> $SUMMARY

# vim: tw=0
