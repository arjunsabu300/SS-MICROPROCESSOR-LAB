#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char label[10];
    int address;
} Symbol;

typedef struct {
    char opcode[10];
    char mnemonic[10];
} OpcodeTable;

Symbol symbolTable[10];
int symbolCount = 0;

OpcodeTable opcodeTable[10];
int opcodeCount = 0;

int searchSymbol(char label[]) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].label, label) == 0) {
            return symbolTable[i].address;
        }
    }
    return -1;
}

void addSymbol(char label[], int address) {
    strcpy(symbolTable[symbolCount].label, label);
    symbolTable[symbolCount].address = address;
    symbolCount++;
}

void loadOpcodeTable(FILE *opcodeFile) {
    char line[20];
    while (fgets(line, sizeof(line), opcodeFile) != NULL) {
        sscanf(line, "%s %s", opcodeTable[opcodeCount].opcode, opcodeTable[opcodeCount].mnemonic);
        opcodeCount++;
    }
}

char* searchOpcode(char opcode[]) {
    for (int i = 0; i < opcodeCount; i++) {
        if (strcmp(opcodeTable[i].opcode, opcode) == 0) {
            return opcodeTable[i].mnemonic;
        }
    }
    return NULL;
}

void onePassAssembler(FILE *inputFile) {
    int locctr = 0x1000;  // Starting address
    char line[100];
    char label[10], opcode[10], operand[10];

    printf("Object Program:\n");

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        sscanf(line, "%s %s %s", label, opcode, operand);
        
        // Check if label exists
        if (label[0] != '.') { // Ignore comments
            if (strcmp(label, "PGM1") != 0) {
                // Add to symbol table if label is present
                if (strcmp(opcode, "START") != 0) {
                    addSymbol(label, locctr);
                }
            }

            // Search for opcode in opcode table
            char *opcodeHex = searchOpcode(opcode);
            if (opcodeHex != NULL) {
                printf("T%04X%s", locctr, opcodeHex);
                int operandAddress = searchSymbol(operand);
                if (operandAddress != -1) {
                    printf("%04X\n", operandAddress);
                } else {
                    printf("0000\n");
                }
                locctr += 3;  // Assume each instruction takes 3 bytes
            } else if (strcmp(opcode, "WORD") == 0) {
                printf("T%04X000000%04X\n", locctr, atoi(operand));
                locctr += 3;
            } else if (strcmp(opcode, "RESW") == 0) {
                locctr += 3 * atoi(operand);
            } else if (strcmp(opcode, "END") == 0) {
                printf("E%04X\n", locctr);
            }
        }
    }
}

int main() {
    FILE *inputFile, *opcodeFile;

    // Open the opcode file and load it into memory
    opcodeFile = fopen("opcodes.txt", "r");
    if (opcodeFile == NULL) {
        printf("Error opening opcode file!\n");
        return 1;
    }
    loadOpcodeTable(opcodeFile);
    fclose(opcodeFile);  // Close the opcode file

    // Open the input assembly file
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening input file!\n");
        return 1;
    }

    onePassAssembler(inputFile);
    fclose(inputFile);  // Close the input file

    return 0;
}
