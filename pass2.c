#pragma warning(disable : 4996)
#include "project.h"
#include"function.h"
int IC2=MEMORY_ADRRES;
boolean stage2(FILE* fd2){
    boolean notError = TRUE;
    int line = 1;
    char arrline[MAX_LINE];
    char* entrys;
    char* labelName;
    char* current;
    int countLine;
    int h;
    int j = 0;
    type  wordtype;
    while (feof(fd2) == 0) {
        fgets(arrline, MAX_LINE, fd2);
        if (feof(fd2) == 1){
            break;
        }
        current = arrline; /* pointer to the current position in row */
        h = 0;
        wordtype = typeWord(current, &h);
        if (wordtype == LABEL){
            labelName = getlable(current, &j); /* get the name of the label */
            current += strlen(labelName) + j + 1; /* updates the current position in the row - after the label */
            h = 0;
            wordtype = typeWord(current, &h); /* skips the label */
        }
        if (wordtype == NOTE || wordtype == END) /* chek if the line is an empty line */
            line++;
        else if ((wordtype == EXTERN) || (wordtype == DATA_ASCIZ) || (wordtype == DATA_DB)||(wordtype==DATA_DH)||(wordtype==DATA_DW))
            line++;
        else if (wordtype == ENTRY) /* add to 'entry' table */{
            entrys = legalsymbol(current,TRUE,FALSE);
            countLine = 0;
            if (notintable(&countLine, entrys)==FALSE) {
                printf("\neror: in line %d export a non-existent label\n", line);
                notError = FALSE;
            }
            else if(symbol_table[countLine].attributes!="external"){
                symbol_table[countLine].entry = TRUE;/*Icon not found*/
                entrySymbol++;
            }
            else {
                /*תוית בעייתית(external&entry)*/
                printf("\neror: in line %d export a non-existent label\n", line);
                notError = FALSE;
            }
            line++;
        }
        else{
            if (lineOfCode2(current, line) == FALSE)/*Handles a line of code*/
                notError = FALSE;
            line++;
            IC2 += 4;
        }
    } /* end of while */
    if (notError == FALSE){
        printf("error exit2");
        return TRUE;
    }
    return FALSE;
}
boolean lineOfCode2(char* current,int line) {
    int i;
    int indexAction = -1;
    char typeAction[MAX_LABEL];
    for (; *current == ' ' || *current == '\t'; (++current));
    for (i = 0; (*current != ' ' && *current != '\t' && *current != '\n') && i < MAX_LABEL; (++current), i++) {
        typeAction[i] = *current;
    }
    typeAction[i + 1] = '\0';
    if ((*current) == '\n' || i > 5)return FALSE;
    for (i = 0; i < 27 && indexAction == -1; i++) {
        if (strcmp(typeAction, allActions[i].methodName) == 0) {
            indexAction = i;
        }
    }
    if (indexAction == -1)return FALSE;
    else {
        for (; *current == ' ' || *current == '\t'; (++current));
        if (allActions[indexAction].methodType == 'R') return TRUE;
        if (allActions[indexAction].methodType == 'I' && allActions[indexAction].help == 2)
            if ((checkLineOfI2(current, indexAction))==FALSE) { return FALSE; }
        if (allActions[indexAction].methodType == 'J')
            if ((checkLineOfJ2(current, indexAction))==FALSE) { return FALSE; }
        return TRUE;
    }
}
boolean checkLineOfI2(char* current,int indexAction) {
    char ch = *current;
    char* labels;
    int countRegister=0, countDigits=0,i=0,place=0,countLines=0,addressLabel=0;
    char* pointerNumber;
    char stringNumber[12];
    int numbers[2];
    char labelName[MAX_LABEL];
    if (ch != '$') { return FALSE; }
    else { ch = *(current++); }
    while (ch != '\n' && ch != '\t' && ch != ' ' && countRegister < 4&&ch!=',') {
        if (ch == '+')
            ch = *(current++);
        pointerNumber = current;
        while (isdigit(ch) || ch == '-') {
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
        if (ch == '\n' && countRegister < 3) { return FALSE; }
        if (ch == ','&&countRegister<2) {
            ch = *(++current);
            for (; ch == ' ' || ch == '\t'; ch = *(++current));
            if (ch != '$' && countRegister != 2) { return FALSE;}
            if (countRegister != 2 && ch == '$') { ch = *(++current);}
            if (ch == '$' && countRegister == 2) { return FALSE; }
        }
    }
    if (countRegister == 2&&ch==',') {
        place = (IC2 - 100) / 4;
        current++;
        labels = legalsymbol(current, FALSE, TRUE);
        if (labels == NULL) {
            return FALSE;
        }
        strcpy(labelName, labels);
        if (notintable(&countLines, labelName) == TRUE)/*the label not exist in the table */
            return FALSE;
        for (; ch == ' ' || ch == '\t'; ch = *(++current));
        if (ch == '\n'&&symbol_table[countLines].attributes!="external"){
            addressLabel = symbol_table[countLines].value;
            addressLabel -= IC2;
            instruction_table[place].iAction.opcode = allActions[indexAction].opcode;
            instruction_table[place].iAction.rs = numbers[0];
            instruction_table[place].iAction.immed = addressLabel;
            instruction_table[place].iAction.rt = numbers[1];
            return TRUE;
        }
    }
    return FALSE;
}
boolean checkLineOfJ2(char* current, int indexAction) {
    int address=0, reg=0, opcode = 0,countline=0, countDigits=0,number=0;
    int place = (IC2 - 100) / 4;
    char* labelName=NULL;
    char*pointerNumber=NULL;
    char stringNumber[12];
    char labelArr[MAX_LABEL];
    boolean useExternalLabel = FALSE;
    if (allActions[indexAction].help == 0)/*stop action*/ {
        if (*current != '\n') {
            return FALSE;
        }
    }
    else if (allActions[indexAction].help == 1)/*la or call actions */ {
        labelName = legalsymbol(current, FALSE, TRUE);
        if (labelName == NULL)
            return FALSE;
        strcpy(labelArr, labelName);
        if (notintable(&countline, labelArr) == FALSE)
            return FALSE;
        current += strlen(labelName)+1;
        for (; *current == ' ' || *current == '\t'; current++);
        if (*current != '\n')
            return FALSE;
        if (symbol_table[countline].attributes == "external")useExternalLabel = TRUE;
        reg = 0;
        address = symbol_table[countline].value;
        opcode = allActions[indexAction].opcode;
    }
    else /*jmp action*/{
        if (*current == '$') {
            current++;
            while (isdigit(*current)) {
                countDigits++;
                current++;
            }
            if (countDigits != 0) {
                strncpy(stringNumber, pointerNumber, countDigits);
                stringNumber[countDigits + 1] = '\0';
                countDigits = 0;
               number= atoi(stringNumber);
            }
            else
                return FALSE;
            for (; *current == ' ' || *current == '\t'; current++);
            if (*current != '\n')
                return FALSE;
            reg = 1;
            address = number;
            opcode = allActions[indexAction].opcode;
        }
        else {
            labelName = legalsymbol(current, FALSE, TRUE);
            if (labelName == NULL)
                return FALSE;
            strcpy(labelArr, labelName);
            if (notintable(&countline, labelArr) == FALSE)
                return FALSE;
            current += strlen(labelName) + 1;
            for (; *current == ' ' || *current == '\t'; current++);
            if (*current != '\n')
                return FALSE;
            if (symbol_table[countline].attributes == "external")useExternalLabel = TRUE;
            reg = 0;
            address= symbol_table[countline].value;
            opcode= allActions[indexAction].opcode;
        }
        instruction_table[place].jAction.reg = reg;
        instruction_table[place].jAction.opcode = opcode;
        instruction_table[place].jAction.address = address;
        if (useExternalLabel == TRUE) {
            allocationOfMemoryToExtenalSymbol();
            external_symbol[externSymbol - 1].labelName = labelName;
            external_symbol[externSymbol - 1].address = instruction_table[place].jAction.addressIC;
        }
    }
    return TRUE;
}
void allocationOfMemoryToExtenalSymbol() {
    if (externSymbol++ == 0)
    {
        external_symbol = (externalsymboltable*)malloc(sizeof(externalsymboltable));
        if (external_symbol == NULL) {
            printf("\nerror in line Allocation failed\n");
            exit(1);
        }
    }
    else {
        external_symbol = (externalsymboltable*)realloc(external_symbol, externSymbol * sizeof(externalsymboltable));
        if (external_symbol == NULL) {
            printf("\nerror in line Allocation failed\n");
            exit(1);
        }
    }
}






