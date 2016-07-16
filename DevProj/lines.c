#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include <string.h>

   void populateOpcodeTable(FILE*);
   void buildSymbolTable(FILE*);
   void parseThrough(FILE*);
   int isAssembly(char *name);

		
		void populateOpcodeTable(FILE* opcodeTable) 
  {

	char* name;
	int value;
	/*array to hold chars/ one string*/
  	char str[80];
	/*delimiter which will parse string until it reaches a space*/
	const char s[2] = " ";
	char* token; //pointer to the first token
	
	while(fgets(str,80,opcodeTable)!=NULL)
  {

	token = strtok(str, s); //this will pull the first word [add]
	name = token;

	token = strtok(NULL,s);
	sscanf(token, "%x", &value);
	addOpcode(name,value);
	
		

  }
//display();
}
	void parseThrough(FILE* input)
  {
   char str[80];
   const char s[2] = " ";
   char *token;
	int lc = 1;
	int startLocation;
	
	struct symbols* symbol = (struct symbols*)malloc(sizeof(struct symbols));
	struct opcode* op = (struct opcode*)malloc(sizeof(struct opcode));

	fgets(str,80,input);
   	token = strtok(str, s);//TESTPROG
	TESTPROG = malloc(strlen(token)+1);
	strcpy(TESTPROG,token);
	printf("TESTPROG[%s]",TESTPROG);

	token = strtok(NULL, s);//START LABEL
	token = strtok(NULL, s);//lc value
	sscanf(token,"%x",&startLocation);

	location[1] = startLocation;
	location[2] = startLocation;
	printf("%x \t\t%s START %x\n",location[1],TESTPROG,location[2]);
	puts("*************END OF FIRST LINE************");

	while(fgets(str,80,input)!=NULL)
	  {
		printf("%d     \t",(lc)+3);  //line number multiples of 5
		printf("%x     \t\t",(location[lc]));  //Location NUMBER
	    location[lc+1] = location[lc] + 3;//adding 3 to every location number

	    token = strtok(str,s);//FIRST
		
		
		op = searchOpcode(token);
		symbol = symbolSearch(token);  //check symbol table for label
		
		
	    if(op != NULL)
	     {
		printf("\n*****The opcode table is populated and working******\n");
		currentLineOp[lc] = (returnValue(op)) << 16;
		printf("%d \t ",currentLineOp[lc]);
	     }
	
	    else if((symbol == NULL) && (isAssembly(token) == 0))
		{
			
			addSymbolNode(1,token);
		}
	
		op = NULL;
	   
	
		while(token != NULL)
		{
		  //printf("%s",token);	
		  token = strtok(NULL,s);
		 
		op = searchOpcode(token); 
		symbol = symbolSearch(token);  
		

		if(op != NULL)
			
	     {
		printf("\n*****The opcode table is populated and working******\n");
		currentLineOp[lc] = (returnValue(op)) << 16;
		printf("opcodelc:A%d \t ",currentLineOp[lc]);
	     }
		
		
	}
	lc++;	
	
  }
}
 /*~~~~~~~~~~~~~~~~~~~~~~~~~MARKS THE LOCATION OF SYMBOLS IN THE SYMBOL TABLE~~~~~~~~~*/
  void pass2(FILE* input)
  {
	char word[80];
	char str[80];
	int i,j;
	i = 0;
	j = 0;
	lc = 1;

	struct symbols* symbol = (struct symbols*)malloc(sizeof(struct symbols));

	while(fgets(str,80,input)!=NULL)
	{
		printf("\n%s\n",str);
		while(!(isspace(str[j])))
	{
	  	word[i++] = str[j++];
	}
	   //check if the word read in is the END LABELINCREMENT LINE NUM
	word[i] = '\0';
	if(strcmp(word, "END") == 0)
	{
		lc++;
		return;
	}

	if(isThisSymbol(word) == 1)
	{
		symbol = symbolSearch(word);
		setAddress(symbol,location[lc]);
		printf("location[%d]\n",location[lc]);
	}
	i=0;
	j=0;
	lc++;
	}
	
  }
	  
	  /*~~~~~~~~~~~~~~~~~ADDS CORRECT ADDRESSES TO SYMBOL TABLE COMPLETING IT~~~~~~~~~~~~~~~~~~~~~~~~~ */
	void pass2Address(FILE* input)
	{
	char string1[50];
	char string2[50];
	char string3[50];
	char str[80];
	lc = 1;
	
	while(fgets(str,80,input)!=NULL)
	{
	  sscanf(str,"%s %s %s",string1, string2, string3);
		
		

	if(returnAddress(string2) > 0)
	  {
		printf("%s is a symbol",string2);
		currentLineOp[lc] = currentLineOp[lc] | returnAddress(string2);
	  }

	if(returnAddress(string3) > 0)
	  {
		printf("%s is a symbol",string3);
		currentLineOp[lc] = currentLineOp[lc] | returnAddress(string3);
	  }

	  if((string3[0] == 'Z') && (string3[1] == 90))
	   {
		currentLineOp[lc] = string3[2] << 16;
	   }

		printf("\nLINE %x\n",currentLineOp[lc]);
		lc++;

	}
}
	

	

	/*~~~~~~~~~FIGURES IF CURRENT TOKEN IS AN ASSEMBLY DIRECTIVE RETURNS AN IN~~~~~~~~~~~~~~~~~*/
  int isAssembly(char *name)
   {
	puts("IsAssemblyDirective?");
   const char s[2] = " ";
   char *token;
   
   
   token = strtok(assemblyDer, s);

	if(assemblyDer == NULL){
		puts("FILE NOT FOUND");
		return 0;
		}
	if((name == NULL) || (token == NULL))
	  {
		puts("There is a problem, one of these values is null");
		return 0;
	  }

	if(strcmp(token,name) == 0)
	   {
		puts("yes");
		puts("Not being inserted in symbol table");
		return 1;
	    }

		while(token != NULL)
		  {
			token = strtok(NULL,s);

			if(token != NULL)
		
		    
		    if(strcmp(token,name) == 0){
	   
		puts("yes");
		puts("Not being inserted in symbol table");
		return 1;
						
	    }

		
	}
		
		puts("no");
		return 0;
}

  void ObjFile(FILE *input,FILE *output)
	{
	int i = 0;
	fprintf(output,"TESTPROG\n");
	for(i=1;i<lc;i++)
	  {
	    fprintf(output,"%06x\n",currentLineOp[i]);
	   }
		fprintf(output,"END\n");
	}
	





