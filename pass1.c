#pragma warning(disable : 4996)
#include "project.h"
#include"function.h"
boolean stage1 (FILE* file) {
	char line[MAX_LINE];/*arr for current line*/
	char label[MAX_LABEL];
	char* pointerlabel;
	char* current;/*pointer for current line*/
	int countWhiteSpace = 0;
	char* externLabel;
	int numberOfLine=0;
	int countLabel = 0;
	boolean if_label = FALSE;/*flag for label*/
	boolean erorrFlag = FALSE;
	type typeword;
	while (feof(file) == 0) {/*while the file not ended*/
		fgets(line, MAX_LINE, file);/*read line from file*/
		if (feof(file) == 1)  /* if this is the end of the input, we will exit the loop */
			break;
		current = line;
		typeword = typeWord(current, &countWhiteSpace);/*check the first word in the sentence*/
		if (typeword == LABEL ){
			if_label = TRUE;
			pointerlabel = getlable(current,&countLabel);
			strcpy(label, pointerlabel);
			current += strlen(label) + countLabel + 1;/* updates the current position in the row - after the label */
			if (typeWord(current, &countWhiteSpace) == EXTERN)
				if_label = FALSE;
		}
		countWhiteSpace = 0;
		typeword = typeWord(current, &countWhiteSpace);/*check the first word in the sentence*/
		if (typeword == END || typeword == NOTE|| typeword==ENTRY) {
			numberOfLine++;
		}
		else if (typeword == EXTERN) {
			externLabel = legalsymbol(current + countWhiteSpace,FALSE,FALSE);/*check if the label is legal*/
			if ((externLabel) == NULL) {
				printf("\nerror in line Allocation failed\n");
				erorrFlag = TRUE;
			}
			else if (enterInTable(label, 0, TRUE, "external") == FALSE) {
				printf("\nerror in line Allocation failed\n");
				erorrFlag = TRUE;
			}
		}
		else if (typeword == DATA_ASCIZ || typeword == DATA_DW|| typeword == DATA_DH || typeword == DATA_DB){/*line of data */
			if (if_label == TRUE) {/*there is label before data*/
				if (enterInTable(label, DC, FALSE, "data") == FALSE) {
					printf("\nerror in line Allocation failed\n");
					erorrFlag = TRUE;
				}
			}
			if (lineOfData(current, typeword) == FALSE) {/*data without label*/
				erorrFlag = TRUE;
			}
		}
		else/*this is line of code*/
		{
			if (if_label == TRUE)
				if (enterInTable(label, IC, FALSE, "code") == FALSE) {
					printf("\nerror in line Allocation failed\n");
					erorrFlag = TRUE;
				}
		}
		if (erorrFlag == TRUE) {
			return TRUE;
		}
	}
	updateAdress();
	return FALSE;
}
void updateAdress() {/*this method update the address*/
	ICF = IC;
	DCF = DC;
	int i;
	for (i = 0; i < numOfLabels; i++) {
		if (symbol_table[i].attributes == "data")
			symbol_table[i].value += ICF;
	}
	for (i = 0; i < numOfDataB; i++) {
		dataB[i].address += ICF;
	}
	for (i = 0; i < numOfDataH; i++) {
		dataH[i].address += ICF;
	}
	for (i = 0; i < numOfDataW; i++) {
		dataW[i].address += ICF;
	}
}
char* legalsymbol(char* start, boolean isEntry,boolean pass2){/* check if extern label is legal */
	static char str[MAX_LABEL];
	int i;
	while (((*start) == ' ') || ((*start) == '\t'))
		start++;
	if(isEntry==FALSE&&pass2==FALSE)
		start += 7;/*if its extern label*/
	else if(isEntry == TRUE && pass2 == FALSE)
		start += 6;/*if its entry label*/
	while ((*start == ' ') || (*start == '\t'))
		start++;
	for (i = 0; (*start != ' ') && (*start != '\t') && (*start != '\n'); i++, start++)
		str[i] = *start;
	str[i] = ':';
	str[i + 1] = '\0';
	if (islabel(str)==TRUE)
	{
		str[i] = '\0';
		return str;
	}
	return NULL;
}
type typeWord(char* start, int* countWhiteSpace) {/*method returns which type the first word in the current line*/
	char str[MAX_LABEL];/*arr for the first word*/
	int i;
	while ((*start == ' ') || (*start == '\t')){
		start++;
		(*countWhiteSpace)++;
	}
	if (*start == '\n')
		return END;
	if (*start == ';')
		return NOTE;
	for (i = 0; (*start != ' ') && (*start != '\t') && (*start != '\n'); i++, start++)
		str[i] = *start;
	str[i] = '\0';
	if ((strcmp(str, ".dh")) == 0)
		return DATA_DH;
	if ((strcmp(str, ".dw")) == 0)
		return DATA_DW;
	if ((strcmp(str, ".db")) == 0)
		return DATA_DB;
	if ((strcmp(str, ".asciz")) == 0)
		return DATA_ASCIZ;
	if ((strcmp(str, ".extern")) == 0)
		return EXTERN;
	if ((strcmp(str, ".entry")) == 0)
		return ENTRY;
	if (islabel(str))
		return LABEL;
	return OTHER;
}
boolean islabel(char word[]){/* check if label is legal */
	int i;
	if (word[strlen(word) - 1] != ':')
		return FALSE;
	if (isalpha(word[0]))
	{
		for (i = 1; i < (strlen(word) - 1); i++)
			if ((!(isalpha(word[i]))) && (!(isdigit(word[i]))))
				return FALSE;
		return TRUE;
	}
	return FALSE;
}
boolean enterInTable(char* name, int address,boolean isExternal, char* type){
/*enter new label to symbol_table*/
	int count = 0;
	if ((notintable(&count, name)) == FALSE){
		if (isExternal == TRUE) {
			if (symbol_table[count].attributes != "external") {
				return FALSE;
			}
		}
		else
		return FALSE;
	}
	else{
		if (numOfLabels++ == 0){
			symbol_table = (label*)malloc(sizeof(label));
			if (symbol_table == NULL)
			{
				printf("\nerror in line Allocation failed\n");
				exit(1);
			}
		}
		else{
			symbol_table = (label*)realloc(symbol_table, numOfLabels * sizeof(label));
			if (symbol_table == NULL)
			{
				printf("\nerror in line Allocation failed\n");
				exit(1);
			}
		}
		strcpy(symbol_table[numOfLabels - 1].symbol, name);
		symbol_table[numOfLabels - 1].value = address;
		symbol_table[numOfLabels - 1].attributes = type;
		return TRUE;
	}
}
char* getlable(char start[], int* j){
	/* return the label */
	static char label[MAX_LABEL];
	int i;
	while ((*start == ' ') || (*start == '\t'))
	{
		start++;
		(*j)++;
	}
	for (i = 0; (start[i] != ':'); i++)
		label[i] = start[i];
	label[i] = '\0';
	return label;
}
boolean notintable( int* countLine, char namel[]){
	/* check if symbol is'nt in symbol-table, holds a pointer to the position of the label in the table */
	int i;
	for (i = 0; i < numOfLabels; i++){
		if (strcmp(symbol_table[i].symbol, namel) == 0)
			return FALSE;
		(*countLine)++;
	}
	return TRUE;
}
boolean lineOfData(char* current, type typeData){
	char dest[12];
	char ch ;
	int number;
	int count=0;
	char* pointerNumber;
	if (typeData != DATA_ASCIZ) {
		ch = *(current + 3);
		for (; ch == ' ' || ch == '\t'; ch = *(++current));
		if (*current == ','|| * current == '\n') {/*נתון שגוי*/
			printf("\nerror in line Allocation failed\n");
			return FALSE;
		}
		while (ch != '\n'){
			if (ch == '+') {ch = *(++current);}		
			pointerNumber = current;
			while (isdigit(ch)||ch=='=') {
				ch = *(++current);
				count++;
			}
			if (count!=0) {
				strncpy(dest, pointerNumber, count);
				*(dest + count + 1) = '\0';
				number = atoi(dest);
				count = 0;
				if (typeData == DATA_DB&& number < -128 || number>127){
					printf("\nerror in line Allocation failed\n");
					return FALSE;
				}
				if (typeData == DATA_DH && number < -32768 || number>32767){
					printf("\nerror in line Allocation failed\n");
					return FALSE;
				}
				if (typeData == DATA_DW && number <  /*-*/2147483648 || number>2147483647) {
					printf("\nerror in line Allocation failed\n");
					return FALSE;
				}
				enterData(typeData, number);
			}
			else {
				printf("\nerror in line Allocation failed\n");
				return FALSE;/*פסיק בסוף שורה*/
			}
			for (; ch == ' ' || ch == '\t'; ch = *(++current));
			if (*current == ',') {
				ch = *(++current);
				for (; ch == ' ' || ch == '\t'; ch = *(++current));
				if (ch == ','||ch=='\n') {
					printf("\nerror in line Allocation failed\n");
					return FALSE;
				}
			}
			for (; ch == ' ' || ch == '\t'; ch = *(++current));	
		}
		allocationOfMemoryToSymbolData();
		symbolData[sumOfCharacters - 1] = 'f';
	}
	else{
		count = 0;
		ch = *(current + 6);
		for (; ch == ' ' || ch == '\t'; ch = *(++current));
		if (ch == '"') {
			ch = *(++current);
			while (ch != '"' && ch != '\n') {
				enterData(DATA_ASCIZ, ch);
				ch = *(++current);
				count++;
				if (ch < 32 || ch>127) {
					printf("\nerror in line Allocation failed\n");
					return FALSE;
				}
			}
			if (ch == '\n') {/*אם המרוזת לא נסגרת במרכאות*/
				printf("\nerror in line Allocation failed\n");
				return FALSE;
			}
			if (count = 0 && (ch == '"' || ch == '\n') || count != 0 && ch == '\n'){/*מחרוזת נפתחת ונסגרת -אין איברים*/
				printf("\nerror in line Allocation failed\n");
				return FALSE;
			}
		}
		allocationOfMemoryToSymbolData();
		symbolData[sumOfCharacters - 1] = 'f';
	}
	return TRUE;
}
void enterData(type dataType, int number){
	char sign;
		if (dataType == DATA_DB){
			allocationOfMemoryToDataB();
			sign = 'b';
			dataB[numOfDataB - 1].numOfBits = number;
			dataB[numOfDataB - 1].address = DC;
			DC += SIZE_OF_BYTE;	
		}
		else if (dataType == DATA_DH){
			allocationOfMemoryToDataH();
			sign = 'h';
			dataH[numOfDataH - 1].numOfBits = number;
			dataH[numOfDataH - 1].address = DC;
			DC += SIZE_OF_HALF_WORD;
		}
		else if (dataType == DATA_DW){
			allocationOfMemoryToDataW();
			sign = 'w';
			dataW[numOfDataW - 1].numOfBits = number;
			dataW[numOfDataW - 1].address = DC;
			DC += SIZE_OF_WORD;
		}
		else{
			allocationOfMemoryToDataB();
			sign = 'a';
			dataB[numOfDataB - 1].numOfBits = number;
			dataB[numOfDataB - 1].address = DC;
			DC += SIZE_OF_BYTE;
		}
		allocationOfMemoryToSymbolData();
		symbolData[sumOfCharacters - 1] = sign;
}
void allocationOfMemoryToDataB() {
	if (numOfDataB++ == 0)
	{

		dataB = (db*)malloc(sizeof(db));
		if (dataB == NULL)
		{
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
	else
	{

		dataB = (db*)realloc(dataB, numOfDataB * sizeof(db));
		if (dataB == NULL)
		{
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
}
void allocationOfMemoryToDataH() {
	if (numOfDataH++ == 0)
	{

		dataH = (dh*)malloc(sizeof(dh));
		if (dataH == NULL)
		{
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
	else
	{

		dataH = (dh*)realloc(dataH, numOfDataH * sizeof(dh));
		if (dataH == NULL)
		{
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
}
void allocationOfMemoryToDataW() {
	if (numOfDataW++ == 0)
	{

		dataW = (dw*)malloc(sizeof(dw));
		if (dataW == NULL)
		{
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
	else
	{

		dataW = (dw*)realloc(dataW, numOfDataW * sizeof(dw));
		if (dataW == NULL)
		{
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
}
void allocationOfMemoryToSymbolData() {
	if (sumOfCharacters++ == 0)
	{
		symbolData = (char*)malloc(sizeof(char));
		if (symbolData == NULL)
		{
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
	else
	{
		symbolData = (char*)realloc(symbolData, sumOfCharacters * sizeof(char));
		if (symbolData == NULL)
		{
			printf("\nerror in line Allocation failed\n");
			exit(1);
		}
	}
}


