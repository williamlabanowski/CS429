#!/bin/bash

CURRENT=`pwd`
SOURCE="$1"
SUMMARY="$2"
UTEID="$3"
TITLE=Program7
PROG=pdp429
TMPDIR=tmpdir
TOTALNUMBER=0
GRADETOOL=parser/parser

CASEDIR=cases
UNDEFDIR=undefined

function testcore {
	cmdline=$1
	outfile=$2
	logfile=$3
	refout=$4
	reflog=$5

	#echo $cmdline
	if [ "$logfile" != "" ]
	then
		bash -c "timeout -s 9 15s $cmdline >$outfile 2>$logfile"
	else
		bash -c "timeout -s 9 15s $cmdline </dev/null >/dev/null 2>/dev/null"
	fi
	RET=$?

	if [ $RET == 139 ]
	then
		SEGF=1
		return 
	elif [ $RET == 137 ]
	then
		TLE=1
		return 
	elif [ "$logfile" == "" ]
	then # Corner case
		CALC="$CALC + 0.5"
		return
	fi
	
	if [ -s $refout ]
	then
		cmp $refout $outfile >$outfile.cmp 2>$outfile.error 
		RET=$?
		if [ $RET == 0 ]
		then
			CALC="$CALC + 0.5"
		else
			WOUT=1
		fi
		logweight="1.5"
	else # Lightweight case
		logweight="0.5"
	fi

	#echo "./$GRADETOOL $reflog $logfile"
	$GRADETOOL $reflog $logfile >$logfile.report 2>$logfile.error
	if [ $RET == 139 ]
	then 
		PARSERCRASH=1
		return
	fi
	gradeinfo=`grep "Grade:" $logfile.report`
	#echo $gradeinfo
	grade="${gradeinfo#Grade:}"
	#echo $grade
	CALC="$CALC + $logweight*($grade)"
}

function testfunc {
	ANSFAILED=0
	VERBFAILED=0
	SEGF=0
	TLE=0
	PARSERCRASH=0
	CALC="0"
	echo -n "Testing $1""..."
	testcore "${@:2}"
	if [ $TLE -ne 0 ]
	then
		echo -e '...\E[;31m'"\033[1mTIME LIMIT EXCEEDED\033[0m"
	elif [ $SEGF -ne 0 ]
	then
		echo -e '...\E[;31m'"\033[1mSEGFAULT\033[0m"
	else
		if [ $PARSERCRASH -ne 0 ]
		then
			echo -e '...\E[;31m'"\033[1mPARSER CRASHED, THIS SHOULD NOT HAPPEN\033[0m"
		fi
		#echo $CALC
		thisscore=`echo "scale=7; $CALC"| bc | awk '{printf "%f", $0}'`
		echo -e '...\E[;32m'"\033[1m$thisscore\033[0m"
		SCORE="$SCORE + ($CALC)"
	fi
}

if [ "$SUMMARY" == "" ]
then
	SUMMARY=/dev/stdout
fi

if [ ! -f $PROG ]
then
	cd $SOURCE
	rm -f *.o $PROG
	make $PROG
	cp $PROG $CURRENT
	cd $CURRENT
fi

if [ ! -f $PROG ]
then
	echo "[$TITLE] Could not build $PROG at $SOURCE. Please check your source code or Makefile";
	echo -e $UTEID'\t'0 >> $SUMMARY
	exit
else
	echo -n "Compiled""..."
	echo -e '...\E[;32m'"\033[1m1\033[0m"
fi

if [ ! -d $TMPDIR ]
then
	mkdir $TMPDIR 
fi

rm -f $TMPDIR/*

# vim: tw=0
