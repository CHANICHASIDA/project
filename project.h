#pragma once
#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include <ctype.h>
#define MEMORY_ADRRES 100
#define MAX_LINE 80 /* maximum chars per line */
#define MAX_LABEL 31 /* maximum number of labels */
#define SIZE_OF_WORD 4
#define SIZE_OF_HALF_WORD 2
#define MAX_FILE_NAME 30
#define SIZE_OF_BYTE 1
typedef enum { FALSE, TRUE } boolean;
/*Action instruction structure*/
typedef struct
{
	char* methodName;
	char methodType;
	int opcode;
	int funct;
	int help;
}action;

/* help=
Auxiliary variable for identifying the types of operations.
For type R operations the variable is used for the correct amount of registers.
For type I operations the variable will contain 1,2,3 depending on the type of instruction.
1-Arithmetic and logical instructions
2- Conditional branching instructions
3-Instructions for loading and storing in memory
For type J operations:
0 per action 'stop' (no registers)
1 for actions: 'la','call' (one register)
2 per action 'jmp'(2 registers)
 */
/*external symbol table*/
typedef struct {
	unsigned int address;
	char* labelName;
}externalsymboltable;
/*R typeAction*/
typedef struct
{
	unsigned int notUsing : 6;
	unsigned int funct : 5;
	unsigned int rd : 5;
	unsigned int rt : 5;
	unsigned int rs : 5;
	unsigned int opcode : 6;
	unsigned int address;
}R;
/*I typeAction*/
typedef struct
{
	unsigned int immed : 16;
	unsigned int rt : 5;
	unsigned int rs : 5;
	unsigned int opcode : 6;
	unsigned int address;
}I;
/*J typeAction*/
typedef struct
{
	unsigned int address : 25;
	unsigned int reg : 1;
	unsigned int opcode : 6;
	unsigned int addressIC;
}J;

typedef struct
{
	int numOfBits : 8;
	int address;
}db;
typedef struct
{
	int numOfBits : 32;
	int address;
}dw;
typedef struct
{
	int numOfBits : 16;
	int address;
}dh;
typedef union
{
	R rAction;
	I iAction;
	J jAction;
	bitToPrint print;
}typeActions;
typedef struct {
	int num : 32;
}bitToPrint;
//typedef union {
//	typeActions v;
//	bitToPrint f;
//}help;
/*labelStructure*/
typedef struct
{
	char* symbol;
	int value;
	char* attributes;
	boolean entry;

}label;
typedef enum {
	LABEL, DATA_DB, DATA_DH, DATA_DW, DATA_ASCIZ, EXTERN, ENTRY, END, NOTE, OTHER
}type; /*
LABEL = a label - symbol
dw= data in size of word
db = data in size of byte
dh = data in size of half-word
DATA_ASCIZ= string data
EXTERN = .extern
ENTRY = .entry
END = empty line
NOTE = note
OTHER = non of the above */




