#!/bin/bash

source "tf.sh"

SCORE="1"

for obj in `ls $CASEDIR/*.obj`
do
	base="${obj%.*}"
	corename="${base#$CASEDIR/}"
	TMPOUT=$TMPDIR/$corename.out
	TMPLOG=$TMPDIR/$corename.log
	REFOUT=$base.out
	REFLOG=$base.log
	if [ -f $base.in ]
	then
		INPUT="<$base.in"
	else
		INPUT=''
	fi

	testfunc $corename "./$PROG -v $base.obj $INPUT" "$TMPOUT" "$TMPLOG" "$REFOUT" "$REFLOG"
done

for obj in `ls $UNDEFDIR/*.obj`
do
	base="${obj%.*}"
	corename="${base#$CASEDIR/}"
	testfunc $corename "./$PROG -v $obj" "" "" "" ""
done


echo -en $UTEID'\t' >> $SUMMARY

echo "scale=2; $SCORE"| bc >> $SUMMARY

# vim: tw=0
