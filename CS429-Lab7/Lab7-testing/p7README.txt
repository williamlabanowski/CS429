Grading of Program 7
Here is the package: p7grade_to_replace_the_damaged_one_on_piazza.tar.bz2
Its usage is much like the one of Program 5, besides you need to build the parser under the parser directory in the package.

The grading policy:
Compiled: 1 point
Full test cases: 0.5 point for correct output, 1.5 points for verbose information (details below)
Lightweight case (renato): 0.5 point for verbose information
Corner case (spover): 0.5 point for no crash

Verbose information is graded by the parser.
Each piece of effective information has equal weights in grading.
A piece of effective information may be:
1. Cycles spent in executing an instruction
2. PC
3. Disassembled instruction
4. Verbose information (Read/Write)

Let me use an example to show the details:
Time   3: PC=0x1003 instruction = 0x8100 (LDA): M[0x1000] -> 0xFFFF, 0xFFFF -> A
Time   4: PC=0x1004 instruction = 0xEB80 (AND): A -> 0xFFFF, A -> 0xFFFF, 0xFFFF -> SP
Let's focus on Time 4.
The cycles used in AND is one, which is tracked by the parser. <- First piece of information
The PC is 0x1004. This is the second piece. (Btw, if you got a wrong PC, all the remaining of this line would be considered as incorrect values)
AND, the disassembled instruction is the third piece.
The "A -> 0xFFFF", "A -> 0xFFFF", "0xFFFF -> SP" are the remaining three pieces of information, which are called read/write information in the following text.

The parser holds a relaxed rule in comparing r/w information. These information pieces are grouped into read/write groups, and the sequence in the same group is arbitrary. Duplicated items are also eliminated before comparison
For example,
A -> 0xFFFF, A -> 0xFFFF, 0xFFFF -> SP
matches
A -> 0xFFFF, 0xFFFF -> SP
, although the second only has one
A -> 0xFFFF
Another example
A -> 0x00AA, D -> 0x0000, 0x0001 -> L
matches
0x0001 -> L, D -> 0x0000, A -> 0x00AA
, as the sequence in a group is arbitrary.

However,
M[0x1039] -> 0xFFFE, 0xFFFF -> M[0x1039]
doesn't match
0xFFFF -> M[0x1039], M[0x1039] -> 0xFFFE
, because the first one reads M[0x1039] and then write to M[0x1039], while the second one writes before reads.

Btw, the goal of introducing r/w groups is to relax the comparison but still hold some necessary requirements on the sequences. For instance, to simulate a POP, the simulator needs to read SP and then write SP+1 to SP, otherwise the simulator violates internal logic of POP -- you can't write SP+1 to SP before knowing the value of SP.