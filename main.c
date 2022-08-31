#include "project.h"
#include "function.h"
#pragma warning(disable : 4996)
action allActions[27] =
{
{"add",'R',1,0,3},
{"sub",'R',2,0,3},
{"and",'R',3,0,3},
{"or",'R',4,0,3},
{"nor",'R',5,0,3},
{"move",'R',1,1,2},
{"mvhi",'R',2,1,2},
{"mvlo",'R',3,1,2},
{"addi",'I',0,10,1},
{"subi",'I',0,11,1},
{"andi",'I',0,12,1},
{"ori",'I',0,13,1},
{"nori",'I',0,14,1},
{"bne",'I',0,15,2},
{"beq",'I',0,16,2},
{"blt",'I',0,17,2},
{"bgt",'I',0,18,2},
{"lb",'I',0,19,3},
{"sb",'I',0,20,3},
{"lw",'I',0,21,3},
{"sw",'I',0,22,3},
{"lh",'I',0,23,3},
{"sh",'I',0,24,3},
{"jmp",'J',0,30,2},
{"la",'J',0,31,1},
{"call",'J',0,32,1},
{"stop",'J',0,33,0}
};
int IC, DC, ICF, DCF;
externalsymboltable* external_symbol;
int numOfInstruction;
typeActions* instruction_table;
int entrySymbol;
int externSymbol;
int sumOfCharacters;
int numOfDataW;
int sumOfCharacters;
int numOfDataW;
int numOfDataH;
int numOfDataB;
char* symbolData;
dw* dataW;
dh* dataH;
db* dataB;
int numOfLabels;
label* symbol_table;
int main(int argc, char* argv[]) {
	FILE* fd;
	boolean error;
	if (argc == 1) {
		printf("\nYou did not enter a file name.\n");
	}
	while (--argc) {
		DC = 0;
		IC = MEMORY_ADRRES;
		fd = fopen(argv[argc], 'r');
		if (fd == NULL)
			printf("\nThe file\"%s\"does not exist.\n", argv[argc]);
		else{
			error = stage1(fd);
			if (error == TRUE) {
				printf("error exist");
				//exit(1);
			}
			else {
				error = stage2(fd);
				if (error == TRUE) {
					printf("error exist");
					//exit(1);
				}
				else {
					
				}
			}
		}
		fclose(argv[argc]);
	}

}

