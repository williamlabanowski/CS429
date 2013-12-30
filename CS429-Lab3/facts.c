//William Labanowski
//wjl427

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0



const int FILENOTFOUND_ERR=-1;
const int BADQUESTION_ERR=-2;
const char* resetStr="";
const int MAXBUFFERSIZE=256;

struct factNode
{
	struct factNode* next;
	char* property;
	char* value;
};


struct compNode
{
    struct compNode* next;
    char* name;
	struct factNode* factList;
	
};


struct compNode* head;

void main(int argc, char** argv){
	if(argc>1){
		if(argv[1]==NULL){
			exit(0);
		}
		char* factsFile=argv[1];
		char* questionsFile;
		if(argc==2){
			questionsFile==NULL;
		}
		else{
			questionsFile=argv[2];
		}
		int ret=read(factsFile);
		if(ret==FILENOTFOUND_ERR){
			exit(0);
		}
		ret=write(questionsFile);
		if(ret==BADQUESTION_ERR){
			exit(0);
		}
	}
	else{
		exit(0);
	}
}






//correct
struct compNode* lookupComp(char* compName){
	struct compNode* temp=head;
	while(temp!=NULL){
		if(strcmp(temp->name,compName)==0){
			return temp;
		}
		temp=temp->next;
	}
	return NULL;
}

//correct
struct factNode* lookupFact(struct compNode* computer, char* property){
	struct factNode* temp=computer->factList;
	while(temp!=NULL){
		if(strcmp(temp->property,property)==0){
			return temp;
		}
		temp=temp->next;
	}
	return NULL;
}


//function to trim whitespace from string
char* trimString(char *input){
    char* output;
    int i = 0;
    int length=strlen(input);
    while (isspace(input[i]))
        i++;
    while (isspace(input[length-1]))
        length--;
    length -= i;
    output=malloc(length*sizeof(char)+1);
    memcpy(output,input+(i*sizeof(char)),length);
    return output;
}


int read(char* name){
	FILE *factsInput = fopen(name, "r");
	if(factsInput==NULL){
		return FILENOTFOUND_ERR;
	}
	char c;
	int step=1;
	int problem=FALSE;
	char* temp=(char*)malloc(sizeof(char)*MAXBUFFERSIZE);
	char* property=malloc(sizeof(char*));
	char* compName=malloc(sizeof(char*));
	char* value=malloc(sizeof(char*));
	char* newTemp=malloc(sizeof(char*));
	int i=0;
	while((c = getc(factsInput)) != EOF){
		if(problem==TRUE){
			if(c=='\n'){
				problem=FALSE;
			}
		}
		else if(c=='\n'){
			if(problem==FALSE && step==4){
				//newTemp=malloc(sizeof(temp));
				newTemp=trimString(temp);
				value = malloc(sizeof(newTemp));
				memcpy(value, newTemp, (strlen(newTemp)*sizeof(char)+1));
				struct compNode* computer=lookupComp(compName);
				if(computer==NULL){
					struct compNode* newComp=(struct compNode*)malloc(sizeof(struct compNode)+1);
					//newComp->name=compName;
					newComp->name=(char*)malloc(strlen(compName)*sizeof(char)+1);
					memcpy(newComp->name,compName,(strlen(compName)*sizeof(char))+1);
					newComp->next=head;
					newComp->factList=NULL;
					head=newComp;
					computer=newComp;
					//free(newComp);//if this gives me problems, take it out
				}
				struct factNode* fact=lookupFact(computer,property);
				if(fact==NULL){
					struct factNode* newFact=(struct factNode*)malloc(sizeof(struct factNode));				
					newFact->property=property;
					newFact->property=(char*)malloc(strlen(property)*sizeof(char)+1);
					memcpy(newFact->property,property,(strlen(property)*sizeof(char))+1);				
					newFact->next=computer->factList;
					//newFact->value=value;
					newFact->value=(char*)malloc(strlen(value)*sizeof(char)+1);
					memcpy(newFact->value,value,(strlen(value)*sizeof(char))+1);
					computer->factList=newFact;
					//free(newFact);//if this gives me problems, take it out
				}
				else{
					//do i neeed to malloc here
					//fact->value=value;
					fact->value=(char*)malloc(strlen(value)*sizeof(char)+1);
					memcpy(fact->value,value,(strlen(value)*sizeof(char))+1);
				}
			}
			problem=FALSE;
			step=1;
			//are these being reset properly
			i=0;
			memset(temp,0,MAXBUFFERSIZE);
			memset(newTemp,0,sizeof(newTemp));
			memset(compName,0,sizeof(compName));
			memset(property,0,sizeof(property));
			memset(value,0,sizeof(value));
		}
		else if(step==1){
			if(c!='F'){
				problem=TRUE;
			}
			step++;
			i=0;
		}
		else if(step==2){
			if(c==':'){
				newTemp=trimString(temp);
				compName = malloc(sizeof(newTemp));
				memcpy(compName, newTemp, (strlen(newTemp)*sizeof(char))+1);
				memset(temp,0,MAXBUFFERSIZE);
				memset(newTemp,0,sizeof(newTemp));
				step++;
				i=0;
			}
			else{
				temp[i]=(char)c;
				i++;
			}
		}
		else if(step==3){
			if(c=='='){
				newTemp=trimString(temp);
				property = malloc(sizeof(newTemp));
				memcpy(property, newTemp, (strlen(newTemp)*sizeof(char)));
				memset(temp,0,MAXBUFFERSIZE);
				memset(newTemp,0,sizeof(newTemp));
				step++;
				i=0;
			}
			else{
				temp[i]=(char)c;
				i++;
			}
		}
		else if(step==4){
			temp[i]=(char)c;
			i++;
		}
	}
	return 0;
}

