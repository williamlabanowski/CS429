#!/bin/bash

CURRENT=`pwd`
SOURCE="$1"
SUMMARY="$2"
UTEID="$3"
TITLE=Program5
PROG=cachesim
TMPDIR=tmpdir
CASENUMBER=12
TOTALNUMBER=864
WA=0

DEFDIR=def
TRACEDIR=trace
REFDIR=nref

function testcore {
	cmdline=$1
	out=$2
	nrefout=$3

	bash -c "timeout -s 9 15s $cmdline > $out 2>/dev/null"
	RET=$?

	if [ $RET == 139 ]
	then
		SEGF=1
		FAILED=1
		return 
	elif [ $RET == 137 ]
	then
		SEGF=1
		FAILED=1
		TLE=1
		return 
	fi

	tail -n +2 $out | grep -i -v "entries" | sed -e 's/\s\+/\n/g' -|egrep -v "[^0-9]"|sed '/^$/d' - > $out.normalized
	RET=`sdiff $out.normalized $nrefout | grep "|" | wc -l`

	if [ $RET != 0 ]
	then 
		ANSFAILED=1
		let "WA+=RET"
	fi
}

function testfunc {
	ANSFAILED=0
	VERBFAILED=0
	SEGF=0
	TLE=0
	echo -n "Testing $1""..."
	testcore "${@:2}"
	if [ $ANSFAILED -eq 0 ]
	then
		echo -e '...\E[;32m'"\033[1mPASS\033[0m"
	elif [ $TLE -ne 0 ]
	then
		echo -e '...\E[;31m'"\033[1m\"$1\"TIME LIMIT EXCEEDED\033[0m"
		let 'WA+=12'
	elif [ $SEGF -ne 0 ]
	then
		echo -e '...\E[;31m'"\033[1m\"$1\"SEGFAULT\033[0m"
		let 'WA+=12'
	else
		echo -e '...\E[;31m'"\033[1m\"$1\"(PARTIALLY) WRONG OUTPUT\033[0m"
	fi
}

if [ ! -f $PROG ]
then
	cd $SOURCE
	rm *.o $PROG
	make $PROG
	cp $PROG $CURRENT
	cd $CURRENT
fi

if [ ! -f $PROG ]
then
	echo "[$TITLE] Could not build $PROG at $SOURCE. Please check your source code or Makefile";
	exit
fi

if [ ! -d $TMPDIR ]
then
	mkdir $TMPDIR 
fi

rm -f $TMPDIR/*

# vim: tw=0
