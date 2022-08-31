#pragma warning(disable : 4996)
#include"function.h"
#include"project.h"
boolean lineOfCode(char*current){
	int i;
	int indexAction=-1;
	char typeAction[MAX_LABEL];
	for (; *current == ' ' || *current == '\t';  (++current));
	for (i = 0; (*current!= ' '&& *current!= '\t' &&*current != '\n')&&i<MAX_LABEL; (++current), i++) {
		typeAction[i] = *current;
	}
	typeAction[i + 1] = '\0';
	if ((*current) == '\n' || i > 5)
		return FALSE;
	for (i = 0; i < 27&&indexAction==-1; i++) {
		if (strcmp(typeAction, allActions[i].methodName) == 0) {
			indexAction = i;
	   }
	}
	if (indexAction == -1)
		return FALSE;
	else {
		for (; *current == ' ' || *current == '\t'; (++current));
		if (allActions[indexAction].methodType == 'R')
			if ((checkLineOfR(current, indexAction))==FALSE) { return FALSE; }
		if (allActions[indexAction].methodType == 'I')
			if ((checkLineOfI(current, indexAction))==FALSE) { return FALSE; }
		if (allActions[indexAction].methodType == 'J')
			if ((checkLineOfJ(current, indexAction))==FALSE) { return FALSE; }
		IC += 4;
	}	
}
boolean checkLineOfR(char* current, int index) {
	int countRegister = 0;
	int countDigits = 0;
	int i=0;
	char ch = *current;
	char stringNumber[12];//dגודל?;
	char* pointerNumber;
	int numbers[4];
	if (ch != '$') { return FALSE; }
	else { ch = *(current++); }
	pointerNumber = current;
	while (ch != '\n'&&ch!='\t'&&ch!=' '&& countRegister<5) {
		while (isdigit(ch)) {
			countDigits++;
			ch = *(current++);
		}
		if (countDigits) {
			strncpy(stringNumber, pointerNumber, countDigits);
			stringNumber[countDigits + 1] = '\0';
			countDigits = 0;
			numbers[i] = atoi(stringNumber);
			countRegister++;
		}
		else
			return FALSE;
		for (; ch == ' ' || ch == '\t'; ch = *(++current));
		if (ch == ',' && countRegister == allActions[index].help) { return FALSE; }
		if (ch == '\n' && countRegister < allActions[index].help) { return FALSE; }
		if (ch != ',' && countRegister < allActions[index].help) { return FALSE; }
		if (ch == ',') {
			ch = *(++current);
		    for (; ch == ' ' || ch == '\t'; ch = *(++current));
			if (ch != '$') { return FALSE; }
			ch = *(++current);
		}
	}
	if (countRegister == allActions[index].help) {
		allocationOfMemoryToInstructionTable();
		instruction_table[numOfInstruction - 1].rAction.opcode = allActions[index].opcode;
		instruction_table[numOfInstruction - 1].rAction.funct = allActions[index].funct;
		instruction_table[numOfInstruction - 1].rAction.notUsing = 0;
		instruction_table[numOfInstruction - 1].rAction.address = IC;
		if (allActions[index].help == 3) {
			instruction_table[numOfInstruction - 1].rAction.rs = numbers[0];
			instruction_table[numOfInstruction - 1].rAction.rt = numbers[1];
			instruction_table[numOfInstruction - 1].rAction.rd = numbers[2];
		}
		else {
			instruction_table[numOfInstruction - 1].rAction.rs = numbers[0];
			instruction_table[numOfInstruction - 1].rAction.rd = numbers[1];
			instruction_table[numOfInstruction - 1].rAction.rt = 0;
		}
		return TRUE;
	}
	return FALSE;
}
boolean checkLineOfI(char* current, int index) {
	if (allActions[index].help != 2) {
		int countRegister = 0;
		int countDigits = 0;
		int i = 0;
		char ch = *current;
		char stringNumber[12];//dגודל?;
		char* pointerNumber;
		int numbers[4];
		if (ch != '$') { return FALSE; }
		else { ch = *(current++); }
		while (ch != '\n' && ch != '\t' && ch != ' ' && countRegister < 5) {
				if (ch == '+')
					ch = *(current++);
				pointerNumber = current;
			while (isdigit(ch)||ch=='-') {
				countDigits++;
				ch = *(current++);
			}
			if (countDigits) {
				strncpy(stringNumber, pointerNumber, countDigits);
				stringNumber[countDigits + 1] = '\0';
				countDigits = 0;
				numbers[i++] = atoi(stringNumber);
				countRegister++;
			}
			else
				return FALSE;
			for (; ch == ' ' || ch == '\t'; ch = *(++current));
			if (ch == ',' && countRegister == 3) { return FALSE; }
			if (ch == '\n' && countRegister < 3) { return FALSE; }
			if (ch != ',' && countRegister < 3) { return FALSE; }
			if (ch == ',') {
				ch = *(++current);
				for (; ch == ' ' || ch == '\t'; ch = *(++current));
				if (ch != '$' && countRegister != 1) { return FALSE; }
				if (countRegister != 1 && ch == '$')
					ch = *(++current);
				if (ch == '$' && countRegister == 1) { return FALSE; }
			}
		}
		if (countRegister == 3) {
			allocationOfMemoryToInstructionTable();
			instruction_table[numOfInstruction - 1].iAction.opcode = allActions[index].opcode;
			instruction_table[numOfInstruction - 1].iAction.rs = numbers[0];
			instruction_table[numOfInstruction - 1].iAction.immed = numbers[1];
			instruction_table[numOfInstruction - 1].iAction.rt = numbers[2];
			instruction_table[numOfInstruction - 1].iAction.address = IC;
			return TRUE;
		}
		return FALSE;
	}
	else {
		allocationOfMemoryToInstructionTable();
		instruction_table[numOfInstruction - 1].iAction.address = IC;
	}

}
boolean checkLineOfJ(char* current, int index) {
/*מעבר שני*/
	allocationOfMemoryToInstructionTable();
	instruction_table[numOfInstruction - 1].jAction.addressIC = IC;
}
void allocationOfMemoryToInstructionTable() {
	if (numOfInstruction++ == 0)
	{
		instruction_table = (typeActions*)malloc(sizeof(typeActions));
		if (instruction_table == NULL){
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
	else{
		instruction_table = (typeActions*)realloc(instruction_table, numOfInstruction * sizeof(typeActions));
		if (instruction_table == NULL){
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
}


