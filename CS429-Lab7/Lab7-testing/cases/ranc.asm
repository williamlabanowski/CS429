		orig 0x100

//
// A program to generate random numbers and run C()
//

//
// print a string.  Address of string is in A register
//
ps0:      0
prints:   sta  ps0		// store pointer, for indirect addressing
ps1:	  lda  i ps0
		  sza rss
		  ret           // return on zero (end of string)
		  iota 4,0      // print character
		  isz  ps0
		  jmp ps1
		      

// print contents of A as a decimal number
// uses a (input), b (10), c (digit)
parray:	0
		0
		0
		0
		0
		0
pradd:	parray
prptr:	parray
m10:	10
czero:	'0'
		
printn:	ldb  pradd
		stb  prptr    // set up pointer
		ldb  m10      // base 10 printing
pr2:	mod  c,a,b    // get low-order digit (c = a mod 10)
		addc czero    // convert to character code
		stc  i prptr  // save digit
		isz  prptr    // increment pointer
		div  a,a,b    // divide a/10 -> a
		sza 
		jmp pr2       // non-zero means print char array

pr3:	lda prptr
		dca            // --prptr;
		sta prptr
		ldc i prptr    // load the digit
		iotc 4,0       // print the digit
		suba pradd
		sma sza
		jmp pr3
		ret


//
//   read a decimal number from input and return the value
//   if there is a number, skip; if EOF, do not skip
//   uses a (running total), b (10), c (input char), d (comparison)
//
rra:   0
readn: pop   rra      // get return address

	   cla            // a holds running sum
	   ldb m10        // b holds 10

rn1:   iotc   3,0     // read character
	   smc rss
	   jmp i rra      // EOF causes exit with A=0

	   subc czero     // c - '0'
	   smc rss
	   jmp rn1        // bad input, skip and keep reading
	   sub d,c,b      // (c - '0') < 10?
	   smd
	   jmp rn1        // still bad input,

//  we have skipped all the non-digit input before the number
//  start reading the number
	   cla 
rn2:   mul a,a,b      // a = a*10
	   add a,a,c      // a = a*10 + c - '0'
	   iotc   3,0     // read character
	   subc   czero   // c - '0'
	   smc rss
	   jmp rn3        // bad input, done with number
	   sub d,c,b      // (c - '0') < 10?
	   smd rss
	   jmp rn2        // loop back for another character

rn3:   isz rra
	   jmp i rra

///////////////////////////////////		
r_seed:  0x1781
r_a:     0xE64D
r_c:     0x1829
r_mod:   32749          // prime number
r_and:   0x7FFF
random:  
		 lda  r_seed    // x(n)
		 mula r_a       // x*a
		 adda r_c       // x*a + c
		 ldb  r_mod
		 mod  a,a,b     // (x*a + c) mod prime
		 anda r_and
		 sta  r_seed    // -> x(n+1)
		 ret


///////////////////////////////////		
//   Main program.
//
//   1. Read a seed number.
//   2. Generate that many random numbers.
//   3. Take the last random number and run C()
//   4. print the recursive values generated by C()
//

stsize:	  40
pFFF:     0xFFF
input_n:   0
loop_n:    0
count:     0

main:   cla    	dca     // a = 0xFFFF
		and 	sp,a,a  // move a to sp
		suba 	stsize
		and 	spl,a,a // move a minus stack size to spl
		
		call	readn              // read a number
		jmp		endinput           // if EOF

		//  call random number generator n times
		sta		 input_n          // save n
		call	 printn
		lda		 crlf
		call	 prints

		lda		 input_n
		cma ina	 
		sta		 loop_n
r_loop: call	 random
		anda     pFFF
		sta		 input_n
		call	 printn
		lda		 crlf
		call	 prints
		isz		 loop_n
		jmp 	 r_loop

		cla
		sta      count
c_loop:	lda		 count
		ina
		sta		 count
		CALL	 printn
		lda		 colons
		call	 prints
		lda		 input_n
		call	 printn
		lda		 crlf
		call	 prints

		ldb      input_n   -- number to evaluate C on
//  C  <= 1 -> done
//     even -> n / 2
//     odd  -> 3 * n + 1	 		
	    cll cla ina	      // 1
		sub     c,b,a     // n-1
		szc smc rss
		jmp     done
		and     c,b,a     // n & 1
		szc 
		jmp     odd
		add		a,a,a     // 2
		div		c,b,a	  // n/2
		stc		input_n
		jmp     c_loop
odd:    add		c,a,a     // 2
		add     c,c,a     // 3
		mul		c,c,b     // n*3
		inc				  // n*3 + 1
		stc		input_n
		snl              // check for overflow
		jmp		c_loop

overflow: lda  overs
		  jmp  prints
		  lda  crlf
		  jmp  prints

done:
		hlt

// EOF on input, now add numbers
endinput: lda   eof_prompt
		CALL     prints
		hlt
eof_prompt:	 eof_strng
eof_strng: 
		'M'
		'i'
		's'
		's'
		'i'
		'n'
		'g'
		' '
		'I'
		'n'
		'p'
		'u'
		't'
		' '
		'N'
		'u'
		'm'
		'b'
		'e'
		'r'
lf:		10
		0
crlf:   lf
colons: colon
colon:  ':'
		' '
		0

overs:  over
over:   
		'O'
		'v'
		'e'
		'r'
		'f'
		'l'
		'o'
		'w'
		'!'
		0

		end main
