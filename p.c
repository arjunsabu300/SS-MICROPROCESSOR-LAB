#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void main()
{
    char label[10], opcode[10], operand[10], symbol[10], code[10], mnemonic[3];
    int locctr, start, length, sym_count = 0;
    int error = 0, op_found;
    char error_desc[100];
    char sym_tab[50][50];
    FILE *fp1, *fp2, *fp3, *fp4;

    fp1 = fopen("input.txt", "r");
    fp2 = fopen("output.txt", "w");
    fp3 = fopen("optab.txt", "r");
    fp4 = fopen("symtab.txt", "w");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL) {
        printf("Error opening file(s).\n");
        exit(1);
    }

    fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        sscanf(operand, "%X", &start);
        locctr = start;
        fprintf(fp2, "\t%s\t%s\t%X\n", label, opcode, locctr);
        fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    } else {
        locctr = 0;
    }

    while (strcmp(opcode, "END") != 0 && error == 0) {
        if (strcmp(label, "#") != 0) {
            fprintf(fp2, "%4X\t", locctr);
            if (strcmp(label, "*") != 0) {
                for (int i = 0; i < sym_count; i++) {
                    if (strcmp(sym_tab[i], label) == 0) {
                        error = 1;
                        snprintf(error_desc, sizeof(error_desc), "ERROR: Duplicate Symbol Found: %s", label);
                        break;
                    }
                }
                if (error) {
                    continue;
                }
                strcpy(sym_tab[sym_count], label);
                sym_count++;
                fprintf(fp4, "%s\t%X\n", label, locctr);
            }
            fscanf(fp3, "%s\t%s", code, mnemonic);
            if (strcmp(opcode, "WORD") == 0) {
                locctr += 3;
            } else if (strcmp(opcode, "RESW") == 0) {
                locctr += (3 * (atoi(operand)));
            } else if (strcmp(opcode, "RESB") == 0) {
                locctr += (atoi(operand));
            } else if (strcmp(opcode, "BYTE") == 0) {
                if (operand[0] == 'C')
                    locctr += strlen(operand) - 3;
                else if (operand[0] == 'X')
                    locctr += (strlen(operand) - 3) / 2;
            } else {
                op_found = 0;
                while (fscanf(fp3, "%s\t%s", code, mnemonic) != EOF) {
                    if (strcmp(opcode, code) == 0) {
                        op_found = 1;
                        locctr += 3;
                        break;
                    }
                }
                rewind(fp3);
                if (op_found == 0) {
                    error = 1;
                    snprintf(error_desc, sizeof(error_desc), "ERROR Opcode cannot be found: %s", opcode);
                    break;
                }
            }
            fprintf(fp2, "%s\t%s\t%s\n", label, opcode, operand);
            fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
        } else {
            fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
        }
    }

    if (error) {
        printf("%s\n", error_desc);
    } else {
        fprintf(fp2, "\t\t%s\n", opcode);
        length = locctr - start;
        printf("The length of the code: %d\n", length);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);

    char *outname = "output.txt";
    FILE *outf = fopen(outname, "r");
    fclose(outf);
    printf("\n");
}
