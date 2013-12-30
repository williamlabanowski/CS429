#
# Makefile that builds btest and other helper programs for the CS:APP data lab
# 
CC = gcc
CFLAGS = -O -Wall -m32
LIBS = -lm

all: btest fshow ishow

btest: btest.c bits.c decl.c tests.c btest.h bits.h
	$(CC) $(CFLAGS) $(LIBS) -o btest bits.c btest.c decl.c tests.c

dlc: dlc
	./dlc -e bits.c

fshow: fshow.c
	$(CC) $(CFLAGS) -o fshow fshow.c

ishow: ishow.c
	$(CC) $(CFLAGS) -o ishow ishow.c

grade: driver.pl
	./driver.pl

# Forces a recompile. Used by the driver program. 
btestexplicit:
	$(CC) $(CFLAGS) $(LIBS) -o btest bits.c btest.c decl.c tests.c 

clean:
	rm -f *.o btest fshow ishow *~

submit:
	turnin --submit zxy lab1 bits.c

verify:
	turnin --verify zxy lab1
