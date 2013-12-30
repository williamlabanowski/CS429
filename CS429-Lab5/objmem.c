/* 
   Assembler for PDP-8.  Memory and object file creation. 
*/

#include "asm8.h"
#define lowerSix 0x3F
#define higherSix 0xFC0
#define newHigherSix 0x3F00



/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* we want to assemble instructions.  We could assemble and output them
   all at once.  But we have a problem of forward references.  So we
   keep the (partially) assembled instructions in an array of them,
   essentially simulated memory.  That allows us to come back and 
   fix them up when the forward reference is resolved.

   We need to know which memory locations are from assembled
   instructions, and which are just empty; so each memory location
   has a bit (defined/not defined).
*/

INST     memory[4096];
Boolean defined[4096];
Address entry_point = 0;


void Clear_Object_Code(void)
{
    int i;
    for (i = 0; i < 4096; i++)
        {
            defined[i] = FALSE;
        }
}

void Define_Object_Code(Address addr, INST inst, Boolean redefine)
{
    if (debug)
        fprintf(stderr, "object code: 0x%03X = 0x%03X\n", addr, inst);
    if (defined[addr] && !redefine)
        {
            fprintf(stderr, "redefined memory location: 0x%03X: was 0x%03X; new value 0x%03X\n",
                    addr, memory[addr], inst);
            number_of_errors += 1;
        }
                
    defined[addr] = TRUE;
    memory[addr] = inst;
}

INST Fetch_Object_Code(Address addr)
{
    INST inst;

    if (defined[addr])
        inst = memory[addr];
    else
        inst = 0;

    if (debug)
        fprintf(stderr, "read object code: 0x%03X = 0x%03X\n", addr, inst);
    return(inst);
}


//CHANGE TO CHAR
//INST the correct type here?
char highOrder(short orig){
	char ret=(char)(orig>>6);
	return (ret&lowerSix);
}

char lowOrder(short orig){
	char ret= (char)(orig&lowerSix);
	return (ret&lowerSix);
}



void Output_Object_Code(void)
{
	//change this
    putc('O',output);
	putc('B',output);
	putc('J',output);
	putc('8',output);
	char temp=highOrder(entry_point);
	putc(temp,output);
	temp=lowOrder(entry_point);
	putc(temp,output);
    int i=0;
    while (i < 4096)
        {
            if (defined[i]){
				//2 byte representation of 12 bit addresses.  int the right way to do this
            	char adding[256];
				adding[1]=highOrder(i);
				adding[2]=lowOrder(i);
				int size=3;
				while(size<255 && i<4096 && defined[i]){
					//right way to implement this as a character?
					adding[size]=highOrder(memory[i]);
					size++;
					adding[size]=lowOrder(memory[i]);
					size++;
					i++;
				}
				adding[0]=(char)size;
				int index=0;
				while(index<size){
					putc(adding[index],output);
					index++;
				}	
            }
			else{
				i++;
			}
        }
}

