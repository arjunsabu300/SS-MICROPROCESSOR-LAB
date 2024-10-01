#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_LABEL_LENGTH 10
#define MAX_OPCODE_LENGTH 10
#define MAX_SYMBOLS 100
#define MAX_OPCODES 100

typedef struct {
    char label[MAX_LABEL_LENGTH];
    int address;
} Symbol;

typedef struct {
    char opcode[MAX_OPCODE_LENGTH];
    char machineCode[3]; // Assuming machine codes are 2-digit hex values
} Opcode;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

Opcode opcodeTable[MAX_OPCODES];
int opcodeCount = 0;

void loadOpcodeTable(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening opcode table file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char opcode[MAX_OPCODE_LENGTH];
        char machineCode[3];
        sscanf(line, "%s %s", opcode, machineCode);
        strcpy(opcodeTable[opcodeCount].opcode, opcode);
        strcpy(opcodeTable[opcodeCount].machineCode, machineCode);
        opcodeCount++;
    }

    fclose(file);
}

void generateObjectCode(char *inputFilename, char *outputFilename) {
    FILE *inputFile = fopen(inputFilename, "r");
    FILE *outputFile = fopen(outputFilename, "w");
    if (inputFile == NULL || outputFile == NULL) {
        perror("Error opening input or output file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int locationCounter = 0;

    while (fgets(line, sizeof(line), inputFile)) {
        char label[MAX_LABEL_LENGTH] = "", opcode[MAX_OPCODE_LENGTH] = "", operand[MAX_LABEL_LENGTH] = "";
        int fields = sscanf(line, "%s %s %s", label, opcode, operand);

        if (fields == 3) {
            strcpy(symbolTable[symbolCount].label, label);
            symbolTable[symbolCount].address = locationCounter;
            symbolCount++;
        } else if (fields == 2) {
            strcpy(opcode, label);
            strcpy(operand, opcode);
        } else if (fields == 1) {
            strcpy(opcode, label);
        } else {
            continue;
        }

        for (int i = 0; i < opcodeCount; i++) {
            if (strcmp(opcodeTable[i].opcode, opcode) == 0) {
                fprintf(outputFile, "T%04X%02X\n", locationCounter, atoi(opcodeTable[i].machineCode));
                locationCounter += 3;
                break;
            }
        }
    }

    fprintf(outputFile, "E%04X\n", locationCounter);

    fclose(inputFile);
    fclose(outputFile);
}

int main() {
    loadOpcodeTable("opcode_table.txt");
    generateObjectCode("input.txt", "object_code.txt");
    return 0;
}