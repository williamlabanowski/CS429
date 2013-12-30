
//William Labanowski
//wjl427
//using 1 slip day on this assignment


#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void main(int argc, char** argv){
	if(argc>1){	
		char* arg=argv[1];
		if(arg[0] == '-' && arg[1]=='d'){
			if(argc==2){
				decode(NULL);
			}
			else{
				decode(argv[2]);
			}
		}	
		else if(argc==1){
			encode(NULL);
		}
		else{
			encode(arg);
		}
	}		
}








void encode(char* fileName){
		FILE* fp;
		if(fileName==NULL){
			fp= stdin;
		}
		else{
			fp= fopen(fileName, "r");
		}
		char charPrinting;
		int c;
		int sig=0;
		int output=0; //important 5 bits to output
		int temp;
		int remaining;
		int numChars=0;
		while ((c = getc(fp)) != EOF){
			temp=255&c;
			remaining=8;
			if(sig>0){
				temp=temp>>(sig+3);
				output+=temp;
				remaining-=5-sig;
				//output
				charPrinting=toChar(output);
				// fputc(stdout,charPrinting);
				fprintf(stdout, "%c", charPrinting);
				output=0;
				numChars++;
				if(numChars==72){
					fprintf(stdout,"%c",'\n');
					numChars=0;
				}
			}
			if(remaining>=5){
				temp=255&c;
				temp=temp>>(remaining-5);
				output=temp&31;
				//output it
				charPrinting=toChar(output);
				// fputc(stdout,charPrinting);
				fprintf(stdout, "%c", charPrinting);
				output=0;
				numChars++;
				if(numChars==72){
					fprintf(stdout,"%c",'\n');
					numChars=0;
				}
				remaining-=5;
			}
			sig=remaining;
			temp=255&c;
			temp=temp<<(5-sig);
			output=temp&31;
		}
		//output it
		charPrinting=toChar(output);
		// fputc(stdout , charPrinting);
		if(sig != 0){
			fprintf(stdout, "%c", charPrinting);
		}
		if(numChars!=0){
			fprintf(stdout,"%c",'\n');
		}
		fclose(fp);
}







int toChar(int output){
	if((output>=0) && (output<=25)){
		return (output+65);
	}
	else if((output>=26) && (output<=31)){
		return (output+22);
	}
	printf("Error!");
	return 32;
}


int fromChar(int input){
	if(input>=65 && input<=90){
		return input-65;
	}
	else if(input>=48 && input<=53){
		return input-22;
	}
	else{
		return -1;
	}
}





void decode(char* fileName){
		FILE* fp;
		if(fileName==NULL){
			fp= stdin;
		}
		else{
			fp= fopen(fileName, "r");
		}
		int c;
		int sig=0;
		int output=0; //important 5 bits to output
		int temp;
		int remaining;
		while ((c = getc(fp)) != EOF){
			c=fromChar(c);
			while(c==-1){
				if((c = getc(fp)) != EOF){
					c=fromChar(c);
				}
				else{
					exit(0);
				}
			}
			temp=31&c;
			remaining=5;
			if(sig>=3){
				temp=temp>>(sig-3);
				output+=temp;
				remaining-=8-sig;
				printf("%c",(char)output);
				output=0;
				sig=remaining;
			}
			else{
				sig+=5;
			}
			temp=31&c;
			temp=temp<<(8-sig);
			output+=temp&255;
		}
	fclose(fp);
}




