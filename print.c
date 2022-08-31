#include "function.h"
#include "project.h"
#pragma warning(disable : 4996)
void writef(char name[])
{
    FILE* fdob=NULL, * fdex=NULL, * fden=NULL;
    int line = MEMORY_ADRRES;
    int i;
    int j;
    char nameob[MAX_FILE_NAME];
    char nameen[MAX_FILE_NAME];
    char nameex[MAX_FILE_NAME];
    strcpy(nameob, name);
    strcat(nameob, ".ob");
    strcpy(nameex, name);
    strcat(nameex, ".ext");
    strcpy(nameen, name);
    strcat(nameen, ".ent");
    fdob = fopen(nameob, "w");
    if (externSymbol != 0)
        fdex = fopen(nameex, "w");
    if (entrySymbol != 0)
        fden = fopen(nameen, "w");
    char x;

    for (int i = 0; i < numOfInstruction; i++) {
        dw word;
        bitToPrint num;
        num.num = 255 << 16;
        for (int i = 0; i < 4; i++) {
            x = num.num & instruction_table[i].print.num;
            fprintf("%X", x);
            num.num = num.num >> 8;
        }
    }

    //for (i = 0; i < externSymbol; i++) /* print the file .ext */
    //{
    //    fprintf(fdex, "\n%s ", external_symbol[i].labelName);
    //    for (j = 1; j < 10000; j *= 10)
    //        if ((external_symbol[i].address / j) == 0)
    //            fprintf(fdex, "%d", 0);
    //    fprintf(fdex, "%d", external_symbol[i].address);
    //}
    //for (i = 0; i < numOfLabels; i++) /* print the file .ent */
    //{
    //    if (symbol_table[i].entry == TRUE)
    //    {
    //        fprintf(fden, "\n%s ", symbol_table[i].symbol);
    //        for (j = 1; j < 10000000; j *= 10)
    //            if ((symbol_table[i].value / j) == 0)
    //                fprintf(fden, "%d", 0);
    //        fprintf(fden, "%d", symbol_table[i].value);
    //    }
    //}

}
    