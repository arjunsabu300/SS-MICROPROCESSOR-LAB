#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_LABEL_LENGTH 10
#define MAX_OPCODE_LENGTH 10
#define MAX_MACHINE_CODE_LENGTH 3

typedef struct Symbol {
    char label[MAX_LABEL_LENGTH];
    int address;
    struct Symbol* next;
} Symbol;

typedef struct Opcode {
    char opcode[MAX_OPCODE_LENGTH];
    char machineCode[MAX_MACHINE_CODE_LENGTH];
    struct Opcode* next;
} Opcode;

typedef struct TextRecord {
    int address;
    char machineCode[MAX_MACHINE_CODE_LENGTH];
    int operandAddress;
    struct TextRecord* next;
} TextRecord;

Symbol* symbolTable = NULL;
Opcode* opcodeTable = NULL;
TextRecord* textRecords = NULL;

void addSymbol(char* label, int address) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    strcpy(newSymbol->label, label);
    newSymbol->address = address;
    newSymbol->next = symbolTable;
    symbolTable = newSymbol;
}

int getSymbolAddress(char* label) {
    Symbol* current = symbolTable;
    while (current != NULL) {
        if (strcmp(current->label, label) == 0) {
            return current->address;
        }
        current = current->next;
    }
    return -1;
}

void addOpcode(char* opcode, char* machineCode) {
    Opcode* newOpcode = (Opcode*)malloc(sizeof(Opcode));
    strcpy(newOpcode->opcode, opcode);
    strcpy(newOpcode->machineCode, machineCode);
    newOpcode->next = opcodeTable;
    opcodeTable = newOpcode;
}

char* getMachineCode(char* opcode) {
    Opcode* current = opcodeTable;
    while (current != NULL) {
        if (strcmp(current->opcode, opcode) == 0) {
            return current->machineCode;
        }
        current = current->next;
    }
    return NULL;
}

void addTextRecord(int address, char* machineCode, int operandAddress) {
    TextRecord* newTextRecord = (TextRecord*)malloc(sizeof(TextRecord));
    newTextRecord->address = address;
    strcpy(newTextRecord->machineCode, machineCode);
    newTextRecord->operandAddress = operandAddress;
    newTextRecord->next = textRecords;
    textRecords = newTextRecord;
}

void loadOpcodeTable(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening opcode table file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char opcode[MAX_OPCODE_LENGTH];
        char machineCode[MAX_MACHINE_CODE_LENGTH];
        sscanf(line, "%s %s", opcode, machineCode);
        addOpcode(opcode, machineCode);
    }

    fclose(file);
}

void generateObjectCode(char* inputFilename, char* outputFilename) {
    FILE* inputFile = fopen(inputFilename, "r");
    FILE* outputFile = fopen(outputFilename, "w");
    if (inputFile == NULL || outputFile == NULL) {
        perror("Error opening input or output file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int locationCounter = 0;
    int startAddress = 0;
    char programName[MAX_LABEL_LENGTH] = "";
    char textRecords[MAX_LINE_LENGTH][MAX_MACHINE_CODE_LENGTH];
    int textRecordCount = 0;

    while (fgets(line, sizeof(line), inputFile)) {
        char label[MAX_LABEL_LENGTH] = "", opcode[MAX_OPCODE_LENGTH] = "", operand[MAX_LABEL_LENGTH] = "";
        int fields = sscanf(line, "%s %s %s", label, opcode, operand);

        if (fields == 3 && strcmp(label, "*") != 0) {
            addSymbol(label, locationCounter);
        }

        if (fields >= 2) {
            if (strcmp(opcode, "START") == 0) {
                startAddress = strtol(operand, NULL, 16);
                locationCounter = startAddress;
                strcpy(programName, label);
                fprintf(outputFile, "H%s  %06X%06X\n", programName, startAddress, 0); // Length to be filled later
            } else if (strcmp(opcode, "END") == 0) {
                break;
            } else {
                char* machineCode = getMachineCode(opcode);
                int address = (operand[0] != '*') ? getSymbolAddress(operand) : 0;
                addTextRecord(locationCounter, machineCode, address);
                textRecordCount++;
                locationCounter += 3;
            }
        }

        if (fields == 1 && strcmp(label, "END") == 0) {
            break;
        }
    }

    int programLength = locationCounter - startAddress;
    fseek(outputFile, 0, SEEK_SET);
    fprintf(outputFile, "H%s  %06X%06X\n", programName, startAddress, programLength);

    TextRecord* current = textRecords;
    while (current != NULL) {
        fprintf(outputFile, "T%06X%s%04X\n", current->address, current->machineCode, current->operandAddress);
        current = current->next;
    }

    fprintf(outputFile, "E%06X\n", startAddress);

    fclose(inputFile);
    fclose(outputFile);
}

int main() {
    loadOpcodeTable("opcode_table.txt");
    generateObjectCode("input.txt", "object_code.txt");
    return 0;
}