void printCompList(){
	printf("\nPRINTING COMPUTER LIST..\n");
	struct compNode* temp = head;
	while(temp!=NULL){
		printFactList(temp);	
		temp = temp->next;
	}
	printf("DONE PRINTING..\n");
}

void printFactList(struct compNode* comp){
	printf("\n\tPRINTING FACT LIST FOR :       %s\n",comp->name);
	struct factNode* temp = comp->factList;
	while(temp!=NULL){
		printf("\t\tfact property: %s  fact value: %s\n", temp->property, temp->value);
		temp = temp->next;
	}
	printf("\tDONE  PRINTING FACT LIST FOR:        %s\n",comp->name);
} 



int write(char* name){
	FILE* questionsInput;
	if(name==NULL){
		questionsInput=stdin;
	}
	else{
		questionsInput = fopen(name, "r");
		if(questionsInput==NULL){
			return FILENOTFOUND_ERR;
		}
	}
	int problem=FALSE;
	char* compName;
	char* property;
	int step=1;
	char* newTemp;
	char* temp;
	char* theValue;
	char c;
	int i=0;
	struct compNode* abc;
	while((c = getc(questionsInput)) != EOF){
		if(problem==TRUE){
			if(c=='\n'){
				problem=FALSE;
			}
		}
		else if(c=='\n'){
			if(problem==FALSE && step==3){
				newTemp=trimString(temp);
				property = (char*)malloc(strlen(newTemp)*sizeof(char)+1);
				memcpy(property, newTemp, (strlen(newTemp)*sizeof(char))+1);
				struct compNode* theComp=lookupComp(compName);
				if(theComp!=NULL){
					struct factNode* theFact=lookupFact(theComp,property);
					if(theFact!=NULL){
						theValue=(char*)malloc(strlen(theFact->value)*sizeof(char)+1);
						memcpy(theValue,theFact->value,(strlen(theFact->value)*sizeof(char))+1);
						printf("F %s: %s=%s\n",compName,property,theValue);
						memset(theValue,0,(strlen(theValue)*sizeof(char)));
					}
					else{
						printf("F %s: %s=%s\n",compName,property,"unknown");
					}
				}
				else{
					printf("F %s: %s=%s\n",compName,property,"unknown");
				}				
			}
			problem=FALSE;
			step=1;
			i=0;
			memset(temp,0,MAXBUFFERSIZE);
			memset(newTemp,0,(strlen(newTemp)*sizeof(char)));
			memset(compName,0,(strlen(compName)*sizeof(char)));
			memset(property,0,(strlen(property)*sizeof(char)));
		}
		else if(step==1){
			if(c!='Q'){
				problem=TRUE;
				exit(0);
			}
			step++;
		}
		else if(step==2){
			if(c==':'){
				newTemp=trimString(temp);
				compName = malloc(sizeof(newTemp));
				memcpy(compName, newTemp, (strlen(newTemp)*sizeof(char))+1);
				memset(temp,0,MAXBUFFERSIZE);
				memset(newTemp,0,sizeof(newTemp));
				step++;
				i=0;
			}
			else{
				temp[i]=(char)c;
				i++;
			}
		}
		else if(step==3){
			temp[i]=(char)c;
			i++;
		}
	}
	
	return 0;
}
