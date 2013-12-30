#!/bin/bash

source "tf.sh"

for i in `ls $CASEDIR/*.obj`
do
	base="${i%.*}"
	number="${base#$CASEDIR/}"
	if [ -f $base.in ]
	then
		INPUT="<$base.in"
	else
		INPUT=''
	fi
	if [ -f $base.log ]
	then
		VERBOSEFLAG="-v"
		LOGFILE=$base.log
	else
		VERBOSEFLAG=""
		LOGFILE=""
	fi
	testfunc $base "./$PROG $VERBOSEFLAG $base.obj $INPUT" "$base.out" "$LOGFILE" "diff -bwi"
done

echo -e '\E[;32m'"\033[1m $TESTPASS OUT OF $TESTCASE TEST(S) PASSED\033[0m" 

let "SCORE=10-WA-WV"

exit $SCORE

# vim: tw=0
