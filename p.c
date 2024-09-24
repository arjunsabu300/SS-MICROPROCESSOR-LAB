#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 50
#define MAX_LENGTH 100

typedef struct {
    char symbol[MAX_LENGTH];
    int address;
} Symbol;

Symbol sym_tab[MAX_SYMBOLS];
int sym_count = 0;

// Function to find the address of a symbol
int find_symbol_address(const char* symbol) {
    for (int i = 0; i < sym_count; i++) {
        if (strcmp(sym_tab[i].symbol, symbol) == 0) {
            return sym_tab[i].address;
        }
    }
    return -1; // Symbol not found
}

int main() {
    char label[MAX_LENGTH], opcode[MAX_LENGTH], operand[MAX_LENGTH], code[MAX_LENGTH];
    char mnemonic[3];
    int locctr, start_address, length;
    FILE *fp1, *fp2, *fp3, *fp4;

    // Open files
    fp1 = fopen("output.txt", "r");      // Intermediate file from Pass 1
    fp2 = fopen("object_code.txt", "w"); // Output file for object code
    fp3 = fopen("symtab.txt", "r");      // Symbol table from Pass 1

    // Read symbol table from Pass 1
    while (fscanf(fp3, "%s\t%d\n", sym_tab[sym_count].symbol, &sym_tab[sym_count].address) != EOF) {
        sym_count++;
    }
    fclose(fp3);

    // Read the first line of the intermediate file to get the start address
    fscanf(fp1, "%s\t%s\t%s\t", label, opcode, operand);
    sscanf(operand, "%X", &start_address);
    locctr = start_address;

    // Write the header record
    fprintf(fp2, "H^%s^%06X^%06X\n", label, start_address, length);

    // Process each line in the intermediate file
    while (fscanf(fp1, "%X\t%s\t%s\t", &locctr, opcode, operand) != EOF) {
        if (strcmp(opcode, "END") == 0) {
            break; // End of the program
        }

        // Check for machine code generation
        if (strcmp(opcode, "WORD") == 0) {
            fprintf(fp2, "T^%06X^03^%s\n", locctr, operand);
        } else if (strcmp(opcode, "RESW") == 0) {
            // No code generated for RESW
            continue;
        } else if (strcmp(opcode, "RESB") == 0) {
            // No code generated for RESB
            continue;
        } else if (strcmp(opcode, "BYTE") == 0) {
            // Handle BYTE directives (C' or X')
            if (operand[0] == 'C') { // Character constant
                fprintf(fp2, "T^%06X^%d^", locctr, strlen(operand) - 3);
                for (int i = 2; i < strlen(operand) - 1; i++) {
                    fprintf(fp2, "%02X", operand[i]);
                }
                fprintf(fp2, "\n");
            } else if (operand[0] == 'X') { // Hexadecimal constant
                fprintf(fp2, "T^%06X^%d^%s\n", locctr, (strlen(operand) - 3) / 2, operand + 2);
            }
        } else {
            // Assume opcode is valid and get the address
            int addr = find_symbol_address(operand);
            if (addr != -1) {
                fprintf(fp2, "T^%06X^03^%02X\n", locctr, addr);
            } else {
                printf("ERROR: Undefined symbol %s\n", operand);
            }
        }
    }

    // Write the end record
    fprintf(fp2, "E^%06X\n", start_address);
    
    // Cleanup
    fclose(fp1);
    fclose(fp2);

    printf("Object code generated in object_code.txt\n");
    return 0;
}
